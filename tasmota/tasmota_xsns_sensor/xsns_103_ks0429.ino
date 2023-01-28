/*
  xsns_103_ks0429.ino - KS0429 TDS sensor support for Tasmota

  Copyright (C) 2022  Mickael Gaillard

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

/* Base on :
  - https://wiki.keyestudio.com/KS0429_keyestudio_TDS_Meter_V1.0
  - https://randomnerdtutorials.com/esp32-tds-water-quality-sensor/
  - 
*/

#ifdef USE_ADC
#ifdef USE_KS0429
/*********************************************************************************************\
 * KS0429 - DTS - sensors
\*********************************************************************************************/

#define XSNS_103          103

#define TREF 25.0             // Temperature reference (°C)
#define VREF 3.3              // analog reference voltage(Volt) of the ADC (3.3V for ESP32 or 5V for Arduino)
#define ECREF 4095.0          // ADC resolution (4096 for ESP32 or 1025 for Arduino)
#define SCOUNT  30            // sum of sample point

struct {
  int8_t pin = 0;            // 
  int analogBuffer[SCOUNT];     // store the analog value in the array, read from ADC
  int analogBufferIndex = -1;
  uint16_t averageRaw = 0;
  float averageVoltage = 0;
  float compensationVoltage = 0;
  float callibrateVoltage = 0;
  float tdsValue = 0;
} KS0429Data;

float temperature = 25;       // current temperature for compensation


/** 
 * @brief Median filtering algorithm.
 */
int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
    bTab[i] = bArray[i];

  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }

  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }

  return bTemp;
}

/**
 * @brief Initialize KS0429 Driver
 * 
 */
void Ks0429Init(void) {
  if (PinUsed(GPIO_KS0429)) {
    KS0429Data.pin = Pin(GPIO_KS0429);

#ifdef ESP32
    analogSetClockDiv(1);               // Default 1

#if CONFIG_IDF_TARGET_ESP32
    analogSetWidth(ANALOG_RESOLUTION);  // Default 12 bits (0 - 4095)
#endif  // CONFIG_IDF_TARGET_ESP32

    analogSetAttenuation(ADC_11db);     // Default 11db
#endif

    pinMode(KS0429Data.pin, INPUT);

#ifdef USE_DEBUG_DRIVER
    AddLog(LOG_LEVEL_DEBUG, PSTR("DRV: %s  GPIO: %x"), "KS-0429", KS0429Data.pin);
#endif  // USE_DEBUG_DRIVER

  }
}

/**
 * @brief Read value from ADC to buffer.
 * 
 */
void Ks0429Fetch(void) {
  if (KS0429Data.pin > 0) {
    uint16_t value = analogRead(KS0429Data.pin);    //read the analog value and store into the buffer

  #ifdef USE_DEBUG_DRIVER
    //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Read value [%d ] : %d"), KS0429Data.analogBufferIndex, KS0429Data.analogBuffer[KS0429Data.analogBufferIndex]);
  #endif  // USE_DEBUG_DRIVER

    if (KS0429Data.analogBufferIndex == -1) {
      for (int i = 0; i < SCOUNT; i ++)
        KS0429Data.analogBuffer[i] = value;
    } else {
      KS0429Data.analogBuffer[KS0429Data.analogBufferIndex] = value;
    }

    KS0429Data.analogBufferIndex++;
    if(KS0429Data.analogBufferIndex == SCOUNT){ 
      KS0429Data.analogBufferIndex = 0;
    }
  }
}

/**
 * @brief 
 * 
 */
void Ks0429Extract(void) {
  if (KS0429Data.pin > 0) {
    // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    KS0429Data.averageRaw = getMedianNum(KS0429Data.analogBuffer, SCOUNT);
    KS0429Data.averageVoltage =  KS0429Data.averageRaw * (float)VREF / (float)ECREF;
    KS0429Data.callibrateVoltage = KS0429Data.averageVoltage; // * (41.0 / 40.0) + (271.0 / 2000.0);

    // Compensations
    if (TasmotaGlobal.temperature_celsius != NAN) {
      temperature = TasmotaGlobal.temperature_celsius;
    } else {
      temperature = 25;
    }
    float compensationCoefficient = 1.0 + 0.0185 * (temperature - (float)TREF);
    KS0429Data.compensationVoltage = KS0429Data.callibrateVoltage / compensationCoefficient;
    
    // Convert voltage value to TDS value
    KS0429Data.tdsValue=(133.42 * pow(KS0429Data.compensationVoltage, 3) - 255.86 * pow(KS0429Data.compensationVoltage, 2) + 857.39 * KS0429Data.compensationVoltage) * 0.5;

#ifdef USE_DEBUG_DRIVER
    // Serial.print("voltage:");
    // Serial.print(KS0429Data.averageVoltage,2);
    // Serial.print("V => ");
    // Serial.print(KS0429Data.compensationVoltage,2);
    // Serial.println("V");
    // Serial.print("TDS Value:");
    // Serial.print(KS0429Data.tdsValue,0);
    // Serial.println("ppm");
#endif  // USE_DEBUG_DRIVER
  }
}

/**
 * @brief 
 * 
 */
void Ks0429Show(void) {
  if (KS0429Data.pin > 0) {
    char strTds[FLOATSZ] = { 0 };
    dtostrfd(KS0429Data.tdsValue, 2, strTds);

    char strCompVoltage[FLOATSZ] = { 0 };
    dtostrfd(KS0429Data.compensationVoltage, 2, strCompVoltage);

    char strCalibrateVoltage[FLOATSZ] = { 0 };
    dtostrfd(KS0429Data.callibrateVoltage, 2, strCalibrateVoltage);

    ResponseAppend_P(PSTR(", \"KS0429\": { \"voltage\":%s, \"ppm\":%s}"), strCompVoltage, strTds);

  #ifdef USE_WEBSERVER
    WSContentSend_PD(HTTP_SNS_TDS, "", strTds);
  #endif  // USE_WEBSERVER

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Analog TDS read. ADC-RAW: %d, voltage:%sV (raw:%sV), ppm:%s)"), KS0429Data.averageRaw, strCompVoltage, strCalibrateVoltage, strTds);
  }
}

boolean Xsns103(uint32_t function) {
  boolean result = false;

  // Check which callback ID is called by Tasmota
  switch (function) {
    case FUNC_COMMAND:
      result = true;
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      break;
    case FUNC_INIT:
      Ks0429Init();
      break;
    default:
      if (Adcs.present) {
        switch (function) {
          case FUNC_EVERY_50_MSECOND:
          //case FUNC_EVERY_250_MSECOND:
            Ks0429Fetch();
            break;
          case FUNC_EVERY_SECOND:
            Ks0429Extract();
            break;
          case FUNC_JSON_APPEND:
            Ks0429Show();
            break;
      #ifdef USE_WEBSERVER
          case FUNC_WEB_SENSOR:
            Ks0429Show();
            break;
      #endif // USE_WEBSERVER
        }
      }
  }

  return result;
}

#endif  // USE_KS0429
#endif  // USE_ADC
