/*

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

#define XSNS_55 55
//I2C register
#define READ_FIRMWARE 0xB4
#define RESET 0xA0
#define READ_STATUS 0xB0
#define READ_MEASURING_TIME 0xB1
#define READ_10MIN_AVG 0xB2 
#define READ_1MIN_AVG  0xB3

//gdk101 variables
bool     gdk_ready;
float    fw_version;
float    mea_10min_avg;
float    mea_1min_avg;
uint8_t  gdk_status;
bool     gdk_vibration = false;
uint8_t  mea_time_min;
uint8_t  mea_time_sec;
uint8_t  rw_buffer[2] = {0, 0};
uint8_t  evr10 = 0;
uint8_t  vibr = 0;

#ifdef USE_WEBSERVER
const char HTTP_GDK101_SNS[] PROGMEM = "%s"
                                       "{s}" "GDK101 Average 10 min" "{m}%0.2f uSv/h{e}"
                                       "{s}" "GDK101 Average 1 min " "{m}%0.2f uSv/h{e}"
                                       "{s}" "GDK101 Status"         "{m}%i{e}"
                                       "{s}" "GDK101 VIBRATION"      "{m}%i{e}";
                                     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER


void gdk101_Init() {

  if (gdk_ready) return;

  bool ts;
  ts = gdk101_reset();
  if (ts) {
    AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "GDK101", USE_GDK101_ADDR);
    gdk_ready = true;
  }

}

bool gdk101_reset() {
  bool val;
  gamma_mod_read(RESET); //
  val = rw_buffer[0];
  return val;
}

void gdk101_update_all() {
  gdk101_get_10min_avg();
  gdk101_get_1min_avg();
  gdk101_get_vib();
  gdk101_get_status();
//  gdk101_get_measuring_time_min();
//  gdk101_get_measuring_time_sec();
}

float gdk101_get_fw_version() {
  gamma_mod_read(READ_FIRMWARE);
  fw_version = rw_buffer[0] + (float)rw_buffer[1] / 10;
}

float gdk101_get_10min_avg() {
  gamma_mod_read(READ_10MIN_AVG);
  mea_10min_avg = rw_buffer[0] + (float)rw_buffer[1] / 100;
}

float gdk101_get_1min_avg() {
  gamma_mod_read(READ_1MIN_AVG);
  mea_1min_avg = rw_buffer[0] + (float)rw_buffer[1] / 100;
}

bool gdk101_get_vib() {
  gamma_mod_read(READ_STATUS);
  if(rw_buffer[1] == 1) gdk_vibration = true;
  else gdk_vibration = false;
}

uint8_t gdk101_get_status() {
  //0 = Ready, 1 = 10min Waiting, 2 = Normal
  gamma_mod_read(READ_STATUS);
  gdk_status = rw_buffer[0];
}

uint8_t gdk101_get_measuring_time_min() {
  gamma_mod_read(READ_MEASURING_TIME);
  mea_time_min = rw_buffer[0];
}

uint8_t gdk101_get_measuring_time_sec() {
  gamma_mod_read(READ_MEASURING_TIME);
  mea_time_sec = rw_buffer[1];
}

void gamma_mod_read(uint8_t reg) {
  Wire.begin();
  uint8_t lg = 2;
  uint8_t _adr = USE_GDK101_ADDR;
  Wire.beginTransmission(_adr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(_adr, lg);
  uint8_t i = 0;
  while (Wire.available())
  {
    rw_buffer[i] = Wire.read();
    i++;
  }
  delay(10);
};

/*********************************************************************************************\
   every sec show...
  \*********************************************************************************************/

void Every_Second(void) {
  if (!gdk_ready) return;
  
  gdk101_Every_Second();
  
  if (evr10 == 10) {
    gdk101_Every_10_Second();
    evr10 = 1;
  }
  else {
    evr10++;
  }

}

void gdk101_Every_Second(void) {
  gdk101_get_vib();
  gdk101_get_status();
  if (gdk_vibration) vibr++;

  if (vibr == 3){
    gdk101_reset();
    delay(10);
    vibr = 0;
  }
  
//  gdk101_get_measuring_time_sec();
}

void gdk101_Every_10_Second(void) {
  gdk101_get_10min_avg();
  gdk101_get_1min_avg();
//  gdk101_get_measuring_time_min();
}

void gdk101_Show(uint8_t json) {
  if (!gdk_ready) return;
  
  if (json) {
    ResponseAppend_P(PSTR(",\"GDK101\":{\"10MIN_AVG\":%.3f,\"1MIN_AVG\":%.3f,\"STATUS\":%i,\"VIBRATION\":%i,\"RAD_UNIT\":\"uSv/h\"}"),
               mea_10min_avg, mea_1min_avg, gdk_status, gdk_vibration);

#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_GDK101_SNS, mqtt_data, mea_10min_avg, mea_1min_avg, gdk_status, gdk_vibration);

#endif  // USE_WEBSERVER
  }
}
/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns55(byte function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        gdk101_Init();
        break;
      case FUNC_EVERY_SECOND:
        Every_Second();
        break;
      case FUNC_JSON_APPEND:
        gdk101_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        gdk101_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}


#endif  // USE_GDK
#endif  // USE_I2C
