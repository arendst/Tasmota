/*
  xsns_86_tfminiplus.ino - TFmini Plus interface for Tasmota

  Copyright (C) 2021  Raphael Breiting and Theo Arends

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

#ifdef USE_TFMINIPLUS
/*****************************************************************************\
 * TFmini, TFmini Plus, TFmini Plus (Indoor Version), TFmini-S - LiDAR Module
 * Manufacturer: Benewake (Beijing) Co. Ltd.
 *
 * Code for Time-Of-Flight (TOF) family single-point short-range LiDAR sensor
 * modules with UART interface.
 *
 * Connection Description for GH1.25-4P (Molex51021-0400) connector:
 *      No     Color       Corresponding PIN   Function    Comment
 *      1       RED         PIN-1               +5V         Power Supply
 *      2       WHITE       PIN-2               RxD/SDA     Receiving/Data
 *      3       GREEN       PIN-3               TxD/SCL     Transmitting/Clock
 *      4       BLACK       PIN-4               GND         Ground
 *
 * Before connecting module to Tasmota, please prepare module with
 * the following sequence through terminal program (e.g. Termite):
 * 1. Connect sensor to USB-to-UART (TTL 3.3V)
 * 2. Set terminal to 115200bps and 8N1
 * 3. Set frame rate to 0 (zero):
 *      send -->: 5A0603000000
 *      receive <-- (OK): 5A0603000000
 * 4. Set baud rate to 9600bps
 *      send -->: 5A0806802500000D
 *      receive <-- (OK): 5A0806802500000D
 * 5. Set terminal to 9600bps and 8N1
 * 6. Save settings
 *      send -->: 5A04116F
 *      receive <-- (OK): 5A05110070
 *      receive <-- (FAIL): 5A05110071
 * 7. When point 6 returned OK, than ready to connect to Tasmota! :)
 *    When point 6 returned FAIL, start with item 1 and
 *      - check connection of RX and TX pin are not exchanged
 *      - check power supply
 *          - 5V +-0,5V
 *          - 110mA average
 *          - 140mA max peak
 *
 * References:
 * - http://en.benewake.com/product
 * - https://de.aliexpress.com/item/32852024277.html?spm=a2g0s.9042311.0.0.27424c4d5Edizk
 * - https://de.aliexpress.com/item/4001076614996.html?spm=a2g0s.9042311.0.0.27424c4d5Edizk
\*****************************************************************************/

#define XSNS_86 86

// Default baudrate
#define TFMP_SPEED 9600

// Serial buffer length for incoming data
#define TFMP_MAX_DATA_LEN 9

#include <TasmotaSerial.h>

char Tfmp_buffer[TFMP_MAX_DATA_LEN + 1];

struct xsns_86_tfminiplus
{
    bool ready = false;
    uint16_t distance = 0;
    uint16_t sigstrength = 0;
    uint16_t chiptemp = 0;
} tfminiplus_sensor;

// Software and hardware serial pointers
TasmotaSerial *TfmpSerial = nullptr;

void TfmpInit(void)
{
    if (!tfminiplus_sensor.ready)
    {
        if (PinUsed(GPIO_TFMINIPLUS_RX) && PinUsed(GPIO_TFMINIPLUS_TX))
        {
            TfmpSerial = new TasmotaSerial(Pin(GPIO_TFMINIPLUS_RX), Pin(GPIO_TFMINIPLUS_TX), 1);
            if (TfmpSerial->begin(TFMP_SPEED))
            {
                if (TfmpSerial->hardwareSerial())
                {
                    ClaimSerial();
                }
                tfminiplus_sensor.ready = true;
                TfmpSerial->flush();
            }
        }
    }
}

void TfmpTrigger(void)
{
    if (TfmpSerial && tfminiplus_sensor.ready)
    {
        TfmpSerial->write(0x5A);
        TfmpSerial->write(0x04);
        TfmpSerial->write(0x04);
        TfmpSerial->write(0x62);
    }
}

void TfmpProcessData(void) {
    // check crc sum
    uint16_t crc = 0;
    for (int i = 0; i < TFMP_MAX_DATA_LEN - 1; ++i) {
        crc += (uint16_t)Tfmp_buffer[i];
    }

    if ((char)(crc & 0xff) == Tfmp_buffer[TFMP_MAX_DATA_LEN-1]) {
        // distance to object (default in cm)
        tfminiplus_sensor.distance = (Tfmp_buffer[3] << 8) + Tfmp_buffer[2];
        // signal strength (between 100 and 65535 fine, otherwise 0 due to out of range of non-reflective surface)
        tfminiplus_sensor.sigstrength = (Tfmp_buffer[5] << 8) + Tfmp_buffer[4];
        // chip temperature
        tfminiplus_sensor.chiptemp = (((Tfmp_buffer[7] << 8) + Tfmp_buffer[6]) >> 3) - 256;

        DEBUG_SENSOR_LOG(PSTR("TFmini Plus: Distance: %d"), tfminiplus_sensor.distance);
        DEBUG_SENSOR_LOG(PSTR("TFmini Plus: Signal: %d"), tfminiplus_sensor.sigstrength);
        DEBUG_SENSOR_LOG(PSTR("TFmini Plus: Chip Temp: %d"), tfminiplus_sensor.chiptemp);
    } else {
        DEBUG_SENSOR_LOG(PSTR("TFmini Plus: crc error"));
    }
    TfmpSerial->flush();
}

void TfmpProcessSerialData (void)
{
    uint8_t data;
    bool dataReady;
    if (TfmpSerial && tfminiplus_sensor.ready)
    {
        while (TfmpSerial->available() > 0)
        {
            data = TfmpSerial->read();
            dataReady = TfmpAddData((char)data);
            if (dataReady)
            {
                TfmpProcessData();
            }
        }
    }
}

bool TfmpAddData(char nextChar)
{
    // Buffer position
    static uint8_t currentIndex = 0;
    // Store data into buffer at position
    if ((currentIndex >0) && (0x59 == Tfmp_buffer[currentIndex-1]) && (0x59 == nextChar))
    {
        currentIndex = 1;
    }
    Tfmp_buffer[currentIndex] = nextChar;
    currentIndex++;
    // Check for too many data
    if (currentIndex > TFMP_MAX_DATA_LEN)
    {
        // Terminate buffer and reset position
        Tfmp_buffer[TFMP_MAX_DATA_LEN] = '\0';
        currentIndex = 0;
        return true;
    }
    return false;
}

#ifdef USE_WEBSERVER
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_SIGNALSTRENGTH[] PROGMEM = "{s}%s " D_SIGNALSTRENGTH "{m}%d{e}";
const char HTTP_SNS_CHIPTEMPERATURE[] PROGMEM = "{s}%s " D_CHIPTEMPERATURE "{m}%d " D_UNIT_DEGREE "%c{e}";
#endif  // USE_WEBSERVER

void TfmpShow(bool json) {
    char sensor_name[12];
    strcpy_P(sensor_name, "TFminiPlus");
    float distance = (float)tfminiplus_sensor.distance;  // cm

    if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f,\"" D_JSON_SIGNALSTRENGTH "\":%d,\"" D_JSON_CHIPTEMPERATURE "\":%d}"),
            sensor_name, &distance, tfminiplus_sensor.sigstrength, tfminiplus_sensor.chiptemp);
#ifdef USE_DOMOTICZ
        if (0 == TasmotaGlobal.tele_period) {
            DomoticzFloatSensor(DZ_COUNT, distance);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
        WSContentSend_P(HTTP_SNS_F_DISTANCE_CM, sensor_name, &distance);
        WSContentSend_P(HTTP_SNS_SIGNALSTRENGTH, sensor_name, tfminiplus_sensor.sigstrength);
        WSContentSend_P(HTTP_SNS_CHIPTEMPERATURE, sensor_name, tfminiplus_sensor.chiptemp, TempUnit());
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns86(uint32_t callback_id)
{
    bool result = false;
    if (FUNC_INIT == callback_id)
    {
        TfmpInit();
    }
    else if (TfmpSerial && tfminiplus_sensor.ready)
    {
        switch (callback_id) {
            case FUNC_EVERY_SECOND:
                TfmpTrigger();
                TfmpProcessSerialData();
                result = true;
                break;
            case FUNC_JSON_APPEND:
                TfmpShow(1);
                break;
#ifdef USE_WEBSERVER
            case FUNC_WEB_SENSOR:
                TfmpShow(0);
                break;
#endif  // USE_WEBSERVER
            case FUNC_SAVE_BEFORE_RESTART:
                break;
            case FUNC_COMMAND:
                break;
        }
    }
    return result;
}
#endif  // USE_TFMINIPLUS
