/*********************************************************************************************\
 * Config - Flash or Spiffs
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}

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
#endif  // USE_SPIFFS
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
#endif  // USE_SPIFFS
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
  boolean _serialoutput = (LOG_LEVEL_DEBUG_MORE <= sysCfg.seriallog_level);

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
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
#endif  // USE_SPIFFS

#ifdef USE_WEBSERVER
/*********************************************************************************************\
 * Web server and WiFi Manager 
 * 
 * Enables configuration and reconfiguration of WiFi credentials using a Captive Portal 
 * Source by AlexT (https://github.com/tzapu)
\*********************************************************************************************/

#define DNS_PORT      53

const char HTTP_HEAD[] PROGMEM       = "<!DOCTYPE html><html lang=\"en\">"
                                       "<head>"
                                       "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
                                       "<title>{v}</title>"
                                       "<script>"
                                       "var cn=120;"
                                       "function u(){"
                                         "if(cn>=0){"
                                           "document.getElementById('t').innerHTML='Restart in '+cn+' seconds';"
                                           "cn--;"
                                           "setTimeout(u,1000);"
                                         "}"
                                       "}"
                                       "function c(l){"
                                         "document.getElementById('s').value=l.innerText||l.textContent;"
                                         "document.getElementById('p').focus();"
                                       "}"
                                       "function l(){"
                                         "var xhttp=new XMLHttpRequest();"
                                         "xhttp.onreadystatechange=function(){"
                                           "if(xhttp.readyState==4&&xhttp.status==200){"
                                             "document.getElementById('t1').value=xhttp.responseText;"
                                           "}"
                                         "};"
                                         "xhttp.open('GET','ax',true);"
                                         "xhttp.send();"
                                         "setTimeout(l,2000);"
                                       "}"
                                       "</script>"
                                       "<style>"
                                       "div,fieldset,input,select{padding:5px;font-size:1em;}"
                                       "input{width:95%;}select{width:100%;}"
                                       "textarea{resize:none;width:98%;padding:5px;}"
                                       "body{text-align:center;font-family:verdana;}"
                                       "td{padding:0px 5px;}"
                                       "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;}"
                                       "button:hover{background-color:#006cba;}"
                                       ".q{float:right;width:64px;text-align:right;}"
                                       ".l{background:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6O"
                                       "Sk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eA"
                                       "XvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;background-size:1em;}"
                                       "</style>"
                                       "</head>"
                                       "<body>"
                                       "<div style='text-align:left;display:inline-block;min-width:260px;'>"
                                       "<div style='text-align:center;'><h2>" APP_NAME "</h2><h3>{h}</h3></div>";
const char HTTP_MSG_RSTRT[] PROGMEM  = "<br/><div style='text-align:center;'>Device will restart in a few seconds</div><br/>";
const char HTTP_BTN_TOGGL[] PROGMEM  = "<div style='text-align:center;font-weight:bold;font-size:60px'>{r0}</div>"
                                       "<br/><form action='/?o=1' method='post'><button>Toggle</button></form><br/>";
const char HTTP_BTN_MENU1[] PROGMEM  = "<br/><form action='/cn' method='post'><button>Configuration</button></form>"
                                       "<br/><form action='/in' method='post'><button>Information</button></form>"
                                       "<br/><form action='/up' method='post'><button>Firmware upgrade</button></form>"
                                       "<br/><form action='/cm' method='post'><button>Console</button></form>";
const char HTTP_BTN_RSTRT[] PROGMEM  = "<br/><form action='/rb' method='post'><button>Restart</button></form>";
const char HTTP_BTN_MENU2[] PROGMEM  = "<br/><form action='/w0' method='post'><button>Configure WiFi</button></form>"
                                       "<br/><form action='/mq' method='post'><button>Configure MQTT</button></form>"
                                       "<br/><form action='/lg' method='post'><button>Configure logging</button></form>"
                                       "<br/><form action='/rt' method='post'><button>Reset Configuration</button></form>";
const char HTTP_BTN_MAIN[] PROGMEM   = "<br/><br/><form action='/' method='post'><button>Main menu</button></form>";
const char HTTP_BTN_CONF[] PROGMEM   = "<br/><br/><form action='/cn' method='post'><button>Configuration menu</button></form>";
const char HTTP_LNK_ITEM[] PROGMEM   = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_LNK_SCAN[] PROGMEM   = "<div><a href='/w1'>Scan for wifi networks</a></div><br/>";
const char HTTP_FORM_WIFI[] PROGMEM  = "<fieldset><legend><b>&nbsp;Wifi parameters&nbsp;</b></legend><form method='post' action='sv'>"
                                       "<input id='w' name='w' value='1' hidden><input id='r' name='r' value='1' hidden>"
                                       "<br/><b>SSId</b> (" STA_SSID ")<br/><input id='s' name='s' length=32 placeholder='" STA_SSID "' value='{s1}'><br/>"
                                       "<br/><b>Password</b></br><input id='p' name='p' length=64 type='password' placeholder='" STA_PASS "' value='{p1}'><br/>"
                                       "<br/><b>Hostname</b> ({h0})<br/><input id='h' name='h' length=32 placeholder='" WIFI_HOSTNAME" ' value='{h1}'><br/>";
const char HTTP_FORM_MQTT[] PROGMEM  = "<fieldset><legend><b>&nbsp;MQTT parameters&nbsp;</b></legend><form method='post' action='sv'>"
                                       "<input id='w' name='w' value='2' hidden><input id='r' name='r' value='1' hidden>"
                                       "<br/><b>Host</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' length=32 placeholder='" MQTT_HOST" ' value='{m1}'><br/>"
                                       "<br/><b>Port</b> ({ml})<br/><input id='ml' name='ml' length=5 placeholder='{ml}' value='{m2}'><br/>"
                                       "<br/><b>Client Id</b> ({m0})<br/><input id='mc' name='mc' length=32 placeholder='" MQTT_CLIENT_ID "' value='{m3}'><br/>"
                                       "<br/><b>User</b> (" MQTT_USER ")<br/><input id='mu' name='mu' length=32 placeholder='" MQTT_USER "' value='{m4}'><br/>"
                                       "<br/><b>Password</b> (" MQTT_PASS ")<br/><input id='mp' name='mp' length=32 placeholder='" MQTT_PASS "' value='{m5}'><br/>"
                                       "<br/><b>Topic</b> (" MQTT_TOPIC ")<br/><input id='mt' name='mt' length=32 placeholder='" MQTT_TOPIC" ' value='{m6}'><br/>";
const char HTTP_FORM_LOG[] PROGMEM   = "<fieldset><legend><b>&nbsp;Logging parameters&nbsp;</b></legend><form method='post' action='sv'>"
                                       "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>"
                                       "<br/><b>Serial log level</b> ({ls})<br/><select id='ls' name='ls'>"
                                       "<option{a0value='0'>0 None</option>"
                                       "<option{a1value='1'>1 Error</option>"
                                       "<option{a2value='2'>2 Info</option>"
                                       "<option{a3value='3'>3 Debug</option>"
                                       "<option{a4value='4'>4 More debug</option>"
                                       "</select></br>"
                                       "<br/><b>Web log level</b> ({lw})<br/><select id='lw' name='lw'>"
                                       "<option{b0value='0'>0 None</option>"
                                       "<option{b1value='1'>1 Error</option>"
                                       "<option{b2value='2'>2 Info</option>"
                                       "<option{b3value='3'>3 Debug</option>"
                                       "<option{b4value='4'>4 More debug</option>"
                                       "</select></br>"
                                       "<br/><b>Syslog level</b> ({ll})<br/><select id='ll' name='ll'>"
                                       "<option{c0value='0'>0 None</option>"
                                       "<option{c1value='1'>1 Error</option>"
                                       "<option{c2value='2'>2 Info</option>"
                                       "<option{c3value='3'>3 Debug</option>"
                                       "<option{c4value='4'>4 More debug</option>"
                                       "</select></br>"
                                       "<br/><b>Syslog host</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' length=32 placeholder='" SYS_LOG_HOST "' value='{l2}'><br/>"
                                       "<br/><b>Syslog port</b> ({lp})<br/><input id='lp' name='lp' length=5 placeholder='{lp}' value='{l3}'><br/>"
                                       "<br/><b>Telemetric period</b> ({lt})<br/><input id='lt' name='lt' length=4 placeholder='{lt}' value='{l4}'><br/>";
const char HTTP_FORM_END[] PROGMEM   = "<br/><button type='submit'>Save</button></form></fieldset>";
const char HTTP_FORM_UPG[] PROGMEM   = "<div id='f1' name='f1' style='display:block;'>"
                                       "<fieldset><legend><b>&nbsp;Upgrade by web server&nbsp;</b></legend>"
                                       "<form method='post' action='u1'>"
                                       "<br/>OTA Url<br/><input id='o' name='o' length=80 placeholder='OTA_URL' value='{o1}'><br/>"
                                       "<br/><button type='submit'>Start upgrade</button></form>"
                                       "</fieldset><br/><br/>"
                                       "<fieldset><legend><b>&nbsp;Upgrade by file upload&nbsp;</b></legend>"
                                       "<form method='post' action='u2' enctype='multipart/form-data'>"
                                       "<br/><input type='file' name='u2'><br/>"
//                                       "<br/><button type='submit' onclick='this.disabled=true;this.form.submit();'>Start upgrade</button></form></fieldset>"
                                       "<br/><button type='submit' onclick='document.getElementById(\"f1\").style.display=\"none\";document.getElementById(\"f2\").style.display=\"block\";this.form.submit();'>Start upgrade</button></form>"
                                       "</fieldset>"
                                       "</div>"
                                       "<div id='f2' name='f2' style='display:none;text-align:center;'><b>Upload started ...</b></div>";
const char HTTP_FORM_CMND[] PROGMEM  = "<br/><textarea readonly id='t1' name='t1' cols='80' rows='16' wrap='off'></textarea><br/><br/>"
                                       "<form method='post' action='cm'>"
                                       "<input style='width:98%' id='" SUB_PREFIX "' name='" SUB_PREFIX "' length=80 placeholder='Enter command' autofocus><br/>"
//                                       "<br/><button type='submit'>Send command</button>"
                                       "</form>";
const char HTTP_COUNTER[] PROGMEM    = "<br/><div id='t' name='t' style='text-align:center;'></div>";
const char HTTP_END[] PROGMEM        = "</div>"
                                       "</body>"
                                       "</html>";

DNSServer *dnsServer;
ESP8266WebServer *webServer;

int _minimumQuality = -1;
boolean _removeDuplicateAPs = true;
int httpflag = 0, _wifimanager = 0, _uploaderror = 0, _colcount;

void startWebserver(int type, IPAddress ipweb)
{
  char log[LOGSZ];

  if (!httpflag) {
    if (!webServer) {
      webServer = new ESP8266WebServer(80);
      webServer->on("/", handleRoot);
      webServer->on("/cn", handleConfig);
      webServer->on("/w1", handleWifi1);
      webServer->on("/w0", handleWifi0);
      webServer->on("/mq", handleMqtt);
      webServer->on("/lg", handleLog);
      webServer->on("/sv", handleSave);
      webServer->on("/rt", handleReset);
      webServer->on("/up", handleUpgrade);
      webServer->on("/u1", handleUpgradeStart);
      webServer->on("/u2", HTTP_POST, handleUploadDone, handleUploadLoop);
      webServer->on("/cm", handleConsole);
      webServer->on("/ax", handleAjax);
      webServer->on("/in", handleInfo);
      webServer->on("/rb", handleRestart);
      webServer->on("/fwlink", handleRoot);  // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
      webServer->onNotFound(handleNotFound);
    }
    webServer->begin(); // Web server start
  }
  snprintf_P(log, sizeof(log), PSTR("HTTP: Webserver active on %s with IP address %s"), Hostname, ipweb.toString().c_str());
  addLog(LOG_LEVEL_INFO, log);
  if (type) httpflag = type;
}

void stopWebserver()
{
  if (httpflag) {
    webServer->close();
//    free(webServer);
    httpflag = 0;
    addLog_P(LOG_LEVEL_INFO, PSTR("HTTP: Webserver stopped"));
  }
}

void beginWifiManager()
{
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

  startWebserver(2, WiFi.softAPIP());
  _wifimanager = 1;
}

void pollDnsWeb()
{
  if (dnsServer) dnsServer->processNextRequest();
  if (webServer) webServer->handleClient();
}

void showPage(String &page)
{
  page.replace("{h}", Hostname);
  if (_wifimanager) {
    if (WIFI_configCounter()) {
      page.replace("<body>", "<body onload='u()'>");
      page += FPSTR(HTTP_COUNTER);
    }
  }
  page += FPSTR(HTTP_END);

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/html", page);
}

void handleRoot()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle root"));

  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  if (_wifimanager) {
    handleWifi0();
  } else {
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
    page += FPSTR(HTTP_BTN_TOGGL);
    page.replace("{r0}", (sysCfg.power) ? "ON" : "OFF");
    if (httpflag == 2)
      page += FPSTR(HTTP_BTN_MENU1);
      page += FPSTR(HTTP_BTN_RSTRT);
    showPage(page);
  }
}

void handleConfig()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configuration");
  page += FPSTR(HTTP_BTN_MENU2);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
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

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Wifi config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure Wifi");

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
          String item = FPSTR(HTTP_LNK_ITEM);
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
    page += FPSTR(HTTP_LNK_SCAN);
  }

  page += FPSTR(HTTP_FORM_WIFI);
  
  char str[33];
  if (!strcmp(WIFI_HOSTNAME, DEF_WIFI_HOSTNAME))
    snprintf_P(str, sizeof(str), PSTR(DEF_WIFI_HOSTNAME), sysCfg.mqtt_topic, ESP.getChipId() & 0x1FFF);
  else
    snprintf_P(str, sizeof(str), PSTR(WIFI_HOSTNAME));
  page.replace("{h0}", str);
  page.replace("{h1}", String(sysCfg.hostname));
  page.replace("{s1}", String(sysCfg.sta_ssid));
  page.replace("{p1}", String(sysCfg.sta_pwd));
  page += FPSTR(HTTP_FORM_END);
  if (_wifimanager)
    page += FPSTR(HTTP_BTN_RSTRT);
  else
    page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleMqtt()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle MQTT config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure MQTT");
  page += FPSTR(HTTP_FORM_MQTT);
  char str[33];
  if (!strcmp(MQTT_CLIENT_ID, DEF_MQTT_CLIENT_ID))
    snprintf_P(str, sizeof(str), PSTR(DEF_MQTT_CLIENT_ID), ESP.getChipId());
  else
    snprintf_P(str, sizeof(str), PSTR(MQTT_CLIENT_ID));
  page.replace("{m0}", str);
  page.replace("{m1}", String(sysCfg.mqtt_host));
  page.replace("{ml}", String((int)MQTT_PORT));
  page.replace("{m2}", String(sysCfg.mqtt_port));
  page.replace("{m3}", String(sysCfg.mqtt_client));
  page.replace("{m4}", String(sysCfg.mqtt_user));
  page.replace("{m5}", String(sysCfg.mqtt_pwd));
  page.replace("{m6}", String(sysCfg.mqtt_topic));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleLog()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Log config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Config logging");
  page += FPSTR(HTTP_FORM_LOG);
  page.replace("{ls}", String((int)SERIAL_LOG_LEVEL));
  page.replace("{lw}", String((int)WEB_LOG_LEVEL));
  page.replace("{ll}", String((int)SYS_LOG_LEVEL));
  for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
    page.replace("{a" + String(i), (i == sysCfg.seriallog_level) ? " selected " : " ");
    page.replace("{b" + String(i), (i == sysCfg.weblog_level) ? " selected " : " ");
    page.replace("{c" + String(i), (i == sysCfg.syslog_level) ? " selected " : " ");
  }  
  page.replace("{l2}", String(sysCfg.syslog_host));
  page.replace("{lp}", String((int)SYS_LOG_PORT));
  page.replace("{l3}", String(sysCfg.syslog_port));
  page.replace("{lt}", String((int)TELE_PERIOD));
  page.replace("{l4}", String(sysCfg.tele_period));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleSave()
{
  char log[LOGSZ];
  byte what = 0, restart;
  String result = "";

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Parameter save"));

  if (strlen(webServer->arg("w").c_str())) what = atoi(webServer->arg("w").c_str());
  switch (what) {
  case 1:
    strlcpy(sysCfg.hostname, (!strlen(webServer->arg("h").c_str())) ? WIFI_HOSTNAME : webServer->arg("h").c_str(), sizeof(sysCfg.hostname));
    if (strstr(sysCfg.hostname,"%")) strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
    strlcpy(sysCfg.sta_ssid, (!strlen(webServer->arg("s").c_str())) ? STA_SSID : webServer->arg("s").c_str(), sizeof(sysCfg.sta_ssid));
    strlcpy(sysCfg.sta_pwd, (!strlen(webServer->arg("p").c_str())) ? STA_PASS : webServer->arg("p").c_str(), sizeof(sysCfg.sta_pwd));
    snprintf_P(log, sizeof(log), PSTR("HTTP: Wifi Hostname %s, SSID %s and Password %s"), sysCfg.hostname, sysCfg.sta_ssid, sysCfg.sta_pwd);
    addLog(LOG_LEVEL_INFO, log);
    result += F("<br/>Trying to connect device to network<br/>If it fails reconnect to try again");
    break;
  case 2:
    strlcpy(sysCfg.mqtt_host, (!strlen(webServer->arg("mh").c_str())) ? MQTT_HOST : webServer->arg("mh").c_str(), sizeof(sysCfg.mqtt_host));
    sysCfg.mqtt_port = (!strlen(webServer->arg("ml").c_str())) ? MQTT_PORT : atoi(webServer->arg("ml").c_str());
    strlcpy(sysCfg.mqtt_client, (!strlen(webServer->arg("mc").c_str())) ? MQTT_CLIENT_ID : webServer->arg("mc").c_str(), sizeof(sysCfg.mqtt_client));
    if (strstr(sysCfg.mqtt_client,"%")) strlcpy(sysCfg.mqtt_client, DEF_MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
    strlcpy(sysCfg.mqtt_user, (!strlen(webServer->arg("mu").c_str())) ? MQTT_USER : webServer->arg("mu").c_str(), sizeof(sysCfg.mqtt_user));
    strlcpy(sysCfg.mqtt_pwd, (!strlen(webServer->arg("mp").c_str())) ? MQTT_PASS : webServer->arg("mp").c_str(), sizeof(sysCfg.mqtt_pwd));
    strlcpy(sysCfg.mqtt_topic, (!strlen(webServer->arg("mt").c_str())) ? MQTT_TOPIC : webServer->arg("mt").c_str(), sizeof(sysCfg.mqtt_topic));
    snprintf_P(log, sizeof(log), PSTR("HTTP: MQTT Host %s, Port %d, Client %s, User %s, Password %s, Topic %s"),
      sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, sysCfg.mqtt_user, sysCfg.mqtt_pwd, sysCfg.mqtt_topic);
    addLog(LOG_LEVEL_INFO, log);
    break;
  case 3:
    sysCfg.seriallog_level = (!strlen(webServer->arg("ls").c_str())) ? SERIAL_LOG_LEVEL : atoi(webServer->arg("ls").c_str());
    sysCfg.weblog_level = (!strlen(webServer->arg("lw").c_str())) ? WEB_LOG_LEVEL : atoi(webServer->arg("lw").c_str());
    sysCfg.syslog_level = (!strlen(webServer->arg("ll").c_str())) ? SYS_LOG_LEVEL : atoi(webServer->arg("ll").c_str());
    strlcpy(sysCfg.syslog_host, (!strlen(webServer->arg("lh").c_str())) ? SYS_LOG_HOST : webServer->arg("lh").c_str(), sizeof(sysCfg.syslog_host));
    sysCfg.syslog_port = (!strlen(webServer->arg("lp").c_str())) ? SYS_LOG_PORT : atoi(webServer->arg("lp").c_str());
    sysCfg.tele_period = (!strlen(webServer->arg("lt").c_str())) ? TELE_PERIOD : atoi(webServer->arg("lt").c_str());
    snprintf_P(log, sizeof(log), PSTR("HTTP: Logging Seriallog %d, Weblog %d, Syslog %d, Host %s, Port %d, TelePeriod %d"),
      sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.syslog_port, sysCfg.tele_period);
    addLog(LOG_LEVEL_INFO, log);
    break;
  }

  restart = (!strlen(webServer->arg("r").c_str())) ? 1 : atoi(webServer->arg("r").c_str());
  if (restart) {
    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Save parameters");
    page += F("<div style='text-align:center;'><b>Parameters saved</b><br/>");
    page += result;
    page += F("</div>");
    page += FPSTR(HTTP_MSG_RSTRT);
    if (!_wifimanager) page += FPSTR(HTTP_BTN_MAIN);
    _wifimanager = 0;
    showPage(page);

    restartflag = 2;
  } else {
    handleConfig();
  }
}

void handleReset()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Reset parameters"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Default parameters");
  page += F("<div style='text-align:center;'>Parameters reset to default</div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR("reset 1"));
  do_cmnd(svalue);
}

void handleUpgrade()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle upgrade"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Firmware upgrade");
  page += FPSTR(HTTP_FORM_UPG);
  page.replace("{o1}", String(sysCfg.otaUrl));
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  _uploaderror = 0;
}

void handleUpgradeStart()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Firmware upgrade start"));
  WIFI_configCounter();

  if (strlen(webServer->arg("o").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR("otaurl %s"), webServer->arg("o").c_str());
    do_cmnd(svalue);
  }
  
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += F("<div style='text-align:center;'><b>Upgrade started ...</b></div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR("upgrade 1"));
  do_cmnd(svalue);
}

void handleUploadDone()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Firmware upload done"));
  WIFI_configCounter();

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += F("<div style='text-align:center;'><b>Upload ");
  if (_uploaderror) {
    page += F("<font color='red'>failed</font></b>");
    if (_uploaderror == 1)
      page += F("<br/><br/>No file selected");
    else if (_uploaderror == 3)
      page += F("<br/><br/>File magic header does not start with 0xE9");
    else if (_uploaderror == 4)
      page += F("<br/><br/>File flash size is larger than device flash size");
    else {
      page += F("<br/><br/>Upload error code ");
      page += String(_uploaderror);
    }
    if (Update.hasError()) {
      page += F("<br/><br/>Update error code ");
      page += String(Update.getError());
    }
  } else {
    page += F("<font color='green'>successful</font></b><br/><br/>Device will restart in a few seconds");
    restartflag = 2;
  }
  page += F("</div><br/>");
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleUploadLoop()
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  char log[LOGSZ];
  boolean _serialoutput = (LOG_LEVEL_DEBUG <= sysCfg.seriallog_level);

  if (_uploaderror) {
    Update.end();
    return;
  }
  
  HTTPUpload& upload = webServer->upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    if (upload.filename.c_str()[0] == 0)
    {
      _uploaderror = 1;
      return;
    }
    WiFiUDP::stopAll();
    snprintf_P(log, sizeof(log), PSTR("Upload: File %s ..."), upload.filename.c_str());
    addLog(LOG_LEVEL_INFO, log);

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) {         //start with max available size
      if (_serialoutput) Update.printError(Serial);
      _uploaderror = 2;
      return;
    }
    _colcount = 0;
  } else if (!_uploaderror && (upload.status == UPLOAD_FILE_WRITE)) {
    if (upload.totalSize == 0)
    {
      if (upload.buf[0] != 0xE9) {
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Upload: File magic header does not start with 0xE9"));
        _uploaderror = 3;
        return;
      }
      uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
      if(bin_flash_size > ESP.getFlashChipRealSize()) {
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Upload: File flash size is larger than device flash size"));
        _uploaderror = 4;
        return;
      }
    }
    if (!_uploaderror && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
      if (_serialoutput) Update.printError(Serial);
      _uploaderror = 5;
      return;
    }
    if (_serialoutput) {
      Serial.printf(".");
      _colcount++;
      if (!(_colcount % 80)) Serial.println();
    }
  } else if(!_uploaderror && (upload.status == UPLOAD_FILE_END)){
    if (_serialoutput && (_colcount % 80)) Serial.println();
    if (Update.end(true)) { // true to set the size to the current progress
      snprintf_P(log, sizeof(log), PSTR("Upload: Successful %u bytes. Restarting"), upload.totalSize);
      addLog(LOG_LEVEL_INFO, log);
    } else {
      if (_serialoutput) Update.printError(Serial);
      _uploaderror = 6;
      return;
    }
  } else if(upload.status == UPLOAD_FILE_ABORTED) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Upload: Update was aborted"));
    _uploaderror = 7;
    Update.end();
  }
  delay(0);
}

void handleConsole()
{
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle console"));

  if (strlen(webServer->arg(SUB_PREFIX).c_str())) {
    snprintf_P(svalue, sizeof(svalue), webServer->arg(SUB_PREFIX).c_str());
    do_cmnd(svalue);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Console");
  page.replace("<body>", "<body onload='l()'>");
  page += FPSTR(HTTP_FORM_CMND);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleAjax()
{
  String message = "";

  byte counter = logidx;  // Points to oldest entry
  do {
    if (Log[counter].length()) {
      if (message.length()) message += F("\n");
      message += Log[counter];
    }
    counter++;
    if (counter > MAX_LOG_LINES -1) counter = 0;
  } while (counter != logidx);
  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/plain", message);
}

void handleInfo()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle info"));

  int freeMem = ESP.getFreeHeap();

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Information");
//  page += F("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>");
  page += F("<table style'width:100%'>");
  page += F("<tr><td><b>Version</b></td><td>"); page += Version; page += F("</td></tr>");
  page += F("<tr><td><b>Core version</b></td><td>"); page += ESP.getCoreVersion(); page += F("</td></tr>");
  page += F("<tr><td><b>SDK version</b></td><td>"); page += String(ESP.getSdkVersion()); page += F("</td></tr>");
//  page += F("<tr><td><b>Boot version</b></td><td>"); page += String(ESP.getBootVersion()); page += F("</td></tr>");
  page += F("<tr><td><b>Uptime</b></td><td>"); page += String(uptime); page += F(" Hours</td></tr>");
  page += F("<tr><td><b>Flash write count</b></td><td>"); page += String(sysCfg.saveFlag); page += F("</td></tr>");
  page += F("<tr><td><b>Boot count</b></td><td>"); page += String(sysCfg.bootcount); page += F("</td></tr>");
  page += F("<tr><td><b>Reset reason</b></td><td>"); page += ESP.getResetReason(); page += F("</td></tr>");
  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><td><b>Hostname</b></td><td>"); page += Hostname; page += F("</td></tr>");
  if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
    page += F("<tr><td><b>IP address</b></td><td>"); page += WiFi.localIP().toString(); page += F("</td></tr>");
    page += F("<tr><td><b>Gateway</b></td><td>"); page += WiFi.gatewayIP().toString(); page += F("</td></tr>");
    page += F("<tr><td><b>MAC address</b></td><td>"); page += WiFi.macAddress(); page += F("</td></tr>");
  }
  if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
    page += F("<tr><td><b>AP IP address</b></td><td>"); page += WiFi.softAPIP().toString(); page += F("</td></tr>");
    page += F("<tr><td><b>AP Gateway</b></td><td>"); page += WiFi.softAPIP().toString(); page += F("</td></tr>");
    page += F("<tr><td><b>AP MAC address</b></td><td>"); page += WiFi.softAPmacAddress(); page += F("</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><td><b>ESP Chip id</b></td><td>"); page += String(ESP.getChipId()); page += F("</td></tr>");
  page += F("<tr><td><b>Flash Chip id</b></td><td>"); page += String(ESP.getFlashChipId()); page += F("</td></tr>");
  page += F("<tr><td><b>Flash size</b></td><td>"); page += String(ESP.getFlashChipRealSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Sketch flash size</b></td><td>"); page += String(ESP.getFlashChipSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Sketch size</b></td><td>"); page += String(ESP.getSketchSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Free sketch space</b></td><td>"); page += String(ESP.getFreeSketchSpace() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Free memory</b></td><td>"); page += String(freeMem / 1024); page += F("kB</td></tr>");
  page += F("</table>");
//  page += F("</fieldset>");
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleRestart()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Restarting"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_MSG_RSTRT);
  if (!_wifimanager) page += FPSTR(HTTP_BTN_MAIN);
  _wifimanager = 0;
  showPage(page);

  restartflag = 2;
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
  webServer->send(404, "text/plain", message);
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
  if (!isIp(webServer->hostHeader())) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Request redirected to captive portal"));

    webServer->sendHeader("Location", String("http://") + webServer->client().localIP().toString(), true);
    webServer->send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
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
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_CONFIG_SEC   60   // seconds before restart
#define WIFI_MANAGER_SEC  120  // seconds before restart
#define WIFI_CHECKSEC     20   // seconds
#define WIFI_RETRY        16

uint8_t wificounter, wifiretry, _wpsresult, wificonfigflag = 0, _wifiConfigCounter = 0;

boolean WIFI_configCounter()
{
  if (_wifiConfigCounter) _wifiConfigCounter = WIFI_MANAGER_SEC;
  return (_wifiConfigCounter);
}

extern "C" {
#include "user_interface.h"
}

void WIFI_wps_status_cb(wps_cb_status status);

void WIFI_wps_status_cb(wps_cb_status status)
{
  char log[LOGSZ];

/* from user_interface.h:
  enum wps_cb_status {
    WPS_CB_ST_SUCCESS = 0,
    WPS_CB_ST_FAILED,
    WPS_CB_ST_TIMEOUT,
    WPS_CB_ST_WEP,      // WPS failed because that WEP is not supported
    WPS_CB_ST_SCAN_ERR, // can not find the target WPS AP
  };
*/

  _wpsresult = status;
  snprintf_P(log, sizeof(log), PSTR("WPSconfig: Result %d"), _wpsresult);
  addLog(LOG_LEVEL_DEBUG, log);
  if (status == WPS_CB_ST_SUCCESS) wifi_wps_disable();
  _wifiConfigCounter = 3;
}

boolean WIFI_beginWPSConfig(void)
{
  WiFi.disconnect();
  if (!wifi_wps_disable()) return false;
  if (!wifi_wps_enable(WPS_TYPE_PBC)) return false;  // so far only WPS_TYPE_PBC is supported (SDK 2.0.0)
  if (!wifi_set_wps_cb((wps_st_cb_t) &WIFI_wps_status_cb)) return false;
  if (!wifi_wps_start()) return false;
  return true;
}

void WIFI_config(int type)
{
  if (!wificonfigflag) {
    wificonfigflag = type;
    _wifiConfigCounter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    wificounter = _wifiConfigCounter +5;
    blinks = 1999;
    if (wificonfigflag == WIFI_SMARTCONFIG) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Active for 1 minute"));
      WiFi.beginSmartConfig();
    }
    else if (wificonfigflag == WIFI_WPSCONFIG) {
      _wpsresult = 99;
      if (WIFI_beginWPSConfig()) 
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Active for 1 minute"));
      else {
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Failed to start"));
        _wifiConfigCounter = 3;
      }
    }
#ifdef USE_WEBSERVER
    else {
      addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Active for 1 minute for initial request"));
      beginWifiManager();
    }
#endif  // USE_WEBSERVER
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
        WIFI_config(sysCfg.sta_config);
        break;
      default:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_IDLE"));
        if (wifiretry == (WIFI_RETRY / 2)) WiFi.begin();
        wifiretry--;
        if (wifiretry)
          wificounter = 1;
        else
          WIFI_config(sysCfg.sta_config);
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
  case WIFI_WPSCONFIG:
    WIFI_config(param);
    break;
  default:
    if (_wifiConfigCounter) {
      _wifiConfigCounter--;
      wificounter = _wifiConfigCounter +5;
      if (_wifiConfigCounter) {
        if ((wificonfigflag == WIFI_SMARTCONFIG) && WiFi.smartConfigDone()) {
          _wifiConfigCounter = 0;
          if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid));
          if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd));
          snprintf_P(log, sizeof(log), PSTR("Smartconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
          addLog(LOG_LEVEL_INFO, log);
        }
        if ((wificonfigflag == WIFI_WPSCONFIG) && (!_wpsresult)) {
          _wifiConfigCounter = 0;
          if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid));
          if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd));
          snprintf_P(log, sizeof(log), PSTR("WPSconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
          addLog(LOG_LEVEL_INFO, log);
        }
      }
      if (_wifiConfigCounter == 0) {
        if (wificonfigflag == WIFI_SMARTCONFIG) WiFi.stopSmartConfig();
        restartflag = 2;     
      }
    } else {
      if (wificounter <= 0) {
        addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Checking connection..."));
        wificounter = WIFI_CHECKSEC;
        WIFI_check_ip();
      }
#ifdef USE_WEBSERVER
      if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
        if (wificonfigflag != WIFI_MANAGER) {
          if (sysCfg.webserver && (sysCfg.webserver != httpflag)) startWebserver(sysCfg.webserver, WiFi.localIP());
          if (!sysCfg.webserver && httpflag) stopWebserver();
        }
      }
#endif  // USE_WEBSERVER
    }
  }
}

int WIFI_State()
{
  int state;

  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) state = WIFI_STATUS;
  if (wificonfigflag) state = wificonfigflag;
  return state;
}

void WIFI_Connect(char *Hostname)
{
  char log[LOGSZ];

  WiFi.persistent(false);   // Solve possible wifi init errors
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFi.hostname(Hostname);
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to %s (%s) as %s"), sysCfg.sta_ssid, sysCfg.sta_pwd, Hostname);
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
#endif  // USE_TICKER

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
#endif  // USE_TICKER
}

#ifdef SEND_TELEMETRY_DS18B20
/*********************************************************************************************\
 * DS18B20
 * 
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy
\*********************************************************************************************/

uint8_t dsb_reset()
{
  uint8_t r;
  uint8_t retries = 125;
  
  pinMode(DSB_PIN, INPUT);
  do  {                                 // wait until the wire is high... just in case
    if (--retries == 0) return 0;
    delayMicroseconds(2);
  } while (!digitalRead(DSB_PIN));
  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(492);               // Dallas spec. = Min. 480uSec. Arduino 500uSec.
  pinMode(DSB_PIN, INPUT);              //Float
  delayMicroseconds(40);
  r = !digitalRead(DSB_PIN);
  delayMicroseconds(420);
  return r;
}

uint8_t dsb_read_bit(void)
{
  uint8_t r;

  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(3);
  pinMode(DSB_PIN, INPUT);             // let pin float, pull up will raise
  delayMicroseconds(10);
  r = digitalRead(DSB_PIN);
  delayMicroseconds(53);
  return r;
}

uint8_t dsb_read(void)
{
  uint8_t bitMask;
  uint8_t r = 0;

  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    if (dsb_read_bit()) r |= bitMask;
  return r;
}

void dsb_write_bit(uint8_t v)
{
  if (v & 1) {
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(10);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(55);
  } else {
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(65);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(5);
  }
}

void dsb_write(uint8_t ByteToWrite)
{
  uint8_t bitMask;
  
  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    dsb_write_bit((bitMask & ByteToWrite) ? 1 : 0);
}

void dsb_readTempPrep()
{
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
}

boolean dsb_readTemp(float &t)
{
  int16_t DSTemp;
  byte msb, lsb;

  t = NAN;

/*
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
  delay(800);
*/
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0xBE);           // Read scratchpad
  lsb = dsb_read();
  msb = dsb_read();
  dsb_reset();
  
  DSTemp = (msb << 8) + lsb;
  t = (float(DSTemp) * 0.0625);
  return (!isnan(t));
}
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DHT
/*********************************************************************************************\
 * DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321)
 * 
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 * Source: Adafruit Industries https://github.com/adafruit/DHT-sensor-library
\*********************************************************************************************/

#define MIN_INTERVAL 2000

uint8_t data[5];
uint32_t _lastreadtime, _maxcycles;
bool _lastresult;

void dht_readPrep()
{
  digitalWrite(DHT_PIN, HIGH);
}

uint32_t dht_expectPulse(bool level)
{
  uint32_t count = 0;
  
  while (digitalRead(DHT_PIN) == level)
    if (count++ >= _maxcycles) return 0;
  return count;
}

boolean dht_read()
{
  char log[LOGSZ];
  uint32_t cycles[80];
  uint32_t currenttime = millis();
  
  if ((currenttime - _lastreadtime) < 2000) {
    return _lastresult;
  }
  _lastreadtime = currenttime;

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

//  digitalWrite(DHT_PIN, HIGH);
//  delay(250);

  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);

  noInterrupts();
  digitalWrite(DHT_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT_PIN, INPUT_PULLUP);
  delayMicroseconds(10);
  if (dht_expectPulse(LOW) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal low pulse"));
    _lastresult = false;
    return _lastresult;
  }
  if (dht_expectPulse(HIGH) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal high pulse"));
    _lastresult = false;
    return _lastresult;
  }
  for (int i=0; i<80; i+=2) {
    cycles[i]   = dht_expectPulse(LOW);
    cycles[i+1] = dht_expectPulse(HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for pulse"));
      _lastresult = false;
      return _lastresult;
    }
    data[i/8] <<= 1;
    if (highCycles > lowCycles) data[i/8] |= 1;
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Received %02X, %02X, %02X, %02X, %02X =? %02X"),
    data[0], data[1], data[2], data[3], data[4], (data[0] + data[1] + data[2] + data[3]) & 0xFF);
  addLog(LOG_LEVEL_DEBUG, log);

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  }
  else {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Checksum failure"));
    _lastresult = false;
    return _lastresult;
  }
}

float dht_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean dht_readTempHum(bool S, float &t, float &h)
{
  t = NAN;
  h = NAN;

  if (dht_read()) {
    switch (DHT_TYPE) {
    case DHT11:
      h = data[0];
      t = data[2];
      if(S) t = dht_convertCtoF(t);
      break;
    case DHT22:
    case DHT21:
      h = data[0];
      h *= 256;
      h += data[1];
      h *= 0.1;
      t = data[2] & 0x7F;
      t *= 256;
      t += data[3];
      t *= 0.1;
      if (data[2] & 0x80) t *= -1;
      if(S) t = dht_convertCtoF(t);
      break;
    }
  }
  return (!isnan(t) && !isnan(h)); 
}

void dht_init()
{
  char log[LOGSZ];
  _maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
  pinMode(DHT_PIN, INPUT_PULLUP);
  _lastreadtime = -MIN_INTERVAL;

  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), _maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}
#endif  // SEND_TELEMETRY_DHT

/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char mess[MESSZ], str[TOPSZ+MESSZ];

  portUDP.beginPacket(sysCfg.syslog_host, sysCfg.syslog_port);
  strlcpy(mess, message, sizeof(mess));
  mess[sizeof(mess)-1] = 0;
  snprintf_P(str, sizeof(str), PSTR("%s ESP-%s"), Hostname, mess);
  portUDP.write(str);
  portUDP.endPacket();
}

void addLog(byte loglevel, const char *line)
{
  char mxtime[9];
  
  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
  
#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.printf("%s %s\n", mxtime, line);  
#endif  // DEBUG_ESP_PORT
#ifdef USE_SERIAL
  if (loglevel <= sysCfg.seriallog_level) Serial.printf("%s %s\n", mxtime, line);
#endif  // USE_SERIAL
#ifdef USE_WEBSERVER
  if (loglevel <= sysCfg.weblog_level) {
//    Log[logidx] = String(mxtime);
//    Log[logidx] += " ";
//    Log[logidx] += String(line);

    Log[logidx] = String(mxtime) + " " + String(line);

    logidx++;
    if (logidx > MAX_LOG_LINES -1) logidx = 0;
  }
#endif  // USE_WEBSERVER
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

