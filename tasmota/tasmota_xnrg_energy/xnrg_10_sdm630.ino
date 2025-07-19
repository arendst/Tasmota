/*
  xnrg_10_sdm630.ino - Eastron SDM630-Modbus energy meter support for Tasmota

  Copyright (C) 2021  Gennaro Tortone and Theo Arends

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
#ifdef USE_SDM630
/*********************************************************************************************\
 * Eastron SDM630-Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XNRG_10             10

// can be user defined in my_user_config.h
#ifndef SDM630_SPEED
  #define SDM630_SPEED      9600    // default SDM630 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef SDM630_ADDR
  #define SDM630_ADDR       1       // default SDM630 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Sdm630Modbus;

#ifdef SDM630_HIGH_UPDATE_RATE
struct sSdm630RequestConfig{
  uint16_t startAddress;
  uint8_t  registerToRead; // according to spec: max 80 register can be read a once
};

const struct sSdm630RequestConfig sdm630ReqConf[] {
  {0x0000, 18*2}, // 0x0000 - 0x0025
  {0x0046, 1*2},  // 0x0046 
  {0x0156, 8*2}   // 0x0156 - 0x0165
};

struct SDM630 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm630;


/* convert data buffer to float value according to IEEE754 */
float convBufToFloat(uint8_t *buffer)
{
  float value;

  ((uint8_t*)&value)[3] = buffer[0];   // Get float values
  ((uint8_t*)&value)[2] = buffer[1];
  ((uint8_t*)&value)[1] = buffer[2];
  ((uint8_t*)&value)[0] = buffer[3];
  return value;
}
#else
const uint16_t sdm630_start_addresses[] {
           // 3P4 3P3 1P2 Unit Description
  0x0000,  //  +   -   +   V    Phase 1 line to neutral volts
  0x0002,  //  +   -   -   V    Phase 2 line to neutral volts
  0x0004,  //  +   -   -   V    Phase 3 line to neutral volts
  0x0006,  //  +   +   +   A    Phase 1 current
  0x0008,  //  +   +   -   A    Phase 2 current
  0x000A,  //  +   +   -   A    Phase 3 current
  0x000C,  //  +   -   +   W    Phase 1 power
  0x000E,  //  +   -   +   W    Phase 2 power
  0x0010,  //  +   -   -   W    Phase 3 power
  0x0018,  //  +   -   +   var  Phase 1 volt amps reactive
  0x001A,  //  +   -   -   var  Phase 2 volt amps reactive
  0x001C,  //  +   -   -   var  Phase 3 volt amps reactive
  0x001E,  //  +   -   +        Phase 1 power factor
  0x0020,  //  +   -   -        Phase 2 power factor
  0x0022,  //  +   -   -        Phase 3 power factor
  0x0046,  //  +   +   +   Hz   Frequency of supply voltages
  0x0160,  //  +   +   +   kWh  Phase 1 export active energy
  0x0162,  //  +   +   +   kWh  Phase 2 export active energy
  0x0164,  //  +   +   +   kWh  Phase 3 export active energy
//#ifdef SDM630_IMPORT
  0x015A,  //  +   +   +   kWh  Phase 1 import active energy
  0x015C,  //  +   +   +   kWh  Phase 2 import active energy
  0x015E,  //  +   +   +   kWh  Phase 3 import active energy
//#endif  // SDM630_IMPORT
  0x0156   //  +   +   +   kWh  Total active energy
};

struct SDM630 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm630;
#endif

/*********************************************************************************************/

#ifdef SDM630_HIGH_UPDATE_RATE
uint8_t sdm630ReadBuffer[128]; // at least 5 + (2*max_RegisterToRead)
#endif

void SDM630Every250ms(void)
{
  bool data_ready = Sdm630Modbus->ReceiveReady();

  if (data_ready) {
#ifdef SDM630_HIGH_UPDATE_RATE
    uint8_t* buffer = &sdm630ReadBuffer[0];
    uint32_t error = Sdm630Modbus->ReceiveBuffer(buffer, sdm630ReqConf[Sdm630.read_state].registerToRead);
#else
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9
    uint32_t error = Sdm630Modbus->ReceiveBuffer(buffer, 2);
#endif
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Sdm630Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: SDM630 error %d"), error);
    } else {
      Energy->data_valid[0] = 0;
      Energy->data_valid[1] = 0;
      Energy->data_valid[2] = 0;

#ifndef SDM630_HIGH_UPDATE_RATE
      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];
#endif

      switch(Sdm630.read_state) {
#ifdef SDM630_HIGH_UPDATE_RATE
        case 0: // start address 0x0000                               // 3P4 3P3 1P2 Unit  Description
          Energy->voltage[0] = convBufToFloat(&buffer[3]);            //  +   -   +   V    Phase 1 line to neutral volts
          Energy->voltage[1] = convBufToFloat(&buffer[7]);            //  +   -   -   V    Phase 2 line to neutral volts
          Energy->voltage[2] = convBufToFloat(&buffer[11]);           //  +   -   -   V    Phase 3 line to neutral volts

          //0x0006
          Energy->current[0] = convBufToFloat(&buffer[15]);           //  +   +   +   A    Phase 1 current
          Energy->current[1] = convBufToFloat(&buffer[19]);           //  +   +   -   A    Phase 2 current
          Energy->current[2] = convBufToFloat(&buffer[23]);           //  +   +   -   A    Phase 3 current

          //0x000C
          Energy->active_power[0] = convBufToFloat(&buffer[27]);      //  +   -   +   W    Phase 1 power
          Energy->active_power[1] = convBufToFloat(&buffer[31]);      //  +   -   -   W    Phase 2 power
          Energy->active_power[2] = convBufToFloat(&buffer[35]);      //  +   -   -   W    Phase 3 power

          //0x0012
          Energy->apparent_power[0] = convBufToFloat(&buffer[39]);    //  +   -   +   VA   Phase 1 volt amps
          Energy->apparent_power[1] = convBufToFloat(&buffer[43]);    //  +   -   -   VA   Phase 2 volt amps
          Energy->apparent_power[2] = convBufToFloat(&buffer[47]);    //  +   -   -   VA   Phase 3 volt amps

          //0x0018
          Energy->reactive_power[0] = convBufToFloat(&buffer[51]);    //  +   -   +   var  Phase 1 volt amps reactive
          Energy->reactive_power[1] = convBufToFloat(&buffer[55]);    //  +   -   -   var  Phase 2 volt amps reactive
          Energy->reactive_power[2] = convBufToFloat(&buffer[59]);    //  +   -   -   var  Phase 3 volt amps reactive

          //0x001E
          Energy->power_factor[0] = convBufToFloat(&buffer[63]);      //  +   -   +        Phase 1 power factor
          Energy->power_factor[1] = convBufToFloat(&buffer[67]);      //  +   -   -        Phase 2 power factor
          Energy->power_factor[2] = convBufToFloat(&buffer[71]);      //  +   -   -        Phase 3 power factor
          break;

        case 1: // start address 0x0046
          Energy->frequency[0] = convBufToFloat(&buffer[3]);          //  +   +   +   Hz   Frequency of supply voltages
          break;

        case 2: // start address 0x0156
          //   0x0156   //  +   +   +   kWh  Total active energy
          //   0x0158   //  +   +   +   kvarh Total reactive energy

#ifdef SDM630_IMPORT
          //0x015A
          Energy->import_active[0] = convBufToFloat(&buffer[11]);     //  +   +   +   kWh  Phase 1 import active energy
          Energy->import_active[1] = convBufToFloat(&buffer[15]);     //  +   +   +   kWh  Phase 2 import active energy
          Energy->import_active[2] = convBufToFloat(&buffer[19]);     //  +   +   +   kWh  Phase 3 import active energy
#endif
          //0x0160
          Energy->export_active[0] = convBufToFloat(&buffer[23]);     //  +   +   +   kWh  Phase 1 export active energy
          Energy->export_active[1] = convBufToFloat(&buffer[27]);     //  +   +   +   kWh  Phase 2 export active energy
          Energy->export_active[2] = convBufToFloat(&buffer[31]);     //  +   +   +   kWh  Phase 3 export active energy

          EnergyUpdateTotal();
          break;

#else //old sdm630 implementation
        case 0:
          Energy->voltage[0] = value;
          break;

        case 1:
          Energy->voltage[1] = value;
          break;

        case 2:
          Energy->voltage[2] = value;
          break;

        case 3:
          Energy->current[0] = value;
          break;

        case 4:
          Energy->current[1] = value;
          break;

        case 5:
          Energy->current[2] = value;
          break;

        case 6:
          Energy->active_power[0] = value;
          break;

        case 7:
          Energy->active_power[1] = value;
          break;

        case 8:
          Energy->active_power[2] = value;
          break;

        case 9:
          Energy->reactive_power[0] = value;
          break;

        case 10:
          Energy->reactive_power[1] = value;
          break;

        case 11:
          Energy->reactive_power[2] = value;
          break;

        case 12:
          Energy->power_factor[0] = value;
          break;

        case 13:
          Energy->power_factor[1] = value;
          break;

        case 14:
          Energy->power_factor[2] = value;
          break;

        case 15:
          Energy->frequency[0] = value;
          break;

        case 16:
          Energy->export_active[0] = value;
          break;

        case 17:
          Energy->export_active[1] = value;
          break;

        case 18:
          Energy->export_active[2] = value;
          break;

        case 19:
          Energy->import_active[0] = value;
          break;

        case 20:
          Energy->import_active[1] = value;
          break;

        case 21:
          Energy->import_active[2] = value;
          break;

        case 22:
//          Energy->import_active[0] = value;
          EnergyUpdateTotal();
          break;
#endif
      }

      Sdm630.read_state++;
#ifdef SDM630_HIGH_UPDATE_RATE
      if ( nitems(sdm630ReqConf) == Sdm630.read_state) {
#else
      if (sizeof(sdm630_start_addresses)/2 == Sdm630.read_state) {
#endif
        Sdm630.read_state = 0;
      }
    }
  } // end data ready

  if (0 == Sdm630.send_retry || data_ready) {
    Sdm630.send_retry = 5;
#ifdef SDM630_HIGH_UPDATE_RATE
    Sdm630Modbus->Send(SDM630_ADDR, 0x04, sdm630ReqConf[Sdm630.read_state].startAddress, sdm630ReqConf[Sdm630.read_state].registerToRead);
#else
    Sdm630Modbus->Send(SDM630_ADDR, 0x04, sdm630_start_addresses[Sdm630.read_state], 2);
#endif
  } else {
    Sdm630.send_retry--;
  }
}

void Sdm630SnsInit(void)
{
  Sdm630Modbus = new TasmotaModbus(Pin(GPIO_SDM630_RX), Pin(GPIO_SDM630_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = Sdm630Modbus->Begin(SDM630_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy->phase_count = 3;
    Energy->frequency_common = true;             // Use common frequency
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm630DrvInit(void)
{
  if (PinUsed(GPIO_SDM630_RX) && PinUsed(GPIO_SDM630_TX)) {
    TasmotaGlobal.energy_driver = XNRG_10;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg10(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      SDM630Every250ms();
      break;
    case FUNC_INIT:
      Sdm630SnsInit();
      break;
    case FUNC_PRE_INIT:
      Sdm630DrvInit();
      break;
  }
  return result;
}

#endif  // USE_SDM630
#endif  // USE_ENERGY_SENSOR
