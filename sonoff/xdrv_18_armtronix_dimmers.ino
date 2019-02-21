/*
  xdrv_18_armtronix_dimmers.ino - Armtronix dimmers support for Sonoff-Tasmota

  Copyright (C) 2018  wvdv2002 and Theo Arends

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

#ifdef USE_ARMTRONIX_DIMMERS
/*********************************************************************************************\
 * This code can be used for Armtronix dimmers.
 * The dimmers contain a Atmega328 to do the actual dimming.
 * Checkout the Tasmota Wiki for information on how to flash this Atmega328 with the firmware
 *   to work together with this driver.
\*********************************************************************************************/

#define XDRV_18                18

#include <TasmotaSerial.h>

TasmotaSerial *ArmtronixSerial = nullptr;

boolean armtronix_ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
int8_t armtronix_wifi_state = -2;                // Keep MCU wifi-status in sync with WifiState()
int8_t armtronix_dimState[2];                    // Dimmer state values.
int8_t armtronix_knobState[2];                   // Dimmer state values.

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

boolean ArmtronixSetChannels(void)
{
  LightSerial2Duty(((uint8_t*)XdrvMailbox.data)[0], ((uint8_t*)XdrvMailbox.data)[1]);
  return true;
}

void LightSerial2Duty(uint8_t duty1, uint8_t duty2)
{
  if (ArmtronixSerial && !armtronix_ignore_dim) {
    duty1 = ((float)duty1)/2.575757; //max 99
    duty2 = ((float)duty2)/2.575757; //max 99
    armtronix_dimState[0] = duty1;
    armtronix_dimState[1] = duty2;
    ArmtronixSerial->print("Dimmer1:");
    ArmtronixSerial->print(duty1);
    ArmtronixSerial->print("\nDimmer2:");
    ArmtronixSerial->println(duty2);

    snprintf_P(log_data, sizeof(log_data), PSTR( "ARM: Send Serial Packet Dim Values=%d,%d"), armtronix_dimState[0],armtronix_dimState[1]);
    AddLog(LOG_LEVEL_DEBUG);

  } else {
    armtronix_ignore_dim = false;
    snprintf_P(log_data, sizeof(log_data), PSTR( "ARM: Send Dim Level skipped due to already set. Value=%d,%d"), armtronix_dimState[0],armtronix_dimState[1]);
    AddLog(LOG_LEVEL_DEBUG);

  }
}

void ArmtronixRequestState(void)
{
  if (ArmtronixSerial) {
    // Get current status of MCU
    snprintf_P(log_data, sizeof(log_data), "TYA: Request MCU state");
    AddLog(LOG_LEVEL_DEBUG);
    ArmtronixSerial->println("Status");

  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

boolean ArmtronixModuleSelected(void)
{
  light_type = LT_SERIAL2;
  return true;
}

void ArmtronixInit(void)
{
  armtronix_dimState[0] = -1;
  armtronix_dimState[1] = -1;
  armtronix_knobState[0] = -1;
  armtronix_knobState[1] = -1;
  ArmtronixSerial = new TasmotaSerial(pin[GPIO_RXD], pin[GPIO_TXD], 2);
  if (ArmtronixSerial->begin(115200)) {
    if (ArmtronixSerial->hardwareSerial()) { ClaimSerial(); }
    ArmtronixSerial->println("Status");
  }
}

void ArmtronixSerialInput(void)
{
  String answer;
  int8_t newDimState[2];
  uint8_t temp;
  int commaIndex;
  char scmnd[20];
  if (ArmtronixSerial->available()) {
    yield();
    answer = ArmtronixSerial->readStringUntil('\n');
    if (answer.substring(0,7) == "Status:") {
      commaIndex = 6;
      for (int i =0; i<2; i++) {
        newDimState[i] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
        if (newDimState[i] != armtronix_dimState[i]) {
          temp = ((float)newDimState[i])*1.01010101010101; //max 255
          armtronix_dimState[i] = newDimState[i];
          armtronix_ignore_dim = true;
          snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "%d %d"),i+1, temp);
          ExecuteCommand(scmnd,SRC_SWITCH);
          snprintf_P(log_data, sizeof(log_data), PSTR("ARM: Send CMND_CHANNEL=%s"), scmnd );
          AddLog(LOG_LEVEL_DEBUG);
        }
        commaIndex = answer.indexOf(',',commaIndex+1);
      }
      armtronix_knobState[0] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
      commaIndex = answer.indexOf(',',commaIndex+1);
      armtronix_knobState[1] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
    }
  }
}

void ArmtronixSetWifiLed(void)
{
  uint8_t wifi_state = 0x02;

  switch (WifiState()) {
    case WIFI_SMARTCONFIG:
      wifi_state = 0x00;
      break;
    case WIFI_MANAGER:
    case WIFI_WPSCONFIG:
      wifi_state = 0x01;
      break;
    case WIFI_RESTART:
      wifi_state =  0x03;
      break;
  }

  snprintf_P(log_data, sizeof(log_data), "ARM: Set WiFi LED to state %d (%d)", wifi_state, WifiState());
  AddLog(LOG_LEVEL_DEBUG);

  char state = '0' + (wifi_state & 1 > 0);
  ArmtronixSerial->print("Setled:");
  ArmtronixSerial->write(state);
  ArmtronixSerial->write(',');
  state = '0' + (wifi_state & 2 > 0);
  ArmtronixSerial->write(state);
  ArmtronixSerial->write(10);
  armtronix_wifi_state = WifiState();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv18(byte function)
{
  boolean result = false;

  if (ARMTRONIX_DIMMERS == Settings.module) {
    switch (function) {
      case FUNC_MODULE_INIT:
        result = ArmtronixModuleSelected();
        break;
      case FUNC_INIT:
        ArmtronixInit();
        break;
      case FUNC_LOOP:
        if (ArmtronixSerial) { ArmtronixSerialInput(); }
        break;
      case FUNC_EVERY_SECOND:
        if (ArmtronixSerial) {
          if (armtronix_wifi_state!=WifiState()) { ArmtronixSetWifiLed(); }
          if (uptime &1) {
            ArmtronixSerial->println("Status");
          }
        }
        break;
      case FUNC_SET_CHANNELS:
        result = ArmtronixSetChannels();
        break;
    }
  }
  return result;
}

#endif  // USE_ARMTRONIX_DIMMERS
