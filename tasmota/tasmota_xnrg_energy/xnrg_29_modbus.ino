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
 * Using a rule file called modbus allows to easy configure modbus energy monitor devices up to three phases.
 *
 * Value pair description:
 * {"Name":"SDM230","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A}
 * Modbus config parameters:
 *   Name          - Name of energy monitoring device
 *   Baud          - Baudrate of device modbus interface - optional. default is 9600
 *   Config        - Serial config parameters like 8N1 - 8 databits, No parity, 1 stop bit
 *   Address       - Modbus device address entered as decimal (1) or hexadecimal (0x01)) - optional default = 1
 *   Function      - Modbus function code to access two registers - optional. default = 4
 * Tasmota default embedded register names:
 *   Voltage       - Voltage register entered as decimal or hexadecimal for one phase (0x0000) or up to three phases ([0x0000,0x0002,0x0004]) or
 *                   Additional defined parameters
 *                   Value pair description:
 *                   {"R":0,"T":0,"M":1}
 *                   R - Modbus register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
 *                   T - Datatype - optional. default is 0 - float:
 *                       0 - float
 *                       1 = 2-byte signed
 *                       2 = 4-byte signed
 *                       3 = 2-byte unsigned
 *                       4 = 4-byte unsigned
 *                   M - Divider allowing to devide the read register by 1, 10, 100, 1000 etc. - optional. default = 1
 *   Current       - Current register entered as decimal or hexadecimal for one phase (0x0006) or up to three phases ([0x0006,0x0008,0x000A]) or
 *                   See additional defines like voltage.
 *   Power         - Active power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010]) or
 *                   See additional defines like voltage.
 *   ApparentPower - Apparent power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010]) or
 *                   See additional defines like voltage.
 *   ReactivePower - Reactive power register entered as decimal or hexadecimal for one phase (0x0018) or up to three phases ([0x0018,0x001A,0x001C]) or
 *                   See additional defines like voltage.
 *   Factor        - Power factor register entered as decimal or hexadecimal for one phase (0x001E) or up to three phases ([0x001E,0x0020,0x0022]) or
 *                   See additional defines like voltage.
 *   Frequency     - Frequency register entered as decimal or hexadecimal for one phase (0x0046) or up to three phases ([0x0046,0x0048,0x004A]) or
 *                   See additional defines like voltage.
 *   Total         - Total active energy register entered as decimal or hexadecimal for one phase (0x0156) or up to three phases ([0x015A,0x015C,0x015E]) or
 *                   See additional defines like voltage.
 *   ExportActive  - Export active energy register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164]) or
 *                   See additional defines like voltage.
 * Optional user defined registers:
 *   User          - Additional user defined registers
 *                   Value pair description:
 *                   "User":{"R":0x0024,"T":0,"M":1,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}
 *                   R - Modbus register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
 *                   T - Datatype - optional. default is 0 - float:
 *                       0 - float
 *                       1 = 2-byte signed
 *                       2 = 4-byte signed
 *                       3 = 2-byte unsigned
 *                       4 = 4-byte unsigned
 *                   M - Divider allowing to devide the read register by 1, 10, 100, 1000 etc. - optional. default = 1
 *                   J - JSON register name (preferrably without spaces like "PhaseAngle")
 *                   G - GUI register name
 *                   U - GUI unit name
 *                   D - Number of decimals for floating point presentation (0 to 20) or a code correspondig to Tasmota resolution command settings:
 *                       21 - VoltRes (V)
 *                       22 - AmpRes (A)
 *                       23 - WattRes (W, VA, VAr)
 *                       24 - EnergyRes (kWh, kVAh, kVArh)
 *                       25 - FreqRes (Hz)
 *                       26 - TempRes (C, F)
 *                       27 - HumRes (%)
 *                       28 - PressRes (hPa, mmHg)
 *                       29 - WeightRes (Kg)
 *
 * Example using default Energy registers:
 * rule3 on file#modbus do {"Name":"SDM230","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A} endon
 * rule3 on file#modbus do {"Name":"SDM230 with hex registers","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0x0000,"Current":0x0006,"Power":0x000C,"ApparentPower":0x0012,"ReactivePower":0x0018,"Factor":0x001E,"Frequency":0x0046,"Total":0x0156,"ExportActive":0x004A} endon
 * rule3 on file#modbus do {"Name":"DDSU666","Baud":9600,"Config":8N1","Address":1,"Function":4,"Voltage":0x2000,"Current":0x2002,"Power":0x2004,"ReactivePower":0x2006,"Factor":0x200A,"Frequency":0x200E,"Total":0x4000,"ExportActive":0x400A} endon
 *
 * Example using default Energy registers and some user defined registers:
 * rule3 on file#modbus do {"Name":"SDM72","Baud":9600,"Config":8N1","Address":0x01,"Function":0x04,"Power":0x0034,"Total":0x0156,"ExportActive":0x004A,"User":[{"R":0x0502,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x0502,"J":"ExportPower","G":"Export Power","U":"W","D":23},{"R":0x0500,"J":"ImportPower","G":"Import Power","U":"W","D":23}]} endon
 * rule3 on file#modbus do {"Name":"SDM120","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x0048,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x004C,"J":"ImportReactive","G":"Import Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM230 with two user registers","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM630","Baud":9600,"Config":8N1","Address":1,"Function":4,"Voltage":[0,2,4],"Current":[6,8,10],"Power":[12,14,16],"ApparentPower":[18,20,22],"ReactivePower":[24,26,28],"Factor":[30,32,34],"Frequency":70,"Total":342,"ExportActive":[352,354,356],"User":{"R":[346,348,350],"J":"ImportActive","G":"Import Active","U":"kWh","D":24}} endon
 *
 * Note:
 * - To enter long rules using the serial console and solve error "Serial buffer overrun" you might need to enlarge the serial input buffer with command serialbuffer 800
 * - Changes to rule file are only executed on restart
 *
 * Restrictions:
 * - Supports Modbus floating point registers
 * - Max number of user defined registers is defined by one rule buffer (511 characters uncompressed, around 800 characters compressed)
 *
 * To do:
 * - Support all three rule slots
 * - Support other modbus register like integers
 *
 * Test set:
 * rule3 on file#modbus do {"Name":"SDM230 test1","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":[70,70,70],"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test2","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":70,"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test3","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":[6,6,6],"Power":[12,12,12],"ApparentPower":[18,18,18],"ReactivePower":[24,24,24],"Factor":[30,30,30],"Frequency":70,"Total":[342,342,342]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test4","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM230 test5","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":[0,0,0],"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":[0x004E,0x004E,0x004E],"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 * rule3 on file#modbus do {"Name":"SDM120 test1","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":0,"Current":6,"Power":12,"ApparentPower":18,"ReactivePower":24,"Factor":30,"Frequency":70,"Total":342,"ExportActive":0x004A,"User":[{"R":0x0048,"J":"ImportActive","G":"Import Active","U":"kWh","D":24},{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":24},{"R":0x004C,"J":"ImportReactive","G":"Import Reactive","U":"kVArh","D":24},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]} endon
 *
 * rule3 on file#modbus do {"Name":"SDM230 test6","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":0,"M":1},"Current":{"R":6,"T":0,"M":1},"Power":{"R":12,"T":0,"M":1},"Frequency":70,"Total":342} endon
 * rule3 on file#modbus do {"Name":"SDM230 test6","Baud":2400,"Config":8N1","Address":1,"Function":4,"Voltage":{"R":0,"T":0,"M":1},"Current":{"R":6,"T":0,"M":1},"Power":{"R":12,"T":0,"M":1},"Frequency":70,"Total":342,"User":{"R":0x0048,"T":0,"M":10,"J":"ImportActive","G":"Import Active","U":"kWh","D":24}} endon
\*********************************************************************************************/

#define XNRG_29                   29

#define ENERGY_MODBUS_SPEED       9600                 // Default Modbus baudrate
#define ENERGY_MODBUS_CONFIG      TS_SERIAL_8N1        // Default Modbus serial configuration
#define ENERGY_MODBUS_ADDR        1                    // Default Modbus device_address
#define ENERGY_MODBUS_FUNC        0x04                 // Default Modbus function code

#define ENERGY_MODBUS_DATATYPE    0                    // Default Modbus datatype is 4-byte float
#define ENERGY_MODBUS_DIVIDER     1                    // Default Modbus data divider

#define ENERGY_MODBUS_DECIMALS    0                    // Default user decimal resolution

#define ENERGY_MODBUS_TICKER                           // Enable for ESP8266 when using softwareserial solving most modbus serial retries

//#define ENERGY_MODBUS_DEBUG
//#define ENERGY_MODBUS_DEBUG_SHOW

const uint16_t nrg_mbs_reg_not_used = 1;               // Odd number 1 is unused register

enum EnergyModbusDataType { NRG_DT_FLOAT,              // 4-byte float
                            NRG_DT_S16,                // 2-byte signed
                            NRG_DT_S32,                // 4-byte signed
                            NRG_DT_U16,                // 2-byte unsigned
                            NRG_DT_U32,                // 4-byte unsigned
                            NRG_DT_MAX };

enum EnergyModbusResolutions { NRG_RES_VOLTAGE = 21,   // V
                               NRG_RES_CURRENT,        // A
                               NRG_RES_POWER,          // W, VA, VAr
                               NRG_RES_ENERGY,         // kWh, kVAh, kVArh
                               NRG_RES_FREQUENCY,      // Hz
                               NRG_RES_TEMPERATURE,    // C, F
                               NRG_RES_HUMIDITY,       // %
                               NRG_RES_PRESSURE,       // hPa, mmHg
                               NRG_RES_WEIGHT };       // Kg

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
  uint8_t device_address;
  uint8_t function;
  uint8_t total_regs;
  uint8_t user_adds;
  uint8_t phase;
  uint8_t state;
  uint8_t retry;
  bool mutex;
} NrgMbsParam;

typedef struct NRGMBSREGISTER {
  uint16_t address[ENERGY_MAX_PHASES];
  uint16_t divider;
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

    register_count = 2 - (NrgMbsReg[NrgMbsParam.state].datatype & 1);
    uint32_t error = EnergyModbus->ReceiveBuffer(buffer, register_count);

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Modbus register %d, phase %d, rcvd %*_H"),
      NrgMbsParam.state, NrgMbsParam.phase, EnergyModbus->ReceiveCount(), buffer);

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
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus error %d"), error);
    } else {
      Energy.data_valid[NrgMbsParam.phase] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      switch (NrgMbsReg[NrgMbsParam.state].datatype) {
        case NRG_DT_FLOAT: {
          ((uint8_t*)&value)[3] = buffer[3];   // Get float values
          ((uint8_t*)&value)[2] = buffer[4];
          ((uint8_t*)&value)[1] = buffer[5];
          ((uint8_t*)&value)[0] = buffer[6];
          break;
        }
        case NRG_DT_S16: {
          int16_t value_buff = ((int16_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_U16: {
          uint16_t value_buff = ((uint16_t)buffer[3])<<8 | buffer[4];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_S32: {
          int32_t value_buff = ((int32_t)buffer[3])<<24 | ((uint32_t)buffer[4])<<16 | ((uint32_t)buffer[5])<<8 | buffer[6];
          value = (float)value_buff;
          break;
        }
        case NRG_DT_U32: {
          uint32_t value_buff = ((uint32_t)buffer[3])<<24 | ((uint32_t)buffer[4])<<16 | ((uint32_t)buffer[5])<<8 | buffer[6];
          value = (float)value_buff;
          break;
        }
      }
      value /= NrgMbsReg[NrgMbsParam.state].divider;

      switch (NrgMbsParam.state) {
        case NRG_MBS_VOLTAGE:
          Energy.voltage[NrgMbsParam.phase] = value;          // 230.2 V
          break;
        case NRG_MBS_CURRENT:
          Energy.current[NrgMbsParam.phase]  = value;         // 1.260 A
          break;
        case NRG_MBS_ACTIVE_POWER:
          Energy.active_power[NrgMbsParam.phase] = value;     // -196.3 W
          break;
        case NRG_MBS_APPARENT_POWER:
          Energy.apparent_power[NrgMbsParam.phase] = value;   // 223.4 VA
          break;
        case NRG_MBS_REACTIVE_POWER:
          Energy.reactive_power[NrgMbsParam.phase] = value;   // 92.2
          break;
        case NRG_MBS_POWER_FACTOR:
          Energy.power_factor[NrgMbsParam.phase] = value;     // -0.91
          break;
        case NRG_MBS_FREQUENCY:
          Energy.frequency[NrgMbsParam.phase] = value;        // 50.0 Hz
          break;
        case NRG_MBS_TOTAL_ENERGY:
          Energy.import_active[NrgMbsParam.phase] = value;    // 6.216 kWh => used in EnergyUpdateTotal()
          break;
        case NRG_MBS_EXPORT_ACTIVE_ENERGY:
          Energy.export_active[NrgMbsParam.phase] = value;    // 478.492 kWh
          break;
        default:
          if (NrgMbsUser) {
            NrgMbsUser[NrgMbsParam.state - NRG_MBS_MAX_REGS].data[NrgMbsParam.phase] = value;
          }
      }

      do {
        NrgMbsParam.phase++;
        if (NrgMbsParam.phase >= Energy.phase_count) {
          NrgMbsParam.phase = 0;
          NrgMbsParam.state++;
          if (NrgMbsParam.state >= NrgMbsParam.total_regs) {
            NrgMbsParam.state = 0;
            NrgMbsParam.phase = 0;
            EnergyUpdateTotal();                 // update every cycle after all registers have been read
            break;
          }
        }
        delay(0);
      } while (NrgMbsReg[NrgMbsParam.state].address[NrgMbsParam.phase] == nrg_mbs_reg_not_used);
    }
  } // end data ready

  if (0 == NrgMbsParam.retry || data_ready) {
    NrgMbsParam.retry = 1;
    register_count = 2 - (NrgMbsReg[NrgMbsParam.state].datatype & 1);
    EnergyModbus->Send(NrgMbsParam.device_address, NrgMbsParam.function, NrgMbsReg[NrgMbsParam.state].address[NrgMbsParam.phase], register_count);
  } else {
    NrgMbsParam.retry--;

#ifdef ENERGY_MODBUS_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus state %d retry %d"), NrgMbsParam.state, NrgMbsParam.retry);
#endif

  }
  delay(0);
  NrgMbsParam.mutex = 0;
}

#ifdef USE_RULES
bool EnergyModbusReadUserRegisters(JsonParserObject user_add_value, uint32_t add_index) {
  // {"R":0x004E,"T":0,"M":1,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
  uint32_t reg_index = NRG_MBS_MAX_REGS + add_index;
  JsonParserToken val;
  val = user_add_value[PSTR("R")];               // Register address
  uint32_t phase = 0;
  if (val.isArray()) {
    JsonParserArray address_arr = val.getArray();
    for (auto value : address_arr) {
      NrgMbsReg[reg_index].address[phase] = value.getUInt();
      phase++;
      if (phase >= ENERGY_MAX_PHASES) { break; }
    }
  } else if (val) {
    NrgMbsReg[reg_index].address[0] = val.getUInt();
    phase++;
  } else {
    return false;
  }
  if (phase > Energy.phase_count) {
    Energy.phase_count = phase;
  }
  val = user_add_value[PSTR("T")];               // Register data type
  if (val) {
    // "T":0
    NrgMbsReg[reg_index].datatype = val.getUInt();
  }
  val = user_add_value[PSTR("M")];               // Register divider
  if (val) {
    // "M":1
    NrgMbsReg[reg_index].divider = val.getUInt();
  }
  val = user_add_value[PSTR("J")];               // JSON value name
  if (val) {
    NrgMbsUser[add_index].json_name = SetStr(val.getStr());
  } else {
    return false;
  }
  val = user_add_value[PSTR("G")];               // GUI value name
  if (val) {
    NrgMbsUser[add_index].gui_name = SetStr(val.getStr());
  } else {
    return false;
  }
  NrgMbsUser[add_index].gui_unit = EmptyStr;
  val = user_add_value[PSTR("U")];               // GUI value Unit
  if (val) {
    NrgMbsUser[add_index].gui_unit = SetStr(val.getStr());
  }
  NrgMbsUser[add_index].resolution = ENERGY_MODBUS_DECIMALS;
  val = user_add_value[PSTR("D")];               // Decimal resolution
  if (val) {
    NrgMbsUser[add_index].resolution = val.getUInt();
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X], T %d, M %d, J '%s', G '%s', U '%s', D %d"),
    add_index,
    NrgMbsReg[reg_index].address[0],
    NrgMbsReg[reg_index].address[1],
    NrgMbsReg[reg_index].address[2],
    NrgMbsReg[reg_index].datatype,
    NrgMbsReg[reg_index].divider,
    NrgMbsUser[add_index].json_name,
    NrgMbsUser[add_index].gui_name,
    NrgMbsUser[add_index].gui_unit,
    NrgMbsUser[add_index].resolution);
#endif

  return true;
}
#endif  // USE_RULES

bool EnergyModbusReadRegisters(void) {
#ifdef USE_RULES
  String modbus = RuleLoadFile("MODBUS");
  if (!modbus.length()) { return false; }        // File not found
//    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: File '%s'"), modbus.c_str());

  const char* json = modbus.c_str();
  uint32_t len = strlen(json) +1;
  if (len < 7) { return false; }                 // Invalid JSON

  char json_buffer[len];
  memcpy(json_buffer, json, len);                // Keep original safe
  JsonParser parser(json_buffer);
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }                   // Invalid JSON

  // Init defaults
  NrgMbsParam.serial_bps = ENERGY_MODBUS_SPEED;
  NrgMbsParam.serial_config = ENERGY_MODBUS_CONFIG;
  NrgMbsParam.device_address = ENERGY_MODBUS_ADDR;
  NrgMbsParam.function = ENERGY_MODBUS_FUNC;
  NrgMbsParam.user_adds = 0;

  JsonParserToken val;
  val = root[PSTR("User")];
  if (val) {
    if (val.isArray()) {
      // "User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]
      NrgMbsParam.user_adds = val.size();
    } else {
      // "User":{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
      NrgMbsParam.user_adds = 1;
    }
  }
  NrgMbsParam.total_regs = NRG_MBS_MAX_REGS + NrgMbsParam.user_adds;
  NrgMbsReg = (NrgMbsRegister_t*)calloc(NrgMbsParam.total_regs, sizeof(NrgMbsRegister_t));
  if (NrgMbsReg == nullptr) { return false; }    // Unable to allocate variables on heap
  // Init defaults
  for (uint32_t i = 0; i < NrgMbsParam.total_regs; i++) {
    NrgMbsReg[i].datatype = ENERGY_MODBUS_DATATYPE;
    NrgMbsReg[i].divider = ENERGY_MODBUS_DIVIDER;
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

  val = root[PSTR("Baud")];
  if (val) {
    NrgMbsParam.serial_bps = val.getInt();        // 2400
  }
  val = root[PSTR("Config")];
  if (val) {
    const char *serial_config = val.getStr();    // 8N1
    NrgMbsParam.serial_config = ConvertSerialConfig(ParseSerialConfig(serial_config));
  }
  val = root[PSTR("Address")];
  if (val) {
    NrgMbsParam.device_address = val.getUInt();   // 1
  }
  val = root[PSTR("Function")];
  if (val) {
    NrgMbsParam.function = val.getUInt();         // 4
  }

  char register_name[32];
  Energy.voltage_available = false;              // Disable voltage is measured
  Energy.current_available = false;              // Disable current is measured
  for (uint32_t names = 0; names < NRG_MBS_MAX_REGS; names++) {
    val = root[GetTextIndexed(register_name, sizeof(register_name), names, kEnergyModbusValues)];
    if (val) {
      // "Voltage":0
      // "Voltage":[0,0,0]
      // "Voltage":{"R":0,"T":0,"M":1}
      // "Voltage":{"R":[0,0,0],"T":0,"M":1}
      uint32_t phase = 0;
      if (val.isObject()) {
        // "Voltage":{"R":0,"T":0,"M":1}
        // "Voltage":{"R":[0,0,0],"T":0,"M":1}
        JsonParserObject register_add_values = val.getObject();
        val = register_add_values[PSTR("R")];    // Register address
        if (val.isArray()) {
          // "R":[0,0,0]
          JsonParserArray address_arr = val.getArray();
          for (auto value : address_arr) {
            NrgMbsReg[names].address[phase] = value.getUInt();
            phase++;
            if (phase >= ENERGY_MAX_PHASES) { break; }
          }
        } else if (val) {
          // "R":0
          NrgMbsReg[names].address[0] = val.getUInt();
          phase++;
        }
        val = register_add_values[PSTR("T")];    // Register data type
        if (val) {
          // "T":0
          NrgMbsReg[names].datatype = val.getUInt();
        }
        val = register_add_values[PSTR("M")];    // Register divider
        if (val) {
          // "M":1
          NrgMbsReg[names].divider = val.getUInt();
        }
      } else if (val.isArray()) {
        // "Voltage":[0,0,0]
        JsonParserArray arr = val.getArray();
        for (auto value : arr) {
          NrgMbsReg[names].address[phase] = value.getUInt();
          phase++;
          if (phase >= ENERGY_MAX_PHASES) { break; }
        }
      } else if (val) {
        // "Voltage":0
        NrgMbsReg[names].address[0] = val.getUInt();
        phase++;
      }
      if (phase > Energy.phase_count) {
        Energy.phase_count = phase;
      }
      switch(names) {
        case NRG_MBS_VOLTAGE:
          Energy.voltage_available = true;       // Enable if voltage is measured
          if (1 == phase) {
            Energy.voltage_common = true;        // Use common voltage
          }
          break;
        case NRG_MBS_CURRENT:
          Energy.current_available = true;       // Enable if current is measured
          break;
        case NRG_MBS_FREQUENCY:
          if (1 == phase) {
            Energy.frequency_common = true;      // Use common frequency
          }
          break;
        case NRG_MBS_TOTAL_ENERGY:
          Settings->flag3.hardware_energy_total = 1;  // SetOption72 - Enable hardware energy total counter as reference (#6561)
          break;
      }

#ifdef ENERGY_MODBUS_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X], T %d, M %d"),
        names,
        NrgMbsReg[names].address[0],
        NrgMbsReg[names].address[1],
        NrgMbsReg[names].address[2],
        NrgMbsReg[names].datatype,
        NrgMbsReg[names].divider);
#endif

    }
  }

  // "User":{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
  // "User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]
  val = root[PSTR("User")];
  if (val) {
    if (val.isArray()) {
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
      if (val.isObject()) {
        if (!EnergyModbusReadUserRegisters(val.getObject(), 0)) {
          AddLog(LOG_LEVEL_INFO, PSTR("NRG: Dropped JSON user input"));
          NrgMbsParam.user_adds--;
        }
      }
    }
    NrgMbsParam.total_regs = NRG_MBS_MAX_REGS + NrgMbsParam.user_adds;
  }

  for (uint32_t i = 0; i < NrgMbsParam.total_regs; i++) {
    if (NrgMbsReg[i].datatype >= NRG_DT_MAX) {
      NrgMbsReg[i].datatype = ENERGY_MODBUS_DATATYPE;
    }
    if (NrgMbsReg[i].divider < 1) {
      NrgMbsReg[i].divider = ENERGY_MODBUS_DIVIDER;
    }
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: RAM usage %d + %d + %d"), sizeof(NrgMbsParam), NrgMbsParam.total_regs * sizeof(NrgMbsRegister_t), NrgMbsParam.user_adds * sizeof(NrgMbsUser_t));
#endif

//  NrgMbsParam.state = 0;    // Set by calloc()
//  NrgMbsParam.phase = 0;

  return true;
#endif  // USE_RULES
  return false;
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
    EnergyModbus = new TasmotaModbus(Pin(GPIO_NRG_MBS_RX), Pin(GPIO_NRG_MBS_TX));
    uint8_t result = EnergyModbus->Begin(NrgMbsParam.serial_bps, NrgMbsParam.serial_config);
    if (result) {
      if (2 == result) { ClaimSerial(); }

#ifdef ENERGY_MODBUS_TICKER
      ticker_energy_modbus.attach_ms(200, EnergyModbusLoop);
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
  char value_chr[GUISZ];
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

#ifdef ENERGY_MODBUS_DEBUG_SHOW
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: resolution %d -> %d"), NrgMbsUser[i].resolution, resolution);
#endif

      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":%s"), NrgMbsUser[i].json_name, EnergyFormat(value_chr, values, resolution));
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(PSTR("{s}%s{m}%s %s{e}"),
          NrgMbsUser[i].gui_name,
          WebEnergyFormat(value_chr, values, resolution),
          NrgMbsUser[i].gui_unit);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg29(uint8_t function) {
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
#ifdef USE_ENERGY_COLUMN_GUI
    case FUNC_WEB_COL_SENSOR:
#else   // not USE_ENERGY_COLUMN_GUI
    case FUNC_WEB_SENSOR:
#endif  // USE_ENERGY_COLUMN_GUI
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
