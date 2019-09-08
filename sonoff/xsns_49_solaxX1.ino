/*
  xsns_48_solaxX1.ino - Solax X1 inverter RS485 support for Sonoff-Tasmota

  Copyright (C) 2019  Pablo Zerón

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

#ifdef USE_SOLAX_X1
/*********************************************************************************************\
 * Solax X1 Inverter
\*********************************************************************************************/

#define XSNS_49            49

#ifndef SOLAXX1_SPEED
#define SOLAXX1_SPEED      9600      // default solax rs485 speed
#endif

#define INVERTER_ADDRESS   0x0A

#define D_SOLAX_X1         "SolaxX1"

#include <TasmotaSerial.h>

enum solaxX1_Error
{
  solaxX1_ERR_NO_ERROR,
  solaxX1_ERR_CRC_ERROR
};

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
} ErrCode;

const char kSolaxMode[] PROGMEM = D_WAITING "|" D_CHECKING "|" D_WORKING "|" D_FAILURE;

const char kSolaxError[] PROGMEM =
  D_SOLAX_ERROR_0 "|" D_SOLAX_ERROR_1 "|" D_SOLAX_ERROR_2 "|" D_SOLAX_ERROR_3 "|" D_SOLAX_ERROR_4 "|" D_SOLAX_ERROR_5 "|"
  D_SOLAX_ERROR_6 "|" D_SOLAX_ERROR_7 "|" D_SOLAX_ERROR_8;

/*********************************************************************************************/

TasmotaSerial *solaxX1Serial;

uint8_t solaxX1_Init = 1;

uint8_t solaxX1_status = 0;
uint32_t solaxX1_errorCode = 0;

float solaxX1_temperature = 0;
float solaxX1_energy_today = 0;
float solaxX1_dc1_voltage = 0;
float solaxX1_dc2_voltage = 0;
float solaxX1_dc1_current = 0;
float solaxX1_dc2_current = 0;
float solaxX1_ac_current = 0;
float solaxX1_ac_voltage = 0;
float solaxX1_frequency = 0;
float solaxX1_power = 0;
float solaxX1_energy_total = 0;
float solaxX1_runtime_total = 0;

float solaxX1_dc1_power = 0;
float solaxX1_dc2_power = 0;

bool queryOffline = false;
bool queryOfflineSend = false;
bool hasAddress = true;
bool inverterAddressSend = false;
bool inverterSnReceived = false;

uint8_t header[2] = {0xAA, 0x55};
uint8_t source[2] = {0x00, 0x00};
uint8_t destination[2] = {0x00, 0x00};
uint8_t controlCode[1] = {0x00};
uint8_t functionCode[1] = {0x00};
uint8_t dataLength[1] = {0x00};
uint8_t data[16] = {0};

uint8_t message[30];

/*********************************************************************************************/

bool solaxX1_RS485ReceiveReady(void)
{
  return (solaxX1Serial->available() > 1);
}

void solaxX1_RS485Send(uint8_t *msg, uint16_t msgLen)
{

  uint16_t crc = solaxX1_calculateCRC(msg, msgLen - 1); // calculate out crc bytes

  while (solaxX1Serial->available() > 0)
  { // read serial if any old data is available
    solaxX1Serial->read();
  }

  solaxX1Serial->flush();
  solaxX1Serial->write(msg, msgLen);
  solaxX1Serial->write(highByte(crc));
  solaxX1Serial->write(lowByte(crc));
}

uint8_t solaxX1_RS485Receive(uint8_t *value)
{
  uint8_t len = 0;

  while (solaxX1Serial->available() > 0)
  {
    value[len++] = (uint8_t)solaxX1Serial->read();
  }

  uint16_t crc = solaxX1_calculateCRC(value, len - 3); // calculate out crc bytes

  if (value[len - 1] == lowByte(crc) && value[len - 2] == highByte(crc))
  { // check calc crc with received crc
    return solaxX1_ERR_NO_ERROR;
  }
  else
  {
    return solaxX1_ERR_CRC_ERROR;
  }
}

uint16_t solaxX1_calculateCRC(uint8_t *bExternTxPackage, uint8_t bLen)
{
  uint8_t i;
  uint16_t wChkSum;
  wChkSum = 0;

  for (i = 0; i <= bLen; i++)
  {
    wChkSum = wChkSum + bExternTxPackage[i];
  }
  return wChkSum;
}

void solaxX1_setMessage(uint8_t *message)
{
  memcpy(message, header, 2);
  memcpy(message + 2, source, 2);
  memcpy(message + 4, destination, 2);
  memcpy(message + 6, controlCode, 1);
  memcpy(message + 7, functionCode, 1);
  memcpy(message + 8, dataLength, 1);
  memcpy(message + 9, data, sizeof(data));
}

void solaxX1_SendInverterAddress()
{
  source[0] = 0x00;
  destination[0] = 0x00;
  destination[1] = 0x00;
  controlCode[0] = 0x10;
  functionCode[0] = 0x01;
  dataLength[0] = 0x0F;

  // Inverter Address, It must be unique in case of more inverters in the same rs485 net.
  data[14] = INVERTER_ADDRESS;

  solaxX1_setMessage(message);
  solaxX1_RS485Send(message, 24);
}

void solaxX1_QueryLiveData()
{
  source[0] = 0x01;
  destination[0] = 0x00;
  destination[1] = INVERTER_ADDRESS;
  controlCode[0] = 0x11;
  functionCode[0] = 0x02;
  dataLength[0] = 0x00;

  solaxX1_setMessage(message);
  solaxX1_RS485Send(message, 9);
}

uint8_t solaxX1_ParseErrorCode(uint32_t code){
  ErrCode.ErrMessage = code;

  if (code == 0) return 0;
  if (ErrCode.MainsLostFault) return 1;
  if (ErrCode.GridVoltFault) return 2;
  if (ErrCode.GridFreqFault) return 3;
  if (ErrCode.PvVoltFault) return 4;
  if (ErrCode.IsolationFault) return 5;
  if (ErrCode.TemperatureOverFault) return 6;
  if (ErrCode.FanFault) return 7;
  if (ErrCode.OtherDeviceFault) return 8;
}

/*********************************************************************************************/

uint8_t solaxX1_send_retry = 0;
uint8_t solaxX1_nodata_count = 0;

void solaxX1_Update(void) // Every Second
{
  uint8_t value[61] = {0};

  bool data_ready = solaxX1_RS485ReceiveReady();

  DEBUG_SENSOR_LOG(PSTR("SX1: queryOffline: %d , queryOfflineSend: %d, hasAddress: %d, inverterAddressSend: %d, solaxX1_send_retry: %d"),
    queryOffline, queryOfflineSend, hasAddress, inverterAddressSend, solaxX1_send_retry);

  if (!hasAddress && (data_ready || solaxX1_send_retry == 0))
  {

    if (data_ready)
    {
      // check address confirmation from inverter
      if (inverterAddressSend)
      {
        uint8_t error = solaxX1_RS485Receive(value);
        if (error)
        {
          DEBUG_SENSOR_LOG(PSTR("SX1: Address confirmation response CRC error"));
        }
        else
        {
          if (value[6] == 0x10 && value[7] == 0x81 && value[9] == 0x06)
          {
            inverterAddressSend = false;
            queryOfflineSend = false;
            hasAddress = true;
          }
        }
      }

      // Check inverter serial number and send the set address request
      if (queryOfflineSend)
      {
        uint8_t error = solaxX1_RS485Receive(value);
        if (error)
        {
          DEBUG_SENSOR_LOG(PSTR("SX1: Query Offline response CRC error"));
        }
        else
        {
          // Serial number from query response
          if (value[6] == 0x10 && value[7] == 0x80 && inverterSnReceived == false)
          {
            for (uint8_t i = 9; i <= 22; i++)
            {
              data[i - 9] = value[i];
            }
            inverterSnReceived = true;
          }

          solaxX1_SendInverterAddress();

          inverterAddressSend = true;
          queryOfflineSend = false;
          queryOffline = false;
        }
      }
    }

    // request to the inverter the serial number if offline
    if (queryOffline)
    {
      // We sent the message to query inverters in offline status
      source[0] = 0x01;
      destination[1] = 0x00;
      controlCode[0] = 0x10;
      functionCode[0] = 0x00;
      dataLength[0] = 0x00;

      solaxX1_setMessage(message);
      solaxX1_RS485Send(message, 9);

      queryOfflineSend = true;
      queryOffline = false;
    }

    if (solaxX1_send_retry == 0)
    {

      if (inverterAddressSend)
      {
        solaxX1_SendInverterAddress();
      }
      if (queryOfflineSend)
      {
        queryOffline = true;
        queryOfflineSend = false;
      }
      solaxX1_send_retry = 2;
    }

  } // end !hasAddress && (data_ready || solaxX1_send_retry == 0)

  if (hasAddress && (data_ready || solaxX1_send_retry == 0))
  {

    if (data_ready)
    {
      uint8_t error = solaxX1_RS485Receive(value);
      if (error)
      {
        DEBUG_SENSOR_LOG(PSTR("SX1: Data response CRC error"));
      }
      else
      {
//        AddLogBuffer(LOG_LEVEL_DEBUG, value, sizeof(value));

        solaxX1_nodata_count = 0;
        solaxX1_send_retry = 2;
        uint32_t temporal = 0;

        temporal = (value[9] << 8) | value[10]; // Temperature
        solaxX1_temperature = temporal;

        temporal = (value[11] << 8) | value[12]; // Energy Today
        solaxX1_energy_today = temporal * 0.1f;

        temporal = (value[13] << 8) | value[14]; // PV1 Voltage
        solaxX1_dc1_voltage = temporal * 0.1f;

        temporal = (value[15] << 8) | value[16]; // PV2 Voltage
        solaxX1_dc2_voltage = temporal * 0.1f;

        temporal = (value[17] << 8) | value[18]; // PV1 Current
        solaxX1_dc1_current = temporal * 0.1f;

        temporal = (value[19] << 8) | value[20]; // PV2 Current
        solaxX1_dc2_current = temporal * 0.1f;

        temporal = (value[21] << 8) | value[22]; // AC Current
        solaxX1_ac_current = temporal * 0.1f;

        temporal = (value[23] << 8) | value[24]; // AC Voltage
        solaxX1_ac_voltage = temporal * 0.1f;

        temporal = (value[25] << 8) | value[26]; // AC Frequency
        solaxX1_frequency = temporal * 0.01f;

        temporal = (value[27] << 8) | value[28]; // AC Power
        solaxX1_power = temporal;

        //temporal = (value[29] << 8) | value[30]; // Not Used
        //solaxX1_notused = temporal * 0.1f;

        temporal = (value[31] << 8) | (value[32] << 8) | (value[33] << 8) | value[34]; // Energy Total
        solaxX1_energy_total = temporal * 0.1f;

        temporal = (value[35] << 8) | (value[36] << 8) | (value[37] << 8) | value[38]; // Work Time Total
        solaxX1_runtime_total = temporal;

        temporal = (value[39] << 8) | value[40]; // Work mode
        solaxX1_status = (uint8_t)temporal;

        //temporal = (value[41] << 8) | value[42]; // Grid voltage fault value 0.1V
        //temporal = (value[43] << 8) | value[44]; // Gird frequency fault value 0.01Hz
        //temporal = (value[45] << 8) | value[46]; // Dc injection fault value 1mA
        //temporal = (value[47] << 8) | value[48]; // Temperature fault value
        //temporal = (value[49] << 8) | value[50]; // Pv1 voltage fault value 0.1V
        //temporal = (value[51] << 8) | value[52]; // Pv2 voltage fault value 0.1V
        //temporal = (value[53] << 8) | value[54]; // GFC fault value

        temporal = (value[58] << 8) | (value[57] << 8) | (value[56] << 8) | value[55]; // Error Code
        solaxX1_errorCode = (uint32_t)temporal;

        solaxX1_dc1_power = solaxX1_dc1_voltage * solaxX1_dc1_current;
        solaxX1_dc2_power = solaxX1_dc2_voltage * solaxX1_dc2_current;

        solaxX1_QueryLiveData();
      } // end else no error
    }

    if (solaxX1_send_retry == 0)
    {
      solaxX1_send_retry = 2;
      solaxX1_QueryLiveData();
    }
  }
  else
  { // end hasAddress && (data_ready || solaxX1_send_retry == 0)

    if (solaxX1_nodata_count <= 10) // max. 10 sec without data
    {
      solaxX1_nodata_count++;
    }
    else if (solaxX1_nodata_count != 255)
    {
      // no data from RS485, reset values to 0
      solaxX1_nodata_count = 255;
      queryOffline = true;
      queryOfflineSend = false;
      hasAddress = false;
      inverterAddressSend = false;
      inverterSnReceived = false;

      solaxX1_temperature = solaxX1_dc1_voltage = solaxX1_dc2_voltage = solaxX1_dc1_current = solaxX1_dc2_current = solaxX1_ac_current = 0;
      solaxX1_ac_voltage = solaxX1_frequency = solaxX1_power = solaxX1_dc1_power = solaxX1_dc2_power = solaxX1_status = 0;

      //solaxX1_energy_today = solaxX1_energy_total = solaxX1_runtime_total = 0;
    }
  }

  if (!data_ready)
    solaxX1_send_retry--;
}

void solaxX1Init(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("Solax X1 Inverter Init"));
  DEBUG_SENSOR_LOG(PSTR("SX1: RX pin: %d, TX pin: %d"), pin[GPIO_SOLAXX1_RX], pin[GPIO_SOLAXX1_TX]);
  solaxX1_Init = 0;
  if ((pin[GPIO_SOLAXX1_RX] < 99) && (pin[GPIO_SOLAXX1_TX] < 99))
  {
    solaxX1Serial = new TasmotaSerial(pin[GPIO_SOLAXX1_RX], pin[GPIO_SOLAXX1_TX], 1);
    if (solaxX1Serial->begin(SOLAXX1_SPEED))
    {
      if (solaxX1Serial->hardwareSerial())
      {
        ClaimSerial();
      }
      solaxX1_Init = 1;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_solaxX1_DATA1[] PROGMEM =
    "{s}" D_SOLAX_X1 " " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
    "{s}" D_SOLAX_X1 " " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
    "{s}" D_SOLAX_X1 " " D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"
    "{s}" D_SOLAX_X1 " " D_INVERTER_POWER "{m}%s " D_UNIT_WATT "{e}"
    "{s}" D_SOLAX_X1 " " D_SOLAR_POWER "{m}%s " D_UNIT_WATT "{e}"
    "{s}" D_SOLAX_X1 " " D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
    "{s}" D_SOLAX_X1 " " D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
    "{s}" D_SOLAX_X1 " " D_PV1_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
    "{s}" D_SOLAX_X1 " " D_PV1_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
    "{s}" D_SOLAX_X1 " " D_PV1_POWER "{m}%s " D_UNIT_WATT "{e}";
#ifdef SOLAXX1_PV2
const char HTTP_SNS_solaxX1_DATA2[] PROGMEM =
    "{s}" D_SOLAX_X1 " " D_PV2_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
    "{s}" D_SOLAX_X1 " " D_PV2_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
    "{s}" D_SOLAX_X1 " " D_PV2_POWER "{m}%s " D_UNIT_WATT "{e}";
#endif
const char HTTP_SNS_solaxX1_DATA3[] PROGMEM =
    "{s}" D_SOLAX_X1 " " D_UPTIME "{m}%s " D_UNIT_HOUR "{e}"
    "{s}" D_SOLAX_X1 " " D_STATUS "{m}%s"
    "{s}" D_SOLAX_X1 " " D_ERROR "{m}%s";
#endif // USE_WEBSERVER

void solaxX1Show(bool json)
{
  char voltage[33];
  dtostrfd(solaxX1_ac_voltage, Settings.flag2.voltage_resolution, voltage);
  char current[33];
  dtostrfd(solaxX1_ac_current, Settings.flag2.current_resolution, current);
  char inverter_power[33];
  dtostrfd(solaxX1_power, Settings.flag2.wattage_resolution, inverter_power);
  char solar_power[33];
  dtostrfd(solaxX1_dc1_power + solaxX1_dc2_power, Settings.flag2.wattage_resolution, solar_power);
  char frequency[33];
  dtostrfd(solaxX1_frequency, Settings.flag2.frequency_resolution, frequency);
  char energy_total[33];
  dtostrfd(solaxX1_energy_total, Settings.flag2.energy_resolution, energy_total);
  char energy_today[33];
  dtostrfd(solaxX1_energy_today, Settings.flag2.energy_resolution, energy_today);
  char pv1_voltage[33];
  dtostrfd(solaxX1_dc1_voltage, Settings.flag2.voltage_resolution, pv1_voltage);
  char pv1_current[33];
  dtostrfd(solaxX1_dc1_current, Settings.flag2.current_resolution, pv1_current);
  char pv1_power[33];
  dtostrfd(solaxX1_dc1_power, Settings.flag2.wattage_resolution, pv1_power);
#ifdef SOLAXX1_PV2
  char pv2_voltage[33];
  dtostrfd(solaxX1_dc2_voltage, Settings.flag2.voltage_resolution, pv2_voltage);
  char pv2_current[33];
  dtostrfd(solaxX1_dc2_current, Settings.flag2.current_resolution, pv2_current);
  char pv2_power[33];
  dtostrfd(solaxX1_dc2_power, Settings.flag2.wattage_resolution, pv2_power);
#endif
  char temperature[33];
  dtostrfd(solaxX1_temperature, Settings.flag2.temperature_resolution, temperature);
  char runtime[33];
  dtostrfd(solaxX1_runtime_total, 0, runtime);
  char status[33];
  GetTextIndexed(status, sizeof(status), solaxX1_status, kSolaxMode);

  if (json)
  {
    ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_ACTIVE_POWERUSAGE "\":%s,\""
                                D_JSON_SOLAR_POWER "\":%s,\"" D_JSON_FREQUENCY "\":%s,\"" D_JSON_TOTAL "\":%s,\"" D_JSON_TODAY "\":%s,\""
                                D_JSON_PV1_VOLTAGE "\":%s,\"" D_JSON_PV1_CURRENT "\":%s,\"" D_JSON_PV1_POWER "\":%s"),
                                voltage, current, inverter_power,
                                solar_power, frequency, energy_total, energy_today,
                                pv1_voltage, pv1_current, pv1_power);
#ifdef SOLAXX1_PV2
    ResponseAppend_P(PSTR(",\"" D_JSON_PV2_VOLTAGE "\":%s,\"" D_JSON_PV2_CURRENT "\":%s,\"" D_JSON_PV2_POWER "\":%s"),
                                pv2_voltage, pv2_current, pv2_power);
#endif
    ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_RUNTIME "\":%s,\"" D_JSON_STATUS "\":\"%s\",\"" D_JSON_ERROR "\":%d}"),
                                temperature, runtime, status, solaxX1_errorCode);


#ifdef USE_DOMOTICZ
    if (0 == tele_period)
    {
      char energy_total_chr[33];
      dtostrfd(solaxX1_energy_total * 1000, 1, energy_total_chr);

      DomoticzSensor(DZ_VOLTAGE, voltage);
      DomoticzSensor(DZ_CURRENT, current);
      // Only do the updates if the values are greater than 0, to avoid wrong data representation in domoticz
      if (solaxX1_temperature  > 0) DomoticzSensor(DZ_TEMP, temperature);
      if (solaxX1_energy_total > 0) DomoticzSensorPowerEnergy((int)solaxX1_power, energy_total_chr);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  }
  else
  {
    WSContentSend_PD(HTTP_SNS_solaxX1_DATA1, voltage, current, frequency, inverter_power, solar_power, energy_total, energy_today, pv1_voltage, pv1_current, pv1_power);
#ifdef SOLAXX1_PV2
    WSContentSend_PD(HTTP_SNS_solaxX1_DATA2, pv2_voltage, pv2_current, pv2_power);
#endif
    WSContentSend_PD(HTTP_SNS_TEMP, D_SOLAX_X1, temperature, TempUnit());
    char errorCodeString[33];
    WSContentSend_PD(HTTP_SNS_solaxX1_DATA3, runtime, status,
      GetTextIndexed(errorCodeString, sizeof(errorCodeString), solaxX1_ParseErrorCode(solaxX1_errorCode), kSolaxError));
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns49(uint8_t function)
{
  bool result = false;

  if (solaxX1_Init)
  {
    switch (function)
    {
    case FUNC_INIT:
      solaxX1Init();
      break;
    case FUNC_EVERY_SECOND:
      solaxX1_Update();
      break;
    case FUNC_JSON_APPEND:
      solaxX1Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      solaxX1Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SOLAX_X1