/*
  xdrv_121_gpioviewer.ino - GPIOViewer for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends, Stephan Hadinger and Charles Giguere

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_GPIO_VIEWER
/*********************************************************************************************\
 * GPIOViewer support based on work by Charles Giguere
 * 
 * Resources:
 *       GPIO Viewer: https://github.com/thelastoutpostworkshop/gpio_viewer
 * Server Sent Event: https://github.com/esp8266/Arduino/issues/7008
\*********************************************************************************************/

#define XDRV_121              121

#define GV_PORT               5557
#define GV_SAMPLING_INTERVAL  100    // milliseconds - Relates to FUNC_EVERY_100_MSECOND
#define GV_KEEP_ALIVE         1000   // milliseconds - If no activity after this do a heap size event anyway

#define GV_BASE_URL "https://thelastoutpostworkshop.github.io/microcontroller_devkit/gpio_viewer/assets/"

const char *GVRelease = "1.0.8";

const char HTTP_GV_PAGE[] PROGMEM =
  "<!DOCTYPE HTML>"
  "<html>"
    "<head>"
    "<title>%s - GPIO Viewer</title>"
    "<base href='" GV_BASE_URL "'>"
    "<link id='defaultStyleSheet' rel='stylesheet' href=''>"
    "<link id='boardStyleSheet' rel='stylesheet' href=''>"
    "<link rel='icon' href='favicon.ico' type='image/x-icon'>"
    "<script src='script/webSocket.js'></script>"
    "<script src='script/boardSwitcher.js'></script>"
    "<script>"
      "var serverPort=" STR(GV_PORT) ";"
      "var ip='%s';"                                                      // WiFi.localIP().toString().c_str()
      "var source=new EventSource('http://%s:" STR(GV_PORT) "/events');"  // WiFi.localIP().toString().c_str()
      "var sampling_interval='" STR(GV_SAMPLING_INTERVAL) "';"
#ifdef ESP32
      "var psramSize='%d KB';"                                            // ESP.getPsramSize() / 1024
#endif  // ESP32
      "var freeSketchSpace='%d KB';"                                      // ESP_getFreeSketchSpace() / 1024
    "</script>"
  "</head>"
  "<body>"
    "<div class='grid-container'>"
      "<div id='messageBox' class='message-box hidden'></div>"
      "<header class='header'></header>"
      // Image
      "<div class='image-container'>"
        "<div id='imageWrapper' class='image-wrapper'>"
          "<img id='boardImage' src='' alt='Board Image'>"
          "<div id='indicators'></div>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
  "</html>";

const char HTTP_GV_EVENT[] PROGMEM =
  "HTTP/1.1 200 OK\n"
  "Content-Type: text/event-stream;\n"
  "Connection: keep-alive\n"
  "Cache-Control: no-cache\n"
  "Access-Control-Allow-Origin: *\n\n";

const char HTTP_BTN_MENU_GV[] PROGMEM =
  "<p><form action='http://%s:" STR(GV_PORT) "/' method='post' target='_blank'><button>" D_GPIO_VIEWER "</button></form></p>";

enum GVPinTypes {
  GV_DigitalPin = 0,
  GV_PWMPin = 1,
  GV_AnalogPin = 2
};

struct {
  WiFiClient WebClient;
  ESP8266WebServer *WebServer;
  uint32_t lastPinStates[MAX_GPIO_PIN];
  uint32_t lastSentWithNoActivity;
  uint32_t freeHeap;
  uint32_t freePSRAM;
  bool sse_ready;
  bool active;
} GV;

int GVReadGPIO(uint32_t pin, uint32_t *pintype) {
  uint32_t pin_type = GetPin(pin) / 32;
/*  
  if (GPIO_NONE == pin_type) {
    *pintype = GV_DigitalPin;
    return 0;
  }
*/
#ifdef ESP32
  int pwm_resolution = ledcReadDutyResolution(pin);
  if (pwm_resolution > 0) {
    *pintype = GV_PWMPin;
    return ledcRead2(pin);
  }
#endif  // ESP32

#ifdef ESP8266
  int pwm_value = AnalogRead(pin);
  if (pwm_value > -1) {
    *pintype = GV_PWMPin;
    return pwm_value;
  }
#endif  // ESP8266

  else if (AdcPin(pin)) {
    *pintype = GV_AnalogPin;
    return AdcRead(pin, 2);
  }

  *pintype = GV_DigitalPin;
  int value = digitalRead(pin);
  if (value == 1) {
    return 256;
  }
  return 0;
}

void GVResetStatePins(void) {
  AddLog(LOG_LEVEL_INFO, "IOV: GPIOViewer Connected, sampling interval is " STR(GV_SAMPLING_INTERVAL) "ms");

  uint32_t pintype;
  for (uint32_t pin = 0; pin < MAX_GPIO_PIN; pin++) {
    GV.lastPinStates[pin] = GVReadGPIO(pin, &pintype);
  }
}

//void GVEventSend(const char *message, const char *event=NULL, uint32_t id=0, uint32_t reconnect=0);
void GVEventSend(const char *message, const char *event, uint32_t id) {
  if (GV.WebClient.connected()) {
    // generateEventMessage() in AsyncEventSource.cpp
//    GV.WebClient.printf_P(PSTR("retry: 0\r\nid: %u\r\nevent: %s\r\ndata: %s\r\n\r\n"), id, event, message);
    GV.WebClient.printf_P(PSTR("id: %u\r\nevent: %s\r\ndata: %s\r\n\r\n"), id, event, message);
  }
}

// Monitor GPIO Values
void GVMonitorTask(void) {
  uint32_t pintype;
  bool hasChanges = false;
  String jsonMessage = "{";
  for (uint32_t pin = 0; pin < MAX_GPIO_PIN; pin++) {
    int currentState = GVReadGPIO(pin, &pintype);
    if (currentState != GV.lastPinStates[pin]) {
      if (hasChanges) { jsonMessage += ","; }
      jsonMessage += "\"" + String(pin) + "\":{\"s\":" + currentState + ",\"v\":" + GV.lastPinStates[pin] + ",\"t\":" + pintype + "}";
      GV.lastPinStates[pin] = currentState;
      hasChanges = true;
    }
  }
  jsonMessage += "}";
  if (hasChanges) {
    GVEventSend(jsonMessage.c_str(), "gpio-state", millis());
  }

  uint32_t heap = ESP_getFreeHeap();
  if (heap != GV.freeHeap) {
    GV.freeHeap = heap;
    char temp[20];
    snprintf_P(temp, sizeof(temp), PSTR("%d KB"), heap / 1024);
    GVEventSend(temp, "free_heap", millis());
    hasChanges = true;
  }

#ifdef ESP32
  if (UsePSRAM()) {
    uint32_t psram = ESP.getFreePsram();
    if (psram != GV.freePSRAM) {
      GV.freePSRAM = psram;
      char temp[20];
      snprintf_P(temp, sizeof(temp), PSTR("%d KB"), psram / 1024);
      GVEventSend(temp, "free_psram", millis());
      hasChanges = true;
    }
  }
#endif  // ESP32

  if (!hasChanges) {
    uint32_t last_sent = millis() - GV.lastSentWithNoActivity;
    if (last_sent > GV_KEEP_ALIVE) {
      // No activity, resending for pulse
      char temp[20];
      snprintf_P(temp, sizeof(temp), PSTR("%d KB"), heap / 1024);
      GVEventSend(temp, "free_heap", millis());
      GV.lastSentWithNoActivity = millis();
    }
  } else {
    GV.lastSentWithNoActivity = millis();
  }
}

void GVBegin(void) {
  GV.WebServer = new ESP8266WebServer(GV_PORT);
  // Set CORS headers for global responses
  GV.WebServer->sendHeader("Access-Control-Allow-Origin", "*");
  GV.WebServer->sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  GV.WebServer->sendHeader("Access-Control-Allow-Headers", "Content-Type");
  GV.WebServer->on("/events", GVHandleEvents);
  GV.WebServer->on("/", GVHandleRoot);
  GV.WebServer->on("/release", GVHandleRelease);
#ifdef ESP32
  GV.WebServer->on("/free_psram", GVHandleFreePSRam);
#endif  // ESP32
  GV.WebServer->begin();
}

void GVHandleEvents(void) {
  GVResetStatePins();

  GV.WebClient = GV.WebServer->client();
  GV.WebClient.setNoDelay(true);
//  GV.WebClient.setSync(true);

  GV.WebServer->setContentLength(CONTENT_LENGTH_UNKNOWN);  // The payload can go on forever
  GV.WebServer->sendContent_P(HTTP_GV_EVENT);

  GV.sse_ready = true;                                     // Ready for async updates
}

void GVHandleRoot(void) {
  char* content = ext_snprintf_malloc_P(HTTP_GV_PAGE, 
                                        SettingsTextEscaped(SET_DEVICENAME).c_str(),
                                        WiFi.localIP().toString().c_str(), 
                                        WiFi.localIP().toString().c_str(), 
#ifdef ESP32
                                        ESP.getPsramSize() / 1024,
#endif  // ESP32
                                        ESP_getFreeSketchSpace() / 1024);
  if (content == nullptr) { return; }                      // Avoid crash
  GV.WebServer->send_P(200, "text/html", content);
  free(content);

  GV.sse_ready = false;                                    // Allow restart of updates on page load
}

void GVHandleRelease(void) {
  String jsonResponse = "{\"release\":\"" + String(GVRelease) + "\"}";
  GV.WebServer->send(200, "application/json", jsonResponse);
}

void GVHandleFreePSRam(void) {
  String jsonResponse = "{\"freePSRAM\":\"";
#ifdef ESP32
  if (UsePSRAM()) {
    jsonResponse += String(ESP.getFreePsram() / 1024) + " KB\"}";
  } else
#endif
    jsonResponse += "No PSRAM\"}";
  GV.WebServer->send(200, "application/json", jsonResponse);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv121(uint32_t function) {
  bool result = false;

  if (GV.active) {
    switch (function) {
      case FUNC_LOOP:
        if (GV.WebServer) { GV.WebServer->handleClient(); }
        break;
      case FUNC_EVERY_100_MSECOND:
        if (GV.sse_ready) { GVMonitorTask(); }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_MANAGEMENT_BUTTON:
        if (XdrvMailbox.index) {
          XdrvMailbox.index++;
        } else {
          WSContentSend_P(HTTP_BTN_MENU_GV, WiFi.localIP().toString().c_str());
        }
        break;
#endif // USE_WEBSERVER
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  } else {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (!TasmotaGlobal.global_state.network_down) {
          // Add delay to finish network setup
          static uint32_t gv_delay = 3;
          if (gv_delay) {
            gv_delay--;
          } else {
            GVBegin();
            GV.active = true;
          }
        }
        break;
    }

  }
  return result;
}

#endif // USE_GPIO_VIEWER
