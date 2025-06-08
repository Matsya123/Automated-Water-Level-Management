#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Xiaomi 11i";
const char* password = "12345678";

// MQTT server details
const char* mqtt_server = "a1xnali8ha38jo-ats.iot.us-east-1.amazonaws.com";
const char* mqtt_topic = "WaterTank/Control";

// Secure MQTT client
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Amazon Root CA Certificate
const char* root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
const char* device_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUXfRBp+BBXYaJwXv8O6jYabaLLEwwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDUxMzEwMzYx
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAPTo8tCZaEs6Tp9wDsJy
m/YOgRjXEOBStdEsGEGTlES08Vv+EJoHOUcwErPGaV3RJquBmllVJgfwvK1k6Z9Z
UOmVlxprIDKHNKIs+HvIQuIq9mjN8rtXH8foTf86UNMIDn9zIXqdRASLB+88tDyt
HNpKf93sjBENPpj6Ah0csJ6AAHWLLejhAKGosk54z85skP9iSO9RqnX3e5fixkbY
T0U58RSVyb7D6uAF47z0S/B4f1HoBRpxK0hUW3PMCbHxHyItr2Z2V+LggxGLddFe
+5yRO7UiLAJ8Bwjk17Moth9Ci1HZ1H4m05SUst998xyiIaZ79KP23RZ+U/UVykZJ
7gECAwEAAaNgMF4wHwYDVR0jBBgwFoAUNnImmOyZEgonu8eStJZXxIWopHEwHQYD
VR0OBBYEFOv89l8Q2T6OCrnUOtSguXl70nPGMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAlASYcBY7sHX6yKHE5o99gtPPD
nPZTyIDKP/J09PvDirZj2guUMCaDaE5Tewung01MYWWAduTylf1+7A+hHghuUwi6
ZgXF6AEV28PeVHaiK5VxHI2ZypljX9Gb5qM1yGPW9jZhqQejtxT16ymIsRMO/t+R
G7xvUaXzeExvR7xBC3JoNVW+kEmWuhUgV9dXCv3sgf1azYGiROvLjm/cQAjtik5a
YyMBZvSnn80Qh+5b8Yiep3EBw7GzK7NCVcZK4R2nb7VHS7rlwDaqtxJImLFD7FXh
mwqpHMDiUcQ3JbINMPBqhzuQbgmBtp6svRo8o69ufIJow/158noHSBDKkqfN
-----END CERTIFICATE-----
)EOF";

// Device Private Key
const char* private_key = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA9Ojy0JloSzpOn3AOwnKb9g6BGNcQ4FK10SwYQZOURLTxW/4Q
mgc5RzASs8ZpXdEmq4GaWVUmB/C8rWTpn1lQ6ZWXGmsgMoc0oiz4e8hC4ir2aM3y
u1cfx+hN/zpQ0wgOf3Mhep1EBIsH7zy0PK0c2kp/3eyMEQ0+mPoCHRywnoAAdYst
6OEAoaiyTnjPzmyQ/2JI71Gqdfd7l+LGRthPRTnxFJXJvsPq4AXjvPRL8Hh/UegF
GnErSFRbc8wJsfEfIi2vZnZX4uCDEYt10V77nJE7tSIsAnwHCOTXsyi2H0KLUdnU
fibTlJSy333zHKIhpnv0o/bdFn5T9RXKRknuAQIDAQABAoIBAHxx5ffzC5i2Lv86
/HT3saIM6rJJjey+el6oLkccC0oCO2ZIKf1qUEOI/OfWmPGv9htNxrU/3AxK2kSt
MzziMIgQl/A3oOur4iXJ/hU/xCSSOhU1GPLyqVZ2BzP6WNlyjZNbWBaq1MYOM2Dw
EpGGUVPmaSTGLsH3XehzmSFb068lUx70s/YckhcIJUxrJTRPYfnbHIXerWKIWFu2
qkcMIZi1PvTE0Kd3CxG/yji86LzyqGtSDjSdeSc1PFFf9s5Oi590yvmg1AohhOKJ
kZls7NpvdZLkxMOALZ5YvkrShnfRqj6SSrU8sUfX/nR+VMUB6gQnQuEOFhxgyIqk
k7dNI5UCgYEA+qanU7780TfMjmrpCVBWOXnz/un3oDjiKnHXawB5mg0bT9xiKECO
y2d3iKWbi7J9joParIpqYrMhZ/8S+guLAUrFtxVkaOd8fkyvxV22FhgCIGB8OAZm
AbK72F4DpVAqci/zngIRGQXrSGqK5rsY7+k8l6QigxWPNCCnZ8oqrgcCgYEA+iLu
QQP6/BJxrI9TpaAJyzNCSIGx3gLUbZGVggWZmLciDaK4iTi3GS0ugEtf18JW/sAG
zMTLsRuWkZcFrEWsMDL15nKij2lurlaltPbhb4cnrAVxFsyT9qo0TvR2BxjYuOgd
wKetZ+2pSGm8LFu0HPmTmDC41EbaMSdwfvm6gbcCgYBQWbZQOWxwYVPY39remntZ
p4eZU0WHflS6SI5HBfr1cHgt7LcmsjNUb1G5Y2VurgRj+E7UevbJtyThYrqhn6uq
+V5SZ7AtlO+qyUibm4PwpzefS27mwt5WcwaWwOSoTP+eZVLSd1Ul0WjwaP8oIhhH
aFx9PCe4H+QtUL55nix6ZwKBgQCHyJ59PKPqYjjC66sEpbOrUtAQfBcJ5Y9bS0RV
qKSQehlKPaLfGqKKUDSGAGSuwxhmgecvEUVhhxBdSBYURl4KbB+AUYpdP2aNzQt3
CO2nlT676thRAAzyQcLTeXD4uUwsVBSSmHNRj3ntjThQ/BbyA6lTXF95HF0lgbtX
skjEUQKBgQDjOeAOU/ejDqPB5zxMYtZAkH6Jlh0hhICMmfJfyplkBCFKjjmDg1AW
b9yxKcTeRu8qbFENhpI4pmUb7f0BSrBAK0jxc4YozBcczBAURQAsyFZLx7J+jXiW
bQ8ar31yjOW1+0IZN/jtbxyYGSMq0IhigS94CCuxhLjmKyyIEte9zQ==
-----END RSA PRIVATE KEY-----
)EOF";

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

// Callback function to handle incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Raw Message: ");
  Serial.println(message); // Debug message

  // Send the received command to Arduino Uno via Serial2
  if (message == "half") {
    Serial2.println("DISPENSE,6000");
    Serial.println("Command sent: DISPENSE,6000");
  } else if (message == "full") {
    Serial2.println("DISPENSE,12000");
    Serial.println("Command sent: DISPENSE,12000");
  } else if (message.endsWith("ml")) {
    int customVolume = message.substring(0, message.length() - 2).toInt();
    int customDelay = (customVolume * 1000) / 28; // 28ml per second
    Serial2.print("DISPENSE,");
    Serial2.println(customDelay);
    Serial.println("Command sent: DISPENSE," + String(customDelay));
  }
}


// Function to reconnect to MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Convert the String to a C-style string (const char*)
    String clientId = "ESP32_Client_" + String(random(0xffff));
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  setup_wifi();
  
  // Configuring SSL/TLS certificates
  espClient.setCACert(root_ca);
  espClient.setCertificate(device_cert);
  espClient.setPrivateKey(private_key);
  
  client.setServer(mqtt_server, 8883); // Secure Port 8883
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
