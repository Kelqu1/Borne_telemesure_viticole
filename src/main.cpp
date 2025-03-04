#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WebServer.h>

// Parameters
const int ilsPin = 21;

// Variables
bool ilsStatus = false;

void readReedSwitch() {
    // Test routine for ReedSwitch
    ilsStatus = digitalRead(ilsPin);
    Serial.println(ilsStatus);
    delay(100);
}

void setup() {
    // Init Serial USB
    Serial.begin(9600);
    Serial.println("Initialize System");
    
    // Init digital input
    pinMode(ilsPin, INPUT);
}

void loop() {
    readReedSwitch();
}
