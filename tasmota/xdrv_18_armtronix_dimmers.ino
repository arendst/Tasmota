/*
  xdrv_18_armtronix_dimmers.ino - Armtronix dimmers support for Tasmota

  Copyright (C) 2020  wvdv2002 and Theo Arends

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

struct ARMTRONIX {
  bool ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
  int8_t wifi_state = -2;             // Keep MCU wifi-status in sync with WifiState()
  int8_t dim_state[2];                // Dimmer state values.
  int8_t knob_state[2];               // Dimmer state values.
} Armtronix;

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

bool ArmtronixSetChannels(void)
{
  LightSerial2Duty(((uint8_t*)XdrvMailbox.data)[0], ((uint8_t*)XdrvMailbox.data)[1]);
  return true;
}

void LightSerial2Duty(uint8_t duty1, uint8_t duty2)
{
  if (ArmtronixSerial && !Armtronix.ignore_dim) {
    duty1 = ((float)duty1)/2.575757; //max 99
    duty2 = ((float)duty2)/2.575757; //max 99
    Armtronix.dim_state[0] = duty1;
    Armtronix.dim_state[1] = duty2;
    ArmtronixSerial->print("Dimmer1:");
    ArmtronixSerial->print(duty1);
    ArmtronixSerial->print("\nDimmer2:");
    ArmtronixSerial->println(duty2);

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ARM: Send Serial Packet Dim Values=%d,%d"), Armtronix.dim_state[0],Armtronix.dim_state[1]);

  } else {
    Armtronix.ignore_dim = false;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ARM: Send Dim Level skipped due to already set. Value=%d,%d"), Armtronix.dim_state[0],Armtronix.dim_state[1]);

  }
}

void ArmtronixRequestState(void)
{
  if (ArmtronixSerial) {
    // Get current status of MCU
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("ARM: Request MCU state"));
    ArmtronixSerial->println("Status");

  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool ArmtronixModuleSelected(void)
{
  devices_present++;
  light_type = LT_SERIAL2;
  return true;
}

void ArmtronixInit(void)
{
  Armtronix.dim_state[0] = -1;
  Armtronix.dim_state[1] = -1;
  Armtronix.knob_state[0] = -1;
  Armtronix.knob_state[1] = -1;
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
      for (uint32_t i =0; i<2; i++) {
        newDimState[i] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
        if (newDimState[i] != Armtronix.dim_state[i]) {
          temp = ((float)newDimState[i])*1.01010101010101; //max 255
          Armtronix.dim_state[i] = newDimState[i];
          Armtronix.ignore_dim = true;
          snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "%d %d"),i+1, temp);
          ExecuteCommand(scmnd,SRC_SWITCH);
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ARM: Send CMND_CHANNEL=%s"), scmnd );
        }
        commaIndex = answer.indexOf(',',commaIndex+1);
      }
      Armtronix.knob_state[0] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
      commaIndex = answer.indexOf(',',commaIndex+1);
      Armtronix.knob_state[1] = answer.substring(commaIndex+1,answer.indexOf(',',commaIndex+1)).toInt();
    }
  }
}

void ArmtronixSetWifiLed(void)
{
  uint8_t wifi_state = 0x02;

  switch (WifiState()) {
    case WIFI_MANAGER:
      wifi_state = 0x01;
      break;
    case WIFI_RESTART:
      wifi_state =  0x03;
      break;
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ARM: Set WiFi LED to state %d (%d)"), wifi_state, WifiState());

  char state = '0' + ((wifi_state & 1) > 0);
  ArmtronixSerial->print("Setled:");
  ArmtronixSerial->write(state);
  ArmtronixSerial->write(',');
  state = '0' + ((wifi_state & 2) > 0);
  ArmtronixSerial->write(state);
  ArmtronixSerial->write(10);
  Armtronix.wifi_state = WifiState();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv18(uint8_t function)
{
  bool result = false;

  if (ARMTRONIX_DIMMERS == my_module_type) {
    switch (function) {
      case FUNC_LOOP:
        if (ArmtronixSerial) { ArmtronixSerialInput(); }
        break;
      case FUNC_MODULE_INIT:
        result = ArmtronixModuleSelected();
        break;
      case FUNC_INIT:
        ArmtronixInit();
        break;
      case FUNC_EVERY_SECOND:
        if (ArmtronixSerial) {
          if (Armtronix.wifi_state!=WifiState()) { ArmtronixSetWifiLed(); }
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
#endif  // USE_LIGHT
