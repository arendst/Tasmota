/*
  xnrg_26_mk_sky_blu.ino - MakeSkyBlue Solar charger support for Tasmota

  Copyright (C) 2025  meMorizE

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
 * Tested with Model S3-60A
 * https://makeskyblue.com/en-de/products/60a-mppt-solar-charge-controller-w-wifi
 *
 * The charge controller should have a Mini-USB socket at bottom right beside the screw terminals.
 * It has a Vcc supply output of +5V and GND but the D+ and D- signals are NOT according USB standard !
 * They are TTL serial signals with 9600bps 8N1, D+=Tx_Out D-=Rx_In
 * When using a DIY ESP hardware be aware of the 5V TTL levels by using e.g. level shifter or isolator
 * 
 * The orignal V119 Wifi Box hardware includes an ESP8285 with 1MB Flash (as module 2AL3B ESP-M)
 * This specific hardware uses
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
 * This implementation is based on information from
 * https://github.com/lasitha-sparrow/Makeskyblue_wifi_Controller
 * https://www.genetrysolar.com/community/forum-134/topic-1219/
 * 
 * and logic analyzer recording of the original Wifi-Box firmware together with the Android app:
 * Status:       - request periodical every 1s
 *               - response typical within 30ms
 * Measurements: - request periodical every 1s, about 380ms after Status request 
 *               - response varies within 30...370ms
 * ReadRegister: - requested on demand with an interval of min. 170ms, valid registers 1...9
 *               - response typical within 30ms
\*********************************************************************************************/

#include <TasmotaSerial.h>

#define XNRG_26                     26

/* available compile options */
/* read and write access to the configuration registers */
#define MKSB_WITH_REGISTER_SUPPORT
/* allow to switch charging off and on again */
#define MKSB_WITH_ON_OFF_SUPPORT
/* Serial debugging provides counters for error detection and statistics 
Practical use: a real device shows frequent timeouts and / or CRC errors, wenn the solar power is more than 350W. 
*/
#define MKSB_WITH_SERIAL_DEBUGGING

/* future compile options */
/* requires new element Energy->phase_names at xdrv_03_energy and xdrv_03_esp32_energy */
//#define MKSB_WITH_CHANNEL_NAME_SUPPORT 


#define MKSB_BAUDRATE               9600

// TxRx first byte of every valid frame
#define MKSB_START_FRAME            0xAA
// TX second byte: Request to the charge controller
#define MKSB_CMD_READ_MEASUREMENTS  0x55 // response: 0xBB
#define MKSB_CMD_READ_REGISTER      0xCB // response: 0xDA
#define MKSB_CMD_WRITE_REGISTER     0xCA // response: 0xDA
#define MKSB_CMD_AUXILARY           0xCC // response: 0xDC or 0xDD
// Rx second byte: Response from the charge controller
#define MKSB_RSP_READ_MEASURES      0xBB // request: 0xAA
#define MKSB_RSP_RW_CONFIG          0xDA // request: 0xCB or 0xCA
#define MKSB_RSP_CLR_WIFI_PASSWORD  0xDC // D05 Clear Wifi-Password (not implemented)
#define MKSB_RSP_POWER              0xDD // request 0xCC

// TxRx third byte: config registers (request 0xCB or 0xCA, response 0xDA)
#define MKSB_REG_FIRST              1
#define MKSB_REG_VOLTAGE_BULK       1    // D02 MPPT Voltage limit BULK, >= stops charging [mV], e.g. 55000mV
#define MKSB_REG_VOLTAGE_FLOAT      2    // D01 MPPT voltage limit FLOAT, <= restarts charging [mV], SLA battery only
#define MKSB_REG_OUT_TIMER          3    // D00 Time duration the load gets connected [mh], default: 24h = 24000mh
#define MKSB_REG_CURRENT            4    //  -  MPPT current limit [mA] e.g. 1000...60000mA, CAUTION: respect limit of hardware
#define MKSB_REG_BATT_UVP_CUT       5    // D03 Battery UnderVoltageProtection, <=limit cuts load [mV], e.g. 42400mV
#define MKSB_REG_BATT_UVP_CONN      6    //  ~  Battery UnderVoltageProtection, >=limit reconnects load [mV], typical: D03 + 200mV, e.g. 44400mV
#define MKSB_REG_COMM_ADDR          7    //  -  Communication Address [mAddress]
#define MKSB_REG_BATT_TYPE          8    // D04 Battery Type: value 0=SLA, 1=LiPo (2=LiLo, 3=LiFE, 4=LiTo) [mSelect], e.g. 1000=LiPo
#define MKSB_REG_BATT_CELLS         9    //  -  Battery System: 1...4 * 12V (Read-Only, set at Batt.connection) [m12V], e.g. 4000m12V
#define MKSB_REG_LAST               9    // ^^^ related local parameter at display UI
#define MKSB_REG_TOTAL              (1+(MKSB_REG_LAST-MKSB_REG_FIRST))


#define MKSB_RX_BUFFER_SIZE         24 // bytes, 20 minimum
#define MKSB_TX_BUFFER_SIZE         8  // bytes,  7 minimum

// module type definition
typedef struct MKSB_MODULE_T_
{
  TasmotaSerial *Serial = nullptr;
  char *pRxBuffer = nullptr;
//  char *pTxBuffer = nullptr;
  uint8_t txBuffer[MKSB_TX_BUFFER_SIZE];
  float temperature;                    // temperature of the charge controller electronics
  uint16_t energy_total;                // totalizer at charge controller (non-volatile there)
//  uint8_t mode;                       // MPPT mode
//  uint8_t error;                      // status / error flags
  uint16_t status;                      // combines mode (LSB) and status / error (MSB)
  uint8_t rxIdx;                        // bytecounter at the Rx data buffer
  uint8_t rxChecksum;                   // for checksum calculation at reception
  uint8_t ev250ms_state;                // for scheduled serial requesting, snyced with everysecond
#ifdef MKSB_WITH_SERIAL_DEBUGGING
  uint32_t cntTx;                       // count requests transmitted to the charge controller
  uint32_t cntRxGood;                   // count valid responses received from the charge controller
  uint32_t cntTxRxLost;                 // count invalid / unresponsed requests
  uint32_t cntRxBadCRC;                 // count CRC-invalid responses
  uint32_t tsTx;                        // timestamp in ms of last byte transmitted
  uint32_t tsRx;                        // timestamp in ms of last byte received
#endif
#ifdef MKSB_WITH_REGISTER_SUPPORT
  uint16_t regs_to_read;                // bit_n = flags register n to read
  uint16_t regs_to_write;               // bit_n = flags register n to write
  uint16_t regs_to_report;              // bit_n = flags register n to report (after valid response received)
  uint16_t regs_value[MKSB_REG_TOTAL];  // value storage of all known configuration registers
#endif
#ifdef MKSB_WITH_ON_OFF_SUPPORT
//  uint8_t disable_charging;             // bit0=target, bit1=actual : 1=disable charging,0=enable charging (default)
  bool actual_state;                    // true = active, false = stop
  bool target_state;                    // true = active, false = stop
#endif
} MKSB_MODULE_T;


// module const data


/* TODO textual info instead of magic number ?
#define MKSB_STATUS_MPPT_IDLE           0
#define MKSB_STATUS_MPPT_OCP_OUTPUT     2
#define MKSB_STATUS_MPPT_OVP_OUTPUT     3
#define MKSB_STATUS_MPPT_CHARGING       4
#define MKSB_STATUS_MPPT_FULL           6

#define MKSB_STATUS_BATT_UVP            1
#define MKSB_STATUS_BATT_OVP            2

static const char mksb_status_0[] PROGMEM = "Idle";                   // 3.0 Night Mode (PV < XYZ V)
static const char mksb_status_2[] PROGMEM = "Overcurrent Protection"; // ??? E73
static const char mksb_status_3[] PROGMEM = "MPPT Bulk Voltage Limit reached";
static const char mksb_status_4[] PROGMEM = "Charging";               // 4.0 MPPT Mode
static const char mksb_status_6[] PROGMEM = "Battery full";           // ???
//
static const char mksb_status_256[] PROGMEM = "Battery Undervoltage Protection"; // load cut, Fault E65
static const char mksb_status_512[] PROGMEM = "Battery Overvoltage";  // load still connected !, Fault E63
static const char mksb_status_U[] PROGMEM = "Unknown=%u";
typedef struct MKSB_STATUS_T_
{
  uint16_t u16;
  const char * fString;
} MKSB_STATUS_T;
static const MKSB_STATUS_T mksb_status[] PROGMEM = {
  { 0, mksb_status_0 },
  { 2, mksb_status_2 },
  { 3, mksb_status_3 },
  { 4, mksb_status_4 }
}
*/
#ifdef MKSB_WITH_REGISTER_SUPPORT
// config register format strings: requires one float as register value
static const char mksb_fsrreg1[] PROGMEM = "NRG: R1 = %1_fV MPPT Bulk Charging Voltage";
static const char mksb_fsrreg2[] PROGMEM = "NRG: R2 = %1_fV MPPT Floating Charging Voltage";
static const char mksb_fsrreg3[] PROGMEM = "NRG: R3 = %0_fh Load-Output duration";
static const char mksb_fsrreg4[] PROGMEM = "NRG: R4 = %1_fA MPPT Charging Current Limit";
static const char mksb_fsrreg5[] PROGMEM = "NRG: R5 = %1_fV Battery Undervoltage Protection";
static const char mksb_fsrreg6[] PROGMEM = "NRG: R6 = %1_fV Battery Undervoltage Recovery";
static const char mksb_fsrreg7[] PROGMEM = "NRG: R7 = %0_f Com Address";
static const char mksb_fsrreg8[] PROGMEM = "NRG: R8 = %0_f Battery Type [0=SLA, 1=Li]";
static const char mksb_fsrreg9[] PROGMEM = "NRG: R9 = %0_f Battery System [1...4 * 12V]";
static const char * mksb_register_fstrings[] PROGMEM = { mksb_fsrreg1, mksb_fsrreg2, mksb_fsrreg3, 
                                                         mksb_fsrreg4, mksb_fsrreg5, mksb_fsrreg6, 
                                                         mksb_fsrreg7, mksb_fsrreg8, mksb_fsrreg9 }; 
#endif

//static const char MKSB_HTTP_SNS_str_m_int[] PROGMEM = "{s}%s " "{m}" "%d{e}";
//static const char MKSB_HTTP_SNS_s_str_str[] PROGMEM = "{s}" "%s{m}" "%s{e}";

#ifdef MKSB_WITH_CHANNEL_NAME_SUPPORT
static const char * mksb_channels_name[] PROGMEM = { PSTR(D_DEVICE_INPUT) , PSTR(D_BATTERY) };
#endif

// module ram data
static MKSB_MODULE_T *mksb = nullptr;


/********************************************************************************************/
/* EXTRACT UNSIGNED INT FROM SERIAL RECEIVED DATA */ 
uint32_t MkSkyBluExtractUint(const char *data, uint8_t offset_lsb, uint8_t offset_msb)
{
	uint32_t result = 0;

  for ( ; offset_msb >= offset_lsb; offset_msb-- ) {
    result = (result << 8) | (uint8_t)data[offset_msb];
  }
	return result;
}

/********************************************************************************************/
/* SEND SERIAL DATA */
void MkSkyBluSend(uint8_t len)
{
  uint32_t i;
  uint8_t crc;
  
#ifdef MKSB_WITH_SERIAL_DEBUGGING
  if ( mksb->rxIdx ) { // bytes received with timediff since last request
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Rx [%ums] %*_H"),
      mksb->tsRx - mksb->tsTx, 
      mksb->rxIdx, mksb->pRxBuffer);
  }
#endif

  // request frame
  mksb->Serial->write( MKSB_START_FRAME );
  crc = 0;
  for ( i = 0; i < len; i++ ) { // variable data
    mksb->Serial->write(mksb->txBuffer[i]);
    crc += mksb->txBuffer[i];
  }
  mksb->Serial->write(crc); // checksum

  mksb->rxIdx = 0; // reset receiver state
  mksb->Serial->flush(); // ensure transmission complete

#ifdef MKSB_WITH_SERIAL_DEBUGGING
  mksb->tsTx = millis();
  mksb->cntTx++;
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Tx %02X%*_H%02X"), 
    MKSB_START_FRAME, len, mksb->txBuffer, crc );
#endif
}

/********************************************************************************************/
/* BUILD SERIAL DATA REQUESTS FOR TRANSMISSION */
void MkSkyBluRequestStatus(void)
{
  static const uint8_t mksb_ser_req_status[] PROGMEM = { MKSB_CMD_AUXILARY, 0, 2, 0 };
  
  memcpy_P(mksb->txBuffer, mksb_ser_req_status, sizeof (mksb_ser_req_status));
  MkSkyBluSend(sizeof (mksb_ser_req_status));
}

void MkSkyBluRequestMeasurements(void)
{
  static const uint8_t mksb_ser_req_measures[]  PROGMEM = { MKSB_CMD_READ_MEASUREMENTS, 0,0,0 };
  
  memcpy_P(mksb->txBuffer, mksb_ser_req_measures, sizeof (mksb_ser_req_measures));
  MkSkyBluSend(sizeof (mksb_ser_req_measures));
}

#ifdef MKSB_WITH_REGISTER_SUPPORT
void MkSkyBluRequestReadRegister(uint8_t reg)
{
  static const uint8_t mksb_ser_req_read_reg[]  PROGMEM = { MKSB_CMD_READ_REGISTER,  0 ,0,0, 0,0,0 };
  
  memcpy_P(mksb->txBuffer, mksb_ser_req_read_reg, sizeof (mksb_ser_req_read_reg));
  // variable register
  mksb->txBuffer[1] = reg;
  MkSkyBluSend(sizeof (mksb_ser_req_read_reg));
}

void MkSkyBluRequestWriteRegister(uint8_t reg, uint16_t value)
{
  static const uint8_t mksb_ser_req_write_reg[] PROGMEM = { MKSB_CMD_WRITE_REGISTER, 0 ,0,0, 0,0,0 };

  memcpy_P(mksb->txBuffer, mksb_ser_req_write_reg, sizeof (mksb_ser_req_write_reg));
  // variable register and value
  mksb->txBuffer[1] = reg;
  mksb->txBuffer[2] = (uint8_t)(value & 0xFF);
  mksb->txBuffer[3] = (uint8_t)(value >> 8);
  MkSkyBluSend(sizeof (mksb_ser_req_write_reg));
}
#endif

void MkSkyBluRequestChargingOn(void)
{
  static const uint8_t mksb_ser_req_chrg_on[]   PROGMEM = { MKSB_CMD_AUXILARY, 2, 0, 0 };

  memcpy_P(mksb->txBuffer, mksb_ser_req_chrg_on, sizeof (mksb_ser_req_chrg_on));
  MkSkyBluSend(sizeof (mksb_ser_req_chrg_on));
}

void MkSkyBluRequestChargingOff(void)
{
  static const uint8_t mksb_ser_req_chrg_off[]  PROGMEM = { MKSB_CMD_AUXILARY, 1, 2, 0 };

  memcpy_P(mksb->txBuffer, mksb_ser_req_chrg_off, sizeof (mksb_ser_req_chrg_off));
  MkSkyBluSend(sizeof (mksb_ser_req_chrg_off));
}

/********************************************************************************************/
/* PARSE SERIAL DATA RECEIVED */
void MkSkyBluParseMeasurementsResponse(void)
{
  // Energy->power_on: not used, because ESP device acts as gateway to the charge controller
  Energy->voltage[1] = (float)MkSkyBluExtractUint(mksb->pRxBuffer,  2, 3) / 10.0f; // battery voltage [0.1V]
  Energy->current[1] = (float)MkSkyBluExtractUint(mksb->pRxBuffer,  4, 5) / 10.0f; // battery current [0.1A]
  Energy->voltage[0] = (float)MkSkyBluExtractUint(mksb->pRxBuffer,  6, 7) / 10.0f; // solar voltage   [0.1V]
  Energy->active_power[0] = (float)MkSkyBluExtractUint(mksb->pRxBuffer,  8, 9);    // solar power     [1.0W]
  // calculate: battery power
  Energy->active_power[1] = Energy->voltage[1] * Energy->current[1]; 
  if ( Energy->voltage[0] >= 0.1f ) { // prevent division by 0
    // calculate: solar current
    Energy->current[0] = Energy->active_power[0] / Energy->voltage[0]; 
  } else {
    Energy->current[0] = 0.0f;
  }
  // all sensor data for energy calculation are refreshed
  Energy->data_valid[0] = 0;
  Energy->data_valid[1] = 0;

  mksb->energy_total = MkSkyBluExtractUint(mksb->pRxBuffer, 12, 13); // solar energy total [1.0kWh]

  // temperature of the charge controller
  mksb->temperature = (float)MkSkyBluExtractUint(mksb->pRxBuffer, 10, 11) / 10.0f; // temperature [0.1degC]
 
  mksb->status = MkSkyBluExtractUint(mksb->pRxBuffer, 16, 17); // mode and status

  // unused response data:
  // MkSkyBluExtractUint(mksb->pRxBuffer, 14, 15); // ?dummy [14:15]
  // MkSkyBluExtractUint(mksb->pRxBuffer, 18, 18); // ?dummy [18]
}

#ifdef MKSB_WITH_REGISTER_SUPPORT
void MkSkyBluParseRegisterResponse(void)
{
  uint8_t reg;

  reg = mksb->pRxBuffer[2] - MKSB_REG_FIRST;
  if ( reg < MKSB_REG_TOTAL ) {
    mksb->regs_value[reg] = MkSkyBluExtractUint(mksb->pRxBuffer, 3, 4);
    mksb->regs_to_report |= 1 << reg;
  }
}
#endif

#ifdef MKSB_WITH_ON_OFF_SUPPORT
void MkSkyBluParsePowerResponse(void)
{
  if ( mksb->pRxBuffer[2] == 0 ) { /* ON */
    if ( mksb->actual_state != true ) {
      AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging ON"));
      mksb->actual_state = true;
    }
  } else 
  if ( mksb->pRxBuffer[2] == 1 ) {  /* OFF */
    if ( mksb->actual_state != false ) {
      AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging OFF"));
      mksb->actual_state = false;
    }
  } else { /* unknown content */
  }
}
#endif

/********************************************************************************************/
/* RECEIVE SERIAL DATA */
void MkSkyBluSerialInput(void)
{
  while ( mksb->Serial->available() ) {
    yield();
    if (mksb->rxIdx < MKSB_RX_BUFFER_SIZE) { // buffer available
      mksb->pRxBuffer[mksb->rxIdx] = mksb->Serial->read();
      mksb->tsRx = millis();
      if (MKSB_START_FRAME != mksb->pRxBuffer[0] ) { // no start of frame yet
        mksb->rxIdx = 0; // reset receiver
      } else {                // [0] start valid
        if (0 == mksb->rxIdx) {  // start of frame present
          mksb->rxChecksum = 0;  // reset checksum calc
        } else {              // [1+] cmd or later
          if ((MKSB_RSP_READ_MEASURES == mksb->pRxBuffer[1]) && (19 == mksb->rxIdx)) {
            if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
              MkSkyBluParseMeasurementsResponse();
              mksb->cntRxGood++;
            } else {
              mksb->cntRxBadCRC++;
            }
          } else
#ifdef MKSB_WITH_REGISTER_SUPPORT
          if ((MKSB_RSP_RW_CONFIG == mksb->pRxBuffer[1]) && (8 == mksb->rxIdx)) {
            if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
              MkSkyBluParseRegisterResponse();
              mksb->cntRxGood++;
            } else {
              mksb->cntRxBadCRC++;
            }
          } else
#endif
          if ((MKSB_RSP_POWER == mksb->pRxBuffer[1]) && (5 == mksb->rxIdx)) {
            if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
              MkSkyBluParsePowerResponse();
              mksb->cntRxGood++;
            } else {
              mksb->cntRxBadCRC++;
            }
          } else { // calc checksum
            mksb->rxChecksum += mksb->pRxBuffer[mksb->rxIdx]; 
          }
        } 
      }
      mksb->rxIdx++;       // more to receive         
    } else { // buffer full
      (void)mksb->Serial->read(); // drop received byte
    }
  }
}

/********************************************************************************************/
/* EVERY SECOND */
void MkSkyBluEverySecond(void)
{
  float fValue;

  if (Energy->data_valid[0] > ENERGY_WATCHDOG) {
    Energy->voltage[0] = Energy->current[0] = Energy->active_power[0] = 0.0f;
    Energy->voltage[1] = Energy->current[1] = Energy->active_power[1] = 0.0f;
  } else {
    Energy->kWhtoday_delta[0] += Energy->active_power[0] * 1000 / 36; // solar energy only

    // import the non-resetable full kWh counter from the charge controller, but with 2 requirements:
    // 1. SetOption72 is active (bug@EnergyUpdateTotal?: a call of it impacts kWhtoday, even if option is off)
    // 2. the firmware counter is smaller than the counter of the charge controller
    if ( Settings->flag3.hardware_energy_total ) {
      fValue = (float)mksb->energy_total;
      if ( Energy->total[0] < fValue ) {
        Energy->import_active[0] = fValue;
        Energy->import_active[1] = 0.0f;
        EnergyUpdateTotal(); // this also calls EnergyUpdateToday() at the end
      } else {
        EnergyUpdateToday();
      }
    } else {
      EnergyUpdateToday();
    }
  }
  
  mksb->ev250ms_state = 0; // sync the 250ms state machine

#ifdef MKSB_WITH_SERIAL_DEBUGGING
  if ( !mksb->cntTx ) { // nothing sent yet 
    return;             // prevent multiple useless logs at startup
  }
  if ( mksb->cntTxRxLost < (mksb->cntTx - mksb->cntRxGood) ) { // new error: immediately
    mksb->cntTxRxLost = (mksb->cntTx - mksb->cntRxGood);
    if( !mksb->regs_to_read ) { // no config-read in progress
      AddLog(LOG_LEVEL_ERROR, PSTR("NRG: Serial errors:%u (CRC:%u)"), 
        mksb->cntTxRxLost,
        mksb->cntRxBadCRC );
    }
  }
  if ( 0 == (mksb->cntTx % (10 * 60) ) ) { // info: about every 10 minutes
    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Serial stats Tx:%u, Rx:%u, errors:%u (CRC:%u)"), 
      mksb->cntTx,
      mksb->cntRxGood,
      mksb->cntTxRxLost,
      mksb->cntRxBadCRC );
//    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charge Controller Total Energy %u kWh"),
//      mksb->energy_total );
  }
#endif
}

/********************************************************************************************/
/* EVERY 250 MS */
void MkSkyBluEvery250ms(void)
{ 
  int i;
  float fVal;

//  if ( mksb->ev250ms_state == 0 ) {
//    MkSkyBluRequestStatus();
//  } else
  if ( mksb->ev250ms_state == 0 ) {
    MkSkyBluRequestMeasurements();
  } else
  if ( mksb->ev250ms_state == 3 ) {
#ifdef MKSB_WITH_REGISTER_SUPPORT
    if ( mksb->regs_to_read || mksb->regs_to_write ) {
      for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
        if ( mksb->regs_to_write & (1 << i) ) { // prio write
          MkSkyBluRequestWriteRegister( i + MKSB_REG_FIRST, mksb->regs_value[i] );
          mksb->regs_to_write &= ~(1 << i);
          break; // only one per call
        } else 
        if ( mksb->regs_to_read & (1 << i) ) { 
          MkSkyBluRequestReadRegister( i + MKSB_REG_FIRST );
          mksb->regs_to_read &= ~(1 << i);
          break; // only one per call
        } else {
        }
      }
    } else 
#endif
#ifdef MKSB_WITH_ON_OFF_SUPPORT
    if ( mksb->actual_state != mksb->target_state ) {
      if ( mksb->target_state == false ) { 
        MkSkyBluRequestChargingOff();
      } else { 
        MkSkyBluRequestChargingOn();
      }
      mksb->actual_state = mksb->target_state;
    }
#else
    {}
#endif
  }
  
#ifdef MKSB_WITH_REGISTER_SUPPORT
  if ( mksb->regs_to_report ) {
    for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
      if ( mksb->regs_to_report & (1 << i) ) {
        fVal = ((float)(mksb->regs_value[i])) / 1000.0f;
        AddLog( LOG_LEVEL_NONE, mksb_register_fstrings[i], &fVal);
        mksb->regs_to_report &= ~(1 << i);
        break; // once per call
      }
    }
  }
#endif

  mksb->ev250ms_state++;
  if ( mksb->ev250ms_state >= 4 ) {
    mksb->ev250ms_state = 0;
  }
}

/********************************************************************************************/
/* ENERGY COMMAND */
bool MkSkyBluEnergyCommand(void)
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

    if ( XdrvMailbox.data_len == 0 ) { // no arguments: Read all registers and report to log
      mksb->regs_to_read = (1 << MKSB_REG_TOTAL) - 1; // flag all registers to be read
    } else {
      str = XdrvMailbox.data;
#ifdef MKSB_WITH_ON_OFF_SUPPORT
      if ('+' == str[0] ) {             // 1st argument: + to set controller charging active
        mksb->target_state = true;
      } else
      if ('-' == str[0] ) {             // 1st argument: - to set controller charging off
        mksb->target_state = false;
      } else
#endif
#ifdef MKSB_WITH_REGISTER_SUPPORT
      {
        reg = (uint8_t)strtoul( str, &str, 10 ) - MKSB_REG_FIRST;
        if ( MKSB_REG_FIRST <= reg && MKSB_REG_LAST >= reg ) { // 1st argument: 1...9 for register number
          while ((*str != '\0') && isspace(*str)) { str++; }   // Trim spaces
          if ( *str ) {                                        // there is a value: write registers value
            value = (int32_t)(CharToFloat(str) * 1000.0f);
            // write Register: no range check here
            mksb->regs_value[reg] = value;   // store value to prepared for write
            mksb->regs_to_write |= 1 << reg; // trigger write
          } else {                                             // there is no value: read registers value
              mksb->regs_to_read |= 1 << reg; 
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

/********************************************************************************************/
/* PUBLISH SENSORS (beyond Energy) */
void MkSkyBluShow(uint32_t function) {
  float fVal;

  if ( Settings->flag.temperature_conversion ) {
    fVal = ConvertTempToFahrenheit( mksb->temperature );
  } else {
    fVal = mksb->temperature; // celsius
  }
 
  if ( FUNC_JSON_APPEND == function ) {
    /* Temperature */
    ResponseAppend_P(JSON_SNS_F_TEMP, PSTR("MkSkyBlu"), Settings->flag2.temperature_resolution, &fVal);
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
    WSContentSend_P( PSTR("{s}%s {m}%d{e}") , D_INFO, mksb->status );
#ifdef MKSB_WITH_SERIAL_DEBUGGING
//    WSContentSend_P(MKSB_HTTP_SNS_str_m_int, D_SERIAL_OUT, mksb->cntTx );
//    WSContentSend_P(MKSB_HTTP_SNS_str_m_int, PSTR( D_COUNTER " " D_SERIAL_IN ), mksb->cntRxGood );
//    WSContentSend_P(MKSB_HTTP_SNS_str_m_int, PSTR( D_COUNTER " " D_FAILED ), mksb->cntTxRxLost );
    WSContentSend_P( PSTR("{s}" D_SERIAL_IN " / " D_FAILED "{m}" "%u / %u{e}"), 
      mksb->cntRxGood, mksb->cntTxRxLost );
//    WSContentSend_P(MKSB_HTTP_SNS_str_m_int, PSTR( D_CHECKSUM_FAILURE ), mksb->cntRxBadCRC );
#endif
//    WSContentSend_P( MKSB_HTTP_SNS_sS_m_Se , D_POWERUSAGE ,mksb->actual_state == true ? D_ENABLED: D_DISABLED );
  } else if ( FUNC_WEB_COL_SENSOR == function ) {
//    WSContentSend_P( PSTR("MakeSkyBlue") ); // headline before values
#ifdef MKSB_WITH_CHANNEL_NAME_SUPPORT
    WSContentSend_P( PSTR("MakeSkyBlue") ); // headline before values
#else
    WSContentSend_P( PSTR("MakeSkyBlue  %s %s"), D_SOLAR_POWER, D_BATTERY ); // headline before values
#endif
//    WSContentSend_P( MKSB_HTTP_SNS_hdl_str_str, D_SOLAR_POWER, D_BATTERY ); // headline before values
  } else {}
#endif  // USE_WEBSERVER
}

/********************************************************************************************/
/* SENSORS INIT */
void MkSkyBluSnsInit(void)
{
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  mksb->Serial = new TasmotaSerial(Pin(GPIO_MKSKYBLU_RX), Pin(GPIO_MKSKYBLU_TX), 1);
  if (mksb->Serial->begin(MKSB_BAUDRATE)) {
//    mksb->pTxBuffer = (char*)(malloc(MKSB_TX_BUFFER_SIZE));
    if (mksb->Serial->hardwareSerial()) {
      ClaimSerial();
      mksb->pRxBuffer = TasmotaGlobal.serial_in_buffer;  // Use idle serial buffer to save RAM
    } else {
      mksb->pRxBuffer = (char*)(malloc(MKSB_RX_BUFFER_SIZE));
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: MkSkyBlu Serial UART%d"), mksb->Serial->getUart());
#endif
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

/********************************************************************************************/
/* DRIVER INIT */
void MkSkyBluDrvInit(void)
{
  if (PinUsed(GPIO_MKSKYBLU_RX) && PinUsed(GPIO_MKSKYBLU_TX)) {
    if ( !mksb ) { // module memory not allocated: allocate
      mksb = (MKSB_MODULE_T *)(malloc(sizeof(MKSB_MODULE_T)));
    }
    if ( !mksb ) { // not enough memory available
      TasmotaGlobal.energy_driver = ENERGY_NONE;
      return;
    }

    // preset / fixed energy values
    Energy->phase_count = 2; // phases as channels: 0=solar input, 1=battery
    Energy->voltage_common = false;
    Energy->frequency_common = true;
    Energy->type_dc = true;
    Energy->use_overtemp = false;     // ESP device acts as separated gateway, charge controller has its own temperature management
    Energy->voltage_available = true; // both direct
    Energy->current_available = true; // solar indirect from power, battery direct
#ifdef MKSB_WITH_CHANNEL_NAME_SUPPORT
    Energy->channels_name = mksb_channels_name; // char * channels_name[]
#endif

    // preset / fixed module values
    mksb->temperature = NAN;
    mksb->energy_total = 0;
    mksb->regs_to_read = 0;
    mksb->regs_to_write = 0;
    mksb->regs_to_report = 0;
#ifdef MKSB_WITH_ON_OFF_SUPPORT
    mksb->actual_state = true; // default: charging enabled
    mksb->target_state = true; // default: charging enabled
#endif
    TasmotaGlobal.energy_driver = XNRG_26;
  } else { // driver not used
#if 0 // not needed, because every configuration change causes a restart
    if ( mksb ) { // but it was active
      if ( mksb->pRxBuffer != TasmotaGlobal.serial_in_buffer ) {
        free( mksb->pRxBuffer ); // free, if separated receive buffer
      }
      free( mksb ); // free module
    }
#endif
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg26(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (mksb->Serial) { MkSkyBluSerialInput(); }
      break;
    case FUNC_EVERY_250_MSECOND:
      if (mksb->Serial) { MkSkyBluEvery250ms(); }
      break;
    case FUNC_ENERGY_EVERY_SECOND:
      if (mksb->Serial) { MkSkyBluEverySecond(); }
      break;
    case FUNC_JSON_APPEND:
    case FUNC_WEB_SENSOR:
    case FUNC_WEB_COL_SENSOR:
      MkSkyBluShow(function);
      break;
    case FUNC_ENERGY_RESET:
      mksb->temperature = NAN;
      break;
    case FUNC_COMMAND:
      result = MkSkyBluEnergyCommand();
      break;
    case FUNC_INIT:
      MkSkyBluSnsInit();
      break;
    case FUNC_PRE_INIT:
      MkSkyBluDrvInit();
      break;
  }
  return result;
}

#endif  // USE_MAKE_SKY_BLUE
#endif  // USE_ENERGY_SENSOR
