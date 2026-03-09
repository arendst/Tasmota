/*
  xdrv_72_pipsolar.ino - pipsolar support for Tasmota

  Copyright (C) 2023 Peter Rustler

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

#ifdef USE_PIPSOLAR

#define XDRV_72 72

#define PIPSOLAR_RECEIVEBUFFER_SIZE TM_SERIAL_BUFFER_SIZE * 2 // 128
#define PIPSOLAR_SENDBUFFER_SIZE TM_SERIAL_BUFFER_SIZE        // 64

#include <TasmotaSerial.h>
#include <memory>

#include <PubSubClient.h>

struct PipSolar
{
  TasmotaSerial *serial;
  uint8_t *receiveBuffer;
  uint8_t *sendBuffer;

  uint8_t receiveBufferPosition;
  enum class CommandType
  {
    NONE,
    QPIGS, // get analog values
    QMOD,  // get mode
    QPIWS, // get status/error information
    QT,    // get inverter time
    QET,   // energy total
    QEY,   // energy year
    QEM,   // energy month
    QED,   // energy day
    QEH,   // energy hour
    DAT,   // set date time
    QPIRI, // Device Rating Information (settings)
    QFLAG, // Device flag status
    CUSTOM,// Custom command
  } commandType;

  enum class CommandState {
    None,
    WaitForResponse,
    GotResponse
  } commandState;

  uint32_t secondIntervalCounter;
  uint16_t commandTimeoutCounter;
  bool pollValues;

  uint32_t successPoll;
  uint32_t errorPoll;
  uint32_t currentErrorPoll;
  uint32_t maxCurrentErrorPoll;

  union Value
  {
    int intValue;
    float floatValue;
    bool boolValue;
    char charValue;
  };

  enum class BoolFlag : int8_t {
    False,
    True,
    Unknown,
  };

  struct QFlagValueSetting {
    BoolFlag value;
    const char identifier;
    const char name[30];
  };

  struct ValueSetting
  {
    Value value;
    enum class Type
    {
      unknownType,
      intType,
      intTypeCopy,
      floatType,
      floatTypeCopy,
      boolType,
      charType,
      stringType,
    } const type;
    const uint8_t position;
    const uint8_t count;
    const char name[30];
    const char unit[4];
    const bool publish;
  };

  struct QueryCommand {
    PipSolar::CommandType commandType;
    bool send;
    char parameter[20];
  } queryCommand;
} PIPSOLAR;

struct PIPSOLARPollingValue {
  const uint32_t interval;
  const PipSolar::CommandType command;
  const uint16_t timeout; // *2ms
  const uint16_t waitAfterResponse; // *2ms
  bool poll;
};

struct PIPSOLARPollingValue PIPSOLARpollingList[] = {
  {3, PipSolar::CommandType::QPIWS, 600, 200, false},
  {3, PipSolar::CommandType::QPIGS, 600, 200, false},
  {3, PipSolar::CommandType::QMOD, 200, 200, false},
  {3, PipSolar::CommandType::QT, 200, 200, false},
  {3, PipSolar::CommandType::QET, 200, 200, false},
  {0, PipSolar::CommandType::QEY, 200, 200, false},
  {0, PipSolar::CommandType::QEM, 200, 200, false},
  {0, PipSolar::CommandType::QED, 200, 200, false},
  {0, PipSolar::CommandType::QEH, 200, 200, false},
  {0, PipSolar::CommandType::DAT, 200, 200, false},
  {0, PipSolar::CommandType::QPIRI, 600, 200, false},
  {0, PipSolar::CommandType::QFLAG, 600, 200, false},
  {0, PipSolar::CommandType::CUSTOM, 600, 200, false},
};
constexpr uint8_t PIPSOLARpollingListCount = sizeof(PIPSOLARpollingList) / sizeof(PIPSOLARPollingValue);
uint8_t PIPSOLARpollingValuePosition = PIPSOLARpollingListCount;

// (233.6 50.0 230.2 49.9 0299 0236 005 363 51.80 000 036 0039 00.1 000.0 00.00 00005 00010000 00 00 00000 010
struct PipSolar::ValueSetting PIPSOLARqpigsValueSettings[] = {
    {{}, PipSolar::ValueSetting::Type::floatType, 1, 5, "Grid_voltage", "V", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 7, 4, "Grid_frequency", "Hz", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 12, 5, "AC_output_voltage", "V", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 18, 4, "AC_output_frequency", "Hz", false},
    {{}, PipSolar::ValueSetting::Type::intType, 23, 4, "AC_output_apparent_power", "W", false},
    {{}, PipSolar::ValueSetting::Type::intType, 28, 4, "AC_output_active_power", "W", true},
    {{}, PipSolar::ValueSetting::Type::intType, 33, 3, "Output_load_percent", "%", true},
    {{}, PipSolar::ValueSetting::Type::intType, 37, 3, "BUS_voltage", "V", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 41, 5, "Battery_voltage", "V", true},
    {{}, PipSolar::ValueSetting::Type::intType, 47, 3, "Battery_charging_current", "A", false},
    {{}, PipSolar::ValueSetting::Type::intType, 51, 3, "Battery_capacity", "Ah", false},
    {{}, PipSolar::ValueSetting::Type::intType, 55, 4, "Heat_sink_temperature", "°C", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 60, 4, "PV_Input_current", "A", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 65, 5, "PV_Input_voltage", "V", false},
    {{}, PipSolar::ValueSetting::Type::floatType, 71, 5, "Battery_voltage_from_SCC", "V", false},
    {{}, PipSolar::ValueSetting::Type::intType, 77, 5, "Battery_discharge_current", "A", false},
    // deviceStatus 8x
    {{}, PipSolar::ValueSetting::Type::stringType, 83, 8, "Device status", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 83, 1, "a7", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 84, 1, "a6", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 85, 1, "a5", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 86, 1, "a4", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 87, 1, "a3", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 88, 1, "a2", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 89, 1, "a1", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 90, 1, "a0", "", false},

    {{}, PipSolar::ValueSetting::Type::intType, 92, 2, "Battery_voltage_offset_ffo", "V", false},
    {{}, PipSolar::ValueSetting::Type::intType, 95, 2, "EEPROM_version", "", false},
    {{}, PipSolar::ValueSetting::Type::intType, 98, 5, "PV_Charging_power", "W", true},
    // deviceStatus 3x
    {{}, PipSolar::ValueSetting::Type::stringType, 100, 3, "Device status", "", false},
    //{{}, PipSolar::ValueSetting::Type::boolType, 100, 1, "Device_status[8]", ""},
    //{{}, PipSolar::ValueSetting::Type::boolType, 101, 1, "Device_status[9]", ""},
    //{{}, PipSolar::ValueSetting::Type::boolType, 102, 1, "Device_status[10]", ""},
};
constexpr uint8_t PIPSOLARqpigsValueSettingsCount = sizeof(PIPSOLARqpigsValueSettings) / sizeof(PipSolar::ValueSetting);

// (230.0 24.3 230.0 50.0 24.3 5600 5600 48.0 49.0 48.0 55.6 55.6 2 002 060 1 2 3 9 00 0 2 49.0 0 1 000
struct PipSolar::ValueSetting PIPSOLARqpiriValueSettings[] = {
    //{{}, PipSolar::ValueSetting::Type::floatType, 1, 5, "Output_voltage", "V", true},
    //{{}, PipSolar::ValueSetting::Type::floatType, 7, 4, "Output_current", "Hz", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 12, 5, "AC_out_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 18, 4, "AC_out_frequency_setting", "Hz", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 23, 4, "AC_out_current_setting", "A", true},
    {{}, PipSolar::ValueSetting::Type::intType,   28, 4, "AC_out_apparent_power_setting", "W", true},
    {{}, PipSolar::ValueSetting::Type::intType,   33, 4, "AC_out_active_power_setting", "W", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 38, 4, "Bat_rating_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 43, 4, "Bat_recharge_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 48, 4, "Bat_under_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 53, 4, "Bat_bulk_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 58, 4, "Bat_float_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::intType,   63, 1, "Bat_type_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   65, 3, "Current_max_ac_charge_setting", "A", true},
    {{}, PipSolar::ValueSetting::Type::intType,   69, 3, "Current_max_charge_setting", "A", true},
    {{}, PipSolar::ValueSetting::Type::intType,   73, 1, "Input_voltage_range_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   75, 1, "Out_source_prio_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   77, 1, "Charge_source_prio_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   79, 1, "Parallel_max_number_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   81, 2, "Maschine_type_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   84, 1, "Topology_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   86, 1, "Output_mode_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::floatType, 88, 4, "Redischarge_voltage_setting", "V", true},
    {{}, PipSolar::ValueSetting::Type::intType,   93, 1, "PV_parallel_ok_cond_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   95, 1, "PV_power_balance_setting", "", true},
    {{}, PipSolar::ValueSetting::Type::intType,   97, 3, "Max_time_cv_stage_setting", "Min", true},
};
constexpr uint8_t PIPSOLARqpiriValueSettingsCount = sizeof(PIPSOLARqpiriValueSettings) / sizeof(PipSolar::ValueSetting);

// (B
struct PipSolar::ValueSetting PIPSOLARqmodValueSettings[] = {
    {{}, PipSolar::ValueSetting::Type::charType, 1, 1, "Mode", "", true},
};
constexpr uint8_t PIPSOLARqmodValueSettingsCount = sizeof(PIPSOLARqmodValueSettings) / sizeof(PipSolar::ValueSetting);

// (YYYYMMDDHHMMSS
struct PipSolar::ValueSetting PIPSOLARqtValueSettings[] = {
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 1, 4, "Year", "", false},
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 5, 2, "Month", "", false},
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 7, 2, "Day", "", false},
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 9, 2, "Hour", "", false},
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 11, 2, "Minute", "", false},
    {{}, PipSolar::ValueSetting::Type::intTypeCopy, 13, 2, "Second", "", false},
};
constexpr uint8_t PIPSOLARqtValueSettingsCount = sizeof(PIPSOLARqtValueSettings) / sizeof(PipSolar::ValueSetting);

struct PipSolar::ValueSetting PIPSOLARqexValueSettings[] = {
    {{}, PipSolar::ValueSetting::Type::intType, 1, 8, "Total_Energy", "Wh", true},
};
constexpr uint8_t PIPSOLARqexValueSettingsCount = sizeof(PIPSOLARqexValueSettings) / sizeof(PipSolar::ValueSetting);

// (EakxyDbjuvz
struct PipSolar::QFlagValueSetting PIPSOLARqflagValueSettings[] = {
    // deviceflags 8x
    {PipSolar::BoolFlag::Unknown, 'a', "Buzzer"},
    {PipSolar::BoolFlag::Unknown, 'b', "OverloadBypass"},
    {PipSolar::BoolFlag::Unknown, 'k', "DisplayTimeout"},
    {PipSolar::BoolFlag::Unknown, 'u', "OverloadRestart"},
    {PipSolar::BoolFlag::Unknown, 'v', "TemperatureRestart"},
    {PipSolar::BoolFlag::Unknown, 'x', "Backlight"},
    {PipSolar::BoolFlag::Unknown, 'y', "SourceInterruptAlarm"},
    {PipSolar::BoolFlag::Unknown, 'z', "FaultCodeRecord"},
};
constexpr uint8_t PIPSOLARqflagValueSettingsCount = sizeof(PIPSOLARqflagValueSettings) / sizeof(PipSolar::QFlagValueSetting);

/********************************************************************************************/
bool PIPSOLARSendCommand(PipSolar::CommandType cmd, const char *parameter = nullptr, bool checksum = false);

bool SetPipSolarSerialBegin(void) {
  return PIPSOLAR.serial->begin(Settings->sbaudrate * 300, ConvertSerialConfig(Settings->sserial_config));  // Reinitialize serial port with new baud rate
}

bool checkNumbersFormat(const char *buffer, int count)
{
  if (count == 0)
    return false;
  for (int i = 0; i < count; ++i)
    if (buffer[i] > '9' || buffer[i] < '0')
      return false;
  return true;
}

void CmndPipSolarNoParameter(PipSolar::CommandType commandType) {
  if (PIPSOLAR.commandType != PipSolar::CommandType::NONE
      && PIPSOLAR.queryCommand.commandType != PipSolar::CommandType::NONE) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Busy"));
  } else if (PIPSOLAR.commandType == PipSolar::CommandType::NONE) {
    ResponseCmndChar_P(PSTR("Send"));
    PIPSOLAR.queryCommand.send = true;
    PIPSOLAR.queryCommand.commandType = commandType;
    PIPSOLARSendCommand(commandType);
  } else {
    PIPSOLAR.queryCommand.commandType = commandType;
    PIPSOLAR.queryCommand.send = false;
    ResponseCmndChar_P(PSTR("Queue"));
  }
}
void CmndPipSolarParameter(PipSolar::CommandType commandType) {
  if (PIPSOLAR.commandType != PipSolar::CommandType::NONE
      && PIPSOLAR.queryCommand.commandType != PipSolar::CommandType::NONE) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Busy"));
  } else if (PIPSOLAR.commandType == PipSolar::CommandType::NONE) {
    PIPSOLAR.queryCommand.send = true;
    strncpy(PIPSOLAR.queryCommand.parameter, XdrvMailbox.data, sizeof(PIPSOLAR.queryCommand.parameter));
    PIPSOLAR.queryCommand.commandType = commandType;
    PIPSOLARSendCommand(commandType, PIPSOLAR.queryCommand.parameter);
    ResponseCmndChar_P(PSTR("Send"));
  } else {
    PIPSOLAR.queryCommand.send = false;
    strncpy(PIPSOLAR.queryCommand.parameter, XdrvMailbox.data, sizeof(PIPSOLAR.queryCommand.parameter));
    PIPSOLAR.queryCommand.commandType = commandType;
    ResponseCmndChar_P(PSTR("Queue"));
  }
}

void CmndPipSolarQT(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QT"));
  CmndPipSolarNoParameter(PipSolar::CommandType::QT);
}

void CmndPipSolarQET(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QET"));
  CmndPipSolarNoParameter(PipSolar::CommandType::QET);
}

void CmndPipSolarQEY(void) {
  if (!checkNumbersFormat(XdrvMailbox.data, XdrvMailbox.data_len)) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"No numeric parameter"));
    return;
  }
  if (XdrvMailbox.data_len != 4) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QEY"));
  CmndPipSolarParameter(PipSolar::CommandType::QEY);
}

void CmndPipSolarQEM(void) {
  if (!checkNumbersFormat(XdrvMailbox.data, XdrvMailbox.data_len)) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"No numeric parameter"));
    return;
  }
  if (XdrvMailbox.data_len != 6) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QEM"));
  CmndPipSolarParameter(PipSolar::CommandType::QEM);
}

void CmndPipSolarQED(void) {
  if (!checkNumbersFormat(XdrvMailbox.data, XdrvMailbox.data_len)) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"No numeric parameter"));
    return;
  }
  if (XdrvMailbox.data_len != 8) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QED"));
  CmndPipSolarParameter(PipSolar::CommandType::QED);
}

void CmndPipSolarQEH(void) {
  if (!checkNumbersFormat(XdrvMailbox.data, XdrvMailbox.data_len)) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"No numeric parameter"));
    return;
  }
  if (XdrvMailbox.data_len != 10) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QEH"));
  CmndPipSolarParameter(PipSolar::CommandType::QEH);
}

void CmndPipSolarDAT(void) {
  if (!checkNumbersFormat(XdrvMailbox.data, XdrvMailbox.data_len)) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"No numeric parameter"));
    return;
  }
  if (XdrvMailbox.data_len != 12) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command DAT"));
  CmndPipSolarParameter(PipSolar::CommandType::DAT);
}
void CmndPipSolarQPIRI(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QPIRI"));
  CmndPipSolarNoParameter(PipSolar::CommandType::QPIRI);
}

void CmndPipSolarQFLAG(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command QFLAG"));
  CmndPipSolarNoParameter(PipSolar::CommandType::QFLAG);
}

void CmndPipSolarCUSTOM(void) {
  if (XdrvMailbox.data_len == 0) {
    ResponseCmndChar_P(PSTR("Error\", \"Error\": \"Parameter count"));
    return;
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command DAT"));
  CmndPipSolarParameter(PipSolar::CommandType::CUSTOM);
}

void CmndPipSolarPollValues(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command PollValues"));
  char argument[XdrvMailbox.data_len];

  if (ArgC()==1) {
    PIPSOLAR.pollValues = atoi(ArgV(argument, 1)) == 0 ? false : true;
  }
  Response_P(PSTR("{\"" D_CMND_PIP_PREFIX D_CMND_PIP_POLLVALUES "\": %d}"), PIPSOLAR.pollValues);
}

void CmndPipSolarBaudRate(void) {
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: command BaudRate"));
  if (XdrvMailbox.payload >= 300) {
    XdrvMailbox.payload /= 300;  // Make it a valid baudrate
    Settings->sbaudrate = XdrvMailbox.payload;
    if (SetPipSolarSerialBegin())
    {
      if (PIPSOLAR.serial->hardwareSerial())
      {
        ClaimSerial();
      }
    }
  }
  ResponseCmndNumber(Settings->sbaudrate * 300);
}

void SetPipSolarSerialConfig(uint32_t serial_config) {
  if (serial_config > TS_SERIAL_8O2) {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->sserial_config) {
    Settings->sserial_config = serial_config;
    SetPipSolarSerialBegin();
  }
}

void CmndPipSolarSerialConfig(void) {

  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.data_len < 3) {                    // Use 0..23 as serial config option
      if ((XdrvMailbox.payload >= TS_SERIAL_5N1) && (XdrvMailbox.payload <= TS_SERIAL_8O2)) {
        SetPipSolarSerialConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8)) {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0) {
        SetPipSolarSerialConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig(Settings->sserial_config).c_str());
}

const char kPipSolarCommands[] PROGMEM = D_CMND_PIP_PREFIX "|"
    D_CMND_PIP_QT "|"
    D_CMND_PIP_QET "|"
    D_CMND_PIP_QEY "|"
    D_CMND_PIP_QEM "|"
    D_CMND_PIP_QED "|"
    D_CMND_PIP_QEH "|"
    D_CMND_PIP_DAT "|"
    D_CMND_PIP_QPIRI "|"
    D_CMND_PIP_QFLAG "|"
    D_CMND_PIP_CUSTOM "|"
    D_CMND_PIP_POLLVALUES "|"
    D_CMND_PIP_BAUDRATE "|"
    D_CMND_PIP_SERIALCONFIG;

void (* const PipSolarCommand[])(void) PROGMEM = {
  &CmndPipSolarQT,
  &CmndPipSolarQET,
  &CmndPipSolarQEY,
  &CmndPipSolarQEM,
  &CmndPipSolarQED,
  &CmndPipSolarQEH,
  &CmndPipSolarDAT,
  &CmndPipSolarQPIRI,
  &CmndPipSolarQFLAG,
  &CmndPipSolarCUSTOM,
  &CmndPipSolarPollValues,
  &CmndPipSolarBaudRate,
  &CmndPipSolarSerialConfig
  };


void PIPSOLARPublishResult(const char *subtopic, const char *payload, const char *parameter)
{
  char buffer[150];
  snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\": \"%s\", \"parameter\": \"%s\"}}"), subtopic, payload, parameter);
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: RESULT %s"), (const char*)buffer);
  MqttPublishPayloadPrefixTopic_P(RESULT_OR_STAT, subtopic, (const char*)buffer);
  XdrvRulesProcess(0, buffer);
}

void PIPSOLARPublishResult(const char *subtopic, int payload, const char *parameter)
{
  char buffer[150];
  snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\": %d, \"parameter\": \"%s\"}}"), subtopic, payload, parameter);
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: RESULT %s"), (const char*)buffer);
  MqttPublishPayloadPrefixTopic_P(RESULT_OR_STAT, subtopic, (const char*)buffer);
  XdrvRulesProcess(0, buffer);
}

void PIPSOLARPublishResult(const char *subtopic, const char* payload)
{
  char buffer[150];
  snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\": \"%s\"}}"), subtopic, payload);
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: RESULT %s"), (const char*)buffer);
  MqttPublishPayloadPrefixTopic_P(RESULT_OR_STAT, subtopic, (const char*)buffer);
  XdrvRulesProcess(0, buffer);
}

void PIPSOLARPublishResult(const char *subtopic, int payload)
{
  char buffer[150];
  snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\": %d}}"), subtopic, payload);
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: RESULT %s"), (const char*)buffer);
  MqttPublishPayloadPrefixTopic_P(RESULT_OR_STAT, subtopic, (const char*)buffer);
  XdrvRulesProcess(0, buffer);
}

void PIPSOLARPublishRaw(const char *subtopic, const char *payload, bool usebracket = false)
{
  MqttPublishPayloadPrefixTopic_P(STAT, subtopic, payload);
  char buffer[300];
  if(usebracket)
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\":\"%s\"}"), subtopic, payload);
  else
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\":%s}"), subtopic, payload);
  
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: PUBLISH %s"), (const char*)buffer);
  XdrvRulesProcess(0, buffer);
}

void PIPSOLARPublish(const char *subtopic, const char *value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  if (json) {
    char buffer[150];
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\":{\"value\":\"%s\",\"unit\":\"%s\"}}"), name, value, unit);
    PIPSOLARPublishRaw(subtopic, buffer);
  } else {
    PIPSOLARPublishRaw(subtopic, value, true);
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: PUBLISH %s"), (const char*)buffer);
  
}

void PIPSOLARPublish(const char *subtopic, int value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  char buffer[150];
  if (json) {
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\":%d,\"unit\":\"%s\"}}"), name, value, unit);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%d"), value);
  }
  PIPSOLARPublishRaw(subtopic, buffer);
}
void PIPSOLARPublish(const char *subtopic, uint32_t value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  char buffer[150];
  if (json) {
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\":%u,\"unit\":\"%s\"}}"), name, value, unit);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%u"), value);
  }
  PIPSOLARPublishRaw(subtopic, buffer);
}

void PIPSOLARPublish(const char *subtopic, float value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  char buffer[150];
  if (json) {
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\":%.1f,\"unit\":\"%s\"}}"), name, value, unit);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%.1f"), value);
  }
  PIPSOLARPublishRaw(subtopic, buffer);
}

void PIPSOLARPublish(const char *subtopic, char value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  char buffer[150];
  if (json) {
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\":\"%c\",\"unit\":\"%s\"}}"), name, value, unit);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%c"), value);
  }
  PIPSOLARPublish(subtopic, buffer);
}

void PIPSOLARPublish(const char *subtopic, bool value, bool json = false, const char *name = nullptr, const char *unit = nullptr)
{
  char buffer[150];
  if (json) {
    snprintf_P(buffer, sizeof(buffer), PSTR("{\"%s\": {\"value\":%s,\"unit\":\"%s\"}}"), name, (value ? PSTR("true") : PSTR("false")), unit);
  } else {
    snprintf_P(buffer, sizeof(buffer), PSTR("%s"), (value ? PSTR("true") : PSTR("false")));
  }
  PIPSOLARPublishRaw(subtopic, buffer);
}

void PIPSOLARQFlagPublish(const char *subtopic)
{
  char buffer[150] = "{";
  uint8_t position = 1;
  for(int i = 0; i < PIPSOLARqflagValueSettingsCount; ++i) {
    int8_t addition = 0;
    if (PIPSOLARqflagValueSettings[i].value == PipSolar::BoolFlag::True) {
      addition = snprintf_P(buffer+position, sizeof(buffer)-position, PSTR("\"%s\":true,"), PIPSOLARqflagValueSettings[i].name);
    } else if (PIPSOLARqflagValueSettings[i].value == PipSolar::BoolFlag::False) {
      addition = snprintf_P(buffer+position, sizeof(buffer)-position, PSTR("\"%s\":false,"), PIPSOLARqflagValueSettings[i].name);
    }
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("qflag: buffer:\"%s\" position:%d"), buffer, position);
    if (position+addition > sizeof(buffer)-1) {
      buffer[position-1] = '}';
      buffer[position] = '\0';
      PIPSOLARPublishRaw(subtopic, buffer);

      // process the last one again
      --i;
      // make room for the missing messages
      buffer[1] = 0;
    }
    position = strlen(buffer);
  }
  if (position == 1) {
    buffer[1] = '}';
    buffer[2] = '\0';
  } else {
    buffer[position-1] = '}';
  }
  PIPSOLARPublishRaw(subtopic, buffer);
}

// crc function is from here: https://forum.arduino.cc/t/rs232-read-data-from-mpp-solar-inverter/600960/6
uint16_t PIPSOLARCalcCrc(const uint8_t *pin, uint8_t len)
{
  uint16_t crc;
  uint8_t da;
  const uint8_t *ptr;
  uint8_t bCRCHign;
  uint8_t bCRCLow;

  const unsigned short crc_ta[16] =
      {
          0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
          0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};

  ptr = pin;
  crc = 0;
  while (len-- != 0)
  {
    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= crc_ta[da ^ (*ptr >> 4)];
    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= crc_ta[da ^ (*ptr & 0x0f)];
    ptr++;
  }
  bCRCLow = crc;
  bCRCHign = (uint8_t)(crc >> 8);
  if (bCRCLow == 0x28 || bCRCLow == 0x0d || bCRCLow == 0x0a)
  {
    bCRCLow++;
  }
  if (bCRCHign == 0x28 || bCRCHign == 0x0d || bCRCHign == 0x0a)
  {
    bCRCHign++;
  }
  crc = ((unsigned short)bCRCHign) << 8;
  crc += bCRCLow;
  return (crc);
}

bool PIPSOLARSendCommand(PipSolar::CommandType cmd, const char *parameter, bool checksum)
{
  const char *command;
  switch (cmd)
  {
  case PipSolar::CommandType::NONE:
    return false;
  case PipSolar::CommandType::QPIGS:
    command = PSTR(D_CMND_PIP_QPIGS);
    break;
  case PipSolar::CommandType::QMOD:
    command = PSTR(D_CMND_PIP_QMOD);
    break;
  case PipSolar::CommandType::QPIWS:
    command = PSTR(D_CMND_PIP_QPIWS);
    break;
  case PipSolar::CommandType::QT:
    command = PSTR(D_CMND_PIP_QT);
    break;
  case PipSolar::CommandType::QET:
    command = PSTR(D_CMND_PIP_QET);
    break;
  case PipSolar::CommandType::QEY:
    command = PSTR(D_CMND_PIP_QEY);
    break;
  case PipSolar::CommandType::QEM:
    command = PSTR(D_CMND_PIP_QEM);
    break;
  case PipSolar::CommandType::QED:
    command = PSTR(D_CMND_PIP_QED);
    break;
  case PipSolar::CommandType::QEH:
    command = PSTR(D_CMND_PIP_QEH);
    break;
  case PipSolar::CommandType::DAT:
    command = PSTR(D_CMND_PIP_DAT);
    break;
  case PipSolar::CommandType::QPIRI:
    command = PSTR(D_CMND_PIP_QPIRI);
    break;
  case PipSolar::CommandType::QFLAG:
    command = PSTR(D_CMND_PIP_QFLAG);
    break;
  case PipSolar::CommandType::CUSTOM:
    command = parameter;
    parameter = nullptr;
    break;
  }
  PIPSOLAR.commandType = cmd;
  PIPSOLAR.commandState = PipSolar::CommandState::WaitForResponse;
  PIPSOLAR.commandTimeoutCounter = 0;

  uint8_t len = strlen(command);
  memcpy(PIPSOLAR.sendBuffer, command, len);
  if (parameter)
  {
    uint8_t parameterLen = strlen(parameter);
    memcpy(PIPSOLAR.sendBuffer + len, parameter, parameterLen);
    len += parameterLen;
    if (checksum) {
      uint8_t checksumValue = 0;
      for (uint8_t i = 0; i < len; ++i)
        checksumValue += PIPSOLAR.sendBuffer[i];
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: Checksum %d len %d"), checksumValue, len+3);
      sprintf(reinterpret_cast<char*>(PIPSOLAR.sendBuffer + len), "%03u", checksumValue);
      len += 3;
    }
  }

  PIPSOLAR.sendBuffer[len] = 0;
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: Send %s"), PIPSOLAR.sendBuffer);

  uint16_t crc16 = PIPSOLARCalcCrc(PIPSOLAR.sendBuffer, len);
  PIPSOLAR.sendBuffer[len++] = ((uint8_t)((crc16) >> 8));
  PIPSOLAR.sendBuffer[len++] = ((uint8_t)((crc16)&0xff));
  PIPSOLAR.sendBuffer[len++] = '\r';


  // PIPSOLAR.serial->setReadChunkMode(1);                            // Enable chunk mode introducing possible Hardware Watchdogs
  PIPSOLAR.serial->flush();
  PIPSOLAR.serial->write(PIPSOLAR.sendBuffer, len);
  PIPSOLAR.commandTimeoutCounter = 0;
  return false;
}

const char*  PIPSOLARGetValueString(char *data, int8_t position, uint8_t count)
{
  data[position+count] = 0;
  return &data[position];
}

static char PIPSOLARGetValueStringCopyString[10];
const char*  PIPSOLARGetValueStringCopy(char *data, int8_t position, uint8_t count)
{
  PIPSOLARGetValueStringCopyString[count] = 0;
  memcpy((void*)PIPSOLARGetValueStringCopyString, &data[position], count);
  
  return PIPSOLARGetValueStringCopyString;
}

void PIPSOLARQFlagInterpret(const char *flag)
{
  bool currentValue = true;
  while(*flag != '\0') {
    switch(*flag) {
      case 'E':
        currentValue = true;
        break;
      case 'D':
        currentValue = false;
        break;
      default:
        for(int i = 0; i < PIPSOLARqflagValueSettingsCount; ++i) {
          if(PIPSOLARqflagValueSettings[i].identifier == *flag) {
            if(currentValue == true) {
              PIPSOLARqflagValueSettings[i].value = PipSolar::BoolFlag::True;
            } else {
              PIPSOLARqflagValueSettings[i].value = PipSolar::BoolFlag::False;
            }
            break;
          }
        }
        break;
    }
    flag++;
  }
}

void PIPSOLARInterpret(struct PipSolar::ValueSetting *settings, uint8_t count, bool json = false, const char *command = nullptr)
{
  //(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
  for (uint8_t i = 0; i < count; ++i)
  {
    auto & setting = settings[i];

    switch (setting.type)
    {
    case PipSolar::ValueSetting::Type::intType: {
      const char* temp = PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, setting.position, setting.count);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d %s %s"), __LINE__, setting.name, temp);
      setting.value.intValue = atoi(temp);
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.intValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.intValue);
        }
      break;
    }
    case PipSolar::ValueSetting::Type::intTypeCopy: {
      const char* temp = PIPSOLARGetValueStringCopy((char*)PIPSOLAR.receiveBuffer, setting.position, setting.count);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d %s %s"), __LINE__, setting.name, temp);
      setting.value.intValue = atoi(temp);
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.intValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.intValue);
        }
      break;
    }
    case PipSolar::ValueSetting::Type::floatType: {
      const char* temp = PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, setting.position, setting.count);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d %s %s"), __LINE__, setting.name, temp);
      setting.value.floatValue = atof(temp);
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.floatValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.floatValue);
        }
      break;
    }
    case PipSolar::ValueSetting::Type::floatTypeCopy: {
      const char* temp = PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, setting.position, setting.count);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d %s %s"), __LINE__, setting.name, temp);
      setting.value.floatValue = atof(temp);
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.floatValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.floatValue);
        }
      break;
    }
    case PipSolar::ValueSetting::Type::boolType:
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
      setting.value.boolValue = PIPSOLAR.receiveBuffer[setting.position] == '1';
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.boolValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.boolValue);
        }
      break;
    case PipSolar::ValueSetting::Type::charType:
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
      setting.value.charValue = PIPSOLAR.receiveBuffer[setting.position];
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, setting.value.charValue, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, setting.value.charValue);
        }
      break;
    case PipSolar::ValueSetting::Type::stringType:
      const char* temp = PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, setting.position, setting.count);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
      if (setting.publish)
        if (json) {
          PIPSOLARPublish(command, temp, true, setting.name, setting.unit);
        } else {
          PIPSOLARPublish(setting.name, temp);
        }
      break;
    }
  }
}

void PIPSOLARInterpret(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d %d"), __LINE__, PIPSOLAR.commandType);
  if (PIPSOLAR.receiveBuffer[0] != '(')
    return;
  bool wasQuery = (PIPSOLAR.commandType == PIPSOLAR.queryCommand.commandType && PIPSOLAR.queryCommand.send);
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
  switch (PIPSOLAR.commandType)
  {
  case PipSolar::CommandType::NONE:
    break;
  case PipSolar::CommandType::QPIGS: {
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d %d"), __FUNCTION__, __LINE__, PIPSOLAR.receiveBufferPosition);
    if (PIPSOLAR.receiveBufferPosition != 107)
      return;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d"), __FUNCTION__, __LINE__);
    PIPSOLARInterpret(PIPSOLARqpigsValueSettings, PIPSOLARqpigsValueSettingsCount);
    int batteryCurrent = PIPSOLARqpigsValueSettings[9].value.intValue - PIPSOLARqpigsValueSettings[15].value.intValue;
    PIPSOLARPublish(PSTR("Battery_current"), batteryCurrent);
    PIPSOLARPublish(PSTR("Battery_power"), PIPSOLARqpigsValueSettings[8].value.floatValue * batteryCurrent);
    char buffer[8+3+1];
    auto &deviceState1 = PIPSOLARqpigsValueSettings[16];
    auto &deviceState2 = PIPSOLARqpigsValueSettings[20];
    snprintf_P(buffer, sizeof(buffer), PSTR("%3s%8s")
    , PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, deviceState2.position, deviceState2.count)
    , PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, deviceState1.position, deviceState1.count));
    PIPSOLARPublish(PSTR("Device_state"), buffer);
    }
    break;
  case PipSolar::CommandType::QMOD:
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
    if (PIPSOLAR.receiveBufferPosition != 2)
      return;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
    PIPSOLARInterpret(PIPSOLARqmodValueSettings, PIPSOLARqmodValueSettingsCount);
    break;
  case PipSolar::CommandType::QPIWS:
    PIPSOLARPublish(PSTR("Device_warning_state"), PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 35));
    break;
  case PipSolar::CommandType::QT:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QT), PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 14));
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: line %d"), __LINE__);
    PIPSOLARInterpret(PIPSOLARqtValueSettings, PIPSOLARqtValueSettingsCount);
    PIPSOLARPublish(PSTR("Inverter_date_time"), PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 14));
    break;
  case PipSolar::CommandType::QET:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QET), atoi(PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 8)));
    PIPSOLARInterpret(PIPSOLARqexValueSettings, PIPSOLARqexValueSettingsCount);
    break;
  case PipSolar::CommandType::QEY:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QEY), atoi(PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 8)), PIPSOLAR.queryCommand.parameter);
    else
      PIPSOLARInterpret(PIPSOLARqexValueSettings, PIPSOLARqexValueSettingsCount);
    break;
  case PipSolar::CommandType::QEM:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QEM), atoi(PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 8)), PIPSOLAR.queryCommand.parameter);
    else
      PIPSOLARInterpret(PIPSOLARqexValueSettings, PIPSOLARqexValueSettingsCount);
    break;
  case PipSolar::CommandType::QED:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QED), atoi(PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 8)), PIPSOLAR.queryCommand.parameter);
    else
      PIPSOLARInterpret(PIPSOLARqexValueSettings, PIPSOLARqexValueSettingsCount);
    break;
  case PipSolar::CommandType::QEH:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_QEH), atoi(PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 8)), PIPSOLAR.queryCommand.parameter);
    else
      PIPSOLARInterpret(PIPSOLARqexValueSettings, PIPSOLARqexValueSettingsCount);
    break;
  case PipSolar::CommandType::DAT:
    if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_DAT), PIPSOLARGetValueString((char*)PIPSOLAR.receiveBuffer, 1, 3), PIPSOLAR.queryCommand.parameter);
    break;
  case PipSolar::CommandType::QPIRI:
  {
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d %d"), __FUNCTION__, __LINE__, PIPSOLAR.receiveBufferPosition);
    if (PIPSOLAR.receiveBufferPosition != 100)
      return;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d"), __FUNCTION__, __LINE__);
    const char qpiri[] = PSTR(D_CMND_PIP_QPIRI);
    PIPSOLARInterpret(PIPSOLARqpiriValueSettings, PIPSOLARqpiriValueSettingsCount, true, qpiri);
    break;
  }
  case PipSolar::CommandType::QFLAG:
    //if (wasQuery)
    PIPSOLARQFlagInterpret((char*)PIPSOLAR.receiveBuffer);
    PIPSOLARQFlagPublish(PSTR(D_CMND_PIP_QFLAG));
    break;
  case PipSolar::CommandType::CUSTOM:
    //if (wasQuery)
      PIPSOLARPublishResult(PSTR(D_CMND_PIP_CUSTOM), (char*)PIPSOLAR.receiveBuffer, PIPSOLAR.queryCommand.parameter);
    break;
  }

  if (wasQuery) {
    PIPSOLAR.queryCommand.commandType = PipSolar::CommandType::NONE;
    PIPSOLAR.queryCommand.parameter[0] = 0;
    PIPSOLAR.queryCommand.send = false;
  }

  PIPSOLAR.receiveBufferPosition = 0;
  if (!PIPSOLAR.pollValues) {
    PIPSOLAR.commandState = PipSolar::CommandState::None;
    PIPSOLAR.commandType = PipSolar::CommandType::NONE;
  }
}

void PIPSOLARInput(void)
{
  while (PIPSOLAR.serial->available())
  {
    uint8_t bytesAvailable = PIPSOLAR.serial->read(PIPSOLAR.receiveBuffer + PIPSOLAR.receiveBufferPosition, PIPSOLAR_RECEIVEBUFFER_SIZE - PIPSOLAR.receiveBufferPosition);
    PIPSOLAR.receiveBufferPosition += bytesAvailable;
    if (PIPSOLAR.receiveBufferPosition == PIPSOLAR_RECEIVEBUFFER_SIZE) { // buffer overflow
      ++PIPSOLAR.currentErrorPoll;
      PIPSOLAR.maxCurrentErrorPoll = std::max(PIPSOLAR.maxCurrentErrorPoll, PIPSOLAR.currentErrorPoll);
      ++PIPSOLAR.errorPoll;
      PIPSOLAR.receiveBufferPosition = 0; // we are in trouble, but keep it going
      PIPSOLARPublish(PSTR("errortext"),PSTR("Buffer overflow"));
      PIPSOLARPublish(PSTR("currentError"),PIPSOLAR.currentErrorPoll);
    }
  }
  if (PIPSOLAR.receiveBufferPosition > 2 && PIPSOLAR.receiveBuffer[PIPSOLAR.receiveBufferPosition - 1] == '\r')
  { // we hope that we always have a complete reply
    bool valid = true;
    uint16_t crc16 = PIPSOLARCalcCrc(PIPSOLAR.receiveBuffer, PIPSOLAR.receiveBufferPosition - 3);
    uint8_t crcLow = PIPSOLAR.receiveBuffer[PIPSOLAR.receiveBufferPosition - 2];
    uint8_t crcHigh = PIPSOLAR.receiveBuffer[PIPSOLAR.receiveBufferPosition - 3];
    if (((uint8_t)((crc16)&0xff)) != crcLow || ((uint8_t)((crc16) >> 8)) != crcHigh)
      valid = false;

    PIPSOLAR.receiveBufferPosition -= 3; // remove crc and \r
    PIPSOLAR.receiveBuffer[PIPSOLAR.receiveBufferPosition] = 0; // terminate string
    if (valid)
    {
      ++PIPSOLAR.successPoll;

      if (PIPSOLAR.currentErrorPoll != 0 || (PIPSOLAR.successPoll % 100) == 0) {
        PIPSOLAR.currentErrorPoll = 0;
        PIPSOLARPublish(PSTR("errortext"),PSTR("Success"));
        PIPSOLARPublish(PSTR("currentError"),PIPSOLAR.currentErrorPoll);
      }

      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: Received valid data \"%s\" timeout %d"), PIPSOLAR.receiveBuffer, PIPSOLAR.commandTimeoutCounter);
      PIPSOLARInterpret();
      PIPSOLAR.commandState = PipSolar::CommandState::GotResponse;
    } else {
      ++PIPSOLAR.currentErrorPoll;
      PIPSOLAR.maxCurrentErrorPoll = std::max(PIPSOLAR.maxCurrentErrorPoll, PIPSOLAR.currentErrorPoll);
      ++PIPSOLAR.errorPoll;
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: Received invalid data \"%s\" crc %02x %02x expected %2x %2x timeout %d"), PIPSOLAR.receiveBuffer, crcLow, crcHigh, ((uint8_t)((crc16)&0xff)), ((uint8_t)((crc16) >> 8)), PIPSOLAR.commandTimeoutCounter);
      PIPSOLAR.receiveBufferPosition = 0;
      PIPSOLAR.commandState = PipSolar::CommandState::None;
      PIPSOLAR.commandType = PipSolar::CommandType::NONE;
      PIPSOLARPublish(PSTR("errortext"),PSTR("Crc error"));
      PIPSOLARPublish(PSTR("currentError"),PIPSOLAR.currentErrorPoll);
    }
    PIPSOLAR.commandTimeoutCounter = -1;
  }
  if (PIPSOLAR.pollValues && PIPSOLAR.commandType != PipSolar::CommandType::NONE) {
    
    switch (PIPSOLAR.commandState) {
      case PipSolar::CommandState::None:
        break;
      case PipSolar::CommandState::WaitForResponse:
        if (PIPSOLAR.commandTimeoutCounter > PIPSOLARpollingList[PIPSOLARpollingValuePosition].timeout) {
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: timeout WaitForResponse %d"), PIPSOLARpollingValuePosition, PIPSOLAR.commandTimeoutCounter, PIPSOLARpollingList[PIPSOLARpollingValuePosition].timeout);
          PIPSOLAR.commandTimeoutCounter = 0;
          PIPSOLAR.commandState = PipSolar::CommandState::None;
          PIPSOLAR.commandType = PipSolar::CommandType::NONE;
          PIPSOLAR.receiveBufferPosition = 0;
          ++PIPSOLAR.currentErrorPoll;
          PIPSOLARPublish(PSTR("errortext"),PSTR("Timeout WaitForResponse"));
          PIPSOLARPublish(PSTR("currentError"),PIPSOLAR.currentErrorPoll);
          PIPSOLAR.maxCurrentErrorPoll = std::max(PIPSOLAR.maxCurrentErrorPoll, PIPSOLAR.currentErrorPoll);
          ++PIPSOLAR.errorPoll;
          PIPSOLARNextPolling();
        }
        break;
      case PipSolar::CommandState::GotResponse:
        if (PIPSOLAR.commandTimeoutCounter > PIPSOLARpollingList[PIPSOLARpollingValuePosition].waitAfterResponse) {
          PIPSOLAR.commandTimeoutCounter = 0;
          PIPSOLAR.commandState = PipSolar::CommandState::None;
          PIPSOLAR.commandType = PipSolar::CommandType::NONE;
          PIPSOLARNextPolling();
        }
        break;
    }
    ++PIPSOLAR.commandTimeoutCounter;
  }
}

/********************************************************************************************/

void PIPSOLARNextPolling() {
  if (PIPSOLAR.queryCommand.commandType != PipSolar::CommandType::NONE
    && !PIPSOLAR.queryCommand.send) {
    switch(PIPSOLAR.queryCommand.commandType) {
    case PipSolar::CommandType::QT:
    case PipSolar::CommandType::QET:
    case PipSolar::CommandType::QPIRI:
    case PipSolar::CommandType::QFLAG:
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: query without parameter"));
      PIPSOLARSendCommand(PIPSOLAR.queryCommand.commandType);
      PIPSOLAR.queryCommand.send = true;
      return;
    case PipSolar::CommandType::QEY:
    case PipSolar::CommandType::QEM:
    case PipSolar::CommandType::QED:
    case PipSolar::CommandType::QEH:
    case PipSolar::CommandType::DAT:
    case PipSolar::CommandType::CUSTOM:
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: query with parameter"));
      PIPSOLARSendCommand(PIPSOLAR.queryCommand.commandType, PIPSOLAR.queryCommand.parameter);
      PIPSOLAR.queryCommand.send = true;
      return;
    default: // unsupported query
      PIPSOLAR.queryCommand.send = false;
      PIPSOLAR.queryCommand.commandType = PipSolar::CommandType::NONE;
      PIPSOLAR.queryCommand.parameter[0] = 0;
      break;
    }
  }
  while(PIPSOLARpollingValuePosition < PIPSOLARpollingListCount) {
    if (!PIPSOLARpollingList[PIPSOLARpollingValuePosition].poll) {
      ++PIPSOLARpollingValuePosition;
      continue;
    }
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %d"), __LINE__);
    auto &command = PIPSOLARpollingList[PIPSOLARpollingValuePosition].command;
    switch(command) { // TODO add the others with parameter
    case PipSolar::CommandType::QPIWS:
    case PipSolar::CommandType::QPIGS:
    case PipSolar::CommandType::QMOD:
    case PipSolar::CommandType::QT:
    case PipSolar::CommandType::QET:
      PIPSOLARSendCommand(command);
      PIPSOLARpollingList[PIPSOLARpollingValuePosition].poll = false;
      break;
    case PipSolar::CommandType::NONE:
      break;
    }
    return;
  }
  PIPSOLARpollingValuePosition = 0;
}

void PIPSOLAREverySecond(void)
{
  if (PIPSOLAR.pollValues) {
    for(int i = 0; i < PIPSOLARpollingListCount; ++i) {
      if (PIPSOLARpollingList[i].interval != 0 && (PIPSOLAR.secondIntervalCounter % PIPSOLARpollingList[i].interval) == 0) {
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: schedule %d"), i);
        PIPSOLARpollingList[i].poll = true;
      }
    }
    if (PIPSOLAR.commandType == PipSolar::CommandType::NONE) {
      PIPSOLARpollingValuePosition = 0;
      PIPSOLARNextPolling();
    }
    ++PIPSOLAR.secondIntervalCounter;
  }
}

void PIPSOLARInit(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d"), __FUNCTION__, __LINE__);
  if (PinUsed(GPIO_PIPSOLAR_RX) && PinUsed(GPIO_PIPSOLAR_TX))
  {
    PIPSOLAR.pollValues = true;
    PIPSOLAR.receiveBuffer = (uint8_t *)malloc(PIPSOLAR_RECEIVEBUFFER_SIZE);
    PIPSOLAR.sendBuffer = (uint8_t *)malloc(PIPSOLAR_SENDBUFFER_SIZE);
    PIPSOLAR.receiveBufferPosition = 0;
    PIPSOLAR.secondIntervalCounter = 0;
    PIPSOLAR.commandType = PipSolar::CommandType::NONE;
    PIPSOLAR.errorPoll = 0;
    PIPSOLAR.successPoll = 0;
    PIPSOLAR.currentErrorPoll = 0;
    PIPSOLAR.queryCommand.commandType = PipSolar::CommandType::NONE;
    PIPSOLAR.queryCommand.send = false;

    if (!PIPSOLAR.receiveBuffer || !PIPSOLAR.sendBuffer)
    {
      return;
    }

    PIPSOLAR.receiveBuffer[0] = 0u;
    PIPSOLAR.sendBuffer[0] = 0u;

    PIPSOLAR.serial = new TasmotaSerial(Pin(GPIO_PIPSOLAR_RX), Pin(GPIO_PIPSOLAR_TX), 2);
    if (SetPipSolarSerialBegin())
    {
      if (PIPSOLAR.serial->hardwareSerial())
      {
        ClaimSerial();
      }
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void PIPSOLARShowWeb(const PipSolar::ValueSetting &setting)
{
  switch (setting.type)
  {
  case PipSolar::ValueSetting::Type::intTypeCopy:
  case PipSolar::ValueSetting::Type::intType:
    WSContentSend_PD(PSTR("{s}%s{m}%d %s{e}"), setting.name, setting.value.intValue, setting.unit);
    break;
  case PipSolar::ValueSetting::Type::floatTypeCopy:
  case PipSolar::ValueSetting::Type::floatType:
    WSContentSend_PD(PSTR("{s}%s{m}%.1f %s{e}"), setting.name, setting.value.floatValue, setting.unit);
    break;
  case PipSolar::ValueSetting::Type::boolType:
    WSContentSend_PD(PSTR("{s}%s{m}%s %s{e}"), setting.name, (setting.value.boolValue ? PSTR("true") : PSTR("false")), setting.unit);
    break;
  case PipSolar::ValueSetting::Type::charType:
    WSContentSend_PD(PSTR("{s}%s{m}%c %s{e}"), setting.name, setting.value.charValue, setting.unit);
    break;
  }
}
void PIPSOLARShow(bool json)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PIPSOLAR: %s %d"), __FUNCTION__, __LINE__);
  if (json)
  { //    //                                                             cm   cm   cm                          %  %
    ResponseAppend_P(PSTR(",\"PIPSOLAR\":1"));
#ifdef USE_WEBSERVER
  }
  else
  {
    WSContentSend_PD(PSTR("{s}<h2>QT</h2>{e}"));
    WSContentSend_PD(PSTR("{s}InverterTime{m}%02d:%02d:%02d %02d.%02d.%04d{e}"),
      PIPSOLARqtValueSettings[3].value.intValue,
      PIPSOLARqtValueSettings[4].value.intValue,
      PIPSOLARqtValueSettings[5].value.intValue,
      PIPSOLARqtValueSettings[2].value.intValue,
      PIPSOLARqtValueSettings[1].value.intValue,
      PIPSOLARqtValueSettings[0].value.intValue
    );
    WSContentSend_PD(PSTR("{s}<h2>QEx</h2>{e}"));
    for (int i = 0; i < PIPSOLARqexValueSettingsCount; ++i)
    {
      auto &setting = PIPSOLARqexValueSettings[i];
      PIPSOLARShowWeb(setting);
    }
    WSContentSend_PD(PSTR("{s}<h2>QMOD</h2>{e}"));
    for (int i = 0; i < PIPSOLARqmodValueSettingsCount; ++i)
    {
      auto &setting = PIPSOLARqmodValueSettings[i];
      PIPSOLARShowWeb(setting);
    }
    WSContentSend_PD(PSTR("{s}<h2>QPIGS</h2>{e}"));
    for (int i = 0; i < PIPSOLARqpigsValueSettingsCount; ++i)
    {
      auto &setting = PIPSOLARqpigsValueSettings[i];
      PIPSOLARShowWeb(setting);
    }
    float percentError = 100.0 * PIPSOLAR.errorPoll / (PIPSOLAR.errorPoll + PIPSOLAR.successPoll);
    WSContentSend_PD(PSTR("{s}<h2>ERROR</h2>{m}Success: %u<br>Error: %u<br>CurrentError: %u<br>Max CurrentError: %u<br>ErrorRate %2_f %%{e}"),
      PIPSOLAR.successPoll, PIPSOLAR.errorPoll, PIPSOLAR.currentErrorPoll, PIPSOLAR.maxCurrentErrorPoll, &percentError);
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv72(uint32_t function)
{
  bool result = true;

  if (PIPSOLAR.serial || function == FUNC_INIT)
  {
    switch (function)
    {
    case FUNC_COMMAND:
      result = DecodeCommand(kPipSolarCommands, PipSolarCommand);
      break;

    case FUNC_INIT:
      PIPSOLARInit();
      break;
    case FUNC_LOOP:
    case FUNC_SLEEP_LOOP:
      PIPSOLARInput();
      break;
    case FUNC_EVERY_50_MSECOND:
      break;
    case FUNC_EVERY_SECOND:
      PIPSOLAREverySecond();
      break;
    case FUNC_JSON_APPEND:
      PIPSOLARShow(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      PIPSOLARShow(false);
      break;
#endif
    }
  }
  return result;
}

#endif // USE_PIPSOLAR
