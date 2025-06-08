
# IoT-Based Automatic Water Tank Refilling System using AWS & WhatsApp

This project monitors water levels in a household tank using an ultrasonic sensor and ESP32. When the water level drops below a threshold, a message is sent via AWS SNS integrated to the user's WhatsApp . The user can reply to refill the tank automatically.

---

## 🛠 Components
- ESP32 microcontroller
- Arduino Uno
- 5V DC Submersible pump
- Ultrasonic Sensor (HC-SR04)
- Relay-controlled water pump
- 5V DC power supply

---

## 🧠 AWS Services Used
- **AWS IoT Core**: Device connectivity
- **AWS Lambda**: Executes logic for alerts and refill commands
- **AWS SNS (WhatsApp API)**: Sends WhatsApp messages


---

## 🔁 Workflow
1. ESP32 measures tank water level
2. Sends data to AWS IoT via MQTT
3. Lambda function triggers WhatsApp alert via SNS when tank is empty
4. User replies with a command (e.g., “Fill”) → Lambda activates pump
5. ESP32 receives MQTT signal and send the command to the Arduino UNO
6. Arduino UNO then sends the command to the relay which controls the pump
7. Also reads the data from ultrasonic sensor and send to ESP32 which then sends to AWS via MQTT

---

## 📱 WhatsApp Control
- Message: “Your tank is empty. Reply 'Fill' to start refilling.”
- User replies “Fill” → system starts filling the desired quantity the user enters


---

## 🚀 Future Scope
- Integrate ML for usage prediction
- Add water quality sensors
- Enable voice control via Lex

---

