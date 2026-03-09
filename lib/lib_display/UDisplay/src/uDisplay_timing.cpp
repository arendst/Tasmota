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
