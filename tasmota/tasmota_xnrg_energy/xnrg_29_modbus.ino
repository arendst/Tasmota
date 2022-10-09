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
 *   Baud          - Baudrate of device modbus interface
 *   Config        - Serial config parameters like 8N1 - 8 databits, No parity, 1 stop bit
 *   Address       - Modbus device address entered as decimal (1) or hexadecimal (0x01))
 *   Function      - Modbus function code to access two registers
 * Tasmota default embedded register names:
 *   Voltage       - Voltage register entered as decimal or hexadecimal for one phase (0x0000) or up to three phases ([0x0000,0x0002,0x0004])
 *   Current       - Current register entered as decimal or hexadecimal for one phase (0x0006) or up to three phases ([0x0006,0x0008,0x000A])
 *   Power         - Active power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010])
 *   ApparentPower - Apparent power register entered as decimal or hexadecimal for one phase (0x000C) or up to three phases ([0x000C,0x000E,0x0010])
 *   ReactivePower - Reactive power register entered as decimal or hexadecimal for one phase (0x0018) or up to three phases ([0x0018,0x001A,0x001C])
 *   Factor        - Power factor register entered as decimal or hexadecimal for one phase (0x001E) or up to three phases ([0x001E,0x0020,0x0022])
 *   Frequency     - Frequency register entered as decimal or hexadecimal for one phase (0x0046) or up to three phases ([0x0046,0x0048,0x004A])
 *   Total         - Total active energy register entered as decimal or hexadecimal for one phase (0x0156) or up to three phases ([0x015A,0x015C,0x015E])
 *   ExportActive  - Export active energy register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
 * Optional user defined registers:
 *   User          - Additional user defined registers
 *                   Value pair description:
 *                   "User":{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}
 *                   R - Modbus register entered as decimal or hexadecimal for one phase (0x0160) or up to three phases ([0x0160,0x0162,0x0164])
 *                   J - JSON register name (preferrably without spaces like "PhaseAngle")
 *                   G - GUI register name
 *                   U - GUI unit name
 *                   D - Number of decimals for floating point presentation or a code correspondig to Tasmota resolution command settings:
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
 * - Max number of uer defined registers is defined by one rule buffer (511 characters uncompressed, around 800 characters compressed)
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
\*********************************************************************************************/

#define XNRG_29                  29

#define ENERGY_MODBUS_SPEED      9600           // Default Modbus baudrate
#define ENERGY_MODBUS_CONFIG     TS_SERIAL_8N1  // Default Modbus serial configuration
#define ENERGY_MODBUS_ADDR       1              // Default Modbus device_address
#define ENERGY_MODBUS_FUNC       0x04           // Default Modbus function code

#define ENERGY_MODBUS_UNITS      ""             // Default user GUI unit
#define ENERGY_MODBUS_DECIMALS   0              // Default user decimal resolution

//#define ENERGY_MODBUS_DEBUG
//#define ENERGY_MODBUS_DEBUG_SHOW

const uint16_t nrg_mbs_reg_not_used = 1;  // Odd number 1 is unused register

enum EnergyModbusResolutions { NRG_RES_VOLTAGE = 21,         // V
                               NRG_RES_CURRENT,              // A
                               NRG_RES_POWER,                // W, VA, VAr
                               NRG_RES_ENERGY,               // kWh, kVAh, kVArh
                               NRG_RES_FREQUENCY,            // Hz
                               NRG_RES_TEMPERATURE,          // C, F
                               NRG_RES_HUMIDITY,             // %
                               NRG_RES_PRESSURE,             // hPa, mmHg
                               NRG_RES_WEIGHT };             // Kg

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
#include <Ticker.h>
Ticker ticker_energy_modbus;

struct NRGMODBUS {
  uint32_t serial_bps;
  uint32_t serial_config;
  uint16_t register_address[NRG_MBS_MAX_REGS][ENERGY_MAX_PHASES];
  uint8_t device_address;
  uint8_t function;
  uint8_t user_adds;
  uint8_t phase;
  uint8_t state;
  uint8_t retry;
  bool mutex;
} *NrgModbus = nullptr;

typedef struct NRGMODBUSUSER {
  float register_data[ENERGY_MAX_PHASES];
  uint16_t register_address[ENERGY_MAX_PHASES];
  uint8_t resolution;
  String json_name;
  String gui_name;
  String gui_unit;
} NrgModbusUser_t;
NrgModbusUser_t* NrgModbusUser = nullptr;

/*********************************************************************************************/

void EnergyModbusLoop(void) {
  if (NrgModbus->mutex) { return; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: EnergyModbusLoop() entry"));

  NrgModbus->mutex = 1;

  uint16_t register_address;
  bool data_ready = EnergyModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[9];  // At least 5 + (2 * 2) = 9
    uint32_t error = EnergyModbus->ReceiveBuffer(buffer, 2);

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Modbus register %d, phase %d, rcvd %*_H"),
      NrgModbus->state, NrgModbus->phase, EnergyModbus->ReceiveCount(), buffer);

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
      Energy.data_valid[NrgModbus->phase] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(NrgModbus->state) {
        case NRG_MBS_VOLTAGE:
          Energy.voltage[NrgModbus->phase] = value;          // 230.2 V
          break;
        case NRG_MBS_CURRENT:
          Energy.current[NrgModbus->phase]  = value;         // 1.260 A
          break;
        case NRG_MBS_ACTIVE_POWER:
          Energy.active_power[NrgModbus->phase] = value;     // -196.3 W
          break;
        case NRG_MBS_APPARENT_POWER:
          Energy.apparent_power[NrgModbus->phase] = value;   // 223.4 VA
          break;
        case NRG_MBS_REACTIVE_POWER:
          Energy.reactive_power[NrgModbus->phase] = value;   // 92.2
          break;
        case NRG_MBS_POWER_FACTOR:
          Energy.power_factor[NrgModbus->phase] = value;     // -0.91
          break;
        case NRG_MBS_FREQUENCY:
          Energy.frequency[NrgModbus->phase] = value;        // 50.0 Hz
          break;
        case NRG_MBS_TOTAL_ENERGY:
          Energy.import_active[NrgModbus->phase] = value;    // 6.216 kWh => used in EnergyUpdateTotal()
          break;
        case NRG_MBS_EXPORT_ACTIVE_ENERGY:
          Energy.export_active[NrgModbus->phase] = value;    // 478.492 kWh
          break;
        default:
          if (NrgModbusUser) {
            NrgModbusUser[NrgModbus->state - NRG_MBS_MAX_REGS].register_data[NrgModbus->phase] = value;
          }
      }

      do {
        NrgModbus->phase++;
        if (NrgModbus->phase >= Energy.phase_count) {
          NrgModbus->phase = 0;
          NrgModbus->state++;
          if (NrgModbus->state >= NRG_MBS_MAX_REGS + NrgModbus->user_adds) {
            NrgModbus->state = 0;
            NrgModbus->phase = 0;
            EnergyUpdateTotal();                 // update every cycle after all registers have been read
            break;
          }
        }
        delay(0);
        register_address = (NrgModbus->state < NRG_MBS_MAX_REGS) ? NrgModbus->register_address[NrgModbus->state][NrgModbus->phase] :
                                                                   NrgModbusUser[NrgModbus->state - NRG_MBS_MAX_REGS].register_address[NrgModbus->phase];
      } while (register_address == nrg_mbs_reg_not_used);
    }
  } // end data ready

  if (0 == NrgModbus->retry || data_ready) {
    NrgModbus->retry = 1;
    register_address = (NrgModbus->state < NRG_MBS_MAX_REGS) ? NrgModbus->register_address[NrgModbus->state][NrgModbus->phase] :
                                                               NrgModbusUser[NrgModbus->state - NRG_MBS_MAX_REGS].register_address[NrgModbus->phase];
    EnergyModbus->Send(NrgModbus->device_address, NrgModbus->function, register_address, 2);
  } else {
    NrgModbus->retry--;

#ifdef ENERGY_MODBUS_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Modbus state %d retry %d"), NrgModbus->state, NrgModbus->retry);
#endif

  }
  delay(0);
  NrgModbus->mutex = 0;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: EnergyModbusLoop() exit"));

}

#ifdef USE_RULES
bool EnergyModbusReadUserRegisters(JsonParserObject user_add_value, uint32_t add_index) {
  // {"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
  JsonParserToken val;
  val = user_add_value[PSTR("R")];               // Register address
  uint32_t phase = 0;
  if (val.isArray()) {
    JsonParserArray address_arr = val.getArray();
    for (auto value : address_arr) {
      NrgModbusUser[add_index].register_address[phase] = value.getUInt();
      phase++;
      if (phase >= ENERGY_MAX_PHASES) { break; }
    }
  } else if (val) {
    NrgModbusUser[add_index].register_address[0] = val.getUInt();
    phase++;
  } else {
    return false;
  }
  if (phase > Energy.phase_count) {
    Energy.phase_count = phase;
  }
  val = user_add_value[PSTR("J")];               // JSON value name
  if (val) {
    NrgModbusUser[add_index].json_name = val.getStr();
  } else {
    return false;
  }
  val = user_add_value[PSTR("G")];               // GUI value name
  if (val) {
    NrgModbusUser[add_index].gui_name = val.getStr();
  } else {
    return false;
  }
  NrgModbusUser[add_index].gui_unit = ENERGY_MODBUS_UNITS;
  val = user_add_value[PSTR("U")];               // GUI value Unit
  if (val) {
    NrgModbusUser[add_index].gui_unit = val.getStr();
  }
  NrgModbusUser[add_index].resolution = ENERGY_MODBUS_DECIMALS;
  val = user_add_value[PSTR("D")];               // Decimal resolution
  if (val) {
    NrgModbusUser[add_index].resolution = val.getUInt();
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X], J '%s', G '%s', U '%s', D %d"),
    add_index,
    NrgModbusUser[add_index].register_address[0],
    NrgModbusUser[add_index].register_address[1],
    NrgModbusUser[add_index].register_address[2],
    NrgModbusUser[add_index].json_name.c_str(),
    NrgModbusUser[add_index].gui_name.c_str(),
    NrgModbusUser[add_index].gui_unit.c_str(),
    NrgModbusUser[add_index].resolution);
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

  NrgModbus = (NRGMODBUS *)calloc(1, sizeof(struct NRGMODBUS));
  if (NrgModbus == nullptr) { return false; }    // Unable to allocate variables on heap

  // Init defaults
  NrgModbus->serial_bps = ENERGY_MODBUS_SPEED;
  NrgModbus->serial_config = ENERGY_MODBUS_CONFIG;
  NrgModbus->device_address = ENERGY_MODBUS_ADDR;
  NrgModbus->function = ENERGY_MODBUS_FUNC;
  for (uint32_t i = 0; i < NRG_MBS_MAX_REGS; i++) {
    for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
      NrgModbus->register_address[i][j] = nrg_mbs_reg_not_used;
    }
  }

  JsonParserToken val;
  val = root[PSTR("Baud")];
  if (val) {
    NrgModbus->serial_bps = val.getInt();        // 2400
  }
  val = root[PSTR("Config")];
  if (val) {
    const char *serial_config = val.getStr();    // 8N1
    NrgModbus->serial_config = ConvertSerialConfig(ParseSerialConfig(serial_config));
  }
  val = root[PSTR("Address")];
  if (val) {
    NrgModbus->device_address = val.getUInt();   // 1
  }
  val = root[PSTR("Function")];
  if (val) {
    NrgModbus->function = val.getUInt();         // 4
  }

  char register_name[32];
  Energy.voltage_available = false;              // Disable voltage is measured
  Energy.current_available = false;              // Disable current is measured
  for (uint32_t names = 0; names < NRG_MBS_MAX_REGS; names++) {
    val = root[GetTextIndexed(register_name, sizeof(register_name), names, kEnergyModbusValues)];
    if (val) {
      // "Voltage":0
      // "Voltage":[0,0,0]
      uint32_t phase = 0;
      if (val.isArray()) {
        JsonParserArray arr = val.getArray();
        for (auto value : arr) {
          NrgModbus->register_address[names][phase] = value.getUInt();
          phase++;
          if (phase >= ENERGY_MAX_PHASES) { break; }
        }
      } else if (val) {
        NrgModbus->register_address[names][0] = val.getUInt();
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
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X]"),
        names,
        NrgModbus->register_address[names][0],
        NrgModbus->register_address[names][1],
        NrgModbus->register_address[names][2]);
#endif

    }
  }

  NrgModbus->user_adds = 0;
  // "User":{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3}
  // "User":[{"R":0x004E,"J":"ExportReactive","G":"Export Reactive","U":"kVArh","D":3},{"R":0x0024,"J":"PhaseAngle","G":"Phase Angle","U":"Deg","D":2}]
  val = root[PSTR("User")];
  if (val) {
    NrgModbus->user_adds = 1;
    if (val.isArray()) {
      NrgModbus->user_adds = val.size();
    }
    NrgModbusUser = (NrgModbusUser_t*)calloc(NrgModbus->user_adds, sizeof(NrgModbusUser_t));
    if (NrgModbusUser) {
      // Init defaults
      for (uint32_t i = 0; i < NrgModbus->user_adds; i++) {
        for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
          NrgModbusUser[i].register_address[j] = nrg_mbs_reg_not_used;
          NrgModbusUser[i].register_data[j] = NAN;
        }
      }
      if (val.isArray()) {
        JsonParserArray user_adds_arr = val.getArray();
        uint32_t add_index = 0;
        for (auto user_add_values : user_adds_arr) {
          if (!user_add_values.isObject()) { break; }
          if (EnergyModbusReadUserRegisters(user_add_values.getObject(), add_index)) {
            add_index++;
          } else {
            AddLog(LOG_LEVEL_INFO, PSTR("NRG: Dropped JSON user input %d"), add_index +1);
            NrgModbus->user_adds--;
          }
        }
      } else if (val) {
        if (val.isObject()) {
          if (!EnergyModbusReadUserRegisters(val.getObject(), 0)) {
            AddLog(LOG_LEVEL_INFO, PSTR("NRG: Dropped JSON user input"));
            NrgModbus->user_adds--;
          }
        }
      }
    } else {
      // Unable to allocate variables on heap
      NrgModbus->user_adds = 0;
    }
  }

#ifdef ENERGY_MODBUS_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: RAM usage %d + %d"), sizeof(struct NRGMODBUS), NrgModbus->user_adds * sizeof(NrgModbusUser_t));
#endif

//  NrgModbus->state = 0;    // Set by calloc()
//  NrgModbus->phase = 0;

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
    uint8_t result = EnergyModbus->Begin(NrgModbus->serial_bps, NrgModbus->serial_config);
    if (result) {
      if (2 == result) { ClaimSerial(); }
      ticker_energy_modbus.attach_ms(150, EnergyModbusLoop);
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
  for (uint32_t i = 0; i < NrgModbus->user_adds; i++) {
    for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
      if (NrgModbusUser[i].register_address[0] != nrg_mbs_reg_not_used) {
        NrgModbusUser[i].register_data[j] = 0;
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
  char value_chr[TOPSZ];
  for (uint32_t i = 0; i < NrgModbus->user_adds; i++) {

#ifdef ENERGY_MODBUS_DEBUG_SHOW
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Idx %d, R [%04X,%04X,%04X], J '%s', G '%s', U '%s', D %d, V [%3_f,%3_f,%3_f]"),
      i,
      NrgModbusUser[i].register_address[0],
      NrgModbusUser[i].register_address[1],
      NrgModbusUser[i].register_address[2],
      NrgModbusUser[i].json_name.c_str(),
      NrgModbusUser[i].gui_name.c_str(),
      NrgModbusUser[i].gui_unit.c_str(),
      NrgModbusUser[i].resolution,
      &NrgModbusUser[i].register_data[0],
      &NrgModbusUser[i].register_data[1],
      &NrgModbusUser[i].register_data[2]);
#endif

    if ((NrgModbusUser[i].register_address[0] != nrg_mbs_reg_not_used) && !isnan(NrgModbusUser[i].register_data[0])) {
      float values[ENERGY_MAX_PHASES];
      for (uint32_t j = 0; j < ENERGY_MAX_PHASES; j++) {
        values[j] = NrgModbusUser[i].register_data[j];
      }
      uint32_t resolution = EnergyModbusResolution(NrgModbusUser[i].resolution);

#ifdef ENERGY_MODBUS_DEBUG_SHOW
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: resolution %d -> %d"), NrgModbusUser[i].resolution, resolution);
#endif

      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":%s"), NrgModbusUser[i].json_name.c_str(), EnergyFormat(value_chr, values, resolution));
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(PSTR("{s}%s{m}%s %s{e}"),
          NrgModbusUser[i].gui_name.c_str(),
          WebEnergyFormat(value_chr, values, resolution),
          NrgModbusUser[i].gui_unit.c_str());
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
/*
    case FUNC_EVERY_200_MSECOND:     // Energy ticker interrupt
//    case FUNC_EVERY_250_MSECOND:     // Tasmota dispatcher
      EnergyModbusLoop();
      break;
*/
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
