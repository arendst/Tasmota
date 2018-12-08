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

#define XDRV_16                16

#ifndef TUYA_DIMMER_ID
#define TUYA_DIMMER_ID         0
#endif

#define TUYA_POWER_ID          1

#define TUYA_CMD_HEARTBEAT     0x00
#define TUYA_CMD_QUERY_PRODUCT 0x01
#define TUYA_CMD_MCU_CONF      0x02
#define TUYA_CMD_WIFI_STATE    0x03
#define TUYA_CMD_WIFI_RESET    0x04
#define TUYA_CMD_WIFI_SELECT   0x05
#define TUYA_CMD_SET_DP        0x06
#define TUYA_CMD_STATE         0x07
#define TUYA_CMD_QUERY_STATE   0x08

#define TUYA_TYPE_BOOL         0x01
#define TUYA_TYPE_VALUE        0x02

#define TUYA_BUFFER_SIZE       256

#include <TasmotaSerial.h>

TasmotaSerial *TuyaSerial = nullptr;

uint8_t tuya_new_dim = 0;                   // Tuya dimmer value temp
boolean tuya_ignore_dim = false;            // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
uint8_t tuya_cmd_status = 0;                // Current status of serial-read
uint8_t tuya_cmd_checksum = 0;              // Checksum of tuya command
uint8_t tuya_data_len = 0;                  // Data lenght of command
int8_t tuya_wifi_state = -2;                // Keep MCU wifi-status in sync with WifiState()

char *tuya_buffer = NULL;                   // Serial receive buffer
int tuya_byte_counter = 0;                  // Index in serial receive buffer

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void TuyaSendCmd(uint8_t cmd, uint8_t payload[] = nullptr, uint16_t payload_len = 0){
  uint8_t checksum = (0xFF + cmd + (payload_len >> 8) + (payload_len & 0xFF));
  TuyaSerial->write(0x55);                  // Tuya header 55AA
  TuyaSerial->write(0xAA);
  TuyaSerial->write((uint8_t)0x00);         // version 00
  TuyaSerial->write(cmd);                   // Tuya command
  TuyaSerial->write(payload_len >> 8);      // following data length (Hi)
  TuyaSerial->write(payload_len & 0xFF);    // following data length (Lo)
  snprintf_P(log_data, sizeof(log_data), PSTR("TYA: TX Packet: \"55aa00%02x%02x%02x"), cmd, payload_len >> 8, payload_len & 0xFF);
  for(int i = 0; i < payload_len; ++i) {
    TuyaSerial->write(payload[i]);
    checksum += payload[i];
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, payload[i]);
  }
  TuyaSerial->write(checksum);
  TuyaSerial->flush();
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x\""), log_data, checksum);
  AddLog(LOG_LEVEL_DEBUG);
}

void TuyaSendState(uint8_t id, uint8_t type, uint8_t* value){
  uint16_t payload_len = 4;
  uint8_t payload_buffer[8];
  payload_buffer[0] = id;
  payload_buffer[1] = type;
  switch(type){
    case TUYA_TYPE_BOOL:
      payload_len += 1;
      payload_buffer[2] = 0x00;
      payload_buffer[3] = 0x01;
      payload_buffer[4] = value[0];
      break;
    case TUYA_TYPE_VALUE:
      payload_len += 4;
      payload_buffer[2] = 0x00;
      payload_buffer[3] = 0x04;
      payload_buffer[4] = value[3];
      payload_buffer[5] = value[2];
      payload_buffer[6] = value[1];
      payload_buffer[7] = value[0];
      break;
  }

  TuyaSendCmd(TUYA_CMD_SET_DP, payload_buffer, payload_len);
}

void TuyaSendBool(uint8_t id, boolean value){
    TuyaSendState(id, TUYA_TYPE_BOOL, &value);
}

void TuyaSendValue(uint8_t id, uint32_t value){
    TuyaSendState(id, TUYA_TYPE_VALUE, (uint8_t*)(&value));
}

boolean TuyaSetPower(void)
{
  boolean status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  if (source != SRC_SWITCH && TuyaSerial) {  // ignore to prevent loop from pushing state from faceplate interaction

    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: SetDevicePower.rpower=%d"), rpower);
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSendBool(TUYA_POWER_ID, rpower);

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

    snprintf_P(log_data, sizeof(log_data), PSTR( "TYA: Send Serial Packet Dim Value=%d (id=%d)"), duty, Settings.param[P_TUYA_DIMMER_ID]);
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSendValue(Settings.param[P_TUYA_DIMMER_ID], duty);

  } else {
    tuya_ignore_dim = false;  // reset flag

    snprintf_P(log_data, sizeof(log_data), PSTR( "TYA: Send Dim Level skipped due to 0 or already set. Value=%d"), duty);
    AddLog(LOG_LEVEL_DEBUG);

  }
}

void TuyaRequestState(void){
  if(TuyaSerial) {

    // Get current status of MCU
    snprintf_P(log_data, sizeof(log_data), "TYA: Request MCU state");
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSendCmd(TUYA_CMD_QUERY_STATE);
  }
}

void TuyaResetWifi(void)
{
  if (!Settings.flag.button_restrict) {
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
    ExecuteCommand(scmnd, SRC_BUTTON);
  }
}

void TuyaPacketProcess(void)
{
  char scmnd[20];

  switch(tuya_buffer[3]) {

    case TUYA_CMD_HEARTBEAT:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Heartbeat"));
      if(tuya_buffer[6] == 0){
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Detected MCU restart"));
        tuya_wifi_state = -2;
      }
      break;

    case TUYA_CMD_STATE:
      if (tuya_buffer[5] == 5) {  // on/off packet

        snprintf_P(log_data, sizeof(log_data),PSTR("TYA: RX - %s State"),tuya_buffer[10]?"On":"Off");
        AddLog(LOG_LEVEL_DEBUG);

        if((power || Settings.light_dimmer > 0) && (power != tuya_buffer[10])) {
          ExecuteCommandPower(1, tuya_buffer[10], SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
        }
      }
      else if (tuya_buffer[5] == 8) {  // dim packet

        snprintf_P(log_data, sizeof(log_data), PSTR("TYA: RX Dim State=%d"), tuya_buffer[13]);
        AddLog(LOG_LEVEL_DEBUG);

        if (!Settings.param[P_TUYA_DIMMER_ID]) {
          snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Autoconfiguring Dimmer ID %d"), tuya_buffer[6]);
          AddLog(LOG_LEVEL_DEBUG);
          Settings.param[P_TUYA_DIMMER_ID] = tuya_buffer[6];
        }

        tuya_new_dim = round(tuya_buffer[13] * (100. / 255.));
        if((power || Settings.flag3.tuya_apply_o20)  && (tuya_new_dim > 0) && (abs(tuya_new_dim - Settings.light_dimmer) > 1)) {

          snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), tuya_new_dim );

          snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Send CMND_DIMMER_STR=%s"), scmnd );
          AddLog(LOG_LEVEL_DEBUG);

          tuya_ignore_dim = true;
          ExecuteCommand(scmnd, SRC_SWITCH);
        }
      }
      break;

    case TUYA_CMD_WIFI_RESET:
    case TUYA_CMD_WIFI_SELECT:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi Reset"));
      TuyaResetWifi();
      break;

    case TUYA_CMD_WIFI_STATE:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi LED set ACK"));
      tuya_wifi_state = WifiState();
      break;

    case TUYA_CMD_MCU_CONF:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX MCU configuration"));

      if (tuya_buffer[5] == 2) {
        uint8_t led1_gpio = tuya_buffer[6];
        uint8_t key1_gpio = tuya_buffer[7];
        boolean key1_set = false;
        boolean led1_set = false;
        for (byte i = 0; i < MAX_GPIO_PIN; i++) {
          if (Settings.my_gp.io[i] == GPIO_LED1) led1_set = true;
          else if (Settings.my_gp.io[i] == GPIO_KEY1) key1_set = true;
        }
        if(!Settings.my_gp.io[led1_gpio] && !led1_set){
          Settings.my_gp.io[led1_gpio] = GPIO_LED1;
          restart_flag = 2;
        }
        if(!Settings.my_gp.io[key1_gpio] && !key1_set){
          Settings.my_gp.io[key1_gpio] = GPIO_KEY1;
          restart_flag = 2;
        }
      }
      TuyaRequestState();
      break;

    default:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX unknown command"));
  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

boolean TuyaModuleSelected(void)
{
  if (!(pin[GPIO_TUYA_RX] < 99) || !(pin[GPIO_TUYA_TX] < 99)) {  // fallback to hardware-serial if not explicitly selected
    pin[GPIO_TUYA_TX] = 1;
    pin[GPIO_TUYA_RX] = 3;
    Settings.my_gp.io[1] = GPIO_TUYA_TX;
    Settings.my_gp.io[3] = GPIO_TUYA_RX;
    restart_flag = 2;
  }
  light_type = LT_SERIAL1;
  return true;
}

void TuyaInit(void)
{
  if (!Settings.param[P_TUYA_DIMMER_ID]) {
    Settings.param[P_TUYA_DIMMER_ID] = TUYA_DIMMER_ID;
  }
  tuya_buffer = (char*)(malloc(TUYA_BUFFER_SIZE));
  if (tuya_buffer != NULL) {
    TuyaSerial = new TasmotaSerial(pin[GPIO_TUYA_RX], pin[GPIO_TUYA_TX], 2);
    if (TuyaSerial->begin(9600)) {
      if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }
      // Get MCU Configuration
      snprintf_P(log_data, sizeof(log_data), "TYA: Request MCU configuration");
      AddLog(LOG_LEVEL_DEBUG);

      TuyaSendCmd(TUYA_CMD_MCU_CONF);
    }
  }
}

void TuyaSerialInput(void)
{
  while (TuyaSerial->available()) {
    yield();
    byte serial_in_byte = TuyaSerial->read();

    if (serial_in_byte == 0x55) {            // Start TUYA Packet
      tuya_cmd_status = 1;
      tuya_buffer[tuya_byte_counter++] = serial_in_byte;
      tuya_cmd_checksum += serial_in_byte;
    }
    else if (tuya_cmd_status == 1 && serial_in_byte == 0xAA){ // Only packtes with header 0x55AA are valid
      tuya_cmd_status = 2;

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

      snprintf_P(log_data, sizeof(log_data), PSTR("TYA: RX Packet: \""));
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


boolean TuyaButtonPressed(void)
{
  if (!XdrvMailbox.index && ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == lastbutton[XdrvMailbox.index]))) {
    snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Reset GPIO triggered"));
    AddLog(LOG_LEVEL_DEBUG);
    TuyaResetWifi();
    return true;  // Reset GPIO served here
  }
  return false;   // Don't serve other buttons
}

void TuyaSetWifiLed(void){
    uint8_t wifi_state = 0x02;
    switch(WifiState()){
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

    snprintf_P(log_data, sizeof(log_data), "TYA: Set WiFi LED to state %d (%d)", wifi_state, WifiState());
    AddLog(LOG_LEVEL_DEBUG);

    TuyaSendCmd(TUYA_CMD_WIFI_STATE, &wifi_state, 1);
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

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
        if(TuyaSerial && tuya_wifi_state!=WifiState()) { TuyaSetWifiLed(); }
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_DIMMER
