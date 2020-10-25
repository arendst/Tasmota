// Calibration code for Grove - Multichannel Gas Sensor
// Note that it need 10 minutes pre-heat before calibration
// This code is writen by Loovee@2016-5-18

#include <Wire.h>
#include "MutichannelGasSensor.h"

#define SENSOR_ADDR     0X04        // default to 0x04
#define PRE_HEAT_TIME   0           // pre-heat time, 10-30 minutes is recommended

void setup()
{
    Serial.begin(115200);
    gas.begin(SENSOR_ADDR);   // 
    Serial.println("power on, and pre-heat");
    
    for(int i=60*PRE_HEAT_TIME; i>=0; i--)
    {
        Serial.print(i/60);
        Serial.print(":");
        Serial.println(i%60);
        delay(1000);
    }
    
    Serial.println("Begin to calibrate...");
    gas.doCalibrate();
    Serial.println("Calibration ok");
    
    gas.display_eeprom();
}

void loop()
{
    
}
