/*
  xsns_44_sps30.ino - Sensirion SPS30 support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_SPS30

#define XSNS_44 44
#define XI2C_30 30  // See I2CDEVICES.md

#define SPS30_ADDR 0x69

#include <Wire.h>
#include <twi.h>

uint8_t sps30_ready = 0;
uint8_t sps30_running;

struct SPS30 {
  float PM1_0;
  float PM2_5;
  float PM4_0;
  float PM10;
  float NCPM0_5;
  float NCPM1_0;
  float NCPM2_5;
  float NCPM4_0;
  float NCPM10;
  float TYPSIZ;
} sps30_result;

#define SPS_CMD_START_MEASUREMENT 0x0010
#define SPS_CMD_START_MEASUREMENT_ARG 0x0300
#define SPS_CMD_STOP_MEASUREMENT 0x0104
#define SPS_CMD_READ_MEASUREMENT 0x0300
#define SPS_CMD_GET_DATA_READY 0x0202
#define SPS_CMD_AUTOCLEAN_INTERVAL 0x8004
#define SPS_CMD_CLEAN 0x5607
#define SPS_CMD_GET_ACODE 0xd025
#define SPS_CMD_GET_SERIAL 0xd033
#define SPS_CMD_RESET 0xd304
#define SPS_WRITE_DELAY_US 20000
#define SPS_MAX_SERIAL_LEN 32

uint8_t sps30_calc_CRC(uint8_t *data) {
    uint8_t crc = 0xFF;
    for (uint32_t i = 0; i < 2; i++) {
        crc ^= data[i];
        for (uint32_t bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x31u;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

void CmdClean(void);

unsigned char twi_readFrom(unsigned char address, unsigned char* buf, unsigned int len, unsigned char sendStop);

void sps30_get_data(uint16_t cmd, uint8_t *data, uint8_t dlen) {
unsigned char cmdb[2];
uint8_t tmp[3];
uint8_t index=0;
memset(data,0,dlen);
uint8_t twi_buff[64];

  Wire.beginTransmission(SPS30_ADDR);
  cmdb[0]=cmd>>8;
  cmdb[1]=cmd;
  Wire.write(cmdb,2);
  Wire.endTransmission();

  // need 60 bytes max
  dlen/=2;
  dlen*=3;

  twi_readFrom(SPS30_ADDR,twi_buff,dlen,1);

  uint8_t bind=0;
  while (bind<dlen) {
    tmp[0] = twi_buff[bind++];
    tmp[1] = twi_buff[bind++];
    tmp[2] = twi_buff[bind++];
    if (sps30_calc_CRC(tmp)!=tmp[2]) {
      // chksum error
      index+=2;
    } else {
      data[index++]=tmp[0];
      data[index++]=tmp[1];
    }
  }
}

void sps30_cmd(uint16_t cmd) {
unsigned char cmdb[6];
  Wire.beginTransmission(SPS30_ADDR);
  cmdb[0]=cmd>>8;
  cmdb[1]=cmd;

  if (cmd==SPS_CMD_START_MEASUREMENT) {
    cmdb[2]=SPS_CMD_START_MEASUREMENT_ARG>>8;
    cmdb[3]=SPS_CMD_START_MEASUREMENT_ARG&0xff;
    cmdb[4]=sps30_calc_CRC(&cmdb[2]);
    Wire.write(cmdb,5);
  } else {
    Wire.write(cmdb,2);
  }
  Wire.endTransmission();
}

void SPS30_Detect(void)
{
  if (!I2cSetDevice(SPS30_ADDR)) { return; }
  I2cSetActiveFound(SPS30_ADDR, "SPS30");

  uint8_t dcode[32];
  sps30_get_data(SPS_CMD_GET_SERIAL,dcode,sizeof(dcode));
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("sps30 found with serial: %s"),dcode);
  sps30_cmd(SPS_CMD_START_MEASUREMENT);
  sps30_running = 1;
  sps30_ready = 1;
}

#define D_UNIT_PM "ug/m3"
#define D_UNIT_NCPM "#/m3"

#ifdef USE_WEBSERVER
const char HTTP_SNS_SPS30_a[] PROGMEM ="{s}SPS30 " "%s" "{m}%s " D_UNIT_PM "{e}";
const char HTTP_SNS_SPS30_b[] PROGMEM ="{s}SPS30 " "%s" "{m}%s " D_UNIT_NCPM "{e}";
const char HTTP_SNS_SPS30_c[] PROGMEM ="{s}SPS30 " "TYPSIZ" "{m}%s " "um" "{e}";
#endif  // USE_WEBSERVER

#define PMDP 2

#define SPS30_HOURS Settings.sps30_inuse_hours
//#define SPS30_HOURS sps30_inuse_hours
//uint8_t sps30_inuse_hours;

void SPS30_Every_Second() {
  if (!sps30_running) return;

  if (uptime%10==0) {
    uint8_t vars[sizeof(float)*10];
    sps30_get_data(SPS_CMD_READ_MEASUREMENT,vars,sizeof(vars));
    float *fp=&sps30_result.PM1_0;

    typedef union {
    uint8_t array[4];
    float value;
    } ByteToFloat;

    ByteToFloat conv;

    for (uint32_t count=0; count<10; count++) {
      for (uint32_t i = 0; i < 4; i++){
        conv.array[3-i] = vars[count*sizeof(float)+i];
      }
      *fp++=conv.value;
    }
  }

  if (uptime%3600==0 && uptime>60) {
    // should auto clean once per week runtime
    // so count hours, should be in Settings
    SPS30_HOURS++;
    if (SPS30_HOURS>(7*24)) {
      CmdClean();
      SPS30_HOURS=0;
    }
  }

}

void SPS30_Show(bool json)
{
  if (!sps30_running) { return; }

  char str[64];
  if (json) {
    dtostrfd(sps30_result.PM1_0,PMDP,str);
    ResponseAppend_P(PSTR(",\"SPS30\":{\"" "PM1_0" "\":%s"), str);
    dtostrfd(sps30_result.PM2_5,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "PM2_5" "\":%s"), str);
    dtostrfd(sps30_result.PM4_0,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "PM4_0" "\":%s"), str);
    dtostrfd(sps30_result.PM10,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "PM10" "\":%s"), str);
    dtostrfd(sps30_result.NCPM0_5,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "NCPM0_5" "\":%s"), str);
    dtostrfd(sps30_result.NCPM1_0,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "NCPM1_0" "\":%s"), str);
    dtostrfd(sps30_result.NCPM2_5,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "NCPM2_5" "\":%s"), str);
    dtostrfd(sps30_result.NCPM4_0,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "NCPM4_0" "\":%s"), str);
    dtostrfd(sps30_result.NCPM10,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "NCPM10" "\":%s"), str);
    dtostrfd(sps30_result.TYPSIZ,PMDP,str);
    ResponseAppend_P(PSTR(",\"" "TYPSIZ" "\":%s}"), str);

#ifdef USE_WEBSERVER
  } else {
    dtostrfd(sps30_result.PM1_0,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_a,"PM 1.0",str);
    dtostrfd(sps30_result.PM2_5,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_a,"PM 2.5",str);
    dtostrfd(sps30_result.PM4_0,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_a,"PM 4.0",str);
    dtostrfd(sps30_result.PM10,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_a,"PM 10",str);
    dtostrfd(sps30_result.NCPM0_5,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_b,"NCPM 0.5",str);
    dtostrfd(sps30_result.NCPM1_0,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_b,"NCPM 1.0",str);
    dtostrfd(sps30_result.NCPM2_5,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_b,"NCPM 2.5",str);
    dtostrfd(sps30_result.NCPM4_0,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_b,"NCPM 4.0",str);
    dtostrfd(sps30_result.NCPM10,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_b,"NCPM 10",str);
    dtostrfd(sps30_result.TYPSIZ,PMDP,str);
    WSContentSend_PD(HTTP_SNS_SPS30_c,str);
#endif
  }
}

void CmdClean(void)
{
  sps30_cmd(SPS_CMD_CLEAN);
  ResponseTime_P(PSTR(",\"SPS30\":{\"CFAN\":\"true\"}}"));
  MqttPublishTeleSensor();
}

bool SPS30_cmd(void)
{
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
      char *cp=XdrvMailbox.data;
      if (*cp=='c') {
        // clean cmd
        CmdClean();
      } else if (*cp=='0' || *cp=='1') {
        sps30_running=*cp&1;
        sps30_cmd(sps30_running?SPS_CMD_START_MEASUREMENT:SPS_CMD_STOP_MEASUREMENT);
      } else {
        serviced=false;
      }
  }
  Response_P(PSTR("{\"SPS30\":\"%s\"}"), sps30_running?"running":"stopped");

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns44(byte function)
{
  if (!I2cEnabled(XI2C_30)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    SPS30_Detect();
  }
  else if (sps30_ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        SPS30_Every_Second();
        break;
      case FUNC_JSON_APPEND:
        SPS30_Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SPS30_Show(0);
        break;
  #endif  // USE_WEBSERVER
      case FUNC_COMMAND_SENSOR:
        if (XSNS_44 == XdrvMailbox.index) {
          result = SPS30_cmd();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_SPS30
#endif  // USE_I2C
