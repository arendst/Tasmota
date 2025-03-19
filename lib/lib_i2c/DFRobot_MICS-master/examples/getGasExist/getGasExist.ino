/*!
 * @file getGasExist.ino
 * @brief Reading Gas concentration, A concentration of one part per million (PPM).
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

#define CALIBRATION_TIME   3                      // Default calibration time is three minutes

// When using I2C communication, use the following program to construct an object by DFRobot_MICS_I2C
/**
 * select i2c device address 
 * MICS_ADDRESS_0               0x75
 * MICS_ADDRESS_1               0x76
 * MICS_ADDRESS_2               0x77
 * MICS_ADDRESS_3               0x78
 */
#define MICS_I2C_ADDRESS MICS_ADDRESS_0
DFRobot_MICS_I2C mics(&Wire, MICS_I2C_ADDRESS);

// When using the Breakout version, use the following program to construct an object from DFRobot_MICS_ADC
/**
 * adcPin is A0~A5
 * powerPin is General IO
 */
#define ADC_PIN   A0
#define POWER_PIN 10
//DFRobot_MICS_ADC mics(/*adcPin*/ADC_PIN, /*powerPin*/POWER_PIN);

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  while(!mics.begin()){
    Serial.println("NO Deivces !");
    delay(1000);
  } Serial.println("Device connected successfully !");

  /**
   * Gets the power mode of the sensor
   * The sensor is in sleep mode when power is on,so it needs to wake up the sensor. 
   * The data obtained in sleep mode is wrong
   */
  uint8_t mode = mics.getPowerState();
  if(mode == SLEEP_MODE){
    mics.wakeUpMode();
    Serial.println("wake up sensor success!");
  }else{
    Serial.println("The sensor is wake up mode");
  }

  /**
   * Do not touch the sensor probe when preheating the sensor.
   * Place the sensor in clean air.
   * The default calibration time is 3 minutes.
   */
  while(!mics.warmUpTime(CALIBRATION_TIME)){
    Serial.println("Please wait until the warm-up time is over!");
    delay(1000);
  }
}

void loop() 
{
  /**
   * Type of detection gas
   * MICS-4514 You can get all gas state
   * MICS-5524 You can get the state of CO, CH4, C2H5OH, C3H8, C4H10, H2, H2S, NH3
   * MICS-2714 You can get the state of NO2, H2 ,NO
   * CO       = 0x01  (Carbon Monoxide)
   * CH4      = 0x02  (Methane)
   * C2H5OH   = 0x03  (Ethanol)
   * C3H8     = 0x04  (Propane)
   * C4H10    = 0x05  (Iso Butane)
   * H2       = 0x06  (Hydrogen)
   * H2S      = 0x07  (Hydrothion)
   * NH3      = 0x08  (Ammonia)
   * NO       = 0x09  (Nitric Oxide)
   * NO2      = 0x0A  (Nitrogen Dioxide)
   */
  int8_t gasFlag = mics.getGasExist(C2H5OH);
  if(gasFlag == EXIST){
    Serial.println("The gas exists!");
  }else{
    Serial.println("The gas does not exist!");
  }
  delay(1000);
  //mics.sleepMode();
}