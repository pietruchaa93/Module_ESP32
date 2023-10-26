#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "ssid";
const char* password = "password";

IPAddress staticIP(192, 168, 1, 184); // Static IP address for ESP32
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const int outputPin = 2; // GPIO pin to control

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Set static IP address
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);

  // HTTP server routing configuration
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String response = "Microcontroller version: ";
    response += ESP.getSdkVersion();
    response += "<br>WiFi MAC address: ";
    response += WiFi.macAddress();
    response += "<br>Supply voltage: ";
    response += String(analogRead(34) / 4095.0 * 3.3, 2);
    response += "V<br>The operating time of the microcontroller from startup: ";
    response += String(millis() / 1000);
    response += "s<br>";
    request->send(200, "text/html", response);
  });

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request) {
    String state = request->arg("state");
    if (state == "on") {
      digitalWrite(outputPin, HIGH);
    } else if (state == "off") {
      digitalWrite(outputPin, LOW);
    }
    String response = "Output state on pin 2: ";
    response += (digitalRead(outputPin) ? "ON" : "OFF");
    request->send(200, "text/html", response);
  });

  server.begin();
}

void loop() {
}
