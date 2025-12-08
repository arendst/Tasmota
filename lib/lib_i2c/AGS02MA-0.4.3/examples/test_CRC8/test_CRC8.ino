//
//    FILE: test_CRC8.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA
//
//  just for develop scratch pad
//  need to make the CRC function public in the library


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

  uint8_t _buffer[8];

  _buffer[0] = 0x00;
  _buffer[1] = 0x0C;
  _buffer[2] = 0xFF;
  _buffer[3] = 0xF3;
  _buffer[4] = 0xFC;
  Serial.println(AGS._CRC8(_buffer, 5), HEX);


  _buffer[0] = 0x02;
  _buffer[1] = 0xFD;
  _buffer[2] = 0x02;
  _buffer[3] = 0xFD;
  _buffer[4] = 0x00;
  Serial.println(AGS._CRC8(_buffer, 5), HEX);

  _buffer[0] = 0x00;
  _buffer[1] = 0xFF;
  _buffer[2] = 0x00;
  _buffer[3] = 0xFF;
  _buffer[4] = 0x30;
  Serial.println(AGS._CRC8(_buffer, 5), HEX);

  _buffer[0] = 0x14;
  _buffer[1] = 0x14;
  _buffer[2] = 0x14;
  _buffer[3] = 0x1C;
  _buffer[4] = 0x75;
  Serial.println(AGS._CRC8(_buffer, 5), HEX);

  _buffer[0] = 0x0;
  _buffer[1] = 0x0;
  _buffer[2] = 0xBB;
  Serial.println( _buffer[0] * 65536UL + _buffer[1] * 256 + _buffer[2]);
  _buffer[0] = 0x0;
  _buffer[1] = 0x01;
  _buffer[2] = 0xAE;
  Serial.println( _buffer[0] * 65536UL + _buffer[1] * 256 + _buffer[2]);
}


void loop()
{
}


//  -- END OF FILE --
