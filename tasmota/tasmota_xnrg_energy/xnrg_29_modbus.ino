/*
  xnrg_29_modbus.ino - Generic Modbus energy meter support for Tasmota

  Copyright (C) 2022  Theo Arends

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
#ifdef USE_MODBUS_ENERGY
/*********************************************************************************************\
 * Generic Modbus energy meter
 *
 * - Supports single three phase device or three single phase devices of same model on bus.
 * - For easy configuration of modbus energy monitor device(s) use:
 *   - a rule file called modbus
 *   - a script using >y
 *   - a filesystem file called modbus.json
 *
 * See files configurations.md and value_pair_description.md in folder energy_modbus_configs
 *
 * Restrictions:
 * - Supports Modbus single and double integer registers in addition to floating point registers
 * - Max number of user defined registers is defined by one rule buffer (511 characters uncompressed, around 800 characters compressed)
 *
 * To do:
 * - Support all three rule slots
 * - Support other modbus register like integers
 *
 * Test set:
 * rule3 on file#modbus do {"Name":"GROWATT","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":[4110,4114,4118],"T":3,"F":-1},"Current":{"R":[4111,4115,4119],"T":3,"F":-1},"Power":{"R":[4112,4116,4120],"T":8,"F":-1},"Frequency":{"R":4109,"T":3,"F":-2},"Total":{"R":4124,"T":8,"F":-1},"User":[{"R":[4099,4103],"J":"VoltagePV","G":"Voltage PV","U":"V","D":21,"T":3,"F":-1},{"R":[4100,4104],"J":"CurrentPV","G":"Current PV","U":"A","D":22,"T":3,"F":-1},{"R":[4101,4105],"J":"PowerPV","G":"Power PV","U":"W","D":23,"T":8,"F":-1}]} endon
 * rule3 on file#modbus do {"Name":"2 x PZEM014","Baud":9600,"Config":"8N1","Address":[1,1],"Function":4,"Voltage":{"R":0,"T":3,"F":-1},"Current":{"R":1,"T":8,"F":-3},"Power":{"R":3,"T":8,"F":-1},"Factor":{"R":8,"T":3,"F":-2},"Frequency":{"R":7,"T":3,"F":-1},"Total":{"R":5,"T":8,"F":-3}} endon
 * rule3 on file#modbus do {"Name":"SDM230 test1","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":[70,70,70],"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test2","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":70,"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test3","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":70,"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test4","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test5","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":[0x004E,0x004E,0x004E],"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test6","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":[70,70,70],"Total":[342,342,342],"ExportActive":0x004A} endon
 * rule3 on file#modbus do {"Name":"SDM230 test7","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":[70,70,70],"Total":[342,342,342],"ExportActive":0x004A,"User":{"J":"PhaseAngle","G":"Phase Angle","R":0x0024,"U":"Deg","D":2}} endon
 * rule3 on file#modbus do {"Name":"SDM230 test8","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":[70,70,70],"Total":[342,342,342],"ExportActive":0x004A,"User":{"J":"PhaseAngle","G":"Phase Angle","R":[0x24,0x24,0x24],"U":"Deg","D":2}} endon
 *
 * rule3 on file#modbus do {"Name":"SDM120 test1","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x0048,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x004C,"J":"ImportReactive","G":"Import Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"PZEM014 test1","Baud":9600,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":3,"F":-1},"Current":{"R":1,"T":8,"F":-3},"Power":{"R":3,"T":8,"F":-1},"Factor":{"R":8,"T":3,"F":-2},"Frequency":{"R":7,"T":3,"F":-1},"Total":{"R":5,"T":8,"F":-3},"User":{"R":0,"J":"VoltageTest","G":"Voltage test","U":"V","D":21,"T":3,"F":-1}} endon
 *
 * rule3 on file#modbus do {"Name":"SDM230 test6","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":0,"M":1},"Current":{"R":6,"T":0,"M":1},"Power":{"R":12,"T":0,"M":1},"Frequency":70,"Total":342} endon
 * rule3 on file#modbus do {"Name":"SDM230 test6","Baud":2400,"Config":"8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":0,"F":0},"Current":{"R":6,"T":0,"F":0},"Power":{"R":12,"T":0,"F":0},"Frequency":70,"Total":342,"User":{"R":0x0048,"T":0,"F":-1,"J":"ImportActive","G":"Import Active","U":"kWh","D":24}} endon
\*********************************************************************************************/

#define XNRG_29                   29

#define ENERGY_MODBUS_SPEED       9600                 // Default Modbus baudrate
#define ENERGY_MODBUS_CONFIG      TS_SERIAL_8N1        // Default Modbus serial configuration
#define ENERGY_MODBUS_ADDR        1                    // Default Modbus device_address
#define ENERGY_MODBUS_FUNC        0x04                 // Default Modbus function code
#define ENERGY_MODBUS_MAX_DEVICES ENERGY_MAX_PHASES    // Support up to three single phase devices as three phases

#define ENERGY_MODBUS_DATATYPE    0                    // Default Modbus datatype is 4-byte float
#define ENERGY_MODBUS_DECIMALS    0                    // Default user decimal resolution

#define ENERGY_MODBUS_TICKER                           // Enable for ESP8266 when using softwareserial solving most modbus serial retries
#define ENERGY_MODBUS_TICKER_POLL 200                  // Modbus poll time in ms between read register requests

//#define ENERGY_MODBUS_DEBUG
//#define ENERGY_MODBUS_DEBUG_SHOW

#define ENERGY_MODBUS_FILE        "/modbus.json"       // Modbus parameter file name used by filesystem

const uint16_t nrg_mbs_reg_not_used = 0xFFFF;          // Odd number 65535 is unused register

// Even data type is single (2-byte) register, Odd data type is double (4-byte) registers
enum EnergyModbusDataType { NRG_DT_FLOAT,              // 0 = 4-byte float
                            NRG_DT_S16,                // 1 = 2-byte signed
                            NRG_DT_S32,                // 2 = 4-byte signed
                            NRG_DT_U16,                // 3 = 2-byte unsigned
                            NRG_DT_U32,                // 4 = 4-byte unsigned
                            NRG_DT_x16_nu1,            // 5 = 2-byte
                            NRG_DT_S32_SW,             // 6 = 4-byte signed with swapped words
                            NRG_DT_x16_nu2,            // 7 = 2-byte
                            NRG_DT_U32_SW,             // 8 = 4-byte unsigned with swapped words
                            NRG_DT_MAX };

enum EnergyModbusResolutions { NRG_RES_VOLTAGE = 21,   // 21 = V
                               NRG_RES_CURRENT,        // 22 = A
                               NRG_RES_POWER,          // 23 = W, VA, VAr
                               NRG_RES_ENERGY,         // 24 = kWh, kVAh, kVArh
                               NRG_RES_FREQUENCY,      // 25 = Hz
                               NRG_RES_TEMPERATURE,    // 26 = C, F
                               NRG_RES_HUMIDITY,       // 27 = %
                               NRG_RES_PRESSURE,       // 28 = hPa, mmHg
                               NRG_RES_WEIGHT };       // 29 = Kg

enum EnergyModbusRegisters { NRG_MBS_VOLTAGE,
                             NRG_MBS_CURRENT,
                             NRG_MBS_ACTIVE_POWER,
                             NRG_MBS_APPARENT_POWER,
                             NRG_MBS_REACTIVE_POWER,
                             NRG_MBS_POWER_FACTOR,
                             NRG_MBS_FREQUENCY,
                             NRG_MBS_TOTAL_ENERGY,
                             NRG_MBS_EXPORT_ACTIVE_ENERGY,
                             NRG_MBS_MAX_REGS };

const char kEnergyModbusValues[] PROGMEM = D_JSON_VOLTAGE "|"              // Voltage
                                           D_JSON_CURRENT "|"              // Current
                                           D_JSON_POWERUSAGE "|"           // Power
                                           D_JSON_APPARENT_POWERUSAGE "|"  // ApparentPower
                                           D_JSON_REACTIVE_POWERUSAGE "|"  // ReactivePower
                                           D_JSON_POWERFACTOR "|"          // Factor
                                           D_JSON_FREQUENCY "|"            // Frequency
                                           D_JSON_TOTAL "|"                // Total
                                           D_JSON_EXPORT_ACTIVE "|"        // ExportActive
                                           ;

#include <TasmotaModbus.h>
TasmotaModbus *EnergyModbus;

#ifdef ENERGY_MODBUS_TICKER
#include <Ticker.h>
Ticker ticker_energy_modbus;
#endif  // ENERGY_MODBUS_TICKER

struct NRGMBSPARAM {
  uint32_t serial_bps;
  uint32_t serial_config;
  uint16_t ticker_poll;
  uint8_t device_address[ENERGY_MODBUS_MAX_DEVICES];
  uint8_t devices;
  uint8_t function;
  uint8_t total_regs;
  uint8_t user_adds;
  uint8_t state;
  uint8_t retry;
  int8_t phase;
  bool mutex;
} NrgMbsParam;

typedef struct NRGMBSREGISTER {
  uint16_t address[ENERGY_MAX_PHASES];
  int16_t factor;
  uint32_t datatype;
} NrgMbsRegister_t;
NrgMbsRegister_t *NrgMbsReg = nullptr;

typedef struct NRGMBSUSER {
  float data[ENERGY_MAX_PHASES];
  char* json_name;
  char* gui_name;
  char* gui_unit;
  uint32_t resolution;
} NrgMbsUser_t;
NrgMbsUser_t *NrgMbsUser = nullptr;

/*********************************************************************************************/

void EnergyModbusLoop(void) {
#ifdef ENERGY_MODBUS_TICKER
  if (NrgMbsParam.mutex || TasmotaGlobal.ota_state_flag) { return; }
#else
  if (NrgMbsParam.mutex) { return; }
#endif  // ENERGY_MODBUS_TICKER
  NrgMbsParam.mutex = 1;

  uint32_t register_count;

  bool data_ready = EnergyModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[15];  // At least 5 + (2 * 2) = 9

    // Even data type is single register, Odd data type is double registers
    register_count = 2 - (NrgMbsReg[NrgMbsParam.state].datatype & 1);
    uint32_t error = EnergyModbus->ReceiveBuffer(buffer, register_count);
    if (error) {
      /* Return codes from TasmotaModbus.h:
      * 0 = No error
      * 1 = Illegal Function,
      * 2 = Illegal Data Address,
      * 3 = Illegal Data Value,
      * 4 = Slave Error
      * 5 = Acknowledge but not finished (no error)
      * 6 = Slave Busy
      * 7 = Not enough minimal data received
      * 8 = Memory Parity error
      * 9 = Crc error
      * 10 = Gateway Path Unavailable
      * 11 = Gateway Target device failed to respond
      * 12 = Wrong number of registers
      * 13 = Register data not specified
      * 14 = To many registers
      */
#ifdef ENERGY_MODBUS_DEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Modbus register %d, phase %d, rcvd %*_H"),
        NrgMbsParam.state, NrgMbsParam.phase, EnergyModbus->ReceiveCount(), buffer);
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus error %d"), error);
    } else {
      /* Modbus protocol format:
      * SA = Device Address
      * FC = Function Code
      * BC = Byte count
      * Fh = First or High word MSB
      * Fl = First or High word LSB
      * Sh = Second or Low word MSB
      * Sl = Second or Low word LSB
      * Cl = CRC lsb
      * Ch = CRC msb
      */
      Energy->data_valid[NrgMbsParam.phase] = 0;

      float value;
      switch (NrgMbsReg[NrgMbsParam.state].datatype) {
        case NRG_DT_FLOAT: {  // 0
          //  0  1  2  3  4  5  6  7  8
          // SA FC BC Fh Fl Sh Sl Cl Ch
          // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
          ((uint8_t*)&value)[3] = buffer[3];   // Get float values
          ((uint8_t*)&value)[2] = buffer[4];
          ((uint8_t*)&value)[1] = buffer[5];
          ((uint8_t*)&value)[0] = buffer[6];
          break;
        }
        case NRG_DT_S16: {  // 1
          //  0  1  2  3  4  5  6
          // SA FC BC Fh Fl Cl Ch
          int16_t value_buff = ((int16_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_U16: {  // 3
          //  0  1  2  3  4  5  6
          // SA FC BC Fh Fl Cl Ch
          uint16_t value_buff = ((uint16_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_S32: {  // 2
          //  0  1  2  3  4  5  6  7  8
          // SA FC BC Fh Fl Sh Sl Cl Ch
          int32_t value_buff = ((int32_t)buffer[3])<<24 | ((uint32_t)buffer[4])<<16 | ((uint32_t)buffer[5])<<8 | buffer[6];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_S32_SW: {  // 6
          //  0  1  2  3  4  5  6  7  8
          // SA FC BC Sh Sl Fh Fl Cl Ch
          int32_t value_buff = ((int32_t)buffer[5])<<24 | ((uint32_t)buffer[6])<<16 | ((uint32_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_U32: {  // 4
          //  0  1  2  3  4  5  6  7  8
          // SA FC BC Fh Fl Sh Sl Cl Ch
          uint32_t value_buff = ((uint32_t)buffer[3])<<24 | ((uint32_t)buffer[4])<<16 | ((uint32_t)buffer[5])<<8 | buffer[6];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_U32_SW: {  // 8
          //  0  1  2  3  4  5  6  7  8
          // SA FC BC Sh Sl Fh Fl Cl Ch
          // 01 04 04 EB EC 00 0E 8E 51 = 977.9000 (Solax protocol X1&X3)
          uint32_t value_buff = ((uint32_t)buffer[5])<<24 | ((uint32_t)buffer[6])<<16 | ((uint32_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
      }
      uint32_t factor = 1;
      // 1 = 10, 2 = 100, 3 = 1000, 4 = 10000
      uint32_t scaler = abs(NrgMbsReg[NrgMbsParam.state].factor);
      while (scaler) {
        factor *= 10;
        scaler--;
      }
      if (NrgMbsReg[NrgMbsParam.state].factor < 0) {
        value /= factor;
      } else {
        value *= factor;
      }

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Modbus register %d, phase %d, rcvd %*_H, T %d, F %d, value %4_f"),
        NrgMbsParam.state, NrgMbsParam.phase, EnergyModbus->ReceiveCount(), buffer,
        NrgMbsReg[NrgMbsParam.state].datatype, NrgMbsReg[NrgMbsParam.state].factor, &value);

      switch (NrgMbsParam.state) {
        case NRG_MBS_VOLTAGE:
          Energy->voltage[NrgMbsParam.phase] = value;          // 230.2 V
          break;
        case NRG_MBS_CURRENT:
          Energy->current[NrgMbsParam.phase]  = value;         // 1.260 A
          break;
        case NRG_MBS_ACTIVE_POWER:
          Energy->active_power[NrgMbsParam.phase] = value;     // -196.3 W
          break;
        case NRG_MBS_APPARENT_POWER:
          Energy->apparent_power[NrgMbsParam.phase] = value;   // 223.4 VA
          break;
        case NRG_MBS_REACTIVE_POWER:
          Energy->reactive_power[NrgMbsParam.phase] = value;   // 92.2
          break;
        case NRG_MBS_POWER_FACTOR:
          Energy->power_factor[NrgMbsParam.phase] = value;     // -0.91
          break;
        case NRG_MBS_FREQUENCY:
          Energy->frequency[NrgMbsParam.phase] = value;        // 50.0 Hz
          break;
        case NRG_MBS_TOTAL_ENERGY:
          Energy->import_active[NrgMbsParam.phase] = value;    // 6.216 kWh => used in EnergyUpdateTotal()
          break;
        case NRG_MBS_EXPORT_ACTIVE_ENERGY:
          Energy->export_active[NrgMbsParam.phase] = value;    // 478.492 kWh
          break;
        default:
          if (NrgMbsUser) {
            NrgMbsUser[NrgMbsParam.state - NRG_MBS_MAX_REGS].data[NrgMbsParam.phase] = value;
          }
      }
    }
  } // end data ready

  if (0 == NrgMbsParam.retry || data_ready) {
    NrgMbsParam.retry = 1;

    uint32_t address = 0;
    uint32_t phase = 0;
    do {
      NrgMbsParam.phase++;
      if (NrgMbsParam.phase >= Energy->phase_count) {
        NrgMbsParam.phase = 0;
        NrgMbsParam.state++;
        if (NrgMbsParam.state >= NrgMbsParam.total_regs) {
          NrgMbsParam.state = 0;
          NrgMbsParam.phase = 0;
          EnergyUpdateTotal();                 // update every cycle after all registers have been read
        }
      }
      delay(0);
      if (NrgMbsParam.devices == 1) {
        phase = NrgMbsParam.phase;
      } else {
        address = NrgMbsParam.phase;
      }
    } while (NrgMbsReg[NrgMbsParam.state].address[phase] == nrg_mbs_reg_not_used);

    // Even data type is single register, Odd data type is double registers
    register_count = 2 - (NrgMbsReg[NrgMbsParam.state].datatype & 1);

#ifdef ENERGY_MODBUS_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Modbus send Device %d, Function %d, Register %04X (%d/%d), Size %d"),
      NrgMbsParam.device_address[address], NrgMbsParam.function,
      NrgMbsReg[NrgMbsParam.state].address[phase], NrgMbsParam.state, phase,
      register_count);
#endif

    EnergyModbus->Send(NrgMbsParam.device_address[address], NrgMbsParam.function, NrgMbsReg[NrgMbsParam.state].address[phase], register_count);
  } else {
    NrgMbsParam.retry--;

#ifdef ENERGY_MODBUS_DEBUG
    if (NrgMbsParam.devices > 1) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus retry device %d state %d"), NrgMbsParam.device_address[NrgMbsParam.phase], NrgMbsParam.state);
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus retry state %d phase %d"), NrgMbsParam.state, NrgMbsParam.phase);
    }
#endif

  }
  delay(0);
  NrgMbsParam.mutex = 0;
}

uint32_t EnergyModbusReadRegisterInfo(JsonParserObject add_value, uint32_t reg_index) {
  // {"R":0,"T":0,"F":0}
  // {"R":[0,2,4],"T":0,"F":0}
  // {"R":[0,2,4],"T":0,"M":10} - [LEGACY]
  uint32_t phase = 0;
  JsonParserToken val;
  val = add_value[PSTR("R")];                    // Register address
  if (val.isArray()) {
    // [0,2,4]
    JsonParserArray address_arr = val.getArray();
    for (auto value : address_arr) {
      NrgMbsReg[reg_index].address[phase] = value.getUInt();
      phase++;
      if (phase >= ENERGY_MAX_PHASES) { break; }
    }
  } else if (val) {
    // 0
    NrgMbsReg[reg_index].address[0] = val.getUInt();
    phase++;
  }
  val = add_value[PSTR("T")];                    // Register data type
  if (val) {
    // 0
    NrgMbsReg[reg_index].datatype = val.getUInt();
  }
  val = add_value[PSTR("F")];                    // Register factor
  if (val) {
    // 1 or -2
    NrgMbsReg[reg_index].factor = val.getInt();
  }
  val = add_value[PSTR("M")];                    // [LEGACY] Register divider
  if (val) {
    // 1
    int32_t divider = val.getUInt();
    int factor = 0;
    while (divider > 1) {
      divider /= 10;
      factor--;
    }
    NrgMbsReg[reg_index].factor = factor;
  }
  return phase;
}

bool EnergyModbusReadUserRegisters(JsonParserObject user_add_value, uint32_t add_index) {
  // {"R":0x004E,"T":0,"F":0,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0}
  // {"R":[0,2,4],"T":0,"F":0,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0}
  uint32_t reg_index = NRG_MBS_MAX_REGS + add_index;

  // {"R":0,"T":0,"F":0}
  // {"R":[0,2,4],"T":0,"F":0}
  // {"R":[0,2,4],"T":0,"M":10} - [LEGACY]
  uint32_t phase = EnergyModbusReadRegisterInfo(user_add_value, reg_index);
  if (!phase) {
    return false;                                // No register entered so skip
  }
  if (phase > Energy->phase_count) {
    Energy->phase_count = phase;
    NrgMbsParam.devices = 1;                     // Only one device allowed with multiple phases
  }

  JsonParserToken val;
  val = user_add_value[PSTR("J")];               // JSON value name
  if (val) {
    NrgMbsUser[add_index].json_name = SetStr(val.getStr());
    char json_name[32];
    if (GetCommandCode(json_name, sizeof(json_name), NrgMbsUser[add_index].json_name, kEnergyModbusValues) > -1) {
      return false;                              // Duplicate JSON name
    }
  } else {
    return false;                                // No mandatory JSON name
  }
  val = user_add_value[PSTR("G")];               // GUI value name
  NrgMbsUser[add_index].gui_name = (val) ? SetStr(val.getStr()) : EmptyStr;
  val = user_add_value[PSTR("U")];               // GUI value Unit
  NrgMbsUser[add_index].gui_unit = (val) ? SetStr(val.getStr()) : EmptyStr;
  NrgMbsUser[add_index].resolution = ENERGY_MODBUS_DECIMALS;
  val = user_add_value[PSTR("D")];               // Decimal resolution
  if (val) {
    NrgMbsUser[add_index].resolution = val.getUInt();
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d (%s), R [%04X,%04X,%04X], T %d, F %d, J '%s', G '%s', U '%s', D %d"),
    reg_index, NrgMbsUser[add_index].json_name,
    NrgMbsReg[reg_index].address[0],
    NrgMbsReg[reg_index].address[1],
    NrgMbsReg[reg_index].address[2],
    NrgMbsReg[reg_index].datatype,
    NrgMbsReg[reg_index].factor,
    NrgMbsUser[add_index].json_name,
    NrgMbsUser[add_index].gui_name,
    NrgMbsUser[add_index].gui_unit,
    NrgMbsUser[add_index].resolution);
#endif

  return true;
}

bool EnergyModbusReadRegisters(void) {
  String modbus = "";

#ifdef USE_UFILESYS
  modbus = TfsLoadString(ENERGY_MODBUS_FILE);
  if (modbus.length()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Loaded from File"));
  }
#endif  // USE_UFILESYS

#ifdef USE_RULES
  if (!modbus.length()) {
    modbus = RuleLoadFile("MODBUS");
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Loaded from Rule"));
  }
#endif  // USE_RULES

#ifdef USE_SCRIPT
  if (!modbus.length()) {
    modbus = ScriptLoadSection(">y");
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Loaded from Script"));
  }
#endif  // USE_SCRIPT

  if (modbus.length() < 7) { return false; }     // File not found or Invalid JSON

//    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: File '%s'"), modbus.c_str());

  const char* json = modbus.c_str();
  uint32_t len = strlen(json) +1;
  char json_buffer[len];
  memcpy(json_buffer, json, len);                // Keep original safe
  JsonParser parser(json_buffer);
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }                   // Invalid JSON

  // Init defaults
  Energy->phase_count = 1;
  NrgMbsParam.serial_bps = ENERGY_MODBUS_SPEED;
  NrgMbsParam.serial_config = ENERGY_MODBUS_CONFIG;
  NrgMbsParam.ticker_poll = ENERGY_MODBUS_TICKER_POLL;
  NrgMbsParam.device_address[0] = ENERGY_MODBUS_ADDR;
  NrgMbsParam.devices = 1;
  NrgMbsParam.function = ENERGY_MODBUS_FUNC;
  NrgMbsParam.user_adds = 0;

  // Detect buffer allocation
  JsonParserToken val;
  val = root[PSTR("User")];
  if (val) {
    if (val.isArray()) {
      // [{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]
      NrgMbsParam.user_adds = val.size();
    } else {
      // {"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
      NrgMbsParam.user_adds = 1;
    }
  }
  NrgMbsParam.total_regs = NRG_MBS_MAX_REGS + NrgMbsParam.user_adds;
  NrgMbsReg = (NrgMbsRegister_t*)calloc(NrgMbsParam.total_regs, sizeof(NrgMbsRegister_t));
  if (NrgMbsReg == nullptr) { return false; }    // Unable to allocate variables on heap

  // Init defaults
  for (uint32_t i = 0; i < NrgMbsParam.total_regs; i++) {
    NrgMbsReg[i].datatype = ENERGY_MODBUS_DATATYPE;
    for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
      NrgMbsReg[i].address[j] = nrg_mbs_reg_not_used;
    }
  }
  if (NrgMbsParam.user_adds) {
    NrgMbsUser = (NrgMbsUser_t*)calloc(NrgMbsParam.user_adds +1, sizeof(NrgMbsUser_t));
    if (NrgMbsUser == nullptr) {
      NrgMbsParam.user_adds = 0;
      NrgMbsParam.total_regs = NRG_MBS_MAX_REGS;
    } else {
      // Init defaults
      for (uint32_t i = 0; i < NrgMbsParam.user_adds; i++) {
        NrgMbsUser[i].resolution = ENERGY_MODBUS_DECIMALS;
        for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
          NrgMbsUser[i].data[j] = NAN;
        }
      }
    }
  }

  // Get global parameters
  val = root[PSTR("Baud")];
  if (val) {
    NrgMbsParam.serial_bps = val.getInt();       // 2400
  }
  val = root[PSTR("Config")];
  if (val) {
    const char *serial_config = val.getStr();    // 8N1
    NrgMbsParam.serial_config = ConvertSerialConfig(ParseSerialConfig(serial_config));
  }
  val = root[PSTR("Poll")];
  if (val) {
    NrgMbsParam.ticker_poll = val.getUInt();     // 200
    if (NrgMbsParam.ticker_poll < 100) {         // Below 100 ms makes no sense as the comms usually is 9600bps
      NrgMbsParam.ticker_poll = ENERGY_MODBUS_TICKER_POLL;
    }
  }
  val = root[PSTR("Address")];
  if (val) {
    NrgMbsParam.devices = 0;
    if (val.isArray()) {
      // [1,2,3]
      JsonParserArray arr = val.getArray();
      for (auto value : arr) {
        NrgMbsParam.device_address[NrgMbsParam.devices] = value.getUInt();   // 1
        NrgMbsParam.devices++;
        if (NrgMbsParam.devices >= ENERGY_MODBUS_MAX_DEVICES) { break; }
      }
    } else if (val) {
      // 1
      NrgMbsParam.device_address[0] = val.getUInt();  // 1
      NrgMbsParam.devices++;
    }
  }
  val = root[PSTR("Function")];
  if (val) {
    NrgMbsParam.function = val.getUInt();        // 4
  }

  // Get default energy registers
  char register_name[32];
  Energy->voltage_available = false;              // Disable voltage is measured
  Energy->current_available = false;              // Disable current is measured
  for (uint32_t names = 0; names < NRG_MBS_MAX_REGS; names++) {
    val = root[GetTextIndexed(register_name, sizeof(register_name), names, kEnergyModbusValues)];
    if (val) {
      // "Voltage":0
      // "Voltage":[0,2,4]
      // "Voltage":{"R":0,"T":0,"F":0}
      // "Voltage":{"R":[0,2,4],"T":0,"F":0}
      uint32_t phase = 0;
      if (val.isObject()) {
        // {"R":0,"T":0,"F":0}
        // {"R":[0,2,4],"T":0,"F":0}
        // {"R":[0,2,4],"T":0,"M":10} - [LEGACY]
        phase = EnergyModbusReadRegisterInfo(val.getObject(), names);
      } else if (val.isArray()) {
        // [0,2,4]
        JsonParserArray arr = val.getArray();
        for (auto value : arr) {
          NrgMbsReg[names].address[phase] = value.getUInt();
          phase++;
          if (phase >= ENERGY_MAX_PHASES) { break; }
        }
      } else if (val) {
        // 0
        NrgMbsReg[names].address[0] = val.getUInt();
        phase++;
      }
      if (phase > Energy->phase_count) {
        Energy->phase_count = phase;
        NrgMbsParam.devices = 1;                 // Only one device allowed with multiple phases
      }

      switch(names) {
        case NRG_MBS_VOLTAGE:
          Energy->voltage_available = true;       // Enable if voltage is measured
          if (1 == phase) {
            Energy->voltage_common = true;        // Use common voltage
          }
          break;
        case NRG_MBS_CURRENT:
          Energy->current_available = true;       // Enable if current is measured
          break;
        case NRG_MBS_FREQUENCY:
          if (1 == phase) {
            Energy->frequency_common = true;      // Use common frequency
          }
          break;
        case NRG_MBS_TOTAL_ENERGY:
          Settings->flag3.hardware_energy_total = 1;  // SetOption72 - Enable hardware energy total counter as reference (#6561)
          break;
      }

#ifdef ENERGY_MODBUS_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d (%s), R [%04X,%04X,%04X], T %d, F %d"),
        names, register_name,
        NrgMbsReg[names].address[0],
        NrgMbsReg[names].address[1],
        NrgMbsReg[names].address[2],
        NrgMbsReg[names].datatype,
        NrgMbsReg[names].factor);
#endif

    }
  }

  // Get user defined registers
  // "User":{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0}
  // "User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2,"T":0,"F":0}]
  val = root[PSTR("User")];
  if (val) {
    if (val.isArray()) {
      // [{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2,"T":0,"F":0}]
      JsonParserArray user_adds_arr = val.getArray();
      uint32_t add_index = 0;
      for (auto user_add_values : user_adds_arr) {
        if (!user_add_values.isObject()) { break; }
        if (EnergyModbusReadUserRegisters(user_add_values.getObject(), add_index)) {
          add_index++;
        } else {
          AddLog(LOG_LEVEL_INFO, PSTR("NRG: Dropped JSON user input %d"), add_index +1);
          NrgMbsParam.user_adds--;
        }
      }
    } else if (val) {
      // {"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3,"T":0,"F":0}
      if (val.isObject()) {
        if (!EnergyModbusReadUserRegisters(val.getObject(), 0)) {
          AddLog(LOG_LEVEL_INFO, PSTR("NRG: Dropped JSON user input"));
          NrgMbsParam.user_adds--;
        }
      }
    }
    NrgMbsParam.total_regs = NRG_MBS_MAX_REGS + NrgMbsParam.user_adds;
  }

  // Fix variable boundaries
  for (uint32_t i = 0; i < NrgMbsParam.total_regs; i++) {
    if (NrgMbsReg[i].datatype >= NRG_DT_MAX) {
      NrgMbsReg[i].datatype = ENERGY_MODBUS_DATATYPE;
    }
  }
  if (NrgMbsParam.devices > 1) {
    // Multiple devices have no common values
    Energy->phase_count = NrgMbsParam.devices;
    Energy->voltage_common = false;        // Use no common voltage
    Energy->frequency_common = false;      // Use no common frequency
    Settings->flag5.energy_phase = 1;     // SetOption129 - (Energy) Show phase information
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Devices %d, RAM usage %d + %d + %d"),
    NrgMbsParam.devices,
    sizeof(NrgMbsParam),
    NrgMbsParam.total_regs * sizeof(NrgMbsRegister_t),
    NrgMbsParam.user_adds * sizeof(NrgMbsUser_t));
#endif

//  NrgMbsParam.state = 0;    // Set by calloc()
  NrgMbsParam.phase = -1;

  return true;
}

bool EnergyModbusRegisters(void) {
  if (EnergyModbusReadRegisters()) {
    return true;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("NRG: No valid modbus data"));
  return false;
}

void EnergyModbusSnsInit(void) {
  if (EnergyModbusRegisters()) {
    EnergyModbus = new TasmotaModbus(Pin(GPIO_NRG_MBS_RX), Pin(GPIO_NRG_MBS_TX), Pin(GPIO_NRG_MBS_TX_ENA));
    uint8_t result = EnergyModbus->Begin(NrgMbsParam.serial_bps, NrgMbsParam.serial_config);
    if (result) {
      if (2 == result) { ClaimSerial(); }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Serial UART%d"), EnergyModbus->getUart());
#endif

#ifdef ENERGY_MODBUS_TICKER
      ticker_energy_modbus.attach_ms(NrgMbsParam.ticker_poll, EnergyModbusLoop);
#endif  // ENERGY_MODBUS_TICKER

      return;
    }
  }
  TasmotaGlobal.energy_driver = ENERGY_NONE;
}

void EnergyModbusDrvInit(void) {
  if (PinUsed(GPIO_NRG_MBS_RX) && PinUsed(GPIO_NRG_MBS_TX)) {
    TasmotaGlobal.energy_driver = XNRG_29;
  }
}

/*********************************************************************************************\
 * Additional presentation
\*********************************************************************************************/

void EnergyModbusReset(void) {
  for (uint32_t i = 0; i < NrgMbsParam.user_adds; i++) {
    for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
      if (NrgMbsReg[NRG_MBS_MAX_REGS + i].address[0] != nrg_mbs_reg_not_used) {
        NrgMbsUser[i].data[j] = 0;
      }
    }
  }
}

uint32_t EnergyModbusResolution(uint32_t resolution) {
  if (resolution >= NRG_RES_VOLTAGE) {
    switch (resolution) {
      case NRG_RES_VOLTAGE:
        return Settings->flag2.voltage_resolution;
      case NRG_RES_CURRENT:
        return Settings->flag2.current_resolution;
      case NRG_RES_POWER:
        return Settings->flag2.wattage_resolution;
      case NRG_RES_ENERGY:
        return Settings->flag2.energy_resolution;
      case NRG_RES_FREQUENCY:
        return Settings->flag2.frequency_resolution;
      case NRG_RES_TEMPERATURE:
        return Settings->flag2.temperature_resolution;
      case NRG_RES_HUMIDITY:
        return Settings->flag2.humidity_resolution;
      case NRG_RES_PRESSURE:
        return Settings->flag2.pressure_resolution;
      case NRG_RES_WEIGHT:
        return Settings->flag2.weight_resolution;
    }
  }
  return resolution;
}

void EnergyModbusShow(bool json) {
  float values[ENERGY_MAX_PHASES];
  for (uint32_t i = 0; i < NrgMbsParam.user_adds; i++) {
    uint32_t reg_index = NRG_MBS_MAX_REGS + i;

#ifdef ENERGY_MODBUS_DEBUG_SHOW
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X], J '%s', G '%s', U '%s', D %d, V [%3_f,%3_f,%3_f]"),
      i,
      NrgMbsReg[reg_index].address[0],
      NrgMbsReg[reg_index].address[1],
      NrgMbsReg[reg_index].address[2],
      NrgMbsUser[i].json_name,
      NrgMbsUser[i].gui_name,
      NrgMbsUser[i].gui_unit,
      NrgMbsUser[i].resolution,
      &NrgMbsUser[i].data[0],
      &NrgMbsUser[i].data[1],
      &NrgMbsUser[i].data[2]);
#endif

    if ((NrgMbsReg[reg_index].address[0] != nrg_mbs_reg_not_used) && !isnan(NrgMbsUser[i].data[0])) {
      for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
        values[j] = NrgMbsUser[i].data[j];
      }
      uint32_t resolution = EnergyModbusResolution(NrgMbsUser[i].resolution);
      uint32_t single = (!isnan(NrgMbsUser[i].data[1]) && !isnan(NrgMbsUser[i].data[2])) ? 0 : 1;

#ifdef ENERGY_MODBUS_DEBUG_SHOW
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: resolution %d -> %d"), NrgMbsUser[i].resolution, resolution);
#endif
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":%s"), NrgMbsUser[i].json_name, EnergyFmt(values, resolution, single));
#ifdef USE_WEBSERVER
      } else {
        if (strlen(NrgMbsUser[i].gui_name)) {    // Skip empty GUI names
          WSContentSend_PD(PSTR("{s}%s{m}%s %s{e}"), NrgMbsUser[i].gui_name, WebEnergyFmt(values, resolution, single), NrgMbsUser[i].gui_unit);
        }
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg29(uint32_t function) {
  bool result = false;

  switch (function) {
#ifndef ENERGY_MODBUS_TICKER
//    case FUNC_EVERY_200_MSECOND:     // Energy ticker interrupt
    case FUNC_EVERY_250_MSECOND:     // Tasmota dispatcher
      EnergyModbusLoop();
      break;
#endif  // No ENERGY_MODBUS_TICKER
    case FUNC_JSON_APPEND:
      EnergyModbusShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
      EnergyModbusShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_ENERGY_RESET:
      EnergyModbusReset();
      break;
    case FUNC_INIT:
      EnergyModbusSnsInit();
      break;
    case FUNC_PRE_INIT:
      EnergyModbusDrvInit();
      break;
  }
  return result;
}

#endif  // USE_MODBUS_ENERGY
#endif  // USE_ENERGY_SENSOR
