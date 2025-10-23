/*
  xnrg_24_biopdu.ino - BioPDU-625x12 (based on xnrg_05_pzem_ac.ino)

  Copyright (C) 2023-24  Fabrizio Amodio

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

#ifdef ESP32
#if defined(USE_ENERGY_SENSOR) && defined(USE_I2C)
#ifdef USE_BIOPDU
/*********************************************************************************************\
  Biomine 625x12 Custom Board rev 2.0
  6 x 25A Relays
  6 x Independent PZEM-004T-100A-D-P Modbus AC energy sensor
  3bit serial switch
  Integrated MCP23008
  SSD1309 OLED Display I2C

  Template {"NAME":"Olimex ESP32-PoE-BioPDU","GPIO":[1,10209,10210,1,10144,1,0,0,5536,640,1,1,608,0,5600,1,1,1,1,5568,1,1,1,1,6210,0,0,0,1,10208,1,1,10176,1,1,1],"FLAG":0,"BASE":1}

  BioPDU 625x12 Factory Settings:

      Template {"NAME":"Olimex ESP32-PoE-BioPDU","GPIO":[1,10209,10210,1,10144,1,0,0,5536,640,1,1,608,0,5600,1,1,1,1,5568,1,1,1,1,6210,0,0,0,1,10208,1,1,10176,1,1,1],"FLAG":0,"BASE":1}
      Module 0
      EthType 0
      EthAddress 0
      EthClockMode 3
      SetOption21 1
      SetOption26 1
      SetOption65 1
      SetOption129 1
      SetOption150 1
      EnergyDisplay 1
      EnergyCols 6
      displaymodel 17
      displaymode 0
      displayrows 8
      displaycols 25
      PowerOnState1 ON
      PowerLock1 1
      TelePeriod 30
      i2cscan
      Sensor29 0,1,0
      Sensor29 1,5,2
      Sensor29 2,5,2
      Sensor29 3,5,2
      Sensor29 4,5,2
      Sensor29 5,5,2
      Sensor29 6,5,2
      Sensor29 7,1,0

  compile with build flags:

      ${env:tasmota32.build_flags}
      USE_ETHERNET
      ETH_TYPE=0
      ETH_ADDRESS=0
      ETH_CLKMODE=3
      USE_MCP230xx
      USE_MCP230xx_ADDR=0x20
      USE_MCP230xx_OUTPUT
      USE_DISPLAY
      USE_UNIVERSAL_DISPLAY
      USE_ALL_EPD_FONTS
      USE_TINY_FONT
      USE_BIOPDU

extra dirs:

  lib_extra_dirs          = ${env:tasmota32_base.lib_extra_dirs} lib/libesp32, lib/lib_basic, lib/lib_display, lib/lib_ssl, lib/lib_rf, lib/lib_i2c, lib/lib_div

\*********************************************************************************************/

#define XNRG_24 24

#define BIOPDU_MAX_PHASES 6 // BioPDU support max six phases/channels

const uint8_t BIOPDU_DEVICE_ADDRESS = 0x01; // PZEM default address
const uint32_t BIOPDU_STABILIZE = 10;       // Number of seconds to stabilize 1 device

#include <TasmotaModbus.h>
TasmotaModbus *BioPduModbus;

struct BIOPDU
{
  float energy = 0;
  float last_energy = 0;
  uint8_t send_retry = 0;
  uint8_t phase = 0;
  uint8_t address = 0;
  uint8_t address_step = ADDR_IDLE;
  uint8_t pins = 0;
  uint8_t current_mux = 99;
} BioPdu;

void BioPduSetPins(uint8_t current)
{
  if (BioPdu.current_mux != current)
  {
    for (uint8_t p = 0; p < BioPdu.pins; p++)
    {
      digitalWrite(Pin(GPIO_BIOPDU_BIT, p), (current + 1) & (1 << p) ? 1 : 0);
    }
    BioPdu.current_mux = current;
  }
}

void BioPduEverySecond(void)
{
  static uint32_t lastms = 0;

  if (millis() >= lastms + 1000)
  {
    lastms = millis();
    EnergyUpdateTotal();
  }
}

void BioPduEvery250ms(void)
{
  bool data_ready = BioPduModbus->ReceiveReady();

  if (data_ready)
  {
    uint8_t buffer[30]; // At least 5 + (2 * 10) = 25

    uint8_t registers = 10;
    if (ADDR_RECEIVE == BioPdu.address_step)
    {
      registers = 2; // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
      BioPdu.address_step--;
    }
    uint8_t error = BioPduModbus->ReceiveBuffer(buffer, registers);
    // AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, BioPduModbus->ReceiveCount());

    if (error)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("PDU: phase %d error %d"), BioPdu.phase, error);
    }
    else
    {
      Energy->data_valid[BioPdu.phase] = 0;
      if (10 == registers)
      {

        //           0     1     2     3     4     5     6     7     8     9           = ModBus register
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
        // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
        // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
        // 01 04 14 08 CE 00 22 00 00 00 00 00 00 00 02 00 00 01 F4 00 00 00 00 33 FE
        Energy->voltage[BioPdu.phase] = (float)((buffer[3] << 8) + buffer[4]) / 10.0f;                                                     // 6553.0 V
        Energy->current[BioPdu.phase] = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0f;           // 4294967.000 A
        Energy->active_power[BioPdu.phase] = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0f;     // 429496729.0 W
        Energy->frequency[BioPdu.phase] = (float)((buffer[17] << 8) + buffer[18]) / 10.0f;                                                 // 50.0 Hz
        Energy->power_factor[BioPdu.phase] = (float)((buffer[19] << 8) + buffer[20]) / 100.0f;                                             // 1.00
        Energy->import_active[BioPdu.phase] = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]) / 1000.0f; // 4294967.295 kWh

        // Sanity check to prevent spike on power ON
        if (Energy->voltage[BioPdu.phase] < 0 || Energy->voltage[BioPdu.phase] > 300                // Voltage range 0-300
            || Energy->current[BioPdu.phase] < 0 || Energy->current[BioPdu.phase] > 25              // Ampere range 0-25
            || Energy->active_power[BioPdu.phase] < 0 || Energy->active_power[BioPdu.phase] > 5500) // Watt range 0-5500
        {
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PDU: phase %d, spike detected"), BioPdu.phase);

          Energy->voltage[BioPdu.phase] = 0;
          Energy->current[BioPdu.phase] = 0;
          Energy->active_power[BioPdu.phase] = 0;
          Energy->frequency[BioPdu.phase] = 0;
          Energy->power_factor[BioPdu.phase] = 0;
          Energy->import_active[BioPdu.phase] = 0;
        }
      }
    }
  }

  if (0 == BioPdu.send_retry || data_ready)
  {
    if (ADDR_SEND == BioPdu.address_step)
    {
      BioPduModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)BioPdu.address);
      BioPdu.address_step--;
    }
    else
    {
      uint8_t gpio = MCP230xx_readGPIO(0);

      BioPdu.send_retry = 1 /*ENERGY_WATCHDOG*/;
      for (uint8_t p = 0; p < Energy->phase_count; p++)
      {
        if (++BioPdu.phase == Energy->phase_count)
        {
          BioPdu.phase = 0;
        }

        if ((gpio >> (BioPdu.phase + 1)) & 1)
        {
          BioPduSetPins(BioPdu.phase);

          delay(1);

          uint8_t res = BioPduModbus->Send(BIOPDU_DEVICE_ADDRESS /*+ BioPdu.phase*/, 0x04, 0, 10);
          if (res != 0)
          {
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("PDU: phase %d modbus_error="), BioPdu.phase, res);
          }
          break;
        }
      }
    }
  }
  else
  {
    BioPdu.send_retry--;
  }
}

void BioPduSnsInit(void)
{
  BioPduModbus = new TasmotaModbus(Pin(GPIO_BIOPDU_PZEM016_RX), Pin(GPIO_BIOPDU_PZEM0XX_TX));
  uint8_t result = BioPduModbus->Begin(9600);
  if (result)
  {
    if (2 == result)
    {
      ClaimSerial();
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("PDU: Serial UART%d"), BioPduModbus->getUart());
#endif
    BioPdu.phase = Energy->phase_count - 1;
  }
  else
  {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void BioPduDrvInit(void)
{
  if (PinUsed(GPIO_BIOPDU_PZEM016_RX) && PinUsed(GPIO_BIOPDU_PZEM0XX_TX) && PinUsed(GPIO_BIOPDU_BIT))
  {
    TasmotaGlobal.energy_driver = XNRG_24;

    AddLog(LOG_LEVEL_DEBUG, PSTR("PDU: checking pins"));

    for (uint8_t p = 0; p < 3; p++)
    {
      if (PinUsed(GPIO_BIOPDU_BIT, p))
      {
        pinMode(Pin(GPIO_BIOPDU_BIT, p), OUTPUT);
        digitalWrite(Pin(GPIO_BIOPDU_BIT, p), 0);

        AddLog(LOG_LEVEL_DEBUG, PSTR("PDU: Add GPIO %d/%d for pin %d"), GPIO_BIOPDU_BIT, p, BioPdu.pins);

        BioPdu.pins++;
      }
      else
      {
        break;
      }
    }

    Energy->phase_count = std::min((uint8_t)(pow(2, BioPdu.pins) - 1), (uint8_t)BIOPDU_MAX_PHASES); // Start off with 6 phases

    AddLog(LOG_LEVEL_DEBUG, PSTR("PDU: number of pins=%d, max_phase=%d"), BioPdu.pins, Energy->phase_count);
  }
}

bool BioPduCommand(void)
{
  bool serviced = true;

  if (CMND_MODULEADDRESS == Energy->command_code)
  {
    BioPdu.address = XdrvMailbox.payload; // Valid addresses are 1, 2 and 3
    BioPdu.address_step = ADDR_SEND;
  }
  else
    serviced = false; // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg24(uint32_t function)
{
  bool result = false;

  switch (function)
  {
  case FUNC_EVERY_250_MSECOND:
    if (TasmotaGlobal.uptime > 4)
    {
      BioPduEvery250ms();
    } // Fix start up issue #5875
    break;
  case FUNC_ENERGY_EVERY_SECOND:
    if (TasmotaGlobal.uptime > (BIOPDU_STABILIZE * BIOPDU_MAX_PHASES))
    {
      BioPduEverySecond();
    }
    break;
  case FUNC_COMMAND:
    result = BioPduCommand();
    break;
  case FUNC_INIT:
    BioPduSnsInit();
    break;
  case FUNC_PRE_INIT:
    BioPduDrvInit();
    break;
  }
  return result;
}

#endif // USE_BIOPDU
#endif // USE_ENERGY_SENSOR
#endif // ESP32
