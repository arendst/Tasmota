#include "uDisplay.h"
#include "uDisplay_config.h"

// ===== Timing and Delay Functions =====

void uDisplay::delay_arg(uint32_t args) {
  uint32_t delay_ms = 0;
  switch (args & 0xE0) {
    case 0x80:  delay_ms = 150; break;
    case 0xA0:  delay_ms =  10; break;
    case 0xE0:  delay_ms = 500; break;
  }
  if (delay_ms > 0) {
    delay(delay_ms);
    AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: delay %d ms"), delay_ms);
  }
}

void uDisplay::reset_pin(int32_t msl, int32_t msh) {
  if (reset > 0) {
    digitalWrite(reset, LOW);
    delay(msl);
    digitalWrite(reset, HIGH);
    delay(msh);
  }
}
#define UDSP_BUSY_TIMEOUT 3000

void uDisplay::delay_sync(int32_t ms) {
  uint8_t busy_level = HIGH;
  if (lvgl_param.busy_invert) {
    busy_level = LOW;
  }
  uint32_t time = millis();
  if (busy_pin > 0) {
    while (digitalRead(busy_pin) == busy_level) {
      delay(1);
      if  ((millis() - time) > UDSP_BUSY_TIMEOUT) {
        break;
      }
    }
  } else {
    delay(ms);
  }
}

// ===== SPI Transaction Control =====

void uDisplay::beginTransaction(SPISettings s) {
#ifdef ESP32
  if (lvgl_param.use_dma) {
    dmaWait();
  }
#endif
  uspi->beginTransaction(s);
}

void uDisplay::endTransaction(void) {
  uspi->endTransaction();
}