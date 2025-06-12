#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

//CAPTEUR HUMIDITé ET TEMPéRATURE
#define DHTPIN 19       // Broche où est branché le DHT22
#define DHTTYPE DHT22   // modèle du capteur

//NIVEAU TENSION
#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4096.0 //nombre d'état numérique possible pour un converstisseur 12 bits
#define tensionPin 34 // Broche où est conecté le pont diviseur

//déclaration des variables météorologiques
float temperature;
float humidite;
float quantite_pluie;
float pourcentage_batterie;
int compteurPluie=0;

//décalages température et humidité
float decalage_temp=1.0;
float decalage_humi=-10.3;

//parametres
DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);//serveur HTTP au port 80

//INTERUPTION CAPTEUR DE PLUVOIMETRIE
int lastMillis = 0;
unsigned long previousMillis = 0;

void IRAM_ATTR handleInterrupt() {  //s'execute dés que l'état de l'I.L.S passe de 0 à 1

    if(millis() - lastMillis > 250){ // code anti-rebond
        compteurPluie++; // ajoute 1 la variable
    lastMillis = millis();
    }
}

//code convertisseur de tension en pourcentage
float tensionversPourcentage(float tension) {
    const int N = 11;
    //tableau des référecences 
    float tensions[N] = {12.89, 12.78, 12.65, 12.51, 12.41, 12.23, 12.11, 11.96, 11.81, 11.70, 11.63};
    float pourcentages[N] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0};
  
    //cas Au-dessus ou en-dessous des bornes de départ
    if (tension >= tensions[0]) return 100;
    if (tension <= tensions[N - 1]) return 0;
  
    //Chercher dans quel intervalle se trouve la tension
    for (int i = 0; i < N - 1; i++) {
      float v1 = tensions[i];
      float v2 = tensions[i + 1];
  
      if (tension <= v1 && tension >= v2) {
        float p1 = pourcentages[i];
        float p2 = pourcentages[i + 1];
  
        //Linéarisation dans l’intervalle
        float pourcent = p1 + (p2 - p1) * (tension - v1) / (v2 - v1);
        return pourcent;
      }
    }
  
    return -1; //en cas d'erreur
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
    //debut code de hugo

    //CAPTEUR PLUVIOMETRIE

    //définir l'interruption 
    attachInterrupt(digitalPinToInterrupt(21), handleInterrupt, RISING);

    //PARTIE API 

    //requete pour tester si L'API est activé
    server.on("/status", HTTP_POST, [](AsyncWebServerRequest *request){
        String response = "{\"status\": \"ok\", \"ip\": \"" + WiFi.softAPIP().toString() + "\"}";
        request->send(200, "application/json", response); 
    });
    
    //requete pour avoir les mesures
    server.on("/Mesures", HTTP_POST, [](AsyncWebServerRequest *request){
        String response = "{\"temperature\": " + String(temperature, 2) + ", \"humidite\": " + String(humidite, 2) + ", \"pluviometrie\": " + String(quantite_pluie, 2) + ", \"latitude\": 47.730814, \"longitude\": 7.303298}";
        request->send(200, "application/json", response);
    });

    //envoi d'une erreur 404 si l'url n'est pas connue
    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "erreur dans l'Url de L'API");
    });
    
    //démarage du serveur HTTP
    server.begin();
    
    //affichage des informations de l'API
    Serial.println("le status de l'api : /status");
    Serial.println("les mesures : /Mesures");
    Serial.println("API est au port 80");

    //fin du code de l'API

    dht.begin(); 
    Serial.println("l'initisalisation est terminé");
    Serial.println("-----------------------------");
}

void loop() {
    //mise à jour de la variable du code anti rebond du capteur de pluviométrie
    lastMillis = millis();

    //NIVEAU BATTERIE
    
    int valeur_brute=  analogRead(tensionPin); // Lecture de la valeur analogique
    float Tension_GPIO = (valeur_brute / nb_etat_max) * tension_ref +0.8 ;// cacul tension avec un décalage
    float Tension_batterie=Tension_GPIO*3.443;
    pourcentage_batterie=tensionversPourcentage(Tension_batterie);

    //PLUVIOMETRIE

    quantite_pluie = compteurPluie*0.25;
     
    //CAPTEUR HUMIDITé ET TEMPERATURE
    
    // Lecture de la température en °C
    temperature = dht.readTemperature()+decalage_temp; // Lecture de la température en °C
    humidite = dht.readHumidity()+decalage_humi; // Lecture de l'humidité en %

    //affichage d'une erreur pour le DHT au besoin 
    if (isnan(temperature) || isnan(humidite)) {
        Serial.println("Échec de lecture du DHT22 !");
        return;
    }

    // Affichage des valeurs dans le moniteur série
    Serial.print("Tension entrée ESP32: ");
    Serial.print(Tension_GPIO);
    Serial.println(" V");

    Serial.print("Tension batterie: ");
    Serial.print(Tension_batterie);
    Serial.println(" V");

    Serial.print("Niveau de batterie: ");
    Serial.print(pourcentage_batterie);
    Serial.println(" %");

    Serial.print("Température: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidité: ");
    Serial.print(humidite);
    Serial.println(" %");

    Serial.print("pluviométrie: ");
    Serial.print(quantite_pluie);
    Serial.println(" mm");

    Serial.println("-----------------------------");
    delay(1000);
}