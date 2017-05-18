/*
  Copyright (c) 2016 Theo Arends.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_PZEM004T
/*********************************************************************************************\
   PZEM004T - Power Meter

   Source: Victor Ferrer https://github.com/vicfergar/Sonoff-MQTT-OTA-Arduino
   Based on: PZEM004T library https://github.com/olehs/PZEM004T
  \*********************************************************************************************/

#include <SoftwareSerial.h>

#define PZEM_VOLTAGE (uint8_t)0xB0
#define RESP_VOLTAGE (uint8_t)0xA0

#define PZEM_CURRENT (uint8_t)0xB1
#define RESP_CURRENT (uint8_t)0xA1

#define PZEM_POWER   (uint8_t)0xB2
#define RESP_POWER   (uint8_t)0xA2

#define PZEM_ENERGY  (uint8_t)0xB3
#define RESP_ENERGY  (uint8_t)0xA3

#define PZEM_SET_ADDRESS (uint8_t)0xB4
#define RESP_SET_ADDRESS (uint8_t)0xA4

#define PZEM_POWER_ALARM (uint8_t)0xB5
#define RESP_POWER_ALARM (uint8_t)0xA5

#define RESPONSE_SIZE sizeof(PZEMCommand)
#define RESPONSE_DATA_SIZE RESPONSE_SIZE - 2

#define PZEM_BAUD_RATE 9600
#define PZEM_DEFAULT_READ_TIMEOUT 1000

#define PZEM_ERROR_VALUE -1.0

struct PZEMCommand {
  uint8_t command;
  uint8_t addr[4];
  uint8_t data;
  uint8_t crc;
};

Stream *serialPtr;

void PZEM004T_init(uint8_t receivePin, uint8_t transmitPin)
{
  auto port = new SoftwareSerial(receivePin, transmitPin);
  port->begin(PZEM_BAUD_RATE);
  serialPtr = port;
}

void PZEM004T_voltage_snd(const IPAddress &addr)
{
  PZEM004T_send(addr, PZEM_VOLTAGE, 0);
}

float PZEM004T_voltage_rcv(const IPAddress &addr)
{
  uint8_t data[RESPONSE_DATA_SIZE];

  if (!PZEM004T_recieve(RESP_VOLTAGE, data))
    return PZEM_ERROR_VALUE;

  return (data[0] << 8) + data[1] + (data[2] / 10.0);
}

void PZEM004T_current_snd(const IPAddress &addr)
{
  PZEM004T_send(addr, PZEM_CURRENT, 0);
}

float PZEM004T_current_rcv(const IPAddress &addr)
{
  uint8_t data[RESPONSE_DATA_SIZE];

  if (!PZEM004T_recieve(RESP_CURRENT, data))
    return PZEM_ERROR_VALUE;

  return (data[0] << 8) + data[1] + (data[2] / 100.0);
}

void PZEM004T_power_snd(const IPAddress &addr)
{
  PZEM004T_send(addr, PZEM_POWER, 0);
}

float PZEM004T_power_rcv(const IPAddress &addr)
{
  uint8_t data[RESPONSE_DATA_SIZE];

  if (!PZEM004T_recieve(RESP_POWER, data))
    return PZEM_ERROR_VALUE;

  return (data[0] << 8) + data[1];
}

void PZEM004T_energy_snd(const IPAddress &addr)
{
  PZEM004T_send(addr, PZEM_ENERGY, 0);
}

float PZEM004T_energy_rcv(const IPAddress &addr)
{
  uint8_t data[RESPONSE_DATA_SIZE];

  if (!PZEM004T_recieve(RESP_ENERGY, data))
    return PZEM_ERROR_VALUE;

  return ((uint32_t)data[0] << 16) + ((uint16_t)data[1] << 8) + data[2];
}

void PZEM004T_setAddress_snd(const IPAddress &newAddr)
{
  PZEM004T_send(newAddr, PZEM_SET_ADDRESS, 0);
}

bool PZEM004T_setAddress_rcv(const IPAddress &newAddr)
{
  return PZEM004T_recieve(RESP_SET_ADDRESS, 0);
}

void PZEM004T_send(const IPAddress &addr, uint8_t cmd, uint8_t data)
{
  PZEMCommand pzem;

  pzem.command = cmd;
  for (int i = 0; i < sizeof(pzem.addr); i++)
    pzem.addr[i] = addr[i];
  pzem.data = data;

  uint8_t *bytes = (uint8_t*)&pzem;
  pzem.crc = PZEM004T_crc(bytes, sizeof(pzem) - 1);

  while (serialPtr->available())
    serialPtr->read();

  serialPtr->write(bytes, sizeof(pzem));
}

bool PZEM004T_isReady()
{
  return serialPtr->available() >= RESPONSE_SIZE;
}

bool PZEM004T_recieve(uint8_t resp, uint8_t *data)
{
  uint8_t buffer[RESPONSE_SIZE];

  uint8_t len = 0;
  while ((len < RESPONSE_SIZE))
  {
    if (serialPtr->available() > 0)
    {
      uint8_t c = (uint8_t)serialPtr->read();
      if (!c && !len)
        continue; // skip 0 at startup
      buffer[len++] = c;
    }
  }

  if (len != RESPONSE_SIZE)
    return false;

  if (buffer[6] != PZEM004T_crc(buffer, len - 1))
    return false;

  if (buffer[0] != resp)
    return false;

  if (data)
  {
    for (int i = 0; i < RESPONSE_DATA_SIZE; i++)
      data[i] = buffer[1 + i];
  }

  return true;
}

uint8_t PZEM004T_crc(uint8_t *data, uint8_t sz)
{
  uint16_t crc = 0;
  for (uint8_t i = 0; i < sz; i++)
    crc += *data++;
  return (uint8_t)(crc & 0xFF);
}

typedef enum
{
  SET_ADDRESS,
  READ_VOLTAGE,
  READ_CURRENT,
  READ_POWER,
  READ_ENERGY,
} PZEMReadStates;

PZEMReadStates _readState = SET_ADDRESS;

float pzem_voltage = 0;
float pzem_current = 0;
float pzem_truePower = 0;
float pzem_Whperiod = 0;
float pzem_Whtotal;
float pzem_Whstart;

unsigned long pzem_kWhtoday; // Wh * 10^-5 (deca micro Watt hours)

IPAddress ip(192, 168, 1, 1);
Ticker tickerPZEM;

byte pzem_pminflg = 0;
byte pzem_pmaxflg = 0;
byte pzem_uminflg = 0;
byte pzem_umaxflg = 0;
byte pzem_iminflg = 0;
byte pzem_imaxflg = 0;

byte pzem_fifth_second;
byte pzem_startup;
byte pzem_sendRetry = 0;
byte power_steady_cntr;

void pzem_init()
{
  pzem_kWhtoday = 0;
  pzem_startup = 1;

  PZEM004T_init(pin[GPIO_PZEM_RX], pin[GPIO_PZEM_TX]);

  tickerPZEM.attach_ms(200, pzem_200ms);
}

void pzem0004t_loop()
{
  bool dataReady = PZEM004T_isReady();

  if (dataReady) {
    float pzem_value;
    switch (_readState) {
      case SET_ADDRESS:
        if (PZEM004T_setAddress_rcv(ip)) _readState = READ_VOLTAGE;
        break;
      case READ_VOLTAGE:
        pzem_value = PZEM004T_voltage_rcv(ip);
        if (pzem_value != PZEM_ERROR_VALUE) {
          pzem_voltage = pzem_value;
          _readState = READ_CURRENT;
        }
        break;
      case READ_CURRENT:
        pzem_value = PZEM004T_current_rcv(ip);
        if (pzem_value != PZEM_ERROR_VALUE) {
          pzem_current = pzem_value;
          _readState = READ_POWER;
        }
        break;
      case READ_POWER:
        pzem_value = PZEM004T_power_rcv(ip);
        if (pzem_value != PZEM_ERROR_VALUE) {
          pzem_truePower = pzem_value;
          _readState = READ_ENERGY;
        }
        break;
      case READ_ENERGY:
        pzem_value = PZEM004T_energy_rcv(ip);
        if (pzem_value != PZEM_ERROR_VALUE) {
          pzem_Whtotal = pzem_value;
          _readState = READ_VOLTAGE;

          if (!pzem_startup) {
            if (pzem_Whtotal < pzem_Whstart) {
              pzem_Whstart = pzem_Whtotal;
              sysCfg.pzem_Whstart = pzem_Whstart * 1000;
            }

            pzem_kWhtoday = (pzem_Whtotal - pzem_Whstart) * 100000;
          }
        }
        break;
    }
  }

  if (pzem_sendRetry == 0 || dataReady) {
    pzem_sendRetry = 5;

    switch (_readState) {
      case SET_ADDRESS:
        PZEM004T_setAddress_snd(ip);
        break;
      case READ_VOLTAGE:
        PZEM004T_voltage_snd(ip);
        break;
      case READ_CURRENT:
        PZEM004T_current_snd(ip);
        break;
      case READ_POWER:
        PZEM004T_power_snd(ip);
        break;
      case READ_ENERGY:
        PZEM004T_energy_snd(ip);
        break;
    }
  }
  else {
    pzem_sendRetry--;
  }
}

void pzem_200ms()
{
  pzem_fifth_second++;
  if (pzem_fifth_second == 5) {
    pzem_fifth_second = 0;

    if (rtc_loctime() == rtc_midnight()) {
      sysCfg.wattmtr_kWhyesterday = pzem_kWhtoday;
      pzem_kWhtoday = 0;
      pzem_Whstart = pzem_Whtotal;
      sysCfg.pzem_Whstart = pzem_Whstart * 1000;
    }

    if (pzem_startup && rtcTime.Valid && (rtcTime.DayOfYear == sysCfg.wattmtr_kWhdoy)) {
      pzem_kWhtoday = sysCfg.wattmtr_kWhtoday;
      pzem_Whstart = (float)sysCfg.pzem_Whstart / 1000;
      pzem_startup = 0;
    }
  }

  pzem0004t_loop();
}

void wattmtr_savestate()
{
  sysCfg.wattmtr_kWhdoy = (rtcTime.Valid) ? rtcTime.DayOfYear : 0;
  sysCfg.wattmtr_kWhtoday = pzem_kWhtoday;
}
/********************************************************************************************/

boolean pzem_margin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &saveflag)
{
  byte change;

  if (!margin) return false;
  change = saveflag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  saveflag = flag;
  return (change != saveflag);
}

void wattmtr_setPowerSteadyCounter(byte value)
{
  power_steady_cntr = value;
}

void wattmtr_margin_chk()
{
  char log[LOGSZ], svalue[200];  // was MESSZ
  uint16_t uped, piv;
  boolean flag, jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  if ((Maxdevice == 0 || power) && (sysCfg.wattmtr_pmin || sysCfg.wattmtr_pmax || sysCfg.wattmtr_umin || sysCfg.wattmtr_umax || sysCfg.wattmtr_imin || sysCfg.wattmtr_imax)) {
    piv = (uint16_t)(pzem_current * 1000);

    //    snprintf_P(log, sizeof(log), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
    //    addLog(LOG_LEVEL_DEBUG, log);

    snprintf_P(svalue, sizeof(svalue), PSTR("{"));
    jsonflg = 0;
    if (pzem_margin(0, sysCfg.wattmtr_pmin, pzem_truePower, flag, pzem_pminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerLow\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (pzem_margin(1, sysCfg.wattmtr_pmax, pzem_truePower, flag, pzem_pmaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerHigh\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (pzem_margin(0, sysCfg.wattmtr_umin, pzem_voltage, flag, pzem_uminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageLow\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (pzem_margin(1, sysCfg.wattmtr_umax, pzem_truePower, flag, pzem_umaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageHigh\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (pzem_margin(0, sysCfg.wattmtr_imin, piv, flag, pzem_iminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentLow\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (pzem_margin(1, sysCfg.wattmtr_imax, piv, flag, pzem_imaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentHigh\":\"%s\""), svalue, (jsonflg) ? ", " : "", getStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
      mqtt_publish_topic_P(1, PSTR("MARGINS"), svalue);
    }
  }
}

/*********************************************************************************************\
   Commands
  \*********************************************************************************************/

boolean wattmtr_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;

  if (!strcmp(type, "POWERLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.wattmtr_pmin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerLow\":\"%d%s\"}"), sysCfg.wattmtr_pmin, (sysCfg.flag.value_units) ? " W" : "");
  }
  else if (!strcmp(type, "POWERHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.wattmtr_pmax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerHigh\":\"%d%s\"}"), sysCfg.wattmtr_pmax, (sysCfg.flag.value_units) ? " W" : "");
  }
  else if (!strcmp(type, "VOLTAGELOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.wattmtr_umin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"VoltageLow\":\"%d%s\"}"), sysCfg.wattmtr_umin, (sysCfg.flag.value_units) ? " V" : "");
  }
  else if (!strcmp(type, "VOLTAGEHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.wattmtr_umax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("[\"VoltageHigh\":\"%d%s\"}"), sysCfg.wattmtr_umax, (sysCfg.flag.value_units) ? " V" : "");
  }
  else if (!strcmp(type, "CURRENTLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.wattmtr_imin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentLow\":\"%d%s\"}"), sysCfg.wattmtr_imin, (sysCfg.flag.value_units) ? " mA" : "");
  }
  else if (!strcmp(type, "CURRENTHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.wattmtr_imax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentHigh\":\"%d%s\"}"), sysCfg.wattmtr_imax, (sysCfg.flag.value_units) ? " mA" : "");
  }
  else if (!strcmp(type, "RESETKWH")) {
    pzem_kWhtoday = 0;
    pzem_Whstart = pzem_Whtotal;
    pzem_Whperiod = pzem_Whtotal;
    sysCfg.pzem_Whstart = pzem_Whstart * 1000;
    snprintf_P(svalue, ssvalue, PSTR("{\"ResetKWh\":\"Done\"}"));
  }
  else {
    serviced = false;
  }
  return serviced;
}

/*********************************************************************************************\
   Presentation
  \*********************************************************************************************/

void pzem_mqttStat(bool withPeriod, char* svalue, uint16_t ssvalue)
{
  char sKWHY[10], sKWHT[10], sTruePower[10], sPowerFactor[10], sVoltage[10], sCurrent[10], sPeriod[20];

  if (withPeriod)
  {    
    if (pzem_Whperiod == 0)
    {
      pzem_Whperiod = pzem_Whtotal;
    }
    
    snprintf_P(sPeriod, sizeof(sPeriod), PSTR(", \"Period\":%d"), (int)(pzem_Whtotal - pzem_Whperiod));
    
    pzem_Whperiod = pzem_Whtotal;
  }

  dtostrf((float)sysCfg.wattmtr_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION & 7, sKWHY);
  dtostrf((float)pzem_kWhtoday / 100000000, 1, ENERGY_RESOLUTION & 7, sKWHT);
  dtostrf(pzem_truePower, 1, 1, sTruePower);
  dtostrf(pzem_voltage, 1, 1, sVoltage);
  dtostrf(pzem_current, 1, 3, sCurrent);
  snprintf_P(svalue, ssvalue, PSTR("%s\"Yesterday\":%s, \"Today\":%s%s, \"Power\":%s, \"Voltage\":%s, \"Current\":%s}"),
             svalue, sKWHY, sKWHT, (withPeriod) ? sPeriod : "", sTruePower, sVoltage, sCurrent);
}

void wattmtr_mqttPresent()
{
  // {"Time":"2017-03-04T13:37:24", "Yesterday":0.013, "Today":0.000, "Period":0, "Power":0, "Factor":0.00, "Voltage":0, "Current":0.000}
  char svalue[200];  // was MESSZ

  snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", "), getDateTime().c_str());
  pzem_mqttStat(true, svalue, sizeof(svalue));

  //  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/ENERGY"), sysCfg.mqtt_prefix[2], sysCfg.mqtt_topic);
  //  mqtt_publish(stopic, svalue);

  mqtt_publish_topic_P(1, PSTR("ENERGY"), svalue);
}

void wattmtr_mqttStatus(char* svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"StatusPWR\":{"));
  pzem_mqttStat(false, svalue, ssvalue);
  snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM =
  "<tr><th>Voltage</th><td>%s V</td></tr>"
  "<tr><th>Current</th><td>%s A</td></tr>"
  "<tr><th>Power</th><td>%s W</td></tr>"
  "<tr><th>Energy Today</th><td>%s kWh</td></tr>"
  "<tr><th>Energy Yesterday</th><td>%s kWh</td></tr>"
  "<tr><th>Energy Total</th><td>%s kWh</td></tr>";

String wattmtr_webPresent()
{
  String page = "";
  char sKWHY[10], sKWHT[10], sWHTotal[10], sTruePower[10], sVoltage[10], sCurrent[10], sensor[300];

  dtostrf((float)sysCfg.wattmtr_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION & 7, sKWHY);
  dtostrf((float)pzem_kWhtoday / 100000000, 1, ENERGY_RESOLUTION & 7, sKWHT);
  dtostrf(pzem_Whtotal / 1000, 1, ENERGY_RESOLUTION & 7, sWHTotal);
  dtostrf(pzem_truePower, 1, 1, sTruePower);
  dtostrf(pzem_voltage, 1, 1, sVoltage);
  dtostrf(pzem_current, 1, 3, sCurrent);

  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, sVoltage, sCurrent, sTruePower, sKWHT, sKWHY, sWHTotal);
  page += sensor;
  return page;
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_PZEM004T
