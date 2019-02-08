#include "Adafruit_Crickit.h"
#include "seesaw_motor.h"

Adafruit_Crickit crickit;

seesaw_Motor motor_a(&crickit);
seesaw_Motor motor_b(&crickit);

void setup() {
  Serial.begin(115200);
  Serial.println("Dual motor demo!");
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");

  //attach motor a
  motor_a.attach(CRICKIT_MOTOR_A1, CRICKIT_MOTOR_A2);

  //attach motor b
  motor_b.attach(CRICKIT_MOTOR_B1, CRICKIT_MOTOR_B2);
}

void loop() {
  motor_a.throttle(1);
  motor_b.throttle(-1);
  delay(1000);

  motor_a.throttle(.5);
  motor_b.throttle(-.5);
  delay(1000);

  motor_a.throttle(0);
  motor_b.throttle(0);
  delay(1000);

  motor_a.throttle(-.5);
  motor_b.throttle(.5);
  delay(1000);

  motor_a.throttle(-1);
  motor_b.throttle(1);
  delay(1000);

  motor_a.throttle(0);
  motor_b.throttle(0);
  delay(500);
}

