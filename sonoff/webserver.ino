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
  "<title>{h} - {v}</title>"

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
        "var s=x.responseText.replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{t}/g,\"%'><div style='text-align:center;font-weight:\");"
        "document.getElementById('l1').innerHTML=s;"
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
  "input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;}"
  "select{width:100%;}"
  "textarea{resize:none;width:98%;height:318px;padding:5px;overflow:auto;}"
  "body{text-align:center;font-family:verdana;}"
  "td{padding:0px;}"
  "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;}"
  "button:hover{background-color:#006cba;}"
  "a{text-decoration:none;}"
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  "</style>"

  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;min-width:340px;'>"
#ifdef BE_MINIMAL
  "<div style='text-align:center;color:red;'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"
#endif
  "<div style='text-align:center;'><h3>{ha " D_MODULE "</h3><h2>{h}</h2></div>";
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
          "if(z.length>0){t.value+=decodeURIComponent(z[0].nodeValue);}"
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
    "}1'%d'>%02d %s}2";     // "}1" and "}2" means do not use "}x" in Module name and Sensor name
const char HTTP_SCRIPT_MODULE3[] PROGMEM =
    "\";"
    "os=o0.replace(/}1/g,\"<option value=\").replace(/}2/g,\"</option>\");";
const char HTTP_SCRIPT_INFO_BEGIN[] PROGMEM =
  "function i(){"
    "var s,o=\"";
const char HTTP_SCRIPT_INFO_END[] PROGMEM =
    "\";"                   // "}1" and "}2" means do not use "}x" in Information text
    "s=o.replace(/}1/g,\"</td></tr><tr><th>\").replace(/}2/g,\"</th><td>\");"
    "document.getElementById('i').innerHTML=s;"
  "}"
  "</script>";
const char HTTP_MSG_SLIDER1[] PROGMEM =
  "<div><span class='p'>" D_COLDLIGHT "</span><span class='q'>" D_WARMLIGHT "</span></div>"
  "<div><input type='range' min='153' max='500' value='%d' onchange='lc(value)'></div>";
const char HTTP_MSG_SLIDER2[] PROGMEM =
  "<div><span class='p'>" D_DARKLIGHT "</span><span class='q'>" D_BRIGHTLIGHT "</span></div>"
  "<div><input type='range' min='1' max='100' value='%d' onchange='lb(value)'></div>";
const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br/><div style='text-align:center;'>" D_DEVICE_WILL_RESTART "</div><br/>";
const char HTTP_BTN_MENU1[] PROGMEM =
#ifndef BE_MINIMAL
  "<br/><form action='cn' method='get'><button>" D_CONFIGURATION "</button></form>"
  "<br/><form action='in' method='get'><button>" D_INFORMATION "</button></form>"
#endif
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
  "<br/>"
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
  "<br/><b>" D_MODULE_TYPE "</b> ({mt)<br/><select id='g99' name='g99'></select><br/>";
const char HTTP_LNK_ITEM[] PROGMEM =
  "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q'>{i} {r}%</span></div>";
const char HTTP_LNK_SCAN[] PROGMEM =
  "<div><a href='/w1'>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br/>";
const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='1' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br/><input id='s1' name='s1' placeholder='" STA_SSID1 "' value='{s1'><br/>"
  "<br/><b>" D_AP1_PASSWORD "</b><br/><input id='p1' name='p1' type='password' placeholder='" STA_PASS1 "' value='{p1'><br/>"
  "<br/><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br/><input id='s2' name='s2' placeholder='" STA_SSID2 "' value='{s2'><br/>"
  "<br/><b>" D_AP2_PASSWORD "</b><br/><input id='p2' name='p2' type='password' placeholder='" STA_PASS2 "' value='{p2'><br/>"
  "<br/><b>" D_HOSTNAME "</b> (" WIFI_HOSTNAME ")<br/><input id='h' name='h' placeholder='" WIFI_HOSTNAME" ' value='{h1'><br/>";
const char HTTP_FORM_MQTT[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MQTT_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='2' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_HOST "</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' placeholder='" MQTT_HOST" ' value='{m1'><br/>"
  "<br/><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br/><input id='ml' name='ml' placeholder='" STR(MQTT_PORT) "' value='{m2'><br/>"
  "<br/><b>" D_CLIENT "</b> ({m0)<br/><input id='mc' name='mc' placeholder='" MQTT_CLIENT_ID "' value='{m3'><br/>"
  "<br/><b>" D_USER "</b> (" MQTT_USER ")<br/><input id='mu' name='mu' placeholder='" MQTT_USER "' value='{m4'><br/>"
  "<br/><b>" D_PASSWORD "</b><br/><input id='mp' name='mp' type='password' placeholder='" MQTT_PASS "' value='{m5'><br/>"
  "<br/><b>" D_TOPIC "</b> = %topic% (" MQTT_TOPIC ")<br/><input id='mt' name='mt' placeholder='" MQTT_TOPIC" ' value='{m6'><br/>"
  "<br/><b>" D_FULL_TOPIC "</b> (" MQTT_FULLTOPIC ")<br/><input id='mf' name='mf' placeholder='" MQTT_FULLTOPIC" ' value='{m7'><br/>";
const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='3' hidden><input id='r' name='r' value='0' hidden>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<br/><b>{b0" D_LOG_LEVEL "</b> ({b1)<br/><select id='{b2' name='{b2'>"
  "<option{a0value='0'>0 " D_NONE "</option>"
  "<option{a1value='1'>1 " D_ERROR "</option>"
  "<option{a2value='2'>2 " D_INFO "</option>"
  "<option{a3value='3'>3 " D_DEBUG "</option>"
  "<option{a4value='4'>4 " D_MORE_DEBUG "</option>"
  "</select><br/>";
const char HTTP_FORM_LOG3[] PROGMEM =
  "<br/><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' placeholder='" SYS_LOG_HOST "' value='{l2'><br/>"
  "<br/><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' placeholder='" STR(SYS_LOG_PORT) "' value='{l3'><br/>"
  "<br/><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' placeholder='" STR(TELE_PERIOD) "' value='{l4'><br/>";
const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend><form method='get' action='sv'>"
  "<input id='w' name='w' value='5' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><b>" D_WEB_ADMIN_PASSWORD "</b><br/><input id='p1' name='p1' type='password' placeholder='" WEB_PASSWORD "' value='{p1'><br/>"
  "<br/><input style='width:10%;' id='b1' name='b1' type='checkbox'{r1><b>" D_MQTT_ENABLE "</b><br/>";
  const char HTTP_FORM_OTHER2[] PROGMEM =
  "<br/><b>" D_FRIENDLY_NAME " {1</b> ({2)<br/><input id='a{1' name='a{1' placeholder='{2' value='{3'><br/>";
#ifdef USE_EMULATION
const char HTTP_FORM_OTHER3a[] PROGMEM =
  "<br/><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend>";
const char HTTP_FORM_OTHER3b[] PROGMEM =
  "<br/><input style='width:10%;' id='b2' name='b2' type='radio' value='{1'{2><b>{3</b>{4";
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
  "<br/>" D_OTA_URL "<br/><input id='o' name='o' placeholder='OTA_URL' value='{o1'><br/>"
  "<br/><button type='submit'>" D_START_UPGRADE "</button></form>"
  "</fieldset><br/><br/>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
const char HTTP_FORM_RST_UPG[] PROGMEM =
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br/><input type='file' name='u2'><br/>"
  "<br/><button type='submit' onclick='document.getElementById(\"f1\").style.display=\"none\";document.getElementById(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " {r1</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char HTTP_FORM_CMND[] PROGMEM =
  "<br/><textarea readonly id='t1' name='t1' cols='" STR(MESSZ) "' wrap='off'></textarea><br/><br/>"
  "<form method='get' onsubmit='return l(1);'>"
  "<input id='c1' name='c1' placeholder='" D_ENTER_COMMAND "' autofocus><br/>"
  //  "<br/><button type='submit'>Send command</button>"
  "</form>";
const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%'>";
const char HTTP_COUNTER[] PROGMEM =
  "<br/><div id='t' name='t' style='text-align:center;'></div>";
const char HTTP_END[] PROGMEM =
  "<br/>"
  "<div style='text-align:right;font-size:11px;'><hr/><a href='" D_WEBLINK "' target='_blank' style='color:#aaa;'>" D_PROGRAMNAME " " VERSION_STRING " " D_BY " " D_AUTHOR "</a></div>"
  "</div>"
  "</body>"
  "</html>";

const char HDR_CTYPE_PLAIN[] PROGMEM = "text/plain";
const char HDR_CTYPE_HTML[] PROGMEM = "text/html";
const char HDR_CTYPE_XML[] PROGMEM = "text/xml";
const char HDR_CTYPE_JSON[] PROGMEM = "application/json";
const char HDR_CTYPE_STREAM[] PROGMEM = "application/octet-stream";

#define DNS_PORT 53
enum HttpOptions {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER};

DNSServer *DnsServer;
ESP8266WebServer *WebServer;

boolean remove_duplicate_access_points = true;
int minimum_signal_quality = -1;
uint8_t webserver_state = HTTP_OFF;
uint8_t upload_error = 0;
uint8_t upload_file_type;
uint8_t upload_progress_dot_count;

void StartWebserver(int type, IPAddress ipweb)
{
  if (!webserver_state) {
    if (!WebServer) {
      WebServer = new ESP8266WebServer((HTTP_MANAGER==type) ? 80 : WEB_PORT);
      WebServer->on("/", HandleRoot);
      WebServer->on("/cn", HandleConfiguration);
      WebServer->on("/md", HandleModuleConfiguration);
      WebServer->on("/w1", HandleWifiConfigurationWithScan);
      WebServer->on("/w0", HandleWifiConfiguration);
      if (Settings.flag.mqtt_enabled) {
        WebServer->on("/mq", HandleMqttConfiguration);
#ifdef USE_DOMOTICZ
        WebServer->on("/dm", HandleDomoticzConfiguration);
#endif  // USE_DOMOTICZ
      }
      WebServer->on("/lg", HandleLoggingConfiguration);
      WebServer->on("/co", HandleOtherConfiguration);
      WebServer->on("/dl", HandleBackupConfiguration);
      WebServer->on("/sv", HandleSaveSettings);
      WebServer->on("/rs", HandleRestoreConfiguration);
      WebServer->on("/rt", HandleResetConfiguration);
      WebServer->on("/up", HandleUpgradeFirmware);
      WebServer->on("/u1", HandleUpgradeFirmwareStart);  // OTA
      WebServer->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);
      WebServer->on("/cm", HandleHttpCommand);
      WebServer->on("/cs", HandleConsole);
      WebServer->on("/ax", HandleAjaxConsoleRefresh);
      WebServer->on("/ay", HandleAjaxStatusRefresh);
      WebServer->on("/in", HandleInformation);
      WebServer->on("/rb", HandleRestart);
      WebServer->on("/fwlink", HandleRoot);  // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
#ifdef USE_EMULATION
      if (EMUL_WEMO == Settings.flag2.emulation) {
        WebServer->on("/upnp/control/basicevent1", HTTP_POST, HandleUpnpEvent);
        WebServer->on("/eventservice.xml", HandleUpnpService);
        WebServer->on("/setup.xml", HandleUpnpSetupWemo);
      }
      if (EMUL_HUE == Settings.flag2.emulation) {
        WebServer->on("/description.xml", HandleUpnpSetupHue);
      }
#endif  // USE_EMULATION
      WebServer->onNotFound(HandleNotFound);
    }
    reset_web_log_flag = 0;
    WebServer->begin(); // Web server start
  }
  if (webserver_state != type) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"),
      my_hostname, (mdns_begun) ? ".local" : "", ipweb.toString().c_str());
    AddLog(LOG_LEVEL_INFO);
  }
  if (type) {
    webserver_state = type;
  }
}

void StopWebserver()
{
  if (webserver_state) {
    WebServer->close();
    webserver_state = HTTP_OFF;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

void WifiManagerBegin()
{
  // setup AP
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    WiFi.mode(WIFI_AP_STA);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
    WiFi.mode(WIFI_AP);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
  }

  StopWebserver();

  DnsServer = new DNSServer();
  WiFi.softAP(my_hostname);
  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  StartWebserver(HTTP_MANAGER, WiFi.softAPIP());
}

void PollDnsWebserver()
{
  if (DnsServer) {
    DnsServer->processNextRequest();
  }
  if (WebServer) {
    WebServer->handleClient();
  }
}

void SetHeader()
{
  WebServer->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  WebServer->sendHeader(F("Pragma"), F("no-cache"));
  WebServer->sendHeader(F("Expires"), F("-1"));
}

void ShowPage(String &page)
{
  if((HTTP_ADMIN == webserver_state) && (Settings.web_password[0] != 0) && !WebServer->authenticate(WEB_USERNAME, Settings.web_password)) {
    return WebServer->requestAuthentication();
  }
  page.replace(F("{ha"), my_module.name);
  page.replace(F("{h}"), Settings.friendlyname[0]);
  if (HTTP_MANAGER == webserver_state) {
    if (WifiConfigCounter()) {
      page.replace(F("<body>"), F("<body onload='u()'>"));
      page += FPSTR(HTTP_COUNTER);
    }
  }
  page += FPSTR(HTTP_END);
  SetHeader();
  WebServer->send(200, FPSTR(HDR_CTYPE_HTML), page);
}

void HandleRoot()
{
//  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MAIN_MENU));
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_MAIN_MENU);

  if (CaptivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }

  if (HTTP_MANAGER == webserver_state) {
    HandleWifiConfiguration();
  } else {
    char stemp[10];
    char line[160];
    String page = FPSTR(HTTP_HEAD);
    page.replace(F("{v}"), FPSTR(S_MAIN_MENU));
    page.replace(F("<body>"), F("<body onload='la()'>"));

    page += F("<div id='l1' name='l1'></div>");
    if (devices_present) {
      if (light_type) {
        if ((LST_COLDWARM == (light_type &7)) || (LST_RGBWC == (light_type &7))) {
          snprintf_P(line, sizeof(line), HTTP_MSG_SLIDER1, LightGetColorTemp());
          page += line;
        }
        snprintf_P(line, sizeof(line), HTTP_MSG_SLIDER2, Settings.light_dimmer);
        page += line;
      }
      page += FPSTR(HTTP_TABLE100);
      page += F("<tr>");
      for (byte idx = 1; idx <= devices_present; idx++) {
        snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
        snprintf_P(line, sizeof(line), PSTR("<td style='width:%d%'><button onclick='la(\"?o=%d\");'>%s%s</button></td>"),
          100 / devices_present, idx, (devices_present < 5) ? D_BUTTON_TOGGLE : "", (devices_present > 1) ? stemp : "");
        page += line;
      }
      page += F("</tr></table>");
    }
    if (SONOFF_BRIDGE == Settings.module) {
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

    if (HTTP_ADMIN == webserver_state) {
      page += FPSTR(HTTP_BTN_MENU1);
      page += FPSTR(HTTP_BTN_RSTRT);
    }
    ShowPage(page);
  }
}

void HandleAjaxStatusRefresh()
{
  char svalue[80];

  if (strlen(WebServer->arg("o").c_str())) {
    ExecuteCommandPower(atoi(WebServer->arg("o").c_str()), 2);
  }
  if (strlen(WebServer->arg("d").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_DIMMER " %s"), WebServer->arg("d").c_str());
    ExecuteCommand(svalue);
  }
  if (strlen(WebServer->arg("t").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_COLORTEMPERATURE " %s"), WebServer->arg("t").c_str());
    ExecuteCommand(svalue);
  }
  if (strlen(WebServer->arg("k").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RFKEY "%s"), WebServer->arg("k").c_str());
    ExecuteCommand(svalue);
  }

  String page = "";
  mqtt_data[0] = '\0';
  XsnsCall(FUNC_XSNS_WEB);
  if (strlen(mqtt_data)) {
    page += FPSTR(HTTP_TABLE100);
    page += mqtt_data;
    page += F("</table>");
  }
  char line[80];
  if (devices_present) {
    page += FPSTR(HTTP_TABLE100);
    page += F("<tr>");
    uint8_t fsize = (devices_present < 5) ? 70 - (devices_present * 8) : 32;
    for (byte idx = 1; idx <= devices_present; idx++) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), bitRead(power, idx -1));
//      snprintf_P(line, sizeof(line), PSTR("<td style='width:%d%'><div style='text-align:center;font-weight:%s;font-size:%dpx'>%s</div></td>"),
      snprintf_P(line, sizeof(line), PSTR("<td style='width:%d{t}%s;font-size:%dpx'>%s</div></td>"),  // {t} = %'><div style='text-align:center;font-weight:
        100 / devices_present, (bitRead(power, idx -1)) ? "bold" : "normal", fsize, (devices_present < 5) ? GetStateText(bitRead(power, idx -1)) : svalue);
      page += line;
    }
    page += F("</tr></table>");
  }
/*
 * Will interrupt user action when selected
  if (light_type) {
    snprintf_P(line, sizeof(line), PSTR("<input type='range' min='1' max='100' value='%d' onchange='lb(value)'>"),
      Settings.light_dimmer);
    page += line;
  }
*/
  WebServer->send(200, FPSTR(HDR_CTYPE_HTML), page);
}

boolean HttpUser()
{
  boolean status = (HTTP_USER == webserver_state);
  if (status) {
    HandleRoot();
  }
  return status;
}

void HandleConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURATION));
  page += FPSTR(HTTP_BTN_MENU2);
  if (Settings.flag.mqtt_enabled) {
    page += FPSTR(HTTP_BTN_MENU3);
  }
  page += FPSTR(HTTP_BTN_MENU4);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

boolean GetUsedInModule(byte val, uint8_t *arr)
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
  if ((val >= GPIO_REL1) && (val < GPIO_REL1 + MAX_RELAYS)) {
    offset = (GPIO_REL1_INV - GPIO_REL1);
  }
  if ((val >= GPIO_REL1_INV) && (val < GPIO_REL1_INV + MAX_RELAYS)) {
    offset = -(GPIO_REL1_INV - GPIO_REL1);
  }

  if ((val >= GPIO_LED1) && (val < GPIO_LED1 + MAX_LEDS)) {
    offset = (GPIO_LED1_INV - GPIO_LED1);
  }
  if ((val >= GPIO_LED1_INV) && (val < GPIO_LED1_INV + MAX_LEDS)) {
    offset = -(GPIO_LED1_INV - GPIO_LED1);
  }

  if ((val >= GPIO_PWM1) && (val < GPIO_PWM1 + MAX_PWMS)) {
    offset = (GPIO_PWM1_INV - GPIO_PWM1);
  }
  if ((val >= GPIO_PWM1_INV) && (val < GPIO_PWM1_INV + MAX_PWMS)) {
    offset = -(GPIO_PWM1_INV - GPIO_PWM1);
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

void HandleModuleConfiguration()
{
  if (HttpUser()) {
    return;
  }
  char stemp[20];
  char line[160];
  uint8_t midx;

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MODULE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MODULE));
  page += FPSTR(HTTP_FORM_MODULE);
  snprintf_P(stemp, sizeof(stemp), kModules[MODULE].name);
  page.replace(F("{mt"), stemp);

  mytmplt cmodule;
  memcpy_P(&cmodule, &kModules[Settings.module], sizeof(cmodule));

  String func = FPSTR(HTTP_SCRIPT_MODULE1);
  for (byte i = 0; i < MAXMODULE; i++) {
    midx = pgm_read_byte(kNiceList + i);
    snprintf_P(stemp, sizeof(stemp), kModules[midx].name);
    snprintf_P(line, sizeof(line), HTTP_SCRIPT_MODULE2, midx, midx +1, stemp);
    func += line;
  }
  func += FPSTR(HTTP_SCRIPT_MODULE3);
  snprintf_P(line, sizeof(line), PSTR("sk(%d,99);o0=\""), Settings.module);  // g99
  func += line;
  for (byte j = 0; j < GPIO_SENSOR_END; j++) {
    if (!GetUsedInModule(j, cmodule.gp.io)) {
      snprintf_P(stemp, sizeof(stemp), kSensors[j]);
      snprintf_P(line, sizeof(line), HTTP_SCRIPT_MODULE2, j, j, stemp);
      func += line;
    }
  }
  func += FPSTR(HTTP_SCRIPT_MODULE3);

  page += F("<br/><table>");
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (GPIO_USER == cmodule.gp.io[i]) {
      snprintf_P(stemp, 3, PINS_WEMOS +i*2);
      snprintf_P(line, sizeof(line), PSTR("<tr><td width='190'>%s <b>" D_GPIO "%d</b> %s</td><td width='126'><select id='g%d' name='g%d'></select></td></tr>"),
        (WEMOS==Settings.module)?stemp:"", i, (0==i)? D_SENSOR_BUTTON "1":(1==i)? D_SERIAL_OUT :(3==i)? D_SERIAL_IN :(12==i)? D_SENSOR_RELAY "1":(13==i)? D_SENSOR_LED "1i":(14==i)? D_SENSOR :"", i, i);
      page += line;
      snprintf_P(line, sizeof(line), PSTR("sk(%d,%d);"), my_module.gp.io[i], i);  // g0 - g16
      func += line;
    }
  }
  page += F("</table>");

  func += F("}</script>");
  page.replace(F("</script>"), func);
  page.replace(F("<body>"), F("<body onload='sl()'>"));

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HandleWifiConfigurationWithScan()
{
  HandleWifi(true);
}

void HandleWifiConfiguration()
{
  HandleWifi(false);
}

void HandleWifi(boolean scan)
{
  if (HttpUser()) {
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_WIFI);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_WIFI));

  if (scan) {
#ifdef USE_EMULATION
    UdpDisconnect();
#endif  // USE_EMULATION
    int n = WiFi.scanNetworks();
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

    if (0 == n) {
      AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, S_NO_NETWORKS_FOUND);
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
      if (remove_duplicate_access_points) {
        String cssid;
        for (int i = 0; i < n; i++) {
          if (-1 == indices[i]) {
            continue;
          }
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              AddLog(LOG_LEVEL_DEBUG);
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
        AddLog(LOG_LEVEL_DEBUG);
        int quality = WifiGetRssiAsQuality(WiFi.RSSI(indices[i]));

        if (minimum_signal_quality == -1 || minimum_signal_quality < quality) {
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
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SKIPPING_LOW_QUALITY));
        }

      }
      page += "<br/>";
    }
  } else {
    page += FPSTR(HTTP_LNK_SCAN);
  }

  page += FPSTR(HTTP_FORM_WIFI);
  page.replace(F("{h1"), Settings.hostname);
  page.replace(F("{s1"), Settings.sta_ssid[0]);
  page.replace(F("{p1"), Settings.sta_pwd[0]);
  page.replace(F("{s2"), Settings.sta_ssid[1]);
  page.replace(F("{p2"), Settings.sta_pwd[1]);
  page += FPSTR(HTTP_FORM_END);
  if (HTTP_MANAGER == webserver_state) {
    page += FPSTR(HTTP_BTN_RSTRT);
  } else {
    page += FPSTR(HTTP_BTN_CONF);
  }
  ShowPage(page);
}

void HandleMqttConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MQTT);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MQTT));
  page += FPSTR(HTTP_FORM_MQTT);
  char str[sizeof(Settings.mqtt_client)];
  GetMqttClient(str, MQTT_CLIENT_ID, sizeof(Settings.mqtt_client));
  page.replace(F("{m0"), str);
  page.replace(F("{m1"), Settings.mqtt_host);
  page.replace(F("{m2"), String(Settings.mqtt_port));
  page.replace(F("{m3"), Settings.mqtt_client);
  page.replace(F("{m4"), (Settings.mqtt_user[0] == '\0')?"0":Settings.mqtt_user);
  page.replace(F("{m5"), (Settings.mqtt_pwd[0] == '\0')?"0":Settings.mqtt_pwd);
  page.replace(F("{m6"), Settings.mqtt_topic);
  page.replace(F("{m7"), Settings.mqtt_fulltopic);
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HandleLoggingConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_LOGGING);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_LOGGING));
  page += FPSTR(HTTP_FORM_LOG1);
  for (byte idx = 0; idx < 3; idx++) {
    page += FPSTR(HTTP_FORM_LOG2);
    switch (idx) {
    case 0:
      page.replace(F("{b0"), F(D_SERIAL " "));
      page.replace(F("{b1"), STR(SERIAL_LOG_LEVEL));
      page.replace(F("{b2"), F("ls"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == Settings.seriallog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 1:
      page.replace(F("{b0"), F(D_WEB " "));
      page.replace(F("{b1"), STR(WEB_LOG_LEVEL));
      page.replace(F("{b2"), F("lw"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == Settings.weblog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 2:
      page.replace(F("{b0"), F(D_SYS));
      page.replace(F("{b1"), STR(SYS_LOG_LEVEL));
      page.replace(F("{b2"), F("ll"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == Settings.syslog_level) ? F(" selected ") : F(" "));
      }
      break;
    }
  }
  page += FPSTR(HTTP_FORM_LOG3);
  page.replace(F("{l2"), Settings.syslog_host);
  page.replace(F("{l3"), String(Settings.syslog_port));
  page.replace(F("{l4"), String(Settings.tele_period));
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HandleOtherConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_OTHER);
  char stemp[40];

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_OTHER));
  page += FPSTR(HTTP_FORM_OTHER);
  page.replace(F("{p1"), Settings.web_password);
  page.replace(F("{r1"), (Settings.flag.mqtt_enabled) ? F(" checked") : F(""));
  page += FPSTR(HTTP_FORM_OTHER2);
  page.replace(F("{1"), F("1"));
  page.replace(F("{2"), FRIENDLY_NAME);
  page.replace(F("{3"), Settings.friendlyname[0]);
#ifdef USE_EMULATION
  page += FPSTR(HTTP_FORM_OTHER3a);
  for (byte i = 0; i < EMUL_MAX; i++) {
    page += FPSTR(HTTP_FORM_OTHER3b);
    page.replace(F("{1"), String(i));
    page.replace(F("{2"), (i == Settings.flag2.emulation) ? F(" checked") : F(""));
    page.replace(F("{3"), (i == EMUL_NONE) ? F(D_NONE) : (i == EMUL_WEMO) ? F(D_BELKIN_WEMO) : F(D_HUE_BRIDGE));
    page.replace(F("{4"), (i == EMUL_NONE) ? F("") : (i == EMUL_WEMO) ? F(" " D_SINGLE_DEVICE) : F(" " D_MULTI_DEVICE));
  }
  page += F("<br/>");
  uint8_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;
  for (byte i = 1; i < maxfn; i++) {
    page += FPSTR(HTTP_FORM_OTHER2);
    page.replace(F("{1"), String(i +1));
    snprintf_P(stemp, sizeof(stemp), PSTR(FRIENDLY_NAME"%d"), i +1);
    page.replace(F("{2"), stemp);
    page.replace(F("{3"), Settings.friendlyname[i]);
  }
  page += F("<br/></fieldset>");
#endif  // USE_EMULATION
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HandleBackupConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  uint8_t buffer[sizeof(Settings)];

  WiFiClient myClient = WebServer->client();
  WebServer->setContentLength(sizeof(buffer));

  char attachment[100];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_" VERSION_STRING ".dmp"),
    Settings.friendlyname[0]);
  WebServer->sendHeader(F("Content-Disposition"), attachment);
  WebServer->send(200, FPSTR(HDR_CTYPE_STREAM), "");
  memcpy(buffer, &Settings, sizeof(buffer));
  buffer[0] = CONFIG_FILE_SIGN;
  buffer[1] = (!CONFIG_FILE_XOR)?0:1;
  if (buffer[1]) {
    for (uint16_t i = 2; i < sizeof(buffer); i++) {
      buffer[i] ^= (CONFIG_FILE_XOR +i);
    }
  }
  myClient.write((const char*)buffer, sizeof(buffer));
}

void HandleSaveSettings()
{
  if (HttpUser()) {
    return;
  }

  char stemp[TOPSZ];
  char stemp2[TOPSZ];
  byte what = 0;
  byte restart;
  String result = "";

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_SAVE_CONFIGURATION);

  if (strlen(WebServer->arg("w").c_str())) {
    what = atoi(WebServer->arg("w").c_str());
  }
  switch (what) {
  case 1:
    strlcpy(Settings.hostname, (!strlen(WebServer->arg("h").c_str())) ? WIFI_HOSTNAME : WebServer->arg("h").c_str(), sizeof(Settings.hostname));
    if (strstr(Settings.hostname,"%")) {
      strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
    }
    strlcpy(Settings.sta_ssid[0], (!strlen(WebServer->arg("s1").c_str())) ? STA_SSID1 : WebServer->arg("s1").c_str(), sizeof(Settings.sta_ssid[0]));
    strlcpy(Settings.sta_pwd[0], (!strlen(WebServer->arg("p1").c_str())) ? STA_PASS1 : WebServer->arg("p1").c_str(), sizeof(Settings.sta_pwd[0]));
    strlcpy(Settings.sta_ssid[1], (!strlen(WebServer->arg("s2").c_str())) ? STA_SSID2 : WebServer->arg("s2").c_str(), sizeof(Settings.sta_ssid[1]));
    strlcpy(Settings.sta_pwd[1], (!strlen(WebServer->arg("p2").c_str())) ? STA_PASS2 : WebServer->arg("p2").c_str(), sizeof(Settings.sta_pwd[1]));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_PASSWORD "1 %s, " D_CMND_SSID "2 %s, " D_CMND_PASSWORD "2 %s"),
      Settings.hostname, Settings.sta_ssid[0], Settings.sta_pwd[0], Settings.sta_ssid[1], Settings.sta_pwd[1]);
    AddLog(LOG_LEVEL_INFO);
    result += F("<br/>" D_TRYING_TO_CONNECT "<br/>");
    break;
  case 2:
    strlcpy(stemp, (!strlen(WebServer->arg("mt").c_str())) ? MQTT_TOPIC : WebServer->arg("mt").c_str(), sizeof(stemp));
    MakeValidMqtt(0, stemp);
    strlcpy(stemp2, (!strlen(WebServer->arg("mf").c_str())) ? MQTT_FULLTOPIC : WebServer->arg("mf").c_str(), sizeof(stemp2));
    MakeValidMqtt(1,stemp2);
    if ((strcmp(stemp, Settings.mqtt_topic)) || (strcmp(stemp2, Settings.mqtt_fulltopic))) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), (Settings.flag.mqtt_offline) ? S_OFFLINE : "");
      MqttPublishPrefixTopic_P(2, S_LWT, true);  // Offline or remove previous retained topic
    }
    strlcpy(Settings.mqtt_topic, stemp, sizeof(Settings.mqtt_topic));
    strlcpy(Settings.mqtt_fulltopic, stemp2, sizeof(Settings.mqtt_fulltopic));
    strlcpy(Settings.mqtt_host, (!strlen(WebServer->arg("mh").c_str())) ? MQTT_HOST : (!strcmp(WebServer->arg("mh").c_str(),"0")) ? "" : WebServer->arg("mh").c_str(), sizeof(Settings.mqtt_host));
    Settings.mqtt_port = (!strlen(WebServer->arg("ml").c_str())) ? MQTT_PORT : atoi(WebServer->arg("ml").c_str());
    strlcpy(Settings.mqtt_client, (!strlen(WebServer->arg("mc").c_str())) ? MQTT_CLIENT_ID : WebServer->arg("mc").c_str(), sizeof(Settings.mqtt_client));
    strlcpy(Settings.mqtt_user, (!strlen(WebServer->arg("mu").c_str())) ? MQTT_USER : (!strcmp(WebServer->arg("mu").c_str(),"0")) ? "" : WebServer->arg("mu").c_str(), sizeof(Settings.mqtt_user));
    strlcpy(Settings.mqtt_pwd, (!strlen(WebServer->arg("mp").c_str())) ? MQTT_PASS : (!strcmp(WebServer->arg("mp").c_str(),"0")) ? "" : WebServer->arg("mp").c_str(), sizeof(Settings.mqtt_pwd));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_MQTTUSER " %s, " D_CMND_MQTTPASSWORD " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
      Settings.mqtt_host, Settings.mqtt_port, Settings.mqtt_client, Settings.mqtt_user, Settings.mqtt_pwd, Settings.mqtt_topic, Settings.mqtt_fulltopic);
    AddLog(LOG_LEVEL_INFO);
    break;
  case 3:
    Settings.seriallog_level = (!strlen(WebServer->arg("ls").c_str())) ? SERIAL_LOG_LEVEL : atoi(WebServer->arg("ls").c_str());
    Settings.weblog_level = (!strlen(WebServer->arg("lw").c_str())) ? WEB_LOG_LEVEL : atoi(WebServer->arg("lw").c_str());
    Settings.syslog_level = (!strlen(WebServer->arg("ll").c_str())) ? SYS_LOG_LEVEL : atoi(WebServer->arg("ll").c_str());
    syslog_level = Settings.syslog_level;
    syslog_timer = 0;
    strlcpy(Settings.syslog_host, (!strlen(WebServer->arg("lh").c_str())) ? SYS_LOG_HOST : WebServer->arg("lh").c_str(), sizeof(Settings.syslog_host));
    Settings.syslog_port = (!strlen(WebServer->arg("lp").c_str())) ? SYS_LOG_PORT : atoi(WebServer->arg("lp").c_str());
    Settings.tele_period = (!strlen(WebServer->arg("lt").c_str())) ? TELE_PERIOD : atoi(WebServer->arg("lt").c_str());
    if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) {
      Settings.tele_period = 10;   // Do not allow periods < 10 seconds
    }
snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
      Settings.seriallog_level, Settings.weblog_level, Settings.syslog_level, Settings.syslog_host, Settings.syslog_port, Settings.tele_period);
    AddLog(LOG_LEVEL_INFO);
    break;
#ifdef USE_DOMOTICZ
  case 4:
    DomoticzSaveSettings();
    break;
#endif  // USE_DOMOTICZ
  case 5:
    strlcpy(Settings.web_password, (!strlen(WebServer->arg("p1").c_str())) ? WEB_PASSWORD : (!strcmp(WebServer->arg("p1").c_str(),"0")) ? "" : WebServer->arg("p1").c_str(), sizeof(Settings.web_password));
    Settings.flag.mqtt_enabled = WebServer->hasArg("b1");
#ifdef USE_EMULATION
    Settings.flag2.emulation = (!strlen(WebServer->arg("b2").c_str())) ? 0 : atoi(WebServer->arg("b2").c_str());
#endif  // USE_EMULATION
    strlcpy(Settings.friendlyname[0], (!strlen(WebServer->arg("a1").c_str())) ? FRIENDLY_NAME : WebServer->arg("a1").c_str(), sizeof(Settings.friendlyname[0]));
    strlcpy(Settings.friendlyname[1], (!strlen(WebServer->arg("a2").c_str())) ? FRIENDLY_NAME"2" : WebServer->arg("a2").c_str(), sizeof(Settings.friendlyname[1]));
    strlcpy(Settings.friendlyname[2], (!strlen(WebServer->arg("a3").c_str())) ? FRIENDLY_NAME"3" : WebServer->arg("a3").c_str(), sizeof(Settings.friendlyname[2]));
    strlcpy(Settings.friendlyname[3], (!strlen(WebServer->arg("a4").c_str())) ? FRIENDLY_NAME"4" : WebServer->arg("a4").c_str(), sizeof(Settings.friendlyname[3]));
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_CMND_EMULATION " %d, " D_CMND_FRIENDLYNAME " %s, %s, %s, %s"),
      GetStateText(Settings.flag.mqtt_enabled), Settings.flag2.emulation, Settings.friendlyname[0], Settings.friendlyname[1], Settings.friendlyname[2], Settings.friendlyname[3]);
    AddLog(LOG_LEVEL_INFO);
    break;
  case 6:
    byte new_module = (!strlen(WebServer->arg("g99").c_str())) ? MODULE : atoi(WebServer->arg("g99").c_str());
    Settings.last_module = Settings.module;
    Settings.module = new_module;
    mytmplt cmodule;
    memcpy_P(&cmodule, &kModules[Settings.module], sizeof(cmodule));
    String gpios = "";
    for (byte i = 0; i < MAX_GPIO_PIN; i++) {
      if (Settings.last_module != new_module) {
        Settings.my_gp.io[i] = 0;
      } else {
        if (GPIO_USER == cmodule.gp.io[i]) {
          snprintf_P(stemp, sizeof(stemp), PSTR("g%d"), i);
          Settings.my_gp.io[i] = (!strlen(WebServer->arg(stemp).c_str())) ? 0 : atoi(WebServer->arg(stemp).c_str());
          gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(Settings.my_gp.io[i]);
        }
      }
    }
    snprintf_P(stemp, sizeof(stemp), kModules[Settings.module].name);
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), stemp, gpios.c_str());
    AddLog(LOG_LEVEL_INFO);
    break;
  }

  restart = (!strlen(WebServer->arg("r").c_str())) ? 1 : atoi(WebServer->arg("r").c_str());
  if (restart) {
    String page = FPSTR(HTTP_HEAD);
    page.replace(F("{v}"), FPSTR(S_SAVE_CONFIGURATION));
    page += F("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br/>");
    page += result;
    page += F("</div>");
    page += FPSTR(HTTP_MSG_RSTRT);
    if (HTTP_MANAGER == webserver_state) {
      webserver_state = HTTP_ADMIN;
    } else {
      page += FPSTR(HTTP_BTN_MAIN);
    }
    ShowPage(page);

    restart_flag = 2;
  } else {
    HandleConfiguration();
  }
}

void HandleResetConfiguration()
{
  if (HttpUser()) {
    return;
  }

  char svalue[16];

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESET_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESET_CONFIGURATION));
  page += F("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RESET " 1"));
  ExecuteCommand(svalue);
}

void HandleRestoreConfiguration()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTORE_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESTORE_CONFIGURATION));
  page += FPSTR(HTTP_FORM_RST);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1"), F(D_RESTORE));
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);

  upload_error = 0;
  upload_file_type = 1;
}

void HandleUpgradeFirmware()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_FIRMWARE_UPGRADE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_FIRMWARE_UPGRADE));
  page += FPSTR(HTTP_FORM_UPG);
  page.replace(F("{o1"), Settings.ota_url);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1"), F(D_UPGRADE));
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);

  upload_error = 0;
  upload_file_type = 0;
}

void HandleUpgradeFirmwareStart()
{
  if (HttpUser()) {
    return;
  }
  char svalue[100];

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  WifiConfigCounter();

  if (strlen(WebServer->arg("o").c_str())) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_OTAURL " %s"), WebServer->arg("o").c_str());
    ExecuteCommand(svalue);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += F("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_UPGRADE " 1"));
  ExecuteCommand(svalue);
}

void HandleUploadDone()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  WifiConfigCounter();
  restart_flag = 0;
  mqtt_retry_counter = 0;

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += F("<div style='text-align:center;'><b>" D_UPLOAD " <font color='");
  if (upload_error) {
    page += F("red'>" D_FAILED "</font></b><br/><br/>");
    switch (upload_error) {
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
        snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), upload_error);
    }
    page += error;
    snprintf_P(log_data, sizeof(log_data), PSTR(D_UPLOAD ": %s"), error);
    AddLog(LOG_LEVEL_DEBUG);
    stop_flash_rotate = Settings.flag.stop_flash_rotate;
  } else {
    page += F("green'>" D_SUCCESSFUL "</font></b><br/>");
    page += FPSTR(HTTP_MSG_RSTRT);
    restart_flag = 2;
  }
  page += F("</div><br/>");
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

void HandleUploadLoop()
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  boolean _serialoutput = (LOG_LEVEL_DEBUG <= seriallog_level);

  if (HTTP_USER == webserver_state) {
    return;
  }
  if (upload_error) {
    if (!upload_file_type) {
      Update.end();
    }
    return;
  }

  HTTPUpload& upload = WebServer->upload();

  if (UPLOAD_FILE_START == upload.status) {
    restart_flag = 60;
    if (0 == upload.filename.c_str()[0]) {
      upload_error = 1;
      return;
    }
    SettingsSave(1);  // Free flash for upload
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    AddLog(LOG_LEVEL_INFO);
    if (!upload_file_type) {
      mqtt_retry_counter = 60;
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
      if (Settings.flag.mqtt_enabled) {
        MqttClient.disconnect();
      }
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {         //start with max available size
        upload_error = 2;
        return;
      }
    }
    upload_progress_dot_count = 0;
  } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
    if (0 == upload.totalSize)
    {
      if (upload_file_type) {
        if (upload.buf[0] != CONFIG_FILE_SIGN) {
          upload_error = 8;
          return;
        }
        if (upload.currentSize > sizeof(Settings)) {
          upload_error = 9;
          return;
        }
      } else {
        if (upload.buf[0] != 0xE9) {
          upload_error = 3;
          return;
        }
        uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
        if(bin_flash_size > ESP.getFlashChipRealSize()) {
          upload_error = 4;
          return;
        }
        upload.buf[2] = 3;  // Force DOUT - ESP8285
      }
    }
    if (upload_file_type) { // config
      if (!upload_error) {
        if (upload.buf[1]) {
          for (uint16_t i = 2; i < upload.currentSize; i++) {
            upload.buf[i] ^= (CONFIG_FILE_XOR +i);
          }
        }
        SettingsDefaultSet2();
        memcpy((char*)&Settings +16, upload.buf +16, upload.currentSize -16);
        memcpy((char*)&Settings +8, upload.buf +8, 4);  // Restore version and auto upgrade
      }
    } else {  // firmware
      if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
        upload_error = 5;
        return;
      }
      if (_serialoutput) {
        Serial.printf(".");
        upload_progress_dot_count++;
        if (!(upload_progress_dot_count % 80)) {
          Serial.println();
        }
      }
    }
  } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
    if (_serialoutput && (upload_progress_dot_count % 80)) {
      Serial.println();
    }
    if (!upload_file_type) {
      if (!Update.end(true)) { // true to set the size to the current progress
        if (_serialoutput) {
          Update.printError(Serial);
        }
        upload_error = 6;
        return;
      }
    }
    if (!upload_error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
      AddLog(LOG_LEVEL_INFO);
    }
  } else if (UPLOAD_FILE_ABORTED == upload.status) {
    restart_flag = 0;
    mqtt_retry_counter = 0;
    upload_error = 7;
    if (!upload_file_type) {
      Update.end();
    }
  }
  delay(0);
}

void HandleHttpCommand()
{
  if (HttpUser()) {
    return;
  }
  char svalue[INPUT_BUFFER_SIZE];  // big to serve Backlog

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  uint8_t valid = 1;
  if (Settings.web_password[0] != 0) {
    if (!(!strcmp(WebServer->arg("user").c_str(),WEB_USERNAME) && !strcmp(WebServer->arg("password").c_str(),Settings.web_password))) {
      valid = 0;
    }
  }

  String message = "";
  if (valid) {
    byte curridx = web_log_index;
    if (strlen(WebServer->arg("cmnd").c_str())) {
//      snprintf_P(svalue, sizeof(svalue), WebServer->arg("cmnd").c_str());  // Processes FullTopic %p
      strlcpy(svalue, WebServer->arg("cmnd").c_str(), sizeof(svalue));       // Fixed 5.8.0b
//      byte syslog_now = syslog_level;
//      syslog_level = 0;  // Disable UDP syslog to not trigger hardware WDT - Seems to work fine since 5.7.1d (global logging)
      ExecuteCommand(svalue);
//      syslog_level = syslog_now;
    }

    if (web_log_index != curridx) {
      byte counter = curridx;
      do {
        if (web_log[counter].length()) {
          if (message.length()) {
            message += F("\n");
          }
          if (Settings.flag.mqtt_enabled) {
            // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
//            message += web_log[counter].substring(17 + strlen(PUB_PREFIX) + strlen(Settings.mqtt_topic));
            message += web_log[counter].substring(web_log[counter].lastIndexOf("/",web_log[counter].indexOf("="))+1);
          } else {
            // [14:49:36 RSLT: RESULT = {"POWER":"OFF"}] > [RESULT = {"POWER":"OFF"}]
            message += web_log[counter].substring(web_log[counter].indexOf(": ")+2);
          }
        }
        counter++;
        if (counter > MAX_LOG_LINES -1) {
          counter = 0;
        }
      } while (counter != web_log_index);
    } else {
      message = F(D_ENABLE_WEBLOG_FOR_RESPONSE "\n");
    }
  } else {
    message = F(D_NEED_USER_AND_PASSWORD "\n");
  }
  WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), message);
}

void HandleConsole()
{
  if (HttpUser()) {
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONSOLE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONSOLE));
  page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_CONSOL));
  page.replace(F("<body>"), F("<body onload='l()'>"));
  page += FPSTR(HTTP_FORM_CMND);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

void HandleAjaxConsoleRefresh()
{
  if (HttpUser()) {
    return;
  }
  char svalue[INPUT_BUFFER_SIZE];  // big to serve Backlog
  byte cflg = 1;
  byte counter = 99;

  if (strlen(WebServer->arg("c1").c_str())) {
//    snprintf_P(svalue, sizeof(svalue), WebServer->arg("c1").c_str());  // Processes FullTopic %p
    strlcpy(svalue, WebServer->arg("c1").c_str(), sizeof(svalue));       // Fixed 5.8.0b
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_COMMAND "%s"), svalue);
    AddLog(LOG_LEVEL_INFO);
//    byte syslog_now = syslog_level;
//    syslog_level = 0;  // Disable UDP syslog to not trigger hardware WDT - Seems to work fine since 5.7.1d (global logging)
    ExecuteCommand(svalue);
//    syslog_level = syslog_now;
  }

  if (strlen(WebServer->arg("c2").c_str())) {
    counter = atoi(WebServer->arg("c2").c_str());
  }

  String message = F("<r><i>");
  message += String(web_log_index);
  message += F("</i><j>");
  message += String(reset_web_log_flag);
  if (!reset_web_log_flag) {
    counter = 99;
    reset_web_log_flag = 1;
  }
  message += F("</j><l>");
  if (counter != web_log_index) {
    if (99 == counter) {
      counter = web_log_index;
      cflg = 0;
    }
    do {
      if (web_log[counter].length()) {
        if (cflg) {
          message += F("\n");
        } else {
          cflg = 1;
        }
        String nextline = web_log[counter];
        nextline.replace(F("<"), F("%3C"));  // XML encoding to fix blank console log in concert with javascript decodeURIComponent
        nextline.replace(F(">"), F("%3E"));
        nextline.replace(F("&"), F("%26"));
        message += nextline;
      }
      counter++;
      if (counter > MAX_LOG_LINES -1) {
        counter = 0;
      }
    } while (counter != web_log_index);
  }
  message += F("</l></r>");
  WebServer->send(200, FPSTR(HDR_CTYPE_XML), message);
}

void HandleInformation()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_INFORMATION);

  char stopic[TOPSZ];

  int freeMem = ESP.getFreeHeap();

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  //  page += F("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>");

  page += F("<style>td{padding:0px 5px;}</style>");
  page += F("<div id='i' name='i'></div>");

  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  String func = FPSTR(HTTP_SCRIPT_INFO_BEGIN);
  func += F("<table style'width:100%;'><tr><th>");
  func += F(D_PROGRAM_VERSION "}2" VERSION_STRING);
  func += F("}1" D_BUILD_DATE_AND_TIME "}2"); func += GetBuildDateAndTime();
  func += F("}1" D_CORE_AND_SDK_VERSION "}2"); func += ESP.getCoreVersion(); func += F("/"); func += String(ESP.getSdkVersion());
  func += F("}1" D_UPTIME "}2"); func += String(uptime); func += F(" Hours");
  snprintf_P(stopic, sizeof(stopic), PSTR(" at %X"), GetSettingsAddress());
  func += F("}1" D_FLASH_WRITE_COUNT "}2"); func += String(Settings.save_flag); func += stopic;
  func += F("}1" D_BOOT_COUNT "}2"); func += String(Settings.bootcount);
  func += F("}1" D_RESTART_REASON "}2"); func += GetResetReason();
  uint8_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;
  for (byte i = 0; i < maxfn; i++) {
    func += F("}1" D_FRIENDLY_NAME " "); func += i +1; func += F("}2"); func += Settings.friendlyname[i];
  }

  func += F("}1}2&nbsp;");  // Empty line
  func += F("}1" D_AP); func += String(Settings.sta_active +1);
    func += F(" " D_SSID " (" D_RSSI ")}2"); func += Settings.sta_ssid[Settings.sta_active]; func += F(" ("); func += WifiGetRssiAsQuality(WiFi.RSSI()); func += F("%)");
  func += F("}1" D_HOSTNAME "}2"); func += my_hostname;
  if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
    func += F("}1" D_IP_ADDRESS "}2"); func += WiFi.localIP().toString();
    func += F("}1" D_GATEWAY "}2"); func += IPAddress(Settings.ip_address[1]).toString();
    func += F("}1" D_SUBNET_MASK "}2"); func += IPAddress(Settings.ip_address[2]).toString();
    func += F("}1" D_DNS_SERVER "}2"); func += IPAddress(Settings.ip_address[3]).toString();
    func += F("}1" D_MAC_ADDRESS "}2"); func += WiFi.macAddress();
  }
  if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
    func += F("}1" D_AP " " D_IP_ADDRESS "}2"); func += WiFi.softAPIP().toString();
    func += F("}1" D_AP " " D_GATEWAY "}2"); func += WiFi.softAPIP().toString();
    func += F("}1" D_AP " " D_MAC_ADDRESS "}2"); func += WiFi.softAPmacAddress();
  }

  func += F("}1}2&nbsp;");  // Empty line
  if (Settings.flag.mqtt_enabled) {
    func += F("}1" D_MQTT_HOST "}2"); func += Settings.mqtt_host;
    func += F("}1" D_MQTT_PORT "}2"); func += String(Settings.mqtt_port);
    func += F("}1" D_MQTT_CLIENT " &<br/>&nbsp;" D_FALLBACK_TOPIC "}2"); func += mqtt_client;
    func += F("}1" D_MQTT_USER "}2"); func += Settings.mqtt_user;
    func += F("}1" D_MQTT_TOPIC "}2"); func += Settings.mqtt_topic;
    func += F("}1" D_MQTT_GROUP_TOPIC "}2"); func += Settings.mqtt_grptopic;
    GetTopic_P(stopic, 0, Settings.mqtt_topic, "");
    func += F("}1" D_MQTT_FULL_TOPIC "}2"); func += stopic;

  } else {
    func += F("}1" D_MQTT "}2" D_DISABLED);
  }

  func += F("}1}2&nbsp;");  // Empty line
  func += F("}1" D_EMULATION "}2");
#ifdef USE_EMULATION
  if (EMUL_WEMO == Settings.flag2.emulation) {
    func += F(D_BELKIN_WEMO);
  }
  else if (EMUL_HUE == Settings.flag2.emulation) {
    func += F(D_HUE_BRIDGE);
  }
  else {
    func += F(D_NONE);
  }
#else
  func += F(D_DISABLED);
#endif // USE_EMULATION

  func += F("}1" D_MDNS_DISCOVERY "}2");
#ifdef USE_DISCOVERY
  func += F(D_ENABLED);
  func += F("}1" D_MDNS_ADVERTISE "}2");
#ifdef WEBSERVER_ADVERTISE
  func += F(D_WEB_SERVER);
#else
  func += F(D_DISABLED);
#endif // WEBSERVER_ADVERTISE
#else
  func += F(D_DISABLED);
#endif // USE_DISCOVERY

  func += F("}1}2&nbsp;");  // Empty line
  func += F("}1" D_ESP_CHIP_ID "}2"); func += String(ESP.getChipId());
  func += F("}1" D_FLASH_CHIP_ID "}2"); func += String(ESP.getFlashChipId());
  func += F("}1" D_FLASH_CHIP_SIZE "}2"); func += String(ESP.getFlashChipRealSize() / 1024); func += F("kB");
  func += F("}1" D_PROGRAM_FLASH_SIZE "}2"); func += String(ESP.getFlashChipSize() / 1024); func += F("kB");
  func += F("}1" D_PROGRAM_SIZE "}2"); func += String(ESP.getSketchSize() / 1024); func += F("kB");
  func += F("}1" D_FREE_PROGRAM_SPACE "}2"); func += String(ESP.getFreeSketchSpace() / 1024); func += F("kB");
  func += F("}1" D_FREE_MEMORY "}2"); func += String(freeMem / 1024); func += F("kB");
  func += F("</td></tr></table>");
  func += FPSTR(HTTP_SCRIPT_INFO_END);
  page.replace(F("</script>"), func);
  page.replace(F("<body>"), F("<body onload='i()'>"));

  //  page += F("</fieldset>");
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

void HandleRestart()
{
  if (HttpUser()) {
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTART);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESTART));
  page += FPSTR(HTTP_MSG_RSTRT);
  if (HTTP_MANAGER == webserver_state) {
    webserver_state = HTTP_ADMIN;
  } else {
    page += FPSTR(HTTP_BTN_MAIN);
  }
  ShowPage(page);

  restart_flag = 2;
}

/********************************************************************************************/

void HandleNotFound()
{
  if (CaptivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }

#ifdef USE_EMULATION
  String path = WebServer->uri();
  if ((EMUL_HUE == Settings.flag2.emulation) && (path.startsWith("/api"))) {
    HandleHueApi(&path);
  } else
#endif // USE_EMULATION
  {
    String message = F(D_FILE_NOT_FOUND "\n\nURI: ");
    message += WebServer->uri();
    message += F("\nMethod: ");
    message += (WebServer->method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += WebServer->args();
    message += F("\n");
    for ( uint8_t i = 0; i < WebServer->args(); i++ ) {
      message += " " + WebServer->argName(i) + ": " + WebServer->arg(i) + "\n";
    }
    SetHeader();
    WebServer->send(404, FPSTR(HDR_CTYPE_PLAIN), message);
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean CaptivePortal()
{
  if ((HTTP_MANAGER == webserver_state) && !ValidIpAddress(WebServer->hostHeader())) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    WebServer->sendHeader(F("Location"), String("http://") + WebServer->client().localIP().toString(), true);
    WebServer->send(302, FPSTR(HDR_CTYPE_PLAIN), ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    WebServer->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Is this an IP? */
boolean ValidIpAddress(String str)
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
