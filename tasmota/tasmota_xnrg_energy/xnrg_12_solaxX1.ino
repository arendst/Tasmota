/*
  xnrg_12_solaxX1.ino - Solax X1 inverter RS485 support for Tasmota

  Copyright (C) 2021 by Pablo Zerón
  Copyright (C) 2024 by Stefan Wershoven

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
#ifdef USE_SOLAX_X1
/*********************************************************************************************\
 * Solax X1 Inverter
\*********************************************************************************************/

#define XNRG_12            12

#ifndef SOLAXX1_SPEED
#define SOLAXX1_SPEED      9600      // default solax rs485 speed
#endif

// #define SOLAXX1_READCONFIG          // enable to read inverters config; disable to save codespace (3k1)

#define INVERTER_ADDRESS   0x0A

#define D_SOLAX_X1         "SolaxX1"

#include <TasmotaSerial.h>

const char kSolaxMode[] PROGMEM =
  D_OFF "|" D_SOLAX_MODE_0 "|" D_SOLAX_MODE_1 "|" D_SOLAX_MODE_2 "|" D_SOLAX_MODE_3 "|"  D_SOLAX_MODE_4 "|"
  D_SOLAX_MODE_5 "|" D_SOLAX_MODE_6;

const char kSolaxError[] PROGMEM =
  D_SOLAX_ERROR_0 "|" D_SOLAX_ERROR_1 "|" D_SOLAX_ERROR_2 "|" D_SOLAX_ERROR_3 "|" D_SOLAX_ERROR_4 "|" D_SOLAX_ERROR_5 "|"
  D_SOLAX_ERROR_6 "|" D_SOLAX_ERROR_7 "|" D_SOLAX_ERROR_8;

#ifdef SOLAXX1_READCONFIG
const char kSolaxSafetyType[] PROGMEM =
  "VDE 0126|VDE-AR-N 4105|AS 4777|G98|C10/11|ÖVE/ÖNORM E 8001|EN 50438 NL|EN 50438 DK|CEB|CEI021|NRS 097-2-1|VDE 0126 Greece/Iceland|"
  "UTE C15-712|IEC 61727|G99|VDE 0126 Greece/Co|Guyana|C15-712 France/Iceland 50|C15-712 France/Iceland 60|New Zeeland|RD1699|Chile|"
  "EN 50438 Ireland|Philippines|Czech PPDS|Czech 50438|EN 50549 EU|Denmark 2019 EU|RD 1699 Island|EN50549 Poland|MEA Thailand|"
  "PEA Thailand|ACEA|AS 4777 2020 B|AS 4777 2020 C|Sri Lanka|BRAZIL 240|EN 50549 SK|EN 50549 EU|G98/NI|Denmark 2019 EU|RD 1699 Island";
#endif // SOLAXX1_READCONFIG

union {
  uint32_t ErrMessage;
  struct {
    //BYTE0
    uint8_t TzProtectFault:1;//0
    uint8_t MainsLostFault:1;//1
    uint8_t GridVoltFault:1;//2
    uint8_t GridFreqFault:1;//3
    uint8_t PLLLostFault:1;//4
    uint8_t BusVoltFault:1;//5
    uint8_t ErrBit06:1;//6
    uint8_t OciFault:1;//7
    //BYTE1
    uint8_t Dci_OCP_Fault:1;//8
    uint8_t ResidualCurrentFault:1;//9
    uint8_t PvVoltFault:1;//10
    uint8_t Ac10Mins_Voltage_Fault:1;//11
    uint8_t IsolationFault:1;//12
    uint8_t TemperatureOverFault:1;//13
    uint8_t FanFault:1;//14
    uint8_t ErrBit15:1;//15
    //BYTE2
    uint8_t SpiCommsFault:1;//16
    uint8_t SciCommsFault:1;//17
    uint8_t ErrBit18:1;//18
    uint8_t InputConfigFault:1;//19
    uint8_t EepromFault:1;//20
    uint8_t RelayFault:1;//21
    uint8_t SampleConsistenceFault:1;//22
    uint8_t ResidualCurrent_DeviceFault:1;//23
    //BYTE3
    uint8_t ErrBit24:1;//24
    uint8_t ErrBit25:1;//25
    uint8_t ErrBit26:1;//26
    uint8_t ErrBit27:1;//27
    uint8_t ErrBit28:1;//28
    uint8_t DCI_DeviceFault:1;//29
    uint8_t OtherDeviceFault:1;//30
    uint8_t ErrBit31:1;//31
  };
} solaxX1_ErrCode;

struct SOLAXX1_LIVEDATA {
  int16_t temperature = 0;
  float energy_today = 0;
  float dc1_voltage = 0;
  float dc2_voltage = 0;
  float dc1_current = 0;
  float dc2_current = 0;
  uint32_t runtime_total = 0;
  float dc1_power = 0;
  float dc2_power = 0;
  int16_t runMode = 0;
  uint32_t errorCode = 0;
  uint8_t SerialNumber[16] = {0x00};
} solaxX1;

struct SOLAXX1_GLOBALDATA {
  bool AddressAssigned = true;
  uint8_t SendRetry_count = 20;
  uint8_t QueryData_count = 0;
  uint8_t QueryID_count = 240;
  bool Command_QueryID = false;;
  bool Command_QueryConfig = false;
} solaxX1_global;

struct SOLAXX1_SENDDATA {
  uint8_t Header[2] = {0xAA, 0x55};
  uint8_t Source[2] = {0x00, 0x00};
  uint8_t Destination[2] = {0x00, 0x00};
  uint8_t ControlCode[1] = {0x00};
  uint8_t FunctionCode[1] = {0x00};
  uint8_t DataLength[1] = {0x00};
  uint8_t Payload[16] = {0x00};
} solaxX1_SendData;

TasmotaSerial *solaxX1Serial;

/*********************************************************************************************/

void solaxX1_RS485Send(void) {
  uint8_t message[30];
  memcpy(message, solaxX1_SendData.Header, 2);
  memcpy(message + 2, solaxX1_SendData.Source, 2);
  memcpy(message + 4, solaxX1_SendData.Destination, 2);
  memcpy(message + 6, solaxX1_SendData.ControlCode, 1);
  memcpy(message + 7, solaxX1_SendData.FunctionCode, 1);
  memcpy(message + 8, solaxX1_SendData.DataLength, 1);
  memcpy(message + 9, solaxX1_SendData.Payload, sizeof(solaxX1_SendData.Payload));
  uint16_t crc = solaxX1_calculateCRC(message, 9 + solaxX1_SendData.DataLength[0]); // calculate out crc bytes
  while (solaxX1Serial->available() > 0) { // read serial if any old data is available
    solaxX1Serial->read();
  }
  if (PinUsed(GPIO_SOLAXX1_RTS)) {
    digitalWrite(Pin(GPIO_SOLAXX1_RTS), HIGH);
  }
  solaxX1Serial->flush();
  solaxX1Serial->write(message, 9 + solaxX1_SendData.DataLength[0]);
  solaxX1Serial->write(highByte(crc));
  solaxX1Serial->write(lowByte(crc));
  solaxX1Serial->flush();
  if (PinUsed(GPIO_SOLAXX1_RTS)) {
    digitalWrite(Pin(GPIO_SOLAXX1_RTS), LOW);
  }
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, message, 9 + solaxX1_SendData.DataLength[0]);
}

bool solaxX1_RS485Receive(uint8_t *ReadBuffer) {
  uint8_t len = 0;
  while (solaxX1Serial->available() > 0) {
    ReadBuffer[len++] = (uint8_t)solaxX1Serial->read();
  }
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, ReadBuffer, len);
  uint16_t crc = solaxX1_calculateCRC(ReadBuffer, len - 2); // calculate out crc bytes
  return !(ReadBuffer[len - 1] == lowByte(crc) && ReadBuffer[len - 2] == highByte(crc));
}

uint16_t solaxX1_calculateCRC(uint8_t *bExternTxPackage, uint8_t bLen) {
  uint8_t i;
  uint16_t wChkSum = 0;
  for (i = 0; i < bLen; i++) {
    wChkSum = wChkSum + bExternTxPackage[i];
  }
  return wChkSum;
}

void solaxX1_ExtractText(uint8_t *DataIn, uint8_t *DataOut, uint8_t Begin, uint8_t End) {
  uint8_t i;
  for (i = Begin; i <= End; i++) {
    DataOut[i - Begin] = DataIn[i];
  }
  DataOut[End - Begin + 1] = 0;
}

void solaxX1_QueryOfflineInverters(void) {
  solaxX1_SendData.Source[0] = 0x01;
  solaxX1_SendData.Destination[0] = 0x00;
  solaxX1_SendData.Destination[1] = 0x00;
  solaxX1_SendData.ControlCode[0] = 0x10;
  solaxX1_SendData.FunctionCode[0] = 0x00;
  solaxX1_SendData.DataLength[0] = 0x00;
  solaxX1_RS485Send();
}

void solaxX1_SendInverterAddress(void) {
  solaxX1_SendData.Source[0] = 0x00;
  solaxX1_SendData.Destination[0] = 0x00;
  solaxX1_SendData.Destination[1] = 0x00;
  solaxX1_SendData.ControlCode[0] = 0x10;
  solaxX1_SendData.FunctionCode[0] = 0x01;
  solaxX1_SendData.DataLength[0] = 0x0F;
  solaxX1_SendData.Payload[14] = INVERTER_ADDRESS; // Inverter Address, It must be unique in case of more inverters in the same rs485 net.
  solaxX1_RS485Send();
}

void solaxX1_QueryLiveData(void) {
  solaxX1_SendData.Source[0] = 0x01;
  solaxX1_SendData.Destination[0] = 0x00;
  solaxX1_SendData.Destination[1] = INVERTER_ADDRESS;
  solaxX1_SendData.ControlCode[0] = 0x11;
  solaxX1_SendData.FunctionCode[0] = 0x02;
  solaxX1_SendData.DataLength[0] = 0x00;
  solaxX1_RS485Send();
}

void solaxX1_QueryIDData(void) {
  solaxX1_SendData.Source[0] = 0x01;
  solaxX1_SendData.Destination[0] = 0x00;
  solaxX1_SendData.Destination[1] = INVERTER_ADDRESS;
  solaxX1_SendData.ControlCode[0] = 0x11;
  solaxX1_SendData.FunctionCode[0] = 0x03;
  solaxX1_SendData.DataLength[0] = 0x00;
  solaxX1_RS485Send();
}

void solaxX1_QueryConfigData(void) {
  solaxX1_SendData.Source[0] = 0x01;
  solaxX1_SendData.Destination[0] = 0x00;
  solaxX1_SendData.Destination[1] = INVERTER_ADDRESS;
  solaxX1_SendData.ControlCode[0] = 0x11;
  solaxX1_SendData.FunctionCode[0] = 0x04;
  solaxX1_SendData.DataLength[0] = 0x00;
  solaxX1_RS485Send();
}

uint8_t solaxX1_ParseErrorCode(uint32_t code) {
  solaxX1_ErrCode.ErrMessage = code;
  if (code == 0) return 0;
  if (solaxX1_ErrCode.MainsLostFault) return 1;
  if (solaxX1_ErrCode.GridVoltFault) return 2;
  if (solaxX1_ErrCode.GridFreqFault) return 3;
  if (solaxX1_ErrCode.PvVoltFault) return 4;
  if (solaxX1_ErrCode.IsolationFault) return 5;
  if (solaxX1_ErrCode.TemperatureOverFault) return 6;
  if (solaxX1_ErrCode.FanFault) return 7;
  if (solaxX1_ErrCode.OtherDeviceFault) return 8;
  return 0;
}

/*********************************************************************************************/

void solaxX1_250MSecond(void) { // Every 250 milliseconds
  uint8_t DataRead[80] = {0};
  uint8_t TempData[16] = {0};
  char TempDataChar[32];
  float TempFloat;

  if (solaxX1Serial->available()) {
    if (solaxX1_RS485Receive(DataRead)) { // CRC-error -> no further action
      DEBUG_SENSOR_LOG(PSTR("SX1: Data response CRC error"));
      return;
    }
  
    solaxX1_global.SendRetry_count = 20; // Inverter is responding

    if (DataRead[0] != 0xAA || DataRead[1] != 0x55) { // Check for header
      DEBUG_SENSOR_LOG(PSTR("SX1: Check for header failed"));
      return;
    }

    if (DataRead[6] == 0x11 && DataRead[7] == 0x82) { // received "Response for query (live data)"
      Energy->data_valid[0] = 0;
      solaxX1.temperature =    (DataRead[9] << 8) | DataRead[10]; // Temperature
      solaxX1.energy_today =   ((DataRead[11] << 8) | DataRead[12]) * 0.1f; // Energy Today
      solaxX1.dc1_voltage =    ((DataRead[13] << 8) | DataRead[14]) * 0.1f; // PV1 Voltage
      solaxX1.dc2_voltage =    ((DataRead[15] << 8) | DataRead[16]) * 0.1f; // PV2 Voltage
      solaxX1.dc1_current =    ((DataRead[17] << 8) | DataRead[18]) * 0.1f; // PV1 Current
      solaxX1.dc2_current =    ((DataRead[19] << 8) | DataRead[20]) * 0.1f; // PV2 Current
      Energy->current[0] =      ((DataRead[21] << 8) | DataRead[22]) * 0.1f; // AC Current
      Energy->voltage[0] =      ((DataRead[23] << 8) | DataRead[24]) * 0.1f; // AC Voltage
      Energy->frequency[0] =    ((DataRead[25] << 8) | DataRead[26]) * 0.01f; // AC Frequency
      Energy->active_power[0] = ((DataRead[27] << 8) | DataRead[28]); // AC Power
      //temporal = (float)((DataRead[29] << 8) | DataRead[30]) * 0.1f; // Not Used
      Energy->import_active[0] = ((DataRead[31] << 24) | (DataRead[32] << 16) | (DataRead[33] << 8) | DataRead[34]) * 0.1f; // Energy Total
      uint32_t runtime_total = (DataRead[35] << 24) | (DataRead[36] << 16) | (DataRead[37] << 8) | DataRead[38]; // Work Time Total
      if (runtime_total) solaxX1.runtime_total = runtime_total; // Work Time valid
      solaxX1.runMode =        (DataRead[39] << 8) | DataRead[40]; // Work mode
      //temporal = (float)((DataRead[41] << 8) | DataRead[42]); // Grid voltage fault value 0.1V
      //temporal = (float)((DataRead[43] << 8) | DataRead[44]); // Gird frequency fault value 0.01Hz
      //temporal = (float)((DataRead[45] << 8) | DataRead[46]); // Dc injection fault value 1mA
      //temporal = (float)((DataRead[47] << 8) | DataRead[48]); // Temperature fault value
      //temporal = (float)((DataRead[49] << 8) | DataRead[50]); // Pv1 voltage fault value 0.1V
      //temporal = (float)((DataRead[51] << 8) | DataRead[52]); // Pv2 voltage fault value 0.1V
      //temporal = (float)((DataRead[53] << 8) | DataRead[54]); // GFC fault value
      solaxX1.errorCode =      (DataRead[58] << 24) | (DataRead[57] << 16) | (DataRead[56] << 8) | DataRead[55]; // Error Code
      solaxX1.dc1_power = solaxX1.dc1_voltage * solaxX1.dc1_current;
      solaxX1.dc2_power = solaxX1.dc2_voltage * solaxX1.dc2_current;
      EnergyUpdateTotal();  // 484.708 kWh
      DEBUG_SENSOR_LOG(PSTR("SX1: received live data"));
      return;
    } // end received "Response for query (live data)"

    if (DataRead[6] == 0x11 && DataRead[7] == 0x83) { // received "Response for query (ID data)"
      solaxX1_ExtractText(DataRead, solaxX1.SerialNumber, 49, 62); // extract "real" serial number
      if (solaxX1_global.Command_QueryID) {
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter phases: %d"),DataRead[9]); // number of phases
        solaxX1_ExtractText(DataRead, TempData, 10, 15); // extract rated bus power (my be empty)
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter rated bus power: %s"),(char*)TempData);
        solaxX1_ExtractText(DataRead, TempData, 16, 20); // extract firmware version
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter firmware version: %s"),(char*)TempData);
        solaxX1_ExtractText(DataRead, TempData, 21, 34); // extract module name (my be empty)
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter module name: %s"),(char*)TempData);
        solaxX1_ExtractText(DataRead, TempData, 35, 48); // extract factory name
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter factory name: %s"),(char*)TempData);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter serial number: %s"),(char*)solaxX1.SerialNumber);
        solaxX1_ExtractText(DataRead, TempData, 63, 66); // extract rated bus voltage
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Inverter rated bus voltage: %s"),(char*)TempData);
        solaxX1_global.Command_QueryID = false;
      } else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("SX1: Inverter serial number: %s"),(char*)solaxX1.SerialNumber);
      }
      DEBUG_SENSOR_LOG(PSTR("SX1: received ID data"));
      return;
    } // end received "Response for query (ID data)"

#ifdef SOLAXX1_READCONFIG
    if (DataRead[6] == 0x11 && DataRead[7] == 0x84) { // received "Response for query (config data)"
      if (solaxX1_global.Command_QueryConfig) {
        // This values are displayed as they were received from the inverter. They are not interpreted in any way.
        TempFloat = ((DataRead[9] << 8) | DataRead[10]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wVpvStart: %1_f V (Inverter launch voltage threshold)"), &TempFloat);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wTimeStart: %d sec (launch wait time)"), (DataRead[11] << 8) | DataRead[12]);
        TempFloat = ((DataRead[13] << 8) | DataRead[14]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wVacMinProtect: %1_f V (allowed minimum grid voltage)"), &TempFloat);
        TempFloat = ((DataRead[15] << 8) | DataRead[16]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wVacMaxProtect: %1_f V (allowed maximum grid voltage)"), &TempFloat);
        TempFloat = ((DataRead[17] << 8) | DataRead[18]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wFacMinProtect: %2_f Hz (allowed minimum grid frequency)"), &TempFloat);
        TempFloat = ((DataRead[19] << 8) | DataRead[20]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wFacMaxProtect: %2_f Hz (allowed maximum grid frequency)"), &TempFloat);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wDciLimits: %d mA (DC component limits)"), (DataRead[21] << 8) | DataRead[22]);
        TempFloat = ((DataRead[23] << 8) | DataRead[24]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wGrid10MinAvgProtect: %1_f V (10 minutes over voltage protect)"), &TempFloat);
        TempFloat = ((DataRead[25] << 8) | DataRead[26]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wVacMinSlowProtect: %1_f V (grid undervoltage protect value)"), &TempFloat);
        TempFloat = ((DataRead[27] << 8) | DataRead[28]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wVacMaxSlowProtect: %1_f V (grid overvoltage protect value)"), &TempFloat);
        TempFloat = ((DataRead[29] << 8) | DataRead[30]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wFacMinSlowProtect: %2_f Hz (grid underfrequency protect value)"), &TempFloat);
        TempFloat = ((DataRead[31] << 8) | DataRead[32]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wFacMaxSlowProtect: %2_f Hz (grid overfrequency protect value)"), &TempFloat);
        GetTextIndexed(TempDataChar, sizeof(TempDataChar), (DataRead[33] << 8) | DataRead[34], kSolaxSafetyType);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wSafety: %d ≙ %s"), (DataRead[33] << 8) | DataRead[34], TempDataChar);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wPowerfactor_mode: %d"), DataRead[35]);
        TempFloat = DataRead[36] * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wPowerfactor_data: %2_f"), &TempFloat);
        TempFloat = DataRead[37] * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wUpperLimit: %2_f (overexcite limits)"), &TempFloat);
        TempFloat = DataRead[38] * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wLowerLimit: %2_f (underexcite limits)"), &TempFloat);
        TempFloat = DataRead[39] * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wPowerLow: %2_f (power ratio change upper limits)"), &TempFloat);
        TempFloat = DataRead[40] * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wPowerUp: %2_f (power ratio change lower limits)"), &TempFloat);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: Qpower_set: %d"), (DataRead[41] << 8) | DataRead[42]);
        TempFloat = ((DataRead[43] << 8) | DataRead[44]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WFreqSetPoint: %2_f Hz (Over Frequency drop output setpoint)"), &TempFloat);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WFreqDroopRate: %d %% (drop output slope)"), (DataRead[45] << 8) | DataRead[46]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: QuVupRate: %d %% (Q(U) curve up set point)"), (DataRead[47] << 8) | DataRead[48]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: QuVlowRate: %d %% (Q(U) curve low set point)"), (DataRead[49] << 8) | DataRead[50]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WPowerLimitsPercent: %d %%"), (DataRead[51] << 8) | DataRead[52]);
        TempFloat = ((DataRead[53] << 8) | DataRead[54]) * 0.01f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WWgra: %2_f %%"), &TempFloat);
        TempFloat = ((DataRead[55] << 8) | DataRead[56]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wWv2: %1_f V"), &TempFloat);
        TempFloat = ((DataRead[57] << 8) | DataRead[58]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wWv3: %1_f V"), &TempFloat);
        TempFloat = ((DataRead[59] << 8) | DataRead[60]) * 0.1f;
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wWv4: %1_f V"), &TempFloat);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wQurangeV1: %d %%"), (DataRead[61] << 8) | DataRead[62]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: wQurangeV4: %d %%"), (DataRead[63] << 8) | DataRead[64]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: BVoltPowerLimit: %d"), (DataRead[65] << 8) | DataRead[66]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WPowerManagerEnable: %d"), (DataRead[67] << 8) | DataRead[68]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WGlobalSeachMPPTStrartFlg: %d"), (DataRead[69] << 8) | DataRead[70]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WFrqProtectRestrictive: %d"), (DataRead[71] << 8) | DataRead[72]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WQuDelayTimer: %d sec"), (DataRead[73] << 8) | DataRead[74]);
        AddLog(LOG_LEVEL_INFO, PSTR("SX1: WFreqActivePowerDelayTimer: %d ms"), (DataRead[75] << 8) | DataRead[76]);

        solaxX1_global.Command_QueryConfig = false;
      }
      DEBUG_SENSOR_LOG(PSTR("SX1: received config data"));
      return;
    } // end received "Response for query (config data)"
#endif // SOLAXX1_READCONFIG

    if (DataRead[6] == 0x10 && DataRead[7] == 0x80) { // received "register request"
      solaxX1_global.QueryData_count = 5; // give time for next query
      solaxX1_ExtractText(DataRead, solaxX1_SendData.Payload, 9, 22); // store serial number for register
      DEBUG_SENSOR_LOG(PSTR("SX1: received register request and send register address"));
      solaxX1_SendInverterAddress(); // "send register address"
      return;
    }

    if (DataRead[6] == 0x10 && DataRead[7] == 0x81 && DataRead[9] == 0x06) { // received "address confirm (ACK)"
      solaxX1_global.QueryData_count = 5; // give time for next query
      solaxX1_global.AddressAssigned = true;
      DEBUG_SENSOR_LOG(PSTR("SX1: received \"address confirm (ACK)\""));
      return;
    }

  } // end solaxX1Serial->available()

//  DEBUG_SENSOR_LOG(PSTR("SX1: solaxX1_global.AddressAssigned: %d, solaxX1_global.QueryData_count: %d, solaxX1_global.SendRetry_count: %d, solaxX1_global.QueryID_count: %d"), solaxX1_global.AddressAssigned, solaxX1_global.QueryData_count, solaxX1_global.SendRetry_count, solaxX1_global.QueryID_count);
  if (solaxX1_global.AddressAssigned) {
    if (!solaxX1_global.QueryData_count) { // normal periodically query
      solaxX1_global.QueryData_count = 5;
      if (!solaxX1_global.QueryID_count || solaxX1_global.Command_QueryID) { // ID query
        DEBUG_SENSOR_LOG(PSTR("SX1: Send ID query"));
        solaxX1_QueryIDData();
      } else if (solaxX1_global.Command_QueryConfig) { // Config query
        DEBUG_SENSOR_LOG(PSTR("SX1: Send config query"));
        solaxX1_QueryConfigData();
      } else { // live query
        DEBUG_SENSOR_LOG(PSTR("SX1: Send live query"));
        solaxX1_QueryLiveData();
      }
      solaxX1_global.QueryID_count++; // query ID every 256th time
    }  // end normal periodically query
    solaxX1_global.QueryData_count--;
    if (!solaxX1_global.SendRetry_count) { // Inverter went "off"
      solaxX1_global.SendRetry_count = 20;
      DEBUG_SENSOR_LOG(PSTR("SX1: Inverter went \"off\""));
      Energy->data_valid[0] = ENERGY_WATCHDOG;
      solaxX1.temperature = solaxX1.dc1_voltage = solaxX1.dc2_voltage = solaxX1.dc1_current = solaxX1.dc2_current = solaxX1.dc1_power = 0;
      solaxX1.dc2_power = Energy->current[0] = Energy->voltage[0] = Energy->frequency[0] = Energy->active_power[0] = 0;
      solaxX1.runMode = -1; // off(line)
      solaxX1_global.AddressAssigned = false;
    } // end Inverter went "off"
  } else { // sent query for inverters in offline status
    if (!solaxX1_global.SendRetry_count) {
      solaxX1_global.SendRetry_count = 20;
      DEBUG_SENSOR_LOG(PSTR("SX1: Sent query for inverters in offline state"));
      solaxX1_QueryOfflineInverters();
    }
  }
  solaxX1_global.SendRetry_count--;
return;  
} // end solaxX1_250MSecond

void solaxX1_SnsInit(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("SX1: Init - RX-pin: %d, TX-pin: %d, RTS-pin: %d"), Pin(GPIO_SOLAXX1_RX), Pin(GPIO_SOLAXX1_TX), Pin(GPIO_SOLAXX1_RTS));
  solaxX1Serial = new TasmotaSerial(Pin(GPIO_SOLAXX1_RX), Pin(GPIO_SOLAXX1_TX), 1);
  if (solaxX1Serial->begin(SOLAXX1_SPEED)) {
    if (solaxX1Serial->hardwareSerial()) { ClaimSerial(); }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("SX1: Serial UART%d"), solaxX1Serial->getUart());
#endif
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
  if (PinUsed(GPIO_SOLAXX1_RTS)) {
    pinMode(Pin(GPIO_SOLAXX1_RTS), OUTPUT);
  }
}

void solaxX1_DrvInit(void) {
  if (PinUsed(GPIO_SOLAXX1_RX) && PinUsed(GPIO_SOLAXX1_TX)) {
    TasmotaGlobal.energy_driver = XNRG_12;
    Energy->type_dc = true; // Handle like DC, because U*I from inverter is not valid for apparent power; U*I could be lower than active power
  }
}

bool SolaxX1_cmd(void) {
  if (!solaxX1_global.AddressAssigned) {
    AddLog(LOG_LEVEL_INFO, PSTR("SX1: No inverter registered"));
    return false;
  }

  if (!strcasecmp(XdrvMailbox.data, "ReadIDinfo")) {
    solaxX1_global.Command_QueryID = true;
    AddLog(LOG_LEVEL_INFO, PSTR("SX1: ReadIDinfo sent..."));
    return true;
  } else if (!strcasecmp(XdrvMailbox.data, "ReadConfig")) {
#ifdef SOLAXX1_READCONFIG
    solaxX1_global.Command_QueryConfig = true;
    AddLog(LOG_LEVEL_INFO, PSTR("SX1: ReadConfig sent..."));
    return true;
#else
    AddLog(LOG_LEVEL_INFO, PSTR("SX1: Command not available. Please set compiler directive '#define SOLAXX1_READCONFIG'."));
    return false;
#endif // SOLAXX1_READCONFIG
  }
  AddLog(LOG_LEVEL_INFO, PSTR("SX1: Unknown command: \"%s\""),XdrvMailbox.data);
  return false;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_solaxX1_Num[] PROGMEM = "{s}" D_SOLAX_X1 " %s{m}</td><td style='text-align:%s'>%s{m}{m} %s{e}";
const char HTTP_SNS_solaxX1_Str[] PROGMEM = "{s}" D_SOLAX_X1 " %s{m}%s{e}";
#endif // USE_WEBSERVER

void solaxX1_Show(uint32_t function) {
  char solar_power[33];
  dtostrfd(solaxX1.dc1_power + solaxX1.dc2_power, Settings->flag2.wattage_resolution, solar_power);
  char pv1_voltage[33];
  dtostrfd(solaxX1.dc1_voltage, Settings->flag2.voltage_resolution, pv1_voltage);
  char pv1_current[33];
  dtostrfd(solaxX1.dc1_current, Settings->flag2.current_resolution, pv1_current);
  char pv1_power[33];
  dtostrfd(solaxX1.dc1_power, Settings->flag2.wattage_resolution, pv1_power);
#ifdef SOLAXX1_PV2
  char pv2_voltage[33];
  dtostrfd(solaxX1.dc2_voltage, Settings->flag2.voltage_resolution, pv2_voltage);
  char pv2_current[33];
  dtostrfd(solaxX1.dc2_current, Settings->flag2.current_resolution, pv2_current);
  char pv2_power[33];
  dtostrfd(solaxX1.dc2_power, Settings->flag2.wattage_resolution, pv2_power);
#endif
  char status[33];
  GetTextIndexed(status, sizeof(status), solaxX1.runMode + 1, kSolaxMode);

  switch (function) {
    case FUNC_JSON_APPEND:
      ResponseAppend_P(PSTR(",\"" D_JSON_SOLAR_POWER "\":%s,\"" D_JSON_PV1_VOLTAGE "\":%s,\"" D_JSON_PV1_CURRENT "\":%s,\"" D_JSON_PV1_POWER "\":%s"),
                                  solar_power, pv1_voltage, pv1_current, pv1_power);
#ifdef SOLAXX1_PV2
      ResponseAppend_P(PSTR(",\"" D_JSON_PV2_VOLTAGE "\":%s,\"" D_JSON_PV2_CURRENT "\":%s,\"" D_JSON_PV2_POWER "\":%s"),
                                  pv2_voltage, pv2_current, pv2_power);
#endif
      ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%d,\"" D_JSON_RUNTIME "\":%d,\"" D_JSON_STATUS "\":\"%s\",\"" D_JSON_ERROR "\":%d"),
                                  solaxX1.temperature, solaxX1.runtime_total, status, solaxX1.errorCode);

#ifdef USE_DOMOTICZ
      // Avoid bad temperature report at beginning of the day (spikes of 1200 celsius degrees)
      if (0 == TasmotaGlobal.tele_period && solaxX1.temperature < 100) { DomoticzSensor(DZ_TEMP, solaxX1.temperature); }
#endif // USE_DOMOTICZ
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR: {
      String table_align = Settings->flag5.gui_table_align?"right":"left";
      static uint32_t LastOnlineTime;
      if (solaxX1.runMode != -1) LastOnlineTime = TasmotaGlobal.uptime;
      if (TasmotaGlobal.uptime < LastOnlineTime + 300) { // Hide numeric live data, when inverter is offline for more than 5 min
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_SOLAR_POWER, table_align.c_str(), solar_power, D_UNIT_WATT);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV1_VOLTAGE, table_align.c_str(), pv1_voltage, D_UNIT_VOLT);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV1_CURRENT, table_align.c_str(), pv1_current, D_UNIT_AMPERE);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV1_POWER,   table_align.c_str(), pv1_power,   D_UNIT_WATT);
#ifdef SOLAXX1_PV2
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV2_VOLTAGE, table_align.c_str(), pv2_voltage, D_UNIT_VOLT);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV2_CURRENT, table_align.c_str(), pv2_current, D_UNIT_AMPERE);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_PV2_POWER,   table_align.c_str(), pv2_power,   D_UNIT_WATT);
#endif
        char SXTemperature[16];
        dtostrfd(solaxX1.temperature, Settings->flag2.temperature_resolution, SXTemperature);
        WSContentSend_PD(HTTP_SNS_solaxX1_Num, D_TEMPERATURE, table_align.c_str(), SXTemperature, D_UNIT_DEGREE D_UNIT_CELSIUS);
      }
      WSContentSend_P(HTTP_SNS_solaxX1_Num, D_UPTIME, table_align.c_str(), String(solaxX1.runtime_total).c_str(), D_UNIT_HOUR);
      break; }
    case FUNC_WEB_SENSOR:
      char errorCodeString[33];
      WSContentSend_P(HTTP_SNS_solaxX1_Str, D_STATUS, status);
      WSContentSend_P(HTTP_SNS_solaxX1_Str, D_ERROR, GetTextIndexed(errorCodeString, sizeof(errorCodeString), solaxX1_ParseErrorCode(solaxX1.errorCode), kSolaxError));
      if (solaxX1.SerialNumber[0]) WSContentSend_P(HTTP_SNS_solaxX1_Str, "Inverter SN", solaxX1.SerialNumber);
      break;
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg12(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      solaxX1_250MSecond();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
    case FUNC_WEB_SENSOR:
#endif  // USE_WEBSERVER
    case FUNC_JSON_APPEND:
      solaxX1_Show(function);
      break;
    case FUNC_INIT:
      solaxX1_SnsInit();
      break;
    case FUNC_PRE_INIT:
      solaxX1_DrvInit();
      break;
    case FUNC_COMMAND:
      result = SolaxX1_cmd();
      break;
  }
  return result;
}

#endif  // USE_SOLAX_X1_NRG
#endif  // USE_ENERGY_SENSOR
