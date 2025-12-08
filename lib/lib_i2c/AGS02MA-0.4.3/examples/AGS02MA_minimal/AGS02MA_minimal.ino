//
//    FILE: AGS02MA_minimal.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA
//
//  default register is 0x00 at start of the sensor
//  datasheet states one can get the value with minimal interaction.
//  note this sketch does not use the library!


#include "Wire.h"


uint8_t buffer[5];


void setup()
{
  //  ESP devices typically miss the first serial log lines after flashing.
  //  Delay somewhat to include all output.
  delay(1000);

  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  //  Serial.print("AGS02MA_LIB_VERSION: ");
  //  Serial.println(AGS02MA_LIB_VERSION);
  Serial.println();

  Wire.begin();
  Wire.setClock(30400);      //  lowest speed an UNO supports that works with sensor.
}


void loop()
{
  delay(3000);
  Wire.requestFrom(26, 5);
  for ( int i = 0; i < 5; i++)
  {
    buffer[i] = Wire.read();
    // Serial.print(buffer[i], HEX);  // for debugging.
    // Serial.print('\t');
  }
  Serial.println();

  //  CONVERT RAW DATA
  Serial.print("STAT:\t");
  Serial.println(buffer[0]);
  Serial.print("PPB:\t");
  Serial.println(buffer[1] * 65536UL + buffer[2] * 256 + buffer[3]);
  Serial.print("CRC:\t");
  Serial.println(buffer[4]);
  Serial.println();
}


//  -- END OF FILE --
