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
 * Precondition to get communication working:
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
 *  {"NAME":"MakeSkyBlue Wifi-Adapter (ESP8285)","GPIO":[1,10528,1,10560,1,1,0,0,1,1,1,1,1,1],"FLAG":0,"BASE":18}
 *
 * Useful runtime options:
 *  command VoltRes 1 to select voltage resolution to 0.1 V (native resolution)
 *  command AmpRes 1 to select current resolution to 0.1 A (native resolution)
 *  SetOption72 to read total energy from the charge controller
???
 * EnergyCols <phases>`   - Change default 4 column GUI display to <phases> columns
 * SetOption129 1         - Display energy for each phase instead of single sum
 * SetOption150 1         - Display no common voltage/frequency
 ???
 * 
 * Note: SetOption129 should be off, because only the solar energy (=L1) is typical of interest
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
#define MKSB_WITH_REGISTER_SUPPORT  // read and write access to configuration registers
// >Use Console command 'EnergyConfig' to read and write configuration registers R1...R9
//  no parameters: = read all registers
//  one parameter: n = read specific register
//  two parameters: n value = write specific register
#define MKSB_WITH_ON_OFF_SUPPORT    // allow to switch charging off and on again
// >Use Console command 'EnergyConfig+' to switch charging on
// >Use Console command 'EnergyConfig-' to switch charging off
#define MKSB_WITH_SERIAL_DEBUGGING  // provide counters for error detection and statistics
// >Practical use: a real device shows frequent timeouts and / or CRC errors, wenn the solar power is more than 350W. 


#define MKSB_BAUDRATE               9600

// TxRx first byte of every valid frame
#define MKSB_START_FRAME            0xAA
// Tx second byte: Request to the charge controller
#define MKSB_CMD_READ_MEASUREMENTS  0x55 // response: 0xBB
#define MKSB_CMD_READ_REGISTER      0xCB // response: 0xDA
#define MKSB_CMD_WRITE_REGISTER     0xCA // response: 0xDA
#define MKSB_CMD_AUXILARY           0xCC // response: 0xDC or 0xDD
// Rx second byte: Response from the charge controller
#define MKSB_RSP_READ_MEASURES      0xBB // request: 0xAA
#define MKSB_RSP_RW_CONFIG          0xDA // request: 0xCB or 0xCA
#define MKSB_RSP_CLR_WIFI_PASSWORD  0xDC // D05 Clear Wifi-Password (not implemented)
#define MKSB_RSP_POWER              0xDD // request 0xCC
//
#define MKSB_RSP_SZ_READ_MEASURES  20 // bytes // minimum size of response frame
#define MKSB_RSP_SZ_RW_CONFIG       9 // bytes // size of response frame



// TxRx third byte: config registers (request 0xCB or 0xCA, response 0xDA)
#define MKSB_REG_FIRST              1    // vvv = related local parameter at display UI
#define MKSB_REG_VOLTAGE_BULK       1    // D02 MPPT Voltage limit BULK, >= stops charging [mV], e.g. 55000mV
#define MKSB_REG_VOLTAGE_FLOAT      2    // D01 MPPT voltage limit FLOAT, <= restarts charging [mV], SLA battery only
#define MKSB_REG_OUT_TIMER          3    // D00 Time duration the load gets connected [mh], default: 24h = 24000mh
#define MKSB_REG_CURRENT            4    //  -  MPPT current limit [mA] e.g. 1000...60000mA, CAUTION: respect limit of hardware
#define MKSB_REG_BATT_UVP_CUT       5    // D03 Battery UnderVoltageProtection, <=limit cuts load [mV], e.g. 42400mV
#define MKSB_REG_BATT_UVP_CONN      6    //  ~  Battery UnderVoltageProtection, >=limit reconnects load [mV], typical: D03 + 200mV, e.g. 44400mV
#define MKSB_REG_COMM_ADDR          7    //  -  Communication Address [mAddress]
#define MKSB_REG_BATT_TYPE          8    // D04 Battery Type: value 0=SLA, 1=LiPo (2=LiLo, 3=LiFE, 4=LiTo) [mSelect], e.g. 1000=LiPo
#define MKSB_REG_BATT_CELLS         9    //  -  Battery System: 1...4 * 12V (Read-Only, set at Batt.connection) [m12V], e.g. 4000m12V
#define MKSB_REG_LAST               9    // ^^^ = related local parameter at display UI
#define MKSB_REG_TOTAL              (1+(MKSB_REG_LAST-MKSB_REG_FIRST))


#define MKSB_RX_BUFFER_SIZE         24 // bytes, 20 minimum
#define MKSB_TX_BUFFER_SIZE         8  // bytes,  7 minimum

// module type definition
typedef struct MKSB_MODULE_T_
{
    uint8_t idx;                          // index of receiver interface
    uint8_t phase_id;                     // phase id
    uint8_t idx_tx;                       // index of transmitter interface
    TasmotaSerial *Serial = nullptr;
    char *pRxBuffer = nullptr;
//  char *pTxBuffer = nullptr;
    uint8_t txBuffer[MKSB_TX_BUFFER_SIZE];
    uint16_t energy_total;                // totalizer at charge controller (non-volatile there)
//  uint8_t mode;                       // MPPT mode
//  uint8_t error;                      // status / error flags
    uint32_t status;                      // combines mode (LSB) and status / error (MSB)
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


MKSB_MODULE_T *pMskbInstance = nullptr;
float *pMksbInstance_FloatArrays = nullptr;
#define MKSB_INSTANCE_FLOATARRAY_EFFICIENCY    0    // offset in MksbInstance_FloatArrays
#define MKSB_INSTANCE_FLOATARRAY_TEMPERATURE   1    // offset in MksbInstance_FloatArrays
#define MKSB_INSTANCE_FLOATARRAY_BATTVOLTAGE   2    // offset in MksbInstance_FloatArrays
#define MKSB_INSTANCE_FLOATARRAY_BATTCURRENT   3    // offset in MksbInstance_FloatArrays
#define MKSB_INSTANCE_FLOATARRAY_SERIAL_QOS    4    // offset in MksbInstance_FloatArrays
#define MKSB_INSTANCE_FLOATARRAY_SIZE          5    // number of different float arrays
// Note: each float array has Energy->phase_count elements
//static float *MksbInstance_Temperature = nullptr;
//static float *MksbInstance_BatteryVoltage = nullptr;
//static float *MksbInstance_BatteryCurrent = nullptr;

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
static const char mksb_fsrreg1[] PROGMEM = "NRG: MkSkyBlu%u R1 = %1_fV [D02] MPPT Bulk Charging Voltage";
static const char mksb_fsrreg2[] PROGMEM = "NRG: MkSkyBlu%u R2 = %1_fV [D01] MPPT Floating Charging Voltage (SLA only)";
static const char mksb_fsrreg3[] PROGMEM = "NRG: MkSkyBlu%u R3 = %0_fh [D00] Load-Output ON duration";
static const char mksb_fsrreg4[] PROGMEM = "NRG: MkSkyBlu%u R4 = %1_fA MPPT Current Limit (CAUTION: change on your own risk!)";
static const char mksb_fsrreg5[] PROGMEM = "NRG: MkSkyBlu%u R5 = %1_fV [D03] Battery Undervoltage Protection (Load-Output OFF)";
static const char mksb_fsrreg6[] PROGMEM = "NRG: MkSkyBlu%u R6 = %1_fV Battery Undervoltage Recovery (Load-Output ON)";
static const char mksb_fsrreg7[] PROGMEM = "NRG: MkSkyBlu%u R7 = %0_f Com Address (not used)";
static const char mksb_fsrreg8[] PROGMEM = "NRG: MkSkyBlu%u R8 = %0_f [D04] Battery Type [0=SLA, 1=Li]";
static const char mksb_fsrreg9[] PROGMEM = "NRG: MkSkyBlu%u R9 = %0_f Battery System [1...4 * 12V] (read-only)";
static const char * mksb_register_fstrings[] PROGMEM = { mksb_fsrreg1, mksb_fsrreg2, mksb_fsrreg3, 
                                                         mksb_fsrreg4, mksb_fsrreg5, mksb_fsrreg6, 
                                                         mksb_fsrreg7, mksb_fsrreg8, mksb_fsrreg9 }; 
#endif

//const char HTTP_SNS_VOLTAGE[]             PROGMEM = "{s}" D_VOLTAGE                 "{m}%s " D_UNIT_VOLT          "{e}";
const char mksb_HTTP_SNS_EFFICIENCY[]     PROGMEM = "{s}" "Efficiency"              "{m}%s %%{e}"; // TODO add D_EFFICIENCY
const char mksb_HTTP_SNS_BATT_VOLTAGE[]   PROGMEM = "{s}" D_BATTERY " " D_VOLTAGE   "{m}%s " D_UNIT_VOLT          "{e}";
const char mksb_HTTP_SNS_BATT_CURRENT[]   PROGMEM = "{s}" D_BATTERY " " D_CURRENT   "{m}%s " D_UNIT_AMPERE        "{e}";
const char mksb_HTTP_SNS_TEMPERATURE[]    PROGMEM = "{s}" D_TEMPERATURE             "{m}%s °%c{e}";
const char mksb_HTTP_SNS_SERIAL_QOS[]     PROGMEM = "{s}" D_SERIAL_IN " " D_FAILED            "{m}%s {e}";
//    const char mksb_HTTP_SNS_SERIAL_IN[]      PROGMEM = "{s}" D_SERIAL_IN               "{m}%s " D_UNIT_FAHRENHEIT    "{e}";
const char mksb_JSON_SNS_F_TEMP[] PROGMEM = ",\"MkSkyBlu%u\":{\"" D_JSON_TEMPERATURE "\":%*_f}";
const char mksb_JSON_SNS_F_BATT_VOLTAGE[] PROGMEM = ",\"MkSkyBlu%u battery\":{\"" D_JSON_VOLTAGE "\":%*_f}";
const char mksb_JSON_SNS_F_BATT_CURRENT[] PROGMEM = ",\"MkSkyBlu%u charge\":{\"" D_JSON_CURRENT "\":%*_f}";

//static const char MKSB_HTTP_SNS_str_m_int[] PROGMEM = "{s}%s " "{m}" "%d{e}";
//static const char MKSB_HTTP_SNS_s_str_str[] PROGMEM = "{s}" "%s{m}" "%s{e}";

// module ram data
//static MKSB_MODULE_T * mksb = nullptr;


/********************************************************************************************/
/* EXTRACT UNSIGNED INT FROM SERIAL RECEIVED DATA */ 
uint32_t mExtractUint32(const char *data, uint8_t offset_lsb, uint8_t offset_msb)
{
    uint32_t result = 0;

  for ( ; offset_msb >= offset_lsb; offset_msb-- ) {
    result = (result << 8) | (uint8_t)data[offset_msb];
  }
    return result;
}


/********************************************************************************************/
/* FINALIZE SERIAL RECEIVE */ 
static void mFinishReceive(void * me)
{
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;

    #ifdef MKSB_WITH_SERIAL_DEBUGGING
    if ( mksb->rxIdx ) { // bytes received with timediff since last request
        if ( mksb->idx_tx ) { // channel is tranceiver, show time since last send
            mksb->tsRx = millis();
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Rx%u [Tx+ %ums] %*_H"),
                mksb->idx, (mksb->tsRx - mksb->tsTx), mksb->rxIdx, mksb->pRxBuffer);
        } else { // channel is receiver only, show time since last receive-finish
            mksb->tsTx = millis(); // use it for now
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Rx%u [Rx+ %ums] %*_H"),
                mksb->idx, (mksb->tsTx - mksb->tsRx), mksb->rxIdx, mksb->pRxBuffer);
            mksb->tsRx = mksb->tsTx;
        }
    }
    #endif
    // finalize reception
    mksb->Serial->flush(); // ensure receive buffer is empty
    mksb->rxIdx = 0; // reset receiver state
}


/********************************************************************************************/
/* SEND SERIAL DATA */
static void mSendSerial(void * me, uint8_t len)
{
    uint32_t i;
    uint8_t crc;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;
  
    if (mksb->idx_tx == 0) {
        return; // the channel has no transmitter, receive only
    }

    mksb->Serial->flush(); // ensure receive buffer is empty
    mksb->rxIdx = 0; // reset receiver state

    // request frame
    mksb->Serial->write( MKSB_START_FRAME );
    crc = 0;
    for ( i = 0; i < len; i++ ) { // variable data
        mksb->Serial->write(mksb->txBuffer[i]);
        crc += mksb->txBuffer[i];
    }
    mksb->Serial->write(crc); // checksum

//    mksb->rxIdx = 0; // reset receiver state
    mksb->Serial->flush(); // ensure transmission complete

#ifdef MKSB_WITH_SERIAL_DEBUGGING
    mksb->tsTx = millis();
    mksb->cntTx++;
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: Tx%u %02X%*_H%02X"), 
        mksb->idx, MKSB_START_FRAME, len, mksb->txBuffer, crc );
#endif
}


/********************************************************************************************/
/* BUILD SERIAL DATA REQUESTS FOR TRANSMISSION */
/* TODO remove ?
void MkSkyBluRequestStatus(void)
{
  static const uint8_t mksb_ser_req_status[] PROGMEM = { MKSB_CMD_AUXILARY, 0, 2, 0 };
  
  memcpy_P(mksb->txBuffer, mksb_ser_req_status, sizeof (mksb_ser_req_status));
  mSendSerial(sizeof (mksb_ser_req_status));
}
*/


/********************************************************************************************/
/* PARSE SERIAL DATA RECEIVED */
static void mParseMeasurements(void * me)
{
    uint8_t phase;
    uint32_t voltage, current, power;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;

    phase = mksb->phase_id;
    // solar
    voltage = mExtractUint32(mksb->pRxBuffer,  6, 7); // solar voltage   [0.1V]
    power   = mExtractUint32(mksb->pRxBuffer,  8, 9); // solar power     [1.0W]
    Energy->voltage[phase] = (float)voltage / 10.0f;
    Energy->active_power[phase] = (float)power;
    if ( voltage ) { // prevent division by 0
        // calculate: solar current I = P / U
        Energy->current[phase] = Energy->active_power[phase] / Energy->voltage[phase]; 
    } else {
        Energy->current[phase] = 0.0f;
    }
    Energy->data_valid[phase] = 0;

    // battery
    current = mExtractUint32(mksb->pRxBuffer,  4, 5); // battery charge current [0.1A]
    voltage = mExtractUint32(mksb->pRxBuffer,  2, 3); // battery voltage        [0.1V]
    // efficiency calculation: battery power / solar power in %
    if ( power ) {
        pMksbInstance_FloatArrays[phase] = float(current * voltage) / Energy->active_power[phase];
    } else {
        pMksbInstance_FloatArrays[phase] = 0.0f;
    }
    phase += Energy->phase_count;
    // temperature of the charge controller electronics, integer resolution is 0.1degC
    pMksbInstance_FloatArrays[phase] = ConvertTempToFahrenheit( (float)mExtractUint32(mksb->pRxBuffer, 10, 11) / 10.0f );
    phase += Energy->phase_count;
    // battery voltage, integer resolution is 0.1V
    pMksbInstance_FloatArrays[phase] = (float)voltage / 10.0f;
    phase += Energy->phase_count;
    // battery charge current, integer resolution is 0.1A
    pMksbInstance_FloatArrays[phase] = (float)current / 10.0f;
    phase += Energy->phase_count;
    // serial communication quality, integer resolution is 1%    
    pMksbInstance_FloatArrays[phase] = (float)(mksb->cntTxRxLost);

    mksb->energy_total = mExtractUint32(mksb->pRxBuffer, 12, 13); // solar energy total [1.0kWh]
 
    mksb->status = mExtractUint32(mksb->pRxBuffer, 16, 17); // mode and status TODO textual info ?

    // unused response data: TODO ?
    // mExtractUint32(mksb->pRxBuffer, 14, 15); // ?dummy [14:15]
    // mExtractUint32(mksb->pRxBuffer, 18, 18); // ?dummy [18]
}


/********************************************************************************************/
/* RECEIVE SERIAL DATA */
static void mSerialReceive(void * me)
{
    int i;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;

    while ( mksb->Serial->available() ) {
        yield();
        if (mksb->rxIdx < MKSB_RX_BUFFER_SIZE) { // buffer available
            mksb->pRxBuffer[mksb->rxIdx] = mksb->Serial->read();
            if (MKSB_START_FRAME != mksb->pRxBuffer[0] ) { // no start of frame yet
                mksb->rxIdx = 0; // reset receiver
            } else {                // [0] start valid
                if (0 == mksb->rxIdx) {  // start of frame present
                    mksb->rxChecksum = 0;  // reset checksum calc
                } else {              // [1+] cmd or later
                    if ((MKSB_RSP_READ_MEASURES == mksb->pRxBuffer[1]) && (19 == mksb->rxIdx)) {
                        if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
                            mParseMeasurements(mksb);
                            mksb->cntRxGood++;
                        } else {
                            mksb->cntRxBadCRC++;
                        }
                        mFinishReceive(me);
                    } else
#ifdef MKSB_WITH_REGISTER_SUPPORT
                    if ((MKSB_RSP_RW_CONFIG == mksb->pRxBuffer[1]) && (8 == mksb->rxIdx)) 
                    {
                        uint8_t reg;
                        if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
                            reg = mksb->pRxBuffer[2] - MKSB_REG_FIRST;
                            if ( reg < MKSB_REG_TOTAL ) {
                                mksb->regs_value[reg] = mExtractUint32(mksb->pRxBuffer, 3, 4);
                                mksb->regs_to_report |= 1 << reg;
                            }
                            mksb->cntRxGood++;                    
                        } else {
                            mksb->cntRxBadCRC++;
                        }
                        mFinishReceive(me);
                    } else
#endif
#ifdef MKSB_WITH_ON_OFF_SUPPORT
                    if ((MKSB_RSP_POWER == mksb->pRxBuffer[1]) && (5 == mksb->rxIdx)) {
                        if ( mksb->rxChecksum == mksb->pRxBuffer[mksb->rxIdx] ) {
                            if ( mksb->pRxBuffer[2] == 0 ) { /* ON */
                                if ( mksb->actual_state != true ) {
                                    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging ON")); // TODO
                                    mksb->actual_state = true;
                                }
                            } else 
                            if ( mksb->pRxBuffer[2] == 1 ) {  /* OFF */
                                if ( mksb->actual_state != false ) {
                                    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charging OFF")); // TODO
                                    mksb->actual_state = false;
                                }
                            } else { /* unknown content */
                            }
                            mksb->cntRxGood++;
                        } else {
                            mksb->cntRxBadCRC++;
                        }
                        mFinishReceive(me);
                    } else 
#endif
                    { // calc checksum
                        mksb->rxChecksum += mksb->pRxBuffer[mksb->rxIdx]; 
                    }
                } 
            }
            mksb->rxIdx++;       // more to receive         
        } else { // buffer full
            mFinishReceive(me);
//            (void)mksb->Serial->read(); // drop received byte
        }
    } 
}


/********************************************************************************************/
/* EVERY SECOND */
void MkSkyBluEverySecond(void * me)
{
    int i;
    float fValue;
    uint8_t phase;
    bool updateTotal = false;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;

    phase = mksb->phase_id;

    if (Energy->data_valid[phase] > ENERGY_WATCHDOG) {
        Energy->voltage[phase] = Energy->current[phase] = Energy->active_power[phase] = 0.0f;
        Energy->voltage[phase] = NAN; // mark as invalid
        Energy->current[phase] = NAN; // mark as invalid
        
        pMksbInstance_FloatArrays[phase] = NAN; // efficiency
        phase += Energy->phase_count;
        pMksbInstance_FloatArrays[phase] = NAN; // temperature
        phase += Energy->phase_count;
        pMksbInstance_FloatArrays[phase] = NAN; // battery voltage
        phase += Energy->phase_count;
        pMksbInstance_FloatArrays[phase] = NAN; // battery current
    } else {
        Energy->kWhtoday_delta[phase] += Energy->active_power[phase] * 1000 / 36; // solar energy only
        // import the non-resetable solar energy full kWh counter from the charge controller, but with 2 requirements:
        // 1. SetOption72 is active (bug@EnergyUpdateTotal?: a call of it impacts kWhtoday, even if option is off)
        // 2. the firmware counter is smaller than the counter of the charge controller
        if ( Settings->flag3.hardware_energy_total ) {
            fValue = (float)mksb->energy_total;
            if ( Energy->total[phase] < fValue ) {
                Energy->import_active[phase] = fValue;
                // Energy->import_active[phase_battery] = 0.0f;
                updateTotal = true;
            }
        }
    }
    mksb->ev250ms_state = 0; // sync the 250ms state machine

#ifdef MKSB_WITH_SERIAL_DEBUGGING
    if ( !mksb->cntTx ) { // nothing sent yet TODO roll-over ?
        return;             // prevent multiple useless logs at startup
    }
    if ( mksb->cntTxRxLost < (mksb->cntTx - mksb->cntRxGood) ) { // new error: immediately
        mksb->cntTxRxLost = (mksb->cntTx - mksb->cntRxGood);
//            if( !mksb->regs_to_read ) { // no config-read in progress
//                AddLog(LOG_LEVEL_ERROR, PSTR("NRG: Serial comm. Rx-total:%u (Rx-CRC:%u)"), 
//                mksb->cntTxRxLost,
//                mksb->cntRxBadCRC );
//            }
    }
    if ( 0 == (mksb->cntTx % (10 * 60) ) ) { // info: about every 10 minutes
        AddLog(LOG_LEVEL_INFO, PSTR("NRG: Serial%u statistics Tx:%u, Rx+:%u, Rx-total:%u (Rx-CRC:%u)"), 
            mksb->idx, mksb->cntTx, mksb->cntRxGood, mksb->cntTxRxLost, mksb->cntRxBadCRC );
//    AddLog(LOG_LEVEL_INFO, PSTR("NRG: Charge Controller Total Energy %u kWh"),
//      mksb->energy_total );
    }
#endif
    if ( updateTotal == true ) {
        EnergyUpdateTotal(); // this also calls EnergyUpdateToday() at the end
    } else {
        EnergyUpdateToday();
    }
}


/********************************************************************************************/
/* EVERY 250 MS */
void MkSkyBluEvery250ms(void * me)
{ 
    static const uint8_t mksb_ser_req_measures[]  PROGMEM = { MKSB_CMD_READ_MEASUREMENTS, 0,0,0 };
    static const uint8_t mksb_ser_req_write_reg[] PROGMEM = { MKSB_CMD_WRITE_REGISTER, 0 ,0,0, 0,0,0 };
    static const uint8_t mksb_ser_req_read_reg[]  PROGMEM = { MKSB_CMD_READ_REGISTER,  0 ,0,0, 0,0,0 };
    static const uint8_t mksb_ser_req_chrg_off[]  PROGMEM = { MKSB_CMD_AUXILARY, 1, 2, 0 };
    static const uint8_t mksb_ser_req_chrg_on[]   PROGMEM = { MKSB_CMD_AUXILARY, 2, 0, 0 };
    int i;
    float fVal;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;

    // if ( mksb->ev250ms_state == 0 ) {
    //     MkSkyBluRequestStatus();
    //  } else
    if ( mksb->ev250ms_state == 0 ) {
        memcpy_P(mksb->txBuffer, mksb_ser_req_measures, sizeof (mksb_ser_req_measures));
        mSendSerial(mksb, sizeof (mksb_ser_req_measures));
    } 
#ifdef MKSB_WITH_REGISTER_SUPPORT
    else if ( mksb->ev250ms_state == 3 ) {
        if ( mksb->regs_to_read || mksb->regs_to_write ) {
            for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
                if ( mksb->regs_to_write & (1 << i) ) { // prio write
                    memcpy_P(mksb->txBuffer, mksb_ser_req_write_reg, sizeof (mksb_ser_req_write_reg));
                    // variable register and value
                    mksb->txBuffer[1] = i + MKSB_REG_FIRST;
                    mksb->txBuffer[2] = (uint8_t)(mksb->regs_value[i] & 0xFF);
                    mksb->txBuffer[3] = (uint8_t)(mksb->regs_value[i] >> 8);
                    mSendSerial(mksb, sizeof (mksb_ser_req_write_reg));
                    mksb->regs_to_write &= ~(1 << i);
                    break; // only one per call
                } else 
                if ( mksb->regs_to_read & (1 << i) ) { 
                    memcpy_P(mksb->txBuffer, mksb_ser_req_read_reg, sizeof (mksb_ser_req_read_reg));
                    // variable register
                    mksb->txBuffer[1] = i + MKSB_REG_FIRST;
                    mSendSerial(mksb, sizeof (mksb_ser_req_read_reg));
                    mksb->regs_to_read &= ~(1 << i);
                    break; // only one per call
                } else {}
            }
        }
    }
#endif
#ifdef MKSB_WITH_ON_OFF_SUPPORT
    else if ( mksb->actual_state != mksb->target_state ) {
        if ( mksb->target_state == false ) { 
            memcpy_P(mksb->txBuffer, mksb_ser_req_chrg_off, sizeof (mksb_ser_req_chrg_off));
            mSendSerial(mksb, sizeof (mksb_ser_req_chrg_off));
        } else { 
            memcpy_P(mksb->txBuffer, mksb_ser_req_chrg_on, sizeof (mksb_ser_req_chrg_on));
            mSendSerial(mksb, sizeof (mksb_ser_req_chrg_on));
        }
        mksb->actual_state = mksb->target_state;
    }
#endif
#ifdef MKSB_WITH_REGISTER_SUPPORT
    if ( mksb->regs_to_report ) 
    {
        for ( i = 0; i < MKSB_REG_TOTAL; i++ ) {
            if ( mksb->regs_to_report & (1 << i) ) {
                fVal = ((float)(mksb->regs_value[i])) / 1000.0f;
                AddLog( LOG_LEVEL_ERROR, mksb_register_fstrings[i], mksb->idx, &fVal); // log always
                mksb->regs_to_report &= ~(1 << i);
                break; // once per call
            }
        }
    }
#endif
    mksb->ev250ms_state++;
    if ( mksb->ev250ms_state >= 4 ) 
    {
        mksb->ev250ms_state = 0;
    }
}


/********************************************************************************************/
/* ENERGY COMMAND */
bool MkSkyBluEnergyCommand(void * me)
{
    bool serviced = true;
    uint8_t reg;
    char *str;
    int32_t value;
    int i;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me; 

    if ((CMND_POWERCAL == Energy->command_code) || (CMND_VOLTAGECAL == Energy->command_code) || (CMND_CURRENTCAL == Energy->command_code)) {
        // Service in xdrv_03_energy.ino
    } else 
    if (CMND_ENERGYCONFIG == Energy->command_code) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Config index %d, payload %d, data '%s'"),
        XdrvMailbox.index, XdrvMailbox.payload, XdrvMailbox.data ? XdrvMailbox.data : "null" );
        if ( XdrvMailbox.data_len == 0 ) { // no arguments: Read all registers and report to log
            AddLog(LOG_LEVEL_INFO, PSTR("NRG: EnergyConfig <IF-Number> [Register-Num:1...9] [Register-Value]"));
        } else {
            str = XdrvMailbox.data;
            i = strtoul( str, &str, 10 ); // 1st argument: interface number
            if( i != mksb->idx ) {
                return serviced; // not this instance addressed
            }
            while ((*str != '\0') && isspace(*str)) { str++; };   // Trim spaces
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
                        if ( *str ) {                                        // 2nd argument: there is a value = write registers value
                            value = (int32_t)(CharToFloat(str) * 1000.0f);
                            // write Register: no range check here
                            mksb->regs_value[reg] = value;   // store value to prepared for write
                            mksb->regs_to_write |= 1 << reg; // trigger write
                        } else {                                             // 2nd argument: there is no value = read registers value
                            mksb->regs_to_read |= 1 << reg; 
                        }
                } else {
                    mksb->regs_to_read = (1 << MKSB_REG_TOTAL) - 1; // flag all registers to be read        
                }
            }
#else       
            {}
#endif
        }
    } else {
        serviced = false;  // Unknown command
    }
    return serviced;
}


/********************************************************************************************/
/* PUBLISH SENSORS (beyond Energy) */
static void MkSkyBluShow(void * me, uint32_t function) 
{
    int i;
    uint8_t phase;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;
    float fVal;
    bool voltage_common = (Settings->flag6.no_voltage_common) ? false : Energy->voltage_common;

    if ( FUNC_JSON_APPEND == function ) { 
        phase = 0;
        // Efficiency: not used for JSON
        phase += Energy->phase_count;
        // Temperature
        ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%s"),
            EnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.temperature_resolution));
#if 0 /* TODO provide battery voltage and current ? Right naming ? */
        phase += Energy->phase_count;
        // Battery Voltage
        ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE " battery\":%s"),
            EnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.voltage_resolution, voltage_common));
        phase += Energy->phase_count;
        // Battery Current
        ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT " battery\":%s"),
            EnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.current_resolution));
#endif
    }

#ifdef USE_WEBSERVER
    if ( FUNC_WEB_SENSOR == function ) {
        phase = 0;
        WSContentSend_PD(mksb_HTTP_SNS_EFFICIENCY, WebEnergyFmt(&pMksbInstance_FloatArrays[phase], 1));
        phase += Energy->phase_count;
        WSContentSend_PD(mksb_HTTP_SNS_TEMPERATURE, WebEnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.temperature_resolution), TempUnit());
        phase += Energy->phase_count;
        WSContentSend_PD(mksb_HTTP_SNS_BATT_VOLTAGE, WebEnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.voltage_resolution));
        phase += Energy->phase_count;
        WSContentSend_PD(mksb_HTTP_SNS_BATT_CURRENT, WebEnergyFmt(&pMksbInstance_FloatArrays[phase], Settings->flag2.current_resolution));
#ifdef MKSB_WITH_SERIAL_DEBUGGING
        phase += Energy->phase_count;
        WSContentSend_PD(mksb_HTTP_SNS_SERIAL_QOS, WebEnergyFmt(&pMksbInstance_FloatArrays[phase], 0));
#endif
//    WSContentSend_P( MKSB_HTTP_SNS_sS_m_Se , D_POWERUSAGE ,mksb->actual_state == true ? D_ENABLED: D_DISABLED );
    } else if ( FUNC_WEB_COL_SENSOR == function ) {
        if( mksb->phase_id == 0 ) { // only once for first channel
            WSContentSend_P( PSTR("MakeSkyBlue " D_SOLAR_POWER) ); // headline before values
        }
    } else {}
#endif  // USE_WEBSERVER
}


/********************************************************************************************/
/* Reset ENERGY */
void MkSkyBluReset(void * me)
{
    int i;
    uint8_t phase;
    MKSB_MODULE_T * mksb = (MKSB_MODULE_T *)me;
    
    // mksb->temperature = NAN;
    mksb->energy_total = 0;

    phase = mksb->phase_id;
    Energy->total[phase] = 0.0f;
//    Energy->data_valid[phase] = 0;
}


/********************************************************************************************/
/* SENSORS INIT */
void MkSkyBluSnsInit(void * me)
{
    int i;
    MKSB_MODULE_T * mksb;

    mksb = (MKSB_MODULE_T *)me;
    i = mksb->idx - 1; // inteface receiver index
    // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
    if ( mksb->idx_tx ) { // transceiver
        mksb->Serial = new TasmotaSerial(Pin(GPIO_MKSKYBLU_RX, i), Pin(GPIO_MKSKYBLU_TX, i), 1);
    } else { // receiver only
        mksb->Serial = new TasmotaSerial(Pin(GPIO_MKSKYBLU_RX, i), -1, 1);
    }
    if (mksb->Serial == nullptr) {
        AddLog(LOG_LEVEL_ERROR, PSTR("NRG: MkSkyBlu Serial alloc failed on interface %d"), mksb->idx);
        return;
        //                continue;
    }
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
        mksb->Serial = nullptr;
        AddLog(LOG_LEVEL_ERROR, PSTR("NRG: MkSkyBlu Serial init failed on interface %d"), i);
    }
}


/********************************************************************************************/
/* DRIVER INIT */
void MkSkyBluDrvInit(void)
{
    int i;
    MKSB_MODULE_T * mksb = nullptr;
    uint8_t phase = 0, u8 = 0;

//    TasmotaGlobal.energy_driver = ENERGY_NONE;
//    Energy->phase_count = 0; // initialize
    for( i = 0; i < MAX_MKSKYBLU_IF; i++ ) {
        if ( PinUsed(GPIO_MKSKYBLU_RX, i) ) { // check for configured receiver
            phase++; // count configured receiver
            if ( PinUsed(GPIO_MKSKYBLU_TX, i) ) { // check for configured transmitter
                u8++; // count configured tranceiver
            }
        }
    }
    if ( !u8 ) { // at least one transceiver needed
        return; // mkskyblu not configured, driver not active
    } else
    if( phase > ENERGY_MAX_PHASES ) { // limit to max supported phases, 2025-10-30: 3 at ESP82xx, 8 at ESP32
        phase = ENERGY_MAX_PHASES;
        AddLog(LOG_LEVEL_INFO, PSTR("NRG: MkSkyBlu phase count limited to %d, check config"), phase);
    }
    // one instance per configured receiver
    pMskbInstance = (MKSB_MODULE_T *)(malloc(phase * sizeof(MKSB_MODULE_T)));
    pMksbInstance_FloatArrays = (float *)(malloc(phase * (MKSB_INSTANCE_FLOATARRAY_SIZE * sizeof(float))));
    if ( pMskbInstance == nullptr || pMksbInstance_FloatArrays == nullptr ) {
        AddLog(LOG_LEVEL_ERROR, PSTR("NRG: MkSkyBlu malloc failed"));
        return;
    }
    // at this point we have at least one transceiver configured
    mksb = pMskbInstance; // first instance
    phase = 0;
    for( i = 0; i < MAX_MKSKYBLU_IF; i++ ) {
        if ( PinUsed(GPIO_MKSKYBLU_RX, i) ) { // check for configured receiver
            mksb->idx = i + 1; // interface receiver index
            if ( PinUsed(GPIO_MKSKYBLU_TX, i) ) { // transceiver
                mksb->idx_tx = mksb->idx; // interface transmitter index
            } else {
                mksb->idx_tx = 0; // unknown related transmitter
            }
            mksb->Serial = nullptr;
            mksb->pRxBuffer = nullptr; // allocated at SnsInit
            mksb->phase_id = phase++;
            // preset / fixed module values
            mksb->energy_total = 0;
            mksb->regs_to_read = 0;
            mksb->regs_to_write = 0;
            mksb->regs_to_report = 0;
#ifdef MKSB_WITH_ON_OFF_SUPPORT
            mksb->actual_state = true; // default: charging enabled
            mksb->target_state = true; // default: charging enabled
#endif
            mksb++; // next instance
        }
    }
    for( i = 0; i < (phase * MKSB_INSTANCE_FLOATARRAY_SIZE); i++ ) {
        pMksbInstance_FloatArrays[i] = NAN;
    }
    // preset / fixed energy values
    Energy->phase_count = phase;
    Energy->voltage_common = false;
    Energy->frequency_common = true;
    Energy->type_dc = true;
    Energy->use_overtemp = false;     // ESP device acts as separated gateway, charge controller has its own temperature management
    Energy->voltage_available = true; // both communicated
    Energy->current_available = true; // solar calculated from communicated power, battery communicated
    AddLog(LOG_LEVEL_INFO, PSTR("NRG: MkSkyBlu driver initialized with %d phase(s)"), Energy->phase_count);
    TasmotaGlobal.energy_driver = XNRG_26;
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg26(uint32_t function)
{
    bool result = false;
    int i;
    MKSB_MODULE_T * mksb;

    if ( function == FUNC_PRE_INIT ) {
        MkSkyBluDrvInit(); // create all instances
        return result;
    }
    for( i = 0; i < Energy->phase_count; i++ ) 
    {
        mksb = &pMskbInstance[i];
        if ( (void *)mksb == nullptr ) {
            continue; // instance not configured
        }
        switch (function) {
        case FUNC_LOOP:
            if (mksb->Serial) { mSerialReceive((void *)mksb); }
            break;
        case FUNC_EVERY_250_MSECOND:
            if (mksb->Serial) { MkSkyBluEvery250ms((void *)mksb); }
            break;
        case FUNC_ENERGY_EVERY_SECOND:
            MkSkyBluEverySecond((void *)mksb);
            break;
        case FUNC_JSON_APPEND:
        case FUNC_WEB_SENSOR:
        case FUNC_WEB_COL_SENSOR:
            MkSkyBluShow((void *)mksb, function);
            return result; // new concept using all columns at once
            break;
        case FUNC_ENERGY_RESET:
//            MkSkyBluReset((void *)mksb);
            break;
        case FUNC_COMMAND:
            result = MkSkyBluEnergyCommand((void *)mksb);
            break;
        case FUNC_INIT:
            MkSkyBluSnsInit((void *)mksb);
            break;
        }
    }
    return result;
}

#endif  // USE_MAKE_SKY_BLUE
#endif  // USE_ENERGY_SENSOR
