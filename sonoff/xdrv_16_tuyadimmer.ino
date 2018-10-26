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
#define TUYA_DIMMER_ID  3
#endif

uint8_t tuya_new_dim = 0;                   // Tuya dimmer value temp
boolean tuya_ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
uint8_t tuya_cmd_status = 0;                // Current status of serial-read
uint8_t tuya_cmd_checksum = 0;              // Checksum of tuya command
uint8_t tuya_data_len = 0;                  // Data lenght of command

boolean TuyaSetPower()
{
  boolean status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  if (source != SRC_SWITCH ) {  // ignore to prevent loop from pushing state from faceplate interaction

    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: SetDevicePower.rpower=%d"), rpower);
    AddLog(LOG_LEVEL_DEBUG);

    Serial.write(0x55); // Tuya header 55AA
    Serial.write(0xAA);
    Serial.write(0x00); // version 00
    Serial.write(0x06); // Tuya command 06
    Serial.write(0x00);
    Serial.write(0x05); // following data length 0x05
    Serial.write(0x01); // relay number 1,2,3
    Serial.write(0x01);
    Serial.write(0x00);
    Serial.write(0x01);
    Serial.write(rpower); // status
    Serial.write(0x0D + rpower); // checksum sum of all bytes in packet mod 256
    Serial.flush();

    status = true;
  }
  return status;
}

void LightSerialDuty(uint8_t duty)
{
  if (duty > 0 && !tuya_ignore_dim ) {
    if (duty < 25) {
      duty = 25;  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
    }
    Serial.write(0x55); // Tuya header 55AA
    Serial.write(0xAA);
    Serial.write(0x00); // version 00
    Serial.write(0x06); // Tuya command 06 - send order
    Serial.write(0x00);
    Serial.write(0x08); // following data length 0x08
    Serial.write(Settings.param[P_TUYA_DIMMER_ID]); // dimmer id
    Serial.write(0x02); // type=value
    Serial.write(0x00); // length hi
    Serial.write(0x04); // length low
    Serial.write(0x00); //
    Serial.write(0x00); //
    Serial.write(0x00); //
    Serial.write( duty ); // dim value (0-255)
    Serial.write( byte(Settings.param[P_TUYA_DIMMER_ID] + 19 + duty) ); // checksum:sum of all bytes in packet mod 256
    Serial.flush();

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

  snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Packet Size=%d"), serial_in_byte_counter);
  AddLog(LOG_LEVEL_DEBUG);

  if (serial_in_byte_counter == 7 && serial_in_buffer[3] == 14 ) {  // heartbeat packet
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Heartbeat"));
  }
  else if (serial_in_byte_counter == 12 && serial_in_buffer[3] == 7 && serial_in_buffer[5] == 5) {  // on/off packet

    snprintf_P(log_data, sizeof(log_data),PSTR("TYA: Rcvd - %s State"),serial_in_buffer[10]?"On":"Off");
    AddLog(LOG_LEVEL_DEBUG);

    if((power || Settings.light_dimmer > 0) && (power != serial_in_buffer[10])) {
      ExecuteCommandPower(1, serial_in_buffer[10], SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
    }
  }
  else if (serial_in_byte_counter == 15 && serial_in_buffer[3] == 7 && serial_in_buffer[5] == 8) {  // dim packet

    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Rcvd Dim State=%d"), serial_in_buffer[13]);
    AddLog(LOG_LEVEL_DEBUG);

    tuya_new_dim = round(serial_in_buffer[13] * (100. / 255.));
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
  else if (serial_in_byte_counter == 8 && serial_in_buffer[3] == 5 && serial_in_buffer[5] == 1 && serial_in_buffer[7] == 5 ) {  // reset WiFi settings packet - to do: reset red MCU LED after WiFi is up

    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: WiFi Reset Rcvd"));

    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " 2");
    ExecuteCommand(scmnd, SRC_BUTTON);
  }
}

void TuyaSerialInput()
{
  while (Serial.available()) {
    yield();
    serial_in_byte = Serial.read();

    //snprintf_P(log_data, sizeof(log_data), PSTR("TYA: serial_in_byte %d, tuya_cmd_status %d, tuya_cmd_checksum %d, tuya_data_len %d, serial_in_byte_counter %d"), serial_in_byte, tuya_cmd_status, tuya_cmd_checksum, tuya_data_len, serial_in_byte_counter);
    //AddLog(LOG_LEVEL_DEBUG);

    if (serial_in_byte == 0x55) {            // Start TUYA Packet
      tuya_cmd_status = 1;
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
      tuya_cmd_checksum += serial_in_byte;
    }
    else if (tuya_cmd_status == 1 && serial_in_byte == 0xAA){ // Only packtes with header 0x55AA are valid
      tuya_cmd_status = 2;

      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: 0x55AA Packet Start"));

      serial_in_byte_counter = 0;
      serial_in_buffer[serial_in_byte_counter++] = 0x55;
      serial_in_buffer[serial_in_byte_counter++] = 0xAA;
      tuya_cmd_checksum = 0xFF;
    }
    else if (tuya_cmd_status == 2){
      if(serial_in_byte_counter == 5){ // Get length of data
        tuya_cmd_status = 3;
        tuya_data_len = serial_in_byte;
      }
      tuya_cmd_checksum += serial_in_byte;
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    }
    else if ((tuya_cmd_status == 3) && (serial_in_byte_counter == (6 + tuya_data_len)) && (tuya_cmd_checksum == serial_in_byte)){ // Compare checksum and process packet
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;

      snprintf_P(log_data, sizeof(log_data), PSTR("TYA: 0x55 Packet End: \""));
      for (int i = 0; i < serial_in_byte_counter; i++) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, serial_in_buffer[i]);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s\""), log_data);
      AddLog(LOG_LEVEL_DEBUG);

      TuyaPacketProcess();
      serial_in_byte_counter = 0;
      tuya_cmd_status = 0;
      tuya_cmd_checksum = 0;
      tuya_data_len = 0;
    }                               // read additional packets from TUYA
    else if(serial_in_byte_counter < INPUT_BUFFER_SIZE -1) {  // add char to string if it still fits
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
      tuya_cmd_checksum += serial_in_byte;
    } else {
      serial_in_byte_counter = 0;
      tuya_cmd_status = 0;
      tuya_cmd_checksum = 0;
      tuya_data_len = 0;
    }
  }
}

boolean TuyaModuleSelected()
{
  baudrate = 9600;
  light_type = LT_SERIAL;
  return true;
}

void TuyaInit()
{
  if (!Settings.param[P_TUYA_DIMMER_ID]) {
    Settings.param[P_TUYA_DIMMER_ID] = TUYA_DIMMER_ID;
  }
  Serial.setDebugOutput(false);
  ClaimSerial();

  // Get current status of MCU
  snprintf_P(log_data, sizeof(log_data), "TYA: Request MCU state");
  AddLog(LOG_LEVEL_DEBUG);
  Serial.write(0x55); // header 55AA
  Serial.write(0xAA);
  Serial.write(0x00); // version 00
  Serial.write(0x08); // command 08 - get status
  Serial.write(0x00);
  Serial.write(0x00); // following data length 0x00
  Serial.write(0x07); // checksum:sum of all bytes in packet mod 256
  Serial.flush();
}

boolean TuyaButtonPressed()
{
  if ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == lastbutton[XdrvMailbox.index])) {

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), XdrvMailbox.index +1);
    AddLog(LOG_LEVEL_DEBUG);

    if (!Settings.flag.button_restrict) {
      char scmnd[20];
      snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
      ExecuteCommand(scmnd, SRC_BUTTON);
    }
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
        TuyaSerialInput();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = TuyaSetPower();
        break;
      case FUNC_BUTTON_PRESSED:
        result = TuyaButtonPressed();
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_DIMMER
