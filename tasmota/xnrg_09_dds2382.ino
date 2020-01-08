/*
  xnrg_09_dds2382.ino - Hiking DDS238-2 Modbus energy meter support for Tasmota

  Copyright (C) 2020  Matteo Campanella - based on the work of Gennaro Tortone

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
#ifdef USE_DDS2382
/*********************************************************************************************\
 * Hiking DDS238-2 Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XNRG_09            9

#ifndef DDS2382_SPEED
#define DDS2382_SPEED      9600    // default dds2382 Modbus address
#endif
#ifndef DDS2382_ADDR
#define DDS2382_ADDR       1       // default dds2382 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Dds2382Modbus;

uint8_t Dds2382_send_retry = 0;

void Dds2382EverySecond(void)
{
  bool data_ready = Dds2382Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[46];  // At least 5 + (2 * 18) = 41

    uint32_t error = Dds2382Modbus->ReceiveBuffer(buffer, 18);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Dds2382Modbus->ReceiveCount());

    if (error) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "DDS2382 response error %d"), error);
    } else {
      Energy.data_valid[0] = 0;

      //           0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F    10    11           = ModBus register
      //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40  = Buffer index
      // 01 03 24 00 00 1C 7B 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 F3 00 00 17 88 09 77 01 A6 03 F8 00 70 03 E1 13 8A 63 ED
      // 01 03 24 00 00 1C AD 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 F3 00 00 17 BA 09 77 01 F1 04 AF 00 6C 03 E3 13 86 41 A2
      // SA FC BC EnergyTotal             ExportActiv ImportActiv                         Volta Curre APowe RPowe PFact Frequ Crc--  = DDS238-2 ZN/S version 1 (#6384)
      // SA FC BC EnergyTotal                                     ExportActiv ImportActiv Volta Curre APowe RPowe PFact Frequ Crc--  = DDS238-2 ZN/S version 2 (#6531)

      // {"TotalStartTime":"2020-01-08T09:43:05","Total":0.060,"Yesterday":0.001,"Today":0.001,"ExportActive":12.670,"Period":0,"Power":1016,"ApparentPower":1020,"ReactivePower":112,"Factor":0.99,"Frequency":50,"Voltage":242,"Current":4.210}}
      // {"TotalStartTime":"2020-01-08T00:00:00","Total":0.061,"Yesterday":0.001,"Today":0.001,"ExportActive":12.670,"Period":0.020,"Power":1199.000,"ApparentPower":1204.231,"ReactivePower":108.000,"Factor":1.00,"Frequency":49.98,"Voltage":242.3,"Current":4.970}}

      Energy.voltage[0] = (float)((buffer[27] << 8) + buffer[28]) / 10.0;
      Energy.current[0] = (float)((buffer[29] << 8) + buffer[30]) / 100.0;
      Energy.active_power[0] = (float)((buffer[31] << 8) + buffer[32]);
      Energy.reactive_power[0] = (float)((buffer[33] << 8) + buffer[34]);
      Energy.power_factor[0] = (float)((buffer[35] << 8) + buffer[36]) / 1000.0;                                          // 1.00
      Energy.frequency[0] = (float)((buffer[37] << 8) + buffer[38]) / 100.0;                                              // 50.0 Hz
      uint8_t offset = 11;
      if (Settings.flag3.dds2382_model) {  // SetOption71 - Select different Modbus registers for Active Energy (#6531)
        offset = 19;
      }
      Energy.export_active = (float)((buffer[offset] << 24) + (buffer[offset +1] << 16) + (buffer[offset +2] << 8) + buffer[offset +3]) / 100.0;    // 429496.729 kW
      float import_active = (float)((buffer[offset +4] << 24) + (buffer[offset +5] << 16) + (buffer[offset +6] << 8) + buffer[offset +7]) / 100.0;  // 429496.729 kW

      EnergyUpdateTotal(import_active, true);  // 484.708 kWh
    }
  } // end data ready

  if (0 == Dds2382_send_retry || data_ready) {
    Dds2382_send_retry = 5;
    Dds2382Modbus->Send(DDS2382_ADDR, 0x03, 0, 18);
  } else {
    Dds2382_send_retry--;
  }
}

void Dds2382SnsInit(void)
{
  Dds2382Modbus = new TasmotaModbus(pin[GPIO_DDS2382_RX], pin[GPIO_DDS2382_TX]);
  uint8_t result = Dds2382Modbus->Begin(DDS2382_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void Dds2382DrvInit(void)
{
  if ((pin[GPIO_DDS2382_RX] < 99) && (pin[GPIO_DDS2382_TX] < 99)) {
    energy_flg = XNRG_09;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg09(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      if (uptime > 4) { Dds2382EverySecond(); }
      break;
    case FUNC_INIT:
      Dds2382SnsInit();
      break;
    case FUNC_PRE_INIT:
      Dds2382DrvInit();
      break;
  }
  return result;
}

#endif  // USE_DDS2382
#endif  // USE_ENERGY_SENSOR
