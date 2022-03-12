/*
  xsns_96_pcf85363.ino - PCF85363 RTC chip support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_PCF85363
/*********************************************************************************************\
 * PCF85363 support
 *
 * I2C Address: 0x51
\*********************************************************************************************/

#define XSNS_96             96
#define XI2C_66             66      // See I2CDEVICES.md

#define USE_PCF85363_ADDR   0x51    // PCF85363 I2C Address

bool pcf85363_detected = false;

/*-------------------------------------------------------------------------------------------*\
 * Read time and return the epoch time (second since 1-1-1970 00:00)
\*-------------------------------------------------------------------------------------------*/
uint32_t Pcf85363ReadTime(void) {
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();

  uint8_t buffer[8];
  Wire.requestFrom((int)USE_PCF85363_ADDR, (int)8);
  for (uint32_t i = 0; i < 8; i++) { buffer[i] = Wire.read(); }
  Wire.endTransmission();

  TIME_T tm;
  tm.second = Bcd2Dec(buffer[1] & 0x7F);
  tm.minute = Bcd2Dec(buffer[2] & 0x7F);
  tm.hour = Bcd2Dec(buffer[3]);
  tm.day_of_month = Bcd2Dec(buffer[4]);
  tm.day_of_week = buffer[5];
  tm.month = Bcd2Dec(buffer[6]);
  tm.year = 30 + Bcd2Dec(buffer[7]);  // Offset from 1970. So 2022 - 1970 = 52
  return MakeTime(tm);
}

/*-------------------------------------------------------------------------------------------*\
 * Get time as TIME_T and set time to this value
\*-------------------------------------------------------------------------------------------*/
void Pcf85363SetTime(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);

  uint8_t buffer[8];
  buffer[0] = 0x00;                  // 100th_seconds (not used)
  buffer[1] = Dec2Bcd(tm.second);
  buffer[2] = Dec2Bcd(tm.minute);
  buffer[3] = Dec2Bcd(tm.hour);
  buffer[4] = Dec2Bcd(tm.day_of_month);
  buffer[5] = tm.day_of_week;
  buffer[6] = Dec2Bcd(tm.month);
  buffer[7] = Dec2Bcd(tm.year -30);  // Offset from 1970
/*
  // Handbook page 13
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x2E);
  Wire.write(0x01);                  // Set stop
  Wire.write(0xA4);                  // Clear prescaler
  for (uint32_t i = 0; i < 8; i++) { Wire.write(buffer[i]); }
  Wire.endTransmission();
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x2E);
  Wire.write(0x00);                  // Set start
  Wire.endTransmission();
*/
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x00);
  for (uint32_t i = 0; i < 8; i++) { Wire.write(buffer[i]); }
  Wire.endTransmission();
}

/*-------------------------------------------------------------------------------------------*\
 * Dump all registers
\*-------------------------------------------------------------------------------------------*/
/*
void Pcf85363Dump(void) {
  uint8_t buffer[64];

  // 0x00 to 0x2F
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom((int)USE_PCF85363_ADDR, (int)48);
  for (uint32_t i = 0; i < 48; i++) {
    buffer[i] = Wire.read();
  }
  Wire.endTransmission();
  AddLog(LOG_LEVEL_DEBUG, PSTR("P85: Read 0x00: %48_H"), buffer);

  // 0x40 to 0x7F
  Wire.beginTransmission((uint8_t)USE_PCF85363_ADDR);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.requestFrom((int)USE_PCF85363_ADDR, (int)64);
  for (uint32_t i = 0; i < 64; i++) {
    buffer[i] = Wire.read();
  }
  Wire.endTransmission();
  AddLog(LOG_LEVEL_DEBUG, PSTR("P85: Read 0x40: %64_H"), buffer);
}
*/
/*********************************************************************************************/

void Pcf85363Detect(void) {
  if (!I2cSetDevice(USE_PCF85363_ADDR)) { return; }
  I2cSetActiveFound(USE_PCF85363_ADDR, "PCF85363");

  pcf85363_detected = true;

  if (Rtc.utc_time < START_VALID_TIME) {                          // Not sync with NTP/GPS (time not valid), so read time
    uint32_t time = Pcf85363ReadTime();                           // Read UTC TIME
    if (time > START_VALID_TIME) {
      Rtc.utc_time = time;
      RtcSync("PCF85363");
    }
  }
}

void Pcf85363TimeSynced(void) {
  if ((Rtc.utc_time > START_VALID_TIME) &&                        // Valid UTC time
      (abs((int32_t)(Rtc.utc_time - Pcf85363ReadTime())) > 2)) {  // Time has drifted from RTC more than 2 seconds
    Pcf85363SetTime(Rtc.utc_time);                                // Update time
    AddLog(LOG_LEVEL_DEBUG, PSTR("P85: Re-synced (" D_UTC_TIME ") %s"), GetDateAndTime(DT_UTC).c_str());
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns96(uint8_t function) {
  if (!I2cEnabled(XI2C_66)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Pcf85363Detect();
  }
  else if (pcf85363_detected) {
    switch (function) {
      case FUNC_TIME_SYNCED:
        Pcf85363TimeSynced();
        break;
    }
  }
  return result;
}

#endif // USE_PCF85363
#endif // USE_I2C
