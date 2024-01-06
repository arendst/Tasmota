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

#define GV_PORT               8080
#define GV_SAMPLING_INTERVAL  100    // Relates to FUNC_EVERY_100_MSECOND

const char *GVRelease = "1.0.5";

#define GV_BASE_URL "https://thelastoutpostworkshop.github.io/microcontroller_devkit/gpio_viewer/assets/"

#ifdef ESP32
const int GVMaxGPIOPins = 49;
// Global variables to capture PMW pins
const int GVMaxChannels = 64;
#endif  // ESP32
#ifdef ESP8266
const int GVMaxGPIOPins = 18;
// Global variables to capture PMW pins
const int GVMaxChannels = MAX_PWMS;
#endif  // ESP8266

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
      "var freeSketchSpace='%d';"                                         // GV.freeRAM
    "</script>"
  "</head>"
  "<body>"
    "<div class='grid-container'>"
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

enum GVPinTypes {
  digitalPin = 0,
  PWMPin = 1,
  analogPin = 2
};

struct {
  WiFiClient WebClient;
  ESP8266WebServer *WebServer;
  int freeHeap;
  uint32_t lastPinStates[GVMaxGPIOPins];
  int ledcChannelPin[GVMaxChannels][2];
  int ledcChannelPinCount;
  int ledcChannelResolution[GVMaxChannels][2];
  int ledcChannelResolutionCount;
  bool first;
  bool active;
} GV;

String GVFormatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + " B";
  }
  else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0, 2) + " KB";
  }
  else {
    return String(bytes / 1024.0 / 1024.0, 2) + " MB";
  }
}

void GVPrintPWNTraps(void) {
#ifdef ESP32    
  for (uint32_t pin = 0; pin < GVMaxChannels; pin++) {
    int32_t channel = analogGetChannel2(pin);
    if (channel > -1) {
      GV.ledcChannelPin[GV.ledcChannelPinCount][0] = pin;
      GV.ledcChannelPin[GV.ledcChannelPinCount++][1] = channel;
      uint8_t resolution = ledcReadResolution(channel);
      GV.ledcChannelResolution[GV.ledcChannelResolutionCount][0] = channel;
      GV.ledcChannelResolution[GV.ledcChannelResolutionCount++][1] = resolution;
    }
  }
#endif  // ESP32
#ifdef ESP8266
  uint32_t pwm_range = Settings->pwm_range + 1;
  uint32_t resolution = 0;
  while (pwm_range) {
    resolution++;
    pwm_range >>= 1;
  }
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1, i)) {
      int32_t channel = i;
      GV.ledcChannelPin[GV.ledcChannelPinCount][0] = Pin(GPIO_PWM1, i);
      GV.ledcChannelPin[GV.ledcChannelPinCount++][1] = channel;
      GV.ledcChannelResolution[GV.ledcChannelResolutionCount][0] = channel;
      GV.ledcChannelResolution[GV.ledcChannelResolutionCount++][1] = resolution;
    }
  }
#endif  // ESP8266

  AddLog(LOG_LEVEL_DEBUG, "IOV: %d pins are PWM", GV.ledcChannelPinCount);
  for (int i = 0; i < GV.ledcChannelPinCount; i++) {
    AddLog(LOG_LEVEL_DEBUG, "IOV: pin %d is using channel %d", GV.ledcChannelPin[i][0], GV.ledcChannelPin[i][1]);
  }
  AddLog(LOG_LEVEL_DEBUG, "IOV: %d channels are used", GV.ledcChannelResolutionCount);
  for (int i = 0; i < GV.ledcChannelResolutionCount; i++) {
    AddLog(LOG_LEVEL_DEBUG, "IOV: channel %d resolution is %d bits", GV.ledcChannelResolution[i][0], GV.ledcChannelResolution[i][1]);
  }
}

int GVGetLedcChannelForPin(int pin) {
  for (int i = 0; i < GV.ledcChannelPinCount; i++) {
    if (GV.ledcChannelPin[i][0] == pin) {
      return GV.ledcChannelPin[i][1];
    }
  }
  return -1; // Pin not found, return -1 to indicate no channel is associated
}

int GVGetChannelResolution(int channel) {
  for (int i = 0; i < GV.ledcChannelResolutionCount; i++) {
    if (GV.ledcChannelResolution[i][0] == channel) {
      return GV.ledcChannelResolution[i][1];
    }
  }
  return -1; // Pin not found, return -1 to indicate no channel is associated
}

int GVMapLedcReadTo8Bit(int channel, uint32_t *originalValue) {
  uint32_t maxDutyCycle = (1 << GVGetChannelResolution(channel)) - 1;

#ifdef ESP32
  *originalValue = ledcRead(channel);
#endif  // ESP32
#ifdef ESP8266
  if (17 == channel) {
    maxDutyCycle = (1 << 10) - 1;  // 10 = ANALOG_RESOLUTION
    *originalValue = AdcRead(channel, 2);
  } else {
    *originalValue = (channel < MAX_PWMS_LEGACY) ? Settings->pwm_value[channel] : Settings->pwm_value_ext[channel - MAX_PWMS_LEGACY];
  }
#endif

  return map(*originalValue, 0, maxDutyCycle, 0, 255);
}

int GVReadGPIO(int gpioNum, uint32_t *originalValue, uint32_t *pintype) {
  int channel = GVGetLedcChannelForPin(gpioNum);
  int value;
  if (channel != -1) {
    // This is a PWM Pin
    value = GVMapLedcReadTo8Bit(channel, originalValue);
    *pintype = PWMPin;
    return value;
  }
#ifdef ESP32
  uint8_t analogChannel = analogGetChannel2(gpioNum);
  if (analogChannel != 0 && analogChannel != 255) {
#endif  // ESP32
#ifdef ESP8266
  uint8_t analogChannel = gpioNum;
  if (17 == analogChannel) {
#endif  // ESP8266
    // This is an analog pin
    // Serial.printf("A Pin %d value=%d,channel=%d\n", gpioNum, value,analogChannel);

    value = GVMapLedcReadTo8Bit(analogChannel, originalValue);
    *pintype = analogPin;
    return value;
  }
  else {
    // This is a digital pin
    *pintype = digitalPin;
    value = digitalRead(gpioNum);
    *originalValue = value;
    if (value == 1) {
      return 256;
    }
    return 0;
  }
}

void GVResetStatePins(void) {
  uint32_t originalValue;
  uint32_t pintype;
  AddLog(LOG_LEVEL_INFO, "IOV: GPIOViewer Connected, sampling interval is " STR(GV_SAMPLING_INTERVAL) "ms");

  for (int i = 0; i < GVMaxGPIOPins; i++) {
    GV.lastPinStates[i] = GVReadGPIO(i, &originalValue, &pintype);
  }
}

//void GVEventsSend(const char *message, const char *event=NULL, uint32_t id=0, uint32_t reconnect=0);
void GVEventsSend(const char *message, const char *event, uint32_t id) {
  if (GV.WebClient.connected()) {
    // generateEventMessage() in AsyncEventSource.cpp
//    GV.WebClient.printf_P(PSTR("retry: 0\r\nid: %u\r\nevent: %s\r\ndata: %s\r\n\r\n"), id, event, message);
    GV.WebClient.printf_P(PSTR("id: %u\r\nevent: %s\r\ndata: %s\r\n\r\n"), id, event, message);
  }
}

// Monitor GPIO Values
void GVMonitorTask(void) {
  uint32_t originalValue;
  uint32_t pintype;

  String jsonMessage = "{";
  bool hasChanges = false;

  for (int i = 0; i < GVMaxGPIOPins; i++) {
    int currentState = GVReadGPIO(i, &originalValue, &pintype);

    if (originalValue != GV.lastPinStates[i]) {
    if (hasChanges) {
        jsonMessage += ", ";
    }
    jsonMessage += "\"" + String(i) + "\": {\"s\": " + currentState + ", \"v\": " + originalValue + ", \"t\": " + pintype + "}";
    GV.lastPinStates[i] = currentState;
    hasChanges = true;
    }
  }

  jsonMessage += "}";

  if (hasChanges) {
//    events->send(jsonMessage.c_str(), "gpio-state", millis());
    GVEventsSend(jsonMessage.c_str(), "gpio-state", millis());
  }

  uint32_t heap = ESP_getFreeHeap();
  if (heap != GV.freeHeap) {
    GV.freeHeap = heap;
//    events->send(GVFormatBytes(GV.freeHeap).c_str(), "free_heap", millis());
    GVEventsSend(GVFormatBytes(GV.freeHeap).c_str(), "free_heap", millis());
  }
}

void GVBegin(void) {
  GVPrintPWNTraps();

  GV.WebServer = new ESP8266WebServer(GV_PORT);

//  GV.WebServer->setContentLength(CONTENT_LENGTH_UNKNOWN);  // the payload can go on forever

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
  if (!GV.first) {
    GVResetStatePins();
    GV.first = true;

    GV.WebClient = GV.WebServer->client();
    GV.WebClient.setNoDelay(true);
//    GV.WebClient.setSync(true);

    GV.WebServer->setContentLength(CONTENT_LENGTH_UNKNOWN);  // The payload can go on forever
    GV.WebServer->sendContent_P(PSTR("HTTP/1.1 200 OK\nContent-Type: text/event-stream;\nConnection: keep-alive\nCache-Control: no-cache\nAccess-Control-Allow-Origin: *\n\n"));
  }
}

void GVHandleRoot(void) {
  char* content = ext_snprintf_malloc_P(HTTP_GV_PAGE, 
                                        WiFi.localIP().toString().c_str(), 
                                        WiFi.localIP().toString().c_str(), 
                                        GVFormatBytes(ESP.getFreeSketchSpace()).c_str());
  if (content == nullptr) { return; }              // Avoid crash

  GV.WebServer->send_P(200, "text/html", content);
  free(content);
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
        if (GV.first) { GVMonitorTask(); }
        break;
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
