/*!
 * @file DFRobot_MAX17043.ino
 * @brief connect gauge I2C interface with your board (please reference board compatibility)
 * @n Voltage, percentage will be printed via serial.
 * @n Use API to config alaram and sleep (please reference to the readme in lib)
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [ouki.wang](ouki.wang@dfrobot.com)
 * @version  V1.0
 * @date  2018-4-14
 * @url https://github.com/DFRobot/DFRobot_MAX17043
 */

#include "DFRobot_MAX17043.h"
#include "Wire.h"

#ifdef __AVR__
  #define ALR_PIN       2
#else
  #define ALR_PIN       D2
#endif

#define PRINT_INTERVAL        2000

DFRobot_MAX17043        gauge;
uint8_t       intFlag = 0;

void interruptCallBack()
{
  intFlag = 1;
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println();
  pinMode(ALR_PIN, INPUT_PULLUP);
  attachInterrupt(ALR_PIN, interruptCallBack, FALLING);  //default alert is 32%
  
  while(gauge.begin() != 0) {
    Serial.println("gauge begin faild!");
    delay(2000);
  }
  delay(2);
  Serial.println("gauge begin successful!");
  //gauge.setInterrupt(32);  //use this to modify alert threshold as 1% - 32% (integer)
}

void loop()
{
  static unsigned long lastMillis = 0;
  if((millis() - lastMillis) > PRINT_INTERVAL) {
    lastMillis = millis();
    Serial.println();

    Serial.print("voltage: ");
    Serial.print(gauge.readVoltage());
    Serial.println(" mV");

    Serial.print("precentage: ");
    Serial.print(gauge.readPercentage());
    Serial.println(" %");
  }

  if(intFlag == 1) {
    intFlag = 0;
    gauge.clearInterrupt();
    Serial.println("Low power alert interrupt!");
    //put your battery low power alert interrupt service routine here
  }
}
