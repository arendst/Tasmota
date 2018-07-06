/*
  xdrv_03_energy.ino - HLW8012 (Sonoff Pow) and PZEM004T energy sensor support for Sonoff-Tasmota

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
/*********************************************************************************************\
 * HLW8012 and PZEM004T - Energy
\*********************************************************************************************/

#define FEATURE_POWER_LIMIT  true

enum EnergyHardware { ENERGY_NONE, ENERGY_HLW8012, ENERGY_CSE7766, ENERGY_PZEM004T };

enum EnergyCommands {
  CMND_POWERDELTA,
  CMND_POWERLOW, CMND_POWERHIGH, CMND_VOLTAGELOW, CMND_VOLTAGEHIGH, CMND_CURRENTLOW, CMND_CURRENTHIGH,
  CMND_POWERCAL, CMND_POWERSET, CMND_VOLTAGECAL, CMND_VOLTAGESET, CMND_CURRENTCAL, CMND_CURRENTSET,
  CMND_ENERGYRESET, CMND_MAXENERGY, CMND_MAXENERGYSTART,
  CMND_MAXPOWER, CMND_MAXPOWERHOLD, CMND_MAXPOWERWINDOW,
  CMND_SAFEPOWER, CMND_SAFEPOWERHOLD, CMND_SAFEPOWERWINDOW };
const char kEnergyCommands[] PROGMEM =
  D_CMND_POWERDELTA "|"
  D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
  D_CMND_POWERCAL "|" D_CMND_POWERSET "|" D_CMND_VOLTAGECAL "|" D_CMND_VOLTAGESET "|" D_CMND_CURRENTCAL "|" D_CMND_CURRENTSET "|"
  D_CMND_ENERGYRESET "|" D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW ;

float energy_voltage = 0;         // 123.1 V
float energy_current = 0;         // 123.123 A
float energy_power = 0;           // 123.1 W
float energy_power_factor = 0;    // 0.12
float energy_daily = 0;           // 123.123 kWh
float energy_total = 0;           // 12345.12345 kWh
float energy_start = 0;           // 12345.12345 kWh total previous
unsigned long energy_kWhtoday_delta = 0;  // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to energy_kWhtoday (HLW and CSE only)
unsigned long energy_kWhtoday;    // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = energy_daily
unsigned long energy_period = 0;  // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = energy_daily

float energy_power_last[3] = { 0 };
uint8_t energy_power_delta = 0;

bool energy_power_on = true;

byte energy_min_power_flag = 0;
byte energy_max_power_flag = 0;
byte energy_min_voltage_flag = 0;
byte energy_max_voltage_flag = 0;
byte energy_min_current_flag = 0;
byte energy_max_current_flag = 0;

byte energy_power_steady_cntr = 8;  // Allow for power on stabilization
byte energy_max_energy_state = 0;

#if FEATURE_POWER_LIMIT
byte energy_mplr_counter = 0;
uint16_t energy_mplh_counter = 0;
uint16_t energy_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT

byte energy_fifth_second = 0;
Ticker ticker_energy;

/********************************************************************************************/

void EnergyUpdateToday()
{
  if (energy_kWhtoday_delta > 1000) {
    unsigned long delta = energy_kWhtoday_delta / 1000;
    energy_kWhtoday_delta -= (delta * 1000);
    energy_kWhtoday += delta;
  }
  RtcSettings.energy_kWhtoday = energy_kWhtoday;
  energy_daily = (float)energy_kWhtoday / 100000;
  energy_total = (float)(RtcSettings.energy_kWhtotal + energy_kWhtoday) / 100000;
}

/*********************************************************************************************\
 * HLW8012, BL0937 or HJL-01 - Energy (Sonoff Pow, HuaFan, KMC70011, BlitzWolf)
 *
 * Based on Source: Shenzhen Heli Technology Co., Ltd
\*********************************************************************************************/

// HLW8012 based (Sonoff Pow, KMC70011, HuaFan)
#define HLW_PREF            10000    // 1000.0W
#define HLW_UREF             2200    // 220.0V
#define HLW_IREF             4545    // 4.545A
#define HLW_SEL_VOLTAGE         1

// HJL-01 based (BlitzWolf, Homecube, Gosund)
#define HJL_PREF             1362
#define HJL_UREF              822
#define HJL_IREF             3300
#define HJL_SEL_VOLTAGE         0

#define HLW_POWER_PROBE_TIME   10    // Number of seconds to probe for power before deciding none used

byte hlw_select_ui_flag;
byte hlw_ui_flag = 1;
byte hlw_load_off;
byte hlw_cf1_timer;
unsigned long hlw_cf_pulse_length;
unsigned long hlw_cf_pulse_last_time;
unsigned long hlw_cf1_pulse_length;
unsigned long hlw_cf1_pulse_last_time;
unsigned long hlw_cf1_summed_pulse_length;
unsigned long hlw_cf1_pulse_counter;
unsigned long hlw_cf1_voltage_pulse_length;
unsigned long hlw_cf1_current_pulse_length;
unsigned long hlw_energy_period_counter;

unsigned long hlw_power_ratio = 0;
unsigned long hlw_voltage_ratio = 0;
unsigned long hlw_current_ratio = 0;

unsigned long hlw_cf1_voltage_max_pulse_counter;
unsigned long hlw_cf1_current_max_pulse_counter;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void HlwCfInterrupt() ICACHE_RAM_ATTR;
void HlwCf1Interrupt() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void HlwCfInterrupt()  // Service Power
{
  unsigned long us = micros();

  if (hlw_load_off) {  // Restart plen measurement
    hlw_cf_pulse_last_time = us;
    hlw_load_off = 0;
  } else {
    hlw_cf_pulse_length = us - hlw_cf_pulse_last_time;
    hlw_cf_pulse_last_time = us;
    hlw_energy_period_counter++;
  }
}

void HlwCf1Interrupt()  // Service Voltage and Current
{
  unsigned long us = micros();

  hlw_cf1_pulse_length = us - hlw_cf1_pulse_last_time;
  hlw_cf1_pulse_last_time = us;
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    hlw_cf1_summed_pulse_length += hlw_cf1_pulse_length;
    hlw_cf1_pulse_counter++;
    if (10 == hlw_cf1_pulse_counter) {
      hlw_cf1_timer = 8;  // We need up to ten samples within 1 second (low current could take up to 0.3 second)
    }
  }
}

void HlwEverySecond()
{
  unsigned long hlw_len;

  if (hlw_energy_period_counter) {
    hlw_len = 10000 / hlw_energy_period_counter;
    hlw_energy_period_counter = 0;
    if (hlw_len) {
      energy_kWhtoday_delta += ((hlw_power_ratio * Settings.energy_power_calibration) / hlw_len) / 36;
      EnergyUpdateToday();
    }
  }
}

void HlwEvery200ms()
{
  unsigned long hlw_w = 0;
  unsigned long hlw_u = 0;
  unsigned long hlw_i = 0;

  if (micros() - hlw_cf_pulse_last_time > (HLW_POWER_PROBE_TIME * 1000000)) {
    hlw_cf_pulse_length = 0;    // No load for some time
    hlw_load_off = 1;
  }

  if (hlw_cf_pulse_length && energy_power_on && !hlw_load_off) {
    hlw_w = (hlw_power_ratio * Settings.energy_power_calibration) / hlw_cf_pulse_length;
    energy_power = (float)hlw_w / 10;
  } else {
    energy_power = 0;
  }

  hlw_cf1_timer++;
  if (hlw_cf1_timer >= 8) {
    hlw_cf1_timer = 0;
    hlw_select_ui_flag = (hlw_select_ui_flag) ? 0 : 1;
    digitalWrite(pin[GPIO_HLW_SEL], hlw_select_ui_flag);

    if (hlw_cf1_pulse_counter) {
      hlw_cf1_pulse_length = hlw_cf1_summed_pulse_length / hlw_cf1_pulse_counter;
    } else {
      hlw_cf1_pulse_length = 0;
    }
    if (hlw_select_ui_flag == hlw_ui_flag) {
      hlw_cf1_voltage_pulse_length = hlw_cf1_pulse_length;
      hlw_cf1_voltage_max_pulse_counter = hlw_cf1_pulse_counter;

      if (hlw_cf1_voltage_pulse_length && energy_power_on) {     // If powered on always provide voltage
        hlw_u = (hlw_voltage_ratio * Settings.energy_voltage_calibration) / hlw_cf1_voltage_pulse_length;
        energy_voltage = (float)hlw_u / 10;
      } else {
        energy_voltage = 0;
      }

    } else {
      hlw_cf1_current_pulse_length = hlw_cf1_pulse_length;
      hlw_cf1_current_max_pulse_counter = hlw_cf1_pulse_counter;

      if (hlw_cf1_current_pulse_length && energy_power) {   // No current if no power being consumed
        hlw_i = (hlw_current_ratio * Settings.energy_current_calibration) / hlw_cf1_current_pulse_length;
        energy_current = (float)hlw_i / 1000;
      } else {
        energy_current = 0;
      }

    }
    hlw_cf1_summed_pulse_length = 0;
    hlw_cf1_pulse_counter = 0;
  }
}

void HlwInit()
{
  if (!Settings.energy_power_calibration || (4975 == Settings.energy_power_calibration)) {
    Settings.energy_power_calibration = HLW_PREF_PULSE;
    Settings.energy_voltage_calibration = HLW_UREF_PULSE;
    Settings.energy_current_calibration = HLW_IREF_PULSE;
  }

  if (BLITZWOLF_BWSHP2 == Settings.module) {
    hlw_power_ratio = HJL_PREF;
    hlw_voltage_ratio = HJL_UREF;
    hlw_current_ratio = HJL_IREF;
    hlw_ui_flag = HJL_SEL_VOLTAGE;
  } else {
    hlw_power_ratio = HLW_PREF;
    hlw_voltage_ratio = HLW_UREF;
    hlw_current_ratio = HLW_IREF;
    hlw_ui_flag = HLW_SEL_VOLTAGE;
  }

  hlw_cf_pulse_length = 0;
  hlw_cf_pulse_last_time = 0;
  hlw_cf1_pulse_length = 0;
  hlw_cf1_pulse_last_time = 0;
  hlw_cf1_voltage_pulse_length = 0;
  hlw_cf1_current_pulse_length = 0;
  hlw_cf1_voltage_max_pulse_counter = 0;
  hlw_cf1_current_max_pulse_counter = 0;

  hlw_load_off = 1;
  hlw_energy_period_counter = 0;

  hlw_select_ui_flag = 0;  // Voltage;

  pinMode(pin[GPIO_HLW_SEL], OUTPUT);
  digitalWrite(pin[GPIO_HLW_SEL], hlw_select_ui_flag);
  pinMode(pin[GPIO_HLW_CF1], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF1], HlwCf1Interrupt, FALLING);
  pinMode(pin[GPIO_HLW_CF], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF], HlwCfInterrupt, FALLING);

  hlw_cf1_timer = 0;
}

/*********************************************************************************************\
 * CSE7766 - Energy (Sonoff S31 and Sonoff Pow R2)
 *
 * Based on datasheet from http://www.chipsea.com/UploadFiles/2017/08/11144342F01B5662.pdf
\*********************************************************************************************/

#define CSE_NOT_CALIBRATED          0xAA

#define CSE_PULSES_NOT_INITIALIZED  -1

#define CSE_PREF                    1000
#define CSE_UREF                    100

uint8_t cse_receive_flag = 0;

long voltage_cycle = 0;
long current_cycle = 0;
long power_cycle = 0;
unsigned long power_cycle_first = 0;
long cf_pulses = 0;
long cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;

void CseReceived()
{
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  // 55 5A 02 F7 60 00 03 AB 00 40 10 02 60 5D 51 A6 58 03 E9 EF 71 0B 7A 36
  // Hd Id VCal---- Voltage- ICal---- Current- PCal---- Power--- Ad CF--- Ck
  AddLogSerial(LOG_LEVEL_DEBUG_MORE);

  uint8_t header = serial_in_buffer[0];
  if ((header & 0xFC) == 0xFC) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: Abnormal hardware"));
    return;
  }

  // Calculate checksum
  uint8_t checksum = 0;
  for (byte i = 2; i < 23; i++) checksum += serial_in_buffer[i];
  if (checksum != serial_in_buffer[23]) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: " D_CHECKSUM_FAILURE));
    return;
  }

  // Get chip calibration data (coefficients) and use as initial defaults
  if (HLW_UREF_PULSE == Settings.energy_voltage_calibration) {
    long voltage_coefficient = 191200;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      voltage_coefficient = serial_in_buffer[2] << 16 | serial_in_buffer[3] << 8 | serial_in_buffer[4];
    }
    Settings.energy_voltage_calibration = voltage_coefficient / CSE_UREF;
  }
  if (HLW_IREF_PULSE == Settings.energy_current_calibration) {
    long current_coefficient = 16140;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      current_coefficient = serial_in_buffer[8] << 16 | serial_in_buffer[9] << 8 | serial_in_buffer[10];
    }
    Settings.energy_current_calibration = current_coefficient;
  }
  if (HLW_PREF_PULSE == Settings.energy_power_calibration) {
    long power_coefficient = 5364000;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      power_coefficient = serial_in_buffer[14] << 16 | serial_in_buffer[15] << 8 | serial_in_buffer[16];
    }
    Settings.energy_power_calibration = power_coefficient / CSE_PREF;
  }

  uint8_t adjustement = serial_in_buffer[20];
  voltage_cycle = serial_in_buffer[5] << 16 | serial_in_buffer[6] << 8 | serial_in_buffer[7];
  current_cycle = serial_in_buffer[11] << 16 | serial_in_buffer[12] << 8 | serial_in_buffer[13];
  power_cycle = serial_in_buffer[17] << 16 | serial_in_buffer[18] << 8 | serial_in_buffer[19];
  cf_pulses = serial_in_buffer[21] << 8 | serial_in_buffer[22];

  if (energy_power_on) {  // Powered on
    if (adjustement & 0x40) {  // Voltage valid
      energy_voltage = (float)(Settings.energy_voltage_calibration * CSE_UREF) / (float)voltage_cycle;
    }
    if (adjustement & 0x10) {  // Power valid
      if ((header & 0xF2) == 0xF2) {  // Power cycle exceeds range
        energy_power = 0;
      } else {
        if (0 == power_cycle_first) power_cycle_first = power_cycle;  // Skip first incomplete power_cycle
        if (power_cycle_first != power_cycle) {
          power_cycle_first = -1;
          energy_power = (float)(Settings.energy_power_calibration * CSE_PREF) / (float)power_cycle;
        } else {
          energy_power = 0;
        }
      }
    } else {
      power_cycle_first = 0;
      energy_power = 0;  // Powered on but no load
    }
    if (adjustement & 0x20) {  // Current valid
      if (0 == energy_power) {
        energy_current = 0;
      } else {
        energy_current = (float)Settings.energy_current_calibration / (float)current_cycle;
      }
    }
  } else {  // Powered off
    power_cycle_first = 0;
    energy_voltage = 0;
    energy_power = 0;
    energy_current = 0;
  }
}

bool CseSerialInput()
{
  if (cse_receive_flag) {
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    if (24 == serial_in_byte_counter) {
      CseReceived();
      cse_receive_flag = 0;
      return 1;
    }
  } else {
    if ((0x5A == serial_in_byte) && (serial_in_byte_counter)) {  // 0x5A - Packet header 2
      if (serial_in_byte_counter > 1) {       // Sync buffer with data (issue #1907)
        serial_in_buffer[0] = serial_in_buffer[--serial_in_byte_counter];
        serial_in_byte_counter = 1;
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: Fixed out-of-sync"));
      }
      cse_receive_flag = 1;
    } else {
      serial_in_byte_counter = 0;
    }
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
  }
  serial_in_byte = 0;                         // Discard
  return 0;
}

void CseEverySecond()
{
  long cf_frequency = 0;

  if (CSE_PULSES_NOT_INITIALIZED == cf_pulses_last_time) {
    cf_pulses_last_time = cf_pulses;  // Init after restart
  } else {
    if (cf_pulses < cf_pulses_last_time) {  // Rolled over after 65535 pulses
      cf_frequency = (65536 - cf_pulses_last_time) + cf_pulses;
    } else {
      cf_frequency = cf_pulses - cf_pulses_last_time;
    }
    if (cf_frequency && energy_power)  {
      cf_pulses_last_time = cf_pulses;
      energy_kWhtoday_delta += (cf_frequency * Settings.energy_power_calibration) / 36;
      EnergyUpdateToday();
    }
  }
}

#ifdef USE_PZEM004T
/*********************************************************************************************\
 * PZEM004T - Energy
 *
 * Source: Victor Ferrer https://github.com/vicfergar/Sonoff-MQTT-OTA-Arduino
 * Based on: PZEM004T library https://github.com/olehs/PZEM004T
 *
 * Hardware Serial will be selected if GPIO1 = [PZEM Rx] and [GPIO3 = PZEM Tx]
\*********************************************************************************************/

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

bool PzemReceiveReady()
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

void PzemEvery200ms()
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
          energy_power = value;
          break;
        case 4:  // Total energy as 99999Wh
          if (!energy_start || (value < energy_start)) energy_start = value;  // Init after restart and hanlde roll-over if any
          energy_kWhtoday += (value - energy_start) * 100;
          energy_start = value;
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

/********************************************************************************************/
#endif  // USE_PZEM004T

void Energy200ms()
{
  energy_fifth_second++;
  if (5 == energy_fifth_second) {
    energy_fifth_second = 0;

    if (ENERGY_HLW8012 == energy_flg) HlwEverySecond();
    if (ENERGY_CSE7766 == energy_flg) CseEverySecond();

    if (RtcTime.valid) {
      if (LocalTime() == Midnight()) {
        Settings.energy_kWhyesterday = energy_kWhtoday;
        Settings.energy_kWhtotal += energy_kWhtoday;
        RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
        energy_kWhtoday = 0;
        energy_kWhtoday_delta = 0;
        energy_period = energy_kWhtoday;
        EnergyUpdateToday();
        energy_max_energy_state = 3;
      }
      if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == energy_max_energy_state)) {
        energy_max_energy_state = 0;
      }
    }
  }

  energy_power_on = (power &1) | Settings.flag.no_power_on_check;

  if (ENERGY_HLW8012 == energy_flg) HlwEvery200ms();
#ifdef USE_PZEM004T
  if (ENERGY_PZEM004T == energy_flg) PzemEvery200ms();
#endif  // USE_PZEM004T

  float power_factor = 0;
  if (energy_voltage && energy_current && energy_power) {
    power_factor = energy_power / (energy_voltage * energy_current);
    if (power_factor > 1) power_factor = 1;
  }
  energy_power_factor = power_factor;
}

void EnergySaveState()
{
  Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;
  Settings.energy_kWhtoday = energy_kWhtoday;
  RtcSettings.energy_kWhtoday = energy_kWhtoday;
  Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;
}

boolean EnergyMargin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &save_flag)
{
  byte change;

  if (!margin) return false;
  change = save_flag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  save_flag = flag;
  return (change != save_flag);
}

void EnergySetPowerSteadyCounter()
{
  energy_power_steady_cntr = 2;
}

void EnergyMarginCheck()
{
  uint16_t energy_daily_u = 0;
  uint16_t energy_power_u = 0;
  uint16_t energy_voltage_u = 0;
  uint16_t energy_current_u = 0;
  boolean flag;
  boolean jsonflg;

  if (energy_power_steady_cntr) {
    energy_power_steady_cntr--;
    return;
  }

  if (Settings.energy_power_delta) {
    float delta = abs(energy_power_last[0] - energy_power);
    // Any delta compared to minimal delta
    float min_power = (energy_power_last[0] > energy_power) ? energy_power : energy_power_last[0];
    if (((delta / min_power) * 100) > Settings.energy_power_delta) {
      energy_power_delta = 1;
      energy_power_last[1] = energy_power;  // We only want one report so reset history
      energy_power_last[2] = energy_power;
    }
  }
  energy_power_last[0] = energy_power_last[1];  // Shift in history every second allowing power changes to settle for up to three seconds
  energy_power_last[1] = energy_power_last[2];
  energy_power_last[2] = energy_power;

  if (energy_power_on && (Settings.energy_min_power || Settings.energy_max_power || Settings.energy_min_voltage || Settings.energy_max_voltage || Settings.energy_min_current || Settings.energy_max_current)) {
    energy_power_u = (uint16_t)(energy_power);
    energy_voltage_u = (uint16_t)(energy_voltage);
    energy_current_u = (uint16_t)(energy_current * 1000);

//    snprintf_P(log_data, sizeof(log_data), PSTR("HLW: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);
//    AddLog(LOG_LEVEL_DEBUG);

    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
    jsonflg = 0;
    if (EnergyMargin(0, Settings.energy_min_power, energy_power_u, flag, energy_min_power_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_power, energy_power_u, flag, energy_max_power_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(0, Settings.energy_min_voltage, energy_voltage_u, flag, energy_min_voltage_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_voltage, energy_voltage_u, flag, energy_max_voltage_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(0, Settings.energy_min_current, energy_current_u, flag, energy_min_current_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_current, energy_current_u, flag, energy_max_current_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_MARGINS));
      EnergyMqttShow();
    }
  }

#if FEATURE_POWER_LIMIT
  // Max Power
  if (Settings.energy_max_power_limit) {
    if (energy_power > Settings.energy_max_power_limit) {
      if (!energy_mplh_counter) {
        energy_mplh_counter = Settings.energy_max_power_limit_hold;
      } else {
        energy_mplh_counter--;
        if (!energy_mplh_counter) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXPOWERREACHED "\":\"%d%s\"}"), energy_power_u, (Settings.flag.value_units) ? " " D_UNIT_WATT : "");
          MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
          EnergyMqttShow();
          ExecuteCommandPower(1, POWER_OFF, SRC_MAXPOWER);
          if (!energy_mplr_counter) {
            energy_mplr_counter = Settings.param[P_MAX_POWER_RETRY] +1;
          }
          energy_mplw_counter = Settings.energy_max_power_limit_window;
        }
      }
    }
    else if (power && (energy_power_u <= Settings.energy_max_power_limit)) {
      energy_mplh_counter = 0;
      energy_mplr_counter = 0;
      energy_mplw_counter = 0;
    }
    if (!power) {
      if (energy_mplw_counter) {
        energy_mplw_counter--;
      } else {
        if (energy_mplr_counter) {
          energy_mplr_counter--;
          if (energy_mplr_counter) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
            ExecuteCommandPower(1, POWER_ON, SRC_MAXPOWER);
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
            EnergyMqttShow();
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings.energy_max_energy) {
    energy_daily_u = (uint16_t)(energy_daily * 1000);
    if (!energy_max_energy_state && (RtcTime.hour == Settings.energy_max_energy_start)) {
      energy_max_energy_state = 1;
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
      ExecuteCommandPower(1, POWER_ON, SRC_MAXENERGY);
    }
    else if ((1 == energy_max_energy_state) && (energy_daily_u >= Settings.energy_max_energy)) {
      energy_max_energy_state = 2;
      dtostrfd(energy_daily, 3, mqtt_data);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXENERGYREACHED "\":\"%s%s\"}"), mqtt_data, (Settings.flag.value_units) ? " " D_UNIT_KILOWATTHOUR : "");
      MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
      EnergyMqttShow();
      ExecuteCommandPower(1, POWER_OFF, SRC_MAXENERGY);
    }
  }
#endif  // FEATURE_POWER_LIMIT

  if (energy_power_delta) EnergyMqttShow();
}

void EnergyMqttShow()
{
// {"Time":"2017-12-16T11:48:55","ENERGY":{"Total":0.212,"Yesterday":0.000,"Today":0.014,"Period":2.0,"Power":22.0,"Factor":1.00,"Voltage":213.6,"Current":0.100}}
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
  EnergyShow(1);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  energy_power_delta = 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean EnergyCommand()
{
  char command [CMDSZ];
  char sunit[CMDSZ];
  boolean serviced = true;
  uint8_t status_flag = 0;
  uint8_t unit = 0;
  unsigned long nvalue = 0;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kEnergyCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_POWERDELTA == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 101)) {
      Settings.energy_power_delta = (1 == XdrvMailbox.payload) ? DEFAULT_POWER_DELTA : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_power_delta;
    unit = UNIT_PERCENTAGE;
  }
  else if (CMND_POWERLOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_min_power = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_power;
    unit = UNIT_WATT;
  }
  else if (CMND_POWERHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power;
    unit = UNIT_WATT;
  }
  else if (CMND_VOLTAGELOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
      Settings.energy_min_voltage = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_voltage;
    unit = UNIT_VOLT;
  }
  else if (CMND_VOLTAGEHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
      Settings.energy_max_voltage = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_voltage;
    unit = UNIT_VOLT;
  }
  else if (CMND_CURRENTLOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
      Settings.energy_min_current = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_current;
    unit = UNIT_MILLIAMPERE;
  }
  else if (CMND_CURRENTHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
      Settings.energy_max_current = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_current;
    unit = UNIT_MILLIAMPERE;
  }
  else if ((CMND_ENERGYRESET == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
    char *p;
    unsigned long lnum = strtoul(XdrvMailbox.data, &p, 10);
    if (p != XdrvMailbox.data) {
      switch (XdrvMailbox.index) {
      case 1:
        energy_kWhtoday = lnum *100;
        energy_kWhtoday_delta = 0;
        energy_period = energy_kWhtoday;
        Settings.energy_kWhtoday = energy_kWhtoday;
        RtcSettings.energy_kWhtoday = energy_kWhtoday;
        energy_daily = (float)energy_kWhtoday / 100000;
        break;
      case 2:
        Settings.energy_kWhyesterday = lnum *100;
        break;
      case 3:
        RtcSettings.energy_kWhtotal = lnum *100;
        Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;
        break;
      }
    }
    char energy_yesterday_chr[10];
    char stoday_energy[10];
    char energy_total_chr[10];
    dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);
    dtostrfd((float)RtcSettings.energy_kWhtoday / 100000, Settings.flag2.energy_resolution, stoday_energy);
    dtostrfd((float)(RtcSettings.energy_kWhtotal + energy_kWhtoday) / 100000, Settings.flag2.energy_resolution, energy_total_chr);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s}}"),
      command, energy_total_chr, energy_yesterday_chr, stoday_energy);
    status_flag = 1;
  }

  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_POWERCAL == command_code)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_power_calibration = (XdrvMailbox.payload > 4000) ? XdrvMailbox.payload : HLW_PREF_PULSE;  // HLW = 12530, CSE = 5364
    }
    nvalue = Settings.energy_power_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_POWERSET == command_code)) {  // Watt
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3601)) {
      if ((ENERGY_HLW8012 == energy_flg) && hlw_cf_pulse_length) {
        Settings.energy_power_calibration = (XdrvMailbox.payload * 10 * hlw_cf_pulse_length) / hlw_power_ratio;
      }
      else if ((ENERGY_CSE7766 == energy_flg) && power_cycle) {
        Settings.energy_power_calibration = (XdrvMailbox.payload * power_cycle) / CSE_PREF;
      }
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_POWERCAL));
    nvalue = Settings.energy_power_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_VOLTAGECAL == command_code)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_voltage_calibration = (XdrvMailbox.payload > 999) ? XdrvMailbox.payload : HLW_UREF_PULSE;  // HLW = 1950, CSE = 1912
    }
    nvalue = Settings.energy_voltage_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_VOLTAGESET == command_code)) {  // Volt
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 501)) {
      if ((ENERGY_HLW8012 == energy_flg) && hlw_cf1_voltage_pulse_length) {
        Settings.energy_voltage_calibration = (XdrvMailbox.payload * 10 * hlw_cf1_voltage_pulse_length) / hlw_voltage_ratio;
      }
      else if ((ENERGY_CSE7766 == energy_flg) && voltage_cycle) {
        Settings.energy_voltage_calibration = (XdrvMailbox.payload * voltage_cycle) / CSE_UREF;
      }
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_VOLTAGECAL));
    nvalue = Settings.energy_voltage_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_CURRENTCAL == command_code)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_current_calibration = (XdrvMailbox.payload > 1100) ? XdrvMailbox.payload : HLW_IREF_PULSE;  // HLW = 3500, CSE = 16140
    }
    nvalue = Settings.energy_current_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (((ENERGY_HLW8012 == energy_flg) || (ENERGY_CSE7766 == energy_flg)) && (CMND_CURRENTSET == command_code)) {  // milliAmpere
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 16001)) {
      if ((ENERGY_HLW8012 == energy_flg) && hlw_cf1_current_pulse_length) {
        Settings.energy_current_calibration = (XdrvMailbox.payload * hlw_cf1_current_pulse_length) / hlw_current_ratio;
      }
      else if ((ENERGY_CSE7766 == energy_flg) && current_cycle) {
        Settings.energy_current_calibration = (XdrvMailbox.payload * current_cycle) / 1000;
      }
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_CURRENTCAL));
    nvalue = Settings.energy_current_calibration;
    unit = UNIT_MICROSECOND;
  }

#if FEATURE_POWER_LIMIT
  else if (CMND_MAXPOWER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit;
    unit = UNIT_WATT;
  }
  else if (CMND_MAXPOWERHOLD == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit_hold;
    unit = UNIT_SECOND;
  }
  else if (CMND_MAXPOWERWINDOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit_window;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit;
    unit = UNIT_WATT;
  }
  else if (CMND_SAFEPOWERHOLD == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit_hold;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWERWINDOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
      Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit_window;
    unit = UNIT_MINUTE;
  }
  else if (CMND_MAXENERGY == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_energy = XdrvMailbox.payload;
      energy_max_energy_state = 3;
    }
    nvalue = Settings.energy_max_energy;
    unit = UNIT_WATTHOUR;
  }
  else if (CMND_MAXENERGYSTART == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
      Settings.energy_max_energy_start = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_energy_start;
    unit = UNIT_HOUR;
  }
#endif  // FEATURE_POWER_LIMIT
  else serviced = false;  // Unknown command

  if (serviced && !status_flag) {
    if (Settings.flag.value_units) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE_SPACE_UNIT, command, nvalue, GetTextIndexed(sunit, sizeof(sunit), unit, kUnitNames));
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE, command, nvalue);
    }
  }

  return serviced;
}

/********************************************************************************************/

void EnergyDrvInit()
{
  energy_flg = ENERGY_NONE;
  if ((pin[GPIO_HLW_SEL] < 99) && (pin[GPIO_HLW_CF1] < 99) && (pin[GPIO_HLW_CF] < 99)) {  // Sonoff Pow or any HLW8012 based device
    energy_flg = ENERGY_HLW8012;
  } else if ((SONOFF_S31 == Settings.module) || (SONOFF_POW_R2 == Settings.module)) {     // Sonoff S31 or Sonoff Pow R2
    baudrate = 4800;
    serial_config = SERIAL_8E1;
    energy_flg = ENERGY_CSE7766;
#ifdef USE_PZEM004T
  } else if ((pin[GPIO_PZEM_RX] < 99) && (pin[GPIO_PZEM_TX] < 99)) {  // Any device with a Pzem004T
    energy_flg = ENERGY_PZEM004T;
#endif  // USE_PZEM004T
  }
}

void EnergySnsInit()
{
  if (ENERGY_HLW8012 == energy_flg) HlwInit();

#ifdef USE_PZEM004T
  if (ENERGY_PZEM004T == energy_flg) {  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
    PzemSerial = new TasmotaSerial(pin[GPIO_PZEM_RX], pin[GPIO_PZEM_TX], 1);
    if (PzemSerial->begin(9600)) {
      if (PzemSerial->hardwareSerial()) { ClaimSerial(); }
    } else {
      energy_flg = ENERGY_NONE;
    }
  }
#endif  // USE_PZEM004T

  if (energy_flg) {
    energy_kWhtoday = (RtcSettingsValid()) ? RtcSettings.energy_kWhtoday : (RtcTime.day_of_year == Settings.energy_kWhdoy) ? Settings.energy_kWhtoday : 0;
    energy_kWhtoday_delta = 0;
    energy_period = energy_kWhtoday;
    EnergyUpdateToday();
    ticker_energy.attach_ms(200, Energy200ms);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM = "%s"
  "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}"
  "{s}" D_ENERGY_TODAY  "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void EnergyShow(boolean json)
{
  char energy_total_chr[10];
  char energy_daily_chr[10];
  char energy_period_chr[10];
  char energy_power_chr[10];
  char energy_voltage_chr[10];
  char energy_current_chr[10];
  char energy_power_factor_chr[10];
  char energy_yesterday_chr[10];
  char speriod[20];

  bool show_energy_period = (0 == tele_period);

  float energy = 0;
  if (show_energy_period) {
    if (energy_period) energy = (float)(energy_kWhtoday - energy_period) / 100;
    energy_period = energy_kWhtoday;
  }

  dtostrfd(energy_total, Settings.flag2.energy_resolution, energy_total_chr);
  dtostrfd(energy_daily, Settings.flag2.energy_resolution, energy_daily_chr);
  dtostrfd(energy, Settings.flag2.wattage_resolution, energy_period_chr);
  dtostrfd(energy_power, Settings.flag2.wattage_resolution, energy_power_chr);
  dtostrfd(energy_voltage, Settings.flag2.voltage_resolution, energy_voltage_chr);
  dtostrfd(energy_current, Settings.flag2.current_resolution, energy_current_chr);
  dtostrfd(energy_power_factor, 2, energy_power_factor_chr);
  dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);

  if (json) {
    snprintf_P(speriod, sizeof(speriod), PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s%s,\"" D_JSON_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
      mqtt_data, energy_total_chr, energy_yesterday_chr, energy_daily_chr, (show_energy_period) ? speriod : "", energy_power_chr, energy_power_factor_chr, energy_voltage_chr, energy_current_chr);
#ifdef USE_DOMOTICZ
    if (show_energy_period) {  // Only send if telemetry
      dtostrfd(energy_total * 1000, 1, energy_total_chr);
      DomoticzSensorPowerEnergy((uint16_t)energy_power, energy_total_chr);  // PowerUsage, EnergyToday
      DomoticzSensor(DZ_VOLTAGE, energy_voltage_chr);  // Voltage
      DomoticzSensor(DZ_CURRENT, energy_current_chr);  // Current
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (show_energy_period) {
      KnxSensor(KNX_ENERGY_VOLTAGE, energy_voltage);
      KnxSensor(KNX_ENERGY_CURRENT, energy_current);
      KnxSensor(KNX_ENERGY_POWER, energy_power);
      KnxSensor(KNX_ENERGY_POWERFACTOR, energy_power_factor);
      KnxSensor(KNX_ENERGY_DAILY, energy_daily);
      KnxSensor(KNX_ENERGY_TOTAL, energy_total);
      KnxSensor(KNX_ENERGY_START, energy_start);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS, mqtt_data, energy_voltage_chr, energy_current_chr, energy_power_chr, energy_power_factor_chr, energy_daily_chr, energy_yesterday_chr, energy_total_chr);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_03

boolean Xdrv03(byte function)
{
  boolean result = false;

  if (energy_flg) {
    switch (function) {
      case FUNC_PRE_INIT:
        EnergyDrvInit();
        break;
      case FUNC_COMMAND:
        result = EnergyCommand();
        break;
      case FUNC_SET_POWER:
        EnergySetPowerSteadyCounter();
        break;
    }
  }
  return result;
}

#define XSNS_03

boolean Xsns03(byte function)
{
  boolean result = false;

  if (energy_flg) {
    switch (function) {
      case FUNC_INIT:
        EnergySnsInit();
        break;
      case FUNC_EVERY_SECOND:
        EnergyMarginCheck();
        break;
      case FUNC_JSON_APPEND:
        EnergyShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        EnergyShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
        EnergySaveState();
        break;
    }
  }
  return result;
}

#endif  // USE_ENERGY_SENSOR