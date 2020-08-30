/*
  xsns_78_mlx90640.ino - MLX90640 I2C Interface

  Copyright (C) 2020 Balaji Srinivasaraghavan

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
#ifdef ESP32
#ifdef USE_MLX90640

#if MQTT_ENABLE
#include <PubSubClient.h>
// --------------------------------------------------------------------
// * * * IMPORTANT * * *
// You must change <PubSubClient.h> to have the following value.
// #define MQTT_MAX_PACKET_SIZE 5700
// --------------------------------------------------------------------
// Check that the user has set MQTT_MAX_PACKET_SIZE to an appropriate size.
#if MQTT_MAX_PACKET_SIZE < 5700
#error \
    "MQTT_MAX_PACKET_SIZE in <PubSubClient.h> is too small. "\
  "Increase the value per comments."
#endif  // MQTT_MAX_PACKET_SIZE < 5700
#endif  // MQTT_ENABLE

/*********************************************************************************************\
 * MLX9064X Far Infrared Thermal Sensor Array
\*********************************************************************************************/
#include <Adafruit_MLX90640.h>

#define XSNS_78 78

Adafruit_MLX90640 mlx;
uint8_t mlx_ready;
float frame[32 * 24];  // buffer for full frame of temperatures
uint8_t elasped_sec = 0;
char result[SENSOR_VAL_STR_LEN];  //[-100.1,] , no comma for 1 noumber
char max_temp_str[6];

const uint8_t TRIM_PIXEL_COLUMNS_CNT = 0;        // image crop. if 2, 2 layers of the outermost pixels are discarded
const uint8_t POLL_INTERVAL_SEC = 5;             //data refresh rate esp for UI. mqtt reporting is teleperiod
const int SENSOR_VAL_STR_LEN = 24 * 32 * 7 + 2;  // chars + [] - one comma + last null ch

void MLX90640_Init() {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("MLX90640: Start of driver init"));
    if (!mlx.begin()) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("MLX90640: Sensor not found"));
        return;
    }

    mlx.setMode(MLX90640_CHESS);
    if (mlx.getMode() != MLX90640_CHESS) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("MLX90640: Sensor did not respond correctly. Driver not initialized"));
        return;
    }
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx.setRefreshRate(MLX90640_1_HZ);
    // case MLX90640_0_5_HZ: Serial.println("0.5 Hz"); break;
    // case MLX90640_1_HZ: Serial.println("1 Hz"); break;
    // case MLX90640_2_HZ: Serial.println("2 Hz"); break;
    // case MLX90640_4_HZ: Serial.println("4 Hz"); break;
    // case MLX90640_8_HZ: Serial.println("8 Hz"); break;
    // case MLX90640_16_HZ: Serial.println("16 Hz"); break;
    mlx_ready = 1;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("MLX90640: Sensor ready"));
}

void MLX90640_Every_Second(void) {
    if (++elasped_sec % POLL_INTERVAL_SEC != 0 || !mlx_ready) return;

    if (mlx.getFrame(frame) != 0) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("MLX90640: Unable to query sensor for temperatures"));
        return;
    }
    delay(0);

    int output_str_index = 0;
    result[output_str_index++] = PSTR('[');
    char value_as_str[6];
    float temp;
    int location;
    float max_temp_so_far = -9999.0;
    for (uint8_t h = 0 + TRIM_PIXEL_COLUMNS_CNT / 2; h < 24 - TRIM_PIXEL_COLUMNS_CNT / 2; h++) {
        for (uint8_t w = 0 + TRIM_PIXEL_COLUMNS_CNT / 2; w < 32 - TRIM_PIXEL_COLUMNS_CNT / 2; w++) {
            location = h * 32 + w;
            temp = frame[location];
            dtostrf((temp * 1.8) + 32, 1, 1, value_as_str);  //1 = min string len, 1 = decimal precision
            if (temp > max_temp_so_far) {
                max_temp_so_far = temp;
                strcpy(max_temp_str, value_as_str);
            }
            for (int str_pos = 0; str_pos < 6 && value_as_str[str_pos] != 0; str_pos++) {  //we dont know the length of value_as_str
                if (value_as_str[str_pos] == PSTR(' ')) continue;
                result[output_str_index++] = value_as_str[str_pos];
            }
            if (h < 23 || (h == 23 && w != 31)) result[output_str_index++] = PSTR(',');
        }
        delay(0);
    }
    result[output_str_index++] = PSTR(']');
    result[output_str_index++] = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_IRMSG[] PROGMEM =
    "{s}MXL90640 "
    "Max Temperature"
    "{m}%s F"
    "{e}";
#endif

void MLX90640_Show(uint8_t json) {
    if (!mlx_ready) return;

    if (json) {
        ResponseAppend_P(PSTR(",\"MLX90640\":{\"max_temp\":\"%s F\", \"temperatures\":%s}"), max_temp_str, result);
        MqttPublishPrefixTopic_P(TELE, mqtt_data);
    }
#ifdef USE_WEBSERVER
    else {
        WSContentSend_PD(HTTP_IRMSG, max_temp_str);
    }
#endif
}

/*********************************************************************************************\
* Interface
\*********************************************************************************************/

bool Xsns78(byte function) {
    if (i2c_flg) {
        long stopwatch_ms = millis();
        switch (function) {
            case FUNC_INIT: {
                MLX90640_Init();
                stopwatch_ms = millis() - stopwatch_ms;
                String msg = String("MLX90640: Sensor init took ");
                msg += stopwatch_ms;
                msg += String(" ms");
                AddLog_P(LOG_LEVEL_DEBUG, msg.c_str());
                break;
            }
            case FUNC_EVERY_SECOND: {
                MLX90640_Every_Second();
                stopwatch_ms = millis() - stopwatch_ms;
                if (stopwatch_ms > 1) {
                    String msg = String("MLX90640: Sensor poll took ");
                    msg += stopwatch_ms;
                    msg += String(" ms");
                    AddLog_P(LOG_LEVEL_DEBUG, msg.c_str());
                }
                break;
            }
            case FUNC_JSON_APPEND:
                MLX90640_Show(1);
                break;
#ifdef USE_WEBSERVER
            case FUNC_WEB_SENSOR:
                MLX90640_Show(0);
                break;
#endif  // USE_WEBSERVER
        }
    }
    // AddLog_P(LOG_LEVEL_DEBUG, (PSTR("MLX90640: Xsns77 ended")));

    return false;
}

#endif  // USE_MLX90640
#endif  // ESP32
