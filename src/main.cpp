//fin de la phase de prototypage 
//et debut de la phase de developpement

#include <Arduino.h>
#define tension_ref 3.3f //tension de reférence pour l'ESP32
#define nb_etat_max 4096.0 //nombre d'état numérique possible pour un converstisseur 12 bits
#define ratio_pont_diviseur 4.343f

void setup() {
    Serial.begin(9600);  // Initialisation de la comunication série 
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
    delay(400);
}
