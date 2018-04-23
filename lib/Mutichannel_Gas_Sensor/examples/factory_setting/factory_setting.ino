// factory setting
// Loovee
// 2016-11-10

#include <Wire.h>
#include "MutichannelGasSensor.h"

#define SENSOR_ADDR     0X04        // default to 0x04

void setup()
{
    Serial.begin(115200);
    Serial.println("Please input something to continue");
    while(!Serial.available());

    gas.begin(SENSOR_ADDR);     // 
    
    Serial.println("FACTORY SETTING OK");
    
    float R0_NH3, R0_CO, R0_NO2;
    
    R0_NH3 = gas.getR0(0);
    R0_CO  = gas.getR0(1);
    R0_NO2 = gas.getR0(2);
    
    Serial.print("R0_NH3 = ");
    Serial.println(R0_NH3);
    Serial.print("R0_CO = ");
    Serial.println(R0_CO);
    Serial.print("R0_NO2 = ");
    Serial.println(R0_NO2);
}

void loop()
{
}
