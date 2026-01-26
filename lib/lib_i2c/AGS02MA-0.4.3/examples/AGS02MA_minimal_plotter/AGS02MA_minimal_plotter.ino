//
//    FILE: AGS02MA_minimal_plotter.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application not using the library
//     URL: https://github.com/RobTillaart/AGS02MA
//
//  default register is 0x00 at start of the sensor
//  datasheet states one can get the value with minimal interaction.
//  note this sketch does not use the library!


#include "Wire.h"


uint8_t buffer[5];

uint8_t cnt = 0;


void setup()
{
  //  ESP devices typically miss the first serial log lines after flashing.
  //  Delay somewhat to include all output.
  delay(1000);

  Serial.begin(115200);
  //  Serial.println();
  //  Serial.println(__FILE__);
  //  Serial.print("AGS02MA_LIB_VERSION: ");
  //  Serial.println(AGS02MA_LIB_VERSION);
  //  Serial.println();

  Wire.begin();
  Wire.setClock(30400);      //  lowest speed an UNO supports that works with sensor.
}


void loop()
{
  Wire.requestFrom(26, 5);
  for ( int i = 0; i < 5; i++)
  {
    buffer[i] = Wire.read();
    // Serial.print(buffer[i], HEX);  //  for debugging.
    // Serial.print('\t');
  }
  // Serial.println();

  if (cnt == 0)
  {
    //  CONVERT RAW DATA
    Serial.println("\nSTAT\tPPB\tCRC");
    cnt = 20;
  }
  cnt--;
  if (buffer[0] == 0x10)
  {
    Serial.print(buffer[0]);
    Serial.print("\t");
    Serial.print(buffer[1] * 65536UL + buffer[2] * 256 + buffer[3]);
    Serial.print("\t");
    Serial.print(buffer[4]);
    Serial.println();
    delay(2000);
  }
}


//  -- END OF FILE --
