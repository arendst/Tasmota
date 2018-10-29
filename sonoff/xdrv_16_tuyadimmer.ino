/*
  xdrv_16_tuyadimmer.ino - Tuya dimmer support for Sonoff-Tasmota

  Copyright (C) 2018  digiblur, Joel Stein and Theo Arends

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

#ifdef USE_TUYA_DIMMER

#ifndef TUYA_DIMMER_ID
#define TUYA_DIMMER_ID    3
#endif
#define TUYA_BUFFER_SIZE  256

#include <TasmotaSerial.h>

TasmotaSerial *TuyaSerial = nullptr;

uint8_t tuya_new_dim = 0;                   // Tuya dimmer value temp
boolean tuya_ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
uint8_t tuya_cmd_status = 0;                // Current status of serial-read
uint8_t tuya_cmd_checksum = 0;              // Checksum of tuya command
uint8_t tuya_data_len = 0;                  // Data lenght of command
bool tuya_wifi_state = false;

char tuya_buffer[TUYA_BUFFER_SIZE];         // Serial receive buffer
int tuya_byte_counter = 0;                  // Index in serial receive buffer

boolean TuyaSetPower()
{
  boolean status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  if (source != SRC_SWITCH && TuyaSerial) {  // ignore to prevent loop from pushing state from faceplate interaction

    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: SetDevicePower.rpower=%d"), rpower);
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSerial->write((uint8_t)0x55); // Tuya header 55AA
    TuyaSerial->write((uint8_t)0xAA);
    TuyaSerial->write((uint8_t)0x00); // version 00
    TuyaSerial->write((uint8_t)0x06); // Tuya command 06
    TuyaSerial->write((uint8_t)0x00);
    TuyaSerial->write((uint8_t)0x05); // following data length 0x05
    TuyaSerial->write((uint8_t)0x01); // relay number 1,2,3
    TuyaSerial->write((uint8_t)0x01);
    TuyaSerial->write((uint8_t)0x00);
    TuyaSerial->write((uint8_t)0x01);
    TuyaSerial->write((uint8_t)rpower); // status
    TuyaSerial->write((uint8_t)0x0D + rpower); // checksum sum of all bytes in packet mod 256
    TuyaSerial->flush();

    status = true;
  }
  return status;
}

void LightSerialDuty(uint8_t duty)
{
  if (duty > 0 && !tuya_ignore_dim && TuyaSerial) {
    if (duty < 25) {
      duty = 25;  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
    }
    TuyaSerial->write((uint8_t)0x55); // Tuya header 55AA
    TuyaSerial->write((uint8_t)0xAA);
    TuyaSerial->write((uint8_t)0x00); // version 00
    TuyaSerial->write((uint8_t)0x06); // Tuya command 06 - send order
    TuyaSerial->write((uint8_t)0x00);
    TuyaSerial->write((uint8_t)0x08); // following data length 0x08
    TuyaSerial->write((uint8_t)Settings.param[P_TUYA_DIMMER_ID]); // dimmer id
    TuyaSerial->write((uint8_t)0x02); // type=value
    TuyaSerial->write((uint8_t)0x00); // length hi
    TuyaSerial->write((uint8_t)0x04); // length low
    TuyaSerial->write((uint8_t)0x00); //
    TuyaSerial->write((uint8_t)0x00); //
    TuyaSerial->write((uint8_t)0x00); //
    TuyaSerial->write((uint8_t) duty ); // dim value (0-255)
    TuyaSerial->write((uint8_t) byte(Settings.param[P_TUYA_DIMMER_ID] + 19 + duty) ); // checksum:sum of all bytes in packet mod 256
    TuyaSerial->flush();

    snprintf_P(log_data, sizeof(log_data), PSTR( "TYA: Send Serial Packet Dim Value=%d (id=%d)"), duty, Settings.param[P_TUYA_DIMMER_ID]);
    AddLog(LOG_LEVEL_DEBUG);

  } else {
    tuya_ignore_dim = false;  // reset flag

    snprintf_P(log_data, sizeof(log_data), PSTR( "TYA: Send Dim Level skipped due to 0 or already set. Value=%d"), duty);
    AddLog(LOG_LEVEL_DEBUG);

  }
}

void TuyaPacketProcess()
{
  char scmnd[20];

  snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Packet Size=%d"), tuya_byte_counter);
  AddLog(LOG_LEVEL_DEBUG);

  if (tuya_byte_counter == 7 && tuya_buffer[3] == 14 ) {  // heartbeat packet
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Heartbeat"));
  }
  else if (tuya_byte_counter == 12 && tuya_buffer[3] == 7 && tuya_buffer[5] == 5) {  // on/off packet

    snprintf_P(log_data, sizeof(log_data),PSTR("TYA: Rcvd - %s State"),tuya_buffer[10]?"On":"Off");
    AddLog(LOG_LEVEL_DEBUG);

    if((power || Settings.light_dimmer > 0) && (power != tuya_buffer[10])) {
      ExecuteCommandPower(1, tuya_buffer[10], SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
    }
  }
  else if (tuya_byte_counter == 15 && tuya_buffer[3] == 7 && tuya_buffer[5] == 8) {  // dim packet

    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Rcvd Dim State=%d"), tuya_buffer[13]);
    AddLog(LOG_LEVEL_DEBUG);

    tuya_new_dim = round(tuya_buffer[13] * (100. / 255.));
    if((power) && (tuya_new_dim > 0) && (abs(tuya_new_dim - Settings.light_dimmer) > 2)) {

      snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Send CMND_DIMMER=%d"), tuya_new_dim );
      AddLog(LOG_LEVEL_DEBUG);

      snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), tuya_new_dim );

      snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Send CMND_DIMMER_STR=%s"), scmnd );
      AddLog(LOG_LEVEL_DEBUG);

      tuya_ignore_dim = true;
      ExecuteCommand(scmnd, SRC_SWITCH);
    }
  }
  else if (tuya_byte_counter == 8 && tuya_buffer[3] == 5 && tuya_buffer[5] == 1 && tuya_buffer[7] == 5 ) {  // reset WiFi settings packet

    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: WiFi Reset Rcvd"));
    TuyaResetWifi();
  }
  else if (tuya_byte_counter == 7 && tuya_buffer[3] == 3 && tuya_buffer[6] == 2) {  // WiFi LED has been sucessfully reset.

    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: WiFi LED reset ACK"));
    tuya_wifi_state = true;
  }
}

void TuyaSerialInput()
{
  while (TuyaSerial->available()) {
    yield();
    byte serial_in_byte = TuyaSerial->read();

    //snprintf_P(log_data, sizeof(log_data), PSTR("TYA: serial_in_byte %d, tuya_cmd_status %d, tuya_cmd_checksum %d, tuya_data_len %d, tuya_byte_counter %d"), serial_in_byte, tuya_cmd_status, tuya_cmd_checksum, tuya_data_len, tuya_byte_counter);
    //AddLog(LOG_LEVEL_DEBUG);

    if (serial_in_byte == 0x55) {            // Start TUYA Packet
      tuya_cmd_status = 1;
      tuya_buffer[tuya_byte_counter++] = serial_in_byte;
      tuya_cmd_checksum += serial_in_byte;
    }
    else if (tuya_cmd_status == 1 && serial_in_byte == 0xAA){ // Only packtes with header 0x55AA are valid
      tuya_cmd_status = 2;

      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: 0x55AA Packet Start"));

      tuya_byte_counter = 0;
      tuya_buffer[tuya_byte_counter++] = 0x55;
      tuya_buffer[tuya_byte_counter++] = 0xAA;
      tuya_cmd_checksum = 0xFF;
    }
    else if (tuya_cmd_status == 2){
      if(tuya_byte_counter == 5){ // Get length of data
        tuya_cmd_status = 3;
        tuya_data_len = serial_in_byte;
      }
      tuya_cmd_checksum += serial_in_byte;
      tuya_buffer[tuya_byte_counter++] = serial_in_byte;
    }
    else if ((tuya_cmd_status == 3) && (tuya_byte_counter == (6 + tuya_data_len)) && (tuya_cmd_checksum == serial_in_byte)){ // Compare checksum and process packet
      tuya_buffer[tuya_byte_counter++] = serial_in_byte;

      snprintf_P(log_data, sizeof(log_data), PSTR("TYA: 0x55 Packet End: \""));
      for (int i = 0; i < tuya_byte_counter; i++) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, tuya_buffer[i]);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s\""), log_data);
      AddLog(LOG_LEVEL_DEBUG);

      TuyaPacketProcess();
      tuya_byte_counter = 0;
      tuya_cmd_status = 0;
      tuya_cmd_checksum = 0;
      tuya_data_len = 0;
    }                               // read additional packets from TUYA
    else if(tuya_byte_counter < TUYA_BUFFER_SIZE -1) {  // add char to string if it still fits
      tuya_buffer[tuya_byte_counter++] = serial_in_byte;
      tuya_cmd_checksum += serial_in_byte;
    } else {
      tuya_byte_counter = 0;
      tuya_cmd_status = 0;
      tuya_cmd_checksum = 0;
      tuya_data_len = 0;
    }
  }
}

boolean TuyaModuleSelected()
{
  if (!(pin[GPIO_TUYA_RX] < 99) || !(pin[GPIO_TUYA_TX] < 99)) {  // fallback to hardware-serial if not explicitly selected
    pin[GPIO_TUYA_RX] = 1;
    pin[GPIO_TUYA_TX] = 3;
  }
  light_type = LT_SERIAL;
  return true;
}

void TuyaResetWifiLed(){
    snprintf_P(log_data, sizeof(log_data), "TYA: Reset WiFi LED");
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSerial->write((uint8_t)0x55); // header 55AA
    TuyaSerial->write((uint8_t)0xAA);
    TuyaSerial->write((uint8_t)0x00); // version 00
    TuyaSerial->write((uint8_t)0x03); // command 03 - set wifi state
    TuyaSerial->write((uint8_t)0x00);
    TuyaSerial->write((uint8_t)0x01); // following data length 0x01
    TuyaSerial->write((uint8_t)0x03); // wifi state 4 (configured and connected)
    TuyaSerial->write((uint8_t)0x06); // checksum:sum of all bytes in packet mod 256
    TuyaSerial->flush();
}

void TuyaInit()
{
  if (!Settings.param[P_TUYA_DIMMER_ID]) {
    Settings.param[P_TUYA_DIMMER_ID] = TUYA_DIMMER_ID;
  }
  TuyaSerial = new TasmotaSerial(pin[GPIO_TUYA_RX], pin[GPIO_TUYA_TX], 1);
  if (TuyaSerial->begin(9600)) {
    if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }

    // Get current status of MCU
    snprintf_P(log_data, sizeof(log_data), "TYA: Request MCU state");
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSerial->write((uint8_t)0x55); // header 55AA
    TuyaSerial->write((uint8_t)0xAA);
    TuyaSerial->write((uint8_t)0x00); // version 00
    TuyaSerial->write((uint8_t)0x08); // command 08 - get status
    TuyaSerial->write((uint8_t)0x00);
    TuyaSerial->write((uint8_t)0x00); // following data length 0x00
    TuyaSerial->write((uint8_t)0x07); // checksum:sum of all bytes in packet mod 256
    TuyaSerial->flush();
  }
}

void TuyaResetWifi()
{
  if (!Settings.flag.button_restrict) {
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
    ExecuteCommand(scmnd, SRC_BUTTON);
    tuya_wifi_state = false;
  }
}

boolean TuyaButtonPressed()
{
  if ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == lastbutton[XdrvMailbox.index])) {

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), XdrvMailbox.index +1);
    AddLog(LOG_LEVEL_DEBUG);
    TuyaResetWifi();

  }
  return true;  // Serviced here
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_16

boolean Xdrv16(byte function)
{
  boolean result = false;

  if (TUYA_DIMMER == Settings.module) {
    switch (function) {
      case FUNC_MODULE_INIT:
        result = TuyaModuleSelected();
        break;
      case FUNC_INIT:
        TuyaInit();
        break;
      case FUNC_LOOP:
        if (TuyaSerial) { TuyaSerialInput(); }
        break;
      case FUNC_SET_DEVICE_POWER:
        result = TuyaSetPower();
        break;
      case FUNC_BUTTON_PRESSED:
        result = TuyaButtonPressed();
        break;
      case FUNC_EVERY_SECOND:
        if(TuyaSerial && !tuya_wifi_state) { TuyaResetWifiLed(); }
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_DIMMER
