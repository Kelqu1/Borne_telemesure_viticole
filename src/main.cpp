#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// CAPTEUR HUMI+TEMP parametres
#define DHTPIN 19        // Broche où est branché le DHT22
#define DHTTYPE DHT22   // Type de capteur

//NIVEAU TENSION
#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4096.0f //nombre d'état numérique possible pour un converstisseur 12 bits
#define tensionPin 34 // Broche où est conecté le pont diviseur

//déclaration des variables météo
float temperature;
float humidite;
float quantite_pluie;
//pourcentage batterie à ajouter

int compteurPluie=0;

//parametres
DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);//serveur HTTP au port 80

//INTERUPTION CAPTEUR PLUVOIMETRIE
int lastMillis = 0;
unsigned long previousMillis = 0;

void IRAM_ATTR handleInterrupt() {  //s'execute dés que l'état de l'I.L.S passe de 0 à 1

    if(millis() - lastMillis > 250){ // Software debouncing buton
        compteurPluie++;
    lastMillis = millis();
    }
}

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
    //debut code hugo

    //CAPTEUR PLUVIOMETRIE

    // définir l'interruption 
    attachInterrupt(digitalPinToInterrupt(21), handleInterrupt, RISING);

    //PARTIE API 

    //requete pour tester si L'API est activé
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        String response = "{\"status\": \"ok\", \"ip\": \"" + WiFi.softAPIP().toString() + "\"}";
        request->send(200, "application/json", response); 
    });
         
    //requete pour avoir la température
    server.on("/Mesures", HTTP_GET, [](AsyncWebServerRequest *request){
        String response = "{\"temperature\": " + String(temperature, 2) + ", \"humidite\": " + String(humidite, 2) + ", \"pluviometrie\": " + String(quantite_pluie, 2) + "\"}";
        request->send(200, "application/json", response);
    });
    
    //démarage du serveur HTTP
    server.begin();
    
    //affichage des informations de l'API
    Serial.println("status de l'api : /status");
    Serial.println("temperature     : /temperature");
    Serial.println("Serveur Web au port 200");

    //fin du code sur l'API

    dht.begin();
    Serial.println("initisalisation terminé");
    Serial.println("-----------------------------");
}

void loop() {
    //mise à jour 
    lastMillis = millis();

    //NIVEAU BATTERIE
    
    int valeur_brute=  analogRead(tensionPin); // Lecture de la valeur analogique
    float Tension_GPIO = (valeur_brute / nb_etat_max) * tension_ref +0.3 ;// cacul tension avec une  

    //PLUVIOMETRIE

    quantite_pluie = compteurPluie*0.25;
     
    //CAPTEUR HUMIDITé ET TEMPERATURE

    // Lecture de la température en °C
    temperature = dht.readTemperature(); // Lecture de la température en °C
    humidite = dht.readHumidity(); // Lecture de l'humidité en %

    //affichage d'une erreur pour le DHT au besoin 
    if (isnan(temperature) || isnan(humidite)) {
        Serial.println("Échec de lecture du DHT22 !");
        return;
    }

    // Affichage des valeurs dans le moniteur série
    Serial.print("Tension entrée ESP32: ");
    Serial.print(Tension_GPIO);
    Serial.println(" V");

    Serial.print("Température: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidité: ");
    Serial.print(humidite);
    Serial.println(" %");

    Serial.print("compteur pluie: ");
    Serial.print(quantite_pluie);
    Serial.println(" mm");

    Serial.println("-----------------------------");
    delay(1000);
}