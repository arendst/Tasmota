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
#define I2_ADR_IRT 0x18

//I2C register
#define READ_FIRMWARE 0xB4
#define RESET 0xA0

#define READ_STATUS 0xB0
#define READ_MEASURING_TIME 0xB1
#define READ_10MIN_AVG 0xB2 //Read Measuring Value (10min avg / 1min update)
#define READ_1MIN_AVG  0xB3 //Read Measuring Value (1min avg / 1min update)
//gdk101 variables
bool gdk_ready;

float fw_version;
float mea_10min_avg;
float mea_1min_avg;
uint8_t  gdk_status;
bool vibration = false;
uint8_t  mea_time_min;
uint8_t  mea_time_sec;
uint8_t rw_buffer[2] = {0, 0};
uint16_t _rw_buffer;

uint8_t  gdk_resolution = 2;

#ifdef USE_WEBSERVER
const char HTTP_GDK101_SNS[] PROGMEM = "%s"
  "{s}" "GDK101 gamma sensor" "{m}"
  "{s}" "average 10 min" "{m}%0.2f uSv/h{e}"
  "{s}" "average 1 min " "{m}%0.2f uSv/h{e}"
  "{s}" "Status"         "{m}%i{e}"
  "{s}" "Vibration"      "{m}%i{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER


void gdk101_Init() {

  if (gdk_ready) return;
  
  bool ts;
  ts = gdk101_reset();
  if (ts) {
    AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "GDK101", I2_ADR_IRT);
    gdk_ready = true;
  }


}

bool gdk101_reset() {
  bool val;
  gamma_mod_read(RESET); //
  val = rw_buffer[0];
  delay(10);
  return val;
}

void gdk101_update_all() {
  gdk101_get_10min_avg();
  gdk101_get_1min_avg();
  gdk101_get_vib();
  gdk101_get_status();
  gdk101_get_measuring_time_min();
  gdk101_get_measuring_time_sec();
}

float gdk101_get_fw_version() {
  gamma_mod_read(READ_FIRMWARE);
  fw_version = rw_buffer[0] + (float)rw_buffer[1] / 10;
  // return fw_version;
}

float gdk101_get_10min_avg() {
  gamma_mod_read(READ_10MIN_AVG);
  mea_10min_avg = rw_buffer[0] + (float)rw_buffer[1] / 100;
  // return mea_10min_avg;
}

float gdk101_get_1min_avg() {
  gamma_mod_read(READ_1MIN_AVG);
  mea_1min_avg = rw_buffer[0] + (float)rw_buffer[1] / 100;
  //  return mea_1min_avg;
}

bool gdk101_get_vib() {
  gamma_mod_read(READ_STATUS);
  vibration = rw_buffer[1];
  // return vib;
}

uint8_t gdk101_get_status() {
  //0 = Ready, 1 = 10min Waiting, 2 = Normal
  gamma_mod_read(READ_STATUS);
  gdk_status = rw_buffer[0];
  //  return gdk_status;
}

uint8_t gdk101_get_measuring_time_min() {
  gamma_mod_read(READ_MEASURING_TIME);
  mea_time_min = rw_buffer[0];
  //  return mea_time_min;
}

uint8_t gdk101_get_measuring_time_sec() {
  gamma_mod_read(READ_MEASURING_TIME);
  mea_time_sec = rw_buffer[1];
  //  return mea_time_sec;
}

void gamma_mod_read_nw(uint8_t reg) {

  uint8_t _adr = I2_ADR_IRT;
  uint8_t data = 0;
  
  I2cWrite8(_adr, data, reg);
  delay(5);
  rw_buffer[0] =  I2cRead8(_adr, reg);
  rw_buffer[1] =  I2cRead8(_adr, reg);

  //Serial.print(">>> buffer ");
  //Serial.print(rw_buffer[0]);
  //Serial.println(rw_buffer[1]);
 
  delay(10);
  
};

void gamma_mod_read(uint8_t reg) {
  Wire.begin();
  uint8_t lg = 2;
  uint8_t _adr = I2_ADR_IRT;
  Wire.beginTransmission(_adr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(10);
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

void gdk101_Every_Second(void) {
  if (!gdk_ready) return;

      gdk101_update_all();
}

void gdk101_Show(uint8_t json) {
  //dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

  if (!gdk_ready) return;

  char mea_10min_avg_tstr[16];
  dtostrfd(mea_10min_avg, gdk_resolution, mea_10min_avg_tstr);

  char mea_1min_avg_tstr[16];
  dtostrfd(mea_1min_avg, gdk_resolution, mea_1min_avg_tstr);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(",\"GDK101\":{\"10MIN_AVG\":%.2f,\"1MIN_AVG\":%.2f,\"MEA_TIME_MIN\":%i,\"MEA_TIME_SEC\":%i,\"STATUS\":%i,\"VIBRATION\":%i}"),
    mea_10min_avg, mea_1min_avg,mea_time_min,mea_time_sec,gdk_status,vibration);

#ifdef USE_WEBSERVER
  } else {
    //snprintf_P(mqtt_data, sizeof(mqtt_data),"GDK101 gamma sensor");
    //snprintf_P(mqtt_data, sizeof(mqtt_data),"10_min_avg:%0.2fuSv/h", mea_10min_avg);
    //snprintf_P(mqtt_data, sizeof(mqtt_data)," 1 min avg:%0.2fuSv/h",mea_1min_avg); HTTP_MGC_3130_SNS, mqtt_data,
    snprintf_P(mqtt_data, sizeof(mqtt_data),
    HTTP_GDK101_SNS, mqtt_data, mea_10min_avg, mea_1min_avg, gdk_status, vibration );

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
        gdk101_Every_Second();
        break;
      case FUNC_JSON_APPEND:
        gdk101_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        gdk101_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}


#endif  // USE_GDK
#endif  // USE_I2C
