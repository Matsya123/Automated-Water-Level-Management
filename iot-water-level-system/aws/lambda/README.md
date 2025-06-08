# AWS Lambda Function – WaterTankAssistantHandler

This Lambda function is part of an IoT-based **automatic water tank filling system**. It connects Amazon Lex (voice assistant), AWS IoT Core (device control), and Amazon SNS (WhatsApp notifications) to manage water tank refilling through natural language input.

---

## 🔁 Trigger Source

- **Amazon Lex Intent**: `RefillIntent` (or similar)
- Triggered when user speaks commands like:
  - “Start”
  - “Full”
  - “Half”
  - “50ml”

---

## ⚙️ What It Does

- **Detects “start”** command to notify user that tank is empty.
- **Parses user input** like “full”, “half”, or custom volume (e.g., “75ml”).
- **Publishes command to AWS IoT MQTT topic** to ESP32 (controls pump).
- **Delays and sends follow-up WhatsApp confirmation** using SNS.

---

## 🔐 Required Environment

- `MQTT_TOPIC`: Must be defined in environment variables (e.g., `'esp32/tank/fill'`)
- `SNS_ARN`: WhatsApp-enabled SNS topic (e.g., `arn:aws:sns:us-east-1:XXXXXXX:WaterLevelAlerts`)

---

## 💬 Sample Lex Dialog

**User:** “Start”  
**Bot:** “The tank is empty. Would you like to refill it by half, full, or your choice?”  
**User:** “Half”  
**Bot:** “Motor is on. Filling the tank… Your tank will now be filled by half successfully.”  
**WhatsApp Message:** `"Tank has successfully been filled by half."`

---

## 🧪 Sample MQTT Payloads

```json
"full"
"half"
"75ml"
