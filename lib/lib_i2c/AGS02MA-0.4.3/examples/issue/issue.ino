//
//    FILE: AGS02MA_PPB.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA


#include "AGS02MA.h"

AGS02MA AGS(26);

uint32_t rounds = 0;


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

  Serial.print("VERSION:\t");
  Serial.println(AGS.getSensorVersion());

  Serial.print("DATE:\t");
  Serial.println(AGS.getSensorDate(), HEX);

  //  pre-heating improves measurement quality
  //  can be skipped
  //  Serial.println("\nWarming up (120 seconds = 24 dots)");
  //  while (AGS.isHeated() == false)
  //  {
  //    delay(5000);
  //    Serial.print(".");
  //  }
  //  Serial.println();

  uint8_t version = AGS.getSensorVersion();
  Serial.print("VERS:\t");
  Serial.println(version);
}


void loop()
{
  delay(3000);

  uint8_t kind = rounds % 20;

  //  Switch mode every 10 and 20 rounds.
  bool b;
  if (kind == 0) {
    b = AGS.setPPBMode();
    uint8_t m = AGS.getMode();
    Serial.print("MODE:\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.println(m);
  } else if (kind == 10) {
    b = AGS.setUGM3Mode();
    uint8_t m = AGS.getMode();
    Serial.print("MODE:\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.println(m);
  }

  // Read PPB in first half of a 20-round cycle.
  if (kind < 10) {
    uint32_t value = AGS.readPPB();
    Serial.print("PPB:\t");
    Serial.print(value);
    Serial.print("\t");
    Serial.print(AGS.lastStatus(), HEX);
    Serial.print("\t");
    Serial.print(AGS.lastError(), HEX);
    Serial.println();
  } else {
    uint32_t value = AGS.readUGM3();
    Serial.print("UGM3:\t");
    Serial.print(value);
    Serial.print("\t");
    Serial.print(AGS.lastStatus(), HEX);
    Serial.print("\t");
    Serial.print(AGS.lastError(), HEX);
    Serial.println();
  }

  rounds++;
}


//  -- END OF FILE --
