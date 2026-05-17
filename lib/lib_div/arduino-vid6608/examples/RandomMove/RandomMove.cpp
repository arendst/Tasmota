#include <Arduino.h>
#include <vid6608.h>

// standard X25.168 range 315 degrees at 1/3 degree steps
#define STEPS (320 * 12)

#define PIN_STEP 26 // Pin, connected to f(scx)
#define PIN_DIR 27  // Pin. connected to CW/CCW

// Custom Acceleration table for the X27.168
// Proposed non-linear curve by ChatGPT
static vid6608::AccelTable accelTable[] = {
    {30, 3000},
    {65, 2920},
    {100, 2780},
    {135, 2600},
    {170, 2380},
    {205, 2140},
    {240, 1890},
    {275, 1650},
    {310, 1420},
    {345, 1210},
    {380, 1020},
    {415, 860},
    {450, 730},
    {485, 620},
    {520, 530},
    {555, 460},
    {590, 410},
    {625, 370},
    {660, 340},
    {695, 320},
    {730, 310},
    {765, 305},
    {800, 300},
};

vid6608 motor1(PIN_STEP, PIN_DIR, STEPS);

unsigned long nextMoveTime = 0;
uint16_t nextMovePos = 0;

void setup(void)
{
  // Initalize debug here
  Serial.begin(9600);
  Serial.println("Setup");
  // Set custom curve
  motor1.setAccelTable(accelTable);
  // run the motor against the stops
  motor1.zero();
  // We are done
  Serial.println("Setup done");
}

void loop(void)
{
  // the motor only moves when you call update
  motor1.loop();

  // Wait for motor to finish
  if (motor1.isStopped()) {
    // Plan next move here, to have real pause between moves
    if (nextMoveTime == 0) {
      nextMoveTime = millis() + random(500, 2000);
    } else {
      // Wait for next move time
      if (millis() > nextMoveTime) {
        // Plan next move here
        nextMoveTime = 0;
        nextMovePos = random(0, STEPS);
        Serial.print("Moving to ");
        Serial.println(nextMovePos);
        motor1.moveTo(nextMovePos);
      }
    }
  }
}
