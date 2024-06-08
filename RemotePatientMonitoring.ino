#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid ="Lakshman";
const char* password = "00012000";
const char* botToken = "https://api.telegram.org/bot6982708025:AAE0ZKHI2AG69wXsch0__SnSHu43ke-RlUE";
const int temperaturePin = A0; // Analog pin connected to LM35
const int heartRatePin = 2; // Digital pin connected to heart rate sensor
const int temperatureThreshold = 37; // Example temperature threshold
const int heartRateThreshold = 80; // Example heart rate threshold

void setup() {
  Serial.begin(115200);
  pinMode(temperaturePin, INPUT);
  pinMode(heartRatePin, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void loop() {
  int temperature = analogRead(temperaturePin) * 0.488; // Convert ADC value to temperature in Celsius
  int heartRate = digitalRead(heartRatePin);
  
  if (temperature > temperatureThreshold || heartRate > heartRateThreshold) {
    sendTelegramMessage("Alert: Temperature or heart rate exceeded for the patient");
    delay(5000); // Delay to avoid sending multiple messages for the same event
  }

  delay(1000); // Adjust delay as needed
}

void sendTelegramMessage(String message) {
  WiFiClientSecure client;
  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Connection failed.");
    return;
  }
  
  String url = "/bot" + String(botToken) + "/sendMessage?chat_id=@-2108450700/2&text=" + message;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
}
