/*
  xnrg_03_pzem004t.ino - PZEM004T energy sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#ifdef USE_PZEM004T
/*********************************************************************************************\
 * PZEM004T - Energy
 *
 * Source: Victor Ferrer https://github.com/vicfergar/Sonoff-MQTT-OTA-Arduino
 * Based on: PZEM004T library https://github.com/olehs/PZEM004T
 *
 * Hardware Serial will be selected if GPIO1 = [63 PZEM004 Rx] and GPIO3 = [62 PZEM0XX Tx]
\*********************************************************************************************/

#define XNRG_03                  3

#include <TasmotaSerial.h>

TasmotaSerial *PzemSerial;

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

#define PZEM_DEFAULT_READ_TIMEOUT 500

/*********************************************************************************************/

struct PZEMCommand {
  uint8_t command;
  uint8_t addr[4];
  uint8_t data;
  uint8_t crc;
};

IPAddress pzem_ip(192, 168, 1, 1);

uint8_t PzemCrc(uint8_t *data)
{
  uint16_t crc = 0;
  for (uint8_t i = 0; i < sizeof(PZEMCommand) -1; i++) crc += *data++;
  return (uint8_t)(crc & 0xFF);
}

void PzemSend(uint8_t cmd)
{
  PZEMCommand pzem;

  pzem.command = cmd;
  for (uint8_t i = 0; i < sizeof(pzem.addr); i++) pzem.addr[i] = pzem_ip[i];
  pzem.data = 0;

  uint8_t *bytes = (uint8_t*)&pzem;
  pzem.crc = PzemCrc(bytes);

  PzemSerial->flush();
  PzemSerial->write(bytes, sizeof(pzem));
}

bool PzemReceiveReady(void)
{
  return PzemSerial->available() >= (int)sizeof(PZEMCommand);
}

bool PzemRecieve(uint8_t resp, float *data)
{
  //  0  1  2  3  4  5  6
  // A4 00 00 00 00 00 A4 - Set address
  // A0 00 D4 07 00 00 7B - Voltage (212.7V)
  // A1 00 00 0A 00 00 AB - Current (0.1A)
  // A1 00 00 00 00 00 A1 - No current
  // A2 00 16 00 00 00 B8 - Power (22W)
  // A2 00 00 00 00 00 A2 - No power
  // A3 00 08 A4 00 00 4F - Energy (2.212kWh)
  // A3 01 86 9F 00 00 C9 - Energy (99.999kWh)

  uint8_t buffer[sizeof(PZEMCommand)] = { 0 };

  unsigned long start = millis();
  uint8_t len = 0;
  while ((len < sizeof(PZEMCommand)) && (millis() - start < PZEM_DEFAULT_READ_TIMEOUT)) {
    if (PzemSerial->available() > 0) {
      uint8_t c = (uint8_t)PzemSerial->read();
      if (!c && !len) {
        continue;  // skip 0 at startup
      }
      if ((1 == len) && (buffer[0] == c)) {
        len--;
        continue;  // fix skewed data
      }
      buffer[len++] = c;
    }
  }

  AddLogSerial(LOG_LEVEL_DEBUG_MORE, buffer, len);

  if (len != sizeof(PZEMCommand)) {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Pzem comms timeout"));
    return false;
  }
  if (buffer[6] != PzemCrc(buffer)) {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Pzem crc error"));
    return false;
  }
  if (buffer[0] != resp) {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Pzem bad response"));
    return false;
  }

  switch (resp) {
    case RESP_VOLTAGE:
      *data = (float)(buffer[1] << 8) + buffer[2] + (buffer[3] / 10.0);    // 65535.x V
      break;
    case RESP_CURRENT:
      *data = (float)(buffer[1] << 8) + buffer[2] + (buffer[3] / 100.0);   // 65535.xx A
      break;
    case RESP_POWER:
      *data = (float)(buffer[1] << 8) + buffer[2];                         // 65535 W
      break;
    case RESP_ENERGY:
      *data = (float)((uint32_t)buffer[1] << 16) + ((uint16_t)buffer[2] << 8) + buffer[3];  // 16777215 Wh
      break;
  }
  return true;
}

/*********************************************************************************************/

const uint8_t pzem_commands[]  { PZEM_SET_ADDRESS, PZEM_VOLTAGE, PZEM_CURRENT, PZEM_POWER, PZEM_ENERGY };
const uint8_t pzem_responses[] { RESP_SET_ADDRESS, RESP_VOLTAGE, RESP_CURRENT, RESP_POWER, RESP_ENERGY };

uint8_t pzem_read_state = 0;
uint8_t pzem_sendRetry = 0;

void PzemEvery200ms(void)
{
  bool data_ready = PzemReceiveReady();

  if (data_ready) {
    float value = 0;
    if (PzemRecieve(pzem_responses[pzem_read_state], &value)) {
      switch (pzem_read_state) {
        case 1:  // Voltage as 230.2V
          energy_voltage = value;
          break;
        case 2:  // Current as 17.32A
          energy_current = value;
          break;
        case 3:  // Power as 20W
          energy_active_power = value;
          break;
        case 4:  // Total energy as 99999Wh
          if (!energy_start || (value < energy_start)) energy_start = value;  // Init after restart and hanlde roll-over if any
          if (value != energy_start) {
            energy_kWhtoday += (unsigned long)((value - energy_start) * 100);
            energy_start = value;
          }
          EnergyUpdateToday();
          break;
      }
      pzem_read_state++;
      if (5 == pzem_read_state) pzem_read_state = 1;
    }
  }

  if (0 == pzem_sendRetry || data_ready) {
    pzem_sendRetry = 5;
    PzemSend(pzem_commands[pzem_read_state]);
  }
  else {
    pzem_sendRetry--;
  }
}

void PzemSnsInit(void)
{
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  PzemSerial = new TasmotaSerial(pin[GPIO_PZEM004_RX], pin[GPIO_PZEM0XX_TX], 1);
  if (PzemSerial->begin(9600)) {
    if (PzemSerial->hardwareSerial()) { ClaimSerial(); }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void PzemDrvInit(void)
{
  if (!energy_flg) {
    if ((pin[GPIO_PZEM004_RX] < 99) && (pin[GPIO_PZEM0XX_TX] < 99)) {  // Any device with a Pzem004T
      energy_flg = XNRG_03;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg03(byte function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    PzemDrvInit();
  }
  else if (XNRG_03 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        PzemSnsInit();
        break;
      case FUNC_EVERY_200_MSECOND:
        PzemEvery200ms();
        break;
    }
  }
  return result;
}

#endif  // USE_PZEM004T
#endif  // USE_ENERGY_SENSOR
