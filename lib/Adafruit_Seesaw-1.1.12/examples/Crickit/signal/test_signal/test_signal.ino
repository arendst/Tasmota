#include "Adafruit_Crickit.h"

Adafruit_Crickit crickit;

#define BUTTON_1 CRICKIT_SIGNAL1
#define BUTTON_2 CRICKIT_SIGNAL2
#define LED_1 CRICKIT_SIGNAL3
#define LED_2 CRICKIT_SIGNAL4

void setup() {
  Serial.begin(9600);
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");

  //Two buttons are pullups, connect to ground to activate
  crickit.pinMode(BUTTON_1, INPUT_PULLUP);
  crickit.pinMode(BUTTON_2, INPUT_PULLUP);

  // Two LEDs are outputs, on by default
  crickit.pinMode(LED_1, OUTPUT);
  crickit.pinMode(LED_2, OUTPUT);
  crickit.digitalWrite(LED_1, HIGH);
  crickit.digitalWrite(LED_2, HIGH);
}

void loop() {
  if(!crickit.digitalRead(BUTTON_1))
    crickit.digitalWrite(LED_1, HIGH);
  else
    crickit.digitalWrite(LED_1, LOW);

  if(!crickit.digitalRead(BUTTON_2))
    crickit.digitalWrite(LED_2, HIGH);
  else
    crickit.digitalWrite(LED_2, LOW);
}
