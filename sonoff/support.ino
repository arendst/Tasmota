/*********************************************************************************************\
 * Config
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
extern "C" uint32_t _SPIFFS_end;
#define CFG_LOCATION (((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2  // End of SPIFFS area

void CFG_Save()
{
  char log[LOGSZ];
  
  if (memcmp(&myCfg, &sysCfg, sizeof(SYSCFG))) {
    noInterrupts();
    if (sysCfg.saveFlag == 0) {  // Handle default and rollover
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    }
    sysCfg.saveFlag++;
    spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
    spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    interrupts();
    snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration to flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
    addLog(LOG_LEVEL_DEBUG, log);
    myCfg = sysCfg;
  }
}

void CFG_Load()
{
  char log[LOGSZ];

  noInterrupts();
  spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
  spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32 *)&myCfg, sizeof(SYSCFG));
  interrupts();
  if (sysCfg.saveFlag < myCfg.saveFlag) sysCfg = myCfg;
  snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
  addLog(LOG_LEVEL_DEBUG, log);
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  myCfg = sysCfg;
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  byte seriallog_level = sysCfg.seriallog_level;

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (LOG_LEVEL_DEBUG_MORE <= seriallog_level) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK)
        Serial.println(F(" OK"));
      else
        Serial.println(F(" Error"));
      delay(10);
    }
  }
}

/*********************************************************************************************\
 * WiFiManager 
 * 
 * Enables configuration and reconfiguration of WiFi credentials using a Captive Portal 
 * Source by AlexT (https://github.com/tzapu)
\*********************************************************************************************/

#define DNS_PORT 53

const char HTTP_HEAD[] PROGMEM       = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>"
                                       "<title>{v}</title>";
const char HTTP_STYLE[] PROGMEM      = "<style>"
                                       "div,input{padding:5px;font-size:1em;} "
                                       "input{width:95%;} "
                                       "body{text-align:center;font-family:verdana;} "
                                       "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} "
                                       ".q{float:right;width:64px;text-align:right;} "
                                       ".l{background:url('data:image/png;base64,"
                                       "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhE"
                                       "BamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg=="
                                       "') no-repeat left center;background-size:1em;}"
                                       "</style>";
const char HTTP_SCRIPT[] PROGMEM     = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM   = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_MENU[] PROGMEM       = "<form action='/0wifi' method='get'><button>Configure WiFi and MQTT</button></form><br/>"
                                       "<form action='/r' method='post'><button>Restart</button></form>";
const char HTTP_ITEM[] PROGMEM       = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_SCAN_LINK[] PROGMEM  = "<div><a href='/wifi'>Scan for wifi networks</a></div><br/>";
const char HTTP_FORM_START[] PROGMEM = "<form method='get' action='wifisave'>"
                                       "<b>Wifi SSId</b> (" STA_SSID ")<br/><input id='s' name='s' length=32 placeholder='STA_SSID' value='{s1}'><br/>"
                                       "<br/><b>Wifi password</b></br><input id='p' name='p' length=64 type='password' placeholder='STA_PASS' value='{p1}'><br/>"
                                       "<br/><b>MQTT host</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' length=32 placeholder='MQTT_HOST' value='{m1}'><br/>"
                                       "<br/><b>MQTT port</b> ({ml})<br/><input id='ml' name='ml' length=5 placeholder='MQTT_PORT' value='{m2}'><br/>"
                                       "<br/><b>MQTT client</b> (" MQTT_CLIENT_ID ")<br/><input id='mc' name='mc' length=32 placeholder='MQTT_CLIENT_ID' value='{m3}'><br/>"
                                       "<br/><b>MQTT user</b> (" MQTT_USER ")<br/><input id='mu' name='mu' length=32 placeholder='MQTT_USER' value='{m4}'><br/>"
                                       "<br/><b>MQTT password</b> (" MQTT_PASS ")<br/><input id='mp' name='mp' length=32 placeholder='MQTT_PASS' value='{m5}'><br/>"
                                       "<br/><b>MQTT topic</b> (" MQTT_TOPIC ")<br/><input id='mt' name='mt' length=32 placeholder='MQTT_TOPIC' value='{m6}'><br/>";
const char HTTP_FORM_END[] PROGMEM   = "<br/><button type='submit'>Save</button></form>"
                                       "<br/><form action='wifidefs' method='get'><button>(Defaults)</button></form>";
const char HTTP_SAVED[] PROGMEM      = "<center><div><b>{p} saved</b><br/><br/>Trying to connect device to network.<br/><br/>If it fails reconnect to try again.</div></center>"
                                       "<br/><form action='/' method='get'><button>Options</button></form>";
const char HTTP_END[] PROGMEM        = "</div></body></html>";

DNSServer *dnsServer;
ESP8266WebServer *server;

const char* _apName = "no-name";
int _minimumQuality = -1;
boolean _removeDuplicateAPs = true;
boolean wm_done = true;
uint8_t managercounter = 0;

void beginWifiManager(char const *apName)
{
  char log[LOGSZ];

  // setup AP
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    WiFi.mode(WIFI_AP_STA);
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Set AccessPoint and Station"));
  } else {
    WiFi.mode(WIFI_AP);
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Set AccessPoint"));
  }

  dnsServer = new DNSServer();
  server = new ESP8266WebServer(80);

  _apName = apName;
  WiFi.softAP(_apName);
  delay(500); // Without delay I've seen the IP address blank

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server->on("/", handleRoot);
  server->on("/wifi", handleWifi1);
  server->on("/0wifi", handleWifi0);
  server->on("/wifisave", handleWifiSave);
  server->on("/wifidefs", handleWifiDefaults);
  server->on("/r", handleReset);
  server->on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server->onNotFound(handleNotFound);
  server->begin(); // Web server start

  snprintf_P(log, sizeof(log), PSTR("Wifimanager: Started HTTP server on AccessPoint %s with IP address %s"),
    _apName, WiFi.softAPIP().toString().c_str());
  addLog(LOG_LEVEL_INFO, log);

  wm_done = false;
}

boolean wifiManagerDone()
{
  if (!wm_done) {
    dnsServer->processNextRequest();  // DNS
    server->handleClient();           // HTTP
  }
  return wm_done;
}

void stopWifiManager()
{
  wm_done = true;
  server->stop();
  dnsServer->stop();
}

void setMinimumSignalQuality(int quality)
{
  _minimumQuality = quality;
}

/** Handle root or redirect to captive portal */
void handleRoot()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Handle root"));
  managercounter = 120;

  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Options");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<center><h1>");
  page += _apName;
  page += F("</h1><h3>" PROJECT " WiFiManager</h3></center>");
  page += FPSTR(HTTP_MENU);
  page += FPSTR(HTTP_END);

  server->send(200, "text/html", page);
}

void handleWifi1()
{
  handleWifi(true);
}

void handleWifi0()
{
  handleWifi(false);
}

/** Wifi config page handler */
void handleWifi(boolean scan)
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Handle config"));
  managercounter = 120;

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Config Wifi");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);

  if (scan) {
    int n = WiFi.scanNetworks();
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Scan done"));

    if (n == 0) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: No networks found"));
      page += F("No networks found. Refresh to scan again.");
    } else {
      //sort networks
      int indices[n];
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }

      // RSSI SORT
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      // remove duplicates ( must be RSSI sorted )
      if (_removeDuplicateAPs) {
        String cssid;
        for (int i = 0; i < n; i++) {
          if (indices[i] == -1) continue;
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {

              snprintf_P(log, sizeof(log), PSTR("Wifimanager: Duplicate AccessPoint %s"), WiFi.SSID(indices[j]).c_str());
              addLog(LOG_LEVEL_DEBUG, log);

              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups

        snprintf_P(log, sizeof(log), PSTR("Wifimanager: SSID %s, RSSI %d"), WiFi.SSID(indices[i]).c_str(), WiFi.RSSI(indices[i]));
        addLog(LOG_LEVEL_DEBUG, log);

        int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

        if (_minimumQuality == -1 || _minimumQuality < quality) {
          String item = FPSTR(HTTP_ITEM);
          String rssiQ;
          rssiQ += quality;
          item.replace("{v}", WiFi.SSID(indices[i]));
          item.replace("{r}", rssiQ);
          if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
            item.replace("{i}", "l");
          } else {
            item.replace("{i}", "");
          }
          page += item;
          delay(0);
        } else {
          addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Skipping due to low quality"));
        }

      }
      page += "<br/>";
    }
  } else {
    page += FPSTR(HTTP_SCAN_LINK);
  }

  page += FPSTR(HTTP_FORM_START);

  page.replace("{s1}", String(sysCfg.sta_ssid));
  page.replace("{p1}", String(sysCfg.sta_pwd));
  page.replace("{m1}", String(sysCfg.mqtt_host));
  int mp = MQTT_PORT;
  String str;
  str += mp;
  page.replace("{ml}", str);
  page.replace("{m2}", String(sysCfg.mqtt_port));
  page.replace("{m3}", String(sysCfg.mqtt_client));
  page.replace("{m4}", String(sysCfg.mqtt_user));
  page.replace("{m5}", String(sysCfg.mqtt_pwd));
  page.replace("{m6}", String(sysCfg.mqtt_topic));
  
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_END);

  server->send(200, "text/html", page);

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Sent config page"));
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave()
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: WiFi save"));

  //SAVE/wm_done here
  if (strlen(server->arg("s").c_str())) strlcpy(sysCfg.sta_ssid, server->arg("s").c_str(), sizeof(sysCfg.sta_ssid));
  if (strlen(server->arg("p").c_str())) strlcpy(sysCfg.sta_pwd, server->arg("p").c_str(), sizeof(sysCfg.sta_pwd));
  snprintf_P(log, sizeof(log), PSTR("Wifimanager: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
  addLog(LOG_LEVEL_INFO, log);

  if (strlen(server->arg("mh").c_str())) strlcpy(sysCfg.mqtt_host, server->arg("mh").c_str(), sizeof(sysCfg.mqtt_host));
  if (strlen(server->arg("ml").c_str())) sysCfg.mqtt_port = atoi(server->arg("ml").c_str());
  if (strlen(server->arg("mc").c_str())) strlcpy(sysCfg.mqtt_client, server->arg("mc").c_str(), sizeof(sysCfg.mqtt_client));
  if (strlen(server->arg("mu").c_str())) strlcpy(sysCfg.mqtt_user, server->arg("mu").c_str(), sizeof(sysCfg.mqtt_user));
  if (strlen(server->arg("mp").c_str())) strlcpy(sysCfg.mqtt_pwd, server->arg("mp").c_str(), sizeof(sysCfg.mqtt_pwd));
  if (strlen(server->arg("mt").c_str())) strlcpy(sysCfg.mqtt_topic, server->arg("mt").c_str(), sizeof(sysCfg.mqtt_topic));
  snprintf_P(log, sizeof(log), PSTR("Wifimanager: Mqtt Host %s, Port %d, Client %s, User %s, Password %s, Topic %s"),
    sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, sysCfg.mqtt_user, sysCfg.mqtt_pwd, sysCfg.mqtt_topic);
  addLog(LOG_LEVEL_INFO, log);

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Parameters");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_SAVED);
  page.replace("{p}", "Parameters");
  page += FPSTR(HTTP_END);

  server->send(200, "text/html", page);

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Sent wifi save page"));

  wm_done = true; //signal ready
}

void handleWifiDefaults()
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: WiFi defaults"));

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Parameters");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_SAVED);
  page.replace("{p}", "Default parameters");
  page += FPSTR(HTTP_END);

  server->send(200, "text/html", page);

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Sent wifi default page"));

  CFG_Default();
  wm_done = true; //signal ready
}

/** Handle the reset page */
void handleReset()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Restart"));

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("Device will restart in a few seconds.");
  page += FPSTR(HTTP_END);
  server->send(200, "text/html", page);

  addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Sent restart page"));

  wm_done = true; //signal ready
}

void handleNotFound()
{
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send ( 404, "text/plain", message );
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
  if (!isIp(server->hostHeader()) ) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Request redirected to captive portal"));

    server->sendHeader("Location", String("http://") + server->client().localIP().toString(), true);
    server->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

int getRSSIasQuality(int RSSI)
{
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

/** Is this an IP? */
boolean isIp(String str)
{
  for (int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_SMARTSEC     60   // seconds
#define WIFI_CHECKSEC     20   // seconds
#define WIFI_RETRY        16

uint8_t wificounter, wifiretry;
uint8_t smartcounter = 0;

void WIFI_manager()
{
  managercounter = WIFI_SMARTSEC;   // Allow up to WIFI_SMARTSEC seconds for wifimanager webserver to provide ssid/pswd
  wificounter = managercounter +5;
  blinks = 1999;
  addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Started and active for 1 minute"));
  beginWifiManager(Hostname);
}

void WIFI_smartconfig()
{
  smartcounter = WIFI_SMARTSEC;   // Allow up to WIFI_SMARTSECS seconds for phone to provide ssid/pswd
  wificounter = smartcounter +5;
  blinks = 1999;
  addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Started and active for 1 minute"));
  WiFi.beginSmartConfig();
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    wificounter = WIFI_CHECKSEC;
    wifiretry = WIFI_RETRY;
    addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Connected"));
  } else {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
      case WL_CONNECT_FAILED:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_CONNECT_FAIL"));
        if (sysCfg.power) WIFI_manager(); else WIFI_smartconfig();
        break;
      default:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_IDLE"));
        if (wifiretry == (WIFI_RETRY / 2)) WiFi.begin();
        wifiretry--;
        if (wifiretry)
          wificounter = 1;
        else
          if (sysCfg.power) WIFI_manager(); else WIFI_smartconfig();
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
    case WIFI_MANAGER:
      WIFI_manager();
      break;
    case WIFI_ABORT:
      if (smartcounter) {
        addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Aborted"));
        smartcounter = 1;
      }
      if (managercounter) {
        addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Aborted"));
        managercounter = 1;
      }
      break;
    default:
      if (wificounter <= 0) {
        addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Checking connection..."));
        wificounter = WIFI_CHECKSEC;
        WIFI_check_ip();
      }
      if (smartcounter) {
        smartcounter--;
        if (smartcounter) {
          if (WiFi.smartConfigDone()) {
            smartcounter = 0;
            if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid));
            if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd));
            snprintf_P(log, sizeof(log), PSTR("Smartconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
            addLog(LOG_LEVEL_INFO, log);
          }
        }
        if (smartcounter == 0) {
          WiFi.stopSmartConfig();
          restartflag = 2;     
        }
      }
      if (managercounter) {
        wificounter = managercounter +5;
        managercounter--;
        if (managercounter) {
          if (wifiManagerDone()) {
            managercounter = 0;
          }
        }
        if (managercounter == 0) {
          stopWifiManager();
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
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to %s as %s"), sysCfg.sta_ssid, Hostname);
  addLog(LOG_LEVEL_INFO, log);
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);     // Disable AP mode
  WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
  wifiretry = WIFI_RETRY;
  wificounter = 1;
}

/*********************************************************************************************\
 * Real Time Clock
 * 
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

extern "C" {
#include "sntp.h"
}

#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

#ifdef USE_TICKER
  Ticker tickerRTC;
#endif

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t utctime = 0, loctime = 0, dsttime = 0, stdtime = 0, ntptime = 0;
uint8_t ntpsync = 0;

void breakTime(uint32_t timeInput, TIME_T &tm)
{
// break the given timeInput into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year, month, monthLength;
  uint32_t time;
  unsigned long days;

  time = timeInput;
  tm.Second = time % 60;
  time /= 60;                // now it is minutes
  tm.Minute = time % 60;
  time /= 60;                // now it is hours
  tm.Hour = time % 24;
  time /= 24;                // now it is days
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.Year = year;            // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  
  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
      break;
    }
  }
  strlcpy(tm.MonthName, monthNames + (month *3), 4);
  tm.Month = month + 1;      // jan is month 1  
  tm.Day = time + 1;         // day of month
}

uint32_t makeTime(TIME_T &tm)
{   
// assemble time elements into time_t 
// note year argument is offset from 1970
  
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.Year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ((i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.Day - 1) * SECS_PER_DAY;
  seconds+= tm.Hour * SECS_PER_HOUR;
  seconds+= tm.Minute * SECS_PER_MIN;
  seconds+= tm.Second;
  return seconds; 
}

uint32_t toTime_t(TimeChangeRule r, int yr)
{
    TIME_T tm;
    uint32_t t;
    uint8_t m, w;            // temp copies of r.month and r.week

    m = r.month;
    w = r.week;
    if (w == 0) {            // Last week = 0
        if (++m > 12) {      // for "Last", go to the next month
            m = 1;
            yr++;
        }
        w = 1;               // and treat as first week of next month, subtract 7 days later
    }

    tm.Hour = r.hour;
    tm.Minute = 0;
    tm.Second = 0;
    tm.Day = 1;
    tm.Month = m;
    tm.Year = yr - 1970;
    t = makeTime(tm);        // First day of the month, or first day of next month for "Last" rules
    breakTime(t, tm); 
    t += (7 * (w - 1) + (r.dow - tm.Wday + 7) % 7) * SECS_PER_DAY;
    if (r.week == 0) t -= 7 * SECS_PER_DAY;    //back up a week if this is a "Last" rule
    return t;
}

String rtc_time(int type)
{
  char stime[25];   // Skip newline

  uint32_t time = utctime;
  if (type == 1) time = loctime;
  if (type == 2) time = dsttime;
  if (type == 3) time = stdtime;
  snprintf_P(stime, sizeof(stime), PSTR("%s"), sntp_get_real_time(time));
  return String(stime);
}

void rtc_second()
{
  char log[LOGSZ];
  uint32_t stdoffset, dstoffset;
  TIME_T tmpTime;

  // NTP Sync every hour at x:0:10
  if (rtcTime.Minute == 0) {
    if ((rtcTime.Second > 9) && !ntpsync) {
      ntpsync = 1;
      ntptime = sntp_get_current_timestamp();
      if (ntptime) {
        utctime = ntptime;
        breakTime(utctime, tmpTime);
        rtcTime.Year = tmpTime.Year + 1970;
        dsttime = toTime_t(myDST, rtcTime.Year);
        stdtime = toTime_t(mySTD, rtcTime.Year);
        snprintf_P(log, sizeof(log), PSTR("RTC: (UTC) %s"), rtc_time(0).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
        snprintf_P(log, sizeof(log), PSTR("RTC: (DST) %s"), rtc_time(2).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
        snprintf_P(log, sizeof(log), PSTR("RTC: (STD) %s"), rtc_time(3).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
      }
    }
  } else {
    ntpsync = 0;
  }
  utctime++;
  loctime = utctime;
  if (loctime > 1451602800) {  // 2016-01-01
    if (sysCfg.timezone == 99) {
      dstoffset = myDST.offset * SECS_PER_MIN;
      stdoffset = mySTD.offset * SECS_PER_MIN;
      if ((utctime >= (dsttime - stdoffset)) && (utctime < (stdtime - dstoffset)))
        loctime += dstoffset;  // Daylight Saving Time
      else
        loctime += stdoffset;  // Standard Time
    } else {
      loctime += sysCfg.timezone * SECS_PER_HOUR;
    }
  }
  breakTime(loctime, rtcTime);
  rtcTime.Year += 1970;
}

void rtc_init()
{
  sntp_setservername(0, (char*)NTP_SERVER1);
  sntp_setservername(1, (char*)NTP_SERVER2);
  sntp_setservername(2, (char*)NTP_SERVER3);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utctime = 0;
#ifdef USE_TICKER
  tickerRTC.attach(1, rtc_second);
#endif
}

/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char mess[MESSZ], str[TOPSZ+MESSZ];

  portUDP.beginPacket(sysCfg.syslog_host, SYS_LOG_PORT);
  strlcpy(mess, message, sizeof(mess));
  mess[sizeof(mess)-1] = 0;
  snprintf_P(str, sizeof(str), PSTR("%s %s"), Hostname, mess);
  portUDP.write(str);
  portUDP.endPacket();
}

void addLog(byte loglevel, const char *line)
{
  char mxtime[9];
  
  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
  
#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.printf("%s %s\n", mxtime, line);  
#endif
#ifdef SERIAL_IO
  if (loglevel <= sysCfg.seriallog_level) Serial.printf("%s %s\n", mxtime, line);
#endif
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= sysCfg.syslog_level)) syslog(line);
}

void addLog(byte loglevel, String& string)
{
  addLog(loglevel, string.c_str());
}

void addLog_P(byte loglevel, const char *formatP)
{
  char mess[MESSZ];
  
  snprintf_P(mess, sizeof(mess), formatP);
  addLog(loglevel, mess);
}

/*********************************************************************************************\
 * 
\*********************************************************************************************/

