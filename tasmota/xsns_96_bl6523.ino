/*
  xsns_96_bl6523.ino - Chinese bl6523 based Watt hour meter support for Tasmota

  Copyright (C) 2022  Jeevas Vasudevan and the Internet

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
 * #define USE_BL6523       // Add support for Chinese BL6523 based Watt hour meter (+1k code)¸
 * 
 * After Installation use the below template sample:
 * {"NAME":"BL6523 Smart Meter","GPIO":[0,0,0,0,7488,7520,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":18}
\*********************************************************************************************/

#define XSNS_96 96

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

/* No idea how to derive human readable units from the byte stream.
For now  dividing the 24-bit values with below constants seems to yield something sane
that matches whatever displayed in the screen of my 240v model. Probably it would be possible
to extract these values from the write register commands (0xCA).*/ 
#define BL6523_DIV_AMPS 297899.4f
#define BL6523_DIV_VOLTS 13304.0f
#define BL6523_DIV_FREQ  3907.0f
#define BL6523_DIV_WATTS 707.0f
#define BL6523_DIV_WATTHR 674.0f

TasmotaSerial *Bl6523RxSerial;
TasmotaSerial *Bl6523TxSerial;

struct BL6523
{
  uint32_t amps = 0;
  uint32_t volts = 0;
  uint32_t freq = 0;
  uint32_t watts = 0;
  uint32_t powf = 0;
  uint32_t watthr = 0;

  uint8_t type = 1;
  uint8_t valid = 0;
  uint8_t got_data_stone = 0;
  bool discovery_triggered = false;
} Bl6523;

bool Bl6523ReadData(void)
{

  if (!Bl6523RxSerial->available())
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6523 No Rx Data available " ));
    return false;
  }

  while ((Bl6523RxSerial->peek() != 0x35) && Bl6523RxSerial->available())
  {
    Bl6523RxSerial->read();
  }

  if (Bl6523RxSerial->available() < BL6523_RX_DATASET_SIZE)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("BL6523 Rx less than expected " ));
    return false;
  }

  uint8_t rx_buffer[BL6523_RX_DATASET_SIZE];
  Bl6523RxSerial->readBytes(rx_buffer, BL6523_RX_DATASET_SIZE);
  Bl6523RxSerial->flush(); // Make room for another burst

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, rx_buffer, BL6523_RX_DATASET_SIZE);
  
  while (Bl6523TxSerial->available() < BL6523_TX_DATASET_SIZE)
  {
    // sleep till TX buffer is full
     unsigned long timeout = millis() + 10;
     while (millis() < timeout) {}
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
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("BL6523 : " D_CHECKSUM_FAILURE));
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
    Bl6523.amps =  ((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<0;
    break;
  case BL6523_REG_VOLTS :
    Bl6523.volts = ((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<1;
    break;
  case BL6523_REG_FREQ :
    Bl6523.freq = ((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<2;
    break;        
  case BL6523_REG_WATTS :
    Bl6523.watts = ((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<3;
    break;
  case BL6523_REG_POWF :
    Bl6523.powf = ((tx_buffer[2]  << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<4;
    break;
  case BL6523_REG_WATTHR :
    Bl6523.watthr = ((tx_buffer[2] << 16) | (tx_buffer[1] << 8) | tx_buffer[0]);
    Bl6523.got_data_stone |= 1<<5;
    break;
  default :  
  break;            
}
 
AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Amps: %d Volts: %d Freq: %d Watts: %d PowF: %d WattHr: %d"), Bl6523.amps, Bl6523.volts, Bl6523.freq, Bl6523.watts, Bl6523.powf, Bl6523.watthr);

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
  if ((PinUsed(GPIO_BL6523_RX)) && (PinUsed(GPIO_BL6523_TX)))
  {
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
      Bl6523.type = 1;
      AddLog(LOG_LEVEL_DEBUG, PSTR("BL6523 Init Success " ));
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_BL6523_SNM[] PROGMEM = "{s}BL6523 Smart Energy Monitor{m}{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_BL6523_SNS[] PROGMEM = "{s}       %s  {m}%s {e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Bl6523Show(bool json)
{
  uint32_t powf_word = 0;
  float amps = 0.0f, volts = 0.0f, freq = 0.0f, watts = 0.0f, powf = 0.0f, watthr = 0 .0f;
  char amps_str[12], volts_str[12], freq_str[12];
  char watts_str[12], powf_str[12], watthr_str[12];

  if (Bl6523.valid)
  {

   amps = (float)Bl6523.amps / BL6523_DIV_AMPS;
   volts =  (float)Bl6523.volts / BL6523_DIV_VOLTS;
   freq = (float)Bl6523.freq / BL6523_DIV_FREQ;
   watts = (float)Bl6523.watts / BL6523_DIV_WATTS;

    /* Power factor =(sign bit)*((PF[22]×2^－1）＋（PF[21]×2^－2）＋。。。)
       Eg., reg value  0x7FFFFF(HEX) -> PF 1, 0x800000(HEX) -> -1, 0x400000(HEX) -> 0.5
    */
   powf = 0.0f;
   powf_word = Bl6523.powf & 0x7fffff; //Extract the 23 bits
   for (int i = 0; i < 23; i++){ // Accumulate powf from 23 bits
    powf += ((powf_word >> (22-i)) * pow(2,(0-(i+1)))); 
    powf_word = powf_word & (0x7fffff >> (1+i));
   }
   powf = (Bl6523.powf >> 23) ? (0.0f - (~powf)) : powf; // Negate if sign bit(24) is set
   
   watthr = (float)Bl6523.watthr / BL6523_DIV_WATTHR;

   ext_snprintf_P(amps_str, sizeof(amps_str), PSTR("%3_f"), &amps);
   ext_snprintf_P(volts_str, sizeof(volts_str), PSTR("%2_f"), &volts);
   ext_snprintf_P(freq_str, sizeof(freq_str), PSTR("%2_f"), &freq);
   ext_snprintf_P(watts_str, sizeof(watts_str), PSTR("%3_f"), &watts);
   ext_snprintf_P(powf_str, sizeof(powf_str), PSTR("%2_f"), &powf);
   ext_snprintf_P(watthr_str, sizeof(watthr_str), PSTR("%3_f"), &watthr);

    if (json)
    {
      ResponseAppend_P(PSTR(",\"BL6523\":{"));
      ResponseAppend_P(PSTR("\"Amps\":%s,"), amps_str);
      ResponseAppend_P(PSTR("\"Volts\":%s,"), volts_str);
      ResponseAppend_P(PSTR("\"Freq\":%s,"), freq_str);
      ResponseAppend_P(PSTR("\"Watts\":%s,"), watts_str);
      ResponseAppend_P(PSTR("\"Powf\":%s,"), powf_str);
      ResponseAppend_P(PSTR("\"WattHr\":%s"), watthr_str);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period)
      {
        DomoticzSensor(DZ_CURRENT, amps_str);    // Amps
        DomoticzSensor(DZ_VOLTAGE, volts_str); // Voltage
        DomoticzSensor(DZ_COUNT, freq_str);  // Frequency
        DomoticzSensor(DZ_ILLUMINANCE, watts_str);    // Watts
        DomoticzSensor(DZ_P1_SMART_METER, powf_str); // Power Factor
        DomoticzSensor(DZ_POWER_ENERGY, watthr_str);  // WattHour
      }
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    }
    else
    {
      WSContentSend_PD(HTTP_BL6523_SNM);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("Amps:"), amps_str);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("Volts:"), volts_str);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("Freq:"), freq_str);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("Watts:"), watts_str);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("PowF:"), powf_str);
      WSContentSend_PD(HTTP_BL6523_SNS, PSTR("WattHr:"), watthr_str);
#endif // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns96(uint8_t function)
{
  bool result = false;

  if (Bl6523.type)
  {
    switch (function)
    {
    case FUNC_EVERY_250_MSECOND:
      Bl6523Update();
      break;
    case FUNC_JSON_APPEND:
      Bl6523Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Bl6523Show(0);
      break;
#endif // USE_WEBSERVER
    case FUNC_INIT:
      Bl6523Init();
      break;
    }
  }
  return result;
}

#endif // USE_BL6523