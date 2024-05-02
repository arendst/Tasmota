/*
  xnrg_25_mk_sky_blu.ino - MakeSkyBlue Solar charger support for Tasmota

  Copyright (C) 2024  meMorizE

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
#ifdef USE_MAKE_SKY_BLUE
/*********************************************************************************************\
 * This implementation communicates with solar charge controller of MakeSkyBlue(Shenzen) Co.LTD 
 * http://www.makeskyblue.com
 * Model: S3-30A/40A/50A/60A MPPT 12V/24V/36V/48V with firmwware V118 (or V119 including a Wifi Box)
 * The device should have a Mini-USB socket at bottom right beside the screw terminals.
 * This socket conforms NOT to the USB standard protocol !
 * It is a TTL serial interface with 9600bps 8N1, D+=Tx D-=Rx plus regular 5V out and GND
 * When using a DIY ESP hardware be aware of the 5V levels by using e.g. level shifter 
 * 
 * The orignal V119 Wifi Box hardware includes an ESP8285 with 1MB Flash (as module 2AL3B ESP-M)
 * It specific hardware uses
 *  GPIO1: TX0,  ESP => Charge controller
 *  GPIO3: RXD0, ESP <= Charge controller
 *  GPIO5 red LED, active low, for e.g. LedLink_i
 *  free: GPIO4, GPIO12, GPIO13, GPIO14, GPIO15, GPIO16, GPIO17(ADC)
 *
 * Useful: command VoltRes 1 to select voltage resolution to 0.1 V (native resolution)
 *         command AmpRes 1 to select current resolution to 0.1 A (native resolution)
 *         SetOption72 to read total energy from the charge controller
 * 
 * Note: SetOption129 should be off, because only the solar energy is typical the main interest
 * 
 * This implementation is based on own logic analyzer recordings and information from
 * https://github.com/lasitha-sparrow/Makeskyblue_wifi_Controller
 * https://www.genetrysolar.com/community/forum-134/topic-1219/
 * 
\*********************************************************************************************/

/* Integration proposal : */
/* 
tasmota_template.h
    ...
    enum UserSelectablePins {
    ...
    GPIO_MKSKYBLU_TX, GPIO_MKSKYBLU_RX,
    ...
    #ifdef USE_ENERGY_SENSOR
    ...
    #ifdef USE_MAKE_SKY_BLUE
    AGPIO(GPIO_MKSKYBLU_TX),     // MakeSkyBlu Solar Charge Controller Serial interface
    AGPIO(GPIO_MKSKYBLU_RX),     // MakeSkyBlu Solar Charge Controller Serial interface
    #endif
    ...
    D_SENSOR_MKSKYBLU_TX "|" D_SENSOR_MKSKYBLU_RX "|"
    ...

be_gpio_defines.h (berry)
    ...
      { "MKSKYBLU_TX", (int32_t) GPIO_MKSKYBLU_TX },
      { "MKSKYBLU_TR", (int32_t) GPIO_MKSKYBLU_RX },
    ...

lv_gpio_enum.h (tools)
    ...
    MKSKYBLU_TX = GPIO_MKSKYBLU_TX
    MKSKYBLU_RX = GPIO_MKSKYBLU_RX
    ...

user_config_override.h
(my_user_config.h)
(tasmota_configurations.h)
(tasmota_configurations_ESP32.h)
    ...
    #define USE_MAKE_SKY_BLUE    // Add Support for MakeSkyBlue Solar Charge Controller
    ...

\tasmota\language\*.h
    ...
    #define D_SENSOR_MKSKYBLU_TX     "MkSkyBlu Tx"
    #define D_SENSOR_MKSKYBLU_RX     "MkSkyBlu Rx"
    ...
 */


#include <TasmotaSerial.h>

#define XNRG_25                   25

/* compile options */
#define _VARIANT_ENERGY_IMPORT    2 /* 0=never import, 1=import once, 2=import changed full kWh */
#define _WITH_CONFIG_REGISTER_SUPPORT
#define _WITH_ON_OFF_SUPPORT
#define _WITH_SERIAL_DEBUGGING
//#define _WITH_PHASE_NAME_SUPPORT /* requires new element Energy->phase_names at xdrv_03_energy and xdrv_03_esp32_energy */


#ifdef _WITH_SERIAL_DEBUGGING
  #define MKSB_COM_RX_IncrementCnt( cnt )  { me.cntRx[cnt]++; if ( cnt ) me.lastRxCntError = cnt; }
#else
  #define MKSB_COM_RX_IncrementCnt( cnt )  (void)0
#endif

#define MKSB_BAUDRATE             9600
#define MKSB_TIMEOUT              3     // seconds

// first byte of every valid frame
#define MKSB_START_FRAME          0xAA

// second byte: Request to the charge controller
#define MKSB_CMD_READ_STATUS      0x55
#define MKSB_CMD_READ_CONFIG      0xCB
#define MKSB_CMD_WRITE_CONFIG     0xCA
#define MKSB_CMD_POWER            0xCC

// second byte: Response from the charge controller
#define MKSB_RSP_READ_STATUS      0xBB
#define MKSB_RSP_RW_CONFIG        0xDA
#define MKSB_RSP_POWER            0xDD

#define MKSB_BUFFER_SIZE          24 // bytes

// config registers                   
#define MKSB_REG_SPECIAL          0   //     ?
#define MKSB_REG_VOLTAGE_BULK     1   // D02 MPPT Voltage limit BULK, >= stops charging [mV], e.g. 55000mV
#define MKSB_REG_VOLTAGE_FLOAT    2   // D01 MPPT voltage limit FLOAT, <= restarts charging [mV], SLA battery only
#define MKSB_REG_OUT_TIMER        3   // D00 Time duration the load gets connected [mh], default: 24h = 24000mh
#define MKSB_REG_CURRENT          4   //  -  MPPT current limit [mA] e.g. 1000...60000mA 
                                      //     CAUTION: do not set above hardware-limit 30000/40000/50000/60000
#define MKSB_REG_BATT_UVP_CUT     5   // D03 Battery UnderVoltageProtection, <=limit cuts load [mV], e.g. 42400mV
#define MKSB_REG_BATT_UVP_CONN    6   //  ~  Battery UnderVoltageProtection, >=limit reconnects load [mV], typical: D03 + 200mV, e.g. 44400mV
#define MKSB_REG_COMM_ADDR        7   //  -  Communication Address [mAddress]
#define MKSB_REG_BATT_TYPE        8   // D04 Battery Type: value 0=SLA, 1=LiPo (2=LiLo, 3=LiFE, 4=LiTo) [mSelect], e.g. 1000=LiPo
#define MKSB_REG_BATT_CELLS       9   //  -  Battery System: 1...4 * 12V (Read-Only, set at Batt.connection) [m12V], e.g. 4000m12V
#define MKSB_REG_TOTAL            10  // ^^^ related local parameter at HMI

#define MKSB_COM_RX_GOOD            0
#define MKSB_COM_ERR_RX_TIMEOUT     1
#define MKSB_COM_ERR_RX_TOO_SHORT   2
#define MKSB_COM_ERR_RX_OVERFLOW    3
#define MKSB_COM_ERR_RX_CRC         4
#define MKSB_COM_ERR_RX_COMMAND     5
#define MKSB_COM_ERR_RX_LENGTH      6
#define MKSB_COM_RX_COUNTER         7

typedef struct mksb_data_t_
{
  TasmotaSerial *Serial;
  char *pRxBuffer;
  unsigned long time_window;            // for Rx silence detection
  float temperature;                    // temperature of the charge controller electronics
#ifdef _WITH_SERIAL_DEBUGGING
  unsigned long cntTx;                  // requests transmitted to the charge controller
  unsigned long cntRx[MKSB_COM_RX_COUNTER];
  uint16_t lastRxCntError;
#endif
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
  uint16_t regs_to_read;                // bit_n = flags register n to read
  uint16_t regs_to_write;               // bit_n = flags register n to write
  uint16_t regs_to_report;              // bit_n = flags register n to report (after valid response received)
  uint16_t regs_value[MKSB_REG_TOTAL];  // temp value storage all known configuration registers
#endif
  uint16_t energy_for_import;           // totalizer at charge controller (non-volatile there)
//  uint8_t mode;                         // status: mode flags
//  uint8_t error;                        // status: error flags
  uint16_t status;
  uint8_t rxCnt;                        // bytecounter at the Rx data buffer
  uint8_t timeout;                      // active after a request, waiting for reponse
#ifdef _WITH_ON_OFF_SUPPORT
  bool actual_state;                    // true = active, false = stop
  bool target_state;                    // true = active, false = stop
#endif
} mksb_data_t;

const char * mksb_register_names[] = {
  "0 unknown",                 //     dummy ? 
  "1 MPPT bulk [V]",           // D02 MPPT Voltage limit BULK, >= stops charging
  "2 MPPT float [V]",          // D01 MPPT voltage limit FLOAT, <= restarts charging, SLA battery only
  "3 Ontime load [h]",         // D00 Time duration the load gets connected, default: 24h 
  "4 MPPT current max.[A]",    //  -  MPPT current limit, default 30A, 40A, 50A or 60A
                               // CAUTION: do not set above the limit of your specific hardware
  "5 Batt.UVP cutload [V]",    // D03 Battery UnderVoltageProtection, <= cuts load
  "6 Batt.UVP connload [V]",   //  ~  Battery UnderVoltageProtection, >=limit reconnects load, typ D03 + 0.2V
  "7 Comm. address (?)",       //  -  Communication Address (ro)
  "8 Batt.Type [0=SLA, 1=Li]", // D04 Battery Type: value 0=SLA, 1=LiPo (ro)
  "9 Batt.System [n*12V]"      //  -  Battery System: 1...4 * 12V (Read-Only, set at Batt.connection)
};

//  const char MKSB_HTTP_SNS_sS_m_De[]       PROGMEM = "{s}%s " "{m}" " %d{e}";
//  const char MKSB_HTTP_SNS_str_m_str[]       PROGMEM = "{s}%s " "{m}" " %s{e}";
//  const char MKSB_HTTP_SNS_str_int_str[]        PROGMEM = "{s}%s " "%d{m}" "%s{e}";
  const char MKSB_HTTP_SNS_str_m_int[]          PROGMEM = "{s}%s " "{m}" "%d{e}";

/*
const char * mksb_status_bits[] = {
                                    // mode
  "",                               // b0 
  "",                               // b1
  D_CHARGE,                         // b2 MPPT Mode Active
  "",                               // b3

  "",                               // b4
  "",                               // b5
  "",                               // b6
  "",                               // b7

                                            // error
  D_BATTERY " " D_VOLTAGE " " D_STR_LOW,    // b0 Battery undervoltage (< register 5 / 6)
  D_BATTERY " " D_VOLTAGE " " D_STR_HIGH,   // b1 Battery overvoltage (> register 1)
  "",                                       // b2
  "",                                       // b3

  "",                                       // b4
  "",                                       // b5
  "",                                       // b6
  ""                                        // b7
};
*/

#ifdef _WITH_PHASE_NAME_SUPPORT
const char * mksb_channels[] = {D_DEVICE_INPUT , D_BATTERY};
#endif

#ifdef _WITH_SERIAL_DEBUGGING
const char * mksb_rx_counter_names[] = {
  "Good",
  "Timeout",
  "TooShort",
  "Overflow",
  "CRC",
  "Command",
  "Length"
};
#endif

// module data
mksb_data_t me = { .Serial = nullptr, .pRxBuffer = nullptr }; // singleton
//mksb_data_t * pMe = nullptr;



uint8_t MksbChecksum(uint8_t *data, uint8_t len)
{
  uint8_t checksum = 0;
  uint32_t i;

  len -= 1;
  for (i = 1; i < len; i++) { checksum += data[i]; }
  return checksum;
}


unsigned long MksbExtractUint(char *data, uint8_t offset_lsb, uint8_t offset_msb)
{
	unsigned long result = 0;

  for ( ; offset_msb >= offset_lsb; offset_msb-- ) {
    result = (result << 8) | (uint8_t)data[offset_msb];
  }
	return result;
}


#if 0 // notused
void MksbSetUint(unsigned long value, uint8_t *data, uint8_t offset_lsb, uint8_t offset_msb)
{
    for ( ; offset_lsb <= offset_msb; offset_lsb++ ) {
      data[offset_lsb] = value & 0xFF;
      value >>= 8;
    }
}
#endif


/********************************************************************************************/
/* SEND DATA */

bool MksbSend(uint8_t *data, uint8_t len)
{
  uint32_t i;

  if (me.timeout) {
//    AddLog( LOG_LEVEL_DEBUG, "NRG: Serial Busy, request pending");
    return false; // transaction busy, a request is already waiting for response
  }
  me.timeout = MKSB_TIMEOUT;

#ifdef _WITH_SERIAL_DEBUGGING
  { // serial debug at LOG_LEVEL_DEBUG_MORE: Request bytes transmitted
    char hex_char[(len * 3) + 2];
    me.cntTx++;
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Serial Tx %s"), ToHex_P(data, len, hex_char, sizeof(hex_char), ' '));
  }
#endif

  for ( i = 0; i < len; i++ ) {
    me.Serial->write(data[i]);
  }
  return true;
}

/********************************************************************************************/
/* COMMAND REQUESTS */

bool MksbRequestStatusData(void)
{
  uint8_t data[] = {  MKSB_START_FRAME, 
                      MKSB_CMD_READ_STATUS, 
                      0, 
                      0,
                      0,
                      0x55 }; // precalculated checksum
  return MksbSend(data, sizeof(data));
}

bool MksbRequestReadRegister(uint8_t reg)
{
  uint8_t data[] = {  MKSB_START_FRAME, 
                      MKSB_CMD_READ_CONFIG, 
                      reg,
                      0, 0,
                      0, 
                      0,
                      0,
                      255 }; // placeholder checksum
  data[sizeof(data)-1] = MksbChecksum(data, sizeof(data));
  return MksbSend(data, sizeof(data));
}

bool MksbRequestWriteRegister(uint8_t reg, uint16_t value)
{
  uint8_t data[] = {  MKSB_START_FRAME, 
                      MKSB_CMD_WRITE_CONFIG, 
                      reg,
                      (uint8_t)(value & 0xFF), (uint8_t)(value >> 8),
                      0, 
                      0,
                      0,
                      255 }; // placeholder checksum
  data[sizeof(data)-1] = MksbChecksum(data, sizeof(data));
  return MksbSend(data, sizeof(data));
}

bool MksbRequestPowerOn(void)
{
  uint8_t data[] = {  MKSB_START_FRAME, 
                      MKSB_CMD_POWER, 
                      2, 
                      0,
                      0,
                      255 }; // placeholder checksum
  data[sizeof(data)-1] = MksbChecksum(data, sizeof(data));
  return MksbSend(data, sizeof(data));
}

bool MksbRequestPowerOff(void)
{
  uint8_t data[] = {  MKSB_START_FRAME, 
                      MKSB_CMD_POWER, 
                      1, 
                      2,
                      0,
                      255 }; // placeholder checksum
  data[sizeof(data)-1] = MksbChecksum(data, sizeof(data));
  return MksbSend(data, sizeof(data));
}

/********************************************************************************************/
/* COMMAND RESPONSES */

void MksbParseStatusDataResponse(uint8_t len)
{
  uint16_t u16 = 0;
  
  if ( 20 != len ) { 
    MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_LENGTH);
    return; // invalid length, bad response
  }

  // Energy->power_on: not used, because ESP device acts as gateway to the charge controller
  
                                                                            // response data content:
  Energy->voltage[1] = (float)MksbExtractUint(me.pRxBuffer,  2, 3) / 10.0f; // battery voltage [0.1V]
  Energy->current[1] = (float)MksbExtractUint(me.pRxBuffer,  4, 5) / 10.0f; // battery current [0.1A]
  Energy->voltage[0] = (float)MksbExtractUint(me.pRxBuffer,  6, 7) / 10.0f; // solar voltage   [0.1V]
  Energy->active_power[0] = (float)MksbExtractUint(me.pRxBuffer,  8, 9);    // solar power     [1.0W]
  // calculate: battery power
  Energy->active_power[1] = Energy->voltage[1] * Energy->current[1]; 
  if ( Energy->voltage[0] >= 0.1f ) { // prevent division by 0
    // calculate: solar current
    Energy->current[0] = Energy->active_power[0] / Energy->voltage[0]; 
  } else {
    Energy->current[0] = 0.0f;
  }
  me.temperature = (float)MksbExtractUint(me.pRxBuffer, 10, 11) / 10.0f;    // temperature     [0.1degC]

#if _VARIANT_ENERGY_IMPORT == 1 /* import only once after restart: jitter only every restart */
  if( !me.energy_for_import ) {
    me.energy_for_import++;
    u16 = MksbExtractUint(me.pRxBuffer, 12, 13);                             // solar energy    [1.0kWh]
    if( u16 ) {
      me.energy_for_import = u16;
      Energy->import_active[0] = (float)u16;
      Energy->import_active[1] = 0.0f; // there is no energy generated from the battery
      EnergyUpdateTotal();
    }
  }
#elif _VARIANT_ENERGY_IMPORT == 2 /* import every full kWh integer resolution: jitter during runtime */
  u16 = MksbExtractUint(me.pRxBuffer, 12, 13);                              // solar energy    [1.0kWh]
  if( me.energy_for_import != u16 ) {
    me.energy_for_import = u16;
    Energy->import_active[0] = (float)u16;
    Energy->import_active[1] = 0.0f; // there is no energy generated from the battery
    EnergyUpdateTotal();
  }
#endif
    
  // done
  Energy->data_valid[0] = 0;
  Energy->data_valid[1] = 0;

  me.status = MksbExtractUint(me.pRxBuffer, 16, 17);  // mode flags [bits]
                                                      //   bit1=solar current limited (register 3)
                                                      //   bit2=MPPT Mode Active
                                                      //   more ?
                                                      // error flags [bits]
                                                      //   bit0=Batt.undervoltage (< register 5 / 6)
                                                      //   bit1=Batt.overvoltage (> register 1)
                                                      //   ?more
  // unused response data:
  // MksbExtractUint(me.pRxBuffer, 14, 15); // ?dummy [14:15]
  // MksbExtractUint(me.pRxBuffer, 18, 18); // ?dummy [18]
    MKSB_COM_RX_IncrementCnt(MKSB_COM_RX_GOOD);
}

#ifdef _WITH_CONFIG_REGISTER_SUPPORT
void MksbParseRegisterResponse(uint8_t len)
{
  uint8_t reg;

  if ( 9 != len ) { 
    MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_LENGTH);
    return; // invalid length, bad response
  }

  reg = me.pRxBuffer[2];
  if ( reg < MKSB_REG_TOTAL ) {
    me.regs_value[reg] = MksbExtractUint(me.pRxBuffer, 3, 4);
    me.regs_to_report |= 1 << reg;
    MKSB_COM_RX_IncrementCnt(MKSB_COM_RX_GOOD);
  }
}
#endif

#ifdef _WITH_ON_OFF_SUPPORT
void MksbParsePowerResponse(uint8_t len)
{
  if ( 6 != len ) {
    MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_LENGTH);
    return; // invalid length, bad response
  }

  if ( me.pRxBuffer[2] == 0 ) { /* ON */
    me.actual_state = true;
    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging enabled"));
    MKSB_COM_RX_IncrementCnt(MKSB_COM_RX_GOOD);
  } else 
  if ( me.pRxBuffer[2] == 1 ) {  /* OFF */
    me.actual_state = false;
    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging disabled"));
    MKSB_COM_RX_IncrementCnt(MKSB_COM_RX_GOOD);
  } else { /* unknown content */
  }
}
#endif

/********************************************************************************************/
/* RECEIVE DATA */

void MksbSerialInput(void)
{
  while ( me.Serial->available() ) {
    yield();
    if ( me.rxCnt < MKSB_BUFFER_SIZE) {  // prevent buffer overrun
      me.pRxBuffer[me.rxCnt] = me.Serial->read();
      if ( me.pRxBuffer[0] == MKSB_START_FRAME ) { // preamble required for a valid frame
        me.rxCnt++;
      } else {
        me.rxCnt = 0;
      }
    } else {
      (void)me.Serial->read(); // overrun: drop byte
    }
    me.time_window = millis();
  }

  // Ignore until non received after 6 chars (= 10 bits/char) time
//  if ( (millis() - me.time_window) > ((60000 / MKSB_BAUDRATE) +1) ) {
  if ( ((millis() - me.time_window) > 100) && me.rxCnt ) {
#ifdef _WITH_SERIAL_DEBUGGING
      { // serial debug at LOG_LEVEL_DEBUG_MORE: Request bytes transmitted
        char hex_char[(me.rxCnt * 3) + 2];
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Serial Rx %s"), ToHex_P( (uint8_t *)me.pRxBuffer, me.rxCnt, hex_char, sizeof(hex_char), ' '));
      }
#endif
    if ( me.rxCnt >= 6) { // shortest response
      if (MKSB_BUFFER_SIZE == me.rxCnt) {
        MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_OVERFLOW);
      } else 
      if (MksbChecksum((uint8_t *)me.pRxBuffer, me.rxCnt) != me.pRxBuffer[me.rxCnt -1]) {
        MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_CRC);
      } else { // valid checksum
        if (MKSB_RSP_READ_STATUS == me.pRxBuffer[1]) { MksbParseStatusDataResponse(me.rxCnt); } else
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
        if (MKSB_RSP_RW_CONFIG == me.pRxBuffer[1]) { MksbParseRegisterResponse(me.rxCnt); } else
#endif
#ifdef _WITH_ON_OFF_SUPPORT
        if (MKSB_RSP_POWER == me.pRxBuffer[1]) { MksbParsePowerResponse(me.rxCnt); } else
#endif
          MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_COMMAND);
        me.timeout = 0;
      }
    } else {
      MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_TOO_SHORT);
    }
    me.rxCnt = 0;
    me.Serial->flush();
  }
}

/********************************************************************************************/

void MksbEverySecond(void)
{
  int i;

  if (Energy->data_valid[0] > ENERGY_WATCHDOG) {
    Energy->voltage[0] = Energy->current[0] = Energy->active_power[0] = 0.0f;
    Energy->voltage[1] = Energy->current[1] = Energy->active_power[1] = 0.0f;
  } else {
    Energy->kWhtoday_delta[0] += Energy->active_power[0] * 1000 / 36; // solar energy only
    EnergyUpdateToday();
  }

  if (me.timeout) { // busy, waiting for answer
    me.timeout--;
    if( me.timeout == 0 ) {
      MKSB_COM_RX_IncrementCnt(MKSB_COM_ERR_RX_TIMEOUT);
    }
  } else { // available
#ifdef _WITH_SERIAL_DEBUGGING
    if ( me.lastRxCntError ) { // new error
      if ( me.cntRx[me.lastRxCntError] ) { // error available
        AddLog(LOG_LEVEL_ERROR, PSTR("NRG: Serial Tx %u, Rx-good %u, Rx-error-%s %u"), 
          me.cntTx,
          me.cntRx[MKSB_COM_RX_GOOD], 
          mksb_rx_counter_names[me.lastRxCntError], me.cntRx[me.lastRxCntError] );
      }
      me.lastRxCntError = 0;
    } else
    if ( 0 == (me.cntTx % (10 * 60) ) ) { // every 10 minutes: Info
      AddLog(LOG_LEVEL_INFO, PSTR("NRG: Serial Tx %u, Rx-good %u"), 
        me.cntTx, 
        me.cntRx[MKSB_COM_RX_GOOD] );
      for( i = 1; i < MKSB_COM_RX_COUNTER; i++ ) { // check all errors
        if( me.cntRx[i] ) { // reports only if there are errors 
          AddLog(LOG_LEVEL_ERROR, PSTR("NRG: Serial Rx-error-%s %u"), 
            mksb_rx_counter_names[i], me.cntRx[i]);
        }
      }
    } else {}
#endif
    // request new measurements data
    MksbRequestStatusData();
  }
}


void MksbEvery250ms(void)
{ 
  int i;
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
  float fVal;

  if ( me.regs_to_read || me.regs_to_write ) {
    for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
      if ( me.regs_to_write & (1 << i) ) { // prio write
        if ( MksbRequestWriteRegister( i, me.regs_value[i] ) == true ) {
          me.regs_to_write &= ~(1 << i);
          break; // only one per call
        }
      } else 
      if ( me.regs_to_read & (1 << i) ) { 
        if ( MksbRequestReadRegister( i ) == true ) {
          me.regs_to_read &= ~(1 << i);
          break; // only one per call
        }
      } else {
      }
    }
  } else 
#endif
#ifdef _WITH_ON_OFF_SUPPORT
  if ( me.actual_state != me.target_state ) {
    if (me.timeout) { // waiting for answer  
      return;
    }
    if ( me.target_state == false ) { // standby
      MksbRequestPowerOff();
    } else { // avtivate
      MksbRequestPowerOn();
    }
  } else 
#endif
  {}
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
  if ( me.regs_to_report ) {
    for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
      if ( me.regs_to_report & (1 << i) ) {
        fVal = ((float)(me.regs_value[i])) / 1000.0f;
        // answer requested manually by user
        AddLog( LOG_LEVEL_NONE, PSTR("NRG: Register%s = %3_f"), mksb_register_names[i], &fVal);
        me.regs_to_report &= ~(1 << i);
        break; // once per call
      }
    }
  }
#endif
}


bool MksbCommand(void)
{
  bool serviced = true;
  uint8_t reg;
  char *str;
  int32_t value;

  if ((CMND_POWERCAL == Energy->command_code) || (CMND_VOLTAGECAL == Energy->command_code) || (CMND_CURRENTCAL == Energy->command_code)) {
    // Service in xdrv_03_energy.ino
  } else 
  if (CMND_ENERGYCONFIG == Energy->command_code) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Config index %d, payload %d, data '%s'"),
      XdrvMailbox.index, XdrvMailbox.payload, XdrvMailbox.data ? XdrvMailbox.data : "null" );

    if ( XdrvMailbox.data_len == 0 ) { // no data: Read all registers and report to log
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
      me.regs_to_read = (1 << MKSB_REG_TOTAL) - 1; // flag all registers to be read
#endif
    } else {
      str = XdrvMailbox.data;
#ifdef _WITH_ON_OFF_SUPPORT
      if ('+' == str[0] ) { // + to set controller active
        me.target_state = true;
      } else
      if ('-' == str[0] ) { // - to set controller stop
        me.target_state = false;
      } else
#endif
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
      {
        reg = (uint8_t)strtoul( str, &str, 10 );
        if ( MKSB_REG_TOTAL > reg ) {
          while ((*str != '\0') && isspace(*str)) { str++; }  // Trim spaces
          if ( *str ) {
            value = (int32_t)(CharToFloat(str) * 1000.0f);
            // write Register: no range check here, all on your own risk
            if ( MKSB_REG_VOLTAGE_BULK <= reg && MKSB_REG_BATT_UVP_CONN >= reg ) {
              me.regs_value[reg] = value;   // store to prepare write
              me.regs_to_write |= 1 << reg; // trigger write
            } // else read only
          } else { // read one register
              me.regs_to_read |= 1 << reg; 
          }
        } // invalid register
      }
#else
      {}
#endif
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}


void MksbShow(uint32_t function) {
  float fVal;

  if ( Settings->flag.temperature_conversion ) {
    fVal = ConvertTempToFahrenheit( me.temperature );
  } else {
    fVal = me.temperature; // celsius
  }
 
  if ( FUNC_JSON_APPEND == function ) {
    /* Temperature */
    ResponseAppend_P(JSON_SNS_F_TEMP, "MkSkyBlu", Settings->flag2.temperature_resolution, &fVal);
    if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
      DomoticzFloatSensor(DZ_TEMP, fVal);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      KnxSensor(KNX_TEMPERATURE, fVal);
#endif // USE_KNX
    }
  }
#ifdef USE_WEBSERVER
  else if ( FUNC_WEB_SENSOR == function ) {
    WSContentSend_Temp("", fVal);
    WSContentSend_P(MKSB_HTTP_SNS_str_m_int, D_INFO, me.status );
//    WSContentSend_P( MKSB_HTTP_SNS_sS_m_Se , D_POWERUSAGE ,me.actual_state == true ? D_ENABLED: D_DISABLED );
  } else if ( FUNC_WEB_COL_SENSOR == function ) {
    WSContentSend_P( "MakeSkyBlue" ); // headline before values
  } else {}
#endif  // USE_WEBSERVER
}


void MksbSnsInit(void)
{
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  me.Serial = new TasmotaSerial(Pin(GPIO_MKSKYBLU_RX), Pin(GPIO_MKSKYBLU_TX), 1);
  if (me.Serial->begin(MKSB_BAUDRATE)) {
    if (me.Serial->hardwareSerial()) {
      ClaimSerial();
      me.pRxBuffer = TasmotaGlobal.serial_in_buffer;  // Use idle serial buffer to save RAM
    } else {
      me.pRxBuffer = (char*)(malloc(MKSB_BUFFER_SIZE));
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: MkSkyBlu Serial UART%d"), me.Serial->getUart());
#endif
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}


void MksbDrvInit(void)
{
  if (PinUsed(GPIO_MKSKYBLU_RX) && PinUsed(GPIO_MKSKYBLU_TX)) {
    Energy->phase_count = 2; // phases as channels: 0=solar input, 1=battery
#ifdef _WITH_PHASE_NAME_SUPPORT
    Energy->phase_names = mksb_channels;
#endif
    Energy->voltage_common = false;
    Energy->frequency_common = true;
    Energy->type_dc = true;
    Energy->use_overtemp = false;     // ESP device acts as separated gateway, charge controller has its own temperature management
    Energy->voltage_available = true; // both direct
    Energy->current_available = true; // solar indirect, battery direct 
    // Energy->local_energy_active_export = ?;

    me.timeout = 2;                   // Initial wait
#ifdef _WITH_SERIAL_DEBUGGING
    me.cntRx[MKSB_COM_ERR_RX_TIMEOUT] = 0xFFFFFFFF; // occurence at startup results to 0
#endif
    me.time_window = 0;
    me.temperature = NAN;
    me.energy_for_import = 0;
#ifdef _WITH_CONFIG_REGISTER_SUPPORT
    me.regs_to_read = 0;
    me.regs_to_write = 0;
    me.regs_to_report = 0;
#endif
#ifdef _WITH_ON_OFF_SUPPORT
    me.actual_state = true; // default
    me.target_state = true; // default
#endif
    TasmotaGlobal.energy_driver = XNRG_25;
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg25(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (me.Serial) { MksbSerialInput(); }
      break;
    case FUNC_EVERY_250_MSECOND:
      if (me.Serial) { MksbEvery250ms(); }
      break;

    case FUNC_ENERGY_EVERY_SECOND:
      if (me.Serial) { MksbEverySecond(); }
      break;

    case FUNC_JSON_APPEND:
    case FUNC_WEB_SENSOR:
    case FUNC_WEB_COL_SENSOR:
      MksbShow(function);
      break;
    case FUNC_ENERGY_RESET:
      me.temperature = NAN;
      break;
    case FUNC_COMMAND:
      result = MksbCommand();
      break;
    case FUNC_INIT:
      MksbSnsInit();
      break;
    case FUNC_PRE_INIT:
      MksbDrvInit();
      break;
  }
  return result;
}

#endif  // USE_MAKE_SKY_BLUE
#endif  // USE_ENERGY_SENSOR
