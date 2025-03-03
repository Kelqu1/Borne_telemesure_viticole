#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h> 

const char *ssid = "BORNE_WIFI";
const char *password = "12345678";

WebServer server(80);

void handleTemperature() {
    float temperature = 25.6; // Simulation d'une valeur de température
    server.send(200, "application/json", "{\"temperature\": " + String(temperature) + "}");
}

void setup() {
    Serial.begin(9600);
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès WiFi activé !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.softAPIP());

    server.on("/temperature", handleTemperature);
    server.begin();
    Serial.println("Serveur HTTP démarré.");
}

void loop() {
    server.handleClient();
}