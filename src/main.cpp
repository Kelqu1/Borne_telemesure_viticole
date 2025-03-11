#include <Arduino.h>

#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4095.0 //nombre d'état numérique possible pour un converstisseur 10bits
#define ratio_pont_diviseur 4.3f

void setup() {
    Serial.begin(9600);  // Initialisation de la comunication série 
}
void loop() {
    //caluls des tension
    int valeur_brute=  analogRead(34); // Lecture de la valeur analogique
    float Tension_GPIO = (valeur_brute / nb_etat_max) * tension_ref; 
    float Tension_Batterie=Tension_GPIO* ratio_pont_diviseur; 

    // Affichage des valeurs dans le moniteur série
    Serial.print("tension à l'entrée de l'ESP32: ");
    Serial.print(Tension_GPIO);  
    Serial.println(" V");

    Serial.print("tension de la batterie: ");
    Serial.print(Tension_Batterie); 
    Serial.println(" V");

    Serial.println("-------------------------------------");
    delay(400);
}
