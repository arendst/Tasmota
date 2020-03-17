/*
  xlgt_06_moodlamp.ino - ElectriQ iQ-wifiMOODL LED support for Tasmota

  Copyright (C) 2019  Theo Arends

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
#ifdef USE_ELECTRIQ_MOODL

/*********************************************************************************************\
 * ElectriQ iQ-wifiMOODL
 * This RGBW mood lamp uses the TYWE3S module to transmit via UART to an unmarked MCU which
 * drives 8 MOSFETs. The MCU does not transmit; it ony receives commands from the TYWE3S.
 * The MCU appears to use a modified/undocumented version of the TuyaMCU protocol.
 * The main PCB has 2 daughter boards which hold the RGBW LEDs - an upper deck and a lower deck.
 * The same RGBW data is transmitted to the upper and lower decks.
 * *********************************************************************************************/

#define XLGT_06                           6

/********************************************************************************************/

bool ElectriqMoodLSetChannels(void)
{
  uint8_t *col = (uint8_t*)XdrvMailbox.data;
  uint8_t checksum = (uint8_t)(0x65 + 0xAA + 0x01 + 0x0A);

  Serial.write(0x65);   // Fixed header
  Serial.write(0xAA);
  Serial.write(0x00);   // Version
  Serial.write(0x01);   // Command
  Serial.write(0x0A);   // Payload length

  uint8_t payload[5];
  payload[0] = col[0];
  payload[1] = col[1];
  payload[2] = col[2];
  payload[3] = col[3];
  payload[4] = 0x0;    // Unused

  // Send payload for the upper LED deck
  for (uint32_t i = 0; i < 5; i++) {
    Serial.write(payload[i]);
    checksum += payload[i];
  }

  // Send payload for the lower LED deck
  for (uint32_t i = 0; i < 5; i++) {
    Serial.write(payload[i]);
    checksum += payload[i];
  }

  Serial.write(checksum);
  Serial.flush();

  return true;
}

void ElectriqMoodLModuleSelected(void)
{
  if (pin[GPIO_ELECTRIC_MOODL_TX] < 99) {
    SetSerial(9600, TS_SERIAL_8N1);
    light_type = LT_RGBW;
    light_flg = XLGT_06;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: ElectriQ Mood Lamp Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt06(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = ElectriqMoodLSetChannels();
      break;
    case FUNC_MODULE_INIT:
      ElectriqMoodLModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_ELECTRIQ_MOODL
#endif  // USE_LIGHT