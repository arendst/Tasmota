/*
  xnrg_14_bl09xx.ino - BL09XX energy sensor support for Tasmota

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
#if defined(USE_BL0940) || defined(USE_BL09XX)
#ifdef USE_BL0940
#warning **** USE_BL0940 is obsolete. Please replace with USE_BLE09XX ****
#endif
/*********************************************************************************************\
 * Support the following Shangai Belling energy sensors:
 *
 * BL0940 - Energy (as in Blitzwolf SHP10)
 * Template {"NAME":"BW-SHP10","GPIO":[0,148,0,207,158,21,0,0,0,17,0,0,0],"FLAG":0,"BASE":18}
 * Based on datasheet from http://www.belling.com.cn/media/file_object/bel_product/BL09XX/datasheet/BL09XX_V1.1_en.pdf
 *
 * BL0939 - Energy (as in Sonoff Dual R3 v2)
 * {"NAME":"Sonoff Dual R3 v2","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,3200,8128,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * Based on datasheet from https://www.belling.com.cn/product_info.html?id=368
 * See https://github.com/arendst/Tasmota/discussions/10793
 \*********************************************************************************************/

#define XNRG_14                     14

#define BL0939_PREF                 713       // =(4046*1*0,51*1000)/(1,218*1,218*(390*5+0,51)) = 713,105
#define BL0939_UREF                 17159     // =(79931*0,51*1000)/(1,218*(390*5+0,51)) = 17158,92
#define BL0939_IREF                 266013    // =(324004*1)/1,218 = 266013,14

#define BL0940_PREF                 1430
#define BL0940_UREF                 33000
#define BL0940_IREF                 275000

#define BL09XX_PULSES_NOT_INITIALIZED  -1

#define BL09XX_BUFFER_SIZE          36

#define BL0939_MODEL                39
#define BL0940_MODEL                40

#define BL0939_ADDRESS              0x05
#define BL0940_ADDRESS              0x00

#define BL09XX_WRITE_COMMAND        0xA0  // 0xA8 according to documentation
#define BL09XX_REG_I_FAST_RMS_CTRL  0x10
#define BL09XX_REG_MODE             0x18
#define BL09XX_REG_SOFT_RESET       0x19
#define BL09XX_REG_USR_WRPROT       0x1A
#define BL09XX_REG_TPS_CTRL         0x1B

#define BL09XX_READ_COMMAND         0x50  // 0x58 according to documentation
#define BL09XX_FULL_PACKET          0xAA

#define BL09XX_PACKET_HEADER        0x55  // 0x58 according to documentation

#include <TasmotaSerial.h>

TasmotaSerial *Bl09XXSerial = nullptr;

struct BL09XX {
  long voltage = 0;
  long current[2] = { 0, };
  long power[2] = { 0, };
  long power_cycle_first = 0;
  long cf_pulses[2] = { 0, };
  long cf_pulses_last_time[2] = { BL09XX_PULSES_NOT_INITIALIZED, BL09XX_PULSES_NOT_INITIALIZED};
  float temperature;
  int byte_counter = 0;
  uint16_t tps1 = 0;
  uint8_t *rx_buffer = nullptr;
  uint8_t address;
  uint8_t model;
  bool received = false;
} Bl09XX;

const uint8_t bl09xx_init[5][6] = {
  { BL09XX_WRITE_COMMAND, BL09XX_REG_SOFT_RESET,      0x5A, 0x5A, 0x5A, 0x38 },   // Reset to default
  { BL09XX_WRITE_COMMAND, BL09XX_REG_USR_WRPROT,      0x55, 0x00, 0x00, 0xF0 },   // Enable User Operation Write
  { BL09XX_WRITE_COMMAND, BL09XX_REG_MODE,            0x00, 0x10, 0x00, 0x37 },   // 0x0100 = CF_UNABLE energy pulse, AC_FREQ_SEL 50Hz, RMS_UPDATE_SEL 800mS
  { BL09XX_WRITE_COMMAND, BL09XX_REG_TPS_CTRL,        0xFF, 0x47, 0x00, 0xFE },   // 0x47FF = Over-current and leakage alarm on, Automatic temperature measurement, Interval 100mS
  { BL09XX_WRITE_COMMAND, BL09XX_REG_I_FAST_RMS_CTRL, 0x1C, 0x18, 0x00, 0x1B }};  // 0x181C = Half cycle, Fast RMS threshold 6172

void Bl09XXReceived(void) {
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
  // Sample from BL0940 (single channel)
  // 55 F2 03 00 00 00 00 7E 02 00 D4 B0 72 AC 01 00 00 00 00 02 01 00 00 00 00 00 00 00 BA 01 00 FE 03 00 83
  // 55 88 02 00 49 00 00 FE 02 00 AF EF 71 D2 01 00 EB FF FF 49 01 00 00 00 00 02 00 00 CF 01 00 FE 03 00 9F
  // 55 B9 33 00 DE 45 00 94 02 00 CF E4 70 63 02 00 6C 4C 00 13 01 00 09 00 00 00 00 00 E4 01 00 FE 03 00 72
  // 55 B8 55 00 2F 73 00 D2 02 00 00 C6 74 F9 01 00 97 89 00 37 01 00 AB 00 00 2D 00 00 02 02 00 FE 03 00 6E = U 7652864, I 29487/0, P 35223/0, C 171/0, T 514
  // Hd IFRms--- Current- Reserved Voltage- Reserved Power--- Reserved CF------ Reserved TPS1---- TPS2---- Ck
  //
  // Sample from BL0939 (dual channel)
  // 55 82 03 00 00 00 00 1E 15 01 65 80 3E E5 C6 00 00 00 00 50 B1 00 00 00 00 00 00 00 F9 01 00 FE 03 00 D2 = U 4096101, I 0/70942, P 0/45392, C 0/0, T 505
  // 55 E6 02 00 00 00 00 37 15 01 0F 83 3E F4 C7 00 00 00 00 69 B1 00 00 00 00 01 00 00 FA 01 00 FE 03 00 7E = U 4096783, I 0/70967, P 0/45417, C 0/1, T 506
  // 55 29 03 00 00 00 00 27 15 01 3A 86 3E AF C8 00 00 00 00 67 B1 00 00 00 00 01 00 00 FA 01 00 FE 03 00 62 = U 4097594, I 0/70951, P 0/45415, C 0/1, T 506
  // 55 04 03 00 00 00 00 D6 14 01 7D 8E 3E 25 C7 00 00 00 00 53 B1 00 00 00 00 01 00 00 F9 01 00 FE 03 00 2E = U 4099709, I 0/70870, P 0/45395, C 0/1, T 505
  // Hd IFRms-A- CurrentA CurrentB Voltage- IFRms-B- PowerA-- PowerB-- CF-A---- CF-B---- TPS1---- TPS2---- Ck

  uint16_t tps1 = Bl09XX.rx_buffer[29] << 8 | Bl09XX.rx_buffer[28];                                      // TPS1 unsigned
  if ((Bl09XX.rx_buffer[0] != BL09XX_PACKET_HEADER) ||                                                   // Bad header
      (Bl09XX.tps1 && ((tps1 < (Bl09XX.tps1 -10)) || (tps1 > (Bl09XX.tps1 +10))))                        // Invalid temperature change
     ) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL9: Invalid data"));
    return;
  }

  Bl09XX.tps1 = tps1;
  float t = ((170.0f/448.0f)*(((float)Bl09XX.tps1/2.0f)-32.0f))-45.0f;
  Bl09XX.temperature = ConvertTemp(t);

  Bl09XX.voltage    = Bl09XX.rx_buffer[12] << 16 | Bl09XX.rx_buffer[11] << 8 | Bl09XX.rx_buffer[10];        // V_RMS unsigned

  int32_t tmp;
  Bl09XX.current[0] = Bl09XX.rx_buffer[6]  << 16 | Bl09XX.rx_buffer[5]  << 8 | Bl09XX.rx_buffer[4];         // IA_RMS unsigned
  tmp = Bl09XX.rx_buffer[18] << 24 | Bl09XX.rx_buffer[17] << 16 | Bl09XX.rx_buffer[16] << 8;                // WATT_A signed
  Bl09XX.power[0] = abs(tmp >> 8);                                                                          // WATT_A unsigned
  tmp = Bl09XX.rx_buffer[24] << 24 | Bl09XX.rx_buffer[23] << 16 | Bl09XX.rx_buffer[22] << 8;                // CFA_CNT signed
  Bl09XX.cf_pulses[0] = abs(tmp >> 8);                                                                      // CFA_CNT unsigned

  if (Energy.phase_count > 1) {
    Bl09XX.current[1] = Bl09XX.rx_buffer[9]  << 16 | Bl09XX.rx_buffer[8]  << 8 | Bl09XX.rx_buffer[7];       // IB_RMS unsigned
    tmp = Bl09XX.rx_buffer[21] << 24 | Bl09XX.rx_buffer[20] << 16 | Bl09XX.rx_buffer[19] << 8;              // WATT_B signed
    Bl09XX.power[1] = abs(tmp >> 8);                                                                        // WATT_B unsigned
    tmp = Bl09XX.rx_buffer[27] << 24 | Bl09XX.rx_buffer[26] << 16 | Bl09XX.rx_buffer[25] << 8;              // CFB_CNT signed
    Bl09XX.cf_pulses[1] = abs(tmp >> 8);                                                                    // CFB_CNT unsigned
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL9: U %d, I %d/%d, P %d/%d, C %d/%d, T %d"),
    Bl09XX.voltage, Bl09XX.current[0], Bl09XX.current[1], Bl09XX.power[0], Bl09XX.power[1], Bl09XX.cf_pulses[0], Bl09XX.cf_pulses[1], Bl09XX.tps1);

  if (Energy.power_on) {  // Powered on
    Energy.voltage[0] = (float)Bl09XX.voltage / Settings->energy_voltage_calibration;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL9: Voltage %f, Temp %f"), Energy.voltage[0], Bl09XX.temperature);
    for (uint32_t chan = 0; chan < Energy.phase_count; chan++) {
      if (Bl09XX.power[chan] > Settings->energy_power_calibration) {                                   // We need at least 1W
        Energy.active_power[chan] = (float)Bl09XX.power[chan] / Settings->energy_power_calibration;
        Energy.current[chan] = (float)Bl09XX.current[chan] / Settings->energy_current_calibration;
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL9: Chan[%d] I %f, P %f"), chan, Energy.current[chan], Energy.active_power[chan]);
      } else {
        Energy.active_power[chan] = 0;
        Energy.current[chan] = 0;
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL9: Chan[%d] I zero, P zero"), chan);
      }
    }
  } else {  // Powered off
    //    Bl09XX.power_cycle_first = 0;
    Energy.voltage[0] = 0;
    Energy.active_power[0] = Energy.active_power[1] = 0;
    Energy.current[0] = Energy.current[1] = 0;
  }
}

void Bl09XXSerialInput(void) {
  while (Bl09XXSerial->available()) {
    yield();
    uint8_t serial_in_byte = Bl09XXSerial->read();
    if (!Bl09XX.received && (BL09XX_PACKET_HEADER == serial_in_byte)) {
      Bl09XX.received = true;
      Bl09XX.byte_counter = 0;
    }
    if (Bl09XX.received) {
      Bl09XX.rx_buffer[Bl09XX.byte_counter++] = serial_in_byte;
      if (BL09XX_BUFFER_SIZE == Bl09XX.byte_counter) {

        AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Bl09XX.rx_buffer, BL09XX_BUFFER_SIZE -1);

        uint8_t checksum = BL09XX_READ_COMMAND | Bl09XX.address;
        for (uint32_t i = 0; i < BL09XX_BUFFER_SIZE -2; i++) { checksum += Bl09XX.rx_buffer[i]; }
        checksum ^= 0xFF;
        if (checksum == Bl09XX.rx_buffer[34]) {
          Energy.data_valid[0] = 0;
          Bl09XXReceived();
          Bl09XX.received = false;
          return;
        } else {
          //AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: " D_CHECKSUM_FAILURE "received 0x%02X instead of 0x%02X"), Bl09XX.rx_buffer[34], checksum);
          do {  // Sync buffer with data (issue #1907 and #3425)
            memmove(Bl09XX.rx_buffer, Bl09XX.rx_buffer +1, BL09XX_BUFFER_SIZE -1);
            Bl09XX.byte_counter--;
          } while ((Bl09XX.byte_counter > 1) && (BL09XX_PACKET_HEADER != Bl09XX.rx_buffer[0]));
          if (BL09XX_PACKET_HEADER != Bl09XX.rx_buffer[0]) {
            //AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: " D_CHECKSUM_FAILURE));
            Bl09XX.received = false;
            Bl09XX.byte_counter = 0;
          }
        }
      }
    }
  }
}

/********************************************************************************************/

void Bl09XXEverySecond(void) {
  if (Energy.data_valid[0] > ENERGY_WATCHDOG) {
    Bl09XX.voltage = 0;
    memset(Bl09XX.current, 0, sizeof(Bl09XX.current));
    memset(Bl09XX.power, 0, sizeof(Bl09XX.power));
  } else {
    // Calculate energy by using active power
    uint32_t energy_sum = 0;
    for (uint32_t channel = 0; channel < Energy.phase_count; channel++) {
      energy_sum += (Energy.active_power[channel] * 1000);
    }
    if (energy_sum) {
      Energy.kWhtoday_delta += energy_sum / 36;
      EnergyUpdateToday();
    }
/*
    // Calculate energy by using active energy pulse count
    bool update_today = false;
    for (int chan = 0 ; chan < Energy.phase_count ; chan++ ) {
      if (BL09XX_PULSES_NOT_INITIALIZED == Bl09XX.cf_pulses_last_time[chan]) {
        Bl09XX.cf_pulses_last_time[chan] = Bl09XX.cf_pulses[chan];  // Init after restart
      } else {
        uint32_t cf_pulses = 0;
        if (Bl09XX.cf_pulses[chan] < Bl09XX.cf_pulses_last_time[chan]) {  // Rolled over after 0xFFFFFF (16777215) pulses
          cf_pulses = (0x1000000 - Bl09XX.cf_pulses_last_time[chan]) + Bl09XX.cf_pulses[chan];
        } else {
          cf_pulses = Bl09XX.cf_pulses[chan] - Bl09XX.cf_pulses_last_time[chan];
        }
        if (cf_pulses && Energy.active_power[chan])  {
          uint32_t watt256 = (1638400 * 256) / Settings->energy_power_calibration;
          uint32_t delta = (cf_pulses * watt256) / 36;
          if (delta <= (4000 * 1000 / 36)) {  // max load for SHP10: 4.00kW (3.68kW)
            Bl09XX.cf_pulses_last_time[chan] = Bl09XX.cf_pulses[chan];
          } else {
            AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: Overload [%d] %d"), chan, delta);
            Bl09XX.cf_pulses_last_time[chan] = BL09XX_PULSES_NOT_INITIALIZED;
          }
          update_today = true;
        }
      }
    }
    if (update_today)
      EnergyUpdateToday();
*/
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BL9: Poll"));

  Bl09XXSerial->flush();
  Bl09XXSerial->write(BL09XX_READ_COMMAND | Bl09XX.address);
  Bl09XXSerial->write(BL09XX_FULL_PACKET);
}

void Bl09XXSnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  int rx_pin = Pin((BL0939_MODEL == Bl09XX.model) ? GPIO_BL0939_RX : GPIO_BL0940_RX);
  Bl09XXSerial = new TasmotaSerial(rx_pin, Pin(GPIO_TXD), 1);
  if (Bl09XXSerial->begin(4800, 1)) {
    if (Bl09XXSerial->hardwareSerial()) {
      ClaimSerial();
    }
    if (HLW_UREF_PULSE == Settings->energy_voltage_calibration) {
      Settings->energy_voltage_calibration = (BL0939_MODEL == Bl09XX.model) ? BL0939_UREF : BL0940_UREF;
      Settings->energy_current_calibration = (BL0939_MODEL == Bl09XX.model) ? BL0939_IREF : BL0940_IREF;
      Settings->energy_power_calibration = (BL0939_MODEL == Bl09XX.model) ? BL0939_PREF : BL0940_PREF;
    }
    if ((BL0940_MODEL == Bl09XX.model) && (Settings->energy_current_calibration < (BL0940_IREF / 20))) {
      Settings->energy_current_calibration *= 100;
    }

    Energy.use_overtemp = true;                 // Use global temperature for overtemp detection

    for (uint32_t i = 0; i < 5; i++) {
      Bl09XXSerial->write(bl09xx_init[i][0] | Bl09XX.address);
      for (uint32_t j = 1; j < 6; j++) {
        Bl09XXSerial->write(bl09xx_init[i][j]);
//        Bl09XXSerial->write(pgm_read_byte(bl09xx_init + (6 * i) + j));  // Wrong byte order!
      }
      delay(1);
    }

  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Bl09XXDrvInit(void) {
  if (PinUsed(GPIO_BL0939_RX) && PinUsed(GPIO_TXD)) {
    Bl09XX.model = BL0939_MODEL;
    Bl09XX.address = BL0939_ADDRESS;
  } else if (PinUsed(GPIO_BL0940_RX) && PinUsed(GPIO_TXD)) {
    Bl09XX.model = BL0940_MODEL;
    Bl09XX.address = BL0940_ADDRESS;
  }
  if (Bl09XX.model) {
    Bl09XX.rx_buffer = (uint8_t*)(malloc(BL09XX_BUFFER_SIZE));
    if (Bl09XX.rx_buffer != nullptr) {
      Energy.voltage_common = true;               // Use common voltage
      Energy.frequency_common = true;             // Use common frequency
      Energy.use_overtemp = true;                 // Use global temperature for overtemp detection
      Energy.phase_count = (BL0939_MODEL == Bl09XX.model) ? 2 : 1;  // Handle two channels as two phases
      TasmotaGlobal.energy_driver = XNRG_14;
    }
  }
}

bool Bl09XXCommand(void) {
  bool serviced = true;

  uint32_t channel = (2 == XdrvMailbox.index) && (Energy.phase_count > 1) ? 1 : 0;
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl09XX.power[channel]) {
      Settings->energy_power_calibration = (Bl09XX.power[channel] * 100) / value;
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl09XX.voltage) {
      Settings->energy_voltage_calibration = (Bl09XX.voltage * 100) / value;
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Bl09XX.current[channel]) {
      Settings->energy_current_calibration = (Bl09XX.current[channel] * 100) / value;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

void Bl09XXShow(bool json) {
  if (json) {
    ResponseAppend_P(JSON_SNS_F_TEMP, "BL09XX", Settings->flag2.temperature_resolution, &Bl09XX.temperature);
    if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
      DomoticzFloatSensor(DZ_TEMP, Bl09XX.temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      KnxSensor(KNX_TEMPERATURE, Bl09XX.temperature);
#endif // USE_KNX
    }
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("", Bl09XX.temperature);
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
      if (Bl09XXSerial) { Bl09XXSerialInput(); }
      break;
    case FUNC_EVERY_SECOND:
      Bl09XXEverySecond();
      break;
    case FUNC_JSON_APPEND:
      Bl09XXShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Bl09XXShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_COMMAND:
      result = Bl09XXCommand();
      break;
    case FUNC_INIT:
      Bl09XXSnsInit();
      break;
    case FUNC_PRE_INIT:
      Bl09XXDrvInit();
      break;
  }
  return result;
}

#endif  // USE_BL09XX
#endif  // USE_ENERGY_SENSOR