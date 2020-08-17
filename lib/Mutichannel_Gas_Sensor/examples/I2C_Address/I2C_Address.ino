// change i2c address
// Loovee
// 2016-11-10

#include <Wire.h>
#include "MutichannelGasSensor.h"

#define SENSOR_ADDR_OLD     0x04        // default to 0x04
#define SENSOR_ADDR_NEW     0x19        // change i2c address to 0x19

void setup()
{
    Serial.begin(115200);
    gas.begin(SENSOR_ADDR_OLD);     // 
    gas.change_i2c_address(SENSOR_ADDR_NEW);
    Serial.print("I2C ADDRESS SET TO 0X");;
    Serial.println(SENSOR_ADDR_NEW, HEX);
}

void loop()
{
    
}

// END FILE
