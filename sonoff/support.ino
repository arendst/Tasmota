/*********************************************************************************************\
 * Config - Flash or Spiffs
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
extern "C" uint32_t _SPIFFS_end;

#define SPIFFS_CONFIG       "/config.ini"
#define SPIFFS_START        ((uint32_t)&_SPIFFS_start - 0x40200000) / SPI_FLASH_SEC_SIZE
#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE
#define CFG_LOCATION        SPIFFS_END - 2

uint32_t _cfgHash = 0;
int spiffsflag = 0;

uint32_t getHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&sysCfg;

  for (int i = 0; i < sizeof(SYSCFG); i++) hash += bytes[i]*(i+1);
  return hash;  
}

void CFG_Save()
{
  char log[LOGSZ];

  if ((getHash() != _cfgHash) && (spiffsPresent())) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      sysCfg.saveFlag++;
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) f.write(bytes[i]);
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration to spiffs count %d"), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Saving configuration failed"));
    } else {
#endif
      noInterrupts();
      if (sysCfg.saveFlag == 0) {  // Handle default and rollover
        spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
        spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      }
      sysCfg.saveFlag++;
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration to flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
    _cfgHash = getHash();
  }
}

void CFG_Load()
{
  char log[LOGSZ];

  if (spiffsPresent()) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) bytes[i] = f.read();
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from spiffs count %d"), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Loading configuration failed"));
    } else {  
#endif
      struct SYSCFGH {
        unsigned long cfg_holder;
        unsigned long saveFlag;
      } _sysCfgH;

      noInterrupts();
      spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  _cfgHash = getHash();
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

boolean spiffsPresent()
{
  return (SPIFFS_END - SPIFFS_START);
}

#ifdef USE_SPIFFS
void initSpiffs()
{
  spiffsflag = 0;
  if (!spiffsPresent()) {
    spiffsflag = 1;
  } else {
    if (!SPIFFS.begin()) {
      addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to mount file system. Will use flash"));
      spiffsflag = 2;
    } else {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Mount successful"));
      File f = SPIFFS.open(SPIFFS_CONFIG, "r");
      if (!f) {
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatting..."));
        SPIFFS.format();
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatted"));
        File f = SPIFFS.open(SPIFFS_CONFIG, "w");
        if (f) {
          for (int i = 0; i < sizeof(SYSCFG); i++) f.write(0);
          f.close();
        } else {
          addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to init config file. Will use flash"));
          spiffsflag = 3;
        }
      }
    }
  }  
}
#endif

/*********************************************************************************************\
 * WiFiManager 
 * 
 * Enables configuration and reconfiguration of WiFi credentials using a Captive Portal 
 * Source by AlexT (https://github.com/tzapu)
\*********************************************************************************************/

#define DNS_PORT      53
#define MANAGER_SEC   120

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
const char HTTP_SCRIPT[] PROGMEM     = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}"
                                       "var cn=120;function u(){if(cn>=0){document.getElementById('t').innerHTML='Restart in '+cn+' seconds';cn--;setTimeout(u,1000);}}</script>";
const char HTTP_HEAD_END[] PROGMEM   = "</head><body onload='u()'><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_MENU1[] PROGMEM      = "<div style='text-align:center;font-weight:bold;font-size:60px'>{r0}</div>"
                                       "<br/><form action='/?o=1' method='post'><button>Toggle relay</button></form><br/>";
const char HTTP_MENU2[] PROGMEM      = "<br/><form action='/0wifi' method='get'><button>Configure WiFi and MQTT</button></form>"
                                       "<br/><form action='/u' method='post'><button>OTA upgrade</button></form>"
                                       "<br/><form action='/c' method='post'><button>Command line</button></form>"
                                       "<br/><form action='/r' method='post'><button>Restart</button></form>";
const char HTTP_COUNTER[] PROGMEM    = "<br/><div id='t' name='t' style='text-align:center;'></div>";
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
const char HTTP_SAVED[] PROGMEM      = "<div style='text-align:center;'><b>{p} saved</b><br/><br/>Trying to connect device to network.<br/><br/>If it fails reconnect to try again.</div>";
const char HTTP_FORM_UPG[] PROGMEM   = "<form method='post' action='u2'>"
                                       "<b>OTA Url</b> (" OTA_URL ")<br/><input id='o' name='o' length=80 placeholder='OTA_URL' value='{o1}'><br/>"
                                       "<br/><button type='submit'>Start upgrade</button></form>";
const char HTTP_FORM_CMND[] PROGMEM  = "<form method='post' action='c'>"
                                       "<input id='" SUB_PREFIX "' name='" SUB_PREFIX "' length=80 placeholder='Enter command' autofocus><br/>"
//                                       "<br/><button type='submit'>Send command</button>";
                                       "</form>";
const char HTTP_MAIN_MENU[] PROGMEM  = "<br/><form action='/' method='post'><button>Main menu</button></form>";
const char HTTP_END[] PROGMEM        = "</div></body></html>";

DNSServer *dnsServer;
ESP8266WebServer *webServer;

int _minimumQuality = -1;
boolean _removeDuplicateAPs = true;
int smartcounter = 0, smartconfigflag = 0, httpflag = 0;

void startWebserver(int type)
{
  if (!httpflag) {
    if (!webServer) {
      webServer = new ESP8266WebServer(80);
      webServer->on("/", handleRoot);
      webServer->on("/wifi", handleWifi1);
      webServer->on("/0wifi", handleWifi0);
      webServer->on("/wifisave", handleWifiSave);
      webServer->on("/wifidefs", handleWifiDefaults);
      webServer->on("/u", handleUpgrade);
      webServer->on("/u2", handleUpgradeStart);
      webServer->on("/c", handleConsole);
      webServer->on("/r", handleReset);
      webServer->on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
      webServer->onNotFound(handleNotFound);
    }
    webServer->begin(); // Web server start
    addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Webserver started"));
  }
  if (type) httpflag = type;
}

void stopWebserver()
{
  if (httpflag) {
    webServer->close();
//    free(webServer);
    httpflag = 0;
    addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Webserver stopped"));
  }
}

void beginWifiManager()
{
  char log[LOGSZ];

  // setup AP
  if ((WiFi.waitForConnectResult() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    WiFi.mode(WIFI_AP_STA);
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Set AccessPoint and keep Station"));
  } else {
    WiFi.mode(WIFI_AP);
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifimanager: Set AccessPoint"));
  }

  stopWebserver();

  dnsServer = new DNSServer();
  WiFi.softAP(Hostname);
  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  startWebserver(2);

  snprintf_P(log, sizeof(log), PSTR("Wifimanager: Started web server on AccessPoint %s with IP address %s"),
    Hostname, WiFi.softAPIP().toString().c_str());
  addLog(LOG_LEVEL_INFO, log);
}

void pollDnsWeb()
{
  if (dnsServer) dnsServer->processNextRequest();
  if (webServer) webServer->handleClient();
}

void handleRoot()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle root"));
  if (smartcounter) smartcounter = MANAGER_SEC;

  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  if (strlen(webServer->arg("o").c_str())) {
#ifdef MQTT_SUBTOPIC
    snprintf_P(svalue, sizeof(svalue), PSTR("%s 2"), sysCfg.mqtt_subtopic);
#else
    snprintf_P(svalue, sizeof(svalue), PSTR("light 2"));
#endif
    do_cmnd(svalue);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Main menu");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<div style='text-align:center;'><h1>");
  page += Hostname;
  page += F("</h1><h3>" PROJECT);
  if (httpflag == 2)
    page += F(" WiFiManager");
  page += F("</h3></div>");
  page += FPSTR(HTTP_MENU1);
  page.replace("{r0}", (sysCfg.power) ? "ON" : "OFF");
  if (httpflag == 2)
    page += FPSTR(HTTP_MENU2);
  if (smartcounter)     
    page += FPSTR(HTTP_COUNTER);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);
}

void handleWifi1()
{
  handleWifi(true);
}

void handleWifi0()
{
  handleWifi(false);
}

void handleWifi(boolean scan)
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle config"));
  if (smartcounter) smartcounter = MANAGER_SEC;

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Config Wifi");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);

  if (scan) {
    int n = WiFi.scanNetworks();
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: Scan done"));

    if (n == 0) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: No networks found"));
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

              snprintf_P(log, sizeof(log), PSTR("Wifi: Duplicate AccessPoint %s"), WiFi.SSID(indices[j]).c_str());
              addLog(LOG_LEVEL_DEBUG, log);

              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups

        snprintf_P(log, sizeof(log), PSTR("Wifi: SSID %s, RSSI %d"), WiFi.SSID(indices[i]).c_str(), WiFi.RSSI(indices[i]));
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
          addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: Skipping due to low quality"));
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
  page += FPSTR(HTTP_MAIN_MENU);
  if (smartcounter)     
    page += FPSTR(HTTP_COUNTER);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);
}

void handleWifiSave()
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: WiFi save"));

  if (strlen(webServer->arg("s").c_str())) strlcpy(sysCfg.sta_ssid, webServer->arg("s").c_str(), sizeof(sysCfg.sta_ssid));
  if (strlen(webServer->arg("p").c_str())) strlcpy(sysCfg.sta_pwd, webServer->arg("p").c_str(), sizeof(sysCfg.sta_pwd));
  snprintf_P(log, sizeof(log), PSTR("Wifi: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
  addLog(LOG_LEVEL_INFO, log);
  if (strlen(webServer->arg("mh").c_str())) strlcpy(sysCfg.mqtt_host, webServer->arg("mh").c_str(), sizeof(sysCfg.mqtt_host));
  if (strlen(webServer->arg("ml").c_str())) sysCfg.mqtt_port = atoi(webServer->arg("ml").c_str());
  if (strlen(webServer->arg("mc").c_str())) strlcpy(sysCfg.mqtt_client, webServer->arg("mc").c_str(), sizeof(sysCfg.mqtt_client));
  if (strlen(webServer->arg("mu").c_str())) strlcpy(sysCfg.mqtt_user, webServer->arg("mu").c_str(), sizeof(sysCfg.mqtt_user));
  if (strlen(webServer->arg("mp").c_str())) strlcpy(sysCfg.mqtt_pwd, webServer->arg("mp").c_str(), sizeof(sysCfg.mqtt_pwd));
  if (strlen(webServer->arg("mt").c_str())) strlcpy(sysCfg.mqtt_topic, webServer->arg("mt").c_str(), sizeof(sysCfg.mqtt_topic));
  snprintf_P(log, sizeof(log), PSTR("Wifi: Mqtt Host %s, Port %d, Client %s, User %s, Password %s, Topic %s"),
    sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, sysCfg.mqtt_user, sysCfg.mqtt_pwd, sysCfg.mqtt_topic);
  addLog(LOG_LEVEL_INFO, log);

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Parameters");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_SAVED);
  page.replace("{p}", "Parameters");
  page += FPSTR(HTTP_MAIN_MENU);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);

  smartcounter = 1;
}

void handleWifiDefaults()
{
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: WiFi defaults"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Parameters");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_SAVED);
  page.replace("{p}", "Default parameters");
  page += FPSTR(HTTP_MAIN_MENU);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);

  CFG_Default();
  smartcounter = 1;
}

void handleUpgrade()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle upgrade"));
  if (smartcounter) smartcounter = MANAGER_SEC;

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "OTA Upgrade");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_FORM_UPG);
  page.replace("{o1}", String(sysCfg.otaUrl));
  page += FPSTR(HTTP_MAIN_MENU);
  if (smartcounter)     
    page += FPSTR(HTTP_COUNTER);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);
}

void handleUpgradeStart()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: OTA Upgrade start"));
  if (smartcounter) smartcounter = 0;

  if (strlen(webServer->arg("o").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR("otaurl %s"), webServer->arg("o").c_str());
    do_cmnd(svalue);
  }
  
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<div style='text-align:center;'><b>Upgrade started</b><br/><br/>Device will restart in a few seconds.</div><br/>");
  page += FPSTR(HTTP_MAIN_MENU);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);

  snprintf_P(svalue, sizeof(svalue), PSTR("upgrade 1"));
  do_cmnd(svalue);
}

void handleConsole()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle command"));
  if (smartcounter) smartcounter = MANAGER_SEC;

  if (strlen(webServer->arg(SUB_PREFIX).c_str())) {
    snprintf_P(svalue, sizeof(svalue), webServer->arg(SUB_PREFIX).c_str());
    do_cmnd(svalue);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Command line");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_FORM_CMND);
  page += FPSTR(HTTP_MAIN_MENU);
  if (smartcounter)     
    page += FPSTR(HTTP_COUNTER);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);
}

void handleReset()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Restarting"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<div style='text-align:center;'>Device will restart in a few seconds.</div><br/>");
  page += FPSTR(HTTP_MAIN_MENU);
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);

  smartcounter = 1;
}

void handleNotFound()
{
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer->uri();
  message += "\nMethod: ";
  message += ( webServer->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer->args();
  message += "\n";
  for ( uint8_t i = 0; i < webServer->args(); i++ ) {
    message += " " + webServer->argName ( i ) + ": " + webServer->arg ( i ) + "\n";
  }
  
  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send ( 404, "text/plain", message );
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
  if (!isIp(webServer->hostHeader()) ) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Request redirected to captive portal"));

    webServer->sendHeader("Location", String("http://") + webServer->client().localIP().toString(), true);
    webServer->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    webServer->client().stop(); // Stop is needed because we sent no content length
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

void WIFI_smartconfig(int type)
{
  if (!smartconfigflag) {
    smartconfigflag = type;
    smartcounter = WIFI_SMARTSEC;   // Allow up to WIFI_SMARTSECS seconds for phone to provide ssid/pswd
    wificounter = smartcounter +5;
    blinks = 1999;
    if (smartconfigflag == WIFI_SMARTCONFIG) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Started and active for 1 minute"));
      WiFi.beginSmartConfig();
    } else {
      addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Started and waiting up to 1 minute for initial request"));
      beginWifiManager();
    }
  }
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
        WIFI_smartconfig(sysCfg.power +1);
        break;
      default:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_IDLE"));
        if (wifiretry == (WIFI_RETRY / 2)) WiFi.begin();
        wifiretry--;
        if (wifiretry)
          wificounter = 1;
        else
          WIFI_smartconfig(sysCfg.power +1);
        break;
    }
  }
}

void WIFI_Check(int param)
{
  char log[LOGSZ];
  
  wificounter--;
  switch (param) {
  case WIFI_SMARTCONFIG:
  case WIFI_MANAGER:
    WIFI_smartconfig(param);
    break;
  default:
    if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
      if (smartconfigflag != WIFI_MANAGER) {
        if (sysCfg.webserver && (sysCfg.webserver != httpflag)) startWebserver(sysCfg.webserver);
        if (!sysCfg.webserver && httpflag) stopWebserver();
      }
    }
    if (wificounter <= 0) {
      addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Checking connection..."));
      wificounter = WIFI_CHECKSEC;
      WIFI_check_ip();
    }
    if (smartcounter) {
      smartcounter--;
      wificounter = smartcounter +5;
      if (smartcounter) {
        if ((smartconfigflag == WIFI_SMARTCONFIG) && WiFi.smartConfigDone()) {
          smartcounter = 0;
          if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid));
          if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd));
          snprintf_P(log, sizeof(log), PSTR("Smartconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
          addLog(LOG_LEVEL_INFO, log);
        }
      }
      if (smartcounter == 0) {
        if (smartconfigflag == WIFI_SMARTCONFIG) WiFi.stopSmartConfig();
        restartflag = 2;     
      }
    }
  }
}

int WIFI_State()
{
  int state;

  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) state = WIFI_STATUS;
  if (smartconfigflag) state = smartconfigflag;
  return state;
}

void WIFI_Connect(char *Hostname)
{
  char log[LOGSZ];

  WiFi.persistent(false);   // Solve possible wifi init errors
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFi.hostname(Hostname);
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to %s as %s"), sysCfg.sta_ssid, Hostname);
  addLog(LOG_LEVEL_INFO, log);
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
#ifdef USE_SERIAL
  if (loglevel <= sysCfg.seriallog_level) Serial.printf("%s %s\n", mxtime, line);
#endif
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= sysCfg.syslog_level)) syslog(line);
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

