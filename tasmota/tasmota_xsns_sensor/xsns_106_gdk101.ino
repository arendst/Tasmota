/*
  xsns_106_gdk101.ino - Support for GDK101 gamma radiation sensor
   
  Copyright (C) 2019   Petr Novacek

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
#ifdef USE_GDK101

/*********************************************************************************************\
 * GDK101 gamma radiation sensor ftlab.co.kr
 *
 * For background information see http://allsmartlab.com/eng/294-2/
 *
 * I2C Address: 
 * A0 Short, A1 Short : 0x18
 * A0 Open,  A1 Short : 0x19
 * A0 Short, A1 Open  : 0x1A
 * A0 Open,  A1 Open  : 0x1B
\*********************************************************************************************/

#define XSNS_106  106

#define XI2C_79   79  // See I2CDEVICES.md

// I2C adresses
#define GDK101_ADDRESS1					0x18
#define GDK101_ADDRESS2					0x19
#define GDK101_ADDRESS3					0x1A
#define GDK101_ADDRESS4					0x1B

// I2C register
#define READ_FIRMWARE           0xB4
#define RESET                   0xA0
#define READ_STATUS             0xB0
#define READ_MEASURING_TIME     0xB1
#define READ_10MIN_AVG          0xB2 
#define READ_1MIN_AVG           0xB3

// gdk101 variables
uint8_t gdk101_address;
uint8_t gdk101_addresses[] = { GDK101_ADDRESS1, GDK101_ADDRESS2, GDK101_ADDRESS3, GDK101_ADDRESS4 };
uint8_t gdk101_discovered = false;

typedef enum {
 GDK_STATUS_READY,
 GDK_STATUS_UNDER_10_MINUTES,
 GDK_STATUS_NORMAL,
 GDK_STATUS_INVALID,
} gdk_status_t;

typedef struct gdk_status_data {
  gdk_status_t status;
  bool vibration;
} gdk_status_data_t;

typedef struct gdk_measuring_time {
  uint8_t min;
  uint8_t sec;
} gdk_measuring_time_t;

typedef struct gdk_fW_version {
  uint8_t major;
  uint8_t minor;
} gdk_fW_version_t;

typedef struct gdk_avg_dose {
  uint8_t integral;
  uint8_t fractional;
} gdk_avg_dose_t;

bool gdk_ready = false;
gdk_fW_version_t fw_version;
gdk_avg_dose_t mea_10min_avg;
gdk_avg_dose_t mea_1min_avg;
gdk_status_data_t gdk_status = {GDK_STATUS_INVALID, false};
gdk_measuring_time_t gdk_time = {0, 0};
uint8_t evr10 = 0;
// uint8_t vibr = 0;

#ifdef USE_WEBSERVER
const char HTTP_GDK101_FW[] PROGMEM = "{s}%s " "FW Version" "{m}%d.%d{e}";
const char HTTP_GDK101_STATUS[] PROGMEM = "{s}%s " "Status" "{m}%d{e}";
const char HTTP_GDK101_VIB_STATUS[] PROGMEM = "{s}%s " "Vibration Status" "{m}%d{e}";
const char HTTP_GDK101_MEAS_TIME[] PROGMEM = "{s}%s " "Measurement Time" "{m}%d:%02d{e}";
#endif  // USE_WEBSERVER

bool gdk101_reset(bool *reset);

void gdk101_err_log(bool ret) {
  if(!ret) {
     DEBUG_SENSOR_LOG("GDK101: I2C read error");
  }
}

void gdk101_detect(void)
{
  bool ret = false;
  bool reset = false;
  for (uint32_t i = 0; i < sizeof(gdk101_addresses); i++) {
    gdk101_address = gdk101_addresses[i];
    if (!I2cSetDevice(gdk101_address)) {
      continue; // do not make the next step without a confirmed device on the bus
    }
    ret = gdk101_reset(&reset);
    if(ret) {
      if(reset) {
        delay(10);
        I2cSetActiveFound(gdk101_address, "GDK101");
        ret = gdk101_get_fw_version(&fw_version);
        gdk101_err_log(ret);
        gdk_ready = true;
        break;
      }
    }
  }
}

void gdk101_init() {

  if (gdk_ready) return;
  
  gdk101_detect();

}

bool gdk101_reset(bool *reset) {
  uint8_t buf;
  bool ret = false;
  ret = I2cValidRead8(&buf, gdk101_address, RESET);
  if (ret) {
    *reset = (bool) buf;
  }
  return ret;
}

bool gdk101_get_fw_version(struct gdk_fW_version *fw) {
  uint8_t buf[2];
  bool ret = false;
  ret = I2cValidRead16LE((uint16_t*) buf, gdk101_address, READ_FIRMWARE);
  if (ret) {
    fw->major = buf[0];
    fw->minor = buf[1];
  }
  return ret;
}

bool gdk101_get_10min_avg(struct gdk_avg_dose *avg) {
  uint8_t buf[2];
  bool ret = false;
  ret = I2cValidRead16LE((uint16_t*) buf, gdk101_address, READ_10MIN_AVG);
  if (ret) {
    avg->integral = buf[0];
    avg->fractional = buf[1];
  }
  return ret;
}

bool gdk101_get_1min_avg(struct gdk_avg_dose *avg) {
  uint8_t buf[2];
  bool ret = false;
  ret = I2cValidRead16LE((uint16_t*) buf, gdk101_address, READ_1MIN_AVG);
  if (ret) {
    avg->integral = buf[0];
    avg->fractional = buf[1];
  }
  return ret;
}

bool gdk101_get_status(struct gdk_status_data *status) {
  uint8_t buf[2];
  bool ret = false;
  ret = I2cValidRead16LE((uint16_t*) buf, gdk101_address, READ_STATUS);
  if (ret) {
    status->status = (gdk_status_t) buf[0];
    status->vibration = (bool) buf[1];
  }
  return ret;
}

bool gdk101_get_measuring_time(struct gdk_measuring_time *meas_time) {
  uint8_t buf[2];
  bool ret = false;
  ret = I2cValidRead16LE((uint16_t*) buf, gdk101_address, READ_MEASURING_TIME);
  if (ret) {
    meas_time->min = buf[0];
    meas_time->sec = buf[1];
  }
  return ret;
}

void every_second(void) {
  if (!gdk_ready) return;
  
  gdk101_every_second();
  
  if (evr10 == 10) {
    gdk101_every_10_second();
    evr10 = 1;
  }
  else {
    evr10++;
  }

}

void gdk101_every_second(void) {
  bool ret = false;

  ret = gdk101_get_status(&gdk_status);
  gdk101_err_log(ret);
}

void gdk101_every_10_second(void) {
  bool ret = false;

  ret = gdk101_get_10min_avg(&mea_10min_avg);
  gdk101_err_log(ret);
  ret = gdk101_get_1min_avg(&mea_1min_avg);
  gdk101_err_log(ret);
  ret = gdk101_get_measuring_time(&gdk_time);
  gdk101_err_log(ret);
}

void gdk101_show(uint8_t json) {
  char types[16];
  strcpy_P(types, PSTR("GDK101"));

  if (!gdk_ready) return;

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{"), types);
    ResponseAppend_P(PSTR("\"FW_V\":%i.%i,"), fw_version.major, fw_version.minor);
    ResponseAppend_P(PSTR("\"10MIN_AVG\":%i.%02i,"), mea_10min_avg.integral, mea_10min_avg.fractional);
    ResponseAppend_P(PSTR("\"1MIN_AVG\":%i.%02i,"), mea_1min_avg.integral, mea_1min_avg.fractional);
    ResponseAppend_P(PSTR("\"STATUS\":%i,"), gdk_status.status);
    ResponseAppend_P(PSTR("\"VIBRATION\":%i,"), gdk_status.vibration);
    ResponseAppend_P(PSTR("\"MEAS_TIME\":\"%i:%02i\""), gdk_time.min, gdk_time.sec);
    ResponseJsonEnd();
#ifdef USE_WEBSERVER
  } else {
#ifdef GDK101_SHOW_FW_VERSION
    WSContentSend_PD(HTTP_GDK101_FW, types, fw_version.major, fw_version.minor);
#endif
    WSContentSend_PD(HTTP_SNS_AVG_RAD_DOSE, types, "@10", mea_10min_avg.integral, mea_10min_avg.fractional);
    WSContentSend_PD(HTTP_SNS_AVG_RAD_DOSE, types, "@1", mea_1min_avg.integral, mea_1min_avg.fractional);
#ifdef GDK101_SHOW_STATUS
    WSContentSend_PD(HTTP_GDK101_STATUS, types, gdk_status.status);
#endif
#ifdef GDK101_SHOW_VIBRATION_STATUS
    WSContentSend_PD(HTTP_GDK101_VIB_STATUS, types, gdk_status.vibration);
#endif
#ifdef GDK101_SHOW_MEAS_TIME
    WSContentSend_PD(HTTP_GDK101_MEAS_TIME, types, gdk_time.min, gdk_time.sec);
#endif
#endif  // USE_WEBSERVER
  }
}
/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns106(uint32_t function)
{
  bool result = false;

  if (!I2cEnabled(XI2C_79)) { return false; }

  switch (function) {
    case FUNC_INIT:
      gdk101_init();
      break;
    case FUNC_EVERY_SECOND:
      every_second();
      break;
    case FUNC_JSON_APPEND:
      gdk101_show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      gdk101_show(0);
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_GDK
#endif  // USE_I2C
