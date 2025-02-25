#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Définition du capteur
#define DHTPIN 19        // Broche où est connecté le DHT22
#define DHTTYPE DHT22   // Type de capteur

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);  // Initialisation de la communication série
    Serial.println("Initialisation du capteur DHT22...");
    dht.begin();
}

void loop() {
    float temperature = dht.readTemperature(); // Lecture de la température en °C
    float humidite = dht.readHumidity(); // Lecture de l'humidité en %

    if (isnan(temperature) || isnan(humidite)) {
        Serial.println("Échec de lecture du DHT22 !");// affichage d'une erreur de lecture
        return;
    }

    Serial.print("Température: ");
    Serial.print(temperature);//écriture de la température
    Serial.println(" °C");

    Serial.print("Humidité: ");
    Serial.print(humidite); //écriture de l'humidité
    Serial.println(" %");

    Serial.println("--------------------");
    delay(2000);  // Attente avant la prochaine lecture
}
