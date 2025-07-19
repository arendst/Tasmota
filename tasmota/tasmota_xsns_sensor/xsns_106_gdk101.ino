/*
  xsns_106_gdk101.ino - Support for GDK101 gamma radiation sensor

  Copyright (C) 2019  Petr Novacek

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
 * For background information see http://allsmartlab.com/eng/294-2/ and AN_GDK101_V1.1_I2C.pdf
 *
 * I2C Address:
 * A0 Short, A1 Short : 0x18
 * A0 Open,  A1 Short : 0x19
 * A0 Short, A1 Open  : 0x1A
 * A0 Open,  A1 Open  : 0x1B
\*********************************************************************************************/

#define XSNS_106                    106

#define XI2C_79                     79  // See I2CDEVICES.md

// I2C adresses
#define GDK101_ADDRESS1             0x18
#define GDK101_ADDRESS2             0x19
#define GDK101_ADDRESS3             0x1A
#define GDK101_ADDRESS4             0x1B

// I2C register
#define GDK101_RESET                0xA0
#define GDK101_READ_STATUS          0xB0
#define GDK101_READ_MEASURING_TIME  0xB1
#define GDK101_READ_10MIN_AVG       0xB2
#define GDK101_READ_1MIN_AVG        0xB3
#define GDK101_READ_FIRMWARE        0xB4

// gdk101 variables
uint8_t gdk101_addresses[] = { GDK101_ADDRESS1, GDK101_ADDRESS2, GDK101_ADDRESS3, GDK101_ADDRESS4 };

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

struct GDK {
  uint32_t time;
//  gdk_measuring_time_t time = {0, 0};
  gdk_fW_version_t version;
  gdk_avg_dose_t ard_10min;
  gdk_avg_dose_t ard_1min;
  gdk_status_data_t status = {GDK_STATUS_INVALID, false};
  uint8_t address;
  uint8_t evr10 = 0;
  bool ready = false;
} Gdk;

/********************************************************************************************/

void Gdk101ErrLog(bool ret) {
  if (!ret) {
     DEBUG_SENSOR_LOG("GDK101: I2C read error");
  }
}

void Gdk101Detect(void) {
  for (uint32_t i = 0; i < sizeof(gdk101_addresses); i++) {
    Gdk.address = gdk101_addresses[i];
    if (!I2cSetDevice(Gdk.address)) {
      continue;  // Do not make the next step without a confirmed device on the bus
    }
    bool reset = false;
    bool ret = Gdk101Reset(&reset);
    if (ret) {
      if (reset) {
        delay(10);
        I2cSetActiveFound(Gdk.address, "GDK101");
        ret = Gdk101Firmware(&Gdk.version);
        Gdk101ErrLog(ret);
        Gdk.ready = true;
        break;
      }
    }
  }
}

bool Gdk101Reset(bool *reset) {
  uint8_t buf;
  bool ret = I2cValidRead8(&buf, Gdk.address, GDK101_RESET);
  if (ret) {
    *reset = (bool) buf;  // 0 = Reset fail, 1 = Reset success
  }
  return ret;
}

bool Gdk101Firmware(struct gdk_fW_version *fw) {
  uint8_t buf[2];
  bool ret = I2cValidRead16LE((uint16_t*) buf, Gdk.address, GDK101_READ_FIRMWARE);
  if (ret) {
    fw->major = buf[0];  // 0
    fw->minor = buf[1];  // 6
  }
  return ret;
}

bool Gdk101Ard10min(struct gdk_avg_dose *avg) {
  uint8_t buf[2];
  bool ret = I2cValidRead16LE((uint16_t*) buf, Gdk.address, GDK101_READ_10MIN_AVG);
  if (ret) {
    avg->integral = buf[0];
    avg->fractional = buf[1];
  }
  return ret;
}

bool Gdk101Ard1min(struct gdk_avg_dose *avg) {
  uint8_t buf[2];
  bool ret = I2cValidRead16LE((uint16_t*) buf, Gdk.address, GDK101_READ_1MIN_AVG);
  if (ret) {
    avg->integral = buf[0];
    avg->fractional = buf[1];
  }
  return ret;
}

bool Gdk101Status(struct gdk_status_data *status) {
  uint8_t buf[2];
  bool ret = I2cValidRead16LE((uint16_t*) buf, Gdk.address, GDK101_READ_STATUS);
  if (ret) {
    status->status = (gdk_status_t) buf[0];  // 0 = Ready, 1 = 10min Waiting, 2 = Normal
    status->vibration = (bool) buf[1];       // 0 = Off, 1 = On
  }
  return ret;
}

bool Gdk101MeasuringTime(struct gdk_measuring_time *meas_time) {
  // Can return 255 minutes and 60 seconds - Not used in Application Note AN_GDK101_V1.1
  uint8_t buf[2];
  bool ret = I2cValidRead16LE((uint16_t*) buf, Gdk.address, GDK101_READ_MEASURING_TIME);
  if (ret) {
    meas_time->min = buf[0];
    meas_time->sec = buf[1];
  }
  return ret;
}

void Gdk101EverySecond(void) {
  bool ret = Gdk101Status(&Gdk.status);
  Gdk101ErrLog(ret);
  if (Gdk.status.status > GDK_STATUS_READY) {
    Gdk.time++;
  } else {
    Gdk.time = 0;
  }

  Gdk.evr10++;
  if (10 == Gdk.evr10) {
    Gdk.evr10 = 0;

    ret = Gdk101Ard10min(&Gdk.ard_10min);
    Gdk101ErrLog(ret);
    ret = Gdk101Ard1min(&Gdk.ard_1min);
    Gdk101ErrLog(ret);
//    ret = Gdk101MeasuringTime(&Gdk.time);
//    Gdk101ErrLog(ret);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_GDK101_FW[] PROGMEM = "{s}%s " "FW Version" "{m}%d.%d{e}";
const char HTTP_GDK101_STATUS[] PROGMEM = "{s}%s " "Status" "{m}%d{e}";
const char HTTP_GDK101_VIB_STATUS[] PROGMEM = "{s}%s " "Vibration Status" "{m}%d{e}";
//const char HTTP_GDK101_MEAS_TIME[] PROGMEM = "{s}%s " "Measurement Time" "{m}%d:%02d{e}";
const char HTTP_GDK101_MEAS_TIME[] PROGMEM = "{s}%s " "Measurement" "{m}%s{e}";
#endif  // USE_WEBSERVER


void Gdk101Show(uint8_t json) {
  char types[16];
  strcpy_P(types, PSTR("GDK101"));

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{"), types);
    ResponseAppend_P(PSTR("\"Firmware\":%i.%i,"), Gdk.version.major, Gdk.version.minor);
    ResponseAppend_P(PSTR("\"RadiationAvg10Min\":%i.%02i,"), Gdk.ard_10min.integral, Gdk.ard_10min.fractional);
    ResponseAppend_P(PSTR("\"RadiationAvg1Min\":%i.%02i,"), Gdk.ard_1min.integral, Gdk.ard_1min.fractional);
    ResponseAppend_P(PSTR("\"Status\":%i,"), Gdk.status.status);
    ResponseAppend_P(PSTR("\"Vibration\":%i,"), Gdk.status.vibration);
//    ResponseAppend_P(PSTR("\"MeasTime\":\"%i:%02i\""), Gdk.time.min, Gdk.time.sec);
    ResponseAppend_P(PSTR("\"Measurement\":\"%s\""), GetDuration(Gdk.time).c_str());
    ResponseJsonEnd();
#ifdef USE_WEBSERVER
  } else {
#ifdef GDK101_SHOW_FW_VERSION
    WSContentSend_PD(HTTP_GDK101_FW, types, Gdk.version.major, Gdk.version.minor);
#endif
    WSContentSend_PD(HTTP_SNS_AVG_RAD_DOSE, types, "10", Gdk.ard_10min.integral, Gdk.ard_10min.fractional);
    WSContentSend_PD(HTTP_SNS_AVG_RAD_DOSE, types, "1", Gdk.ard_1min.integral, Gdk.ard_1min.fractional);
#ifdef GDK101_SHOW_STATUS
    WSContentSend_PD(HTTP_GDK101_STATUS, types, Gdk.status.status);
#endif
#ifdef GDK101_SHOW_VIBRATION_STATUS
    WSContentSend_PD(HTTP_GDK101_VIB_STATUS, types, Gdk.status.vibration);
#endif
//#ifdef GDK101_SHOW_MEAS_TIME
//    WSContentSend_PD(HTTP_GDK101_MEAS_TIME, types, Gdk.time.min, Gdk.time.sec);
    WSContentSend_PD(HTTP_GDK101_MEAS_TIME, types, GetDuration(Gdk.time).c_str());
//#endif
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns106(uint32_t function) {
  if (!I2cEnabled(XI2C_79)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Gdk101Detect();
  }
  else if (Gdk.ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Gdk101EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Gdk101Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Gdk101Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_GDK
#endif  // USE_I2C
