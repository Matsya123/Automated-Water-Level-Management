import json
import boto3
import logging
import time
import threading

logger = logging.getLogger()
logger.setLevel(logging.INFO)

mqtt_client = boto3.client('iot-data', region_name='us-east-1')  # Ensure region is correct
sns_client = boto3.client('sns', region_name='us-east-1')

SNS_ARN = 'arn:aws:sns:us-east-1:654654177043:WaterLevelAlerts'

# Function to send SNS notification
def send_sns_message(message):
    try:
        sns_client.publish(
            TopicArn=SNS_ARN,
            Message=message
        )
        logger.info(f"SNS message sent: {message}")
    except Exception as e:
        logger.error(f"Failed to send SNS message: {str(e)}")

# Function to delay SNS message by 12 seconds
def delayed_sns(command):
    time.sleep(12)
    send_sns_message(f"Tank has successfully been filled by {command}.")


def lambda_handler(event, context):
    logger.info("EVENT: " + json.dumps(event))

    if 'sessionState' not in event:
        logger.error("Missing 'sessionState' in event")
        return {"statusCode": 400, "body": "Invalid event format: 'sessionState' key is missing."}

    try:
        intent_name = event['sessionState']['intent']['name']
        user_input = event['inputTranscript'].strip().lower()

        if user_input == "start":
            send_sns_message("Tank is empty !! Please refill it")
            return respond_with_options(intent_name)

        if user_input == "full":
            command = "full"
        elif user_input == "half":
            command = "half"
        elif user_input.endswith("ml"):
            try:
                custom_amount = int(user_input.replace("ml", "").strip())
                command = f"{custom_amount}ml"
            except ValueError:
                return respond_with_message(intent_name, "Invalid custom amount. Please enter a number followed by 'ml'.")
        else:
            return respond_with_message(intent_name, "Invalid choice. Please enter 'full', 'half', or a custom amount like '50ml'.")

        mqtt_client.publish(topic=MQTT_TOPIC, qos=1, payload=command)
        logger.info(f"Command '{command}' sent to ESP32 via MQTT.")

        threading.Thread(target=delayed_sns, args=(command,)).start()

        return respond_with_message(intent_name, f"Motor is on. Filling the tank...\nYour Tank will now be filled by {command} successfully.")

    except Exception as e:
        logger.error("Error processing event: " + str(e))
        return {"statusCode": 500, "body": "Internal Server Error"}


def respond_with_message(intent_name, message):
    return {
        "sessionState": {
            "dialogAction": {"type": "Close"},
            "intent": {"name": intent_name, "state": "Fulfilled"}
        },
        "messages": [{"contentType": "PlainText", "content": message}]
    }


def respond_with_options(intent_name):
    return {
        "sessionState": {
            "dialogAction": {"type": "ElicitSlot", "slotToElicit": "FillChoice"},
            "intent": {"name": intent_name, "state": "InProgress"}
        },
        "messages": [{
            "contentType": "PlainText",
            "content": "The tank is empty. Would you like to refill it by half, full, or your choice?"
        }],
        "responseCard": {
            "version": 1,
            "contentType": "application/vnd.amazonaws.card.generic",
            "genericAttachments": [{
                "title": "Choose an option:",
                "buttons": [
                    {"text": "Half", "value": "half"},
                    {"text": "Full", "value": "full"},
                    {"text": "My Choice", "value": "my choice"}
                ]
            }]
        }
    }

