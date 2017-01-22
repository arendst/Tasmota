/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_WEBSERVER
/*********************************************************************************************\
 * Web server and WiFi Manager
 *
 * Enables configuration and reconfiguration of WiFi credentials using a Captive Portal
 * Source by AlexT (https://github.com/tzapu)
\*********************************************************************************************/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

const char HTTP_HEAD[] PROGMEM =
  "<!DOCTYPE html><html lang=\"en\">"
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
    "document.getElementById('s1').value=l.innerText||l.textContent;"
    "document.getElementById('p1').focus();"
  "}"
  "var sn=0;"
  "function l(){"
    "var e=document.getElementById('t1');"
    "if(e.scrollTop>=sn){"
      "var x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "e.value=x.responseText;"
          "e.scrollTop=100000;"
          "sn=e.scrollTop;"
        "}"
      "};"
      "x.open('GET','ax',true);"
      "x.send();"
    "}"
    "setTimeout(l,2000);"
  "}"
  "</script>"
  "<style>"
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "input{width:95%;}"
  "select{width:100%;}"
  "textarea{resize:none;width:98%;height:312px;padding:5px;overflow:auto;}"
  "body{text-align:center;font-family:verdana;}"
  "td{padding:0px;}"
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
  "<div style='text-align:center;'><h3>{ha} Module</h3><h2>{h}</h2></div>";
const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br/><div style='text-align:center;'>Device will restart in a few seconds</div><br/>";
const char HTTP_BTN_MENU1[] PROGMEM =
  "<br/><form action='/cn' method='post'><button>Configuration</button></form>"
  "<br/><form action='/in' method='post'><button>Information</button></form>"
  "<br/><form action='/up' method='post'><button>Firmware upgrade</button></form>"
  "<br/><form action='/cs' method='post'><button>Console</button></form>";
const char HTTP_BTN_RSTRT[] PROGMEM =
  "<br/><form action='/rb' method='post'><button>Restart</button></form>";
const char HTTP_BTN_MENU2[] PROGMEM =
  "<br/><form action='/md' method='post'><button>Configure Module</button></form>"
  "<br/><form action='/w0' method='post'><button>Configure WiFi</button></form>";
const char HTTP_BTN_MENU3[] PROGMEM =
  "<br/><form action='/mq' method='post'><button>Configure MQTT</button></form>"
#ifdef USE_DOMOTICZ
  "<br/><form action='/dm' method='post'><button>Configure Domoticz</button></form>"
#endif  // USE_DOMOTICZ
  "";
const char HTTP_BTN_MENU4[] PROGMEM =
  "<br/><form action='/lg' method='post'><button>Configure Logging</button></form>"
  "<br/><form action='/co' method='post'><button>Configure Other</button></form>"
  "<br/><form action='/rt' method='post'><button>Reset Configuration</button></form>";
const char HTTP_BTN_MAIN[] PROGMEM =
  "<br/><br/><form action='/' method='post'><button>Main menu</button></form>";
const char HTTP_BTN_CONF[] PROGMEM =
  "<br/><br/><form action='/cn' method='post'><button>Configuration menu</button></form>";
const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Module parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='6' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>Module type</b> ({mt})<br/><select id='mt' name='mt'>";
const char HTTP_LNK_ITEM[] PROGMEM =
  "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_LNK_SCAN[] PROGMEM =
  "<div><a href='/w1'>Scan for wifi networks</a></div><br/>";
const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Wifi parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='1' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>AP1 SSId</b> (" STA_SSID1 ")<br/><input id='s1' name='s1' length=32 placeholder='" STA_SSID1 "' value='{s1}'><br/>"
  "<br/><b>AP1 Password</b></br><input id='p1' name='p1' length=64 type='password' placeholder='" STA_PASS1 "' value='{p1}'><br/>"
  "<br/><b>AP2 SSId</b> (" STA_SSID2 ")<br/><input id='s2' name='s2' length=32 placeholder='" STA_SSID2 "' value='{s2}'><br/>"
  "<br/><b>AP2 Password</b></br><input id='p2' name='p2' length=64 type='password' placeholder='" STA_PASS2 "' value='{p2}'><br/>"
  "<br/><b>Hostname</b> ({h0})<br/><input id='h' name='h' length=32 placeholder='" WIFI_HOSTNAME" ' value='{h1}'><br/>";
const char HTTP_FORM_MQTT[] PROGMEM =
  "<fieldset><legend><b>&nbsp;MQTT parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='2' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>Host</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' length=32 placeholder='" MQTT_HOST" ' value='{m1}'><br/>"
  "<br/><b>Port</b> (" STR(MQTT_PORT) ")<br/><input id='ml' name='ml' length=5 placeholder='" STR(MQTT_PORT) "' value='{m2}'><br/>"
  "<br/><b>Client Id</b> ({m0})<br/><input id='mc' name='mc' length=32 placeholder='" MQTT_CLIENT_ID "' value='{m3}'><br/>"
  "<br/><b>User</b> (" MQTT_USER ")<br/><input id='mu' name='mu' length=32 placeholder='" MQTT_USER "' value='{m4}'><br/>"
  "<br/><b>Password</b><br/><input id='mp' name='mp' length=32 type='password' placeholder='" MQTT_PASS "' value='{m5}'><br/>"
  "<br/><b>Topic</b> (" MQTT_TOPIC ")<br/><input id='mt' name='mt' length=32 placeholder='" MQTT_TOPIC" ' value='{m6}'><br/>";
#ifdef USE_DOMOTICZ
const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Domoticz parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='4' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>In topic</b> (" DOMOTICZ_IN_TOPIC ")<br/><input id='it' name='it' length=32 placeholder='" DOMOTICZ_IN_TOPIC "' value='{d1}'><br/>"
  "<br/><b>Out topic</b> (" DOMOTICZ_OUT_TOPIC ")<br/><input id='ot' name='ot' length=32 placeholder='" DOMOTICZ_OUT_TOPIC "' value='{d2}'><br/>"
  "<br/><b>Idx 1</b> (" STR(DOMOTICZ_RELAY_IDX1) ")<br/><input id='ix' name='ix' length=8 placeholder='" STR(DOMOTICZ_RELAY_IDX1) "' value='{d3}'><br/>";
const char HTTP_FORM_DOMOTICZ2[] PROGMEM =
  "<br/><b>Update timer</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")<br/><input id='ut' name='ut' length=32 placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='{d7}'><br/>";
#endif  // USE_DOMOTICZ
const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Logging parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<br/><b>{b0} level</b> ({b1})<br/><select id='{b2}' name='{b2}'>"
  "<option{a0value='0'>0 None</option>"
  "<option{a1value='1'>1 Error</option>"
  "<option{a2value='2'>2 Info</option>"
  "<option{a3value='3'>3 Debug</option>"
  "<option{a4value='4'>4 More debug</option>"
  "</select></br>";  
const char HTTP_FORM_LOG3[] PROGMEM =
  "<br/><b>Syslog host</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' length=32 placeholder='" SYS_LOG_HOST "' value='{l2}'><br/>"
  "<br/><b>Syslog port</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' length=5 placeholder='" STR(SYS_LOG_PORT) "' value='{l3}'><br/>"
  "<br/><b>Telemetric period</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' length=4 placeholder='" STR(TELE_PERIOD) "' value='{l4}'><br/>";
const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Other parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='5' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><input style='width:10%;float:left' id='r1' name='r1' type='checkbox'{r1}><b>MQTT enable</b><br/>";
const char HTTP_FORM_END[] PROGMEM =
  "<br/><button type='submit'>Save</button></form></fieldset>";
const char HTTP_FORM_UPG[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;Upgrade by web server&nbsp;</b></legend>"
  "<form method='post' action='u1'>"
  "<br/>OTA Url<br/><input id='o' name='o' length=80 placeholder='OTA_URL' value='{o1}'><br/>"
  "<br/><button type='submit'>Start upgrade</button></form>"
  "</fieldset><br/><br/>"
  "<fieldset><legend><b>&nbsp;Upgrade by file upload&nbsp;</b></legend>"
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br/><input type='file' name='u2'><br/>"
//  "<br/><button type='submit' onclick='this.disabled=true;this.form.submit();'>Start upgrade</button></form></fieldset>"
  "<br/><button type='submit' onclick='document.getElementById(\"f1\").style.display=\"none\";document.getElementById(\"f2\").style.display=\"block\";this.form.submit();'>Start upgrade</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>Upload started ...</b></div>";
const char HTTP_FORM_CMND[] PROGMEM =
  "<br/><textarea readonly id='t1' name='t1' cols='80' wrap='off'></textarea><br/><br/>"
  "<form method='post' action='cs'>"
  "<input style='width:98%' id='c1' name='c1' length=80 placeholder='Enter command' autofocus><br/>"
//  "<br/><button type='submit'>Send command</button>"
  "</form>";
const char HTTP_COUNTER[] PROGMEM =
  "<br/><div id='t' name='t' style='text-align:center;'></div>";
const char HTTP_END[] PROGMEM =
  "</div>"
  "</body>"
  "</html>";
#ifdef USE_WEMO_EMULATION
const char WEMO_EVENTSERVICE_XML[] PROGMEM =
  "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
  "<actionList>"
    "<action>"
      "<name>SetBinaryState</name>"
      "<argumentList>"
        "<argument>"
          "<retval/>"
          "<name>BinaryState</name>"
          "<relatedStateVariable>BinaryState</relatedStateVariable>"
          "<direction>in</direction>"
        "</argument>"
      "</argumentList>"
      "<serviceStateTable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>BinaryState</name>"
          "<dataType>Boolean</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>level</name>"
          "<dataType>string</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
      "</serviceStateTable>"
    "</action>"
  "</scpd>\r\n"
  "\r\n";
const char WEMO_SETUP_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root>"
    "<device>"
      "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
      "<friendlyName>{x1}</friendlyName>"
      "<manufacturer>Belkin International Inc.</manufacturer>"
      "<modelName>Sonoff Socket</modelName>"
      "<modelNumber>3.1415</modelNumber>"
      "<UDN>uuid:{x2}</UDN>"
      "<serialNumber>{x3}</serialNumber>"
      "<binaryState>0</binaryState>"
      "<serviceList>"
        "<service>"
          "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
          "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
          "<controlURL>/upnp/control/basicevent1</controlURL>"
          "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
          "<SCPDURL>/eventservice.xml</SCPDURL>"
        "</service>"
      "</serviceList>"
    "</device>"
  "</root>\r\n"
  "\r\n";
#endif  // USE_WEMO_EMULATION
#ifdef USE_HUE_EMULATION
const char HUE_DESCRIPTION_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
  "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
  "</specVersion>"
  "<URLBase>http://{x1}/</URLBase>"
  "<device>"
      "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
      "<friendlyName>Amazon-Echo-HA-Bridge ({x1})</friendlyName>"
      "<manufacturer>Royal Philips Electronics</manufacturer>"
      "<modelName>Philips hue bridge 2012</modelName>"
      "<modelNumber>929000226503</modelNumber>"
      "<UDN>uuid:{x2}</UDN>"
  "</device>"
  "</root>\r\n"
  "\r\n";

const char HUE_LIGHT_STATUS_JSON[] PROGMEM =
  "{\"state\":"
      "{\"on\":{state},"
      "\"bri\":0,"
      "\"hue\":0,"
      "\"sat\":0,"
      "\"effect\":\"none\","
      "\"ct\":0,"
      "\"alert\":\"none\","
      "\"reachable\":true"
  "},"
  "\"type\":\"Dimmable light\","
  "\"name\":\"{j1}\","
  "\"modelid\":\"LWB004\","
  "\"manufacturername\":\"Philips\","
  "\"uniqueid\":\"{j2}\","
  "\"swversion\":\"66012040\""
  "}";
#endif  // USE_HUE_EMULATION

#define DNS_PORT 53
enum http_t {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER};

DNSServer *dnsServer;
ESP8266WebServer *webServer;

boolean _removeDuplicateAPs = true;
int _minimumQuality = -1, _httpflag = HTTP_OFF, _uploaderror = 0, _colcount;

void startWebserver(int type, IPAddress ipweb)
{
  char log[LOGSZ];

  if (!_httpflag) {
    if (!webServer) {
      webServer = new ESP8266WebServer(80);
      webServer->on("/", handleRoot);
      webServer->on("/cn", handleConfig);
      webServer->on("/md", handleModule);
      webServer->on("/w1", handleWifi1);
      webServer->on("/w0", handleWifi0);
      if (sysCfg.mqtt_enabled) {
        webServer->on("/mq", handleMqtt);
#ifdef USE_DOMOTICZ
        webServer->on("/dm", handleDomoticz);
#endif  // USE_DOMOTICZ
      }
      webServer->on("/lg", handleLog);
      webServer->on("/co", handleOther);
      webServer->on("/sv", handleSave);
      webServer->on("/rt", handleReset);
      webServer->on("/up", handleUpgrade);
      webServer->on("/u1", handleUpgradeStart);
      webServer->on("/u2", HTTP_POST, handleUploadDone, handleUploadLoop);
      webServer->on("/cm", handleCmnd);
      webServer->on("/cs", handleConsole);
      webServer->on("/ax", handleAjax);
      webServer->on("/in", handleInfo);
      webServer->on("/rb", handleRestart);
      webServer->on("/fwlink", handleRoot);  // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
#ifdef USE_WEMO_EMULATION
      webServer->on("/upnp/control/basicevent1", HTTP_POST, handleUPnPevent);
      webServer->on("/eventservice.xml", handleUPnPservice);
      webServer->on("/setup.xml", handleUPnPsetup);
#endif  // USE_WEMO_EMULATION
#ifdef USE_HUE_EMULATION
      webServer->on("/description.xml", handleUPnPsetup);
#endif  // USE_HUE_EMULATION
      webServer->onNotFound(handleNotFound);
    }
    webServer->begin(); // Web server start
  }
  if (_httpflag != type) {
    snprintf_P(log, sizeof(log), PSTR("HTTP: Webserver active on %s%s with IP address %s"),
      Hostname, (mDNSbegun)?".local":"", ipweb.toString().c_str());
    addLog(LOG_LEVEL_INFO, log);
  }
  if (type) _httpflag = type;
}

void stopWebserver()
{
  if (_httpflag) {
    webServer->close();
    _httpflag = HTTP_OFF;
    addLog_P(LOG_LEVEL_INFO, PSTR("HTTP: Webserver stopped"));
  }
}

void beginWifiManager()
{
  // setup AP
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
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

  startWebserver(HTTP_MANAGER, WiFi.softAPIP());
}

void pollDnsWeb()
{
  if (dnsServer) dnsServer->processNextRequest();
  if (webServer) webServer->handleClient();
}

void showPage(String &page)
{
  page.replace("{ha}", my_module.name);
  page.replace("{h}", String(sysCfg.friendlyname[0]));
//  page.replace("{ha}", Hostname);
  if (_httpflag == HTTP_MANAGER) {
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
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle root"));

  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  if (_httpflag == HTTP_MANAGER) {
    handleWifi0();
  } else {

    String page = FPSTR(HTTP_HEAD);
//    page.replace("<meta", "<meta http-equiv=\"refresh\" content=\"4; URL=/\"><meta");                    // Fails Edge (asks for reload)
//    page.replace("</script>", "setTimeout(function(){window.location.reload(1);},4000);</script>");     // Repeats POST on All
    page.replace("</script>", "setTimeout(function(){window.location.replace(\"/\");},4000);</script>");  // OK on All
    page.replace("{v}", "Main menu");

    if (Maxdevice) {
      if (strlen(webServer->arg("o").c_str())) {
        do_cmnd_power(atoi(webServer->arg("o").c_str()), 2);
      }

      page += F("<table style='width:100%'><tr>");
      for (byte idx = 1; idx <= Maxdevice; idx++) {
        page += F("<td style='width:");
        page += String(100 / Maxdevice);
        page += F("%'><form action='/?o=");
        page += String(idx);
        page += F("' method='post'><div style='text-align:center;font-weight:bold;font-size:");
        page += String(70 - (Maxdevice * 8));
        page += F("px'>");
        page += (power & (0x01 << (idx -1))) ? "ON" : "OFF";
        page += F("</div><br/><button>Toggle");
        if (Maxdevice > 1) {
          page += F(" ");
          page += String(idx);
        }
        page += F("</button></form></td>");
      }
      page += F("</tr></table><br/>");
    }

    String tpage = "";
    if (hlw_flg) tpage += hlw_webPresent();
#ifdef USE_DS18B20
    if (pin[GPIO_DSB] < 99) tpage += dsb_webPresent();
#endif  // USE_DS18B20
#ifdef USE_DS18x20
    if (pin[GPIO_DSB] < 99) page += ds18x20_webPresent();
#endif  // USE_DS18x20
#if defined(USE_DHT) || defined(USE_DHT2)
    if (dht_type) tpage += dht_webPresent();
#endif  // USE_DHT/2
#ifdef USE_I2C
    if (i2c_flg) {
      tpage += htu_webPresent();
      tpage += bmp_webPresent();
      tpage += bh1750_webPresent();
    }
#endif  // USE_I2C    
    if (tpage.length() > 0) {
      page += F("<table style='width:100%'>");
      page += tpage;
      page += F("</table><br/>");
    }

    if (_httpflag == HTTP_ADMIN) {
      page += FPSTR(HTTP_BTN_MENU1);
      page += FPSTR(HTTP_BTN_RSTRT);
    }
    showPage(page);

#ifdef USE_DS18x20
    ds18x20_search();      // Check for changes in sensors number
    ds18x20_convert();     // Start Conversion, takes up to one second
#endif  // USE_DS18x20
  }
}

void handleConfig()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configuration");
  page += FPSTR(HTTP_BTN_MENU2);
  if (sysCfg.mqtt_enabled)  page += FPSTR(HTTP_BTN_MENU3);
  page += FPSTR(HTTP_BTN_MENU4);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleModule()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }

  char stemp[20];
  
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Module config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Config module");
  page += FPSTR(HTTP_FORM_MODULE);

  snprintf_P(stemp, sizeof(stemp), modules[MODULE].name);
  page.replace("{mt}", stemp);

  for (byte i = 0; i < MAXMODULE; i++) {
    page += F("<option ");
    if (i == sysCfg.module) page += F("selected ");
    snprintf_P(stemp, sizeof(stemp), modules[i].name);
    page += F("value='"); page += String(i); page += F("'>"); page += String(i +1); page += F(" "); page += stemp; page += F("</option>");
  }
  page += F("</select></br>");

  mytmplt cmodule;
  memcpy_P(&cmodule, &modules[sysCfg.module], sizeof(cmodule));
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (cmodule.gp.io[i] == GPIO_USER) {
      page += F("<br/><b>GPIO"); page += String(i); page += F("</b> <select id='g"); page += String(i); page += F("' name='g"); page += String(i); page += F("'>");
      byte k = 0;
      for (byte j = GPIO_SENSOR_START; j < GPIO_SENSOR_END; j++) {
        page += F("<option ");
        if (j == my_module.gp.io[i]) page += F("selected ");
        page += F("value='"); page += String(j); page += F("'>");
        page += String(j); page += F(" ");
        snprintf_P(stemp, sizeof(stemp), sensors[j]);
        page += stemp;  page += F("</option>");
      }
      page += F("</select></br>");
    }
  }

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  char log[LOGSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Wifi config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure Wifi");

  if (scan) {
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
    UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
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
        int quality = WIFI_getRSSIasQuality(WiFi.RSSI(indices[i]));

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
  if (!strcmp(WIFI_HOSTNAME, DEF_WIFI_HOSTNAME)) {
    snprintf_P(str, sizeof(str), PSTR(DEF_WIFI_HOSTNAME), sysCfg.mqtt_topic, ESP.getChipId() & 0x1FFF);
  } else {
    snprintf_P(str, sizeof(str), PSTR(WIFI_HOSTNAME));
  }
  page.replace("{h0}", str);
  page.replace("{h1}", String(sysCfg.hostname));
  page.replace("{s1}", String(sysCfg.sta_ssid[0]));
  page.replace("{p1}", String(sysCfg.sta_pwd[0]));
  page.replace("{s2}", String(sysCfg.sta_ssid[1]));
  page.replace("{p2}", String(sysCfg.sta_pwd[1]));
  page += FPSTR(HTTP_FORM_END);
  if (_httpflag == HTTP_MANAGER) {
    page += FPSTR(HTTP_BTN_RSTRT);
  } else {
    page += FPSTR(HTTP_BTN_CONF);
  }
  showPage(page);
}

void handleMqtt()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle MQTT config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure MQTT");
  page += FPSTR(HTTP_FORM_MQTT);
  char str[sizeof(sysCfg.mqtt_client)];
  getClient(str, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  page.replace("{m0}", str);
  page.replace("{m1}", String(sysCfg.mqtt_host));
  page.replace("{m2}", String(sysCfg.mqtt_port));
  page.replace("{m3}", String(sysCfg.mqtt_client));
  page.replace("{m4}", String(sysCfg.mqtt_user));
  page.replace("{m5}", String(sysCfg.mqtt_pwd));
  page.replace("{m6}", String(sysCfg.mqtt_topic));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

#ifdef USE_DOMOTICZ
void handleDomoticz()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Domoticz config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure Domoticz");
  page += FPSTR(HTTP_FORM_DOMOTICZ);
  page.replace("{d1}", String(sysCfg.domoticz_in_topic));
  page.replace("{d2}", String(sysCfg.domoticz_out_topic));
  page.replace("{d3}", String((int)sysCfg.domoticz_relay_idx[0]));
  if (Maxdevice > 1) {
    page += F("<br/><b>Idx 2</b> (" STR(DOMOTICZ_RELAY_IDX2) ")<br/><input id='iy' name='iy' length=8 placeholder='" STR(DOMOTICZ_RELAY_IDX2) "' value='{d4}'><br/>");
    page.replace("{d4}", String((int)sysCfg.domoticz_relay_idx[1]));
  }
  if (Maxdevice > 2) {
    page += F("<br/><b>Idx 3</b> (" STR(DOMOTICZ_RELAY_IDX3) ")<br/><input id='iz' name='iz' length=8 placeholder='" STR(DOMOTICZ_RELAY_IDX3) "' value='{d5}'><br/>");
    page.replace("{d5}", String((int)sysCfg.domoticz_relay_idx[2]));
  }
  if (Maxdevice > 3) {
    page += F("<br/><b>Idx 4</b> (" STR(DOMOTICZ_RELAY_IDX4) ")<br/><input id='iw' name='iw' length=8 placeholder='" STR(DOMOTICZ_RELAY_IDX4) "' value='{d6}'><br/>");
    page.replace("{d6}", String((int)sysCfg.domoticz_relay_idx[3]));
  }
  page += FPSTR(HTTP_FORM_DOMOTICZ2);
  page.replace("{d7}", String((int)sysCfg.domoticz_update_timer));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}
#endif  // USE_DOMOTICZ

void handleLog()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Log config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Config logging");
  page += FPSTR(HTTP_FORM_LOG1);
  for (byte idx = 0; idx < 3; idx++) {
    page += FPSTR(HTTP_FORM_LOG2);
    switch (idx) {
    case 0:
      page.replace("{b0}", "Serial log");
      page.replace("{b1}", STR(SERIAL_LOG_LEVEL));
      page.replace("{b2}", "ls");
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.seriallog_level) ? " selected " : " ");
      }
      break;
    case 1:
      page.replace("{b0}", "Web log");
      page.replace("{b1}", STR(WEB_LOG_LEVEL));
      page.replace("{b2}", "lw");
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.weblog_level) ? " selected " : " ");
      }
      break;
    case 2:
      page.replace("{b0}", "Syslog");
      page.replace("{b1}", STR(SYS_LOG_LEVEL));
      page.replace("{b2}", "ll");
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.syslog_level) ? " selected " : " ");
      }
      break;
    }
  }
  page += FPSTR(HTTP_FORM_LOG3);
  page.replace("{l2}", String(sysCfg.syslog_host));
  page.replace("{l3}", String(sysCfg.syslog_port));
  page.replace("{l4}", String(sysCfg.tele_period));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleOther()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle other config"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure Other");
  page += FPSTR(HTTP_FORM_OTHER);
  page.replace("{r1}", (sysCfg.mqtt_enabled) ? " checked" : "");
  for (byte i = 0; i < Maxdevice; i++) {
    page += F("<br/><b>Friendly Name ");
    page += i +1;
    page += F("</b> ({fn})<br/><input id='{id}' name='{id}' length=32 placeholder='{fn}' value='{f1}'><br/>");
    if (i == 0) {
      page.replace("{fn}", FRIENDLY_NAME1);
      page.replace("{id}", "a1");
    }
    if (i == 1) {
      page.replace("{fn}", FRIENDLY_NAME2);
      page.replace("{id}", "a2");
    }
    else if (i == 2) {
      page.replace("{fn}", FRIENDLY_NAME3);
      page.replace("{id}", "a3");
    }
    else if (i == 3) {
      page.replace("{fn}", FRIENDLY_NAME4);
      page.replace("{id}", "a4");
    }
    page.replace("{f1}", String(sysCfg.friendlyname[i]));
  }
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleSave()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  char log[LOGSZ], stemp[20];
  byte what = 0, restart;
  String result = "";

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Parameter save"));

  if (strlen(webServer->arg("w").c_str())) what = atoi(webServer->arg("w").c_str());
  switch (what) {
  case 1:
    strlcpy(sysCfg.hostname, (!strlen(webServer->arg("h").c_str())) ? WIFI_HOSTNAME : webServer->arg("h").c_str(), sizeof(sysCfg.hostname));
    if (strstr(sysCfg.hostname,"%")) strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
    strlcpy(sysCfg.sta_ssid[0], (!strlen(webServer->arg("s1").c_str())) ? STA_SSID1 : webServer->arg("s1").c_str(), sizeof(sysCfg.sta_ssid[0]));
    strlcpy(sysCfg.sta_pwd[0], (!strlen(webServer->arg("p1").c_str())) ? STA_PASS1 : webServer->arg("p1").c_str(), sizeof(sysCfg.sta_pwd[0]));
    strlcpy(sysCfg.sta_ssid[1], (!strlen(webServer->arg("s2").c_str())) ? STA_SSID2 : webServer->arg("s2").c_str(), sizeof(sysCfg.sta_ssid[1]));
    strlcpy(sysCfg.sta_pwd[1], (!strlen(webServer->arg("p2").c_str())) ? STA_PASS2 : webServer->arg("p2").c_str(), sizeof(sysCfg.sta_pwd[1]));
    snprintf_P(log, sizeof(log), PSTR("HTTP: Wifi Hostname %s, SSID1 %s, Password1 %s, SSID2 %s, Password2 %s"),
      sysCfg.hostname, sysCfg.sta_ssid[0], sysCfg.sta_pwd[0], sysCfg.sta_ssid[1], sysCfg.sta_pwd[1]);
    addLog(LOG_LEVEL_INFO, log);
    result += F("<br/>Trying to connect device to network<br/>If it fails reconnect to try again");
    break;
  case 2:
    strlcpy(sysCfg.mqtt_host, (!strlen(webServer->arg("mh").c_str())) ? MQTT_HOST : webServer->arg("mh").c_str(), sizeof(sysCfg.mqtt_host));
    sysCfg.mqtt_port = (!strlen(webServer->arg("ml").c_str())) ? MQTT_PORT : atoi(webServer->arg("ml").c_str());
    strlcpy(sysCfg.mqtt_client, (!strlen(webServer->arg("mc").c_str())) ? MQTT_CLIENT_ID : webServer->arg("mc").c_str(), sizeof(sysCfg.mqtt_client));
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
    syslog_level = sysCfg.syslog_level;
    syslog_timer = 0;
    strlcpy(sysCfg.syslog_host, (!strlen(webServer->arg("lh").c_str())) ? SYS_LOG_HOST : webServer->arg("lh").c_str(), sizeof(sysCfg.syslog_host));
    sysCfg.syslog_port = (!strlen(webServer->arg("lp").c_str())) ? SYS_LOG_PORT : atoi(webServer->arg("lp").c_str());
    sysCfg.tele_period = (!strlen(webServer->arg("lt").c_str())) ? TELE_PERIOD : atoi(webServer->arg("lt").c_str());
    snprintf_P(log, sizeof(log), PSTR("HTTP: Logging Seriallog %d, Weblog %d, Syslog %d, Host %s, Port %d, TelePeriod %d"),
      sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.syslog_port, sysCfg.tele_period);
    addLog(LOG_LEVEL_INFO, log);
    break;
#ifdef USE_DOMOTICZ
  case 4:
    strlcpy(sysCfg.domoticz_in_topic, (!strlen(webServer->arg("it").c_str())) ? DOMOTICZ_IN_TOPIC : webServer->arg("it").c_str(), sizeof(sysCfg.domoticz_in_topic));
    strlcpy(sysCfg.domoticz_out_topic, (!strlen(webServer->arg("ot").c_str())) ? DOMOTICZ_OUT_TOPIC : webServer->arg("ot").c_str(), sizeof(sysCfg.domoticz_out_topic));
    sysCfg.domoticz_relay_idx[0] = (!strlen(webServer->arg("ix").c_str())) ? DOMOTICZ_RELAY_IDX1 : atoi(webServer->arg("ix").c_str());
    sysCfg.domoticz_relay_idx[1] = (!strlen(webServer->arg("iy").c_str())) ? DOMOTICZ_RELAY_IDX2 : atoi(webServer->arg("iy").c_str());
    sysCfg.domoticz_relay_idx[2] = (!strlen(webServer->arg("iz").c_str())) ? DOMOTICZ_RELAY_IDX3 : atoi(webServer->arg("iz").c_str());
    sysCfg.domoticz_relay_idx[3] = (!strlen(webServer->arg("iw").c_str())) ? DOMOTICZ_RELAY_IDX4 : atoi(webServer->arg("iw").c_str());
    sysCfg.domoticz_update_timer = (!strlen(webServer->arg("ut").c_str())) ? DOMOTICZ_UPDATE_TIMER : atoi(webServer->arg("ut").c_str());
    snprintf_P(log, sizeof(log), PSTR("HTTP: Domoticz in_topic %s, out_topic %s, idx1 %d, idx2 %d, idx3 %d, idx4 %d, update_timer %d"),
      sysCfg.domoticz_in_topic, sysCfg.domoticz_out_topic, sysCfg.domoticz_relay_idx[0], sysCfg.domoticz_relay_idx[1],
      sysCfg.domoticz_relay_idx[2], sysCfg.domoticz_relay_idx[3], sysCfg.domoticz_update_timer);
    addLog(LOG_LEVEL_INFO, log);
    break;
#endif  // USE_DOMOTICZ
  case 5:
    sysCfg.mqtt_enabled = webServer->hasArg("r1");
    strlcpy(sysCfg.friendlyname[0], (!strlen(webServer->arg("a1").c_str())) ? FRIENDLY_NAME1 : webServer->arg("a1").c_str(), sizeof(sysCfg.friendlyname[0]));
    strlcpy(sysCfg.friendlyname[1], (!strlen(webServer->arg("a2").c_str())) ? FRIENDLY_NAME2 : webServer->arg("a2").c_str(), sizeof(sysCfg.friendlyname[1]));
    strlcpy(sysCfg.friendlyname[2], (!strlen(webServer->arg("a3").c_str())) ? FRIENDLY_NAME3 : webServer->arg("a3").c_str(), sizeof(sysCfg.friendlyname[2]));
    strlcpy(sysCfg.friendlyname[3], (!strlen(webServer->arg("a4").c_str())) ? FRIENDLY_NAME4 : webServer->arg("a4").c_str(), sizeof(sysCfg.friendlyname[3]));
    snprintf_P(log, sizeof(log), PSTR("HTTP: Other MQTT Enable %s, Friendly Names %s, %s, %s and %s"),
      (sysCfg.mqtt_enabled) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sysCfg.friendlyname[0], sysCfg.friendlyname[1], sysCfg.friendlyname[2], sysCfg.friendlyname[3]);
    addLog(LOG_LEVEL_INFO, log);
    break;
  case 6:
    sysCfg.module = (!strlen(webServer->arg("mt").c_str())) ? MODULE : atoi(webServer->arg("mt").c_str());
    mytmplt cmodule;
    memcpy_P(&cmodule, &modules[sysCfg.module], sizeof(cmodule));
    String gpios = "";
    for (byte i = 0; i < MAX_GPIO_PIN; i++) {
      if (cmodule.gp.io[i] == GPIO_USER) {
        snprintf_P(stemp, sizeof(stemp), PSTR("g%d"), i);
        sysCfg.my_module.gp.io[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
        gpios += F(", GPIO"); gpios += String(i); gpios += F(" "); gpios += String(sysCfg.my_module.gp.io[i]);
      }
    }
    snprintf_P(stemp, sizeof(stemp), modules[sysCfg.module].name);
    snprintf_P(log, sizeof(log), PSTR("HTTP: %s Module%s"), stemp, gpios.c_str());
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
    if (_httpflag == HTTP_MANAGER) {
      _httpflag = HTTP_ADMIN;
    } else {
      page += FPSTR(HTTP_BTN_MAIN);
    }
    showPage(page);

    restartflag = 2;
  } else {
    handleConfig();
  }
}

void handleReset()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Firmware upload done"));
  WIFI_configCounter();
  restartflag = 0;
  mqttcounter = 0;

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += F("<div style='text-align:center;'><b>Upload ");
  if (_uploaderror) {
    page += F("<font color='red'>failed</font></b>");
    if (_uploaderror == 1) {
      page += F("<br/><br/>No file selected");
    } else if (_uploaderror == 2) {
      page += F("<br/><br/>File size is larger than available free space");
    } else if (_uploaderror == 3) {
      page += F("<br/><br/>File magic header does not start with 0xE9");
    } else if (_uploaderror == 4) {
      page += F("<br/><br/>File flash size is larger than device flash size");
    } else if (_uploaderror == 5) {
      page += F("<br/><br/>File upload buffer miscompare");
    } else if (_uploaderror == 6) {
      page += F("<br/><br/>Upload failed. Enable logging option 3 for more information");
    } else if (_uploaderror == 7) {
      page += F("<br/><br/>Upload aborted");
    } else {
      page += F("<br/><br/>Upload error code ");
      page += String(_uploaderror);
    }
    if (Update.hasError()) {
      page += F("<br/><br/>Update error code (see Updater.cpp) ");
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
  boolean _serialoutput = (LOG_LEVEL_DEBUG <= seriallog_level);

  if (_httpflag == HTTP_USER) return;
  if (_uploaderror) {
    Update.end();
    return;
  }

  HTTPUpload& upload = webServer->upload();

  if (upload.status == UPLOAD_FILE_START) {
    restartflag = 60;
    mqttcounter = 60;
    if (upload.filename.c_str()[0] == 0)
    {
      _uploaderror = 1;
      return;
    }
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
    UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
    if (sysCfg.mqtt_enabled) mqttClient.disconnect();

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
      if ((sysCfg.module == SONOFF_TOUCH) || (sysCfg.module == SONOFF_4CH)) {
        upload.buf[2] = 3; // DOUT - ESP8285
      } else {
        upload.buf[2] = 2; // DIO - ESP8266
      }
//      snprintf_P(log, sizeof(log), PSTR("Upload: Flash Chip Mode %02X"), upload.buf[2]);
//      addLog(LOG_LEVEL_DEBUG, log);
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
    restartflag = 0;
    mqttcounter = 0;
    _uploaderror = 7;
    Update.end();
  }
  delay(0);
}

void handleCmnd()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle cmnd"));

  byte curridx = logidx;
  if (strlen(webServer->arg(0).c_str())) {
    snprintf_P(svalue, sizeof(svalue), webServer->arg(0).c_str());
    do_cmnd(svalue);
  }

  String message = "";
  if (logidx != curridx) {
    byte counter = curridx;
    do {
      if (Log[counter].length()) {
        if (message.length()) message += F("\n");
        if (sysCfg.mqtt_enabled) {
          // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
//          message += Log[counter].substring(17 + strlen(PUB_PREFIX) + strlen(sysCfg.mqtt_topic));
          message += Log[counter].substring(Log[counter].lastIndexOf("/",Log[counter].indexOf("="))+1);
        } else {
          // [14:49:36 RSLT: RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
          message += Log[counter].substring(Log[counter].indexOf(": ")+2);
        }
      }
      counter++;
      if (counter > MAX_LOG_LINES -1) counter = 0;
    } while (counter != logidx);
  } else {
    message = F("Enable weblog 2 if response expected\n");
  }

  webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer->sendHeader("Pragma", "no-cache");
  webServer->sendHeader("Expires", "-1");
  webServer->send(200, "text/plain", message);
}

void handleConsole()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  char svalue[MESSZ];

  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle console"));

  if (strlen(webServer->arg("c1").c_str())) {
    snprintf_P(svalue, sizeof(svalue), webServer->arg("c1").c_str());
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  String message = "";
  uint16_t size = 0;

  int maxSize = ESP.getFreeHeap() - 6000;

  byte counter = logidx;
  do {
    counter--;
    if (counter == 255) counter = MAX_LOG_LINES -1;
    size += Log[counter].length();
  } while ((counter != logidx) && (size < maxSize));
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
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle info"));

  int freeMem = ESP.getFreeHeap();

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Information");
//  page += F("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>");
  page += F("<style>td{padding:0px 5px;}</style>");
  page += F("<table style'width:100%;'>");
  page += F("<tr><td><b>Program version</b></td><td>"); page += Version; page += F("</td></tr>");
  page += F("<tr><td><b>Core/SDK version</b></td><td>"); page += ESP.getCoreVersion(); page += F("/"); page += String(ESP.getSdkVersion()); page += F("</td></tr>");
//  page += F("<tr><td><b>Boot version</b></td><td>"); page += String(ESP.getBootVersion()); page += F("</td></tr>");
  page += F("<tr><td><b>Uptime</b></td><td>"); page += String(uptime); page += F(" Hours</td></tr>");
  page += F("<tr><td><b>Flash write count</b></td><td>"); page += String(sysCfg.saveFlag); page += F("</td></tr>");
  page += F("<tr><td><b>Boot count</b></td><td>"); page += String(sysCfg.bootcount); page += F("</td></tr>");
  page += F("<tr><td><b>Reset reason</b></td><td>"); page += ESP.getResetReason(); page += F("</td></tr>");
  for (byte i = 0; i < Maxdevice; i++) {
    page += F("<tr><td><b>Friendly name ");
    page += i +1;
    page += F("</b></td><td>"); page += String(sysCfg.friendlyname[i]); page += F("</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
//  page += F("<tr><td><b>SSId (RSSI)</b></td><td>"); page += (sysCfg.sta_active)? sysCfg.sta_ssid2 : sysCfg.sta_ssid1; page += F(" ("); page += WIFI_getRSSIasQuality(WiFi.RSSI()); page += F("%)</td></tr>");
  page += F("<tr><td><b>AP"); page += String(sysCfg.sta_active +1); page += F(" SSId (RSSI)</b></td><td>"); page += sysCfg.sta_ssid[sysCfg.sta_active]; page += F(" ("); page += WIFI_getRSSIasQuality(WiFi.RSSI()); page += F("%)</td></tr>");
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
  if (sysCfg.mqtt_enabled) {
    page += F("<tr><td><b>MQTT Host</b></td><td>"); page += sysCfg.mqtt_host; page += F("</td></tr>");
    page += F("<tr><td><b>MQTT Port</b></td><td>"); page += String(sysCfg.mqtt_port); page += F("</td></tr>");
    page += F("<tr><td><b>MQTT Client and<br/>&nbsp;Fallback Topic</b></td><td>"); page += MQTTClient; page += F("</td></tr>");
    page += F("<tr><td><b>MQTT User</b></td><td>"); page += sysCfg.mqtt_user; page += F("</td></tr>");
//    page += F("<tr><td><b>MQTT Password</b></td><td>"); page += sysCfg.mqtt_pwd; page += F("</td></tr>");
    page += F("<tr><td><b>MQTT Topic</b></td><td>"); page += sysCfg.mqtt_topic; page += F("</td></tr>");
    page += F("<tr><td><b>MQTT Group Topic</b></td><td>"); page += sysCfg.mqtt_grptopic; page += F("</td></tr>");
  } else {
    page += F("<tr><td><b>MQTT</b></td><td>Disabled</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><td><b>ESP Chip id</b></td><td>"); page += String(ESP.getChipId()); page += F("</td></tr>");
  page += F("<tr><td><b>Flash Chip id</b></td><td>"); page += String(ESP.getFlashChipId()); page += F("</td></tr>");
  page += F("<tr><td><b>Flash size</b></td><td>"); page += String(ESP.getFlashChipRealSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Program flash size</b></td><td>"); page += String(ESP.getFlashChipSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Program size</b></td><td>"); page += String(ESP.getSketchSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Free program space</b></td><td>"); page += String(ESP.getFreeSketchSpace() / 1024); page += F("kB</td></tr>");
  page += F("<tr><td><b>Free memory</b></td><td>"); page += String(freeMem / 1024); page += F("kB</td></tr>");
  page += F("</table>");
//  page += F("</fieldset>");
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleRestart()
{
  if (_httpflag == HTTP_USER) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Restarting"));

  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_MSG_RSTRT);
  if (_httpflag == HTTP_MANAGER) {
    _httpflag = HTTP_ADMIN;
  } else {
    page += FPSTR(HTTP_BTN_MAIN);
  }
  showPage(page);

  restartflag = 2;
}

/********************************************************************************************/

#ifdef USE_WEMO_EMULATION
void handleUPnPevent()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo basic event"));

  String request = webServer->arg(0);
  if(request.indexOf("State>1</Binary") > 0) do_cmnd_power(1, 1);
  if(request.indexOf("State>0</Binary") > 0) do_cmnd_power(1, 0);
  webServer->send(200, "text/plain", "");
}

void handleUPnPservice()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo event service"));

  String eventservice_xml = FPSTR(WEMO_EVENTSERVICE_XML);
  webServer->send(200, "text/plain", eventservice_xml);
}

void handleUPnPsetup()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo setup"));

  String setup_xml = FPSTR(WEMO_SETUP_XML);
//  setup_xml.replace("{x1}", String(MQTTClient));
  setup_xml.replace("{x1}", String(sysCfg.friendlyname[0]));
  setup_xml.replace("{x2}", wemo_UUID());
  setup_xml.replace("{x3}", wemo_serial());
  webServer->send(200, "text/xml", setup_xml);
}
#endif  // USE_WEMO_EMULATION

/********************************************************************************************/

#ifdef USE_HUE_EMULATION
String hue_deviceId(uint8_t id)
{
  char deviceid[16];
  snprintf_P(deviceid, sizeof(deviceid), PSTR("5CCF7F%03X-%0d"), ESP.getChipId(), id);
  return String(deviceid);
}

void handleUPnPsetup()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Hue Bridge setup"));

  String description_xml = FPSTR(HUE_DESCRIPTION_XML);
  description_xml.replace("{x1}", WiFi.localIP().toString());
  description_xml.replace("{x2}", hue_UUID());
  webServer->send(200, "text/xml", description_xml);
}

void handle_hue_api(String path)
{
  /* HUE API uses /api/<userid>/<command> syntax. The userid is created by the echo device and
   * on original HUE the pressed button allows for creation of this user. We simply ignore the
   * user part and allow every caller as with Web or WeMo. */
   
  char log[LOGSZ];
  String response;
  String command=path;
  uint8_t device=1;
  char id[4];

  command.remove(0, command.indexOf("/lights") +7); // remove all including lights cmd
  if (path.startsWith("/api/invalid")) {}           // Ignore /api/invalid
  else if (command.length() == 0) {                 // only /lights requested
//    Serial.println("HUE: /lights");
    response = "{\"";
    for (uint8_t i = 1; i <= Maxdevice; i++) {
      response += i;
      response += "\":";
      response += FPSTR(HUE_LIGHT_STATUS_JSON);
      if (i < Maxdevice) response += ",\"";
      response.replace("{state}", (power & (0x01 << (i-1))) ? "true" : "false");
      response.replace("{j1}", sysCfg.friendlyname[i-1]);
      response.replace("{j2}", hue_deviceId(i));  
    }
    response += "}";
    webServer->send(200, "application/json", response);    
  }
  else if (command.length() <= 3) {                // Only device ID (up to 63 on real Bridge)
    device = atoi(command.c_str() +1);             // Skip leading '/'
    if ((device < 1) || (device > Maxdevice)) device = 1;
    response = FPSTR(HUE_LIGHT_STATUS_JSON);
    response.replace("{state}", (power & (0x01 << (device -1))) ? "true" : "false");
    response.replace("{j1}", sysCfg.friendlyname[device -1]);
    response.replace("{j2}", hue_deviceId(device));
//    Serial.print("HUE: Get state of device "); Serial.println(device);
    webServer->send(200, "application/json", response);
  }
  else if (command.endsWith("/state")) {          // Got ID/state
//    Serial.println("HUE: Handle API /state");
    command.remove(command.lastIndexOf("/state"));
    device = atoi(command.c_str() +1);
    if ((device < 1) || (device > Maxdevice)) device = 1;
    response = "{\"success\":{\"/lights/";
    response += device;
    if (webServer->args() == 1) {
      String json = webServer->arg(0);
      Serial.println(json.c_str());
      if (json.startsWith("{\"on\": false")) {
        do_cmnd_power(device, 0);
        response +="/state/on\": false}}";
      }
      else {
        do_cmnd_power(device, 1);
        response +="/state/on\": true}}";        
      }
      webServer->send(200, "application/json", response);
    }
    else  {
//      Serial.println("HUE: /state no POST args");
      webServer->send(406, "application/json", "{}");
    }
  }
  else {
    snprintf_P(log, sizeof(log), PSTR("HTTP: Handle Hue API (%s)"),path.c_str());
    addLog(LOG_LEVEL_DEBUG_MORE, log);
    webServer->send(406, "application/json", "{}");
  }
}
#endif  // USE_HUE_EMULATION

/********************************************************************************************/

void handleNotFound()
{
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }

#ifdef USE_HUE_EMULATION  
  String path = webServer->uri();
  if (path.startsWith("/api"))
    handle_hue_api(path);
  else {
#endif // USE_HUE_EMULATION
  
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
#ifdef USE_HUE_EMULATION
  addLog_P(LOG_LEVEL_DEBUG_MORE, message.c_str());
  }
#endif // USE_HUE_EMULATION
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

/** Is this an IP? */
boolean isIp(String str)
{
  for (uint16_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

#endif  // USE_WEBSERVER
