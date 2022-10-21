// get RAW data from the sensor
// Loovee
// 2016-11-10

#include <Wire.h>
#include "MutichannelGasSensor.h"

#define SENSOR_ADDR     0X04        // default to 0x04

void setup()
{
    Serial.begin(115200);
    gas.begin(SENSOR_ADDR);     // 
}

void loop()
{
    float R0_NH3, R0_CO, R0_NO2;
    float Rs_NH3, Rs_CO, Rs_NO2;
    float ratio_NH3, ratio_CO, ratio_NO2;
    
    R0_NH3 = gas.getR0(0);
    R0_CO  = gas.getR0(1);
    R0_NO2 = gas.getR0(2);
    
    Rs_NH3 = gas.getRs(0);
    Rs_CO  = gas.getRs(1);
    Rs_NO2 = gas.getRs(2);
    
    ratio_NH3 = Rs_NH3/R0_NH3;
    ratio_CO  = Rs_CO/R0_CO;
    ratio_NO2 = Rs_NH3/R0_NO2;
    
    Serial.println("R0:");
    Serial.print(R0_NH3);
    Serial.print('\t');
    Serial.print(R0_CO);
    Serial.print('\t');
    Serial.println(R0_NO2);
    
    Serial.println("Rs:");
    Serial.print(Rs_NH3);
    Serial.print('\t');
    Serial.print(Rs_CO);
    Serial.print('\t');
    Serial.println(Rs_NO2);
    
    Serial.println("ratio:");
    Serial.print(ratio_NH3);
    Serial.print('\t');
    Serial.print(ratio_CO);
    Serial.print('\t');
    Serial.println(ratio_NO2);

    Serial.println("------------------------");
    delay(1000);
}
