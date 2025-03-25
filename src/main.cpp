//fin de la phase de prototypage 
//et debut de la phase de developpement

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Définition du capteur
#define DHTPIN 19        // Broche où est connecté le DHT22
#define DHTTYPE DHT22   // Type de capteur
#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4096.0 //nombre d'état numérique possible pour un converstisseur 12 bits
#define ratio_pont_diviseur 4.343f

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);  // Initialisation de la communication série
    Serial.println("Initialisation du capteur DHT22...");
    dht.begin();
}

void loop() {
    //caluls des tension
    int valeur_brute=  analogRead(34); // Lecture de la valeur analogique
    float Tension_GPIO = (valeur_brute / nb_etat_max) * tension_ref; 
    float Tension_Batterie=Tension_GPIO* ratio_pont_diviseur; 

    // Affichage des valeurs dans le moniteur série
    Serial.print("Valeur brute: ");
    Serial.println( valeur_brute); 

    Serial.print("Tension à l'entrée de l'ESP32: ");
    Serial.print(Tension_GPIO);  
    Serial.println(" V");

    Serial.print("Tension de la batterie: ");
    Serial.print(Tension_Batterie); 
    Serial.println(" V");

    Serial.println("-------------------------------------");

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
