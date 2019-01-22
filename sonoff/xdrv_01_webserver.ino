/*
  xdrv_01_webserver.ino - webserver for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#define XDRV_01                               1

#ifndef WIFI_SOFT_AP_CHANNEL
#define WIFI_SOFT_AP_CHANNEL                  1   // Soft Access Point Channel number between 1 and 11 as used by SmartConfig web GUI
#endif

#define HTTP_REFRESH_TIME                  2345   // milliseconds
#define HTTP_RESTART_RECONNECT_TIME        9000   // milliseconds
#define HTTP_OTA_RESTART_RECONNECT_TIME   20000   // milliseconds

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#ifdef USE_RF_FLASH
uint8_t *efm8bb1_update = NULL;
#endif  // USE_RF_FLASH

enum UploadTypes { UPL_TASMOTA, UPL_SETTINGS, UPL_EFM8BB1 };

const char HTTP_HEAD[] PROGMEM =
  "<!DOCTYPE html><html lang=\"" D_HTML_LANGUAGE "\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<title>{h} - {v}</title>"

  "<script>"
  "var x=null,lt,to,tp,pc='';"   // x=null allow for abortion
  "function eb(s){"
    "return document.getElementById(s);"  // Save code space
  "}";

const char HTTP_SCRIPT_COUNTER[] PROGMEM =
  "var cn=180;"                  // seconds
  "function u(){"
    "if(cn>=0){"
      "eb('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "</script>";

const char HTTP_SCRIPT_ROOT[] PROGMEM =
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
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "x.open('GET','ay'+a,true);"
    "x.send();"
    "lt=setTimeout(la,{a});"    // Settings.web_refresh
  "}"
  "function lb(p){"
    "la('?d='+p);"              // ?d related to WebGetArg("d", tmp, sizeof(tmp));
  "}"
  "function lc(p){"
    "la('?t='+p);"              // ?t related to WebGetArg("t", tmp, sizeof(tmp));
  "}";

const char HTTP_SCRIPT_WIFI[] PROGMEM =
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}";

const char HTTP_SCRIPT_RELOAD[] PROGMEM =
  "setTimeout(function(){location.href='.';}," STR(HTTP_RESTART_RECONNECT_TIME) ");"
  "</script>";

// Local OTA upgrade requires more time to complete cp: before web ui should be reloaded
const char HTTP_SCRIPT_RELOAD_OTA[] PROGMEM =
  "setTimeout(function(){location.href='.';}," STR(HTTP_OTA_RESTART_RECONNECT_TIME) ");"
  "</script>";

const char HTTP_SCRIPT_CONSOL[] PROGMEM =
  "var sn=0;"                    // Scroll position
  "var id=0;"                    // Get most of weblog initially
  "function l(p){"               // Console log and command service
    "var c,o,t;"
    "clearTimeout(lt);"
    "o='';"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"
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
    "lt=setTimeout(l,{a});"
    "return false;"
  "}"
  "</script>";

const char HTTP_SCRIPT_MODULE1[] PROGMEM =
  "var os;"
  "function sk(s,g){"            // s = value, g = id and name
    "var o=os.replace(\"value='\"+s+\"'\",\"selected value='\"+s+\"'\");"
    "eb('g'+g).innerHTML=o;"
  "}"
  "function sl(){"
    "if(x!=null){x.abort();}"    // Abort any request pending
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var i,o=x.responseText.replace(/}1/g,\"<option value=\").replace(/}2/g,\"</option>\");"
        "i=o.indexOf(\"}3\");"   // String separator means do not use "}3" in Module name and Sensor name
        "os=o.substring(0,i);"
        "sk(}4,99);"
        "os=o.substring(i+2);";  // +2 is length "}3"
const char HTTP_SCRIPT_MODULE2[] PROGMEM =
      "}"
    "};"
    "x.open('GET','md?m=1',true);"  // ?m related to WebServer->hasArg("m")
    "x.send();"
  "}";
const char HTTP_SCRIPT_MODULE3[] PROGMEM =
  "}1'%d'>%s (%02d)}2";            // "}1" and "}2" means do not use "}x" in Module name and Sensor name

const char HTTP_SCRIPT_INFO_BEGIN[] PROGMEM =
  "function i(){"
    "var s,o=\"";
const char HTTP_SCRIPT_INFO_END[] PROGMEM =
    "\";"                        // "}1" and "}2" means do not use "}x" in Information text
    "s=o.replace(/}1/g,\"</td></tr><tr><th>\").replace(/}2/g,\"</th><td>\");"
    "eb('i').innerHTML=s;"
  "}"
  "</script>";

const char HTTP_HEAD_STYLE[] PROGMEM =
  "</script>"

  "<style>"
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;}"
  "select{width:100%;}"
  "textarea{resize:none;width:98%;height:318px;padding:5px;overflow:auto;}"
  "body{text-align:center;font-family:verdana;}"
  "td{padding:0px;}"
  "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"
  "button:hover{background-color:#0e70a4;}"
  ".bred{background-color:#d43535;}"
  ".bred:hover{background-color:#931f1f;}"
  ".bgrn{background-color:#47c266;}"
  ".bgrn:hover{background-color:#5aaf6f;}"
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
  "<div style='text-align:center;'><noscript>" D_NOSCRIPT "<br/></noscript>"
#ifdef LANGUAGE_MODULE_NAME
  "<h3>" D_MODULE " {ha</h3>"
#else
  "<h3>{ha " D_MODULE "</h3>"
#endif
  "<h2>{h}</h2>{j}</div>";
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
  "<br/><form action='.' method='get' onsubmit='return confirm(\"" D_CONFIRM_RESTART "\");'><button name='rstrt' class='button bred'>" D_RESTART "</button></form>";
const char HTTP_BTN_MENU_MODULE[] PROGMEM =
  "<br/><form action='md' method='get'><button>" D_CONFIGURE_MODULE "</button></form>"
  "<br/><form action='wi' method='get'><button>" D_CONFIGURE_WIFI "</button></form>";
const char HTTP_BTN_MENU4[] PROGMEM =
  "<br/><form action='lg' method='get'><button>" D_CONFIGURE_LOGGING "</button></form>"
  "<br/><form action='co' method='get'><button>" D_CONFIGURE_OTHER "</button></form>"
  "<br/>"
  "<br/><form action='rt' method='get' onsubmit='return confirm(\"" D_CONFIRM_RESET_CONFIGURATION "\");'><button class='button bred'>" D_RESET_CONFIGURATION "</button></form>"
  "<br/><form action='dl' method='get'><button>" D_BACKUP_CONFIGURATION "</button></form>"
  "<br/><form action='rs' method='get'><button>" D_RESTORE_CONFIGURATION "</button></form>";
const char HTTP_BTN_MAIN[] PROGMEM =
  "<br/><br/><form action='.' method='get'><button>" D_MAIN_MENU "</button></form>";
const char HTTP_FORM_LOGIN[] PROGMEM =
  "<form method='post' action='/'>"
  "<br/><b>" D_USER "</b><br/><input name='USER1' placeholder='" D_USER "'><br/>"
  "<br/><b>" D_PASSWORD "</b><br/><input name='PASS1' type='password' placeholder='" D_PASSWORD "'><br/>"
  "<br/>"
  "<br/><button>" D_OK "</button></form>";
const char HTTP_BTN_CONF[] PROGMEM =
  "<br/><br/><form action='cn' method='get'><button>" D_CONFIGURATION "</button></form>";
const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend><form method='get' action='md'>"
  "<br/><b>" D_MODULE_TYPE "</b> ({mt)<br/><select id='g99' name='g99'></select><br/>";
const char HTTP_LNK_ITEM[] PROGMEM =
  "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;({w})&nbsp<span class='q'>{i} {r}%</span></div>";
const char HTTP_LNK_SCAN[] PROGMEM =
  "<div><a href='/wi?scan='>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br/>";
const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend><form method='get' action='wi'>"
  "<br/><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br/><input id='s1' name='s1' placeholder='" STA_SSID1 "' value='{s1'><br/>"
  "<br/><b>" D_AP1_PASSWORD "</b><br/><input id='p1' name='p1' type='password' placeholder='" D_AP1_PASSWORD "' value='" D_ASTERIX "'><br/>"
  "<br/><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br/><input id='s2' name='s2' placeholder='" STA_SSID2 "' value='{s2'><br/>"
  "<br/><b>" D_AP2_PASSWORD "</b><br/><input id='p2' name='p2' type='password' placeholder='" D_AP2_PASSWORD "' value='" D_ASTERIX "'><br/>"
  "<br/><b>" D_HOSTNAME "</b> (" WIFI_HOSTNAME ")<br/><input id='h' name='h' placeholder='" WIFI_HOSTNAME" ' value='{h1'><br/>";
const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b></legend><form method='get' action='lg'>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<br/><b>{b0</b> ({b1)<br/><select id='{b2' name='{b2'>"
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
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend><form method='get' action='co'>"
//  "<input id='w' name='w' value='5,1' hidden>"
  "<br/><b>" D_WEB_ADMIN_PASSWORD "</b><br/><input id='p1' name='p1' type='password' placeholder='" D_WEB_ADMIN_PASSWORD "' value='" D_ASTERIX "'><br/>"
  "<br/><input style='width:10%;' id='b1' name='b1' type='checkbox'{r1><b>" D_MQTT_ENABLE "</b><br/>";
  const char HTTP_FORM_OTHER2[] PROGMEM =
  "<br/><b>" D_FRIENDLY_NAME " {1</b> ({2)<br/><input id='a{1' name='a{1' placeholder='{2' value='{3'><br/>";
#ifdef USE_EMULATION
const char HTTP_FORM_OTHER3a[] PROGMEM =
  "<br/><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend>";
const char HTTP_FORM_OTHER3b[] PROGMEM =
  "<br/><input style='width:10%;' id='r{1' name='b2' type='radio' value='{1'{2><b>{3</b>{4";  // Different id only used for labels
#endif  // USE_EMULATION
const char HTTP_FORM_END[] PROGMEM =
  "<br/><button name='save' type='submit' class='button bgrn'>" D_SAVE "</button></form></fieldset>";
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
  "<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " {r1</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char HTTP_FORM_CMND[] PROGMEM =
  "<br/><textarea readonly id='t1' name='t1' cols='340' wrap='off'></textarea><br/><br/>"
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
  "<div style='text-align:right;font-size:11px;'><hr/><a href='" D_WEBLINK "' target='_blank' style='color:#aaa;'>" D_PROGRAMNAME " {mv " D_BY " " D_AUTHOR "</a></div>"
  "</div>"
  "</body>"
  "</html>";

const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"?o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg("o", tmp, sizeof(tmp));
const char HTTP_DEVICE_STATE[] PROGMEM = "%s<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:

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
uint8_t config_block_count = 0;
uint8_t config_xor_on = 0;
uint8_t config_xor_on_set = CONFIG_FILE_XOR;

// Helper function to avoid code duplication (saves 4k Flash)
static void WebGetArg(const char* arg, char* out, size_t max)
{
  String s = WebServer->arg(arg);
  strlcpy(out, s.c_str(), max);
//  out[max-1] = '\0';  // Ensure terminating NUL
}

void ShowWebSource(int source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    snprintf_P(log_data, sizeof(log_data), PSTR("SRC: %s from %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource), WebServer->client().remoteIP().toString().c_str());
    AddLog(LOG_LEVEL_DEBUG);
  }
}

void ExecuteWebCommand(char* svalue, int source)
{
  ShowWebSource(source);
  ExecuteCommand(svalue, SRC_IGNORE);
}

void StartWebserver(int type, IPAddress ipweb)
{
  if (!Settings.web_refresh) { Settings.web_refresh = HTTP_REFRESH_TIME; }
  if (!webserver_state) {
    if (!WebServer) {
      WebServer = new ESP8266WebServer((HTTP_MANAGER==type) ? 80 : WEB_PORT);
      WebServer->on("/", HandleRoot);
      WebServer->on("/up", HandleUpgradeFirmware);
      WebServer->on("/u1", HandleUpgradeFirmwareStart);  // OTA
      WebServer->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);
      WebServer->on("/u2", HTTP_OPTIONS, HandlePreflightRequest);
      WebServer->on("/cs", HandleConsole);
      WebServer->on("/ax", HandleAjaxConsoleRefresh);
      WebServer->on("/ay", HandleAjaxStatusRefresh);
      WebServer->on("/cm", HandleHttpCommand);
      WebServer->onNotFound(HandleNotFound);
#ifndef BE_MINIMAL
      WebServer->on("/cn", HandleConfiguration);
      WebServer->on("/md", HandleModuleConfiguration);
      WebServer->on("/wi", HandleWifiConfiguration);
      WebServer->on("/lg", HandleLoggingConfiguration);
      WebServer->on("/co", HandleOtherConfiguration);
      WebServer->on("/dl", HandleBackupConfiguration);
      WebServer->on("/rs", HandleRestoreConfiguration);
      WebServer->on("/rt", HandleResetConfiguration);
      WebServer->on("/in", HandleInformation);
#ifdef USE_EMULATION
      HueWemoAddHandlers();
#endif  // USE_EMULATION
      XdrvCall(FUNC_WEB_ADD_HANDLER);
      XsnsCall(FUNC_WEB_ADD_HANDLER);
#endif  // Not BE_MINIMAL
    }
    reset_web_log_flag = 0;
    WebServer->begin(); // Web server start
  }
  if (webserver_state != type) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"),
      my_hostname, (mdns_begun) ? ".local" : "", ipweb.toString().c_str());
    AddLog(LOG_LEVEL_INFO);
  }
  if (type) { webserver_state = type; }
}

void StopWebserver(void)
{
  if (webserver_state) {
    WebServer->close();
    webserver_state = HTTP_OFF;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

void WifiManagerBegin(void)
{
  // setup AP
  if (!global_state.wifi_down) {
    WiFi.mode(WIFI_AP_STA);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
    WiFi.mode(WIFI_AP);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
  }

  StopWebserver();

  DnsServer = new DNSServer();

  int channel = WIFI_SOFT_AP_CHANNEL;
  if ((channel < 1) || (channel > 13)) { channel = 1; }
  WiFi.softAP(my_hostname, NULL, channel);

  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  StartWebserver(HTTP_MANAGER, WiFi.softAPIP());
}

void PollDnsWebserver(void)
{
  if (DnsServer) { DnsServer->processNextRequest(); }
  if (WebServer) { WebServer->handleClient(); }
}

/*********************************************************************************************/

void SetHeader(void)
{
  WebServer->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  WebServer->sendHeader(F("Pragma"), F("no-cache"));
  WebServer->sendHeader(F("Expires"), F("-1"));
#ifndef ARDUINO_ESP8266_RELEASE_2_3_0
  WebServer->sendHeader(F("Access-Control-Allow-Origin"), F("*"));
#endif
}

bool WebAuthenticate(void)
{
  if (Settings.web_password[0] != 0) {
    return WebServer->authenticate(WEB_USERNAME, Settings.web_password);
  } else {
    return true;
  }
}

void ShowPage(String &page, bool auth)
{
  if (auth && (Settings.web_password[0] != 0) && !WebServer->authenticate(WEB_USERNAME, Settings.web_password)) {
    return WebServer->requestAuthentication();
  }

  page.replace(F("{a}"), String(Settings.web_refresh));
  page.replace(F("{ha"), my_module.name);
  page.replace(F("{h}"), Settings.friendlyname[0]);

  String info = "";
  if (Settings.flag3.gui_hostname_ip) {
    uint8_t more_ips = 0;
    info += F("<h3>"); info += my_hostname;
    if (mdns_begun) { info += F(".local"); }
    info += F(" (");
    if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
      info += WiFi.localIP().toString();
      more_ips++;
    }
    if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
      if (more_ips) { info += F(", "); }
      info += WiFi.softAPIP().toString();
    }
    info += F(")</h3>");
  }
  page.replace(F("{j}"), info);

  if (HTTP_MANAGER == webserver_state) {
    if (WifiConfigCounter()) {
      page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_COUNTER));
      page.replace(F("<body>"), F("<body onload='u()'>"));
      page += FPSTR(HTTP_COUNTER);
    }
  }
  page += FPSTR(HTTP_END);
  page.replace(F("{mv"), my_version);
  SetHeader();

  ShowFreeMem(PSTR("ShowPage"));

  WebServer->send(200, FPSTR(HDR_CTYPE_HTML), page);
}

void ShowPage(String &page)
{
  ShowPage(page, true);
}

/*-------------------------------------------------------------------------------------------*/

void WebRestart(uint8_t type)
{
  // type 0 = restart
  // type 1 = restart after config change
  // type 2 = restart after config change with possible ip address change too
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTART);

  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_HEAD_STYLE);

  if (type) {
    page.replace(F("{v}"), FPSTR(S_SAVE_CONFIGURATION));
    page += F("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br/>");
    if (2 == type) {
      page += F("<br/>" D_TRYING_TO_CONNECT "<br/>");
    }
    page += F("</div>");
  }
  else {
    page.replace(F("{v}"), FPSTR(S_RESTART));
  }

  page += FPSTR(HTTP_MSG_RSTRT);
  if (HTTP_MANAGER == webserver_state) {
    webserver_state = HTTP_ADMIN;
  } else {
    page += FPSTR(HTTP_BTN_MAIN);
  }
  page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_RELOAD));
  ShowPage(page);

  ShowWebSource(SRC_WEBGUI);
  restart_flag = 2;
}

/*********************************************************************************************/

void HandleWifiLogin(void)
{
  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR( D_CONFIGURE_WIFI ));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_LOGIN);
  ShowPage(page, false);  // false means show page no matter if the client has or has not credentials
}

void HandleRoot(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_MAIN_MENU);

  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the page.

  if ( WebServer->hasArg("rstrt") ) {
    WebRestart(0);
    return;
  }

  if (HTTP_MANAGER == webserver_state) {
#ifndef BE_MINIMAL
    if ((Settings.web_password[0] != 0) && !(WebServer->hasArg("USER1")) && !(WebServer->hasArg("PASS1"))) {
      HandleWifiLogin();
    } else {
/*
      char tmp1[100];
      WebGetArg("USER1", tmp1, sizeof(tmp1));
      char tmp2[100];
      WebGetArg("PASS1", tmp2, sizeof(tmp2));
      if (!(Settings.web_password[0] != 0) || (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, Settings.web_password)))) {
*/
      if (!(Settings.web_password[0] != 0) || ((WebServer->arg("USER1") == WEB_USERNAME ) && (WebServer->arg("PASS1") == Settings.web_password ))) {
        HandleWifiConfiguration();
      } else {
        // wrong user and pass
        HandleWifiLogin();
      }
    }
#endif  // Not BE_MINIMAL
  } else {
    char stemp[10];
    String page = FPSTR(HTTP_HEAD);
    page.replace(F("{v}"), FPSTR(S_MAIN_MENU));
    page += FPSTR(HTTP_SCRIPT_ROOT);
    page += FPSTR(HTTP_HEAD_STYLE);
    page.replace(F("<body>"), F("<body onload='la()'>"));

    page += F("<div id='l1' name='l1'></div>");
    if (devices_present) {
      if (light_type) {
        if ((LST_COLDWARM == (light_type &7)) || (LST_RGBWC == (light_type &7))) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MSG_SLIDER1, LightGetColorTemp());
          page += mqtt_data;
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MSG_SLIDER2, Settings.light_dimmer);
        page += mqtt_data;
      }
      page += FPSTR(HTTP_TABLE100);
      page += F("<tr>");
      if (SONOFF_IFAN02 == Settings.module) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_CONTROL, 36, 1, D_BUTTON_TOGGLE, "");
        page += mqtt_data;
        for (byte i = 0; i < MAX_FAN_SPEED; i++) {
          snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i);
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_CONTROL, 16, i +2, stemp, "");
          page += mqtt_data;
        }
      } else {
        for (byte idx = 1; idx <= devices_present; idx++) {
          snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_CONTROL,
            100 / devices_present, idx, (devices_present < 5) ? D_BUTTON_TOGGLE : "", (devices_present > 1) ? stemp : "");
          page += mqtt_data;
        }
      }
      page += F("</tr></table>");
    }
    if (SONOFF_BRIDGE == Settings.module) {
      page += FPSTR(HTTP_TABLE100);
      page += F("<tr>");
      byte idx = 0;
      for (byte i = 0; i < 4; i++) {
        if (idx > 0) { page += F("</tr><tr>"); }
        for (byte j = 0; j < 4; j++) {
          idx++;
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("<td style='width:25%'><button onclick='la(\"?k=%d\");'>%d</button></td>"), idx, idx);  // ?k is related to WebGetArg("k", tmp, sizeof(tmp));
          page += mqtt_data;
        }
      }
      page += F("</tr></table>");
    }

#ifndef BE_MINIMAL
    mqtt_data[0] = '\0';
    XdrvCall(FUNC_WEB_ADD_MAIN_BUTTON);
    XsnsCall(FUNC_WEB_ADD_MAIN_BUTTON);
    page += String(mqtt_data);
#endif  // Not BE_MINIMAL

    if (HTTP_ADMIN == webserver_state) {
      page += FPSTR(HTTP_BTN_MENU1);
      page += FPSTR(HTTP_BTN_RSTRT);
    }
    ShowPage(page);
  }
}

void HandleAjaxStatusRefresh(void)
{
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }

  char svalue[80];
  char tmp[100];

  WebGetArg("o", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    ShowWebSource(SRC_WEBGUI);
    uint8_t device = atoi(tmp);
    if (SONOFF_IFAN02 == Settings.module) {
      if (device < 2) {
        ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), device -2);
        ExecuteCommand(svalue, SRC_WEBGUI);
      }
    } else {
      ExecuteCommandPower(device, POWER_TOGGLE, SRC_IGNORE);
    }
  }
  WebGetArg("d", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_DIMMER " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("t", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_COLORTEMPERATURE " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("k", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RFKEY "%s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }

  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{t}"));
  XsnsCall(FUNC_WEB_APPEND);
  if (D_DECIMAL_SEPARATOR[0] != '.') {
    for (uint16_t i = 0; i < strlen(mqtt_data); i++) {
      if ('.' == mqtt_data[i]) {
        mqtt_data[i] = D_DECIMAL_SEPARATOR[0];
      }
    }
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s</table>"), mqtt_data);
  if (devices_present) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s{t}<tr>"), mqtt_data);
    uint8_t fsize = (devices_present < 5) ? 70 - (devices_present * 8) : 32;
    if (SONOFF_IFAN02 == Settings.module) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_STATE,
        mqtt_data, 36, (bitRead(power, 0)) ? "bold" : "normal", 54, GetStateText(bitRead(power, 0)));
      uint8_t fanspeed = GetFanspeed();
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fanspeed);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_STATE,
        mqtt_data, 64, (fanspeed) ? "bold" : "normal", 54, (fanspeed) ? svalue : GetStateText(0));
    } else {
      for (byte idx = 1; idx <= devices_present; idx++) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), bitRead(power, idx -1));
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_DEVICE_STATE,
          mqtt_data, 100 / devices_present, (bitRead(power, idx -1)) ? "bold" : "normal", fsize, (devices_present < 5) ? GetStateText(bitRead(power, idx -1)) : svalue);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s</tr></table>"), mqtt_data);
  }
  WebServer->send(200, FPSTR(HDR_CTYPE_HTML), mqtt_data);
}

boolean HttpUser(void)
{
  boolean status = (HTTP_USER == webserver_state);
  if (status) { HandleRoot(); }
  return status;
}

/*-------------------------------------------------------------------------------------------*/

#ifndef BE_MINIMAL

void HandleConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURATION));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_BTN_MENU_MODULE);

  mqtt_data[0] = '\0';
  XdrvCall(FUNC_WEB_ADD_BUTTON);
  XsnsCall(FUNC_WEB_ADD_BUTTON);
  page += String(mqtt_data);

  page += FPSTR(HTTP_BTN_MENU4);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

/*-------------------------------------------------------------------------------------------*/

void HandleModuleConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }

  if (WebServer->hasArg("save")) {
    ModuleSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[20];
  uint8_t midx;
  mytmplt cmodule;
  memcpy_P(&cmodule, &kModules[Settings.module], sizeof(cmodule));

  if (WebServer->hasArg("m")) {
    String page = "";
    for (byte i = 0; i < MAXMODULE; i++) {
      midx = pgm_read_byte(kModuleNiceList + i);
      snprintf_P(stemp, sizeof(stemp), kModules[midx].name);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SCRIPT_MODULE3, midx, stemp, midx +1);
      page += mqtt_data;
    }
    page += "}3";  // String separator means do not use "}3" in Module name and Sensor name
    for (byte j = 0; j < sizeof(kGpioNiceList); j++) {
      midx = pgm_read_byte(kGpioNiceList + j);
      if (!GetUsedInModule(midx, cmodule.gp.io)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SCRIPT_MODULE3, midx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames), midx);
        page += mqtt_data;
      }
    }
    WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), page);
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MODULE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MODULE));
  page += FPSTR(HTTP_SCRIPT_MODULE1);
  page.replace(F("}4"), String(Settings.module));
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (GPIO_USER == ValidGPIO(i, cmodule.gp.io[i])) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("sk(%d,%d);"), my_module.gp.io[i], i);  // g0 - g16
      page += mqtt_data;
    }
  }
  page += FPSTR(HTTP_SCRIPT_MODULE2);
  page += FPSTR(HTTP_HEAD_STYLE);
  page.replace(F("<body>"), F("<body onload='sl()'>"));
  page += FPSTR(HTTP_FORM_MODULE);
  snprintf_P(stemp, sizeof(stemp), kModules[MODULE].name);
  page.replace(F("{mt"), stemp);
  page += F("<br/><table>");
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (GPIO_USER == ValidGPIO(i, cmodule.gp.io[i])) {
      snprintf_P(stemp, 3, PINS_WEMOS +i*2);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("<tr><td style='width:190px'>%s <b>" D_GPIO "%d</b> %s</td><td style='width:160px'><select id='g%d' name='g%d'></select></td></tr>"),
        (WEMOS==Settings.module)?stemp:"", i, (0==i)? D_SENSOR_BUTTON "1":(1==i)? D_SERIAL_OUT :(3==i)? D_SERIAL_IN :(9==i)? "<font color='red'>ESP8285</font>" :(10==i)? "<font color='red'>ESP8285</font>" :(12==i)? D_SENSOR_RELAY "1":(13==i)? D_SENSOR_LED "1i":(14==i)? D_SENSOR :"", i, i);
      page += mqtt_data;
    }
  }
  page += F("</table>");
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void ModuleSaveSettings(void)
{
  char tmp[100];
  char stemp[TOPSZ];

  WebGetArg("g99", tmp, sizeof(tmp));
  byte new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
  Settings.last_module = Settings.module;
  Settings.module = new_module;
  mytmplt cmodule;
  memcpy_P(&cmodule, &kModules[Settings.module], sizeof(cmodule));
  String gpios = "";
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (Settings.last_module != new_module) {
      Settings.my_gp.io[i] = 0;
    } else {
      if (GPIO_USER == ValidGPIO(i, cmodule.gp.io[i])) {
        snprintf_P(stemp, sizeof(stemp), PSTR("g%d"), i);
        WebGetArg(stemp, tmp, sizeof(tmp));
        Settings.my_gp.io[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
        gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(Settings.my_gp.io[i]);
      }
    }
  }
  snprintf_P(stemp, sizeof(stemp), kModules[Settings.module].name);
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), stemp, gpios.c_str());
  AddLog(LOG_LEVEL_INFO);
}

/*-------------------------------------------------------------------------------------------*/

String htmlEscape(String s)
{
    s.replace("&", "&amp;");
    s.replace("<", "&lt;");
    s.replace(">", "&gt;");
    s.replace("\"", "&quot;");
    s.replace("'", "&#x27;");
    s.replace("/", "&#x2F;");
    return s;
}

void HandleWifiConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_WIFI);

  if (WebServer->hasArg("save")) {
    WifiSaveSettings();
    WebRestart(2);
    return;
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_WIFI));
  page += FPSTR(HTTP_SCRIPT_WIFI);
  page += FPSTR(HTTP_HEAD_STYLE);

  if (WebServer->hasArg("scan")) {
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
          if (-1 == indices[i]) { continue; }
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              AddLog(LOG_LEVEL_DEBUG);
              indices[j] = -1;  // set dup aps to index -1
            }
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (-1 == indices[i]) { continue; }  // skip dups
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"), WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), WiFi.RSSI(indices[i]));
        AddLog(LOG_LEVEL_DEBUG);
        int quality = WifiGetRssiAsQuality(WiFi.RSSI(indices[i]));

        if (minimum_signal_quality == -1 || minimum_signal_quality < quality) {
          String item = FPSTR(HTTP_LNK_ITEM);
          String rssiQ;
          rssiQ += quality;
          item.replace(F("{v}"), htmlEscape(WiFi.SSID(indices[i])));
          item.replace(F("{w}"), String(WiFi.channel(indices[i])));
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
  page.replace(F("{s2"), Settings.sta_ssid[1]);
  page += FPSTR(HTTP_FORM_END);
  if (HTTP_MANAGER == webserver_state) {
    page += FPSTR(HTTP_BTN_RSTRT);
  } else {
    page += FPSTR(HTTP_BTN_CONF);
  }
//  ShowPage(page);
  ShowPage(page, !(HTTP_MANAGER == webserver_state));
}

void WifiSaveSettings(void)
{
  char tmp[100];

  WebGetArg("h", tmp, sizeof(tmp));
  strlcpy(Settings.hostname, (!strlen(tmp)) ? WIFI_HOSTNAME : tmp, sizeof(Settings.hostname));
  if (strstr(Settings.hostname,"%")) {
    strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
  }
  WebGetArg("s1", tmp, sizeof(tmp));
  strlcpy(Settings.sta_ssid[0], (!strlen(tmp)) ? STA_SSID1 : tmp, sizeof(Settings.sta_ssid[0]));
  WebGetArg("s2", tmp, sizeof(tmp));
  strlcpy(Settings.sta_ssid[1], (!strlen(tmp)) ? STA_SSID2 : tmp, sizeof(Settings.sta_ssid[1]));
  WebGetArg("p1", tmp, sizeof(tmp));
  strlcpy(Settings.sta_pwd[0], (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? Settings.sta_pwd[0] : tmp, sizeof(Settings.sta_pwd[0]));
  WebGetArg("p2", tmp, sizeof(tmp));
  strlcpy(Settings.sta_pwd[1], (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? Settings.sta_pwd[1] : tmp, sizeof(Settings.sta_pwd[1]));
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_SSID "2 %s"),
    Settings.hostname, Settings.sta_ssid[0], Settings.sta_ssid[1]);
  AddLog(LOG_LEVEL_INFO);
}

/*-------------------------------------------------------------------------------------------*/

void HandleLoggingConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_LOGGING);

  if (WebServer->hasArg("save")) {
    LoggingSaveSettings();
    HandleConfiguration();
    return;
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_LOGGING));
  page += FPSTR(HTTP_HEAD_STYLE);

  page += FPSTR(HTTP_FORM_LOG1);
  for (byte idx = 0; idx < 3; idx++) {
    page += FPSTR(HTTP_FORM_LOG2);
    switch (idx) {
    case 0:
      page.replace(F("{b0"), F(D_SERIAL_LOG_LEVEL));
      page.replace(F("{b1"), STR(SERIAL_LOG_LEVEL));
      page.replace(F("{b2"), F("ls"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == Settings.seriallog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 1:
      page.replace(F("{b0"), F(D_WEB_LOG_LEVEL));
      page.replace(F("{b1"), STR(WEB_LOG_LEVEL));
      page.replace(F("{b2"), F("lw"));
      for (byte i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
        page.replace("{a" + String(i), (i == Settings.weblog_level) ? F(" selected ") : F(" "));
      }
      break;
    case 2:
      page.replace(F("{b0"), F(D_SYS_LOG_LEVEL));
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

void LoggingSaveSettings(void)
{
  char tmp[100];

  WebGetArg("ls", tmp, sizeof(tmp));
  Settings.seriallog_level = (!strlen(tmp)) ? SERIAL_LOG_LEVEL : atoi(tmp);
  WebGetArg("lw", tmp, sizeof(tmp));
  Settings.weblog_level = (!strlen(tmp)) ? WEB_LOG_LEVEL : atoi(tmp);
  WebGetArg("ll", tmp, sizeof(tmp));
  Settings.syslog_level = (!strlen(tmp)) ? SYS_LOG_LEVEL : atoi(tmp);
  syslog_level = Settings.syslog_level;
  syslog_timer = 0;
  WebGetArg("lh", tmp, sizeof(tmp));
  strlcpy(Settings.syslog_host, (!strlen(tmp)) ? SYS_LOG_HOST : tmp, sizeof(Settings.syslog_host));
  WebGetArg("lp", tmp, sizeof(tmp));
  Settings.syslog_port = (!strlen(tmp)) ? SYS_LOG_PORT : atoi(tmp);
  WebGetArg("lt", tmp, sizeof(tmp));
  Settings.tele_period = (!strlen(tmp)) ? TELE_PERIOD : atoi(tmp);
  if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) {
    Settings.tele_period = 10;   // Do not allow periods < 10 seconds
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
    Settings.seriallog_level, Settings.weblog_level, Settings.syslog_level, Settings.syslog_host, Settings.syslog_port, Settings.tele_period);
  AddLog(LOG_LEVEL_INFO);
}

/*-------------------------------------------------------------------------------------------*/

void HandleOtherConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_OTHER);

  if (WebServer->hasArg("save")) {
    OtherSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[40];

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_OTHER));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_OTHER);
  page.replace(F("{r1"), (Settings.flag.mqtt_enabled) ? F(" checked") : F(""));
  uint8_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!devices_present) ? 1 : devices_present;
  if (SONOFF_IFAN02 == Settings.module) { maxfn = 1; }
  for (byte i = 0; i < maxfn; i++) {
    page += FPSTR(HTTP_FORM_OTHER2);
    page.replace(F("{1"), String(i +1));
    snprintf_P(stemp, sizeof(stemp), PSTR(FRIENDLY_NAME"%d"), i +1);
    page.replace(F("{2"), (i) ? stemp : FRIENDLY_NAME);
    page.replace(F("{3"), Settings.friendlyname[i]);
  }
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
  page += F("<br/></fieldset>");
#endif  // USE_EMULATION
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void OtherSaveSettings(void)
{
  char tmp[100];
  char stemp[TOPSZ];
  char stemp2[TOPSZ];

  WebGetArg("p1", tmp, sizeof(tmp));
  strlcpy(Settings.web_password, (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? Settings.web_password : tmp, sizeof(Settings.web_password));
  Settings.flag.mqtt_enabled = WebServer->hasArg("b1");
#ifdef USE_EMULATION
  WebGetArg("b2", tmp, sizeof(tmp));
  Settings.flag2.emulation = (!strlen(tmp)) ? 0 : atoi(tmp);
#endif  // USE_EMULATION
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_CMND_EMULATION " %d, " D_CMND_FRIENDLYNAME), GetStateText(Settings.flag.mqtt_enabled), Settings.flag2.emulation);
  for (byte i = 0; i < MAX_FRIENDLYNAMES; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("a%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    snprintf_P(stemp2, sizeof(stemp2), PSTR(FRIENDLY_NAME"%d"), i +1);
    strlcpy(Settings.friendlyname[i], (!strlen(tmp)) ? (i) ? stemp2 : FRIENDLY_NAME : tmp, sizeof(Settings.friendlyname[i]));
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%s %s"), log_data, (i) ? "," : "", Settings.friendlyname[i]);
  }
  AddLog(LOG_LEVEL_INFO);
}

/*-------------------------------------------------------------------------------------------*/

void HandleBackupConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  if (!SettingsBufferAlloc()) { return; }

  WiFiClient myClient = WebServer->client();
  WebServer->setContentLength(sizeof(Settings));

  char attachment[100];
  char friendlyname[sizeof(Settings.friendlyname[0])];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), NoAlNumToUnderscore(friendlyname, Settings.friendlyname[0]), my_version);
  WebServer->sendHeader(F("Content-Disposition"), attachment);

  WebServer->send(200, FPSTR(HDR_CTYPE_STREAM), "");

  uint16_t cfg_crc = Settings.cfg_crc;
  Settings.cfg_crc = GetSettingsCrc();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

  memcpy(settings_buffer, &Settings, sizeof(Settings));
  if (config_xor_on_set) {
    for (uint16_t i = 2; i < sizeof(Settings); i++) {
      settings_buffer[i] ^= (config_xor_on_set +i);
    }
  }

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
  size_t written = myClient.write((const char*)settings_buffer, sizeof(Settings));
  if (written < sizeof(Settings)) {  // https://github.com/esp8266/Arduino/issues/3218
    myClient.write((const char*)settings_buffer +written, sizeof(Settings) -written);
  }
#else
  myClient.write((const char*)settings_buffer, sizeof(Settings));
#endif

  SettingsBufferFree();

  Settings.cfg_crc = cfg_crc;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed
}

/*-------------------------------------------------------------------------------------------*/

void HandleResetConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }

  char svalue[33];

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESET_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESET_CONFIGURATION));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += F("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RESET " 1"));
  ExecuteWebCommand(svalue, SRC_WEBGUI);
}

void HandleRestoreConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTORE_CONFIGURATION);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_RESTORE_CONFIGURATION));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_RST);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1"), F(D_RESTORE));
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);

  upload_error = 0;
  upload_file_type = UPL_SETTINGS;
}

/*-------------------------------------------------------------------------------------------*/

void HandleInformation(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_INFORMATION);

  char stopic[TOPSZ];

  int freeMem = ESP.getFreeHeap();

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += FPSTR(HTTP_HEAD_STYLE);
  //  page += F("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>");

  page += F("<style>td{padding:0px 5px;}</style>");
  page += F("<div id='i' name='i'></div>");

  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  String func = FPSTR(HTTP_SCRIPT_INFO_BEGIN);
  func += F("<table style='width:100%'><tr><th>");
  func += F(D_PROGRAM_VERSION "}2"); func += my_version; func += my_image;
  func += F("}1" D_BUILD_DATE_AND_TIME "}2"); func += GetBuildDateAndTime();
  func += F("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_ESP8266_RELEASE "/"); func += String(ESP.getSdkVersion());
  func += F("}1" D_UPTIME "}2"); func += GetUptime();
  snprintf_P(stopic, sizeof(stopic), PSTR(" at 0x%X"), GetSettingsAddress());
  func += F("}1" D_FLASH_WRITE_COUNT "}2"); func += String(Settings.save_flag); func += stopic;
  func += F("}1" D_BOOT_COUNT "}2"); func += String(Settings.bootcount);
  func += F("}1" D_RESTART_REASON "}2"); func += GetResetReason();
  uint8_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;
  if (SONOFF_IFAN02 == Settings.module) { maxfn = 1; }
  for (byte i = 0; i < maxfn; i++) {
    func += F("}1" D_FRIENDLY_NAME " "); func += i +1; func += F("}2"); func += Settings.friendlyname[i];
  }

  func += F("}1}2&nbsp;");  // Empty line
  func += F("}1" D_AP); func += String(Settings.sta_active +1);
    func += F(" " D_SSID " (" D_RSSI ")}2"); func += Settings.sta_ssid[Settings.sta_active]; func += F(" ("); func += WifiGetRssiAsQuality(WiFi.RSSI()); func += F("%)");
  func += F("}1" D_HOSTNAME "}2"); func += my_hostname;
  if (mdns_begun) { func += F(".local"); }
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
    func += F("}1" D_MQTT_USER "}2"); func += Settings.mqtt_user;
    func += F("}1" D_MQTT_CLIENT "}2"); func += mqtt_client;
    func += F("}1" D_MQTT_TOPIC "}2"); func += Settings.mqtt_topic;
    func += F("}1" D_MQTT_GROUP_TOPIC "}2"); func += Settings.mqtt_grptopic;
    func += F("}1" D_MQTT_FULL_TOPIC "}2"); func += GetTopic_P(stopic, CMND, mqtt_topic, "");
    func += F("}1" D_MQTT " " D_FALLBACK_TOPIC "}2"); func += GetFallbackTopic_P(stopic, CMND, "");
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
  snprintf_P(stopic, sizeof(stopic), PSTR("0x%06X"), ESP.getFlashChipId());
  func += F("}1" D_FLASH_CHIP_ID "}2"); func += stopic;
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
#endif  // Not BE_MINIMAL

/*-------------------------------------------------------------------------------------------*/

void HandleUpgradeFirmware(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_FIRMWARE_UPGRADE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_FIRMWARE_UPGRADE));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_UPG);
  page.replace(F("{o1"), Settings.ota_url);
  page += FPSTR(HTTP_FORM_RST_UPG);
  page.replace(F("{r1"), F(D_UPGRADE));
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);

  upload_error = 0;
  upload_file_type = UPL_TASMOTA;
}

void HandleUpgradeFirmwareStart(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  char svalue[100];

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  WifiConfigCounter();

  char tmp[100];
  WebGetArg("o", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_OTAURL " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += F("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>");
  page += FPSTR(HTTP_MSG_RSTRT);
  page += FPSTR(HTTP_BTN_MAIN);
  page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_RELOAD_OTA));
  ShowPage(page);

  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_UPGRADE " 1"));
  ExecuteWebCommand(svalue, SRC_WEBGUI);
}

void HandleUploadDone(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  WifiConfigCounter();
  restart_flag = 0;
  MqttRetryCounter(0);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_INFORMATION));
  page += FPSTR(HTTP_HEAD_STYLE);
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
#ifdef USE_RF_FLASH
      case 10: strncpy_P(error, PSTR(D_UPLOAD_ERR_10), sizeof(error)); break;
      case 11: strncpy_P(error, PSTR(D_UPLOAD_ERR_11), sizeof(error)); break;
      case 12: strncpy_P(error, PSTR(D_UPLOAD_ERR_12), sizeof(error)); break;
      case 13: strncpy_P(error, PSTR(D_UPLOAD_ERR_13), sizeof(error)); break;
#endif
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
    page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_RELOAD_OTA)); // Refesh main web ui after OTA upgrade
    ShowWebSource(SRC_WEBGUI);
    restart_flag = 2;  // Always restart to re-enable disabled features during update
  }
  SettingsBufferFree();
  page += F("</div><br/>");
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

void HandleUploadLoop(void)
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  boolean _serialoutput = (LOG_LEVEL_DEBUG <= seriallog_level);

  if (HTTP_USER == webserver_state) { return; }
  if (upload_error) {
    if (UPL_TASMOTA == upload_file_type) { Update.end(); }
    return;
  }

  HTTPUpload& upload = WebServer->upload();

  if (UPLOAD_FILE_START == upload.status) {
    restart_flag = 60;
    if (0 == upload.filename.c_str()[0]) {
      upload_error = 1;  // No file selected
      return;
    }
    SettingsSave(1);  // Free flash for upload
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    AddLog(LOG_LEVEL_INFO);
    if (UPL_SETTINGS == upload_file_type) {
      if (!SettingsBufferAlloc()) {
        upload_error = 2;  // Not enough space
        return;
      }
    } else {
      MqttRetryCounter(60);
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
#ifdef USE_ARILUX_RF
      AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
      if (Settings.flag.mqtt_enabled) MqttDisconnect();
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {         //start with max available size

//        if (_serialoutput) Update.printError(Serial);
//        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
//          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
//        }

        upload_error = 2;  // Not enough space
        return;
      }
    }
    upload_progress_dot_count = 0;
  } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
    if (0 == upload.totalSize) {
      if (UPL_SETTINGS == upload_file_type) {
        config_block_count = 0;
      }
      else {
#ifdef USE_RF_FLASH
        if ((SONOFF_BRIDGE == Settings.module) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
          Update.end();              // End esp8266 update session
          upload_file_type = UPL_EFM8BB1;

          upload_error = SnfBrUpdateInit();
          if (upload_error != 0) { return; }
        } else
#endif  // USE_RF_FLASH
        {
          if (upload.buf[0] != 0xE9) {
            upload_error = 3;  // Magic byte is not 0xE9
            return;
          }
          uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
          if(bin_flash_size > ESP.getFlashChipRealSize()) {
            upload_error = 4;  // Program flash size is larger than real flash size
            return;
          }
//          upload.buf[2] = 3;  // Force DOUT - ESP8285
        }
      }
    }
    if (UPL_SETTINGS == upload_file_type) {
      if (!upload_error) {
        if (upload.currentSize > (sizeof(Settings) - (config_block_count * HTTP_UPLOAD_BUFLEN))) {
          upload_error = 9;  // File too large
          return;
        }
        memcpy(settings_buffer + (config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
        config_block_count++;
      }
    }
#ifdef USE_RF_FLASH
    else if (UPL_EFM8BB1 == upload_file_type) {
      if (efm8bb1_update != NULL) {    // We have carry over data since last write, i. e. a start but not an end
        ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
        free(efm8bb1_update);
        efm8bb1_update = NULL;
        if (result != 0) {
          upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid
          return;
        }
      }
      ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
      if (result < 0) {
        upload_error = abs(result);
        return;
      } else if (result > 0) {
        if (result > upload.currentSize) {
          // Offset is larger than the buffer supplied, this should not happen
          upload_error = 9;  // File too large - Failed to decode RF firmware
          return;
        }
        // A remnant has been detected, allocate data for it plus a null termination byte
        size_t remnant_sz = upload.currentSize - result;
        efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
        if (efm8bb1_update == NULL) {
          upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
          return;
        }
        memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
        // Add null termination at the end of of remnant buffer
        efm8bb1_update[remnant_sz] = '\0';
      }
    }
#endif  // USE_RF_FLASH
    else {  // firmware
      if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
        upload_error = 5;  // Upload buffer miscompare
        return;
      }
      if (_serialoutput) {
        Serial.printf(".");
        upload_progress_dot_count++;
        if (!(upload_progress_dot_count % 80)) { Serial.println(); }
      }
    }
  } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
    if (_serialoutput && (upload_progress_dot_count % 80)) {
      Serial.println();
    }
    if (UPL_SETTINGS == upload_file_type) {
      if (config_xor_on_set) {
        for (uint16_t i = 2; i < sizeof(Settings); i++) {
          settings_buffer[i] ^= (config_xor_on_set +i);
        }
      }
      bool valid_settings = false;
      unsigned long buffer_version = settings_buffer[11] << 24 | settings_buffer[10] << 16 | settings_buffer[9] << 8 | settings_buffer[8];
      if (buffer_version > 0x06000000) {
        uint16_t buffer_size = settings_buffer[3] << 8 | settings_buffer[2];
        uint16_t buffer_crc = settings_buffer[15] << 8 | settings_buffer[14];
        uint16_t crc = 0;
        for (uint16_t i = 0; i < buffer_size; i++) {
          if ((i < 14) || (i > 15)) { crc += settings_buffer[i]*(i+1); }  // Skip crc
        }
        valid_settings = (buffer_crc == crc);
      } else {
        valid_settings = (settings_buffer[0] == CONFIG_FILE_SIGN);
      }
      if (valid_settings) {
        SettingsDefaultSet2();
        memcpy((char*)&Settings +16, settings_buffer +16, sizeof(Settings) -16);
        Settings.version = buffer_version;  // Restore version and auto upgrade after restart
        SettingsBufferFree();
      } else {
        upload_error = 8;  // File invalid
        return;
      }
    }
#ifdef USE_RF_FLASH
    else if (UPL_EFM8BB1 == upload_file_type) {
      // RF FW flash done
      upload_file_type = UPL_TASMOTA;
    }
#endif  // USE_RF_FLASH
    else {
      if (!Update.end(true)) { // true to set the size to the current progress
        if (_serialoutput) { Update.printError(Serial); }
        upload_error = 6;  // Upload failed. Enable logging 3
        return;
      }
    }
    if (!upload_error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
      AddLog(LOG_LEVEL_INFO);
    }
  } else if (UPLOAD_FILE_ABORTED == upload.status) {
    restart_flag = 0;
    MqttRetryCounter(0);
    upload_error = 7;  // Upload aborted
    if (UPL_TASMOTA == upload_file_type) { Update.end(); }
  }
  delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void HandlePreflightRequest(void)
{
  WebServer->sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  WebServer->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  WebServer->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  WebServer->send(200, FPSTR(HDR_CTYPE_HTML), "");
}

/*-------------------------------------------------------------------------------------------*/

void HandleHttpCommand(void)
{
  if (HttpUser()) { return; }
//  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  char svalue[INPUT_BUFFER_SIZE];  // Large to serve Backlog

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  uint8_t valid = 1;
  if (Settings.web_password[0] != 0) {
    char tmp1[100];
    WebGetArg("user", tmp1, sizeof(tmp1));
    char tmp2[100];
    WebGetArg("password", tmp2, sizeof(tmp2));
    if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, Settings.web_password))) { valid = 0; }
  }

  String message = F("{\"" D_RSLT_WARNING "\":\"");
  if (valid) {
    byte curridx = web_log_index;
    WebGetArg("cmnd", svalue, sizeof(svalue));
    if (strlen(svalue)) {
      ExecuteWebCommand(svalue, SRC_WEBCOMMAND);

      if (web_log_index != curridx) {
        byte counter = curridx;
        message = F("{");
        do {
          char* tmp;
          size_t len;
          GetLog(counter, &tmp, &len);
          if (len) {
            // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
            char* JSON = (char*)memchr(tmp, '{', len);
            if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
              if (message.length() > 1) { message += F(","); }
              size_t JSONlen = len - (JSON - tmp);
              strlcpy(mqtt_data, JSON +1, JSONlen -2);
              message += mqtt_data;
            }
          }
          counter++;
          if (!counter) counter++;  // Skip 0 as it is not allowed
        } while (counter != web_log_index);
        message += F("}");
      } else {
        message += F(D_ENABLE_WEBLOG_FOR_RESPONSE "\"}");
      }
    } else {
      message += F(D_ENTER_COMMAND " cmnd=\"}");
    }
  } else {
    message += F(D_NEED_USER_AND_PASSWORD "\"}");
  }
  SetHeader();
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), message);
}

/*-------------------------------------------------------------------------------------------*/

void HandleConsole(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONSOLE);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONSOLE));
  page += FPSTR(HTTP_HEAD_STYLE);
  page.replace(F("</script>"), FPSTR(HTTP_SCRIPT_CONSOL));
  page.replace(F("<body>"), F("<body onload='l()'>"));
  page += FPSTR(HTTP_FORM_CMND);
  page += FPSTR(HTTP_BTN_MAIN);
  ShowPage(page);
}

void HandleAjaxConsoleRefresh(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  char svalue[INPUT_BUFFER_SIZE];  // Large to serve Backlog
  byte cflg = 1;
  byte counter = 0;                // Initial start, should never be 0 again

  WebGetArg("c1", svalue, sizeof(svalue));
  if (strlen(svalue)) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_COMMAND "%s"), svalue);
    AddLog(LOG_LEVEL_INFO);
    ExecuteWebCommand(svalue, SRC_WEBCONSOLE);
  }

  WebGetArg("c2", svalue, sizeof(svalue));
  if (strlen(svalue)) { counter = atoi(svalue); }

  byte last_reset_web_log_flag = reset_web_log_flag;
  String message = F("}9");  // Cannot load mqtt_data here as <> will be encoded by replacements below
  if (!reset_web_log_flag) {
    counter = 0;
    reset_web_log_flag = 1;
  }
  if (counter != web_log_index) {
    if (!counter) {
      counter = web_log_index;
      cflg = 0;
    }
    do {
      char* tmp;
      size_t len;
      GetLog(counter, &tmp, &len);
      if (len) {
        if (cflg) {
          message += F("\n");
        } else {
          cflg = 1;
        }
        strlcpy(mqtt_data, tmp, len);
        message += mqtt_data;
      }
      counter++;
      if (!counter) { counter++; } // Skip 0 as it is not allowed
    } while (counter != web_log_index);
    // XML encoding to fix blank console log in concert with javascript decodeURIComponent
    message.replace(F("%"), F("%25"));  // Needs to be done first as otherwise the % in %26 will also be converted
    message.replace(F("&"), F("%26"));
    message.replace(F("<"), F("%3C"));
    message.replace(F(">"), F("%3E"));
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("<r><i>%d</i><j>%d</j><l>"), web_log_index, last_reset_web_log_flag);
  message.replace(F("}9"), mqtt_data);  // Save to load here
  message += F("</l></r>");
  WebServer->send(200, FPSTR(HDR_CTYPE_XML), message);
}

/********************************************************************************************/

void HandleNotFound(void)
{
//  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP "Not fount (%s)"), WebServer->uri().c_str());
//  AddLog(LOG_LEVEL_DEBUG);

  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the error page.

#ifdef USE_EMULATION
  String path = WebServer->uri();
  if ((EMUL_HUE == Settings.flag2.emulation) && (path.startsWith("/api"))) {
    HandleHueApi(&path);
  } else
#endif // USE_EMULATION
  {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"),
      WebServer->uri().c_str(), (WebServer->method() == HTTP_GET) ? "GET" : "POST", WebServer->args());
    for (uint8_t i = 0; i < WebServer->args(); i++) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s %s: %s\n"), mqtt_data, WebServer->argName(i).c_str(), WebServer->arg(i).c_str());
    }
    SetHeader();
    WebServer->send(404, FPSTR(HDR_CTYPE_PLAIN), mqtt_data);
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean CaptivePortal(void)
{
  if ((HTTP_MANAGER == webserver_state) && !ValidIpAddress(WebServer->hostHeader())) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    WebServer->sendHeader(F("Location"), String("http://") + WebServer->client().localIP().toString(), true);
    WebServer->send(302, FPSTR(HDR_CTYPE_PLAIN), "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    WebServer->client().stop();  // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Is this an IP? */
boolean ValidIpAddress(String str)
{
  for (uint16_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) { return false; }
  }
  return true;
}

/*********************************************************************************************/

String UrlEncode(const String& text)
{
  const char hex[] = "0123456789ABCDEF";

	String encoded = "";
	int len = text.length();
	int i = 0;
	while (i < len)	{
		char decodedChar = text.charAt(i++);

/*
    if (('a' <= decodedChar && decodedChar <= 'z') ||
        ('A' <= decodedChar && decodedChar <= 'Z') ||
        ('0' <= decodedChar && decodedChar <= '9') ||
        ('=' == decodedChar)) {
      encoded += decodedChar;
		} else {
      encoded += '%';
			encoded += hex[decodedChar >> 4];
			encoded += hex[decodedChar & 0xF];
    }
*/
    if (' ' == decodedChar) {
      encoded += '%';
			encoded += hex[decodedChar >> 4];
			encoded += hex[decodedChar & 0xF];
    } else {
      encoded += decodedChar;
    }

	}
	return encoded;
}

int WebSend(char *buffer)
{
  /* [sonoff] POWER1 ON                                               --> Sends http://sonoff/cm?cmnd=POWER1 ON
   * [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
   * [sonoff] /any/link/starting/with/a/slash.php?log=123             --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123
   * [sonoff,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123
   */

  char *host;
  char *port;
  char *user;
  char *password;
  char *command;
  uint16_t nport = 80;
  int status = 1;                             // Wrong parameters

                                              // buffer = |  [  192.168.178.86  :  80  ,  admin  :  joker  ]    POWER1 ON   |
  host = strtok_r(buffer, "]", &command);     // host = |  [  192.168.178.86  :  80  ,  admin  :  joker  |, command = |    POWER1 ON   |
  if (host && command) {
    host = Trim(host);                        // host = |[  192.168.178.86  :  80  ,  admin  :  joker|
    host++;                                   // host = |  192.168.178.86  :  80  ,  admin  :  joker| - Skip [
    host = strtok_r(host, ",", &user);        // host = |  192.168.178.86  :  80  |, user = |  admin  :  joker|
    host = strtok_r(host, ":", &port);        // host = |  192.168.178.86  |, port = |  80  |
    host = Trim(host);                        // host = |192.168.178.86|
    if (port) {
      port = Trim(port);                      // port = |80|
      nport = atoi(port);
    }
    if (user) {
      user = strtok_r(user, ":", &password);  // user = |  admin  |, password = |  joker|
      user = Trim(user);                      // user = |admin|
      if (password) { password = Trim(password); }  // password = |joker|
    }
    command = Trim(command);                  // command = |POWER1 ON| or |/any/link/starting/with/a/slash.php?log=123|

    String nuri = "";
    if (command[0] != '/') {
      nuri = "/cm?";
      if (user && password) {
        nuri += F("user=");
        nuri += user;
        nuri += F("&password=");
        nuri += password;
        nuri += F("&");
      }
      nuri += F("cmnd=");
    }
    nuri += command;
    String uri = UrlEncode(nuri);

    IPAddress host_ip;
    if (WiFi.hostByName(host, host_ip)) {
      WiFiClient client;

      bool connected = false;
      byte retry = 2;
      while ((retry > 0) && !connected) {
        --retry;
        connected = client.connect(host_ip, nport);
        if (connected) break;
      }

      if (connected) {
        String url = F("GET ");
        url += uri;
        url += F(" HTTP/1.1\r\nHost: ");
//        url += IPAddress(host_ip).toString();
        url += host;   // https://tools.ietf.org/html/rfc7230#section-5.4 (#4331)
        if (port) {
          url += F(":");
          url += port;
        }
        url += F("\r\nConnection: close\r\n\r\n");

//snprintf_P(log_data, sizeof(log_data), PSTR("DBG: Url |%s|"), url.c_str());
//AddLog(LOG_LEVEL_DEBUG);

        client.print(url.c_str());
        client.flush();
        client.stop();
        status = 0;                           // No error - Done
      } else {
        status = 2;                           // Connection failed
      }
    } else {
      status = 3;                             // Host not found
    }
  }
  return status;
}

/*********************************************************************************************/

enum WebCommands { CMND_WEBSERVER, CMND_WEBPASSWORD, CMND_WEBLOG, CMND_WEBREFRESH, CMND_WEBSEND, CMND_EMULATION };
const char kWebCommands[] PROGMEM = D_CMND_WEBSERVER "|" D_CMND_WEBPASSWORD "|" D_CMND_WEBLOG "|" D_CMND_WEBREFRESH "|" D_CMND_WEBSEND "|" D_CMND_EMULATION ;
const char kWebSendStatus[] PROGMEM = D_JSON_DONE "|" D_JSON_WRONG_PARAMETERS "|" D_JSON_CONNECT_FAILED "|" D_JSON_HOST_NOT_FOUND ;

bool WebCommand(void)
{
  char command[CMDSZ];
  bool serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kWebCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  if (CMND_WEBSERVER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) { Settings.webserver = XdrvMailbox.payload; }
    if (Settings.webserver) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WEBSERVER "\":\"" D_JSON_ACTIVE_FOR " %s " D_JSON_ON_DEVICE " %s " D_JSON_WITH_IP_ADDRESS " %s\"}"),
        (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str());
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(0));
    }
  }
  else if (CMND_WEBPASSWORD == command_code) {
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(Settings.web_password))) {
      strlcpy(Settings.web_password, (SC_CLEAR == Shortcut(XdrvMailbox.data)) ? "" : (SC_DEFAULT == Shortcut(XdrvMailbox.data)) ? WEB_PASSWORD : XdrvMailbox.data, sizeof(Settings.web_password));
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.web_password);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_ASTERIX, command);
    }
  }
  else if (CMND_WEBLOG == command_code) {
    if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_ALL)) { Settings.weblog_level = XdrvMailbox.payload; }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.weblog_level);
  }
  else if (CMND_WEBREFRESH == command_code) {
    if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload <= 10000)) { Settings.web_refresh = XdrvMailbox.payload; }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.web_refresh);
  }
  else if (CMND_WEBSEND == command_code) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t result = WebSend(XdrvMailbox.data);
      char stemp1[20];
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetTextIndexed(stemp1, sizeof(stemp1), result, kWebSendStatus));
    }
  }
#ifdef USE_EMULATION
  else if (CMND_EMULATION == command_code) {
    if ((XdrvMailbox.payload >= EMUL_NONE) && (XdrvMailbox.payload < EMUL_MAX)) {
      Settings.flag2.emulation = XdrvMailbox.payload;
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.flag2.emulation);
  }
#endif  // USE_EMULATION
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv01(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_LOOP:
      PollDnsWebserver();
#ifdef USE_EMULATION
      if (Settings.flag2.emulation) PollUdp();
#endif  // USE_EMULATION
      break;
    case FUNC_COMMAND:
      result = WebCommand();
      break;
  }
  return result;
}
#endif  // USE_WEBSERVER
