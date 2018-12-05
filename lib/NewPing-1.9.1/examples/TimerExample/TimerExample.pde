// ---------------------------------------------------------------------------
// While the NewPing library's primary goal is to interface with ultrasonic sensors, interfacing with
// the Timer2 interrupt was a result of creating an interrupt-based ping method. Since these Timer2
// interrupt methods were built, the library may as well provide the functionality to use these methods
// in your sketches.  This shows how simple it is (no ultrasonic sensor required).  Keep in mind that
// these methods use Timer2, as does NewPing's ping_timer method for using ultrasonic sensors.  You
// can't use ping_timer at the same time you're using timer_ms or timer_us as all use the same timer.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define LED_PIN 13 // Pin with LED attached.

void setup() {
  pinMode(LED_PIN, OUTPUT);
  NewPing::timer_ms(500, toggleLED); // Create a Timer2 interrupt that calls toggleLED in your sketch once every 500 milliseconds.
}

void loop() {
  // Do anything here, the Timer2 interrupt will take care of the flashing LED without your intervention.
}

void toggleLED() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle the LED.
}