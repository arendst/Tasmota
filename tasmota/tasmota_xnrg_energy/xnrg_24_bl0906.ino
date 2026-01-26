/*
  xnrg_24_bl0906.ino - BL0906 energy sensor support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_ENERGY_SENSOR
#ifdef USE_BL0906
/*********************************************************************************************\
 * Support the following Shangai Belling energy sensors:
 *
 * BL0906 - Energy (as in Athom 6CH Energy Meter EM6)
 * Based on athom-tech https://github.com/athom-tech/esp32-configs/tree/main/components/bl0906
 * See https://github.com/arendst/Tasmota/discussions/22167
 * 
 * {"NAME":"Athom EM2","GPIO":[0,0,0,0,0,0,0,3200,11329,1,544,0,0,0,0,0,0,0,1,0,0,0],"FLAG":0,"BASE":1}
 * {"NAME":"Athom EM6","GPIO":[0,0,0,0,0,0,0,3200,11333,1,544,0,0,0,0,0,0,0,1,0,0,0],"FLAG":0,"BASE":1}
 * 
 * Optional commands:
 * EnergyCols <phases>`   - Change default 4 column GUI display to <phases> columns
 * VoltRes 1              - Change none to 1 decimal display
 * FreqRes 1              - Change none to 1 decimal display
 * WattRes 2              - Change none to 2 decimals display
 * SetOption21 1          - Display Voltage
 * SetOption129 1         - Display energy for each phase instead of single sum
 * SetOption150 1         - Display no common voltage/frequency
 * EnergyExportActive 1   - Enable display of Export Active energy based on negative Active Power
 \*********************************************************************************************/

#define XNRG_24                     24

#ifndef BL0906_UPDATE
#define BL0906_UPDATE               2    // Update every 2 seconds (Must be lower than ENERGY_WATCHDOG)
#endif
//#define DEBUG_BL0906

// Total power conversion
static const float BL0906_WATT = 16 * 1.097 * 1.097 * (20000 + 20000 + 20000 + 20000 + 20000) /
                                 (40.41259 * ((5.1 + 5.1) * 1000 / 2000) * 1 * 100 * 1 * 1000);
// Total Energy conversion
static const float BL0906_CF = 16 * 4194304 * 0.032768 * 16 /
                               (3600000 * 16 *
                                (40.4125 * ((5.1 + 5.1) * 1000 / 2000) * 1 * 100 * 1 * 1000 /
                                 (1.097 * 1.097 * (20000 + 20000 + 20000 + 20000 + 20000))));
// Frequency conversion
static const float BL0906_FREF = 10000000;
// Temperature conversion
static const float BL0906_TREF = 12.5 / 59 - 40;  // Celsius
// Current conversion
static const float BL0906_IREF = 1.097 / (12875 * 1 * (5.1 + 5.1) * 1000 / 2000);
// Voltage conversion
static const float BL0906_UREF = 1.097 * (20000 + 20000 + 20000 + 20000 + 20000) / (13162 * 1 * 100 * 1000);
// Power conversion
static const float BL0906_PREF = 1.097 * 1.097 * (20000 + 20000 + 20000 + 20000 + 20000) /
                                 (40.41259 * ((5.1 + 5.1) * 1000 / 2000) * 1 * 100 * 1 * 1000);
// Energy conversion
static const float BL0906_EREF = 4194304 * 0.032768 * 16 /
                                 (3600000 * 16 *
                                  (40.4125 * ((5.1 + 5.1) * 1000 / 2000) * 1 * 100 * 1 * 1000 /
                                   (1.097 * 1.097 * (20000 + 20000 + 20000 + 20000 + 20000))));
// Current coefficient
static const float BL0906_KI = 12875 * 1 * (5.1 + 5.1) * 1000 / 2000 / 1.097;
// Power coefficient
static const float BL0906_KP = 40.4125 * ((5.1 + 5.1) * 1000 / 2000) * 1 * 100 * 1 * 1000 / 1.097 / 1.097 /
                               (20000 + 20000 + 20000 + 20000 + 20000);

// Register address
// Voltage
static const uint8_t BL0906_V_RMS = 0x16;

// Total power
static const uint8_t BL0906_WATT_SUM = 0X2C;

// Current1~6
static const uint8_t BL0906_I_1_RMS = 0x0D;  // current_1
static const uint8_t BL0906_I_2_RMS = 0x0E;
static const uint8_t BL0906_I_3_RMS = 0x0F;
static const uint8_t BL0906_I_4_RMS = 0x10;
static const uint8_t BL0906_I_5_RMS = 0x13;
static const uint8_t BL0906_I_6_RMS = 0x14;  // current_6

// Power1~6
static const uint8_t BL0906_WATT_1 = 0X23;  // power_1
static const uint8_t BL0906_WATT_2 = 0X24;
static const uint8_t BL0906_WATT_3 = 0X25;
static const uint8_t BL0906_WATT_4 = 0X26;
static const uint8_t BL0906_WATT_5 = 0X29;
static const uint8_t BL0906_WATT_6 = 0X2A;  // power_6

// Active pulse count, unsigned
static const uint8_t BL0906_CF_1_CNT = 0X30;  // Channel_1
static const uint8_t BL0906_CF_2_CNT = 0X31;
static const uint8_t BL0906_CF_3_CNT = 0X32;
static const uint8_t BL0906_CF_4_CNT = 0X33;
static const uint8_t BL0906_CF_5_CNT = 0X36;
static const uint8_t BL0906_CF_6_CNT = 0X37;  // Channel_6

// Total active pulse count, unsigned
static const uint8_t BL0906_CF_SUM_CNT = 0X39;

// Voltage frequency cycle
static const uint8_t BL0906_FREQUENCY = 0X4E;

// Internal temperature
static const uint8_t BL0906_TEMPERATURE = 0X5E;

// Calibration register
// RMS gain adjustment register
static const uint8_t BL0906_RMSGN_1 = 0x6D;  // Channel_1
static const uint8_t BL0906_RMSGN_2 = 0x6E;
static const uint8_t BL0906_RMSGN_3 = 0x6F;
static const uint8_t BL0906_RMSGN_4 = 0x70;
static const uint8_t BL0906_RMSGN_5 = 0x73;
static const uint8_t BL0906_RMSGN_6 = 0x74;  // Channel_6

// RMS offset correction register
static const uint8_t BL0906_RMSOS_1 = 0x78;  // Channel_1
static const uint8_t BL0906_RMSOS_2 = 0x79;
static const uint8_t BL0906_RMSOS_3 = 0x7A;
static const uint8_t BL0906_RMSOS_4 = 0x7B;
static const uint8_t BL0906_RMSOS_5 = 0x7E;
static const uint8_t BL0906_RMSOS_6 = 0x7F;  // Channel_6

// Active power gain adjustment register
static const uint8_t BL0906_WATTGN_1 = 0xB7;  // Channel_1
static const uint8_t BL0906_WATTGN_2 = 0xB8;
static const uint8_t BL0906_WATTGN_3 = 0xB9;
static const uint8_t BL0906_WATTGN_4 = 0xBA;
static const uint8_t BL0906_WATTGN_5 = 0xBD;
static const uint8_t BL0906_WATTGN_6 = 0xBE;  // Channel_6

// Commands
static const uint8_t BL0906_READ_COMMAND = 0x35;
static const uint8_t BL0906_WRITE_COMMAND = 0xCA;

// User write protection setting register,
// You must first write 0x5555 to the write protection setting register before writing to other registers.
static const uint8_t BL0906_USR_WRPROT = 0x9E;
// Enable User Operation Write
static const uint8_t BL0906_WRPROT_WRITABLE[6] = {BL0906_WRITE_COMMAND, BL0906_USR_WRPROT, 0x55, 0x55, 0x00, 0xB7};
// Disable User Operation Write
static const uint8_t BL0906_WRPROT_ONLYREAD[6] = {BL0906_WRITE_COMMAND, BL0906_USR_WRPROT, 0x00, 0x00, 0x00, 0x61};

// Reset Register
static const uint8_t BL0906_SOFT_RESET = 0x9F;
// Reset to default
static const uint8_t BL0906_INIT[6] = {BL0906_WRITE_COMMAND, BL0906_SOFT_RESET, 0x5A, 0x5A, 0x5A, 0x52};

typedef struct Bl0906DataPacket {
  uint8_t l{0};
  uint8_t m{0};
  uint8_t h{0};
  uint8_t checksum;
  uint8_t address;
} Bl0906DataPacket;

typedef struct Bl0906ube24_t {
  uint8_t l{0};
  uint8_t m{0};
  uint8_t h{0};
} Bl0906ube24_t;

typedef struct Bl0906sbe24_t {
  uint8_t l{0};
  uint8_t m{0};
  int8_t h{0};
} Bl0906sbe24_t;

#include <TasmotaSerial.h>
TasmotaSerial *Bl0906Serial = nullptr;

struct BL0906 {
  float temperature;
  uint16_t baudrate;
  uint8_t current_channel = 0;
  uint8_t model = 0;
  uint8_t rx_pin;
} Bl0906;

/********************************************************************************************/

bool Bl0906_check_read_timeout(size_t len) {
  if (Bl0906Serial->available() >= int(len)) {
    return true;
  }
  uint32_t start_time = millis();
  while (Bl0906Serial->available() < int(len)) {
    if (millis() - start_time > 100) {
#ifdef DEBUG_BL0906
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: Timeout at %u/%u"), Bl0906Serial->available(), len);
#endif  // DEBUG_BL0906
      return false;
    }
    yield();
  }
  return true;
}

bool Bl0906ReadArray(uint8_t *data, size_t len) {
  if (!Bl0906_check_read_timeout(len)) {
    return false;
  }
  Bl0906Serial->read(data, len);
  return true;
}

/********************************************************************************************/

uint32_t Bl0906_to_uint32_t(Bl0906ube24_t input);
uint32_t Bl0906_to_uint32_t(Bl0906ube24_t input) { 
  return input.h << 16 | input.m << 8 | input.l;
}

int32_t Bl0906_to_int32_t(Bl0906sbe24_t input);
int32_t Bl0906_to_int32_t(Bl0906sbe24_t input) { 
  return input.h << 16 | input.m << 8 | input.l;
}

/********************************************************************************************/

// The SUM byte is (Addr+Data_L+Data_M+Data_H)&0xFF negated;
uint8_t Bl0906Checksum(const uint8_t address, const Bl0906DataPacket *data);  // Pre-declare to fix compile error on Bl0906DataPacket
uint8_t Bl0906Checksum(const uint8_t address, const Bl0906DataPacket *data) {
  return (address + data->l + data->m + data->h) ^ 0xFF;
}

// RMS offset correction
void Bl0906BiasCorrection(uint8_t address, float measurements, float correction) {
  Bl0906DataPacket data;
  float ki = 12875 * 1 * (5.1 + 5.1) * 1000 / 2000 / 1.097;  // Current coefficient
  float i_rms0 = measurements * ki;
  float i_rms = correction * ki;
  int32_t value = (i_rms * i_rms - i_rms0 * i_rms0) / 256;
  data.l = value << 24 >> 24;
  data.m = value << 16 >> 24;
  if (value < 0) {
    data.h = (value << 8 >> 24) | 0b10000000;
  }
  data.address = Bl0906Checksum(address, &data);
#ifdef DEBUG_BL0906
  AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: BiasCorrection %02X %02X %02X %02X %02X %02X"), BL0906_WRITE_COMMAND, address, data.l, data.m, data.h, data.address);
#endif  // DEBUG_BL0906
  Bl0906Serial->write(BL0906_WRITE_COMMAND);
  Bl0906Serial->write(address);
  Bl0906Serial->write(data.l);
  Bl0906Serial->write(data.m);
  Bl0906Serial->write(data.h);
  Bl0906Serial->write(data.address);
}
/*
// Gain adjustment
void Bl0906GainCorrection(const uint8_t address, const float measurements, const float Correction, const float coefficient) {
  Bl0906DataPacket data;
  float I_RMS0 = measurements * coefficient;
  float I_RMS = Correction * coefficient;
  float rms_gn = int((I_RMS / I_RMS0 - 1) * 65536);
  int16_t value;
  if (rms_gn <= -32767) {
    value = -32767;
  } else {
    value = int(rms_gn);
  }
  data.h = 0xFF;
  data.m = value >> 8;
  data.l = value << 8 >> 8;
  data.address = Bl0906Checksum(address, &data);
#ifdef DEBUG_BL0906
  AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: GainCorrection %02X %02X %02X %02X %02X %02X"), BL0906_WRITE_COMMAND, address, data.l, data.m, data.h, data.address);
#endif  // DEBUG_BL0906
  Bl0906Serial->write(BL0906_WRITE_COMMAND);
  Bl0906Serial->write(address);
  Bl0906Serial->write(data.l);
  Bl0906Serial->write(data.m);
  Bl0906Serial->write(data.h);
  Bl0906Serial->write(data.address);
}
*/
void Bl0906Setup(void) {
  while (Bl0906Serial->available()) {
    Bl0906Serial->flush();
  }
  Bl0906Serial->write(BL0906_WRPROT_WRITABLE, sizeof(BL0906_WRPROT_WRITABLE));
  // Calibration (1: register address; 2: value before calibration; 3: value after calibration)
  Bl0906BiasCorrection(BL0906_RMSOS_1, 0.01600, 0);  // Calibration current_1
  Bl0906BiasCorrection(BL0906_RMSOS_2, 0.01500, 0);
  Bl0906BiasCorrection(BL0906_RMSOS_3, 0.01400, 0);
  Bl0906BiasCorrection(BL0906_RMSOS_4, 0.01300, 0);
  Bl0906BiasCorrection(BL0906_RMSOS_5, 0.01200, 0);
  Bl0906BiasCorrection(BL0906_RMSOS_6, 0.01200, 0);  // Calibration current_6
/*
  Bl0906GainCorrection(BL0906_RMSGN_1, 2.15000, 2.148, BL0906_KI);   //RMS gain adjustment current_1
  Bl0906GainCorrection(BL0906_RMSGN_2, 2.15100, 2.148, BL0906_KI);
  Bl0906GainCorrection(BL0906_RMSGN_3, 2.15200, 2.148, BL0906_KI);
  Bl0906GainCorrection(BL0906_RMSGN_4, 2.14500, 2.148, BL0906_KI);
  Bl0906GainCorrection(BL0906_RMSGN_5, 2.14600, 2.148, BL0906_KI);
  Bl0906GainCorrection(BL0906_RMSGN_6, 2.14600, 2.148, BL0906_KI);   //RMS gain adjustment current_6

  Bl0906GainCorrection(BL0906_WATTGN_1, 15.13427, 14.5, BL0906_KP);  //Active power gain adjustment power_1
  Bl0906GainCorrection(BL0906_WATTGN_2, 15.23937, 14.5, BL0906_KP);
  Bl0906GainCorrection(BL0906_WATTGN_3, 15.44956, 14.5, BL0906_KP);
  Bl0906GainCorrection(BL0906_WATTGN_4, 16.57646, 14.5, BL0906_KP);
  Bl0906GainCorrection(BL0906_WATTGN_5, 15.27440, 14.5, BL0906_KP);
  Bl0906GainCorrection(BL0906_WATTGN_6, 31.75744, 14.5, BL0906_KP);  //Active power gain adjustment power_6
*/
  Bl0906Serial->write(BL0906_WRPROT_ONLYREAD, sizeof(BL0906_WRPROT_ONLYREAD));
}

// Reset energy
void Bl0906ResetEnergy(void) {
  Bl0906Serial->write(BL0906_INIT, sizeof(BL0906_INIT));
  delay(1);
  Bl0906Serial->flush();    // Flush send buffer
}

// Read data
void Bl0906ReadData(const uint8_t address, const float reference, float *sensor) {
  if (sensor == nullptr) {
    return;
  }

  Bl0906Serial->write(BL0906_READ_COMMAND);
  Bl0906Serial->write(address);
  Bl0906DataPacket buffer;
  if (Bl0906ReadArray((uint8_t *) &buffer, sizeof(buffer) - 1)) {
    if (Bl0906Checksum(address, &buffer) == buffer.checksum) {

      Bl0906ube24_t data_u24;
      Bl0906sbe24_t data_s24;
      bool signed_result = reference == BL0906_TREF || reference == BL0906_WATT || reference == BL0906_PREF;
      if (signed_result) {
        data_s24.l = buffer.l;
        data_s24.m = buffer.m;
        data_s24.h = buffer.h;
      } else {
        data_u24.l = buffer.l;
        data_u24.m = buffer.m;
        data_u24.h = buffer.h;
      }

      float value = 0;
      // Power
      if (reference == BL0906_PREF) {
        value = (float) Bl0906_to_int32_t(data_s24) * reference;
      }
      // Total power
      if (reference == BL0906_WATT) {
        value = (float) Bl0906_to_int32_t(data_s24) * reference;
      }
      // Voltage, current, power, total power
      if (reference == BL0906_UREF || reference == BL0906_IREF || reference == BL0906_EREF || reference == BL0906_CF) {
        value = (float) Bl0906_to_uint32_t(data_u24) * reference;
      }
      // Frequency
      if (reference == BL0906_FREF) {
        value = reference / (float) Bl0906_to_uint32_t(data_u24);
      }
      // Chip temperature
      if (reference == BL0906_TREF) {
        value = (float) Bl0906_to_int32_t(data_s24);
        value = (value - 64) * 12.5 / 59 - 40;  // Celsius
//        value = (value - 64) * reference;
        value = ConvertTemp(value);
      }
      *sensor = value;
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: CRC error"));
      while (Bl0906Serial->read() >= 0);  // Flush receive buffer
    }
  }
}

/********************************************************************************************/

void Bl0906Loop(void) {
  if (UINT8_MAX == Bl0906.current_channel) {
    return;
  }

  while (Bl0906Serial->available()) {
    Bl0906Serial->flush();
  }
  if (0 == Bl0906.current_channel) {
#ifdef DEBUG_BL0906
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: Start polling"));
#endif  // DEBUG_BL0906
    // Temperature
    Bl0906ReadData(BL0906_TEMPERATURE, BL0906_TREF, &Bl0906.temperature);
  } else if (1 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_1_RMS, BL0906_IREF, &Energy->current[0]);
    Bl0906ReadData(BL0906_WATT_1, BL0906_PREF, &Energy->active_power[0]);
    Bl0906ReadData(BL0906_CF_1_CNT, BL0906_EREF, &Energy->import_active[0]);
    Energy->data_valid[0] = 0;
  } else if (2 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_2_RMS, BL0906_IREF, &Energy->current[1]);
    Bl0906ReadData(BL0906_WATT_2, BL0906_PREF, &Energy->active_power[1]);
    Bl0906ReadData(BL0906_CF_2_CNT, BL0906_EREF, &Energy->import_active[1]);
    Energy->data_valid[1] = 0;
  } else if (3 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_3_RMS, BL0906_IREF, &Energy->current[2]);
    Bl0906ReadData(BL0906_WATT_3, BL0906_PREF, &Energy->active_power[2]);
    Bl0906ReadData(BL0906_CF_3_CNT, BL0906_EREF, &Energy->import_active[2]);
    Energy->data_valid[2] = 0;
  } else if (4 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_4_RMS, BL0906_IREF, &Energy->current[3]);
    Bl0906ReadData(BL0906_WATT_4, BL0906_PREF, &Energy->active_power[3]);
    Bl0906ReadData(BL0906_CF_4_CNT, BL0906_EREF, &Energy->import_active[3]);
    Energy->data_valid[3] = 0;
  } else if (5 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_5_RMS, BL0906_IREF, &Energy->current[4]);
    Bl0906ReadData(BL0906_WATT_5, BL0906_PREF, &Energy->active_power[4]);
    Bl0906ReadData(BL0906_CF_5_CNT, BL0906_EREF, &Energy->import_active[4]);
    Energy->data_valid[4] = 0;
  } else if (6 == Bl0906.current_channel) {
    Bl0906ReadData(BL0906_I_6_RMS, BL0906_IREF, &Energy->current[5]);
    Bl0906ReadData(BL0906_WATT_6, BL0906_PREF, &Energy->active_power[5]);
    Bl0906ReadData(BL0906_CF_6_CNT, BL0906_EREF, &Energy->import_active[5]);
    Energy->data_valid[5] = 0;
  } else if (8 == Bl0906.current_channel) {
    // Frequency
    Bl0906ReadData(BL0906_FREQUENCY, BL0906_FREF, &Energy->frequency[0]);
    // Voltage
    Bl0906ReadData(BL0906_V_RMS, BL0906_UREF, &Energy->voltage[0]);
  } else if (9 == Bl0906.current_channel) {
    // Total power
//    Bl0906ReadData(BL0906_WATT_SUM, BL0906_WATT, this->total_power_sensor_);
    // Total Energy
//    Bl0906ReadData(BL0906_CF_SUM_CNT, BL0906_CF, &Energy->total[0]);

    EnergyUpdateTotal();
  } else {
    Bl0906.current_channel = UINT8_MAX - 1;  // Stop
#ifdef DEBUG_BL0906
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: Stop polling"));
#endif  // DEBUG_BL0906
  }
  if (Bl0906.current_channel == Energy->phase_count) {
    Bl0906.current_channel = 7;    // Skip next phases and go to frequency and voltage
  }

  Bl0906.current_channel++;

/*
  while (Bl0906Serial->available()) {
    Bl0906Serial->read();
  }
*/
  while (Bl0906Serial->read() >= 0);
}

void Bl0906EverySecond(void) {
  if (!(TasmotaGlobal.uptime % BL0906_UPDATE)) {  // Every BL0906_UPDATE seconds an update
    if (UINT8_MAX == Bl0906.current_channel) {
      Bl0906.current_channel = 0;
    }
  }
}

void Bl0906Show(bool json) {
  if (json) {
    ResponseAppend_P(JSON_SNS_F_TEMP, "BL0906", Settings->flag2.temperature_resolution, &Bl0906.temperature);
    if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
      DomoticzFloatSensor(DZ_TEMP, Bl0906.temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      KnxSensor(KNX_TEMPERATURE, Bl0906.temperature);
#endif // USE_KNX
    }
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("", Bl0906.temperature);
#endif  // USE_WEBSERVER
  }
}

void Bl0906Init(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Bl0906Serial = new TasmotaSerial(Bl0906.rx_pin, Pin(GPIO_TXD), 1);
  if (Bl0906Serial->begin(Bl0906.baudrate)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: Serial UART%d"), Bl0906Serial->getUart());
    Bl0906Setup();
//    Bl0906ResetEnergy();
  }
}

void Bl0906PreInit(void) {
  if (PinUsed(GPIO_TXD) && PinUsed(GPIO_BL0906_RX, GPIO_ANY)) {
    Bl0906.rx_pin = Pin(GPIO_BL0906_RX, GPIO_ANY);
    uint32_t option = GetPin(Bl0906.rx_pin) - AGPIO(GPIO_BL0906_RX);  // 0 .. 5
    Bl0906.baudrate = 19200;

    Energy->voltage_common = true;                 // Use common voltage
    Energy->frequency_common = true;               // Use common frequency
    Energy->use_overtemp = true;                   // Use global temperature for overtemp detection
    Energy->phase_count = option +1;               // Handle 1 to 6 channels as phases

    TasmotaGlobal.energy_driver = XNRG_24;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg24(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (Bl0906Serial) { Bl0906Loop(); }
      break;
    case FUNC_EVERY_SECOND:
      Bl0906EverySecond();
      break;
    case FUNC_JSON_APPEND:
      Bl0906Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Bl0906Show(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      Bl0906Init();
      break;
    case FUNC_PRE_INIT:
      Bl0906PreInit();
      break;
  }
  return result;
}

#endif  // USE_BL0906
#endif  // USE_ENERGY_SENSOR
#endif  // ESP32
