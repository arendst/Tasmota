/*
  xdrv_77_wizmote.ino - WiZ Smart Remote decoder support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_WIZMOTE
/*********************************************************************************************\
 * WiZ WiFi ESP-NOW Smart Remote decoder
 *
 * Example rule
 * on wizmote#action do var1 %value% endon               - Store current button
 * on wizmote#id=12318994 do event action=%var1% endon   - Test for correct Id and execute event
 * on event#action=on do power 1 endon                   - On button ON do power 1
 * on event#action=off do power 0 endon                  - On button OFF do power 0
 * on event#action=bd do dimmer - endon                  - On button Brihtness - do dimmer - (10% decrease)
 * on event#action=bu do dimmer + endon                  - On button Brightness + do dimmer + (10% increase)
 * on event#action=bn do dimmer 20 endon                 - On button Moon do dimmer 20
\*********************************************************************************************/

#define XDRV_77              77

#ifndef WIZMOTE_CHANNEL
#define WIZMOTE_CHANNEL      1                   // WiZ Smart Remote ESP-NOW channel if WiFi is disabled
#endif

#include <QuickEspNow.h>

struct WizMote {
  uint32_t last_seq;
  uint32_t id;
  int rssi;
  uint8_t index;
  uint8_t battery_level;
  uint8_t active;
} WizMote;

/*********************************************************************************************\
 * WiZ Smart Remote
\*********************************************************************************************/

#define WIZMOTE_BUTTON_ON              1
#define WIZMOTE_BUTTON_OFF             2
#define WIZMOTE_BUTTON_NIGHT           3
#define WIZMOTE_BUTTON_BRIGHT_DOWN     8
#define WIZMOTE_BUTTON_BRIGHT_UP       9
#define WIZMOTE_BUTTON_ONE            16
#define WIZMOTE_BUTTON_TWO            17
#define WIZMOTE_BUTTON_THREE          18
#define WIZMOTE_BUTTON_FOUR           19
#define WIZSMART_BUTTON_ON           100
#define WIZSMART_BUTTON_OFF          101
#define WIZSMART_BUTTON_BRIGHT_UP    102
#define WIZSMART_BUTTON_BRIGHT_DOWN  103

//                                 01   2  3  4  5  6  7  8  9
const char kWMButtons[] PROGMEM = "|OFF|ON|BN|B1|B2|B3|B4|BU|BD";
const uint8_t sWMButtons[] PROGMEM = {
  WIZMOTE_BUTTON_ON, WIZMOTE_BUTTON_OFF, WIZMOTE_BUTTON_NIGHT,
  WIZMOTE_BUTTON_ONE, WIZMOTE_BUTTON_TWO, WIZMOTE_BUTTON_THREE, WIZMOTE_BUTTON_FOUR,
  WIZMOTE_BUTTON_BRIGHT_DOWN, WIZMOTE_BUTTON_BRIGHT_UP,
  WIZSMART_BUTTON_ON, WIZSMART_BUTTON_OFF,
  WIZSMART_BUTTON_BRIGHT_DOWN, WIZSMART_BUTTON_BRIGHT_UP
};
const uint8_t dWMButtons[] PROGMEM = {
  2, 1, 3,
  4, 5, 6, 7,
  9, 8,
  2, 1,
  9, 8
};

typedef struct WizMotePacket {
  union {
    uint8_t program;     // 0x91 for ON button, 0x81 for all others
    struct {
      uint8_t : 4;
      uint8_t pairing : 1;
      uint8_t : 3;
    };
  };
  uint32_t sequence;     // Incremental sequence number 32 bit unsigned integer LE
  uint8_t data_type1;    // Data type: button (32)
  uint8_t button;        // Identifies which button is being pressed
  uint8_t data_type2;    // Data type: batteryLevel (1)
  uint8_t battery_level; // WiZMote batteryLevel out of 0 .. 100
  uint8_t mac[4];        // CCM MAC (Message Authentication Code)
} __attribute__((packed)) WizMotePacket_t;

void WizMoteHandleRemoteData(uint8_t *mac, uint8_t *incoming_data, size_t len, signed int rssi) {
  // WiZ mac = 44:4F:8E:00:00:00 to 44:4F:8E:FF:FF:FF

  WizMotePacket_t *incoming = reinterpret_cast<WizMotePacket_t *>(incoming_data);

  if (len != sizeof(WizMotePacket_t)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIZ: ERROR Rcvd unknown message of length %d"), len);
    return;
  }

  if (incoming->sequence == WizMote.last_seq) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WIZ: WARNING Rcvd same sequence %d"), incoming->sequence);
    return;
  }

  WizMote.index = 0;
  for (uint32_t i = 0; i < 13; i++) {
    if (incoming->button == pgm_read_byte(sWMButtons + i)) {
      WizMote.index = pgm_read_byte(dWMButtons + i);
      break;
    }
  }
  if (0 == WizMote.index) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIZ: ERROR Rcvd invalid button code %d"), incoming->button);
    return;
  }
  WizMote.id = mac[5] | (mac[4] << 8) | (mac[3] << 16);  // WiZ mac block range
  WizMote.rssi = rssi;
  WizMote.battery_level = incoming->battery_level;
  WizMote.last_seq = incoming->sequence;
}

void WizMoteResponse(void) {
  // This needs to be executed here as it will panic if placed within the above Callback
  char text[5];
  Response_P(PSTR("{\"WiZMote\":{\"Id\":%d,\"Action\":\"%s\",\"RSSI\":%d,\"Battery\":%d}}"),
    WizMote.id, GetTextIndexed(text, sizeof(text), WizMote.index, kWMButtons), WizMote.rssi, WizMote.battery_level);
  if (Settings->flag6.mqtt_disable_publish ) {  // SetOption147 - If it is activated, Tasmota will not publish MQTT message, but it will proccess event trigger rules
    XdrvRulesProcess(0);
  } else {
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR("WIZMOTE"));
  }
  WizMote.index = 0;
}

/*********************************************************************************************\
 * ESP-NOW
\*********************************************************************************************/

void EspNowDataReceived(uint8_t* mac, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
  char _destMAC[18];
  ToHex_P(mac, 6, _destMAC, 18, ':');

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NOW: Rcvd %*_H, RSSI %d, MAC %s, %s"),
    len, data, rssi, _destMAC, broadcast ? "Broadcast" : "Unicast");

  if (data[0] == 0x91 || data[0] == 0x81 || data[0] == 0x80) {
    WizMoteHandleRemoteData(mac, data, len, rssi);
  }
}

void EspNowInit(void) {
  if (0 == Settings->flag4.network_wifi) {   // WiFi Off ..
    if (!TasmotaGlobal.global_state.wifi_down) { return; }  // .. and processed by WifiDisable()
    if (1 == WizMote.active) {
      WizMote.active--;                      // Add a second delay
      return;
    }
    if (WizMote.active != 2) {

//      AddLog(LOG_LEVEL_DEBUG, PSTR("NOW: Status %d, SSID '%s', Ch %d, IP %s, MAC %s"),
//        WiFi.status(), WiFi.SSID().c_str(), WiFi.channel(), WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());

      WiFi.mode(WIFI_STA);
#ifdef ESP32
      WiFi.disconnect(false, true);
#else
      WiFi.disconnect(false);
#endif  // ESP32

//      AddLog(LOG_LEVEL_DEBUG, PSTR("NOW: Status %d, SSID '%s', Ch %d, IP %s, MAC %s"),
//        WiFi.status(), WiFi.SSID().c_str(), WiFi.channel(), WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());

      quickEspNow.stop();
      if (quickEspNow.begin(WIZMOTE_CHANNEL)) {  // Specify channel if no connected WiFi
        quickEspNow.onDataRcvd(EspNowDataReceived);
        Settings->flag.global_state = 1;     // SetOption31 - (Wifi, MQTT) Control link led blinking (1)
        AddLog(LOG_LEVEL_INFO, PSTR("NOW: Started on channel " STR(WIZMOTE_CHANNEL)));
        WizMote.active = 2;
      }
    }
  } else {                                   // WiFi On
    if (TasmotaGlobal.global_state.wifi_down) {
      WizMote.active = 0;
    } 
    else if (WizMote.active != 1) {
      quickEspNow.stop();
      if (quickEspNow.begin()) {
        quickEspNow.onDataRcvd(EspNowDataReceived);
        AddLog(LOG_LEVEL_INFO, PSTR("NOW: Started"));
        WizMote.active = 1;
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv77(uint32_t function) {
  if (!Settings->flag6.wizmote_enabled) {   // SetOption164 - (WizMote) Enable WiZ Smart Remote support (1)
    if (WizMote.active) {
      quickEspNow.stop();
      AddLog(LOG_LEVEL_INFO, PSTR("NOW: Stopped"));
      WizMote.active = 0;
    }
    return false;
  }

  bool result = false;

  if (FUNC_EVERY_SECOND == function) {
    EspNowInit();
  }
  else if (WizMote.active) {
    switch (function) {
      case FUNC_LOOP:
        if (WizMote.index > 0) {
          WizMoteResponse();
        }
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_WIZMOTE
