/*
  XSNS_95_cm1107.ino - CM1107(B) CO2 sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef USE_CM110x
/*********************************************************************************************\
 * CM11xx - CO2 sensor
 * https://en.gassensor.com.cn/CO2Sensor/list.html
 * Adapted from Mhz19 plugin by Maksim (rekin.m ___ gmail.com)
 *
 * Hardware Serial will be selected if GPIO1 = [CM11 Rx] and GPIO3 = [CM11 Tx]
 **********************************************************************************************
 * Filter usage
 *
 * Select filter usage on low stability readings
 * 
 * *******************************************************************************************
 * Some CM11 models has manual or continuos modes - this logic not implemented.
\*********************************************************************************************/

#define XSNS_95                      95

enum CM11FilterOptions {CM1107_FILTER_OFF, CM1107_FILTER_FAST, CM1107_FILTER_MEDIUM, CM1107_FILTER_MEDIUM2, CM1107_FILTER_SLOW};

#ifndef CM1107_FILTER_OPTION
  #define CM1107_FILTER_OPTION          CM1107_FILTER_FAST
#endif
/*********************************************************************************************\
 * Source: https://en.gassensor.com.cn/CO2Sensor/list.html (pdf for 1106/1107/1109 sensors)
 *  
 * 
 * Automatic Baseline Correction (ABC logic function) is enabled by default but may be disabled with command
 * Sensor95 0
 * and enabled again with command
 * Sensor95 1
 *
 * ABC logic function refers to that sensor itself do zero point judgment and automatic calibration procedure
 * intelligently after a continuous operation period. The automatic calibration cycle is first 24 hours and 7 days cycle after powered on.
 *
 * The zero point of automatic calibration is 400ppm.
 *
 * This function is usually suitable for indoor air quality monitor such as offices, schools and homes,
 * not suitable for greenhouse, farm and refrigeratory where this function should be off.
 *
 * Please do zero calibration timely, such as manual or command calibration.
\*********************************************************************************************/

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#define CM1107_READ_TIMEOUT           400     // Must be way less than 1000 but enough to read 16 bytes at 9600 bps
#define CM1107_RETRY_COUNT            8

TasmotaSerial *CM11Serial;


const char CM11_ABC_ENABLED[] = "ABC is Enabled";
const char CM11_ABC_DISABLED[] = "ABC is Disabled";

//First [0] element - lenght of cmd and data
const uint8_t cmd_read[] = {0x01,0x01};   // cm11_cmnd_read_ppm
uint8_t cmd_abc_enable[] =  {0x07,0x10,0x64,0x00,0x07,0x01,0x90,0x64};   // cm11_cmnd_abc_enable. Not const because can be modified
const uint8_t cmd_abc_disable[] = {0x07,0x10,0x64,0x02,0x07,0x01,0x90,0x64};   // cm11_cmnd_abc_disable
const uint8_t cmd_zeropoint[] = {0x03,0x03,0x01,0x90};   // cm11_cmnd_zeropoint_400
const uint8_t cmd_serial[] = {0x01,0x1F};   // cm11_cmnd_read_serial
const uint8_t cmd_sw_version[] = {0x01,0x1E};  // cm11_cmnd_read_sw_version


enum CM11Commands { CM11_CMND_READPPM, CM11_CMND_ABCENABLE, CM11_CMND_ABCDISABLE, CM11_CMND_ZEROPOINT, CM11_CMND_SERIAL,CM11_CMND_SW_VERSION };
const uint8_t* kCM11Commands[] PROGMEM = {
  cmd_read,
  cmd_abc_enable,
  cmd_abc_disable,
  cmd_zeropoint,
  cmd_serial,
  cmd_sw_version
};


uint8_t cm11_type = 1;
uint16_t cm11_last_ppm = 0;
uint8_t cm11_filter = CM1107_FILTER_OPTION;
bool cm11_abc_must_apply = false;

float cm11_temperature = 0;
uint16_t cm11_humidity = 0;
char cm11_sw_version[30] = {0};
char cm11_serial_number[21] = {0};
uint8_t cm11_retry = CM1107_RETRY_COUNT;
uint8_t cm11_received = 0;
uint8_t cm11_state = 0;
uint16_t ppm_low_limit = 0;
uint16_t ppm_high_limit = 5000;

/*********************************************************************************************/
//256-(HEAD+LEN+CMD+DATA)%256
uint8_t CM11CalculateChecksum(uint8_t *array,uint8_t start, uint8_t len)
{
  uint8_t checksum = 0;
  for (uint8_t i = start; i < len; i++) {
    checksum += array[i];
  }
  checksum = checksum%256;
  checksum = 255 - checksum;
  return (checksum +1);
}

size_t CM11SendCmd(uint8_t command_id)
{
  uint8_t len =kCM11Commands[command_id][0];
  uint8_t cm11_send[len+3];// = {0}; //Fix length
  memset( cm11_send, 0, (len+3)*sizeof(uint8_t) );

  cm11_send[0] = 0x11;  // Start byte, fixed

  memcpy_P(&cm11_send[1], kCM11Commands[command_id], (len+1) * sizeof(uint8_t));

  cm11_send[len+2] = CM11CalculateChecksum(cm11_send,0, len+2);
  
#ifdef DEBUG_TASMOTA_SENSOR
  char cmdFull[len+30];// = {0};
  memset( cmdFull, 0, (len+3)*sizeof(char) );
  for(int i=0, j=0;i<len+3;i=i+1, j=j+3)
  {
    sprintf(&cmdFull[j],"%02x ", cm11_send[i]);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("Final CM11Command: %s"),cmdFull);
#endif  // DEBUG_TASMOTA_SENSOR
  cm11_received = 0;
  cm11_state = 0;
  return CM11Serial->write(cm11_send, sizeof(cm11_send));
}

/*********************************************************************************************/

bool CM11CheckAndApplyFilter(uint16_t ppm, uint8_t drift)
{
#ifdef DEBUG_TASMOTA_SENSOR
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM11 ppm: %u, last ppm: %u"),ppm, cm11_last_ppm);
#endif //DEBUG_TASMOTA_SENSOR
  if (cm11_last_ppm < ppm_low_limit || cm11_last_ppm > ppm_high_limit) {
    // Prevent unrealistic values during start-up with filtering enabled.
    // Just assume the entered value is correct.
    cm11_last_ppm = ppm;
    return true;
  }
  int32_t difference = ppm - cm11_last_ppm;
  if (drift > 0  && cm11_filter != CM1107_FILTER_OFF) {
    difference >>= CM1107_FILTER_SLOW; // If drifting values -> apply slow filter
  }else if (CM1107_FILTER_OFF == cm11_filter) {
    if (drift != 0 ) {
      return false; //Do not alarm on such unstable values
    }
   }else {
    difference >>=cm11_filter;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM11 diff: %d"),difference);
  cm11_last_ppm = static_cast<uint16_t>(cm11_last_ppm + difference);
  return true;
}

void CM11EverySecond(void)
{
  cm11_state++;
  //If more than one command was send
  //Reading preffered
  if (CM11Serial->available() > 0){
    cm11_received = 0;
  }

  if ((8 == cm11_state && cm11_received) || 16 == cm11_state) {                   // Every 8 sec start a CM11 measuring cycle (which takes 1005 +5% ms)
    cm11_state = 0;

    if (cm11_retry) {
      cm11_retry--;
      if (!cm11_retry) {
        cm11_last_ppm = 0;
        cm11_temperature = 0;
        cm11_humidity = 0;
      }
    }

    CM11Serial->flush();                    // Sync reception
    CM11SendCmd(CM11_CMND_READPPM);
    cm11_received = 0;
  }

  if ((cm11_state > 2) && !cm11_received) {  // Start reading response after 3 seconds every second until received
    uint8_t cm11_response[50];
    unsigned long start = millis();
    uint8_t counter = 0;
    uint8_t resp_len = 50;
    while (((millis() - start) < CM1107_READ_TIMEOUT) && (counter < resp_len)) {
      if (CM11Serial->available() > 0) {
        cm11_response[counter++] = CM11Serial->read();
        if (counter ==2 && cm11_response[0] == 0x16) { //0x16 - first byte in response
          resp_len = cm11_response[1] +3 ; // Get expected response len (according protocol desc), +3 - first byte, len and checksum
        }
      } else {
        delay(5);
      }
    }

    if (counter < 5) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM1107 timeout (command sent, no responce"));
      return;
    }

    uint8_t crc = CM11CalculateChecksum(cm11_response,0, cm11_response[1]+2);
    if (cm11_response[cm11_response[1]+2] != crc) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM1107 crc error"));
      return;
    }
    if (0x16 != cm11_response[0]) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM1107 bad response"));
      return;
    }

    cm11_received = 1;

    if (cm11_response[2]==cmd_read[1]){  //0x01 - read command
      uint16_t ppm = (cm11_response[3] << 8) | cm11_response[4];
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM11 PPM: %u"),ppm);
      if (ppm ==550) { // Preheating mode, fixed value. 
      //DOCs says that preheating is cm11_response[5] & (1 << 0)) ==1 (first bit ==1), but mine sensor (CM1107, sw V1.07.0.02 )
      // set first bit 0 when preheating at switch to 1 then finished.
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM11 preheating"));
        if (Settings->SensorBits1.mhz19b_abc_disable) {
          // After bootup of the sensor the ABC will be enabled.
          // Thus only actively disable after bootup.
          cm11_abc_must_apply = true;
      }
        return;
      }
      if(cm11_response[1] ==13) { // CM1107T with temperature and humidity
        cm11_temperature = (float)(((cm11_response[7] << 8) | cm11_response[8]) - 4685)/100.0f;
        cm11_humidity =  (((cm11_response[9] << 8) | cm11_response[10]) - 600)/100;
        cm11_type = 2;
      }
      uint8_t cm11_drift = (cm11_response[5] & (1 << 7)) ? 1:0;
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "CM11 flags DF3: %02x"),cm11_response[5]);

      if (CM11CheckAndApplyFilter(ppm,cm11_drift)) {
        cm11_retry = CM1107_RETRY_COUNT;
#ifdef USE_LIGHT
        LightSetSignal(CO2_LOW, CO2_HIGH, cm11_last_ppm);
#endif  // USE_LIGHT

        if (!cm11_drift) {  // Measuring is stable.
          if (cm11_abc_must_apply) {
            cm11_abc_must_apply = false;
            if (!Settings->SensorBits1.mhz19b_abc_disable) {
              CM11SendCmd(CM11_CMND_ABCENABLE);
            } else {
              CM11SendCmd(CM11_CMND_ABCDISABLE);
            }
          }
        }

      }
    }

    if (cm11_response[2]==cmd_sw_version[1]){  //0x1E - read SW version
      memcpy_P(cm11_sw_version, &cm11_response[3], cm11_response[1] * sizeof(uint8_t));
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "SW version: %s"),cm11_sw_version);
    }

    if (cm11_response[2]==cmd_serial[1]){  //0x1F - read serial
    // Serial num coded as 5 integers 0..9999. Each integer is uint16_t size
      for (uint8_t i=0; i<cm11_response[1]-1;i=i+2){ //for each 2 uint8_t
        uint16_t v = (cm11_response[3+i] <<8) | cm11_response[3+i+1]; // get int value
        sprintf_P(cm11_serial_number+i*2,"%04u", v); //print int value to result str
      }
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "Serial number: %s"),cm11_serial_number);
    }   

  }
}

/*********************************************************************************************\
 * Command Sensor15
 *
 * 0    - ABC Off
 * 1    - ABC On (Default)
 * 2    - Manual start = ABC Off (set zero point)
 * 3    - Get SW version
 * 4    - Get serial
 * 5    - Set limits
 * 6    - Set filter mode
\*********************************************************************************************/


bool CM11CommandSensor(void)
{
  bool serviced = true;

  switch (XdrvMailbox.payload) {
    case 0:
      Settings->SensorBits1.mhz19b_abc_disable = true;
      CM11SendCmd(CM11_CMND_ABCDISABLE);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, CM11_ABC_DISABLED);
      break;
    case 1:
      Settings->SensorBits1.mhz19b_abc_disable = false;
      CM11SendCmd(CM11_CMND_ABCENABLE);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, CM11_ABC_ENABLED);
      break;
    case 2:
      CM11SendCmd(CM11_CMND_ZEROPOINT); 
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, D_JSON_ZERO_POINT_CALIBRATION);
      break;
    case 3:
      CM11SendCmd(CM11_CMND_SW_VERSION);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "CM11 sw version");
      break;
    case 4:
      CM11SendCmd(CM11_CMND_SERIAL);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "CM11 serial number");
      break;

    default:
     // Set ppm limits: 5,<low_limit>,<high_limit>
     // ABS period cmd(with enabling ABS): 1,[1..30]
      uint32_t parm[3] = { 0 };
      ParseParameters(3, parm);
      switch (parm[0]) {
        case 1:
          if (parm[1]>=1 && parm[1]<=30){
            cmd_abc_enable[4] = parm[1]; //set uint8 from uint32 *o*, but value limited by 30
            Settings->SensorBits1.mhz19b_abc_disable = false;
            CM11SendCmd(CM11_CMND_ABCENABLE);
            Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, CM11_ABC_ENABLED);
          } else {
            Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "Valid period value: [1..30]");
          }
          break;
        // Set sensor ppm limit. Default 0..5000, but some sensors has another range.
        case 5:
          if(parm[1]>=0 && parm[1] <=10000 && parm[2]>=0 && parm[2] <=10000 && parm[1]<parm[2]) {
            ppm_low_limit = parm[1];
            ppm_high_limit = parm[2];
          }else{
            Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "Invalid PPM limits: [0..10000]");
          }
          break;
        // Set filter mode
        case 6:
          if (parm[1] >=0 && parm[1]<=4) {
            cm11_filter = parm[1];
            Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "CM11 set filter mode");
          }
          else {
            Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "Invalid filter mode: [0..4]. 0 - Off, 1 (Fast) -> 4 (Slow)");
          }
          break;
        default:
          Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_95, "Unknown command");
          break;
      }

  }

  return serviced;
}

/*********************************************************************************************/

void CM11Init(void)
{
  cm11_type = 0;
  if (PinUsed(GPIO_CM11_RXD) && PinUsed(GPIO_CM11_TXD)) {
    CM11Serial = new TasmotaSerial(Pin(GPIO_CM11_RXD), Pin(GPIO_CM11_TXD), 1);
    if (CM11Serial->begin(9600)) {
      if (CM11Serial->hardwareSerial()) { ClaimSerial(); }
      cm11_type = 1;
      CM11SendCmd(CM11_CMND_SW_VERSION);
    }
  }
}

void CM11Show(bool json)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"CM11\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_VERSION "\":\"%s\",\"Serial number\":\"%s\""),
      cm11_last_ppm, cm11_sw_version, cm11_serial_number);
    if(cm11_type == 2) { // With temp and humidity
      ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%*_f"),
       Settings->flag2.temperature_resolution, &cm11_temperature);
    }
    ResponseAppend_P(PSTR("}"));
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, cm11_last_ppm);
      if(cm11_type == 2) { // With temp and humidity
        DomoticzFloatSensor(DZ_TEMP, cm11_temperature);
      }
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, "CM11", cm11_last_ppm);
    if(cm11_type == 2) { // With temp and humidity
      WSContentSend_Temp("CM11", cm11_temperature);
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns95(uint8_t function)
{
  bool result = false;

  if (cm11_type) {
    switch (function) {
      case FUNC_INIT:
        CM11Init();
        break;
      case FUNC_EVERY_SECOND:
        CM11EverySecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_95 == XdrvMailbox.index) {
          result = CM11CommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        CM11Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        CM11Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_CM110x
