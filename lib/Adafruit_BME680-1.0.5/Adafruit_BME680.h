/***************************************************************************
  This is a library for the BME680 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/XXXX

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#ifndef __BME680_H__
#define __BME680_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "bme680.h"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define BME680_DEFAULT_ADDRESS                (0x77)
/*=========================================================================*/
#define BME680_DEFAULT_SPIFREQ               (1000000)



/*
class Adafruit_BME680_Unified : public Adafruit_Sensor
{
public:
    Adafruit_BME680_Unified(int32_t sensorID = -1);

    bool  begin(uint8_t addr = BME680_ADDRESS);
    void  getTemperature(float *temp);
    void  getPressure(float *pressure);
    float pressureToAltitude(float seaLevel, float atmospheric, float temp);
    float seaLevelForAltitude(float altitude, float atmospheric, float temp);
    void  getEvent(sensors_event_t*);
    void  getSensor(sensor_t*);

  private:
    uint8_t   _i2c_addr;
    int32_t   _sensorID;
};

*/

/** Adafruit_BME680 Class for both I2C and SPI usage. 
 *  Wraps the Bosch library for Arduino usage
 */

class Adafruit_BME680
{
  public:
    Adafruit_BME680(int8_t cspin = -1);
    Adafruit_BME680(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool  begin(uint8_t addr = BME680_DEFAULT_ADDRESS);
    float readTemperature(void);
    float readPressure(void);
    float readHumidity(void);
    uint32_t readGas(void);
    float readAltitude(float seaLevel);

    bool setTemperatureOversampling(uint8_t os);
    bool setPressureOversampling(uint8_t os);
    bool setHumidityOversampling(uint8_t os);
    bool setIIRFilterSize(uint8_t fs);
    bool setGasHeater(uint16_t heaterTemp, uint16_t heaterTime);

    bool performReading(void);

    /// Temperature (Celsius) assigned after calling performReading() 
    float temperature;  
    /// Pressure (Pascals) assigned after calling performReading() 
    float pressure;
    /// Humidity (RH %) assigned after calling performReading()
    float humidity;
    /// Gas resistor (ohms) assigned after calling performReading()
    float gas_resistance;
  private:

    bool _filterEnabled, _tempEnabled, _humEnabled, _presEnabled, _gasEnabled;
    uint8_t _i2caddr;
    int32_t _sensorID;
    int8_t _cs;

    uint8_t spixfer(uint8_t x);

    struct bme680_dev gas_sensor;
};

#endif
