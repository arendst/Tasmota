/*
  xnrg_22_bl6523.ino - BL6523 based Watt hour meter support for Tasmota

  Copyright (C) 2022  Jeevas Vasudevan

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
#ifdef USE_BL6523
/*********************************************************************************************\
 * Chinese BL6523  based Watt hour meter
 *
 * This meter provides accurate Voltage, Frequency, Ampere, Wattage, Power Factor, KWh
 * To use Tasmota the user needs to add an ESP8266 or ESP32
 * Three lines need to be connected via 1KOhh resistors to ESP from the main board(RX,TX GND)
 *
 * Connection Eg (ESP8266) - Non - Isolated:
 *    BL6523 RX ->1KOhm-> ESP IO4(D2) (Should be Input Capable)
 *    BL6523 TX ->1KOhm->  ESP IO5(D1) (Should be Input Capable)
 *    BL6523 GND -> ESP GND
 *
 * Connection Eg (ESP32) - Non - Isolated:
 *    BL6523 RX ->1KOhm-> ESP IO4 (Should be Input Capable)
 *    BL6523 TX ->1KOhm-> ESP IO5 (Should be Input Capable)
 *    BL6523 GND -> ESP GND
 *
 * To build add the below to user_config_override.h
 * #define USE_ENERGY_SENSOR  // Enable Energy sensor framework
 * #define USE_BL6523         // Add support for Chinese BL6523 based Watt hour meter (+1k code)¸
 *
 * After Installation use the below template sample:
 * {"NAME":"BL6523 Smart Meter","GPIO":[0,0,0,0,7488,7520,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
\*********************************************************************************************/

#define XNRG_22 22

#include <TasmotaSerial.h>

#define BL6523_RX_DATASET_SIZE 2
#define BL6523_TX_DATASET_SIZE 4

#define BL6523_BAUD 4800
#define BL6523_REG_AMPS 0x05
#define BL6523_REG_VOLTS 0x07
#define BL6523_REG_FREQ  0x09
#define BL6523_REG_WATTS 0x0A
#define BL6523_REG_POWF  0x08
#define BL6523_REG_WATTHR 0x0C

#define SINGLE_PHASE 0
#define RX_WAIT 100

#define BL6523_IREF 297899
#define BL6523_UREF 13304
#define BL6523_FREF 3907
#define BL6523_PREF 707
#define BL6523_PWHRREF_D 33 // Substract this from BL6523_PREF to get WattHr Div.

TasmotaSerial *Bl6523RxSerial;
TasmotaSerial *Bl6523TxSerial;

struct BL6523
{
  uint8_t type = 1;
  uint8_t valid = 0;
  uint8_t got_data_stone = 0;
  bool discovery_triggered = false;
} Bl6523;

bool Bl6523ReadData(void)
{
   uint32_t powf_word = 0, powf_buf = 0, i = 0;
   float powf = 0.0f;

  if (!Bl6523RxSerial->available())
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:No Rx Data available" ));
    return false;
  }

  while ((Bl6523RxSerial->peek() != 0x35) && Bl6523RxSerial->available())
  {
    Bl6523RxSerial->read();
  }

  if (Bl6523RxSerial->available() < BL6523_RX_DATASET_SIZE)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:Rx less than expected" ));
    return false;
  }

  uint8_t rx_buffer[BL6523_RX_DATASET_SIZE];
  Bl6523RxSerial->readBytes(rx_buffer, BL6523_RX_DATASET_SIZE);
  Bl6523RxSerial->flush(); // Make room for another burst

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, rx_buffer, BL6523_RX_DATASET_SIZE);

  i=0;
  while (Bl6523TxSerial->available() < BL6523_TX_DATASET_SIZE)
  {
    // sleep till TX buffer is full
    delay(10);
    if ( i++ > RX_WAIT ){
      break;
    }
  }


  uint8_t tx_buffer[BL6523_TX_DATASET_SIZE];
  Bl6523TxSerial->readBytes(tx_buffer, BL6523_TX_DATASET_SIZE);
  Bl6523TxSerial->flush(); // Make room for another burst


  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, tx_buffer, BL6523_TX_DATASET_SIZE);

  /* Checksum: （Addr+Data_L+Data_M+Data_H） & 0xFF, then byte invert */
  uint8_t crc = rx_buffer[1]; //Addr
  for (uint32_t i = 0; i < (BL6523_TX_DATASET_SIZE - 1); i++)
  {
    crc += tx_buffer[i]; //Add Data_L,Data_M and Data_H to Addr
  }
  crc &= 0xff; // Bitwise AND 0xFF
  crc = ~crc; // Invert the byte
  if (crc != tx_buffer[BL6523_TX_DATASET_SIZE - 1])
  {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL6:" D_CHECKSUM_FAILURE));
    Bl6523TxSerial->flush();
    Bl6523RxSerial->flush();
    return false;
  }

  /* WRITE DATA (format: command(write->0xCA) address data_low data_mid data_high checksum )
WRITE Sample(RX):
RX: CA 3E 55 00 00 6C (WRPROT - allow)
RX: CA 14 00 00 10 DB (MODE)
RX: CA 15 04 00 00 E6 (GAIN - IB 16x gain )
RX: CA 19 08 00 00 DE (WA_CFDIV )
RX: CA 3E AA 00 00 17 (WRPROT - disable)
*/

  /* READ DATA (format: command(read->0x35) address data_low data_mid data_high checksum )
READ Sample(RX-TX) Data:
RX: 35 05 TX: E4 00 00 16 (IA rms )
RX: 35 07 TX: D5 A3 2E 52 (V rms )
RX: 35 09 TX: F0 FB 02 09 (FREQ)
RX: 35 0A TX: 00 00 00 F5 (WATT)
RX: 35 08 TX: 00 00 00 F7 (PF)
RX: 35 0C TX: 00 00 00 F3 (WATT_HR)
*/

switch(rx_buffer[1]) {
  case BL6523_REG_AMPS :
    Energy->current[SINGLE_PHASE] =  (float)((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]) / EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION);     // 1.260 A
    break;
  case BL6523_REG_VOLTS :
    Energy->voltage[SINGLE_PHASE] = (float)((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]) / EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION);     // 230.2 V
    break;
  case BL6523_REG_FREQ :
    Energy->frequency[SINGLE_PHASE] = (float)((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]) / EnergyGetCalibration(ENERGY_FREQUENCY_CALIBRATION);    // 50.0 Hz
    break;
  case BL6523_REG_WATTS :
    Energy->active_power[SINGLE_PHASE] = (float)((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]) / EnergyGetCalibration(ENERGY_POWER_CALIBRATION); // -196.3 W
    break;
  case BL6523_REG_POWF :
   /* Power factor =(sign bit)*((PF[22]×2^－1）＋（PF[21]×2^－2）＋。。。)
       Eg., reg value  0x7FFFFF(HEX) -> PF 1, 0x800000(HEX) -> -1, 0x400000(HEX) -> 0.5
    */
   powf = 0.0f;
   powf_buf = ((tx_buffer[2]  << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
   powf_word = (powf_buf >> 23) ? ~(powf_buf & 0x7fffff) : powf_buf & 0x7fffff; //Extract the 23 bits and invert if sign bit(24) is set
   for (int i = 0; i < 23; i++){ // Accumulate powf from 23 bits
    powf += ((powf_word >> (22-i)) * pow(2,(0-(i+1))));
    powf_word = powf_word & (0x7fffff >> (1+i));
   }
   powf = (powf_buf >> 23) ? (0.0f - (powf)) : powf; // Negate if sign bit(24) is set
   Energy->power_factor[SINGLE_PHASE] = powf;
    break;
  case BL6523_REG_WATTHR :
    Energy->import_active[SINGLE_PHASE] = (float)((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]) / ( EnergyGetCalibration(ENERGY_POWER_CALIBRATION) - BL6523_PWHRREF_D ); // 6.216 kWh => used in EnergyUpdateTotal()
    break;
  default :
  break;
}
  Energy->data_valid[SINGLE_PHASE] = 0;
  EnergyUpdateTotal();
  if (!Bl6523.discovery_triggered)
  {
    TasmotaGlobal.discovery_counter = 1; // force TasDiscovery()
    Bl6523.discovery_triggered = true;
  }
  return true;

}

/*********************************************************************************************/

void Bl6523Update(void)
{ // Every 250 millisecond
  if (Bl6523ReadData())
  {
    Bl6523.valid = 60;
  }
  else
  {
    if (Bl6523.valid) {
    Bl6523.valid--;
    }
  }
}

/*********************************************************************************************/

void Bl6523Init(void)
{

    Bl6523.type = 0;
    Bl6523RxSerial = new TasmotaSerial(Pin(GPIO_BL6523_RX), -1, 1);
    Bl6523TxSerial = new TasmotaSerial(Pin(GPIO_BL6523_TX), -1, 1);
    if ((Bl6523RxSerial->begin(BL6523_BAUD)) && (Bl6523TxSerial->begin(BL6523_BAUD)))
    {
      if (Bl6523RxSerial->hardwareSerial())
      {
        ClaimSerial();
      }
      if (Bl6523TxSerial->hardwareSerial())
      {
        ClaimSerial();
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6: Serial UART%d and UART%d"), Bl6523RxSerial->getUart(), Bl6523TxSerial->getUart());
#endif
      Bl6523.type = 1;
      Energy->phase_count = 1;
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:Init Success" ));
    }
   else
     {
       AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:Init Failure!" ));
       TasmotaGlobal.energy_driver = ENERGY_NONE;
     }

}

bool Bl6523Command(void) {
  bool serviced = true;

  int32_t value = (int32_t)(CharToFloat(XdrvMailbox.data) * 1000);  // 1.234 = 1234, -1.234 = -1234
  uint32_t abs_value = abs(value) / 10;                             // 1.23 = 123,   -1.23 = 123

  if ((CMND_POWERCAL == Energy->command_code) || (CMND_VOLTAGECAL == Energy->command_code) || (CMND_CURRENTCAL == Energy->command_code)) {
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value > 100) && (abs_value < 2000000)) {    // Between 1.00 and 20000.00 W
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value > 10000) && (abs_value < 40000)) {   // Between 100.00 and 400.00 V
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_CURRENTSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value > 1000) && (abs_value < 10000000)) {  // Between 10.00 mA and 100.00000 A
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_FREQUENCYSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value > 4500) && (abs_value < 6500)) {     // Between 45.00 and 65.00 Hz
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_ENERGYCONFIG == Energy->command_code) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Config index %d, payload %d, value %d, data '%s'"),
      XdrvMailbox.index, XdrvMailbox.payload, value, XdrvMailbox.data ? XdrvMailbox.data : "null" );

    // EnergyConfig1 to 3 = Set Energy->current[channel] in A like 0.417 for 417mA
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index < 4)) {
      //Bl6523.current[XdrvMailbox.index -1] = value;
    }
    // EnergyConfig4 to 6 = Set Energy->active_power[channel] in W like 100 for 100W
    if ((XdrvMailbox.index > 3) && (XdrvMailbox.index < 7)) {
      //Bl6523.power[XdrvMailbox.index -4] = value;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

void Bl6523DrvInit(void)
{
  if (PinUsed(GPIO_BL6523_RX) && PinUsed(GPIO_BL6523_TX)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:PreInit Success" ));
    TasmotaGlobal.energy_driver = XNRG_22;
    if (HLW_PREF_PULSE == EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) {
      EnergySetCalibration(ENERGY_POWER_CALIBRATION, BL6523_PREF);
      EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, BL6523_UREF);
      EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, BL6523_IREF);
      EnergySetCalibration(ENERGY_FREQUENCY_CALIBRATION, BL6523_FREF);
    }
  }
  else
  {
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6:PreInit Failure!" ));
       TasmotaGlobal.energy_driver = ENERGY_NONE;
  }

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg22(uint32_t function)
{
  bool result = false;

    switch (function)
    {
    case FUNC_EVERY_250_MSECOND:
      Bl6523Update();
      break;
    case FUNC_COMMAND:
      result = Bl6523Command();
      break;
    case FUNC_INIT:
      Bl6523Init();
      break;
    case FUNC_PRE_INIT:
      Bl6523DrvInit();
      break;
    }

  return result;
}

#endif // USE_BL6523
#endif  // USE_ENERGY_SENSOR