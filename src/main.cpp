#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WebServer.h>

// Parametertes du montage
const int ilsPin = 21;

// Variables
int pluie= 0;

void IRAM_ATTR handleInterrupt() {  //s'execute dés que l'état de l'ILS passe de 0 à 1
    // ajoute 1 à la variable pluie
    pluie++;
}

void setup() {
    // mise en place de la liason série
    Serial.begin(9600);
    Serial.println("systeme initialisé");

    pinMode(ilsPin, INPUT);
    // définir l'interruption 
    attachInterrupt(digitalPinToInterrupt(ilsPin), handleInterrupt, RISING);
}

void loop()   
{
    //affichage de la variable
    Serial.print("niveau de la pluie : ");
    Serial.println(pluie);
    delay(10);
}
