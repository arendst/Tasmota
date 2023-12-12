/*
  xdrv_42_0_i2s_2_dacbuffer_idf51.ino - Simplified Audio library, core class

  Copyright (C) 2021  Gerhard Mutz, Theo Arends, Staars, Stephan Hadinger

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

#if defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
#ifdef USE_I2S_AUDIO
#ifdef SOC_DAC_SUPPORTED

#include "driver/dac_continuous.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/dac_continuous.h"

// #define DAC_BUFFER_SIZE (960 * 2)
#define DAC_BUFFER_SIZE (240 * 2)

SemaphoreHandle_t   dac_buffer_Mutex;
size_t              dac_buffer_len = 0;
TaskHandle_t        dac_task_handle = NULL;
dac_continuous_handle_t   dac_handle = NULL;
uint8_t            *dac_buffer = NULL;

int32_t dac_submit_buffer(uint8_t* data, size_t length) {
static int cnt = 0;
// if (cnt++ < 10) { AddLog(LOG_LEVEL_DEBUG, "dac_submit_buffer: %*_H", length, data); }
  // send to dac audio
  size_t i2s_bytes_written = 0;
  if (length == 0) { return 0; }

// Serial.printf("%i\n", length);
  esp_err_t err = dac_continuous_write(dac_handle, data, length, &i2s_bytes_written, 0);
  if (err && err != ESP_ERR_TIMEOUT) {
    // AddLog(LOG_LEVEL_INFO, "I2S: Could not write samples (count=%i): %i", length, err);
    return -1;
  } else if (err == ESP_OK) {
    // AddLog(LOG_LEVEL_DEBUG, "I2S: dac_continuous_write bytes_written=%i buf=%*_H", i2s_bytes_written, length, data);
  }


  // Submit buffer and return bytes consumed 
  return i2s_bytes_written;
}

void dac_buffer_filling_task(void*) {
  size_t bytesCopied;
  while (true) {
    // Wait for data
    if (dac_buffer_Mutex != NULL) {
      xSemaphoreTake(dac_buffer_Mutex, portMAX_DELAY);
      // bytesCopied = fillBuffer(data, length);
      if (dac_buffer_len > 0) {    
        int32_t bytes_consumed = dac_submit_buffer(dac_buffer, dac_buffer_len);
        if (bytes_consumed > 0) {
          if (bytes_consumed >= dac_buffer_len) {
            dac_buffer_len = 0;
          } else {
            dac_buffer_len -= bytes_consumed;
            memmove(dac_buffer, dac_buffer + bytes_consumed, dac_buffer_len);       // shift what's left
          }
        }
      }
      xSemaphoreGive(dac_buffer_Mutex);
    }

    // Wait 1ms
    vTaskDelay(1); 
  }
}

int32_t send_dac_data(uint8_t* buf, size_t len_in_bytes) {
// static int cnt = 11;
// cnt++;
// if (cnt < 10) { AddLog(LOG_LEVEL_DEBUG, "send_dac_data: %*_H", len_in_bytes, buf); }
  if (dac_buffer_Mutex != NULL && len_in_bytes > 0 && buf != NULL) {
    xSemaphoreTake(dac_buffer_Mutex, portMAX_DELAY);
// if (cnt < 10) { AddLog(LOG_LEVEL_DEBUG, "mutex gained"); }

    // Calculate available space
    int32_t space_left = DAC_BUFFER_SIZE - dac_buffer_len;
    int32_t bytes_sent = 0;

    if (space_left > 0) {
      if (len_in_bytes > space_left) {
        // send only the portion to fill the buffer
        bytes_sent = space_left;
      } else {
        // send all of it
        bytes_sent = len_in_bytes;
      }
// if (cnt < 10) { AddLog(LOG_LEVEL_DEBUG, "memmove buf:%p from:%p len:%i", dac_buffer + dac_buffer_len, buf, bytes_sent); }
      memmove(dac_buffer + dac_buffer_len, buf, bytes_sent);
      dac_buffer_len += bytes_sent;
// if (cnt < 10) { AddLog(LOG_LEVEL_DEBUG, "dac_buffer_len:%i", dac_buffer_len); }
    }
    xSemaphoreGive(dac_buffer_Mutex);
    return bytes_sent;
  } else {
    return -1;
  }
}

// returns `true` if successful
bool dac_task_start(void* _dac_handle) {
  dac_handle = (dac_continuous_handle_t)_dac_handle;

  // allocate buffer TODO
  if (dac_buffer == NULL) {
    dac_buffer = (uint8_t*)malloc(DAC_BUFFER_SIZE);
    if (dac_buffer == NULL) {
      return false;
    }
  }

  // create mutex
  if (dac_buffer_Mutex == NULL) {
    dac_buffer_Mutex = xSemaphoreCreateMutex();
    if (dac_buffer_Mutex == NULL) {
      return false;
    }
  }

  // create task
  if (dac_task_handle == NULL) {
    // Create task
    xTaskCreatePinnedToCore(
      dac_buffer_filling_task,  /* Function to implement the task */
      "dac_buffer_fill",        /* Name of the task */
      1024,                     /* Stack size in words */
      NULL,                     /* Task input parameter */
      1,                        /* Priority of the task */
      &dac_task_handle,         /* Task handle. */
      0);                       /* Core where the task should run */
  }
  return true;
}

bool dac_task_stop(void) {
  if (dac_task_handle != NULL) {
    vTaskDelete(dac_task_handle);
    dac_task_handle = NULL;
  }
  return true;
}

#endif // SOC_DAC_SUPPORTED
#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
