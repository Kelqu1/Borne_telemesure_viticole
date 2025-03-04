#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
//debut code de yanis
const char *ssid = "BORNE_WIFI";
const char *password = "12345678";

void setup() {
   
    Serial.begin(9600);
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès WiFi activé !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.softAPIP());
    //fin code de yanis
  
    //requete pour tester si L'API est on
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        String response = "{\"status\": \"ok\", \"ip\": \"" + WiFi.softAPIP().toString() + "\"}";
        request->send(200, "application/json", response); 
    });
     
    //requete pour avoir la température
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        float temperature = 25.60; 
        String response = "{\"temperature\": " + String(temperature, 2) + "}";
        request->send(200, "application/json", response);
    });

    server.begin();
    Serial.println("Serveur Web démarré !");
}