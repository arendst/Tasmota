/*
  xsns_51_rdm6300.ino - Support for RDM6300 NFC Tag Reader on Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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


#ifdef USE_RDM6300

#define XSNS_51                          51

#define RDM6300_BAUDRATE 9600

#include <TasmotaSerial.h>

#define RDM_TIMEOUT 100
char rdm_uid_str[10];

// 2 seconds block time
#define RDM6300_BLOCK 2*10

uint8_t rdm_blcnt;
TasmotaSerial *RDM6300_Serial = nullptr;

void RDM6300_Init() {
  if (pin[GPIO_RDM6300_RX] < 99) {
    RDM6300_Serial = new TasmotaSerial(pin[GPIO_RDM6300_RX],-1,1);
    if (RDM6300_Serial->begin(RDM6300_BAUDRATE)) {
      if (RDM6300_Serial->hardwareSerial()) {
        ClaimSerial();
      }
    }
  }
  rdm_blcnt=0;
}

// 14 bytes payload; // RFID DATA FRAME FORMAT: 1byte head (value: 2), 10byte data (2byte version + 8byte tag), 2byte checksum, 1byte tail (value: 3)
void RDM6300_ScanForTag() {
  char rdm_buffer[14];
  uint8_t rdm_index;
  uint8_t rdm_array[6];

  if (!RDM6300_Serial) return;

  if (rdm_blcnt>0) {
    rdm_blcnt--;
    while (RDM6300_Serial->available()) RDM6300_Serial->read();
    return;
  }

  if (RDM6300_Serial->available()) {

    char c=RDM6300_Serial->read();
    if (c!=2) return;
    // head detected
    // read rest of message 11 more bytes
    rdm_index=0;
    uint32_t cmillis=millis();
    while (1) {
      if (RDM6300_Serial->available()) {
        char c=RDM6300_Serial->read();
        if (c==3) {
          // tail marker
          break;
        }
        rdm_buffer[rdm_index++]=c;
        if (rdm_index>13) {
          // illegal message
          return;
        }
      }
      if ((millis()-cmillis)>RDM_TIMEOUT) {
        // timeout
        return;
      }
    }

    // block for 2 seconds
    rdm_blcnt=RDM6300_BLOCK;

    // calc checksum,
    rm6300_hstring_to_array(rdm_array,sizeof(rdm_array),rdm_buffer);
    uint8_t accu=0;
    for (uint8_t count=0;count<5;count++) {
      accu^=rdm_array[count];
    }
    if (accu!=rdm_array[5]) {
      // checksum error
      return;
    }

    // copy 4 hex bytes
    memcpy(rdm_uid_str,&rdm_buffer[2],8);
    rdm_uid_str[9]=0;

    ResponseTime_P(PSTR(",\"RDM6300\":{\"UID\":\"%s\"}}"), rdm_uid_str);
    MqttPublishTeleSensor();
/*
    char command[24];
    sprintf(command,"event RDM6300=%s",rdm_uid_str);
    ExecuteCommand(command, SRC_RULE);
    */
  }


}

uint8_t rm6300_hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
    if (chr >= 'a' && chr <= 'f') rVal = chr + 10 - 'a';
  }
  return rVal;
}

// convert hex string to int array
void rm6300_hstring_to_array(uint8_t array[], uint8_t len, char buffer[])
{
  char *cp=buffer;
  for (uint8_t i = 0; i < len; i++) {
    uint8_t val = rm6300_hexnibble(*cp++) << 4;
    array[i]= val | rm6300_hexnibble(*cp++);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_RDM6300[] PROGMEM =
 "{s}RDM6300 " "UID" "{m}%s" "{e}";

void RDM6300_Show(void) {
  if (!RDM6300_Serial) return;
  if (!rdm_uid_str[0]) strcpy(rdm_uid_str,"????");
  WSContentSend_PD(HTTP_RDM6300,rdm_uid_str);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns51(byte function)
{
  bool result = false;

    switch (function) {
      case FUNC_INIT:
        RDM6300_Init();
        break;
      case FUNC_EVERY_100_MSECOND:
        RDM6300_ScanForTag();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        RDM6300_Show();
        break;
#endif  // USE_WEBSERVER
    }
    return result;
}

#endif  // USE_RDM6300
