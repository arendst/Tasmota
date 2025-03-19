/*!
 * @file enablePower.ino
 * @brief Enable the power, and the information is printed on the serial port.
 * @n the dialing switch A0, A1 (MICS_ADDRESS_0 is [0 0]), (MICS_ADDRESS_1 is [1 0])
 * @n (MICS_ADDRESS_2 is [0 1]), (MICS_ADDRESS_3 is [1 1]).
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author ZhixinLiu(zhixin.liu@dfrobot.com)
 * @version V1.2
 * @date 2021-06-18
 * @url https://github.com/DFRobot/DFRobot_MICS
 */
#include "DFRobot_MICS.h"

/**
 * select i2c device address 
 * MICS_ADDRESS_0               0x75
 * MICS_ADDRESS_1               0x76
 * MICS_ADDRESS_2               0x77
 * MICS_ADDRESS_3               0x78
 */
#define MICS_I2C_ADDRESS MICS_ADDRESS_0
DFRobot_MICS_I2C mics(&Wire, MICS_I2C_ADDRESS);

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  while(!mics.begin()){
    Serial.println("NO Deivces !");
    delay(1000);
  } Serial.println("Device connected successfully !");
}

void loop() 
{
  /**
   * Gets the power mode of the sensor
   * The sensor is in sleep mode when power is on,so it needs to wake up the sensor. 
   * The data obtained in sleep mode is wrong
   */
  if(SLEEP_MODE == mics.getPowerState()){
    mics.wakeUpMode();
    Serial.println("sleep mode,   wake up sensor success!");
  }else{
    mics.sleepMode();
    Serial.println("wake up mode, sleep sensor success!");
  }
  delay(3000);
}