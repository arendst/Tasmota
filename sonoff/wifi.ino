#define WIFI_SMARTSEC  60   // seconds
#define WIFI_CHECKSEC  20   // seconds
#define WIFI_RETRY     16

uint8_t wificounter;
uint8_t wifiretry = WIFI_RETRY;
uint8_t smartcounter = 0;

void WIFI_smartconfig()
{
  smartcounter = WIFI_SMARTSEC;   // Allow up to WIFI_SMARTSECS seconds for phone to provide ssid/pswd
  wificounter = smartcounter +5;
  addLog(LOG_LEVEL_INFO, "Smartconfig: Started and active for 1 minute");
  WiFi.beginSmartConfig();
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    wificounter = WIFI_CHECKSEC;
    wifiretry = WIFI_RETRY;
  } else {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
      case WL_CONNECT_FAILED:
        addLog(LOG_LEVEL_DEBUG, "Wifi: STATION_CONNECT_FAIL");
        WIFI_smartconfig();
        break;
      default:
        addLog(LOG_LEVEL_DEBUG, "Wifi: STATION_IDLE");
        if (wifiretry == (WIFI_RETRY / 2)) WiFi.begin();
        wifiretry--;
        if (wifiretry)
          wificounter = 1;
        else
          WIFI_smartconfig();
        break;
    }
  }
}

void WIFI_Check(uint8_t param)
{
  char log[LOGSZ];
  
  wificounter--;
  switch (param) {
    case WIFI_SMARTCONFIG:
      WIFI_smartconfig();
      break;
    default:
      if (wificounter <= 0) {
        addLog(LOG_LEVEL_DEBUG_MORE, "Wifi: Check connection");
        wificounter = WIFI_CHECKSEC;
        WIFI_check_ip();
      }
      if (smartcounter) {
        smartcounter--;
        if (smartcounter) {
          if (WiFi.smartConfigDone()) {
            smartcounter = 0;
            memcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), strlen(WiFi.SSID().c_str())+1);
            memcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), strlen(WiFi.psk().c_str())+1);
            snprintf_P(log, LOGSZ, PSTR("Smartconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
            addLog(LOG_LEVEL_INFO, log);
          }
        }
        if (smartcounter == 0) {
          WiFi.stopSmartConfig();
          restartflag = 2;     
        }
      }
      break;
  }
}

void WIFI_Connect(char *Hostname)
{
  char log[LOGSZ];

  WiFi.persistent(false);   // Solve possible wifi init errors
  WiFi.hostname(Hostname);
  snprintf_P(log, LOGSZ, PSTR("Wifi: Connecting to %s as %s"), sysCfg.sta_ssid, Hostname);
  addLog(LOG_LEVEL_DEBUG, log);
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);     // Disable AP mode
  WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
  wificounter = 1;
}
