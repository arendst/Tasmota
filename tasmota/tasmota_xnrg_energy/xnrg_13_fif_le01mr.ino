/*
  xnrg_13_fif_le01mr.ino - F&F LE-01MR energy meter with Modbus interface - support for Tasmota

  Copyright (C) 2021  Przemyslaw Wistuba

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
#ifdef USE_LE01MR
/*********************************************************************************************\
 * F&F LE-01MR - This is a single phase energy meter with rs485 modbus interface
 *               (and bidirectional energy counting - enabled by RS485).
 * It measure: Active energy imported AE+ [kWh] , Reactive energy imported RE+ [kvarh],
 *             Voltage V [V], Current I [A], Frequency F [Hz], power factor (aka "cos-phi"),
 *             Active power P [kW], Reactive power Q [kVAr], Apparent power S [kVA],
 *             *Active energy exported AE- [kWh] (when meter is switched to bi-directional counting then
 *                  reactive energy imported register contains value of Active energy exported).
 *
 * Meter descriptions at manufacturer page (english version have some description errors):
 *    EN: https://www.fif.com.pl/en/usage-electric-power-meters/517-electricity-consumption-meter-le-01mr.html
 *    PL: https://www.fif.com.pl/pl/liczniki-zuzycia-energii-elektrycznej/517-licznik-zuzycia-energii-le-01mr.html
 *
 * Note about communication settings: The meter must be reconfigured to use baudrate 2400 (or 9600) *without*
 *                                    parity bit - by default the meter is configured to 9600 8E1
 *                                    (Frame format: "EVEN 1") . To make those changes, use LE-Config
 *                                    software (can be found in download tab in product page - link above)
 *                                    and USB-RS485 dongle (those cheap ~2$ from ali works fine)
 *
 * Register descriptions (not all, only those that are being read):
 *
 * /----------------------------------- Register address
 * |        /-------------------------- Registers count
 * |        |   /---------------------- Datatype and size
 * |        |   |    /----------------- Resolution (or multiplier)
 * |        |   |    |      /---------- Unit
 * |        |   |    |      |     /---- Description
 * 0x0130   1   U16  0.01   Hz    Frequency
 * 0x0131   1   U16  0.01   V     Voltage
 * 0x0139   2   U32  0.001  A     Current
 * 0x0140   2   U32  0.001  kW    Active power
 * 0x0148   2   U32  0.001  kvar  Reactive power
 * 0x0150   2   U32  0.001  kVA   Apparent power
 * 0x0158   1   S16  0.001  -     Power factor
 * 0xA000   2   U32  0.01   kWh   Active energy imported
 * 0xA01E   2   U32  0.01   kvarh Reactive energy imported
 *
 * Datatype: S = signed int, U = unsigend int,
 *           U32 - the first (lower) register contains high word,
 *                 second register contains lower word of 32bit dword:
 *                   value_32bit = (register+0)<<16 | (register+1);
 *                   /or/ val32bit = (reg+0)*65536 + (reg+1);
 *
 * Note about MQTT/JSON: In fields "ENERGY.TotalActive" and "ENERGY.TotalReactive" there are
 *                        counters values directly from the meter (without Tasmota calculation,
 *                        energy used calculated by Tasmota is in Total/Today fields ).
 *                       Filed "ENERGY.Period" is always zero.
\*********************************************************************************************/

#define XNRG_13             13

// can be user defined in my_user_config.h
#ifndef LE01MR_SPEED
  #define LE01MR_SPEED      2400    // default LE01MR Modbus speed
#endif
// can be user defined in my_user_config.h
#ifndef LE01MR_ADDR
  #define LE01MR_ADDR       1       // default LE01MR Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *FifLEModbus;

const uint8_t le01mr_table_sz = 9;

const uint16_t le01mr_register_addresses[] {
            // IDX                               (reg count/datatype)    [unit]
  0x0130,   // 00 . LE01MR_FREQUENCY             (1/U16)    [Hz]
  0x0131,   // 01 . LE01MR_VOLTAGE               (1/U16)    [V]
  0x0158,   // 02 . LE01MR_POWER_FACTOR          (1/S16)
  0x0139,   // 03 . LE01MR_CURRENT               (2/U32)    [A]
  0x0140,   // 04 . LE01MR_ACTIVE_POWER          (2/U32)    [kW]
  0x0148,   // 05 . LE01MR_REACTIVE_POWER        (2/U32)    [kvar]
  0x0150,   // 06 . LE01MR_APPARENT_POWER        (2/U32)    [kVA]
  0xA000,   // 07 . LE01MR_TOTAL_ACTIVE_ENERGY   (2/U32)    [kWh]
  0xA01E    // 08 . LE01MR_TOTAL_REACTIVE_ENERGY (2/U32)    [kvarh]
};

struct LE01MR {
  float total_active = 0;
  float total_reactive = 0;
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
  uint8_t start_address_count = le01mr_table_sz;
} Le01mr;

/*********************************************************************************************/

void FifLEEvery250ms(void)
{
  bool data_ready = FifLEModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 9
    uint8_t reg_count = 2;
    if (Le01mr.read_state < 3) {
      reg_count=1;
    }

    uint32_t error = FifLEModbus->ReceiveBuffer(buffer, reg_count);

    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, FifLEModbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("FiF-LE: LE01MR Modbus error %d"), error);
    } else {
      Energy->data_valid[0] = 0;

      // CA=Client Address, FC=Function Code, BC=Byte Count, B3..B0=Data byte, Ch Cl = crc16 checksum
      // U32 registers:
      // 00 01 02 03 04 05 06 07 08
      // CA FC BC B3 B2 B1 B0 Cl Ch
      // 01 03 04 00 00 00 72 7A 16  = REG[B3..B2=0x0139,B1..B0=0x013A]   114 = 0.114 A
      // 01 03 04 00 00 00 B0 FB 87  = REG[B3..B2=0xA01E,B1..B0=0xA01F]   176 = 1.76 kvarh
      // U16/S16 registers:
      // 00 01 02 03 04 05 06
      // CA FC BC B1 B0 Cl Ch
      // 01 03 02 5B 02 02 B5  = REG[B1..B0=0x0131]   23298 = 232.98 V
      // 01 03 02 03 E8 B8 FA  = REG[B1..B0=0x0158]   1000  = 1.000 (power factor)
      // there are 3 data types used:
      // U16 - uint16_t
      // U32 - uint32_t
      // S16 -  int16_t
      // everything drop into uint32 value, but depending on register ther will be 2 or 4 bytes
      uint32_t value_buff = 0;
      // for register table items 0..2 use 2 bytes (U16)
      if (Le01mr.read_state >= 0 &&  Le01mr.read_state < 3) { //
        value_buff = ((uint32_t)buffer[3])<<8 | buffer[4];
      } else {
        value_buff = ((uint32_t)buffer[3])<<24 | ((uint32_t)buffer[4])<<16 | ((uint32_t)buffer[5])<<8 | buffer[6];
      }

      switch(Le01mr.read_state) {
        case 0:
          Energy->frequency[0] = value_buff * 0.01f;  // 5000 => 50.00
          break;

        case 1:
          Energy->voltage[0] = value_buff * 0.01f;   // 23298 => 232.98 V
          break;

        case 2:
          Energy->power_factor[0] = ((int16_t)value_buff) * 0.001f; // 1000 => 1.000 //note: I never saw this negative...
          break;

        case 3:
          Energy->current[0] = value_buff * 0.001f; // 114 => 0.114 A
          break;

        case 4:
          Energy->active_power[0] = value_buff * 1.0f; // P [W]
          break;

        case 5:
          Energy->reactive_power[0] = value_buff * 1.0f; // Q [var]
          break;

        case 6:
          Energy->apparent_power[0] = value_buff * 1.0f; // S [VA]
          break;

        case 7:
          Energy->import_active[0] = value_buff * 0.01f; // [kWh]
          Le01mr.total_active = Energy->import_active[0];  // Useless
          break;

        case 8:
          Le01mr.total_reactive = value_buff * 0.01f; // [kvarh] 176 => 1.76
          break;
      }

      Le01mr.read_state++;
      if (Le01mr.read_state == Le01mr.start_address_count) {
        Le01mr.read_state = 0;

        EnergyUpdateTotal();
      }
    }
  } // end data ready

  if (0 == Le01mr.send_retry || data_ready) {
    uint8_t reg_count = 2;

    Le01mr.send_retry = 5;
    // some registers are 1reg in size
    if (Le01mr.read_state < 3) reg_count=1;
    // send request
    FifLEModbus->Send(LE01MR_ADDR, 0x03, le01mr_register_addresses[Le01mr.read_state], reg_count);
  } else {
    Le01mr.send_retry--;
  }
}

void FifLESnsInit(void)
{
  FifLEModbus = new TasmotaModbus(Pin(GPIO_LE01MR_RX), Pin(GPIO_LE01MR_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = FifLEModbus->Begin(LE01MR_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void FifLEDrvInit(void)
{
  if (PinUsed(GPIO_LE01MR_RX) && PinUsed(GPIO_LE01MR_TX)) {
    TasmotaGlobal.energy_driver = XNRG_13;
  }
}

void FifLEReset(void)
{
  Le01mr.total_active = 0;
  Le01mr.total_reactive = 0;
}

void FifLEShow(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_TOTAL_ACTIVE "\":%s"), EnergyFmt(&Le01mr.total_active, Settings->flag2.energy_resolution));
    ResponseAppend_P(PSTR(",\"" D_JSON_TOTAL_REACTIVE "\":%s"), EnergyFmt(&Le01mr.total_reactive, Settings->flag2.energy_resolution));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_TOTAL_ACTIVE, WebEnergyFmt(&Le01mr.total_active, Settings->flag2.energy_resolution));
    WSContentSend_PD(HTTP_SNS_TOTAL_REACTIVE, WebEnergyFmt(&Le01mr.total_reactive, Settings->flag2.energy_resolution));
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg13(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      FifLEEvery250ms();
      break;
    case FUNC_JSON_APPEND:
      FifLEShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
      FifLEShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_ENERGY_RESET:
      FifLEReset();
      break;
    case FUNC_INIT:
      FifLESnsInit();
      break;
    case FUNC_PRE_INIT:
      FifLEDrvInit();
      break;
  }
  return result;
}

#endif  // USE_LE01MR
#endif  // USE_ENERGY_SENSOR
