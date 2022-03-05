/*
  xnrg_23_ade7880.ino - ADE7880 energy sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends and AndreKR

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_ENERGY_SENSOR
#ifdef USE_ADE7880
/*********************************************************************************************\
 * ADE7880 - Energy used in Shelly 3EM
 *
 * {"NAME":"Shelly 3EM","GPIO":[0,0,0,0,0,8065,0,0,640,8064,608,0,0,0],"FLAG":0,"BASE":18}
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7880.html
 *
 * I2C Address: 0x38
\*********************************************************************************************/

#define XNRG_23                 23
#define XI2C_65                 65  // See I2CDEVICES.md

#define ADE7880_ADDR            0x38


int Ade7880RegSize(uint16_t reg) {
  int size = 0;
/*
  TBD
  switch ((reg >> 8) & 0x0F) {
    case 0x03:
      size++;
    case 0x02:
      size++;
    case 0x01:
      size++;
    case 0x00:
    case 0x07:
    case 0x08:
      size++;
  }
*/
  return size;
}

void Ade7880Write(uint16_t reg, uint32_t val) {
  int size = Ade7880RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7880_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    while (size--) {
      Wire.write((val >> (8 * size)) & 0xFF);  // Write data, MSB first
    }
    Wire.endTransmission();
    delayMicroseconds(5);    // Bus-free time minimum 4.7us
  }
}

int32_t Ade7880Read(uint16_t reg) {
	uint32_t response = 0;

  int size = Ade7880RegSize(reg);
  if (size) {
    Wire.beginTransmission(ADE7880_ADDR);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    Wire.endTransmission(0);
    Wire.requestFrom(ADE7880_ADDR, size);
    if (size <= Wire.available()) {
      for (uint32_t i = 0; i < size; i++) {
        response = response << 8 | Wire.read();   // receive DATA (MSB first)
      }
    }
  }
	return response;
}

void Ade7880Init(void) {
//  Ade7953Write(0x102, 0x0004);    // Locking the communication interface (Clear bit COMM_LOCK), Enable HPF
//  Ade7953Write(0x0FE, 0x00AD);    // Unlock register 0x120
//  Ade7953Write(0x120, 0x0030);    // Configure optimum setting
}

void Ade7880EnergyEverySecond(void) {

}

void Ade7880DrvInit(void) {
  if (PinUsed(GPIO_ADE7880_IRQ) && PinUsed(GPIO_ADE7880_IRQ, 1)) {




  }
}

bool Ade7880Command(void) {
  // Will need calibration for all three phases
  bool serviced = true;


  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg23(uint8_t function) {
  if (!I2cEnabled(XI2C_07)) { return false; }

  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      Ade7880EnergyEverySecond();
      break;
    case FUNC_COMMAND:
      result = Ade7880Command();
      break;
    case FUNC_PRE_INIT:
      Ade7880DrvInit();
      break;
  }
  return result;
}

#endif  // USE_ADE7880
#endif  // USE_ENERGY_SENSOR
#endif  // USE_I2C
