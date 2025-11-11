//
//    FILE: AGS02MA_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA


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

  b = AGS.setPPBMode();
  uint8_t m = AGS.getMode();
  Serial.print("MODE:\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.println(m);

  uint8_t version = AGS.getSensorVersion();
  Serial.print("VERS:\t");
  Serial.println(version);
}


void loop()
{
  delay(2000);
  uint32_t value = AGS.readPPB();
  Serial.print("PPB:\t");
  Serial.println(value);
}


//  -- END OF FILE --
