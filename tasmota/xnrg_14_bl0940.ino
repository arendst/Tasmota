/*
  xnrg_14_bl0940.ino - BL0940 energy sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_ENERGY_SENSOR
#ifdef USE_BL0940
/*********************************************************************************************\
 * BL0940 - Energy (Blitzwolf SHP10)
 *
 * Template {"NAME":"BW-SHP10","GPIO":[0,148,0,207,158,21,0,0,0,17,0,0,0],"FLAG":0,"BASE":18}
 *
 * Based on datasheet from http://www.belling.com.cn/media/file_object/bel_product/BL0940/datasheet/BL0940_V1.1_en.pdf
\*********************************************************************************************/

#define XNRG_14                     14

#define BL0940_PREF                 1430
#define BL0940_UREF                 33000
#define BL0940_IREF                 2750

#define BL0940_PULSES_NOT_INITIALIZED  -1

#define BL0940_BUFFER_SIZE          36

#define BL0940_WRITE_COMMAND        0xA0  // 0xA8 according to documentation
#define BL0940_REG_I_FAST_RMS_CTRL  0x10
#define BL0940_REG_MODE             0x18
#define BL0940_REG_SOFT_RESET       0x19
#define BL0940_REG_USR_WRPROT       0x1A
#define BL0940_REG_TPS_CTRL         0x1B

#define BL0940_READ_COMMAND         0x50  // 0x58 according to documentation
#define BL0940_FULL_PACKET          0xAA

#define BL0940_PACKET_HEADER        0x55  // 0x58 according to documentation

#include <TasmotaSerial.h>

TasmotaSerial *Bl0940Serial = nullptr;

struct BL0940 {
  long voltage = 0;
  long current = 0;
  long power = 0;
  long power_cycle_first = 0;
  long cf_pulses = 0;
  long cf_pulses_last_time = BL0940_PULSES_NOT_INITIALIZED;
  float temperature;

  int byte_counter = 0;
  uint16_t tps1 = 0;
  uint8_t *rx_buffer = nullptr;
  bool received = false;
} Bl0940;

const uint8_t bl0940_init[5][6] = {
  { BL0940_WRITE_COMMAND, BL0940_REG_SOFT_RESET,      0x5A, 0x5A, 0x5A, 0x38 },   // Reset to default
  { BL0940_WRITE_COMMAND, BL0940_REG_USR_WRPROT,      0x55, 0x00, 0x00, 0xF0 },   // Enable User Operation Write
  { BL0940_WRITE_COMMAND, BL0940_REG_MODE,            0x00, 0x10, 0x00, 0x37 },   // 0x0100 = CF_UNABLE energy pulse, AC_FREQ_SEL 50Hz, RMS_UPDATE_SEL 800mS
  { BL0940_WRITE_COMMAND, BL0940_REG_TPS_CTRL,        0xFF, 0x47, 0x00, 0xFE },   // 0x47FF = Over-current and leakage alarm on, Automatic temperature measurement, Interval 100mS
  { BL0940_WRITE_COMMAND, BL0940_REG_I_FAST_RMS_CTRL, 0x1C, 0x18, 0x00, 0x1B }};  // 0x181C = Half cycle, Fast RMS threshold 6172

void Bl0940Received(void) {
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
  // 55 F2 03 00 00 00 00 7E 02 00 D4 B0 72 AC 01 00 00 00 00 02 01 00 00 00 00 00 00 00 BA 01 00 FE 03 00 83
  // 55 88 02 00 49 00 00 FE 02 00 AF EF 71 D2 01 00 EB FF FF 49 01 00 00 00 00 02 00 00 CF 01 00 FE 03 00 9F
  // 55 B9 33 00 DE 45 00 94 02 00 CF E4 70 63 02 00 6C 4C 00 13 01 00 09 00 00 00 00 00 E4 01 00 FE 03 00 72
  // Hd IFRms--- Current- Reserved Voltage- Reserved Power--- Reserved CF------ Reserved TPS1---- TPS2---- Ck

  uint16_t tps1 = Bl0940.rx_buffer[29] << 8 | Bl0940.rx_buffer[28];                                      // TPS1 unsigned
  if ((Bl0940.rx_buffer[0] != BL0940_PACKET_HEADER) ||                                                   // Bad header
      (Bl0940.tps1 && ((tps1 < (Bl0940.tps1 -10)) || (tps1 > (Bl0940.tps1 +10))))                        // Invalid temperature change
     ) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: Invalid data"));
    return;
  }

  Bl0940.tps1 = tps1;
  float t = ((170.0f/448.0f)*(((float)Bl0940.tps1/2.0f)-32.0f))-45.0f;
  Bl0940.temperature = ConvertTemp(t);

  Bl0940.voltage = Bl0940.rx_buffer[12] << 16 | Bl0940.rx_buffer[11] << 8 | Bl0940.rx_buffer[10];        // V_RMS unsigned
  Bl0940.current = Bl0940.rx_buffer[6] << 16 | Bl0940.rx_buffer[5] << 8 | Bl0940.rx_buffer[4];           // I_RMS unsigned
  int32_t power = Bl0940.rx_buffer[18] << 24 | Bl0940.rx_buffer[17] << 16 | Bl0940.rx_buffer[16] << 8;   // WATT signed
  Bl0940.power = abs(power) >> 8;                                                                        // WATT unsigned
  int32_t cf_cnt = Bl0940.rx_buffer[24] << 24 | Bl0940.rx_buffer[23] << 16 | Bl0940.rx_buffer[22] << 8;  // CF_CNT signed
  Bl0940.cf_pulses = abs(cf_cnt) >> 8;

  AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: U %d, I %d, P %d, C %d, T %d"),
    Bl0940.voltage, Bl0940.current, Bl0940.power, Bl0940.cf_pulses, Bl0940.tps1);

  if (Energy.power_on) {  // Powered on
    Energy.voltage[0] = (float)Bl0940.voltage / Settings->energy_voltage_calibration;
    if (power && (Bl0940.power > Settings->energy_power_calibration)) {                                   // We need at least 1W
      Energy.active_power[0] = (float)Bl0940.power / Settings->energy_power_calibration;
      Energy.current[0] = (float)Bl0940.current / (Settings->energy_current_calibration * 100);
    } else {
      Energy.active_power[0] = 0;
      Energy.current[0] = 0;
    }
  } else {  // Powered off
//    Bl0940.power_cycle_first = 0;
    Energy.voltage[0] = 0;
    Energy.active_power[0] = 0;
    Energy.current[0] = 0;
  }
}

void Bl0940SerialInput(void) {
  while (Bl0940Serial->available()) {
    yield();
    uint8_t serial_in_byte = Bl0940Serial->read();
    if (!Bl0940.received && (BL0940_PACKET_HEADER == serial_in_byte)) {
      Bl0940.received = true;
      Bl0940.byte_counter = 0;
    }
    if (Bl0940.received) {
      Bl0940.rx_buffer[Bl0940.byte_counter++] = serial_in_byte;
      if (BL0940_BUFFER_SIZE == Bl0940.byte_counter) {

        AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Bl0940.rx_buffer, BL0940_BUFFER_SIZE -1);

        uint8_t checksum = BL0940_READ_COMMAND;
        for (uint32_t i = 0; i < BL0940_BUFFER_SIZE -2; i++) { checksum += Bl0940.rx_buffer[i]; }
        checksum ^= 0xFF;
        if (checksum == Bl0940.rx_buffer[34]) {
          Energy.data_valid[0] = 0;
          Bl0940Received();
          Bl0940.received = false;
          return;
        } else {
          do {  // Sync buffer with data (issue #1907 and #3425)
            memmove(Bl0940.rx_buffer, Bl0940.rx_buffer +1, BL0940_BUFFER_SIZE -1);
            Bl0940.byte_counter--;
          } while ((Bl0940.byte_counter > 1) && (BL0940_PACKET_HEADER != Bl0940.rx_buffer[0]));
          if (BL0940_PACKET_HEADER != Bl0940.rx_buffer[0]) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: " D_CHECKSUM_FAILURE));
            Bl0940.received = false;
            Bl0940.byte_counter = 0;
          }
        }
      }
    }
  }
}

/********************************************************************************************/

void Bl0940EverySecond(void) {
  if (Energy.data_valid[0] > ENERGY_WATCHDOG) {
    Bl0940.voltage = 0;
    Bl0940.current = 0;
    Bl0940.power = 0;
  } else {
/*
    // Calculate energy by using active power
    if (Energy.active_power[0]) {
      Energy.kWhtoday_delta += (Energy.active_power[0] * 1000) / 36;
      EnergyUpdateToday();
    }
*/
    // Calculate energy by using active energy pulse count
    if (BL0940_PULSES_NOT_INITIALIZED == Bl0940.cf_pulses_last_time) {
      Bl0940.cf_pulses_last_time = Bl0940.cf_pulses;  // Init after restart
    } else {
      uint32_t cf_pulses = 0;
      if (Bl0940.cf_pulses < Bl0940.cf_pulses_last_time) {  // Rolled over after 0xFFFFFF (16777215) pulses
        cf_pulses = (0x1000000 - Bl0940.cf_pulses_last_time) + Bl0940.cf_pulses;
      } else {
        cf_pulses = Bl0940.cf_pulses - Bl0940.cf_pulses_last_time;
      }
      if (cf_pulses && Energy.active_power[0])  {
        uint32_t watt256 = (1638400 * 256) / Settings->energy_power_calibration;
        uint32_t delta = (cf_pulses * watt256) / 36;
        if (delta <= (4000 * 1000 / 36)) {  // max load for SHP10: 4.00kW (3.68kW)
          Bl0940.cf_pulses_last_time = Bl0940.cf_pulses;
          Energy.kWhtoday_delta += delta;
        } else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: Overload"));
          Bl0940.cf_pulses_last_time = BL0940_PULSES_NOT_INITIALIZED;
        }
        EnergyUpdateToday();
      }
    }

  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: Poll"));

  Bl0940Serial->flush();
  Bl0940Serial->write(BL0940_READ_COMMAND);
  Bl0940Serial->write(BL0940_FULL_PACKET);
}

void Bl0940SnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Bl0940Serial = new TasmotaSerial(Pin(GPIO_BL0940_RX), Pin(GPIO_TXD), 1);
  if (Bl0940Serial->begin(4800, 1)) {
    if (Bl0940Serial->hardwareSerial()) {
      ClaimSerial();
    }
    if (HLW_UREF_PULSE == Settings->energy_voltage_calibration) {
      Settings->energy_voltage_calibration = BL0940_UREF;
      Settings->energy_current_calibration = BL0940_IREF;
      Settings->energy_power_calibration = BL0940_PREF;
    }
    Energy.use_overtemp = true;                 // Use global temperature for overtemp detection

    for (uint32_t i = 0; i < 5; i++) {
      for (uint32_t j = 0; j < 6; j++) {
        Bl0940Serial->write(bl0940_init[i][j]);
//        Bl0940Serial->write(pgm_read_byte(bl0940_init + (6 * i) + j));  // Wrong byte order!
      }
      delay(1);
    }

  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Bl0940DrvInit(void) {
  if (PinUsed(GPIO_BL0940_RX) && PinUsed(GPIO_TXD)) {
    Bl0940.rx_buffer = (uint8_t*)(malloc(BL0940_BUFFER_SIZE));
    if (Bl0940.rx_buffer != nullptr) {
      TasmotaGlobal.energy_driver = XNRG_14;
    }
  }
}

bool Bl0940Command(void) {
  bool serviced = true;

  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl0940.power) {
      Settings->energy_power_calibration = (Bl0940.power * 100) / value;
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl0940.voltage) {
      Settings->energy_voltage_calibration = (Bl0940.voltage * 100) / value;
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl0940.current) {
      Settings->energy_current_calibration = Bl0940.current / value;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

void Bl0940Show(bool json) {
  if (json) {
    ResponseAppend_P(JSON_SNS_F_TEMP, "BL0940", Settings->flag2.temperature_resolution, &Bl0940.temperature);
    if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
      DomoticzFloatSensor(DZ_TEMP, Bl0940.temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      KnxSensor(KNX_TEMPERATURE, Bl0940.temperature);
#endif // USE_KNX
    }
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("", Bl0940.temperature);
#endif  // USE_WEBSERVER

  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg14(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (Bl0940Serial) { Bl0940SerialInput(); }
      break;
    case FUNC_EVERY_SECOND:
      Bl0940EverySecond();
      break;
    case FUNC_JSON_APPEND:
      Bl0940Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Bl0940Show(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_COMMAND:
      result = Bl0940Command();
      break;
    case FUNC_INIT:
      Bl0940SnsInit();
      break;
    case FUNC_PRE_INIT:
      Bl0940DrvInit();
      break;
  }
  return result;
}

#endif  // USE_BL0940
#endif  // USE_ENERGY_SENSOR
