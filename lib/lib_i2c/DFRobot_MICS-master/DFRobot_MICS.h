/*!
 * @file DFRobot_MICS.h
 * @brief Define the basic structure of class DFRobot_MicsSensor
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.1
 * @date 2020-4-20
 * @url https://github.com/DFRobot/DFRobot_MICS
 */
#ifndef __DFROBOT_MICS_H__
#define __DFROBOT_MICS_H__
#include <Arduino.h>
#include <Wire.h>

#define ENABLE_DBG                ///< Open this macro to see the program running in detail

#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

#define           MICS_ADDRESS_0            0x75
#define           MICS_ADDRESS_1            0x76
#define           MICS_ADDRESS_2            0x77
#define           MICS_ADDRESS_3            0x78
#define           OX_REGISTER_HIGH          0x04
#define           OX_REGISTER_LOW           0x05
#define           RED_REGISTER_HIGH         0x06
#define           RED_REGISTER_LOW          0x07
#define           POWER_REGISTER_HIGH       0x08
#define           POWER_REGISTER_LOW        0x09
#define           POWER_MODE_REGISTER       0x0a
#define           SLEEP_MODE                0x00
#define           WAKE_UP_MODE              0x01
#define           OX_MODE                   0x00
#define           RED_MODE                  0x01
#define           ERROR                     -1
#define           EXIST                     0x00
#define           NO_EXIST                  0x02

#define           CO                        0x01          ///< Carbon Monoxide
#define           CH4                       0x02          ///< Methane
#define           C2H5OH                    0x03          ///< Ethanol
#define           C3H8                      0x04          ///< Propane
#define           C4H10                     0x05          ///< Iso Butane
#define           H2                        0x06          ///< Hydrogen
#define           H2S                       0x07          ///< Hydrothion
#define           NH3                       0x08          ///< Ammonia
#define           NO                        0x09          ///< Nitric Oxide
#define           NO2                       0x0A          ///< Nitrogen Dioxide


class DFRobot_MICS{
public:
  DFRobot_MICS();
  ~DFRobot_MICS();

  /**
   * @fn warmUpTime
   * @brief Waiting time for warm-up
   * @param minute Units of minutes
   * @return bool type
   * @retval true  is warm-up success
   * @retval false is wait warm-up
   */
  bool warmUpTime(uint8_t minute);

  /**
   * @fn getADCData
   * @brief Read sensor ADC data
   * @param mode oxmode redmode
   * @n     OX_MODE
   * @n     RED_MODE
   * @return adcValue (0-1024)
   */
  int16_t getADCData(uint8_t mode);

  /**
   * @fn getGasData
   * @brief Read the concentration of the gas
   * @param type gas type
   * @n   Methane          (CH4)    (1000 - 25000)PPM
   * @n   Ethanol          (C2H5OH) (10   - 500)PPM
   * @n   Hydrogen         (H2)     (1    - 1000)PPM
   * @n   Ammonia          (NH3)    (1    - 500)PPM
   * @n   Carbon Monoxide  (CO)     (1    - 1000)PPM
   * @n   Nitrogen Dioxide (NO2)    (0.1  - 10)PPM
   * @return concentration Units of PPM
   */
  float getGasData(uint8_t type);

   /**
   * @fn getGasExist
   * @brief Read whether the gas is present
   * @param gas type
   * @n   CO       = 0x01  (Carbon Monoxide)
   * @n   CH4      = 0x02  (Methane)
   * @n   C2H5OH   = 0x03  (Ethanol)
   * @n   C3H8     = 0x04  (Propane)
   * @n   C4H10    = 0x05  (Iso Butane)
   * @n   H2       = 0x06  (Hydrogen)
   * @n   H2S      = 0x07  (Hydrothion)
   * @n   NH3      = 0x08  (Ammonia)
   * @n   NO       = 0x09  (Nitric Oxide)
   * @n   NO2      = 0x0A  (Nitrogen Dioxide)
   * @return state
   * @retval NO_EXIST
   * @retval EXIST
   */
  int8_t getGasExist(uint8_t gas);

  /**
   * @fn sleepMode
   * @brief Sleep sensor
   */ 
  void sleepMode(void);

  /**
   * @fn wakeUpMode
   * @brief wakeup sensor
   */ 
  void wakeUpMode(void);

  /**
   * @fn getPowerState
   * @brief Gets the power mode of the sensor
   * @return state
   * @retval SLEEP_MODE
   * @retval WAKE_UP_MODE
   */ 
  uint8_t getPowerState(void);

private:
  int16_t  __r0_ox   = 0;
  int16_t  __r0_red  = 0;
  uint32_t __nowTime = 0;
  uint8_t  __flag    = 0;
  int16_t  getSensorData(uint16_t *oxData ,uint16_t *redData ,uint16_t *powerData);
  float    getCarbonMonoxide(float data);
  float    getMethane(float data);
  float    getEthanol(float data);
  float    getHydrogen(float data);
  float    getAmmonia(float data);
  float    getNitrogenDioxide(float data);
  bool     existPropane(float data);
  bool     existNitricOxide(float data);
  bool     existIsoButane(float data);
  bool     existHydrothion(float data);
  bool     existCarbonMonoxide(float data);
  bool     existMethane(float data);
  bool     existEthanol(float data);
  bool     existHydrogen(float data);
  bool     existAmmonia(float data);
  bool     existNitrogenDioxide(float data);
  virtual void writeData(uint8_t reg ,uint8_t *data ,uint8_t len) = 0;
  virtual int16_t readData(uint8_t reg ,uint8_t *data ,uint8_t len) = 0;
};

class DFRobot_MICS_I2C:public DFRobot_MICS{
public:
  DFRobot_MICS_I2C(TwoWire *pWire=&Wire ,uint8_t addr = MICS_ADDRESS_3);
  bool begin(void);
protected:
  virtual void writeData(uint8_t reg ,uint8_t *data ,uint8_t len);
  virtual int16_t readData(uint8_t reg ,uint8_t *data ,uint8_t len);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;
};

class DFRobot_MICS_ADC:public DFRobot_MICS{
public:
  DFRobot_MICS_ADC(uint8_t adcPin = A0, uint8_t powerPin = 10);
  bool begin(void);
protected:
  virtual void writeData(uint8_t reg ,uint8_t *data ,uint8_t len);
  virtual int16_t readData(uint8_t reg ,uint8_t *data ,uint8_t len);
private:
  uint8_t __powerPin = 0;
  uint8_t __adcPin   = 0;
};

#endif