/*
  xsns_79_as608.ino - AS608 and R503 fingerprint sensor support for Tasmota

  Copyright (C) 2021  boaschti and Theo Arends

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

#ifdef USE_AS608
/*********************************************************************************************\
 * AS608 optical and R503 capacitive Fingerprint sensor
 *
 * Uses Adafruit-Fingerprint-sensor-library with TasmotaSerial
 *
 * Changes made to Adafruit_Fingerprint.h and Adafruit_Fingerprint.cpp:
 * - Replace SoftwareSerial with TasmotaSerial
 * - Add defined(ESP32) where also defined(ESP8266)
\*********************************************************************************************/

#define XSNS_79               79

//#define USE_AS608_MESSAGES

#define D_JSON_FPRINT "FPrint"

#define D_PRFX_FP "Fp"
#define D_CMND_FP_ENROLL "Enroll"
#define D_CMND_FP_DELETE "Delete"
#define D_CMND_FP_COUNT "Count"

const char kAs608Commands[] PROGMEM = D_PRFX_FP "|" D_CMND_FP_ENROLL "|" D_CMND_FP_DELETE "|" D_CMND_FP_COUNT;

void (*const As608Commands[])(void) PROGMEM = { &CmndFpEnroll, &CmndFpDelete, &CmndFpCount };

#ifdef USE_AS608_MESSAGES
const char kAs608Messages[] PROGMEM =
  D_DONE "|" D_FP_PACKETRECIEVEERR "|" D_FP_NOFINGER "|" D_FP_IMAGEFAIL "|" D_FP_UNKNOWNERROR "|" D_FP_IMAGEMESS "|" D_FP_FEATUREFAIL "|" D_FP_NOMATCH "|"
  D_FP_NOTFOUND "|" D_FP_ENROLLMISMATCH "|" D_FP_BADLOCATION "|" D_FP_DBRANGEFAIL "|" D_FP_UPLOADFEATUREFAIL "|" D_FP_PACKETRESPONSEFAIL "|"
  D_FP_UPLOADFAIL "|" D_FP_DELETEFAIL "|" D_FP_DBCLEARFAIL "|" D_FP_PASSFAIL "|" D_FP_INVALIDIMAGE "|" D_FP_FLASHERR "|" D_FP_INVALIDREG "|"
  D_FP_ADDRCODE "|" D_FP_PASSVERIFY;

const uint8_t As608Reference[] PROGMEM = { 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 4, 17, 4, 18, 4, 4, 19, 4, 20, 4, 4, 4, 4, 4, 21, 22 };
#else
const char kAs608Messages[] PROGMEM = D_DONE "|" D_FP_UNKNOWNERROR "|" D_FP_NOFINGER;
#endif

#include <TasmotaSerial.h>
#include <Adafruit_Fingerprint.h>

Adafruit_Fingerprint *As608Finger;
TasmotaSerial *As608Serial;

struct AS608 {
  bool selected = false;
  uint8_t enroll_step = 0;
  uint8_t model_number = 0;
} As608;

char* As608Message(char* response, uint32_t index) {
#ifdef USE_AS608_MESSAGES
  if (index > sizeof(As608Reference)) { index = 4; }
  uint32_t i = pgm_read_byte(&As608Reference[index]);
#else
  if (index > 2) { index = 1; }
  uint32_t i = index;
#endif
  return GetTextIndexed(response, TOPSZ, i, kAs608Messages);
}

void As608PublishMessage(const char* message) {
  char romram[TOPSZ];
  snprintf_P(romram, sizeof(romram), message);
  if (strlen(romram) > 0) {
    char json_name[20];
    if (As608.enroll_step) {
      strcpy_P(json_name, PSTR(D_PRFX_FP D_CMND_FP_ENROLL));
    } else {
      strcpy_P(json_name, PSTR(D_JSON_FPRINT));
    }
    Response_P(S_JSON_COMMAND_SVALUE, json_name, romram);
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, json_name);
  }
}

void As608Init(void) {
  if (PinUsed(GPIO_AS608_RX) && PinUsed(GPIO_AS608_TX)) {
    As608Serial = new TasmotaSerial(Pin(GPIO_AS608_RX), Pin(GPIO_AS608_TX), 0);
    As608Finger = new Adafruit_Fingerprint(As608Serial, 0);

    As608Finger->begin(57600);
    if (As608Serial->hardwareSerial()) { ClaimSerial(); }

    if (As608Finger->verifyPassword()) {
      As608Finger->getTemplateCount();
      AddLog_P(LOG_LEVEL_INFO, PSTR("AS6: Detected with %d fingerprint(s) stored"), As608Finger->templateCount);
      As608.selected = true;
    }
  }
}

int As608GetFingerImage(void) {
  int p = As608Finger->getImage();
  if (p != FINGERPRINT_OK) {
    char response[TOPSZ];
    As608PublishMessage(As608Message(response, p));
  }
  return p;
}

int As608ConvertFingerImage(uint8_t slot) {
  int p = As608Finger->image2Tz(slot);
  if (p != FINGERPRINT_OK) {
    char response[TOPSZ];
    As608PublishMessage(As608Message(response, p));
  }
  return p;
}

void As608Loop(void) {
  uint32_t p = 0;

  if (!As608.enroll_step) {
    // Search for Finger

//    As608Finger->LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_RED);
//    As608Finger->LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_BLUE);
//    As608Finger->LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);
//    As608Finger->LEDcontrol(0);

    p = As608Finger->getImage();          // Take image
    if (p != FINGERPRINT_OK) { return; }

    p = As608Finger->image2Tz();          // Convert image
    if (p != FINGERPRINT_OK) { return; }

//    p = As608Finger->fingerFastSearch();  // Match found - fails on R503
    p = As608Finger->fingerSearch();      // Match found
    if (p != FINGERPRINT_OK) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR("AS6: No matching finger"));
      return;
    }

    // Found a match
    Response_P(PSTR("{\"" D_JSON_FPRINT "\":{\"" D_JSON_ID "\":%d,\"" D_JSON_CONFIDENCE "\":%d}}"), As608Finger->fingerID, As608Finger->confidence);
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_JSON_FPRINT));
    return;
  } else {
    // enroll is active
    switch (As608.enroll_step) {
      case 1:
        As608PublishMessage(PSTR(D_FP_ENROLL_PLACEFINGER));
//        As608Finger->LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
        As608.enroll_step++;
        break;
      case 2:
        // get first image
        if (As608GetFingerImage() == FINGERPRINT_OK) {
          As608.enroll_step++;
        }
        break;
      case 3:
        // convert image
        if (As608ConvertFingerImage(1) == FINGERPRINT_OK) {
          As608.enroll_step++;
        } else {
          As608.enroll_step--;
        }
        break;
      case 4:
        As608PublishMessage(PSTR(D_FP_ENROLL_REMOVEFINGER));
        As608.enroll_step++;
        break;
      case 5:
        // Remove finger
        p = As608Finger->getImage();
        if (p == FINGERPRINT_NOFINGER) {
          As608.enroll_step++;
        }
        break;
      case 6:
        As608PublishMessage(PSTR(D_FP_ENROLL_PLACESAMEFINGER));
//        As608Finger->LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);
        As608.enroll_step++;
        break;
      case 7:
        // get second image of finger
        if (As608GetFingerImage() == FINGERPRINT_OK) {
          As608.enroll_step++;
        }
        break;
      case 8:
        // convert second image
        if (As608ConvertFingerImage(2) != FINGERPRINT_OK) {
          As608PublishMessage(PSTR(D_FP_ENROLL_RETRY));
          As608.enroll_step -= 2;
          break;
        }
        // Create model
        p = As608Finger->createModel();
        if (p != FINGERPRINT_OK) {
          char response[TOPSZ];
          As608PublishMessage(As608Message(response, p));
          As608.enroll_step = 99;
          break;
        }
        // Store model
        p = As608Finger->storeModel(As608.model_number);
        char response[TOPSZ];
        As608PublishMessage(As608Message(response, p));
        if (p == FINGERPRINT_OK) {
          As608.enroll_step = 0;
          As608.model_number = 0;
        } else {
          As608.enroll_step = 99;
        }
        break;
      case 99:
        As608PublishMessage(PSTR(D_FP_ENROLL_RESTART));
        As608.enroll_step = 1;
        break;
      default:
        As608PublishMessage(PSTR(D_FP_ENROLL_ERROR));
        As608.enroll_step = 0;
        As608.model_number = 0;
        break;
      }
  }
}

/*********************************************************************************************\
* Commands
\*********************************************************************************************/

void CmndFpEnroll(void) {
  if (As608.enroll_step) {
    if (0 == XdrvMailbox.payload) {
      // FpEnroll 0 - Stop enrollement
      As608.enroll_step = 0;
      ResponseCmndChar_P(PSTR(D_FP_ENROLL_RESET));
    } else {
      // FpEnroll - Enrollement state
      ResponseCmndChar_P(PSTR(D_FP_ENROLL_ACTIVE));
    }
  } else {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 128)) {
      // FpEnroll 1..128 - Start enrollement into slot x
      As608.enroll_step = 1;
      As608.model_number = XdrvMailbox.payload;
      ResponseClear();  // Will use loop start message
    } else {
      // FpEnroll - Enrollement state
      ResponseCmndChar_P(PSTR(D_FP_ENROLL_INACTIVE));
    }
  }
}

void CmndFpDelete(void) {
  if (0 == XdrvMailbox.payload) {
    // FpDelete 0 - Clear database
    As608Finger->emptyDatabase();
    As608Finger->getTemplateCount();
    if (As608Finger->templateCount) {
      ResponseCmndChar_P(PSTR(D_FP_DBCLEARFAIL));
    } else {
      ResponseCmndDone();
    }
  }
  else if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 128)) {
    // FpDelete 1..128 - Delete single entry from database
    int p = As608Finger->deleteModel(XdrvMailbox.payload);
    char response[TOPSZ];
    ResponseCmndChar(As608Message(response, p));
  }
}

void CmndFpCount(void) {
  // FpCount - Show number of slots used
  As608Finger->getTemplateCount();
  ResponseCmndNumber(As608Finger->templateCount);
}

/*********************************************************************************************\
* Interface
\*********************************************************************************************/

bool Xsns79(uint8_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    As608Init();
  }
  else if (As608.selected) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        As608Loop();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kAs608Commands, As608Commands);
        break;
    }
  }
  return result;
}

#endif  // USE_AS608
