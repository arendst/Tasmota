//
//    FILE: AGS02MA_get_registers.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: low level develop application
//     URL: https://github.com/RobTillaart/AGS02MA
//
// PURPOSE: this is a debugging tool for developing / investigating.
//          Do not use it unless you are willing to crash your sensor.
//
//  usage:  make _readRegister(), _writeRegister() and _buffer public
//
//          USE AT OWN RISK


#include "AGS02MA.h"


AGS02MA AGS(26);


void setup()
{
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

  //  AGS._buffer[0] = 0;
  //  AGS._buffer[1] = 13;
  //  AGS._buffer[2] = 14;
  //  AGS._buffer[3] = 90;
  //  AGS._buffer[5] = 248;
  //  int x = AGS._writeRegister(0x01);  // does not work.
  //  Serial.println(x);

  for (uint8_t reg = 0; reg < 9; reg++)
  {
    dumpRegister(reg);
  }
  dumpRegister(0x11);
  dumpRegister(0x20);
  dumpRegister(0x21);

  AGS.setPPBMode();
}


void loop()
{
//  dumpRegister(0);
//  uint32_t zero = dumpRegister(1);
//  uint32_t x = dumpRegister(0x20);  // seems to be the raw value.
//  delay(100);
//  uint32_t y = AGS.readPPB();
//  Serial.print(zero);
//  Serial.print("\t");
//  Serial.print(x);
//  Serial.print("\t");
//  Serial.print(y);
//  Serial.print("\t");
//  Serial.print((1.0 * x) / y, 2);
//  Serial.print("\t");
//  Serial.print((zero - x) / 350);
//  Serial.println();
//  Serial.println();
//  delay(2000);
}


uint32_t dumpRegister(uint8_t reg)
{
  Serial.print("REG[");
  Serial.print(reg);
  Serial.print("]");

  bool b = AGS._readRegister(reg);
  uint32_t value = 0;
  for (int i = 0; i < 4; i++)
  {
    Serial.print("\t");
    Serial.print(AGS._buffer[i]);

    value *= 256;
    value += AGS._buffer[i];
  }
  Serial.print("\t");
  Serial.print(value);

  Serial.println();
  delay(100);

  return value;
}


//  -- END OF FILE --
