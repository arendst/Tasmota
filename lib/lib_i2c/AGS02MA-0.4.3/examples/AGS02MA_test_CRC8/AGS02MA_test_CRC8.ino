//
//    FILE: AGS02MA_test_CRC8.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA
//
//  NOTE: this is a low level test for the communication / CRC
//  to have this example to work,
//  one need to make the _CRC8() and _buffer[]
//  public in the AGS02MA.h file.


#include "AGS02MA.h"


AGS02MA AGS(26);


void setup()
{
  //  ESP devices typically miss the first serial log lines after flashing.
  //  Delay somewhat to include all output.
  delay(1000);

  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AGS02MA_LIB_VERSION: ");
  Serial.println(AGS02MA_LIB_VERSION);
  Serial.println();

  Wire.begin();

  bool b = AGS.begin();
  Serial.print("BEGIN:\t");
  Serial.println(b);

  uint8_t version = AGS.getSensorVersion();
  Serial.print("VERS:\t");
  Serial.println(version);
  dump("getSensorVersion");
}


void loop()
{
  delay(3000);
  AGS.setPPBMode();
  dump("MODE0");
  AGS.readPPB();
  dump("PPB");

  delay(3000);
  AGS.setUGM3Mode();
  dump("MODE1");
  AGS.readUGM3();
  dump("UGM3");
}


void dump(char * str)
{
  Serial.print(str);
  for (int i = 0; i < 5; i++)
  {
    Serial.print("\t");
    Serial.print(AGS._buffer[i], HEX);
  }
  Serial.print('\t');
  Serial.print(AGS._CRC8(AGS._buffer, 4), HEX);
  Serial.println();
}


//  -- END OF FILE --

