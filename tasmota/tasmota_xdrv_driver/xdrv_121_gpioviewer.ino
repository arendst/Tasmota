/*
  xdrv_121_gpioviewer.ino - GPIOViewer for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_GPIO_VIEWER
/*********************************************************************************************\
 * GPIOViewer support
 * 
 * Open webpage <device_ip_address>:8080 and watch realtime GPIO states
\*********************************************************************************************/

#define XDRV_121              121

#define GV_PORT               5557
#define GV_SAMPLING_INTERVAL  100    // Relates to FUNC_EVERY_100_MSECOND

const char *GVRelease = "1.0.7";

#define GV_BASE_URL "https://thelastoutpostworkshop.github.io/microcontroller_devkit/gpio_viewer/assets/"

const char HTTP_GV_PAGE[] PROGMEM =
  "<!DOCTYPE HTML>"
  "<html>"
    "<head>"
    "<title>Tasmota GPIO State</title>"
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
  digitalPin = 0,
  PWMPin = 1,
  analogPin = 2
};

struct {
  WiFiClient WebClient;
  ESP8266WebServer *WebServer;
  uint32_t lastPinStates[MAX_GPIO_PIN];
  uint32_t resolution;
  uint32_t freeHeap;
  uint32_t freePSRAM;
  bool sse_ready;
  bool active;
} GV;

int GVReadGPIO(int gpioNum, uint32_t *originalValue, uint32_t *pintype) {
  uint32_t pin_type = GetPin(gpioNum) / 32;
/*  
  if (GPIO_NONE == pin_type) {
    *pintype = digitalPin;
    *originalValue = 0;
    return 0;
  }
*/
#ifdef ESP32
  int pwm_resolution = ledcReadDutyResolution(gpioNum);
  if (pwm_resolution > 0) {
    *pintype = PWMPin;
    *originalValue = ledcRead2(gpioNum);
    return changeUIntScale(*originalValue, 0, pwm_resolution, 0, 255);   // bring back to 0..255
  }
#endif  // ESP32

#ifdef ESP8266
  int pwm_value = AnalogRead(gpioNum);
  if (pwm_value > -1) {
    *pintype = PWMPin;
    *originalValue = pwm_value;
    int pwm_resolution = GV.resolution;
    return changeUIntScale(*originalValue, 0, pwm_resolution, 0, 255);   // bring back to 0..255
  }
#endif  // ESP8266

  else if (AdcPin(gpioNum)) {
    int adc_resolution = (1 << AdcResolution()) - 1;
    *originalValue = AdcRead(gpioNum, 2);
    *pintype = analogPin;
    return changeUIntScale(*originalValue, 0, adc_resolution, 0, 255);   // bring back to 0..255
  }

  *pintype = digitalPin;
  int value = digitalRead(gpioNum);
  *originalValue = value;
  if (value == 1) {
    return 256;
  }
  return 0;
}

void GVResetStatePins(void) {
  uint32_t originalValue;
  uint32_t pintype;
  AddLog(LOG_LEVEL_INFO, "IOV: GPIOViewer Connected, sampling interval is " STR(GV_SAMPLING_INTERVAL) "ms");

  for (uint32_t pin = 0; pin < MAX_GPIO_PIN; pin++) {
    GV.lastPinStates[pin] = GVReadGPIO(pin, &originalValue, &pintype);
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
#ifdef ESP8266
  // Can change on the fly
  uint32_t pwm_range = Settings->pwm_range + 1;
  GV.resolution = 0;
  while (pwm_range) {
    GV.resolution++;
    pwm_range >>= 1;
  }
#endif  // ESP8266

  uint32_t originalValue;
  uint32_t pintype;

  String jsonMessage = "{";
  bool hasChanges = false;
  for (uint32_t pin = 0; pin < MAX_GPIO_PIN; pin++) {
    int currentState = GVReadGPIO(pin, &originalValue, &pintype);

    if (originalValue != GV.lastPinStates[pin]) {
      if (hasChanges) {
        jsonMessage += ", ";
      }
      jsonMessage += "\"" + String(pin) + "\": {\"s\": " + currentState + ", \"v\": " + originalValue + ", \"t\": " + pintype + "}";
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
  }

#ifdef ESP32
  if (UsePSRAM()) {
    uint32_t psram = ESP.getFreePsram();
    if (psram != GV.freePSRAM) {
      GV.freePSRAM = psram;
      char temp[20];
      snprintf_P(temp, sizeof(temp), PSTR("%d KB"), psram / 1024);
      GVEventSend(temp, "free_psram", millis());
    }
  } else {
    GVEventSend("No PSRAM", "free_psram", millis());
  }
#endif  // ESP32
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
                                        WiFi.localIP().toString().c_str(), 
                                        WiFi.localIP().toString().c_str(), 
                                        ESP_getFreeSketchSpace() / 1024);
  if (content == nullptr) { return; }                      // Avoid crash

  GV.WebServer->send_P(200, "text/html", content);
  free(content);

  GV.sse_ready = false;                                    // Allow restart of updates on page load
}

void GVHandleRelease(void) {
  String jsonResponse = "{\"release\": \"" + String(GVRelease) + "\"}";

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
          GVBegin();
          GV.active = true;
        }
        break;
    }

  }
  return result;
}

#endif // USE_GPIO_VIEWER
