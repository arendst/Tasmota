//
//    FILE: AGS02MA_readRegister.ino
//  AUTHOR: Rob Tillaart, Beanow
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA


#include "AGS02MA.h"


const uint8_t addresses[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0x11, 0x20, 0x21};


AGS02MA::RegisterData reg;
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
}


void loop()
{
  delay(3000);
  for (auto address : addresses)
  {
    bool b = AGS.readRegister(address, reg);
    Serial.print("REG[0x");
    Serial.print(address, HEX);
    Serial.print("]");

    if(b)
    {
      printRegister(address, reg);
    }
    else
    {
      Serial.print("\tError:\t");
      Serial.println(AGS.lastError());
    }
    delay(50);
  }
  Serial.println();
}


void printRegister(uint8_t address, AGS02MA::RegisterData &reg) {
  //  Raw bytes first for any register.
  for (auto b : reg.data)
  {
    Serial.print("\t");
    Serial.print(b);
  }

  Serial.print("\tCRC: ");
  Serial.print(reg.crcValid ? "OK  " : "ERR ");
  Serial.print(reg.crc);

  //  Specific interpretations
  switch (address)
  {
  case 0x00:
    Serial.print("\tSensor data:\t");
    Serial.print(reg.data[0]);
    Serial.print("\t");
    Serial.print(
      (reg.data[1] << 16) +
      (reg.data[2] << 8) +
      reg.data[3]
    );
    break;

  case 0x01:
  case 0x02:
  case 0x03:
  case 0x04:
    Serial.print("\tCalibration:\t");
    Serial.print(
      (reg.data[0] << 8) +
      reg.data[1]
    );
    Serial.print("\t");
    Serial.print(
      (reg.data[2] << 8) +
      reg.data[3]
    );
    break;

  case 0x11:
    Serial.print("\tVersion:\t");
    Serial.print(reg.data[3]);
    break;

  case 0x21:
    Serial.print("\tI2C address:\t0x");
    Serial.print(reg.data[0], HEX);
    break;

  default:
    break;
  }
  Serial.println();
}


//  -- END OF FILE --
