/*
  xdrv_84_gosund.ino - Gosund Dimmer SW2

  Copyright (C) 2020 Gabriel Gooslin

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

#ifdef USE_LIGHT
#ifdef USE_GOSUND_DIMMER

#include <TasmotaSerial.h>

#define XDRV_84 84
#define GOSUND_BUFFER_SIZE 256

struct Gosund
{
  /* Serial data */
  TasmotaSerial *serial = nullptr;
  uint8_t *buffer = nullptr;
  uint32_t serialStream = 0;
  uint32_t syncWord = 0;

  /* LEDs */
  uint8_t powerLedPin = 12;
  uint8_t lockLedPin = 13;
  uint8_t rangeLow = 0x7F;
  uint8_t rangeHigh = 0xEB;

  /* Switch state */
  int32_t currentPower = -1;
  int32_t desiredPower = 0;
  int32_t currentBrightnessPercent = -1;
  int32_t desiredBrightnessPercent = 0;
  uint32_t state_lockout = 0;
  bool lockout = false;

  /* Version info */
  uint8_t majorVersion = 1;
  uint8_t minorVersion = 2;
} Gosund;


void GosundSetLockout(bool showLed, uint32_t delayMs) {
  if (showLed) {
    digitalWrite(Gosund.powerLedPin, HIGH);
    digitalWrite(Gosund.lockLedPin, LOW);
  }
  SetNextTimeInterval(Gosund.state_lockout, delayMs);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Set lockout %ums"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, delayMs);
  Gosund.lockout=true;
}

bool GosundCheckLockout() {
  if (Gosund.lockout) {
    if (TimeReached(Gosund.state_lockout)) {
      digitalWrite(Gosund.powerLedPin, Gosund.currentPower ? LOW : HIGH);
      digitalWrite(Gosund.lockLedPin, HIGH);
      Gosund.lockout=false;
    }
  }

  return Gosund.lockout;
}

void GosundSerialInput(void) {
  int32_t newBrightness = -1;
  while (Gosund.serial->available()) {
    yield();
    uint8_t serial_in_byte = Gosund.serial->read();
    if (Gosund.syncWord && Gosund.serialStream == Gosund.syncWord) {
      newBrightness = serial_in_byte;

      /* If the brightness level is 0, set it to 1. We don't want the light to turn off because we dimmed on the touch panel */
      if (newBrightness == 0)
        newBrightness = 1;

      AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Sync word match. Read brightness %u from touch panel"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, newBrightness);
      Gosund.serialStream=0;
    }
    else if (((Gosund.serialStream >> 24) == 0x01) && (Gosund.syncWord != Gosund.serialStream)) {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Switching to syncword 0x%08x"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, Gosund.syncWord);
      Gosund.syncWord = Gosund.serialStream;
    }
    else {
      Gosund.serialStream = ((Gosund.serialStream << 8) & 0xFFFFFF00) | serial_in_byte;
    }
  }

  if (newBrightness >= 0) {
    /* We've read a new brighness percentage from the touch panel. */
    char scmnd[32];
    Gosund.desiredBrightnessPercent = newBrightness;
    Gosund.currentBrightnessPercent = newBrightness; /* The switch will have already adjusted the brightness. Setting it here keeps the dimming smooth when using the touch pad */
    snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), newBrightness);
    ExecuteCommand(scmnd, SRC_SWITCH);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Sending brightness %u from touch panel"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, newBrightness);
  }
}

void GosundSynchronize(void) {
  bool syncNeeded = false;
  uint32_t stepDiff = 0;
  uint32_t stepBrightness = 0;
  /* If still locked out, return */
  if (GosundCheckLockout()) return;

  /* 1 is a special lowest case. We don't want to check this value and round to 0 and turn the lights off */
  if (Gosund.desiredBrightnessPercent > 1 && (stepDiff = Gosund.desiredBrightnessPercent % Settings.dimmer_step) != 0) {
    stepBrightness = Gosund.desiredBrightnessPercent / Settings.dimmer_step * Settings.dimmer_step;
    if (stepDiff >= Settings.dimmer_step/2) stepBrightness += Settings.dimmer_step;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Reset desired brightness %u to step level %u"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, Gosund.desiredBrightnessPercent, stepBrightness);
    Gosund.desiredBrightnessPercent = stepBrightness;
    syncNeeded=true;
  }

  /* Ensure our brightness is above the hw minimum. Adjusting this way will cause falshing when using the touch pad since the brightness has already be adjusted by the switch */
  if (Gosund.desiredBrightnessPercent < Settings.dimmer_hw_min) {
    Gosund.desiredBrightnessPercent = Settings.dimmer_hw_min;
    syncNeeded=true;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Reset desired brightness %u to hw minimum %u"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, Gosund.desiredBrightnessPercent, Settings.dimmer_hw_min);
  }

  /* Ensure our brightness is above the hw maximum. Adjusting this way will cause falshing when using the touch pad since the brightness has already be adjusted by the switch */
  if (Gosund.desiredBrightnessPercent > Settings.dimmer_hw_max) {
    Gosund.desiredBrightnessPercent = Settings.dimmer_hw_max;
    syncNeeded=true;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Reset desired brightness %u to hw maximun %u"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, Gosund.desiredBrightnessPercent, Settings.dimmer_hw_max);
  }

  /* Ensure our brightness is above the hw minimum. Adjusting this way will cause falshing when using the touch pad since the brightness has already be adjusted by the switch */
  if (Gosund.desiredBrightnessPercent < 1) {
    Gosund.desiredBrightnessPercent = 1;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Reset desired brightness %u to be at least 1"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, Gosund.desiredBrightnessPercent);
    syncNeeded=true;
  }

  syncNeeded |= (Gosund.currentPower != Gosund.desiredPower); /* sync if the power state has changed */
  syncNeeded |= (Gosund.desiredPower && (Gosund.currentBrightnessPercent != Gosund.desiredBrightnessPercent)); /* sync if the power is on and the brightness has changed */

  /* If either our power state or brightness state are not the same, synchronize */
  if (syncNeeded) {
    char scmnd[32];
    uint8_t brightValue;

    if (Gosund.desiredPower) {
      /* If power is on, convert the desired brighness percent to a scale from rangeLow to rangeHigh */
      brightValue = changeUIntScale(Gosund.desiredBrightnessPercent, 0, 100, Gosund.rangeLow, Gosund.rangeHigh);
    }
    else {
      /* If power is off, convert the current brighness percent on the same scale as if power was on, however we need to reduce by rangeLow. */
      brightValue = changeUIntScale(Gosund.currentBrightnessPercent, 0, 100, 0, Gosund.rangeHigh - Gosund.rangeLow);
    }

    /* Inform the switch of the new brightness value */
    Gosund.serial->write(brightValue);

    /* If we're changing the power state, lock out future changes to allow the chip to get ready */
    if (Gosund.currentPower != Gosund.desiredPower)
      GosundSetLockout(true, 1000);

    Gosund.currentBrightnessPercent = Gosund.desiredBrightnessPercent;
    Gosund.currentPower = Gosund.desiredPower;

    if (Gosund.currentPower) {
      /* IF the power is on send the current brightness to ensure everything is up to date */
      snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), Gosund.currentBrightnessPercent);
      ExecuteCommand(scmnd, SRC_SWITCH);
    }

    digitalWrite(Gosund.powerLedPin, Gosund.currentPower ? LOW : HIGH);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Changed brightness with value: 0x%02x"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent, brightValue);
  }
}

void GosundInit(void)
{
  AddLog_P(LOG_LEVEL_INFO, PSTR("GS: Initializing Gosund Dimmer SW2 (v%u.%u)"),  Gosund.majorVersion, Gosund.minorVersion);
  Gosund.buffer = (uint8_t *)malloc(GOSUND_BUFFER_SIZE);
  if (Gosund.buffer != nullptr){
    Gosund.serial = new TasmotaSerial(Pin(GPIO_RXD), Pin(GPIO_TXD), 2);
    if (Gosund.serial->begin(115200)) {
      if (Gosund.serial->hardwareSerial()) {
        ClaimSerial();
      }
      Gosund.serial->flush();
   }
  }

  /* Enable LEDs and turn them off */
  pinMode(Gosund.lockLedPin, OUTPUT);
  pinMode(Gosund.powerLedPin, OUTPUT);
  digitalWrite(Gosund.lockLedPin, HIGH);
  digitalWrite(Gosund.powerLedPin, HIGH);

  /* Lock out communication with the switch for 2 seocnds to let it come online  */
  GosundSetLockout(true, 2000);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Initialized"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent);
}

bool GosundSetPower(void) {
  Gosund.desiredPower = XdrvMailbox.index;
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] Setpower"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent);
  return true;
}

bool GosundSetBrightness(void) {
  Gosund.desiredBrightnessPercent=LightGetDimmer(0);

  /* If we've dimmed to 0, turn off the lights */
  Gosund.desiredPower=(Gosund.desiredBrightnessPercent != 0);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] SetBrightness"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent);
  return true;
}

bool GosundButtonPressed(void) {
  if (!XdrvMailbox.index && ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == Button.last_state[XdrvMailbox.index]))) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("GS: [CP:%d DP:%u CB:%d DB:%u] ButtonPressed"),  Gosund.currentPower, Gosund.desiredPower, Gosund.currentBrightnessPercent, Gosund.desiredBrightnessPercent);
    ExecuteCommandPower(1, POWER_TOGGLE, SRC_LIGHT);
    return true;
  }
  return false;
}

/*
 * Interface
 */

bool Xdrv84(uint8_t function) {
  bool result = false;

  if (GOSUND_DIMMER == TasmotaGlobal.module_type) {
    switch (function) {
    case FUNC_LOOP:
      if (Gosund.serial) {
        GosundSynchronize();
        GosundSerialInput();
      }
      break;
    case FUNC_INIT:
      GosundInit();
      break;
    case FUNC_SET_DEVICE_POWER:
      result = GosundSetPower();
      break;
    case FUNC_SET_CHANNELS:
      result = GosundSetBrightness();
      break;
    case FUNC_BUTTON_PRESSED:
      result = GosundButtonPressed();
      break;
    }
  }
  return result;
}

#endif  // USE_GOSUND_DIMMER
#endif  // USE_LIGHT
