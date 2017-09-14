/*
  webserver.ino - webserver for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_WEBSERVER
/*********************************************************************************************\
 * Web server and WiFi Manager
 *
 * Enables configuration and reconfiguration of WiFi credentials using a Captive Portal
 * Based on source by AlexT (https://github.com/tzapu)
\*********************************************************************************************/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

const char HTTP_HEAD[] PROGMEM =
  "<!DOCTYPE html><html lang=\"en\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<title>{v}</title>"

  "<script>"
  "var cn,x,lt;"
  "cn=120;"
  "x=null;"                  // Allow for abortion
  "function u(){"
    "if(cn>=0){"
      "document.getElementById('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "function c(l){"
    "document.getElementById('s1').value=l.innerText||l.textContent;"
    "document.getElementById('p1').focus();"
  "}"
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"    // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "document.getElementById('l1').innerHTML=x.responseText;"
      "}"
    "};"
    "x.open('GET','ay'+a,true);"
    "x.send();"
    "lt=setTimeout(la,2345);"
  "}"
  "function lb(p){"
    "la('?d='+p);"
  "}"
  "function lc(p){"
    "la('?t='+p);"
  "}"
  "</script>"

  "<style>"
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "input{width:95%;}"
  "select{width:100%;}"
  "textarea{resize:none;width:98%;height:318px;padding:5px;overflow:auto;}"
  "body{text-align:center;font-family:verdana;}"
  "td{padding:0px;}"
  "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;}"
  "button:hover{background-color:#006cba;}"
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  "</style>"

  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;min-width:340px;'>"
  "<div style='text-align:center;'><h3>{ha} " D_MODULE "</h3><h2>{h}</h2></div>";
const char HTTP_SCRIPT_CONSOL[] PROGMEM =
  "var sn=0;"                    // Scroll position
  "var id=99;"                   // Get most of weblog initially
  "function l(p){"               // Console log and command service
    "var c,o,t;"
    "clearTimeout(lt);"
    "o='';"
    "t=document.getElementById('t1');"
    "if(p==1){"
      "c=document.getElementById('c1');"
      "o='&c1='+encodeURIComponent(c.value);"
      "c.value='';"
      "t.scrollTop=sn;"
    "}"
    "if(t.scrollTop>=sn){"       // User scrolled back so no updates
      "if(x!=null){x.abort();}"  // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var z,d;"
          "d=x.responseXML;"
          "id=d.getElementsByTagName('i')[0].childNodes[0].nodeValue;"
          "if(d.getElementsByTagName('j')[0].childNodes[0].nodeValue==0){t.value='';}"
          "z=d.getElementsByTagName('l')[0].childNodes;"
          "if(z.length>0){t.value+=z[0].nodeValue;}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
        "}"
      "};"
      "x.open('GET','ax?c2='+id+o,true);"
      "x.send();"
    "}"
    "lt=setTimeout(l,2345);"
    "return false;"
  "}"
  "</script>";
const char HTTP_SCRIPT_MODULE1[] PROGMEM =
  "var os;"
  "function sk(s,g){"
    "var o=os.replace(\"value='\"+s+\"'\",\"selected value='\"+s+\"'\");"
    "document.getElementById('g'+g).innerHTML=o;"
  "}"
  "function sl(){"
    "var o0=\"";
const char HTTP_SCRIPT_MODULE2[] PROGMEM =
    "}1'%d'>%02d %s}2";     // "}1" and "}2" means do not use "}" in Module name and Sensor name
const char HTTP_SCRIPT_MODULE3[] PROGMEM =
    "\";"
    "os=o0.replace(/}1/g,\"<option value=\").replace(/}2/g,\"</option>\");";
const char HTTP_MSG_SLIDER1[] PROGMEM =
  "<div><span class='p'>" D_COLDLIGHT "</span><span class='q'>" D_WARMLIGHT "</span></div>"
  "<div><input type='range' min='153' max='500' value='%d' onchange='lc(value)'></div>";
const char HTTP_MSG_SLIDER2[] PROGMEM =
  "<div><span class='p'>" D_DARKLIGHT "</span><span class='q'>" D_BRIGHTLIGHT "</span></div>"
  "<div><input type='range' min='1' max='100' value='%d' onchange='lb(value)'></div>";
const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br/><div style='text-align:center;'>" D_DEVICE_WILL_RESTART "</div><br/>";
const char HTTP_BTN_MENU1[] PROGMEM =
  "<br/><form action='cn' method='get'><button>" D_CONFIGURATION "</button></form>"
  "<br/><form action='in' method='get'><button>" D_INFORMATION "</button></form>"
  "<br/><form action='up' method='get'><button>" D_FIRMWARE_UPGRADE "</button></form>"
  "<br/><form action='cs' method='get'><button>" D_CONSOLE "</button></form>";
const char HTTP_BTN_RSTRT[] PROGMEM =
  "<br/><form action='rb' method='get' onsubmit='return confirm(\"" D_CONFIRM_RESTART "\");'><button>" D_RESTART "</button></form>";
const char HTTP_BTN_MENU2[] PROGMEM =
  "<br/><form action='md' method='get'><button>" D_CONFIGURE_MODULE "</button></form>"
  "<br/><form action='w0' method='get'><button>" D_CONFIGURE_WIFI "</button></form>";
const char HTTP_BTN_MENU3[] PROGMEM =
  "<br/><form action='mq' method='get'><button>" D_CONFIGURE_MQTT "</button></form>"
#ifdef USE_DOMOTICZ
  "<br/><form action='dm' method='get'><button>" D_CONFIGURE_DOMOTICZ "</button></form>"
#endif  // USE_DOMOTICZ
  "";
const char HTTP_BTN_MENU4[] PROGMEM =
  "<br/><form action='lg' method='get'><button>" D_CONFIGURE_LOGGING "</button></form>"
  "<br/><form action='co' method='get'><button>" D_CONFIGURE_OTHER "</button></form>"
  "<br/><form action='rt' method='get' onsubmit='return confirm(\"" D_CONFIRM_RESET_CONFIGURATION "\");'><button>" D_RESET_CONFIGURATION "</button></form>"
  "<br/><form action='dl' method='get'><button>" D_BACKUP_CONFIGURATION "</button></form>"
  "<br/><form action='rs' method='get'><button>" D_RESTORE_CONFIGURATION "</button></form>";
const char HTTP_BTN_MAIN[] PROGMEM =
  "<br/><br/><form action='.' method='get'><button>" D_MAIN_MENU "</button></form>";
const char HTTP_BTN_CONF[] PROGMEM =
  "<br/><br/><form action='cn' method='get'><button>" D_CONFIGURATION "</button></form>";
const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='6' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_MODULE_TYPE "</b> ({mt})<br/><select id='g99' name='g99'></select></br>";
const char HTTP_LNK_ITEM[] PROGMEM =
  "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q'>{i} {r}%</span></div>";
const char HTTP_LNK_SCAN[] PROGMEM =
  "<div><a href='/w1'>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br/>";
const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='1' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br/><input id='s1' name='s1' length=32 placeholder='" STA_SSID1 "' value='{s1}'><br/>"
  "<br/><b>" D_AP1_PASSWORD "</b></br><input id='p1' name='p1' length=64 type='password' placeholder='" STA_PASS1 "' value='{p1}'><br/>"
  "<br/><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br/><input id='s2' name='s2' length=32 placeholder='" STA_SSID2 "' value='{s2}'><br/>"
  "<br/><b>" D_AP2_PASSWORD "</b></br><input id='p2' name='p2' length=64 type='password' placeholder='" STA_PASS2 "' value='{p2}'><br/>"
  "<br/><b>" D_HOSTNAME "</b> (" WIFI_HOSTNAME ")<br/><input id='h' name='h' length=32 placeholder='" WIFI_HOSTNAME" ' value='{h1}'><br/>";
const char HTTP_FORM_MQTT[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MQTT_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='2' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_HOST "</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' length=32 placeholder='" MQTT_HOST" ' value='{m1}'><br/>"
  "<br/><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br/><input id='ml' name='ml' length=5 placeholder='" STR(MQTT_PORT) "' value='{m2}'><br/>"
  "<br/><b>" D_CLIENT "</b> ({m0})<br/><input id='mc' name='mc' length=32 placeholder='" MQTT_CLIENT_ID "' value='{m3}'><br/>"
  "<br/><b>" D_USER "</b> (" MQTT_USER ")<br/><input id='mu' name='mu' length=32 placeholder='" MQTT_USER "' value='{m4}'><br/>"
  "<br/><b>" D_PASSWORD "</b><br/><input id='mp' name='mp' length=32 type='password' placeholder='" MQTT_PASS "' value='{m5}'><br/>"
  "<br/><b>" D_TOPIC "</b> = %topic% (" MQTT_TOPIC ")<br/><input id='mt' name='mt' length=32 placeholder='" MQTT_TOPIC" ' value='{m6}'><br/>"
  "<br/><b>" D_FULL_TOPIC "</b> (" MQTT_FULLTOPIC ")<br/><input id='mf' name='mf' length=80 placeholder='" MQTT_FULLTOPIC" ' value='{m7}'><br/>";
const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<br/><b>{b0}" D_LOG_LEVEL "</b> ({b1})<br/><select id='{b2}' name='{b2}'>"
  "<option{a0value='0'>0 " D_NONE "</option>"
  "<option{a1value='1'>1 " D_ERROR "</option>"
  "<option{a2value='2'>2 " D_INFO "</option>"
  "<option{a3value='3'>3 " D_DEBUG "</option>"
  "<option{a4value='4'>4 " D_MORE_DEBUG "</option>"
  "</select></br>";
const char HTTP_FORM_LOG3[] PROGMEM =
  "<br/><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' length=32 placeholder='" SYS_LOG_HOST "' value='{l2}'><br/>"
  "<br/><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' length=5 placeholder='" STR(SYS_LOG_PORT) "' value='{l3}'><br/>"
  "<br/><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' length=4 placeholder='" STR(TELE_PERIOD) "' value='{l4}'><br/>";
const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='5' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_WEB_ADMIN_PASSWORD "</b><br/><input id='p1' name='p1' length=32 type='password' placeholder='" WEB_PASSWORD "' value='{p1}'><br/>"
  "<br/><input style='width:10%;float:left' id='b1' name='b1' type='checkbox'{r1}><b>" D_MQTT_ENABLE "</b><br/>";
const char HTTP_FORM_OTHER2[] PROGMEM =
  "<br/><b>" D_FRIENDLY_NAME " {1</b> ({2)<br/><input id='a{1' name='a{1' length=32 placeholder='{2' value='{3'><br/>";
#ifdef USE_EMULATION
const char HTTP_FORM_OTHER3a[] PROGMEM =
  "<br/><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend>";
const char HTTP_FORM_OTHER3b[] PROGMEM =
  "<br/><input style='width:10%;float:left' id='b2' name='b2' type='radio' value='{1'{2><b>{3</b>{4";
#endif  // USE_EMULATION
const char HTTP_FORM_END[] PROGMEM =
  "<br/><button type='submit'>" D_SAVE "</button></form></fieldset>";
const char HTTP_FORM_RST[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_RESTORE_CONFIGURATION "&nbsp;</b></legend>";
const char HTTP_FORM_UPG[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
  "<form method='get' action='u1'>"
  "<br/>" D_OTA_URL "<br/><input id='o' name='o' length=80 placeholder='OTA_URL' value='{o1}'><br/>"
  "<br/><button type='submit'>" D_START_UPGRADE "</button></form>"
  "</fieldset><br/><br/>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
const char HTTP_FORM_RST_UPG[] PROGMEM =
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br/><input type='file' name='u2'><br/>"
  "<br/><button type='submit' onclick='document.getElementById(\"f1\").style.display=\"none\";document.getElementById(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " {r1}</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char HTTP_FORM_CMND[] PROGMEM =
  "<br/><textarea readonly id='t1' name='t1' cols='" STR(MESSZ) "' wrap='off'></textarea><br/><br/>"
  "<form method='get' onsubmit='return l(1);'>"
  "<input style='width:98%' id='c1' name='c1' length='99' placeholder='" D_ENTER_COMMAND "' autofocus><br/>"
//  "<br/><button type='submit'>Send command</button>"
  "</form>";
const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%'>";
const char HTTP_COUNTER[] PROGMEM =
  "<br/><div id='t' name='t' style='text-align:center;'></div>";
const char HTTP_SNS_TEMP[] PROGMEM =
  "<tr><th>%s " D_TEMPERATURE "</th><td>%s&deg;%c</td></tr>";
const char HTTP_SNS_HUM[] PROGMEM =
  "<tr><th>%s " D_HUMIDITY "</th><td>%s%</td></tr>";
const char HTTP_SNS_PRESSURE[] PROGMEM =
  "<tr><th>%s " D_PRESSURE "</th><td>%s " D_UNIT_PRESSURE "</td></tr>";
const char HTTP_SNS_LIGHT[] PROGMEM =
  "<tr><th>%s " D_LIGHT "</th><td>%d%</td></tr>";
const char HTTP_SNS_NOISE[] PROGMEM =
  "<tr><th>%s " D_NOISE "</th><td>%d%</td></tr>";
const char HTTP_SNS_DUST[] PROGMEM =
  "<tr><th>%s " D_AIR_QUALITY "</th><td>%d%</td></tr>";
const char HTTP_END[] PROGMEM =
  "</div>"
  "</body>"
  "</html>";

const char HDR_CTYPE_PLAIN[] PROGMEM = "text/plain";
const char HDR_CTYPE_HTML[] PROGMEM = "text/html";
const char HDR_CTYPE_XML[] PROGMEM = "text/xml";
const char HDR_CTYPE_JSON[] PROGMEM = "application/json";
const char HDR_CTYPE_STREAM[] PROGMEM = "application/octet-stream";

#define DNS_PORT 53
enum http_t {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER};

DNSServer *dnsServer;
ESP8266WebServer *webServer;

boolean _removeDuplicateAPs = true;
int _minimumQuality = -1;
uint8_t _httpflag = HTTP_OFF;
uint8_t _uploaderror = 0;
uint8_t _uploadfiletype;
uint8_t _colcount;

void startWebserver(int type, IPAddress ipweb)
{
  if (!_httpflag) {
    if (!webServer) {
      webServer = new ESP8266WebServer((HTTP_MANAGER==type)?80:WEB_PORT);
      webServer->on("/", handleRoot);
      webServer->on("/cn", handleConfig);
      webServer->on("/md", handleModule);
      webServer->on("/w1", handleWifi1);
      webServer->on("/w0", handleWifi0);
      if (sysCfg.flag.mqtt_enabled) {
        webServer->on("/mq", handleMqtt);
#ifdef USE_DOMOTICZ
        webServer->on("/dm", handleDomoticz);
#endif  // USE_DOMOTICZ
      }
      webServer->on("/lg", handleLog);
      webServer->on("/co", handleOther);
      webServer->on("/dl", handleDownload);
      webServer->on("/sv", handleSave);
      webServer->on("/rs", handleRestore);
      webServer->on("/rt", handleReset);
      webServer->on("/up", handleUpgrade);
      webServer->on("/u1", handleUpgradeStart);  // OTA
      webServer->on("/u2", HTTP_POST, handleUploadDone, handleUploadLoop);
      webServer->on("/cm", handleCmnd);
      webServer->on("/cs", handleConsole);
      webServer->on("/ax", handleAjax);
      webServer->on("/ay", handleAjax2);
      webServer->on("/in", handleInfo);
      webServer->on("/rb", handleRestart);
      webServer->on("/fwlink", handleRoot);  // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
#ifdef USE_EMULATION
      if (EMUL_WEMO == sysCfg.flag.emulation) {
        webServer->on("/upnp/control/basicevent1", HTTP_POST, handleUPnPevent);
        webServer->on("/eventservice.xml", handleUPnPservice);
        webServer->on("/setup.xml", handleUPnPsetupWemo);
      }
      if (EMUL_HUE == sysCfg.flag.emulation) {
        webServer->on("/description.xml", handleUPnPsetupHue);
      }
#endif  // USE_EMULATION
      webServer->onNotFound(handleNotFound);
    }
    logajaxflg = 0;
    webServer->begin(); // Web server start
  }
  if (_httpflag != type) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"),
      Hostname, (mDNSbegun) ? ".local" : "", ipweb.toString().c_str());
    addLog(LOG_LEVEL_INFO);
  }
  if (type) _httpflag = type;
}

void stopWebserver()
{
  if (_httpflag) {
    webServer->close();
    _httpflag = HTTP_OFF;
    addLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

void beginWifiManager()
{
  // setup AP
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    WiFi.mode(WIFI_AP_STA);
    addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
    WiFi.mode(WIFI_AP);
    addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
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
  if (dnsServer) {
    dnsServer->processNextRequest();
  }
  if (webServer) {
    webServer->handleClient();
  }
}

void setHeader()
{
  webServer->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  webServer->sendHeader(F("Pragma"), F("no-cache"));
  webServer->sendHeader(F("Expires"), F("-1"));
}

void showPage(String &page)
{
  if((HTTP_ADMIN == _httpflag) && (sysCfg.web_password[0] != 0) && !webServer->authenticate(WEB_USERNAME, sysCfg.web_password)) {
    return webServer->requestAuthentication();
  }
  page.replace(F("{ha}"), my_module.name);
  page.replace(F("{h}"), sysCfg.friendlyname[0]);
  if (HTTP_MANAGER == _httpflag) {
    if (WIFI_configCounter()) {
      page.replace(F("<body>"), F("<body onload='u()'>"));
      page += FPSTR(HTTP_COUNTER);
    }
  }
  page += FPSTR(HTTP_END);
  setHeader();
  webServer->send(200, FPSTR(HDR_CTYPE_HTML), page);
}

void handleRoot()
{
//  addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MAIN_MENU));
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_MAIN_MENU);

  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  if (HTTP_MANAGER == _httpflag) {
    handleWifi0();
  } else {
    char stemp[10];
    char line[160];
    String page = FPSTR(HTTP_HEAD);
    page.replace(F("{v}"), FPSTR(S_MAIN_MENU));
    page.replace(F("<body>"), F("<body onload='la()'>"));

    page += F("<div id='l1' name='l1'></div>");
    if (Maxdevice) {
      if (sfl_flg) {
        if ((2 == sfl_flg) || (5 == sfl_flg)) {
          snprintf_P(line, sizeof(line), HTTP_MSG_SLIDER1, sl_getColorTemp());
          page += line;
        }
        snprintf_P(line, sizeof(line), HTTP_MSG_SLIDER2, sysCfg.led_dimmer[0]);
        page += line;
      }
      page += FPSTR(HTTP_TABLE100);
      page += F("<tr>");
      for (byte idx = 1; idx <= Maxdevice; idx++) {
        snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
        snprintf_P(line, sizeof(line), PSTR("<td style='width:%d%'><button onclick='la(\"?o=%d\");'>" D_BUTTON_TOGGLE "%s</button></td>"),
          100 / Maxdevice, idx, (Maxdevice > 1) ? stemp : "");
        page += line;
      }
      page += F("</tr></table>");
    }
    if (SONOFF_BRIDGE == sysCfg.module) {
      page += FPSTR(HTTP_TABLE100);
      page += F("<tr>");
      byte idx = 0;
      for (byte i = 0; i < 4; i++) {
        if (idx > 0) {
          page += F("</tr><tr>");
        }
        for (byte j = 0; j < 4; j++) {
          idx++;
          snprintf_P(line, sizeof(line), PSTR("<td style='width:25%'><button onclick='la(\"?k=%d\");'>%d</button></td>"),
            idx, idx);
          page += line;
        }
      }
      page += F("</tr></table>");
    }

    if (HTTP_ADMIN == _httpflag) {
      page += FPSTR(HTTP_BTN_MENU1);
      page += FPSTR(HTTP_BTN_RSTRT);
    }
    showPage(page);
  }
}

void handleAjax2()
{
  char svalue[50];

  if (strlen(webServer->arg("o").c_str())) {
    do_cmnd_power(atoi(webServer->arg("o").c_str()), 2);
  }
  if (strlen(webServer->arg("d").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_DIMMER " %s"), webServer->arg("d").c_str());
    do_cmnd(svalue);
  }
  if (strlen(webServer->arg("t").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_COLORTEMPERATURE " %s"), webServer->arg("t").c_str());
    do_cmnd(svalue);
  }
  if (strlen(webServer->arg("k").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RFKEY "%s"), webServer->arg("k").c_str());
    do_cmnd(svalue);
  }

  String tpage = "";
  tpage += counter_webPresent();
#ifndef USE_ADC_VCC
  if (pin[GPIO_ADC0] < 99) {
    snprintf_P(svalue, sizeof(svalue), PSTR("<tr><th>" D_ANALOG_INPUT0 "</th><td>%d</td></tr>"), getAdc0());
    tpage += svalue;
  }
#endif
  if (hlw_flg) {
    tpage += hlw_webPresent();
  }
  if (SONOFF_SC == sysCfg.module) {
    tpage += sc_webPresent();
  }
#ifdef USE_DS18B20
  if (pin[GPIO_DSB] < 99) {
    tpage += dsb_webPresent();
  }
#endif  // USE_DS18B20
#ifdef USE_DS18x20
  if (pin[GPIO_DSB] < 99) {
    tpage += ds18x20_webPresent();
  }
#endif  // USE_DS18x20
#ifdef USE_DHT
  if (dht_flg) {
    tpage += dht_webPresent();
  }
#endif  // USE_DHT
#ifdef USE_I2C
  if (i2c_flg) {
#ifdef USE_SHT
    tpage += sht_webPresent();
#endif
#ifdef USE_HTU
    tpage += htu_webPresent();
#endif
#ifdef USE_BMP
    tpage += bmp_webPresent();
#endif
#ifdef USE_BH1750
    tpage += bh1750_webPresent();
#endif
  }
#endif  // USE_I2C
  String page = "";
  if (tpage.length() > 0) {
    page += FPSTR(HTTP_TABLE100);
    page += tpage;
    page += F("</table>");
  }
  char line[120];
  if (Maxdevice) {
    page += FPSTR(HTTP_TABLE100);
    page += F("<tr>");
    for (byte idx = 1; idx <= Maxdevice; idx++) {
      snprintf_P(line, sizeof(line), PSTR("<td style='width:%d%'><div style='text-align:center;font-weight:bold;font-size:%dpx'>%s</div></td>"),
        100 / Maxdevice, 70 - (Maxdevice * 8), getStateText(bitRead(power, idx -1)));
      page += line;
    }
    page += F("</tr></table>");
  }
/*
 * Will interrupt user action when selected
  if (sfl_flg) {
    snprintf_P(line, sizeof(line), PSTR("<input type='range' min='1' max='100' value='%d' onchange='lb(value)'>"),
      sysCfg.led_dimmer[0]);
    page += line;
  }
*/
  webServer->send(200, FPSTR(HDR_CTYPE_HTML), page);
}

boolean httpUser()
{
  boolean status = (HTTP_USER == _httpflag);
  if (status) {
    handleRoot();
  }
  return status;
}

void handleConfig()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURATION));
  page += FPSTR(HTTP_BTN_MENU2);
  if (sysCfg.flag.mqtt_enabled) {
    page += FPSTR(HTTP_BTN_MENU3);
  }
  page += FPSTR(HTTP_BTN_MENU4);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

boolean inModule(byte val, uint8_t *arr)
{
  int offset = 0;

  if (!val) {
    return false;  // None
  }
#ifndef USE_I2C
  if (GPIO_I2C_SCL == val) {
    return true;
  }
  if (GPIO_I2C_SDA == val) {
    return true;
  }
#endif
#ifndef USE_WS2812
  if (GPIO_WS2812 == val) {
    return true;
  }
#endif
#ifndef USE_IR_REMOTE
  if (GPIO_IRSEND == val) {
    return true;
  }
#endif
  if (((val >= GPIO_REL1) && (val <= GPIO_REL4)) || ((val >= GPIO_LED1) && (val <= GPIO_LED4))) {
    offset = 4;
  }
  if (((val >= GPIO_REL1_INV) && (val <= GPIO_REL4_INV)) || ((val >= GPIO_LED1_INV) && (val <= GPIO_LED4_INV))) {
    offset = -4;
  }
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (arr[i] == val) {
      return true;
    }
    if (arr[i] == val + offset) {
      return true;
    }
  }
  return false;
}

void handleModule()
{
  if (httpUser()) {
    return;
  }
  char stemp[20];
  char line[128];
  uint8_t midx;

  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MODULE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MODULE));
  page += FPSTR(HTTP_FORM_MODULE);
  snprintf_P(stemp, sizeof(stemp), modules[MODULE].name);
  page.replace(F("{mt}"), stemp);

  mytmplt cmodule;
  memcpy_P(&cmodule, &modules[sysCfg.module], sizeof(cmodule));

  String func = FPSTR(HTTP_SCRIPT_MODULE1);
  for (byte i = 0; i < MAXMODULE; i++) {
    midx = pgm_read_byte(nicelist + i);
    snprintf_P(stemp, sizeof(stemp), modules[midx].name);
    snprintf_P(line, sizeof(line), HTTP_SCRIPT_MODULE2, midx, midx +1, stemp);
    func += line;
  }
  func += FPSTR(HTTP_SCRIPT_MODULE3);
  snprintf_P(line, sizeof(line), PSTR("sk(%d,99);o0=\""), sysCfg.module);  // g99
  func += line;
  for (byte j = 0; j < GPIO_SENSOR_END; j++) {
    if (!inModule(j, cmodule.gp.io)) {
      snprintf_P(stemp, sizeof(stemp), sensors[j]);
      snprintf_P(line, sizeof(line), HTTP_SCRIPT_MODULE2, j, j, stemp);
      func += line;
    }
  }
  func += FPSTR(HTTP_SCRIPT_MODULE3);
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (GPIO_USER == cmodule.gp.io[i]) {
      snprintf_P(line, sizeof(line), PSTR("<br/><b>" D_GPIO "%d</b> %s<select id='g%d' name='g%d'></select></br>"),
        i, (0==i)? D_SENSOR_BUTTON "1":(1==i)? D_SERIAL_OUT :(3==i)? D_SERIAL_IN :(12==i)? D_SENSOR_RELAY "1":(13==i)? D_SENSOR_LED "1I":(14==i)? D_SENSOR :"", i, i);
      page += line;
      snprintf_P(line, sizeof(line), PSTR("sk(%d,%d);"), my_module.gp.io[i], i);  // g0 - g16
      func += line;
    }
  }
  func += F("}</script>");
  page.replace(F("</script>"), func);
  page.replace(F("<body>"), F("<body onload='sl()'>"));

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
  if (httpUser()) {
    return;
  }

  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_WIFI);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_WIFI));

  if (scan) {
#ifdef USE_EMULATION
    UDP_Disconnect();
#endif  // USE_EMULATION
    int n = WiFi.scanNetworks();
    addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

    if (0 == n) {
      addLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, S_NO_NETWORKS_FOUND);
      page += FPSTR(S_NO_NETWORKS_FOUND);
      page += F(". " D_REFRESH_TO_SCAN_AGAIN ".");
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
          if (-1 == indices[i]) {
            continue;
          }
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              addLog(LOG_LEVEL_DEBUG);
              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (-1 == indices[i]) {
          continue; // skip dups
        }
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_SSID " %s, " D_RSSI " %d"), WiFi.SSID(indices[i]).c_str(), WiFi.RSSI(indices[i]));
        addLog(LOG_LEVEL_DEBUG);
        int quality = WIFI_getRSSIasQuality(WiFi.RSSI(indices[i]));

        if (_minimumQuality == -1 || _minimumQuality < quality) {
          String item = FPSTR(HTTP_LNK_ITEM);
          String rssiQ;
          rssiQ += quality;
          item.replace(F("{v}"), WiFi.SSID(indices[i]));
          item.replace(F("{r}"), rssiQ);
          uint8_t auth = WiFi.encryptionType(indices[i]);
          item.replace(F("{i}"), (ENC_TYPE_WEP == auth) ? F(D_WEP) : (ENC_TYPE_TKIP == auth) ? F(D_WPA_PSK) : (ENC_TYPE_CCMP == auth) ? F(D_WPA2_PSK) : (ENC_TYPE_AUTO == auth) ? F(D_AUTO) : F(""));
          page += item;
          delay(0);
        } else {
          addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SKIPPING_LOW_QUALITY));
        }

      }
      page += "<br/>";
    }
  } else {
    page += FPSTR(HTTP_LNK_SCAN);
  }

  page += FPSTR(HTTP_FORM_WIFI);
  page.replace(F("{h1}"), sysCfg.hostname);
  page.replace(F("{s1}"), sysCfg.sta_ssid[0]);
  page.replace(F("{p1}"), sysCfg.sta_pwd[0]);
  page.replace(F("{s2}"), sysCfg.sta_ssid[1]);
  page.replace(F("{p2}"), sysCfg.sta_pwd[1]);
  page += FPSTR(HTTP_FORM_END);
  if (HTTP_MANAGER == _httpflag) {
    page += FPSTR(HTTP_BTN_RSTRT);
  } else {
    page += FPSTR(HTTP_BTN_CONF);
  }
  showPage(page);
}

void handleMqtt()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MQTT);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MQTT));
  page += FPSTR(HTTP_FORM_MQTT);
  char str[sizeof(sysCfg.mqtt_client)];
  getClient(str, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  page.replace(F("{m0}"), str);
  page.replace(F("{m1}"), sysCfg.mqtt_host);
  page.replace(F("{m2}"), String(sysCfg.mqtt_port));
  page.replace(F("{m3}"), sysCfg.mqtt_client);
  page.replace(F("{m4}"), (sysCfg.mqtt_user[0] == '\0')?"0":sysCfg.mqtt_user);
  page.replace(F("{m5}"), (sysCfg.mqtt_pwd[0] == '\0')?"0":sysCfg.mqtt_pwd);
  page.replace(F("{m6}"), sysCfg.mqtt_topic);
  page.replace(F("{m7}"), sysCfg.mqtt_fulltopic);
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleLog()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_LOGGING);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_LOGGING));
  page += FPSTR(HTTP_FORM_LOG1);
  for (byte idx = 0; idx < 3; idx++) {
    page += FPSTR(HTTP_FORM_LOG2);
    switch (idx) {
    case 0:
      page.replace(F("{b0}"), F(D_SERIAL " "));
      page.replace(F("{b1}"), STR(SERIAL_LOG_LEVEL));
      page.replace(F("{b2}"), F("ls"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.seriallog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 1:
      page.replace(F("{b0}"), F(D_WEB " "));
      page.replace(F("{b1}"), STR(WEB_LOG_LEVEL));
      page.replace(F("{b2}"), F("lw"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.weblog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 2:
      page.replace(F("{b0}"), F(D_SYS));
      page.replace(F("{b1}"), STR(SYS_LOG_LEVEL));
      page.replace(F("{b2}"), F("ll"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == sysCfg.syslog_level) ? F(" selected ") : F(" "));
      }
      break;
    }
  }
  page += FPSTR(HTTP_FORM_LOG3);
  page.replace(F("{l2}"), sysCfg.syslog_host);
  page.replace(F("{l3}"), String(sysCfg.syslog_port));
  page.replace(F("{l4}"), String(sysCfg.tele_period));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleOther()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_OTHER);
  char stemp[40];

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_OTHER));
  page += FPSTR(HTTP_FORM_OTHER);
  page.replace(F("{p1}"), sysCfg.web_password);
  page.replace(F("{r1}"), (sysCfg.flag.mqtt_enabled) ? F(" checked") : F(""));
  page += FPSTR(HTTP_FORM_OTHER2);
  page.replace(F("{1"), F("1"));
  page.replace(F("{2"), FRIENDLY_NAME);
  page.replace(F("{3"), sysCfg.friendlyname[0]);
#ifdef USE_EMULATION
  page += FPSTR(HTTP_FORM_OTHER3a);
  for (byte i = 0; i < EMUL_MAX; i++) {
    page += FPSTR(HTTP_FORM_OTHER3b);
    page.replace(F("{1"), String(i));
    page.replace(F("{2"), (i == sysCfg.flag.emulation) ? F(" checked") : F(""));
    page.replace(F("{3"), (i == EMUL_NONE) ? F(D_NONE) : (i == EMUL_WEMO) ? F(D_BELKIN_WEMO) : F(D_HUE_BRIDGE));
    page.replace(F("{4"), (i == EMUL_NONE) ? F("") : (i == EMUL_WEMO) ? F(" " D_SINGLE_DEVICE) : F(" " D_MULTI_DEVICE));
  }
  page += F("<br/>");
  for (int i = 1; i < Maxdevice; i++) {
    page += FPSTR(HTTP_FORM_OTHER2);
    page.replace(F("{1"), String(i +1));
    snprintf_P(stemp, sizeof(stemp), PSTR(FRIENDLY_NAME"%d"), i +1);
    page.replace(F("{2"), stemp);
    page.replace(F("{3"), sysCfg.friendlyname[i]);
  }
  page += F("<br/></fieldset>");
#endif  // USE_EMULATION
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void handleDownload()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  uint8_t buffer[sizeof(sysCfg)];

  WiFiClient myClient = webServer->client();
  webServer->setContentLength(4096);

  char attachment[100];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"),
    sysCfg.friendlyname[0], Version);
  webServer->sendHeader(F("Content-Disposition"), attachment);
  webServer->send(200, FPSTR(HDR_CTYPE_STREAM), "");
  memcpy(buffer, &sysCfg, sizeof(sysCfg));
  buffer[0] = CONFIG_FILE_SIGN;
  buffer[1] = (!CONFIG_FILE_XOR)?0:1;
  if (buffer[1]) {
    for (uint16_t i = 2; i < sizeof(buffer); i++) {
      buffer[i] ^= (CONFIG_FILE_XOR +i);
    }
  }
  myClient.write((const char*)buffer, sizeof(buffer));
}

void handleSave()
{
  if (httpUser()) {
    return;
  }

  char stemp[TOPSZ];
  char stemp2[TOPSZ];
  byte what = 0;
  byte restart;
  String result = "";

  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_SAVE_CONFIGURATION);

  if (strlen(webServer->arg("w").c_str())) {
    what = atoi(webServer->arg("w").c_str());
  }
  switch (what) {
  case 1:
    strlcpy(sysCfg.hostname, (!strlen(webServer->arg("h").c_str())) ? WIFI_HOSTNAME : webServer->arg("h").c_str(), sizeof(sysCfg.hostname));
    if (strstr(sysCfg.hostname,"%")) {
      strlcpy(sysCfg.hostname, WIFI_HOSTNAME, sizeof(sysCfg.hostname));
    }
    strlcpy(sysCfg.sta_ssid[0], (!strlen(webServer->arg("s1").c_str())) ? STA_SSID1 : webServer->arg("s1").c_str(), sizeof(sysCfg.sta_ssid[0]));
    strlcpy(sysCfg.sta_pwd[0], (!strlen(webServer->arg("p1").c_str())) ? STA_PASS1 : webServer->arg("p1").c_str(), sizeof(sysCfg.sta_pwd[0]));
    strlcpy(sysCfg.sta_ssid[1], (!strlen(webServer->arg("s2").c_str())) ? STA_SSID2 : webServer->arg("s2").c_str(), sizeof(sysCfg.sta_ssid[1]));
    strlcpy(sysCfg.sta_pwd[1], (!strlen(webServer->arg("p2").c_str())) ? STA_PASS2 : webServer->arg("p2").c_str(), sizeof(sysCfg.sta_pwd[1]));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_PASSWORD "1 %s, " D_CMND_SSID "2 %s, " D_CMND_PASSWORD "2 %s"),
      sysCfg.hostname, sysCfg.sta_ssid[0], sysCfg.sta_pwd[0], sysCfg.sta_ssid[1], sysCfg.sta_pwd[1]);
    addLog(LOG_LEVEL_INFO);
    result += F("<br/>" D_TRYING_TO_CONNECT "<br/>");
    break;
  case 2:
    strlcpy(stemp, (!strlen(webServer->arg("mt").c_str())) ? MQTT_TOPIC : webServer->arg("mt").c_str(), sizeof(stemp));
    mqttfy(0, stemp);
    strlcpy(stemp2, (!strlen(webServer->arg("mf").c_str())) ? MQTT_FULLTOPIC : webServer->arg("mf").c_str(), sizeof(stemp2));
    mqttfy(1,stemp2);
    if ((strcmp(stemp, sysCfg.mqtt_topic)) || (strcmp(stemp2, sysCfg.mqtt_fulltopic))) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), (sysCfg.flag.mqtt_offline) ? S_OFFLINE : "");
      mqtt_publish_topic_P(2, S_LWT, true);  // Offline or remove previous retained topic
    }
    strlcpy(sysCfg.mqtt_topic, stemp, sizeof(sysCfg.mqtt_topic));
    strlcpy(sysCfg.mqtt_fulltopic, stemp2, sizeof(sysCfg.mqtt_fulltopic));
    strlcpy(sysCfg.mqtt_host, (!strlen(webServer->arg("mh").c_str())) ? MQTT_HOST : webServer->arg("mh").c_str(), sizeof(sysCfg.mqtt_host));
    sysCfg.mqtt_port = (!strlen(webServer->arg("ml").c_str())) ? MQTT_PORT : atoi(webServer->arg("ml").c_str());
    strlcpy(sysCfg.mqtt_client, (!strlen(webServer->arg("mc").c_str())) ? MQTT_CLIENT_ID : webServer->arg("mc").c_str(), sizeof(sysCfg.mqtt_client));
    strlcpy(sysCfg.mqtt_user, (!strlen(webServer->arg("mu").c_str())) ? MQTT_USER : (!strcmp(webServer->arg("mu").c_str(),"0")) ? "" : webServer->arg("mu").c_str(), sizeof(sysCfg.mqtt_user));
    strlcpy(sysCfg.mqtt_pwd, (!strlen(webServer->arg("mp").c_str())) ? MQTT_PASS : (!strcmp(webServer->arg("mp").c_str(),"0")) ? "" : webServer->arg("mp").c_str(), sizeof(sysCfg.mqtt_pwd));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_MQTTUSER " %s, " D_CMND_MQTTPASSWORD " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
      sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, sysCfg.mqtt_user, sysCfg.mqtt_pwd, sysCfg.mqtt_topic, sysCfg.mqtt_fulltopic);
    addLog(LOG_LEVEL_INFO);
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
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
      sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.syslog_port, sysCfg.tele_period);
    addLog(LOG_LEVEL_INFO);
    break;
#ifdef USE_DOMOTICZ
  case 4:
    domoticz_saveSettings();
    break;
#endif  // USE_DOMOTICZ
  case 5:
    strlcpy(sysCfg.web_password, (!strlen(webServer->arg("p1").c_str())) ? WEB_PASSWORD : (!strcmp(webServer->arg("p1").c_str(),"0")) ? "" : webServer->arg("p1").c_str(), sizeof(sysCfg.web_password));
    sysCfg.flag.mqtt_enabled = webServer->hasArg("b1");
#ifdef USE_EMULATION
    sysCfg.flag.emulation = (!strlen(webServer->arg("b2").c_str())) ? 0 : atoi(webServer->arg("b2").c_str());
#endif  // USE_EMULATION
    strlcpy(sysCfg.friendlyname[0], (!strlen(webServer->arg("a1").c_str())) ? FRIENDLY_NAME : webServer->arg("a1").c_str(), sizeof(sysCfg.friendlyname[0]));
    strlcpy(sysCfg.friendlyname[1], (!strlen(webServer->arg("a2").c_str())) ? FRIENDLY_NAME"2" : webServer->arg("a2").c_str(), sizeof(sysCfg.friendlyname[1]));
    strlcpy(sysCfg.friendlyname[2], (!strlen(webServer->arg("a3").c_str())) ? FRIENDLY_NAME"3" : webServer->arg("a3").c_str(), sizeof(sysCfg.friendlyname[2]));
    strlcpy(sysCfg.friendlyname[3], (!strlen(webServer->arg("a4").c_str())) ? FRIENDLY_NAME"4" : webServer->arg("a4").c_str(), sizeof(sysCfg.friendlyname[3]));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_CMND_EMULATION " %d, " D_CMND_FRIENDLYNAME " %s, %s, %s, %s"),
      getStateText(sysCfg.flag.mqtt_enabled), sysCfg.flag.emulation, sysCfg.friendlyname[0], sysCfg.friendlyname[1], sysCfg.friendlyname[2], sysCfg.friendlyname[3]);
    addLog(LOG_LEVEL_INFO);
    break;
  case 6:
    byte new_module = (!strlen(webServer->arg("g99").c_str())) ? MODULE : atoi(webServer->arg("g99").c_str());
    byte new_modflg = (sysCfg.module != new_module);
    sysCfg.module = new_module;
    mytmplt cmodule;
    memcpy_P(&cmodule, &modules[sysCfg.module], sizeof(cmodule));
    String gpios = "";
    for (byte i = 0; i < MAX_GPIO_PIN; i++) {
      if (new_modflg) {
        sysCfg.my_module.gp.io[i] = 0;
      }
      if (GPIO_USER == cmodule.gp.io[i]) {
        snprintf_P(stemp, sizeof(stemp), PSTR("g%d"), i);
        sysCfg.my_module.gp.io[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
        gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(sysCfg.my_module.gp.io[i]);
      }
    }
    snprintf_P(stemp, sizeof(stemp), modules[sysCfg.module].name);
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), stemp, gpios.c_str());
    addLog(LOG_LEVEL_INFO);
    break;
  }

  restart = (!strlen(webServer->arg("r").c_str())) ? 1 : atoi(webServer->arg("r").c_str());
  if (restart) {
    String page = FPSTR(HTTP_HEAD);
    page.replace(F("{v}"), FPSTR(S_SAVE_CONFIGURATION));
    page += F("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br/>");
    page += result;
    page += F("</div>");
    page += FPSTR(HTTP_MSG_RSTRT);
    if (HTTP_MANAGER == _httpflag) {
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
  if (httpUser()) {
    return;
  }

  char svalue[16];

  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESET_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESET_CONFIGURATION));
  page += F("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RESET " 1"));
  do_cmnd(svalue);
}

void handleRestore()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTORE_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESTORE_CONFIGURATION));
  page += FPSTR(HTTP_FORM_RST);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1}"), F(D_RESTORE));
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);

  _uploaderror = 0;
  _uploadfiletype = 1;
}

void handleUpgrade()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_FIRMWARE_UPGRADE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_FIRMWARE_UPGRADE));
  page += FPSTR(HTTP_FORM_UPG);
  page.replace(F("{o1}"), sysCfg.otaUrl);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1}"), F(D_UPGRADE));
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  _uploaderror = 0;
  _uploadfiletype = 0;
}

void handleUpgradeStart()
{
  if (httpUser()) {
    return;
  }
  char svalue[100];

  addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  WIFI_configCounter();

  if (strlen(webServer->arg("o").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_OTAURL " %s"), webServer->arg("o").c_str());
    do_cmnd(svalue);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += F("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_UPGRADE " 1"));
  do_cmnd(svalue);
}

void handleUploadDone()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  WIFI_configCounter();
  restartflag = 0;
  mqttcounter = 0;

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += F("<div style='text-align:center;'><b>" D_UPLOAD " <font color='");
  if (_uploaderror) {
    page += F("red'>" D_FAILED "</font></b><br/><br/>");
    switch (_uploaderror) {
      case 1: strncpy_P(error, PSTR(D_UPLOAD_ERR_1), sizeof(error)); break;
      case 2: strncpy_P(error, PSTR(D_UPLOAD_ERR_2), sizeof(error)); break;
      case 3: strncpy_P(error, PSTR(D_UPLOAD_ERR_3), sizeof(error)); break;
      case 4: strncpy_P(error, PSTR(D_UPLOAD_ERR_4), sizeof(error)); break;
      case 5: strncpy_P(error, PSTR(D_UPLOAD_ERR_5), sizeof(error)); break;
      case 6: strncpy_P(error, PSTR(D_UPLOAD_ERR_6), sizeof(error)); break;
      case 7: strncpy_P(error, PSTR(D_UPLOAD_ERR_7), sizeof(error)); break;
      case 8: strncpy_P(error, PSTR(D_UPLOAD_ERR_8), sizeof(error)); break;
      case 9: strncpy_P(error, PSTR(D_UPLOAD_ERR_9), sizeof(error)); break;
      default:
        snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), _uploaderror);
    }
    page += error;
    snprintf_P(log_data, sizeof(log_data), PSTR(D_UPLOAD ": %s"), error);
    addLog(LOG_LEVEL_DEBUG);
    stop_flash_rotate = sysCfg.flag.stop_flash_rotate;
  } else {
    page += F("green'>" D_SUCCESSFUL "</font></b><br/>");
    page += FPSTR(HTTP_MSG_RSTRT);
    restartflag = 2;
  }
  page += F("</div><br/>");
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleUploadLoop()
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  boolean _serialoutput = (LOG_LEVEL_DEBUG <= seriallog_level);

  if (HTTP_USER == _httpflag) {
    return;
  }
  if (_uploaderror) {
    if (!_uploadfiletype) {
      Update.end();
    }
    return;
  }

  HTTPUpload& upload = webServer->upload();

  if (UPLOAD_FILE_START == upload.status) {
    restartflag = 60;
    if (0 == upload.filename.c_str()[0]) {
      _uploaderror = 1;
      return;
    }
    CFG_Save(1);  // Free flash for upload
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    addLog(LOG_LEVEL_INFO);
    if (!_uploadfiletype) {
      mqttcounter = 60;
#ifdef USE_EMULATION
      UDP_Disconnect();
#endif  // USE_EMULATION
      if (sysCfg.flag.mqtt_enabled) {
        mqttClient.disconnect();
      }
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {         //start with max available size
        _uploaderror = 2;
        return;
      }
    }
    _colcount = 0;
  } else if (!_uploaderror && (UPLOAD_FILE_WRITE == upload.status)) {
    if (0 == upload.totalSize)
    {
      if (_uploadfiletype) {
        if (upload.buf[0] != CONFIG_FILE_SIGN) {
          _uploaderror = 8;
          return;
        }
        if (upload.currentSize > sizeof(sysCfg)) {
          _uploaderror = 9;
          return;
        }
      } else {
        if (upload.buf[0] != 0xE9) {
          _uploaderror = 3;
          return;
        }
        uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
        if(bin_flash_size > ESP.getFlashChipRealSize()) {
          _uploaderror = 4;
          return;
        }
        upload.buf[2] = 3;  // Force DOUT - ESP8285
      }
    }
    if (_uploadfiletype) { // config
      if (!_uploaderror) {
        if (upload.buf[1]) {
          for (uint16_t i = 2; i < upload.currentSize; i++) {
            upload.buf[i] ^= (CONFIG_FILE_XOR +i);
          }
        }
        CFG_DefaultSet2();
        memcpy((char*)&sysCfg +16, upload.buf +16, upload.currentSize -16);
        memcpy((char*)&sysCfg +8, upload.buf +8, 4);  // Restore version and auto upgrade
      }
    } else {  // firmware
      if (!_uploaderror && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
        _uploaderror = 5;
        return;
      }
      if (_serialoutput) {
        Serial.printf(".");
        _colcount++;
        if (!(_colcount % 80)) {
          Serial.println();
        }
      }
    }
  } else if(!_uploaderror && (UPLOAD_FILE_END == upload.status)) {
    if (_serialoutput && (_colcount % 80)) {
      Serial.println();
    }
    if (!_uploadfiletype) {
      if (!Update.end(true)) { // true to set the size to the current progress
        if (_serialoutput) {
          Update.printError(Serial);
        }
        _uploaderror = 6;
        return;
      }
    }
    if (!_uploaderror) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
      addLog(LOG_LEVEL_INFO);
    }
  } else if (UPLOAD_FILE_ABORTED == upload.status) {
    restartflag = 0;
    mqttcounter = 0;
    _uploaderror = 7;
    if (!_uploadfiletype) {
      Update.end();
    }
  }
  delay(0);
}

void handleCmnd()
{
  if (httpUser()) {
    return;
  }
  char svalue[INPUT_BUFFER_SIZE];  // big to serve Backlog

  addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  uint8_t valid = 1;
  if (sysCfg.web_password[0] != 0) {
    if (!(!strcmp(webServer->arg("user").c_str(),WEB_USERNAME) && !strcmp(webServer->arg("password").c_str(),sysCfg.web_password))) {
      valid = 0;
    }
  }

  String message = "";
  if (valid) {
    byte curridx = logidx;
    if (strlen(webServer->arg("cmnd").c_str())) {
      snprintf_P(svalue, sizeof(svalue), webServer->arg("cmnd").c_str());
//      byte syslog_now = syslog_level;
//      syslog_level = 0;  // Disable UDP syslog to not trigger hardware WDT - Seems to work fine since 5.7.1d (global logging)
      do_cmnd(svalue);
//      syslog_level = syslog_now;
    }

    if (logidx != curridx) {
      byte counter = curridx;
      do {
        if (Log[counter].length()) {
          if (message.length()) {
            message += F("\n");
          }
          if (sysCfg.flag.mqtt_enabled) {
            // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
//            message += Log[counter].substring(17 + strlen(PUB_PREFIX) + strlen(sysCfg.mqtt_topic));
            message += Log[counter].substring(Log[counter].lastIndexOf("/",Log[counter].indexOf("="))+1);
          } else {
            // [14:49:36 RSLT: RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
            message += Log[counter].substring(Log[counter].indexOf(": ")+2);
          }
        }
        counter++;
        if (counter > MAX_LOG_LINES -1) {
          counter = 0;
        }
      } while (counter != logidx);
    } else {
      message = F(D_ENABLE_WEBLOG_FOR_RESPONSE "\n");
    }
  } else {
    message = F(D_NEED_USER_AND_PASSWORD "\n");
  }
  webServer->send(200, FPSTR(HDR_CTYPE_PLAIN), message);
}

void handleConsole()
{
  if (httpUser()) {
    return;
  }

  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONSOLE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONSOLE));
  page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_CONSOL));
  page.replace(F("<body>"), F("<body onload='l()'>"));
  page += FPSTR(HTTP_FORM_CMND);
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleAjax()
{
  if (httpUser()) {
    return;
  }
  char svalue[INPUT_BUFFER_SIZE];  // big to serve Backlog
  byte cflg = 1;
  byte counter = 99;

  if (strlen(webServer->arg("c1").c_str())) {
    snprintf_P(svalue, sizeof(svalue), webServer->arg("c1").c_str());
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_COMMAND "%s"), svalue);
    addLog(LOG_LEVEL_INFO);
//    byte syslog_now = syslog_level;
//    syslog_level = 0;  // Disable UDP syslog to not trigger hardware WDT - Seems to work fine since 5.7.1d (global logging)
    do_cmnd(svalue);
//    syslog_level = syslog_now;
  }

  if (strlen(webServer->arg("c2").c_str())) {
    counter = atoi(webServer->arg("c2").c_str());
  }

  String message = F("<r><i>");
  message += String(logidx);
  message += F("</i><j>");
  message += String(logajaxflg);
  if (!logajaxflg) {
    counter = 99;
    logajaxflg = 1;
  }
  message += F("</j><l>");
  if (counter != logidx) {
    if (99 == counter) {
      counter = logidx;
      cflg = 0;
    }
    do {
      if (Log[counter].length()) {
        if (cflg) {
          message += F("\n");
        } else {
          cflg = 1;
        }
        message += Log[counter];
      }
      counter++;
      if (counter > MAX_LOG_LINES -1) {
        counter = 0;
      }
    } while (counter != logidx);
  }
  message += F("</l></r>");
  webServer->send(200, FPSTR(HDR_CTYPE_XML), message);
}

void handleInfo()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_INFORMATION);

  char stopic[TOPSZ];

  int freeMem = ESP.getFreeHeap();

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
//  page += F("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>");
  page += F("<style>td{padding:0px 5px;}</style>");
  page += F("<table style'width:100%;'>");
  page += F("<tr><th>" D_PROGRAM_VERSION "</th><td>"); page += Version; page += F("</td></tr>");
  page += F("<tr><th>" D_BUILD_DATE_AND_TIME "</th><td>"); page += getBuildDateTime(); page += F("</td></tr>");
  page += F("<tr><th>" D_CORE_AND_SDK_VERSION "</th><td>"); page += ESP.getCoreVersion(); page += F("/"); page += String(ESP.getSdkVersion()); page += F("</td></tr>");
  page += F("<tr><th>" D_UPTIME "</th><td>"); page += String(uptime); page += F(" Hours</td></tr>");
  snprintf_P(stopic, sizeof(stopic), PSTR(" at %X"), CFG_Address());
  page += F("<tr><th>" D_FLASH_WRITE_COUNT "</th><td>"); page += String(sysCfg.saveFlag); page += stopic; page += F("</td></tr>");
  page += F("<tr><th>" D_BOOT_COUNT "</th><td>"); page += String(sysCfg.bootcount); page += F("</td></tr>");
  page += F("<tr><th>" D_RESTART_REASON "</th><td>"); page += getResetReason(); page += F("</td></tr>");
  for (byte i = 0; i < Maxdevice; i++) {
    page += F("<tr><th>" D_FRIENDLY_NAME " ");
    page += i +1;
    page += F("</th><td>"); page += sysCfg.friendlyname[i]; page += F("</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><th>" D_AP); page += String(sysCfg.sta_active +1);
    page += F(" " D_SSID " (" D_RSSI ")</th><td>"); page += sysCfg.sta_ssid[sysCfg.sta_active]; page += F(" ("); page += WIFI_getRSSIasQuality(WiFi.RSSI()); page += F("%)</td></tr>");
  page += F("<tr><th>" D_HOSTNAME "</th><td>"); page += Hostname; page += F("</td></tr>");
  if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
    page += F("<tr><th>" D_IP_ADDRESS "</th><td>"); page += WiFi.localIP().toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_GATEWAY "</th><td>"); page += IPAddress(sysCfg.ip_address[1]).toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_SUBNET_MASK "</th><td>"); page += IPAddress(sysCfg.ip_address[2]).toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_DNS_SERVER "</th><td>"); page += IPAddress(sysCfg.ip_address[3]).toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_MAC_ADDRESS "</th><td>"); page += WiFi.macAddress(); page += F("</td></tr>");
  }
  if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
    page += F("<tr><th>" D_AP " " D_IP_ADDRESS "</th><td>"); page += WiFi.softAPIP().toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_AP " " D_GATEWAY "</th><td>"); page += WiFi.softAPIP().toString(); page += F("</td></tr>");
    page += F("<tr><th>" D_AP " " D_MAC_ADDRESS "</th><td>"); page += WiFi.softAPmacAddress(); page += F("</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
  if (sysCfg.flag.mqtt_enabled) {
    page += F("<tr><th>" D_MQTT_HOST "</th><td>"); page += sysCfg.mqtt_host; page += F("</td></tr>");
    page += F("<tr><th>" D_MQTT_PORT "</th><td>"); page += String(sysCfg.mqtt_port); page += F("</td></tr>");
    page += F("<tr><th>" D_MQTT_CLIENT " &<br/>&nbsp;" D_FALLBACK_TOPIC "</th><td>"); page += MQTTClient; page += F("</td></tr>");
    page += F("<tr><th>" D_MQTT_USER "</th><td>"); page += sysCfg.mqtt_user; page += F("</td></tr>");
    page += F("<tr><th>" D_MQTT_TOPIC "</th><td>"); page += sysCfg.mqtt_topic; page += F("</td></tr>");
    page += F("<tr><th>" D_MQTT_GROUP_TOPIC "</th><td>"); page += sysCfg.mqtt_grptopic; page += F("</td></tr>");
    getTopic_P(stopic, 0, sysCfg.mqtt_topic, "");
    page += F("<tr><th>" D_MQTT_FULL_TOPIC "</th><td>"); page += stopic; page += F("</td></tr>");

  } else {
    page += F("<tr><th>" D_MQTT "</th><td>" D_DISABLED "</td></tr>");
  }
  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><th>" D_EMULATION "</th><td>");
#ifdef USE_EMULATION
  if (EMUL_WEMO == sysCfg.flag.emulation) {
    page += F(D_BELKIN_WEMO);
  }
  else if (EMUL_HUE == sysCfg.flag.emulation) {
    page += F(D_HUE_BRIDGE);
  }
  else {
    page += F(D_NONE);
  }
#else
  page += F(D_DISABLED);
#endif // USE_EMULATION
  page += F("</td></tr>");

  page += F("<tr><th>" D_MDNS_DISCOVERY "</th><td>");
#ifdef USE_DISCOVERY
  page += F(D_ENABLED);
  page += F("</td></tr>");
  page += F("<tr><th>" D_MDNS_ADVERTISE "</th><td>");
#ifdef WEBSERVER_ADVERTISE
  page += F(D_WEB_SERVER);
#else
  page += F(D_DISABLED);
#endif // WEBSERVER_ADVERTISE
#else
  page += F(D_DISABLED);
#endif // USE_DISCOVERY
  page += F("</td></tr>");

  page += F("<tr><td>&nbsp;</td></tr>");
  page += F("<tr><th>" D_ESP_CHIP_ID "</th><td>"); page += String(ESP.getChipId()); page += F("</td></tr>");
  page += F("<tr><th>" D_FLASH_CHIP_ID "</th><td>"); page += String(ESP.getFlashChipId()); page += F("</td></tr>");
  page += F("<tr><th>" D_FLASH_CHIP_SIZE "</th><td>"); page += String(ESP.getFlashChipRealSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><th>" D_PROGRAM_FLASH_SIZE "</th><td>"); page += String(ESP.getFlashChipSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><th>" D_PROGRAM_SIZE "</th><td>"); page += String(ESP.getSketchSize() / 1024); page += F("kB</td></tr>");
  page += F("<tr><th>" D_FREE_PROGRAM_SPACE "</th><td>"); page += String(ESP.getFreeSketchSpace() / 1024); page += F("kB</td></tr>");
  page += F("<tr><th>" D_FREE_MEMORY "</th><td>"); page += String(freeMem / 1024); page += F("kB</td></tr>");
  page += F("</table>");
//  page += F("</fieldset>");
  page += FPSTR(HTTP_BTN_MAIN);
  showPage(page);
}

void handleRestart()
{
  if (httpUser()) {
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTART);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESTART));
  page += FPSTR(HTTP_MSG_RSTRT);
  if (HTTP_MANAGER == _httpflag) {
    _httpflag = HTTP_ADMIN;
  } else {
    page += FPSTR(HTTP_BTN_MAIN);
  }
  showPage(page);

  restartflag = 2;
}

/********************************************************************************************/

void handleNotFound()
{
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }

#ifdef USE_EMULATION
  String path = webServer->uri();
  if ((EMUL_HUE == sysCfg.flag.emulation) && (path.startsWith("/api"))) {
    handle_hue_api(&path);
  } else
#endif // USE_EMULATION
  {
    String message = F(D_FILE_NOT_FOUND "\n\nURI: ");
    message += webServer->uri();
    message += F("\nMethod: ");
    message += (webServer->method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += webServer->args();
    message += F("\n");
    for ( uint8_t i = 0; i < webServer->args(); i++ ) {
      message += " " + webServer->argName(i) + ": " + webServer->arg(i) + "\n";
    }
    setHeader();
    webServer->send(404, FPSTR(HDR_CTYPE_PLAIN), message);
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
  if ((HTTP_MANAGER == _httpflag) && !isIp(webServer->hostHeader())) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    webServer->sendHeader(F("Location"), String("http://") + webServer->client().localIP().toString(), true);
    webServer->send(302, FPSTR(HDR_CTYPE_PLAIN), ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
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
