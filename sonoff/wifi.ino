#define CHKSECS   20   // seconds

uint8_t wificounter = CHKSECS;
uint8_t wifiretry = 4;
uint8_t smrtcounter = 0;

void WIFI_smartconfig()
{
  smrtcounter = 100;   // Allow up to 100 seconds for phone to provide ssid/pswd
  wificounter = smrtcounter +5;
  Serial.printf("Smartconfig active for %d seconds", smrtcounter);
  WiFi.beginSmartConfig();
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    wificounter = CHKSECS;
    wifiretry = 4;
  } else {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
      case WL_CONNECT_FAILED:
        DEBUG_MSG("WIFI: STATION_CONNECT_FAIL\n");
        WIFI_smartconfig();
        break;
      default:
        DEBUG_MSG("WIFI: STATION_IDLE\n");
        wifiretry--;
        if (wifiretry) {
          WiFi.begin();
          wificounter = 4;
        } else {
          WIFI_smartconfig();
        }
        break;
    }
  }
}

void WIFI_Check(uint8_t param)
{
  wificounter--;
  switch (param) {
    case WIFI_SMARTCONFIG:
      DEBUG_MSG("WIFI: WIFI_SMARTCONFIG\n");
      WIFI_smartconfig();
      break;
    default:
      if (wificounter <= 0) {
        DEBUG_MSG("WIFI: WIFI_STATUS\n");
        wificounter = CHKSECS;
        WIFI_check_ip();
      }
      if (smrtcounter) {
        smrtcounter--;
        if (smrtcounter) {
          if (WiFi.smartConfigDone()) {
            smrtcounter = 0;
            memcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), strlen(WiFi.SSID().c_str())+1);
            memcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), strlen(WiFi.psk().c_str())+1);
            Serial.printf("\nSmartconfig SSID = %s and Password = %s\n", sysCfg.sta_ssid, sysCfg.sta_pwd);
            restartflag = 3;
          }
        }
        if (smrtcounter == 0) {
          WiFi.stopSmartConfig();
          restartflag = 2;     
        }
      }
      break;
  }
}

void WIFI_Connect()
{
  char hostname[32];

  sprintf_P(hostname, PSTR(WIFI_HOSTNAME), ESP.getChipId(), sysCfg.mqtt_topic);
  WiFi.hostname(hostname);
  DEBUG_MSG("APP: Connecting to %s as %s\n", sysCfg.sta_ssid, hostname);
  WiFi.setAutoConnect(true);
  WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
  wificounter = 4;
}
