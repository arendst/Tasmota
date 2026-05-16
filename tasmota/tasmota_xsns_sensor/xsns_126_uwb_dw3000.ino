/*
 * xsns_126_uwb_dw3000.ino
 * Tasmota driver — UWB Ranging via Qorvo DW3000
 *
 * Licence : Apache 2.0 (compatible Tasmota)
 *
 * Supported hardware:
 *  - Makerfabs ESP32 UWB DW3000 (WROOM)
 *  - Any ESP32 + DW3000 module with standard SPI
 *
 * Available Tasmota commands:
 *  UWBMode 0|1|2         → 0=off, 1=tag (initiator), 2=anchor (responder)
 *  UWBAnchorId 0-3       → ANCHOR: identifies this anchor (address = 0x0002+id)
 *  UWBAnchorCount 1-4    → TAG: number of anchors to range in round-robin
 *  UWBInterval <ms>      → Ranging interval (default 200 ms)
 *  UWBChannel 5|9        → UWB radio channel
 *  UWBAntennaDelay <tks> → Antenna delay in DW3000 ticks (default 16384)
 *  UWBFilterWindow 1-10  → Sliding average window size per anchor (default 3)
 *
 * JSON MQTT format (TAG, 2 anchors):
 *  {"UWB":{"Mode":"tag","Channel":5,"Anchor0":234.5,"Anchor1":187.2}}
 *
 * JSON MQTT format (TAG, 1 anchor):
 *  {"UWB":{"Mode":"tag","AnchorId":0,"Distance":234.5,"Unit":"cm","Method":"DS-TWR","Channel":5}}
 *
 * JSON MQTT format (ANCHOR):
 *  {"UWB":{"Mode":"anchor","AnchorId":0,"Distance":234.5,"Unit":"cm","Method":"DS-TWR","Channel":5}}
 *
 * Flash persistence:
 *  Settings->free_eb0[0..10] (reserved block in TSettings)
 *  [0] autostart mode (0=off 1=tag 2=anchor)
 *  [1] channel (5 or 9)
 *  [2..3] intervalMs uint16_t LE
 *  [4..5] antennaDelay uint16_t LE (ticks)
 *  [6] magic 0xAD (initialization sentinel)
 *  [7] flags: bit0 = mqttRealtime
 *  [8] anchorId (ANCHOR only, 0-3)
 *  [9] anchorCount (TAG only, 1-4)
 *  [10] filterWindow (1-10, default 3)
 */

#ifdef USE_UWB_DW3000

#include "DW3000Driver.h"
#include "DW3000Ranging.h"

#define XSNS_126  126

#define D_CMND_UWB_MODE        "UWBMode"
#define D_CMND_UWB_ANCHOR_ID   "UWBAnchorId"
#define D_CMND_UWB_ANCHOR_CNT  "UWBAnchorCount"
#define D_CMND_UWB_INTERVAL    "UWBInterval"
#define D_CMND_UWB_CHANNEL     "UWBChannel"
#define D_CMND_UWB_ANTENNA_DLY "UWBAntennaDelay"
#define D_CMND_UWB_FILTER_WIN  "UWBFilterWindow"

#define UWB_SETTINGS_MAGIC  0xAD
#define UWB_FILTER_MAX_WINDOW 10

// Sliding average per anchor (ring buffer of size UWB_FILTER_MAX_WINDOW)
struct UwbFilter {
  float   buf[UWB_FILTER_MAX_WINDOW] = {};
  uint8_t total = 0;
  uint8_t head  = 0;

  void reset() { total = 0; head = 0; }

  float push(float v, uint8_t window) {
    buf[head] = v;
    head = (head + 1) % UWB_FILTER_MAX_WINDOW;
    if (total < UWB_FILTER_MAX_WINDOW) total++;
    uint8_t n = (total < window) ? total : window;
    float sum = 0;
    for (uint8_t i = 0; i < n; i++)
      sum += buf[(head - 1 - i + UWB_FILTER_MAX_WINDOW) % UWB_FILTER_MAX_WINDOW];
    return sum / n;
  }
};

static inline uint8_t* UwbFlash() { return &Settings->free_eb0[0]; }

// ─── Driver globals ───────────────────────────────────────────────────────────
namespace UwbDW3000 {

  DW3000Driver  driver;
  DW3000Ranging ranging(driver);

  bool initialized   = false;
  bool enabled       = false;

  uint8_t  mode         = UWB_ROLE_OFF;
  uint8_t  anchorId     = 0;    // ANCHOR: identifies this anchor (address = 0x0002+anchorId)
  uint8_t  anchorCount  = 1;    // TAG: number of anchors to range in round-robin
  uint32_t intervalMs   = 200;
  uint8_t  channel      = 5;
  uint16_t antennaDelay  = 16384;
  bool     autostart     = false;
  bool     mqttRealtime  = false;
  uint8_t  filterWindow  = 3;

  UWBRangingResult lastResults[UWB_MAX_ANCHORS];
  UwbFilter        filters[UWB_MAX_ANCHORS];

}  // namespace UwbDW3000

// ─── Flash persistence ────────────────────────────────────────────────────────

void UwbDW3000LoadSettings() {
  using namespace UwbDW3000;
  uint8_t* f = UwbFlash();
  if (f[6] != UWB_SETTINGS_MAGIC) return;

  uint8_t savedRole = f[0];
  if (savedRole <= 2) {
    mode      = savedRole;
    autostart = (savedRole != UWB_ROLE_OFF);
  }
  uint8_t savedCh = f[1];
  if (savedCh == 5 || savedCh == 9) channel = savedCh;

  uint16_t savedIv = (uint16_t)f[2] | ((uint16_t)f[3] << 8);
  if (savedIv >= 50 && savedIv <= 10000) intervalMs = savedIv;

  uint16_t savedAd = (uint16_t)f[4] | ((uint16_t)f[5] << 8);
  if (savedAd >= 10000 && savedAd <= 32767) antennaDelay = savedAd;

  mqttRealtime = (f[7] & 0x01) != 0;

  uint8_t savedAi = f[8];
  if (savedAi <= 3) anchorId = savedAi;

  uint8_t savedAc = f[9];
  if (savedAc >= 1 && savedAc <= UWB_MAX_ANCHORS) anchorCount = savedAc;

  uint8_t savedFw = f[10];
  if (savedFw >= 1 && savedFw <= UWB_FILTER_MAX_WINDOW) filterWindow = savedFw;
}

void UwbDW3000SaveSettings() {
  using namespace UwbDW3000;
  uint8_t* f = UwbFlash();
  f[0] = autostart ? mode : (uint8_t)UWB_ROLE_OFF;
  f[1] = channel;
  f[2] = (uint8_t)(intervalMs & 0xFF);
  f[3] = (uint8_t)(intervalMs >> 8);
  f[4] = (uint8_t)(antennaDelay & 0xFF);
  f[5] = (uint8_t)(antennaDelay >> 8);
  f[6] = UWB_SETTINGS_MAGIC;
  f[7] = mqttRealtime ? 0x01 : 0x00;
  f[8] = anchorId;
  f[9] = (uint8_t)anchorCount;
  f[10] = filterWindow;
  SettingsSave(0);
}

// ─── Initialization ───────────────────────────────────────────────────────────

void UwbDW3000HardwareInit() {
  using namespace UwbDW3000;

  int csPin  = Pin(GPIO_UWB_CS);
  int rstPin = Pin(GPIO_UWB_RST);
  int irqPin = Pin(GPIO_UWB_IRQ);

  if (csPin < 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("UWB: CS pin not configured in template"));
    return;
  }

  enabled = false;
  driver = DW3000Driver(csPin, rstPin, irqPin);
  driver.begin();
  driver.hardReset();
  delay(10);

  if (!driver.checkConnection()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("UWB: DW3000 not found (Device ID mismatch)"));
    initialized = false;
    return;
  }

  DW3000Config cfg;
  cfg.channel = (channel == 9) ? UWB_CHANNEL_9 : UWB_CHANNEL_5;

  if (!driver.init(cfg)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("UWB: DW3000 init failed"));
    initialized = false;
    return;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("UWB: DW3000 initialized, channel %d"), channel);
  initialized = true;

  if (mode != UWB_ROLE_OFF) {
    UwbDW3000StartRanging();
  }
}

void UwbDW3000Init() {
  UwbDW3000LoadSettings();
  UwbDW3000HardwareInit();
}

void UwbDW3000StartRanging() {
  using namespace UwbDW3000;
  if (!initialized) return;

  ranging.setRole((UWBNodeRole)mode);
  ranging.setRangingInterval(intervalMs);
  ranging.setAntennaDelay((float)antennaDelay);
  if (mode == UWB_ROLE_TAG) {
    ranging.setNodeAddress(0x0001);
    ranging.setAnchorAddress(0x0002);           // reset the anchor list
    for (uint8_t i = 1; i < anchorCount; i++)
      ranging.addAnchorAddress(0x0002 + i);     // add anchor 2, 3...
  } else {
    ranging.setNodeAddress(0x0002 + anchorId);  // ANCHOR_0=0x0002, ANCHOR_1=0x0003
    ranging.setAnchorAddress(0x0001);
  }

  for (uint8_t i = 0; i < UWB_MAX_ANCHORS; i++) filters[i].reset();

  if (ranging.begin()) {
    enabled = true;
    AddLog(LOG_LEVEL_INFO, PSTR("UWB: Ranging started, role=%d anchors=%d ad=%d fw=%d"),
           mode, (mode == UWB_ROLE_TAG) ? (int)anchorCount : 1, (int)antennaDelay, (int)filterWindow);
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("UWB: Ranging start failed"));
  }
}

// ─── Main loop ────────────────────────────────────────────────────────────────

void UwbDW3000Loop() {
  using namespace UwbDW3000;
  if (!initialized || !enabled) return;

  UWBRangingState stateBefore = ranging.getState();
  ranging.loop();
  UWBRangingState stateAfter  = ranging.getState();

  if (stateAfter != stateBefore) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("UWB: state %d -> %d"), (int)stateBefore, (int)stateAfter);

    if (stateAfter == UWB_STATE_SEND_REPORT) {
      uint64_t tPTx = ranging.getTPollTx(),  tPRx = ranging.getTPollRx();
      uint64_t tRTx = ranging.getTRespTx(),  tRRx = ranging.getTRespRx();
      uint64_t tFTx = ranging.getTFinalTx(), tFRx = ranging.getTFinalRx();
      AddLog(LOG_LEVEL_DEBUG, PSTR("UWB TS: tPollTx=%lu|%lu tPollRx=%lu|%lu"),
             (uint32_t)(tPTx >> 8), (uint32_t)(tPTx & 0xFF),
             (uint32_t)(tPRx >> 8), (uint32_t)(tPRx & 0xFF));
      AddLog(LOG_LEVEL_DEBUG, PSTR("UWB TS: tRespTx=%lu|%lu tRespRx=%lu|%lu"),
             (uint32_t)(tRTx >> 8), (uint32_t)(tRTx & 0xFF),
             (uint32_t)(tRRx >> 8), (uint32_t)(tRRx & 0xFF));
      AddLog(LOG_LEVEL_DEBUG, PSTR("UWB TS: tFinalTx=%lu|%lu tFinalRx=%lu|%lu"),
             (uint32_t)(tFTx >> 8), (uint32_t)(tFTx & 0xFF),
             (uint32_t)(tFRx >> 8), (uint32_t)(tFRx & 0xFF));
      uint64_t tRound1 = (tRRx - tPTx) & 0xFFFFFFFFFFULL;
      uint64_t tReply1 = (tRTx - tPRx) & 0xFFFFFFFFFFULL;
      uint64_t tRound2 = (tFRx - tRTx) & 0xFFFFFFFFFFULL;
      uint64_t tReply2 = (tFTx - tRRx) & 0xFFFFFFFFFFULL;
      AddLog(LOG_LEVEL_DEBUG, PSTR("UWB TS: tRound1=%lu tReply1=%lu tRound2=%lu tReply2=%lu"),
             (uint32_t)tRound1, (uint32_t)tReply1,
             (uint32_t)tRound2, (uint32_t)tReply2);
    }
  }

  UWBRangingResult result;
  if (ranging.getResult(result)) {
    uint8_t aid = result.anchorId;
    result.distanceCm = filters[aid].push(result.distanceCm, filterWindow);
    lastResults[aid] = result;
    AddLog(LOG_LEVEL_DEBUG, PSTR("UWB: dist=%*_f cm (anchor %d)"), 1, &result.distanceCm, aid);
    if (mqttRealtime) { MqttPublishSensor(); }
  }
}

void UwbDW3000Every1s() {
  using namespace UwbDW3000;
  if (!initialized || !enabled) return;
  AddLog(LOG_LEVEL_DEBUG, PSTR("UWB: state=%d errors=%lu"), (int)ranging.getState(), ranging.getErrorCount());
}

// ─── JSON MQTT publication ────────────────────────────────────────────────────

void UwbDW3000JsonAppend() {
  using namespace UwbDW3000;
  if (!initialized) return;

  const char* modeStr = (mode == UWB_ROLE_TAG)   ? "tag"
                      : (mode == UWB_ROLE_ANCHOR) ? "anchor"
                      : "off";

  if (mode == UWB_ROLE_TAG && anchorCount > 1) {
    bool any = false;
    for (uint8_t i = 0; i < anchorCount; i++) {
      if (lastResults[i].valid) { any = true; break; }
    }
    if (!any) return;
    ResponseAppend_P(PSTR(",\"UWB\":{\"Mode\":\"tag\",\"Channel\":%d"), channel);
    for (uint8_t i = 0; i < anchorCount; i++) {
      if (!lastResults[i].valid) continue;
      ResponseAppend_P(PSTR(",\"Anchor%d\":%*_f"), i, 1, &lastResults[i].distanceCm);
    }
    ResponseAppend_P(PSTR("}"));
  } else {
    if (!lastResults[0].valid) return;
    ResponseAppend_P(PSTR(",\"UWB\":{"
      "\"Mode\":\"%s\","
      "\"AnchorId\":%d,"
      "\"Distance\":%*_f,"
      "\"Unit\":\"cm\","
      "\"Method\":\"%s\","
      "\"Channel\":%d"
      "}"),
      modeStr,
      lastResults[0].anchorId,
      1, &lastResults[0].distanceCm,
      lastResults[0].method,
      channel
    );
  }
}

// ─── WebUI display (main page) ────────────────────────────────────────────────

void UwbDW3000ShowSensor() {
  using namespace UwbDW3000;
  if (!initialized) return;

  bool shown = false;
  for (uint8_t i = 0; i < UWB_MAX_ANCHORS; i++) {
    if (!lastResults[i].valid) continue;
    shown = true;
    WSContentSend_PD(PSTR("{s}UWB Anchor %d{m}%*_f cm{e}"), i, 1, &lastResults[i].distanceCm);
  }
  if (shown)
    WSContentSend_PD(PSTR("{s}UWB Method{m}%s{e}"), lastResults[0].method);
}

// ─── Tasmota commands ────────────────────────────────────────────────────────

void CmndUwbMode() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint8_t newMode = (uint8_t)XdrvMailbox.payload;
    if (newMode > 2) { ResponseCmndError(); return; }
    mode = newMode;
    autostart = (newMode != UWB_ROLE_OFF);
    UwbDW3000SaveSettings();
    if (initialized) {
      ranging.reset();
      if (mode != UWB_ROLE_OFF) {
        UwbDW3000StartRanging();
      } else {
        enabled = false;
      }
    }
  }
  ResponseCmndNumber(mode);
}

void CmndUwbAnchorId() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint8_t id = (uint8_t)XdrvMailbox.payload;
    if (id > 3) { ResponseCmndError(); return; }
    anchorId = id;
    UwbDW3000SaveSettings();
    if (initialized && mode != UWB_ROLE_OFF) {
      ranging.reset();
      enabled = false;
      UwbDW3000StartRanging();
    }
  }
  ResponseCmndNumber(anchorId);
}

void CmndUwbAnchorCount() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint8_t n = (uint8_t)XdrvMailbox.payload;
    if (n < 1 || n > UWB_MAX_ANCHORS) { ResponseCmndError(); return; }
    anchorCount = n;
    UwbDW3000SaveSettings();
    if (initialized && mode == UWB_ROLE_TAG) {
      ranging.reset();
      enabled = false;
      UwbDW3000StartRanging();
    }
  }
  ResponseCmndNumber(anchorCount);
}

void CmndUwbInterval() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint32_t ms = (uint32_t)XdrvMailbox.payload;
    if (ms < 50 || ms > 10000) { ResponseCmndError(); return; }
    intervalMs = ms;
    ranging.setRangingInterval(intervalMs);
  }
  ResponseCmndNumber(intervalMs);
}

void CmndUwbChannel() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint8_t ch = (uint8_t)XdrvMailbox.payload;
    if (ch != 5 && ch != 9) { ResponseCmndError(); return; }
    channel = ch;
    if (initialized) { UwbDW3000HardwareInit(); }
  }
  ResponseCmndNumber(channel);
}

void CmndUwbAntennaDelay() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint32_t ad = (uint32_t)XdrvMailbox.payload;
    if (ad < 10000 || ad > 32767) { ResponseCmndError(); return; }
    antennaDelay = (uint16_t)ad;
    ranging.setAntennaDelay((float)antennaDelay);
  }
  ResponseCmndNumber(antennaDelay);
}

void CmndUwbFilterWindow() {
  using namespace UwbDW3000;
  if (XdrvMailbox.data_len > 0) {
    uint8_t fw = (uint8_t)XdrvMailbox.payload;
    if (fw < 1 || fw > UWB_FILTER_MAX_WINDOW) { ResponseCmndError(); return; }
    filterWindow = fw;
    UwbDW3000SaveSettings();
    for (uint8_t i = 0; i < UWB_MAX_ANCHORS; i++) filters[i].reset();
  }
  ResponseCmndNumber(filterWindow);
}

const char kUwbCommands[] PROGMEM = "UWB|Mode|AnchorId|AnchorCount|Interval|Channel|AntennaDelay|FilterWindow";

void (* const UwbCommand[])(void) PROGMEM = {
  &CmndUwbMode,
  &CmndUwbAnchorId,
  &CmndUwbAnchorCount,
  &CmndUwbInterval,
  &CmndUwbChannel,
  &CmndUwbAntennaDelay,
  &CmndUwbFilterWindow
};

// ─── WebUI configuration page ────────────────────────────────────────────────

#ifdef USE_WEBSERVER

#define WEB_HANDLE_UWB "s126"

const char HTTP_BTN_MENU_UWB[] PROGMEM =
  "<p><form action='" WEB_HANDLE_UWB "' method='get'>"
  "<button>UWB</button></form></p>";

const char HTTP_FORM_UWB[] PROGMEM =
  "<form method='post' action='" WEB_HANDLE_UWB "'>"
  "<fieldset><legend><b>&nbsp;UWB DW3000&nbsp;</b></legend>"
  "<p><b>Role</b><br>"
  "<label><input type='radio' name='ro' value='0'%s>&nbsp;Off</label>&emsp;"
  "<label><input type='radio' name='ro' value='1'%s>&nbsp;Tag</label>&emsp;"
  "<label><input type='radio' name='ro' value='2'%s>&nbsp;Anchor</label></p>"
  "<p><b>UWB Channel</b><br>"
  "<label><input type='radio' name='ch' value='5'%s>&nbsp;Channel&nbsp;5 (6.5&thinsp;GHz)</label>&emsp;"
  "<label><input type='radio' name='ch' value='9'%s>&nbsp;Channel&nbsp;9 (8.0&thinsp;GHz)</label></p>"
  "<p><b>Ranging interval (ms)</b><br>"
  "<input type='number' name='iv' min='50' max='10000' value='%d'></p>"
  "<p><b>Antenna delay (ticks)</b><br>"
  "<input type='number' name='ad' min='10000' max='32767' value='%d'></p>"
  "<p><b>Anchor ID &mdash; ANCHOR only (0&nbsp;&rarr;&nbsp;0x0002, 1&nbsp;&rarr;&nbsp;0x0003&hellip;)</b><br>"
  "<input type='number' name='ai' min='0' max='3' value='%d'></p>"
  "<p><b>Anchor count &mdash; TAG only (round-robin)</b><br>"
  "<input type='number' name='ac' min='1' max='4' value='%d'></p>"
  "<p><b>Sliding average filter &mdash; window size (per anchor)</b><br>"
  "<input type='number' name='fw' min='1' max='10' value='%d'></p>"
  "<p><label><input type='checkbox' name='as' value='1'%s>&nbsp;"
  "<b>Auto-start on boot</b></label></p>"
  "<p><label><input type='checkbox' name='mr' value='1'%s>&nbsp;"
  "<b>Immediate MQTT publish (outside TelePeriod)</b></label></p>"
  "</fieldset>"
  "<br><button name='save' type='submit'>Save</button>"
  "</form>";

void HandleUwbConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("save")) {
    using namespace UwbDW3000;
    char sval[8];

    WebGetArg("ro", sval, sizeof(sval));
    if (strlen(sval)) {
      uint8_t m = (uint8_t)atoi(sval);
      if (m <= 2) mode = m;
    }

    WebGetArg("ch", sval, sizeof(sval));
    if (strlen(sval)) {
      uint8_t ch = (uint8_t)atoi(sval);
      if (ch == 5 || ch == 9) channel = ch;
    }

    WebGetArg("iv", sval, sizeof(sval));
    if (strlen(sval)) {
      uint32_t ms = (uint32_t)atol(sval);
      if (ms >= 50 && ms <= 10000) intervalMs = ms;
    }

    WebGetArg("ad", sval, sizeof(sval));
    if (strlen(sval)) {
      uint32_t ad = (uint32_t)atol(sval);
      if (ad >= 10000 && ad <= 32767) antennaDelay = (uint16_t)ad;
    }

    WebGetArg("ai", sval, sizeof(sval));
    if (strlen(sval)) {
      uint8_t ai = (uint8_t)atoi(sval);
      if (ai <= 3) anchorId = ai;
    }

    WebGetArg("ac", sval, sizeof(sval));
    if (strlen(sval)) {
      uint8_t ac = (uint8_t)atoi(sval);
      if (ac >= 1 && ac <= UWB_MAX_ANCHORS) anchorCount = ac;
    }

    WebGetArg("fw", sval, sizeof(sval));
    if (strlen(sval)) {
      uint8_t fw = (uint8_t)atoi(sval);
      if (fw >= 1 && fw <= UWB_FILTER_MAX_WINDOW) {
        filterWindow = fw;
        for (uint8_t i = 0; i < UWB_MAX_ANCHORS; i++) filters[i].reset();
      }
    }

    autostart    = Webserver->hasArg("as");
    mqttRealtime = Webserver->hasArg("mr");

    UwbDW3000SaveSettings();

    // Full reinit to apply the new channel/mode
    ranging.reset();
    enabled = false;
    UwbDW3000HardwareInit();

    HandleConfiguration();
    return;
  }

  using namespace UwbDW3000;
  WSContentStart_P(PSTR("Configure UWB"));
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_UWB,
    (mode == 0) ? " checked" : "", (mode == 1) ? " checked" : "", (mode == 2) ? " checked" : "",
    (channel == 5) ? " checked" : "", (channel == 9) ? " checked" : "",
    (int)intervalMs,
    (int)antennaDelay,
    (int)anchorId,
    (int)anchorCount,
    (int)filterWindow,
    autostart    ? " checked" : "",
    mqttRealtime ? " checked" : ""
  );
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

#endif  // USE_WEBSERVER

// ─── Tasmota main dispatcher ─────────────────────────────────────────────────

bool Xsns126(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      UwbDW3000Init();
      break;
    case FUNC_LOOP:
      UwbDW3000Loop();
      break;
    case FUNC_EVERY_SECOND:
      UwbDW3000Every1s();
      break;
    case FUNC_JSON_APPEND:
      UwbDW3000JsonAppend();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kUwbCommands, UwbCommand);
      break;
    case FUNC_WEB_SENSOR:
      UwbDW3000ShowSensor();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(HTTP_BTN_MENU_UWB);
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/" WEB_HANDLE_UWB), HandleUwbConfiguration);
      break;
#endif
  }

  return result;
}

#endif  // USE_UWB_DW3000
