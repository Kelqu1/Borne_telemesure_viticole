#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// CAPTEUR HUMI+TEMP parametres
#define DHTPIN 19        // Broche où est branché le DHT22
#define DHTTYPE DHT22   // Type de capteur

// CAPTEUR NIVEAU TENSION
#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4096.0f //nombre d'état numérique possible pour un converstisseur 12 bits
#define tensionPin 34 // Broche où est conecté le pont diviseur

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);  // Initialisation de la communication série
    dht.begin();
    Serial.println("Initialisation de la borne de télémesure");
}

void loop() {
    //CAPTEUR NIVEAU BATTERIE
    int valeur_brute=  analogRead(tensionPin); // Lecture de la valeur analogique
    float Tension_GPIO = (valeur_brute / nb_etat_max) * tension_ref +0.3 ;// cacul tension avec une  
     
    //CAPTEUR HUMIDITé ET TEMPERATURE
    float temperature = dht.readTemperature(); // Lecture de la température en °C
    float humidite = dht.readHumidity(); // Lecture de l'humidité en %

    //affichage d'une erreur pour le DHT au besoin 
    if (isnan(temperature) || isnan(humidite)) {
        Serial.println("Échec de lecture du DHT22 !");
        return;
    }

    // Affichage des valeurs dans le moniteur série
    Serial.print("Tension entrée ESP32: ");
    Serial.print(Tension_GPIO); //ecriture tension
    Serial.println(" V");

    Serial.print("Température: ");
    Serial.print(temperature);//écriture de la température
    Serial.println(" °C");

    Serial.print("Humidité: ");
    Serial.print(humidite); //écriture de l'humidité
    Serial.println(" %");

    Serial.println("-----------------------------");
    delay(1000);  // Attente avant les prochaines lectures
}