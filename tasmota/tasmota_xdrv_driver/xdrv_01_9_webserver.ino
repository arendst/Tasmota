/*
  xdrv_01_webserver.ino - webserver for Tasmota

  Copyright (C) 2021  Theo Arends and Adrian Scillato

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

#define XDRV_01                                   1

// Enable below demo feature only if defines USE_UNISHOX_COMPRESSION and USE_SCRIPT_WEB_DISPLAY are disabled
//#define USE_WEB_SSE

#define USE_CONSOLE_CSS_FLEX

#ifndef WIFI_SOFT_AP_CHANNEL
#define WIFI_SOFT_AP_CHANNEL                      1      // Soft Access Point Channel number between 1 and 11 as used by WifiManager web GUI
#endif

#ifndef MAX_WIFI_NETWORKS_TO_SHOW
#define MAX_WIFI_NETWORKS_TO_SHOW                 3      // Maximum number of Wifi Networks to show in the Wifi Configuration Menu BEFORE clicking on Show More Networks.
#endif

#ifndef RESTART_AFTER_INITIAL_WIFI_CONFIG
#define RESTART_AFTER_INITIAL_WIFI_CONFIG         true   // Restart Tasmota after initial Wifi Config of a blank device
#endif                                                   //   If disabled, Tasmota will keep both the wifi AP and the wifi connection to the router
                                                         //   but only until next restart.
#ifndef AFTER_INITIAL_WIFI_CONFIG_GO_TO_NEW_IP           // If RESTART_AFTER_INITIAL_WIFI_CONFIG and AFTER_INITIAL_WIFI_CONFIG_GO_TO_NEW_IP are true,
#define AFTER_INITIAL_WIFI_CONFIG_GO_TO_NEW_IP    true   //   the user will be redirected to the new IP of Tasmota (in the new Network).
#endif                                                   //   If the first is true, but this is false, the device will restart but the user will see
                                                         //   a window telling that the WiFi Configuration was Ok and that the window can be closed.

const uint16_t CHUNKED_BUFFER_SIZE = 500;                // Chunk buffer size (needs to be well below stack space (4k for ESP8266, 8k for ESP32) but large enough to cache some small messages)

const uint16_t HTTP_REFRESH_TIME = 2345;                 // milliseconds
const uint16_t HTTP_RESTART_RECONNECT_TIME = 10000;      // milliseconds - Allow time for restart and wifi reconnect
#ifdef ESP8266
const uint16_t HTTP_OTA_RESTART_RECONNECT_TIME = 24000;  // milliseconds - Allow time for uploading binary, unzip/write to final destination and wifi reconnect
#endif  // ESP8266
#ifdef ESP32
const uint16_t HTTP_OTA_RESTART_RECONNECT_TIME = 10000;  // milliseconds - Allow time for restart and wifi reconnect
#endif  // ESP32

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#ifdef USE_UNISHOX_COMPRESSION
  #include "./html_compressed/HTTP_HEADER1_ES6.h"
#else
  #include "./html_uncompressed/HTTP_HEADER1_ES6.h"
#endif

#ifdef USE_ALPINEJS
#include "include/alpinejs.h"
#endif  // USE_ALPINEJS

const char HTTP_SCRIPT_COUNTER[] PROGMEM =
  "var cn=180;"                           // seconds
  "function u(){"
    "if(cn>=0){"
      "eb('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "wl(u);";

#ifdef USE_UNISHOX_COMPRESSION
  #ifdef USE_SCRIPT_WEB_DISPLAY
    #include "./html_compressed/HTTP_SCRIPT_ROOT_WEB_DISPLAY.h"
  #else
    #include "./html_compressed/HTTP_SCRIPT_ROOT_NO_WEB_DISPLAY.h"
  #endif
  #include "./html_compressed/HTTP_SCRIPT_ROOT_PART2.h"
#else
  #ifdef USE_SCRIPT_WEB_DISPLAY
    #include "./html_uncompressed/HTTP_SCRIPT_ROOT_WEB_DISPLAY.h"
  #else
    #ifdef USE_WEB_SSE
      #include "./html_uncompressed/HTTP_SCRIPT_ROOT_SSE_NO_WEB_DISPLAY.h"
    #else
      #include "./html_uncompressed/HTTP_SCRIPT_ROOT_NO_WEB_DISPLAY.h"
    #endif  // USE_WEB_SSE
  #endif
  #include "./html_uncompressed/HTTP_SCRIPT_ROOT_PART2.h"
#endif

const char HTTP_SCRIPT_WIFI[] PROGMEM =
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}";

const char HTTP_SCRIPT_HIDE[] PROGMEM =
  "function hidBtns(){"
    "eb('butmo').style.display='none';"
    "eb('butmod').style.display='none';"
    "eb('but0').style.display='block';"
    "eb('but1').style.display='block';"
    "eb('but13').style.display='block';"
    "eb('but0d').style.display='block';"
    "eb('but13d').style.display='block';"
  "}";

const char HTTP_SCRIPT_RELOAD_TIME[] PROGMEM =
  "setTimeout(function(){location.href='.';},%d);";

#ifdef USE_UNISHOX_COMPRESSION
  #include "./html_compressed/HTTP_SCRIPT_CONSOL.h"
#else
  #include "./html_uncompressed/HTTP_SCRIPT_CONSOL.h"
#endif

const char HTTP_MODULE_TEMPLATE_REPLACE_INDEX[] PROGMEM =
  "}2%d'>%s (%d)}3";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX[] PROGMEM =
  "}2%d'>%s}3";                           // }2 and }3 are used in below os.replace

#ifdef USE_UNISHOX_COMPRESSION
  #include "./html_compressed/HTTP_SCRIPT_MODULE_TEMPLATE.h"
  #include "./html_compressed/HTTP_SCRIPT_TEMPLATE.h"
#else
  #include "./html_uncompressed/HTTP_SCRIPT_MODULE_TEMPLATE.h"
  #include "./html_uncompressed/HTTP_SCRIPT_TEMPLATE.h"
#endif

#ifdef ESP8266
const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
    "j=0;"
    "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"  // Supports 13 GPIOs
      "if(6==i){j=9;}"
      "if(8==i){j=12;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}";
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32P4
const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
    "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"
      "sk(g[i],i);"                       // Set GPIO
    "}";
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
    "j=0;"
    "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"  // Skip 22-32
      "if(22==i){j=33;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}";
#else  // ESP32
const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
    "j=0;"
    "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"  // Skip 28-31
      "if(28==i){j=32;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}";
#endif  // Non plain ESP32    
#endif  // ESP32

const char HTTP_SCRIPT_TEMPLATE3[] PROGMEM =
    "\";"
    "sk(g[13]," STR(ADC0_PIN) ");";       // Set ADC0

const char HTTP_SCRIPT_TEMPLATE4[] PROGMEM =
    "g=o.shift();"                        // FLAG
    "for(i=0;i<" STR(GPIO_FLAG_USED) ";i++){"
      "p=(g>>i)&1;"
      "eb('c'+i).checked=p;"              // Set FLAG checkboxes
    "}"
    "if(" STR(USER_MODULE) "==c){"
      "g=o.shift();"
      "eb('g99').value=g;"                // Set BASE for initial select
    "}"
  "}"
  "function st(t){"
    "c=t;"                                // Needed for initial BASE select
    "var a='tp?t='+t;"
    "ld(a,x1);"                           // ?t related to WebGetArg("t", stemp, sizeof(stemp));
  "}"
  "function sl(){"
    "os=\"";                              // }2'0'>Sonoff Basic (1)}3...
const char HTTP_SCRIPT_TEMPLATE5[] PROGMEM =
    "\";"
    "sk(" STR(WEMOS_MODULE) ",99);"       // 17 = WEMOS
    "st(" STR(USER_MODULE) ");"
  "}"
  "wl(sl);";

const char HTTP_SCRIPT_INFO_BEGIN[] PROGMEM =
  "function i(){"
    "var s,o=\"";
const char HTTP_SCRIPT_INFO_END[] PROGMEM =
    "\";"                                 // "}1" and "}2" means do not use "}x" in Information text
    "s=o.replace(/}1/g,\"</td></tr><tr><th>\").replace(/}2/g,\"</th><td>\");"
    "eb('i').innerHTML=s;"
  "}"
  "wl(i);";

#ifdef USE_UNISHOX_COMPRESSION
  #include "./html_compressed/HTTP_HEAD_LAST_SCRIPT.h"
  #include "./html_compressed/HTTP_HEAD_LAST_SCRIPT32.h"
  #include "./html_compressed/HTTP_HEAD_STYLE_ROOT_COLOR.h"
  #include "./html_compressed/HTTP_HEAD_STYLE1.h"
  #include "./html_compressed/HTTP_HEAD_STYLE2.h"
  #include "./html_compressed/HTTP_HEAD_STYLE3.h"
  #include "./html_compressed/HTTP_HEAD_STYLE_WIFI.h"
#else
  #include "./html_uncompressed/HTTP_HEAD_LAST_SCRIPT.h"
  #include "./html_uncompressed/HTTP_HEAD_LAST_SCRIPT32.h"
  #include "./html_uncompressed/HTTP_HEAD_STYLE_ROOT_COLOR.h"
  #include "./html_uncompressed/HTTP_HEAD_STYLE1.h"
  #include "./html_uncompressed/HTTP_HEAD_STYLE2.h"
  #include "./html_uncompressed/HTTP_HEAD_STYLE3.h"
  #include "./html_uncompressed/HTTP_HEAD_STYLE_WIFI.h"
#endif

#if defined(USE_ZIGBEE) || defined(USE_LORAWAN_BRIDGE)
// Styles used for Zigbee and LoRaWan Web UI
// Battery icon from https://css.gg/battery
//
  #ifdef USE_UNISHOX_COMPRESSION
    #include "./html_compressed/HTTP_HEAD_STYLE_ZIGBEE.h"
  #else
    #include "./html_uncompressed/HTTP_HEAD_STYLE_ZIGBEE.h"
  #endif
#endif // USE_ZIGBEE

const char HTTP_HEAD_STYLE_SSI[] PROGMEM =
  // Signal Strength Indicator
  ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0;"
  "i{width:3px;margin-right:1px;border-radius:3px;background-color:var(--c_txt)}"
  ".b0{height:25%}.b1{height:50%}.b2{height:75%}.b3{height:100%}}"
  ".o30{opacity:.3}";

// special case if MINIMAL, then we don't use compressed version
#ifdef FIRMWARE_MINIMAL
const char HTTP_HEAD_STYLE3_MINIMAL[] PROGMEM =
  "</style>"

  "</head>"
  "<body>"
  "<div style='background:var(--c_bg);text-align:left;display:inline-block;color:var(--c_txt);min-width:340px;position:relative;'>"
#ifdef FIRMWARE_SAFEBOOT
  "<span style='text-align:center;color:var(--c_txtwrn);'><h3>" D_SAFEBOOT "</h3></span>"
#else
  "<div style='text-align:center;color:var(--c_txtwrn);'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"
#endif  // FIRMWARE_SAFEBOOT
  "<div style='text-align:center;color:var(--c_ttl);'><noscript>" D_NOSCRIPT "<br></noscript>"
  "<h3>%s</h3>"    // Module name
  "<h2>%s</h2>";   // Device name
#endif  // FIRMWARE_MINIMAL

const char HTTP_MENU_HEAD[] PROGMEM =
  "<div style='padding:0px 5px;text-align:center;'><h3><hr>%s<hr></h3></div>";

const char HTTP_MSG_SLIDER_SHUTTER[] PROGMEM =
  "<td style='width:70%%'>"
  "<div style='padding:0px 2px;text-align:center;font-size:12px;'><span>%s</span>"
  "<input id='s27%d' type='range' min='0' max='100' value='%d' onchange='lc(\"u\",%d,value)'>"
  "</div>"
  "</td>";

const char HTTP_MSG_SLIDER_GRADIENT[] PROGMEM =
  "<td colspan='%d' style='width:%d%%'>"
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc(\"%c\",%d,value)'>"
  "</div>"
  "</td>";

// https://stackoverflow.com/questions/4057236/how-to-add-onload-event-to-a-div-element
const char HTTP_MSG_EXEC_JAVASCRIPT[] PROGMEM =
  "<img style='display:none;' src onerror=\"";

const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br><div style='text-align:center;'>" D_DEVICE_WILL_RESTART "</div><br>";

const char HTTP_FORM_LOGIN[] PROGMEM =
  "<fieldset>"
  "<form method='post' action='/'>"
  "<p><b>" D_USER "</b><br><input name='USER1' placeholder='" D_USER "'></p>"
  "<p><b>" D_PASSWORD "</b><br><input name='PASS1' type='password' placeholder='" D_PASSWORD "'></p>"
  "<br>"
  "<button>" D_OK "</button>"
  "</form></fieldset>";

const char HTTP_FORM_TEMPLATE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='tp'>";
const char HTTP_FORM_TEMPLATE_FLAG[] PROGMEM =
  "<p></p>"  // Keep close so do not use <br>
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_FLAGS "&nbsp;</b></legend><p>"
//  "<label><input id='c0' name='c0' type='checkbox'><b>" D_OPTION_TEXT "</b></label><br>"
  "</p></fieldset>";

const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='md'>"
  "<p></p><b>" D_MODULE_TYPE "</b> (%s)<br><select id='g99'></select><br>"
  "<br><table>";

const char HTTP_FORM_WIFI_PART1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='wi'>"
  "<p><b>" D_AP1_SSID "</b>%s<br><input id='s1' placeholder=\"" D_AP1_SSID_HELP "\" value=\"%s\"></p>"  // Need \" instead of ' to be able to use ' in text (#8489)
  "<p><label><b>" D_AP_PASSWORD "</b><input type='checkbox' onclick='sp(\"p1\")'></label><br><input id='p1' type='password' placeholder=\"" D_AP_PASSWORD_HELP "\"";

const char HTTP_FORM_WIFI_PART2[] PROGMEM =
  " value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br><input id='s2' placeholder=\"" D_AP2_SSID_HELP "\" value=\"%s\"></p>"
  "<p><label><b>" D_AP_PASSWORD "</b><input type='checkbox' onclick='sp(\"p2\")'></label><br><input id='p2' type='password' placeholder=\"" D_AP_PASSWORD_HELP "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>" D_HOSTNAME "</b> (%s)<br><input id='h' placeholder=\"%s\" value=\"%s\"></p>"
#ifdef USE_CORS
  "<p><b>" D_CORS_DOMAIN "</b><input id='c' placeholder=\"" CORS_DOMAIN "\" value=\"%s\"></p>"
#endif
  ;

const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b>"
  "</legend><form method='get' action='lg'>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br><input id='lh' placeholder=\"" SYS_LOG_HOST "\" value=\"%s\"></p>"
  "<p><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br><input id='lp' placeholder='" STR(SYS_LOG_PORT) "' value='%d'></p>"
  "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br><input id='lt' placeholder='" STR(TELE_PERIOD) "' value='%d'></p>";

const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='co'>"
  "<p></p>"
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE "&nbsp;</b></legend>"
  "<p><input id='t1' placeholder=\"" D_TEMPLATE "\" value='%s'></p>"  // We need ' apostrophe here as the template contains " quotation mark
  "<p><label><input id='t2' type='checkbox'%s><b>" D_ACTIVATE "</b></label></p>"
  "</fieldset>"
  "<br>"
  "<label><b>" D_WEB_ADMIN_PASSWORD "</b><input type='checkbox' onclick='sp(\"wp\")'></label><br><input id='wp' type='password' placeholder=\"" D_WEB_ADMIN_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"><br>"
  "<br>"
  "<label><input id='b3' type='checkbox'%s><b>" D_HTTP_API_ENABLE "</b></label><br>"
  "<label><input id='b1' type='checkbox'%s><b>" D_MQTT_ENABLE "</b></label><br>"
  "<br>"
  "<label><b>" D_DEVICE_NAME "</b> (%s)</label><br><input id='dn' placeholder=\"\" value=\"%s\"><br>"
  "<br>";

const char HTTP_FORM_END[] PROGMEM =
  "<br>"
  "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";

const char HTTP_FORM_RST[] PROGMEM =
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_RESTORE_CONFIGURATION "&nbsp;</b></legend>";
const char HTTP_FORM_UPG[] PROGMEM =
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
  "<form method='get' action='u1'>"
  "<br><b>" D_OTA_URL "</b><br><input id='o' placeholder=\"OTA_URL\" value=\"%s\"><br>"
  "<br><button type='submit'>" D_START_UPGRADE "</button></form>"
  "</fieldset><br><br>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
const char HTTP_FORM_RST_UPG[] PROGMEM =                  // ESP8266 or ESP32 dual partition
  "<form method='post' action='u2?fsz=' enctype='multipart/form-data'>"
  "<br><input type='file' name='u2'><br>"
  "<br><button type='submit' "
  "onclick='"
    "eb(\"f1\").style.display=\"none\";"                  // Disable display of form f1
    "eb(\"f2\").style.display=\"block\";"                 // Enable display of D_UPLOAD_STARTED
    "this.form.action+=this.form[\"u2\"].files[0].size;"  // Set return file size
    "this.form.submit();"                                 // Form response
  "'>%s</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED "...</b></div>";

// upload via factory partition
const char HTTP_FORM_RST_UPG_FCT[] PROGMEM =              // ESP32 safeboot partition
  "<form method='post' action='u2?fsz=' enctype='multipart/form-data'>"
  "<br><input type='file' name='u2'><br>"
  "<br><button type='submit' "
  "onclick='"
    "eb(\"f1\").style.display=\"none\";"                  // Disable display of form f1
    "var fs=this.form[\"u2\"].files[0].size;"             // Retreive file size of requested file
    "eb((fs>900000)?\"f3\":\"f2\").style.display=\"block\";"  // Enable display of either D_UPLOAD_FACTORY or D_UPLOAD_STARTED based on arbitrary file size of 900k
    "this.form.action+=fs;"                               // Set return file size
    "return upl(this);"                                   // Form response
  "'>%s</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f3' style='display:none;text-align:center;'><b>" D_UPLOAD_FACTORY "...</b></div>"
  "<div id='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED "...</b></div>";

#ifdef USE_CONSOLE_CSS_FLEX
const char HTTP_CMND_STYLE[] PROGMEM =  // Overrule CSS for flex console
  "html,body{height:99%%;}"
  "body{display:flex;flex-flow:column;}"
  "textarea{resize:none;flex:auto;min-height:99px}";

const char HTTP_FORM_CMND[] PROGMEM =
  "</div>"                     // Close HTTP_HEAD_STYLE3 <div>
  "<textarea readonly id='t1' wrap='off'></textarea>"
  "<form method='get' onsubmit='return l(1);'>"
  "<br>"                       // <br> here fixes Firefox layout
  "<input id='c1' placeholder='" D_ENTER_COMMAND "' autofocus>"
  //  "<br><button type='submit'>Send command</button>"
  "</form>"
  "<div style='padding:0;'>";  // Add dummy <div> replacing HTTP_HEAD_STYLE3 closed <div>
#else
const char HTTP_FORM_CMND[] PROGMEM =
  "<br><textarea readonly id='t1' cols='340' wrap='off'></textarea><br><br>"
  "<form method='get' onsubmit='return l(1);'>"
  "<input id='c1' placeholder='" D_ENTER_COMMAND "' autofocus><br>"
  //  "<br><button type='submit'>Send command</button>"
  "</form>";
#endif  // USE_CONSOLE_CSS_FLEX

const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%%'>";

const char HTTP_COUNTER[] PROGMEM =
  "<br><div id='t' style='text-align:center;'></div>";

const char HTTP_END[] PROGMEM =
  "<p></p><div style='text-align:right;font-size:11px;'><hr><a href='https://github.com/arendst/Tasmota' target='_blank' style='color:#aaa;'>Tasmota %s %s " D_BY " Theo Arends</a></div>"
  "</div>"
  "</body>"
  "</html>";

const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button id='o%d' onclick='la(\"&o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg(PSTR("o"), tmp, sizeof(tmp))
const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d%%;text-align:center;font-weight:%s;font-size:%dpx'>%s</td>";

const char HTTP_STATUS_STICKER[] PROGMEM =
  "<span style='"
  "margin:2px;"
  "cursor:default;"
  "padding:1px 2px;"
  "border-color:#%06x;border-radius:5px;border-style:solid;border-width:1px;"
  "' "
  "%s"    // optional 'title' attributes
  ">"
  "%s"
  "</span>";

enum ButtonTitle {
  BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
  BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_MANAGEMENT,
  BUTTON_MODULE, BUTTON_WIFI, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, BUTTON_RESTORE,
  BUTTON_CONSOLE };
const char kButtonTitle[] PROGMEM =
  D_RESTART "|" D_RESET_CONFIGURATION "|"
  D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_MANAGEMENT "|"
  D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION "|"
  D_CONSOLE;
const char kButtonAction[] PROGMEM =
  ".|rt|"
  ".|cn|in|up|mn|"
  "md|wi|lg|co|tp|dl|rs|"
  "cs";
const char kButtonConfirm[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;

enum CTypes { CT_HTML, CT_PLAIN, CT_XML, CT_STREAM, CT_APP_JSON, CT_APP_STREAM };
const char kContentTypes[] PROGMEM = "text/html|text/plain|text/xml|text/event-stream|application/json|application/octet-stream";

const char kLoggingOptions[] PROGMEM = D_SERIAL_LOG_LEVEL "|" D_WEB_LOG_LEVEL "|" D_MQTT_LOG_LEVEL "|" D_SYS_LOG_LEVEL;
const char kLoggingLevels[] PROGMEM = D_NONE "|" D_ERROR "|" D_INFO "|" D_DEBUG "|" D_MORE_DEBUG;

const char kEmulationOptions[] PROGMEM = D_NONE "|" D_BELKIN_WEMO "|" D_HUE_BRIDGE;

const char kUploadErrors[] PROGMEM =
//  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "|" D_UPLOAD_ERR_5 "|" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9;
  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "| |" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9;

const uint16_t DNS_PORT = 53;
enum HttpOptions { HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER, HTTP_MANAGER_RESET_ONLY };
enum WebCmndStatus { WEBCMND_DONE, WEBCMND_WRONG_PARAMETERS, WEBCMND_CONNECT_FAILED, WEBCMND_HOST_NOT_FOUND, WEBCMND_MEMORY_ERROR, WEBCMND_VALID_RESPONSE
#ifdef USE_WEBGETCONFIG
  ,WEBCMND_FILE_NOT_FOUND, WEBCMND_OTHER_HTTP_ERROR, WEBCMND_CONNECTION_LOST, WEBCMND_INVALID_FILE
#endif // USE_WEBGETCONFIG
                   };

DNSServer *DnsServer;
ESP8266WebServer *Webserver;

struct WEB {
  String chunk_buffer = "";                         // Could be max 2 * CHUNKED_BUFFER_SIZE
  uint32_t upload_size = 0;
  uint32_t light_shutter_button_mask;
  uint32_t buttons_non_light_non_shutter;
  uint32_t slider_update_time = 0;
  int slider[LST_MAX];
  int8_t shutter_slider[16];                        // MAX_SHUTTERS_ESP32
  uint16_t upload_error = 0;
  uint8_t state = HTTP_OFF;
  uint8_t upload_file_type;
  uint8_t config_block_count = 0;
  bool upload_services_stopped = false;
  bool reset_web_log_flag = false;                  // Reset web console log
  bool initial_config = false;
  bool cflg;
} Web;

/*********************************************************************************************/

// Helper function to avoid code duplication (saves 4k Flash)
// arg can be in PROGMEM
static void WebGetArg(const char* arg, char* out, size_t max) {
  String s = Webserver->arg((const __FlashStringHelper *)arg);
  strlcpy(out, s.c_str(), max);
//  out[max-1] = '\0';  // Ensure terminating NUL
}

/*-------------------------------------------------------------------------------------------*/

String AddWebCommand(const char* command, const char* arg, const char* dflt) {
/*
  // OK but fixed max argument
  char param[200];                             // Allow parameter with lenght up to 199 characters
  WebGetArg(arg, param, sizeof(param));
  uint32_t len = strlen(param);
  char cmnd[232];
  snprintf_P(cmnd, sizeof(cmnd), PSTR(";%s %s"), command, (0 == len) ? dflt : (StrCaseStr_P(command, PSTR("Password")) && (len < 5)) ? "" : param);
  return String(cmnd);
*/
/*
  // Any argument size (within stack space) +48 bytes
  String param = Webserver->arg((const __FlashStringHelper *)arg);
  uint32_t len = param.length();
//  char cmnd[len + strlen_P(command) + strlen_P(dflt) + 4];
  char cmnd[64 + len];
  snprintf_P(cmnd, sizeof(cmnd), PSTR(";%s %s"), command, (0 == len) ? dflt : (StrCaseStr_P(command, PSTR("Password")) && (len < 5)) ? "" : param.c_str());
  return String(cmnd);
*/
  // Any argument size (within heap space) +24 bytes
  // Exception (3) if not first moved from flash to stack
  // Exception (3) if not using __FlashStringHelper
  // Exception (3) if not FPSTR()
//  char rcommand[strlen_P(command) +1];
//  snprintf_P(rcommand, sizeof(rcommand), command);
//  char rdflt[strlen_P(dflt) +1];
//  snprintf_P(rdflt, sizeof(rdflt), dflt);
  String result = F(";");
//  result += rcommand;
//  result += (const __FlashStringHelper *)command;
  result += FPSTR(command);
  result += F(" ");
  String param = Webserver->arg(FPSTR(arg));
  uint32_t len = param.length();
  if (0 == len) {
//    result += rdflt;
//    result += (const __FlashStringHelper *)dflt;
    result += FPSTR(dflt);
  }
  else if (!(StrCaseStr_P(command, PSTR("Password")) && (len < 5))) {
    result += param;
  }
  return result;
}

/*-------------------------------------------------------------------------------------------*/

static bool WifiIsInManagerMode(void) {
  return (HTTP_MANAGER == Web.state || HTTP_MANAGER_RESET_ONLY == Web.state);
}

/*-------------------------------------------------------------------------------------------*/

void ShowWebSource(uint32_t source) {
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    AddLog(LOG_LEVEL_DEBUG, PSTR("SRC: %s from %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource), Webserver->client().remoteIP().toString().c_str());
  }
}

/*-------------------------------------------------------------------------------------------*/

void ExecuteWebCommand(char* svalue, uint32_t source = SRC_WEBGUI);
void ExecuteWebCommand(char* svalue, uint32_t source) {
  ShowWebSource(source);
  TasmotaGlobal.last_source = source;
  ExecuteCommand(svalue, SRC_IGNORE);
}

/*-------------------------------------------------------------------------------------------*/

// replace the series of `Webserver->on()` with a table in PROGMEM
typedef struct WebServerDispatch_t {
  char uri[3];   // the prefix "/" is added automatically
  uint8_t method;
  void (*handler)(void);
} WebServerDispatch_t;

const WebServerDispatch_t WebServerDispatch[] PROGMEM = {
  { "",   HTTP_ANY, HandleRoot },
  { "up", HTTP_ANY, HandleUpgradeFirmware },
  { "u1", HTTP_ANY, HandleUpgradeFirmwareStart },   // OTA
  { "u2", HTTP_OPTIONS, HandlePreflightRequest },
  { "u3", HTTP_ANY, HandleUploadDone },
#ifdef ESP32
  { "u4", HTTP_GET, HandleSwitchBootPartition },
#endif // ESP32
  { "mn", HTTP_GET, HandleManagement },
  { "cs", HTTP_GET, HandleConsole },
  { "cs", HTTP_OPTIONS, HandlePreflightRequest },
  { "cm", HTTP_ANY, HandleHttpCommand },
#ifndef FIRMWARE_MINIMAL
  { "cn", HTTP_ANY, HandleConfiguration },
  { "md", HTTP_ANY, HandleModuleConfiguration },
  { "wi", HTTP_ANY, HandleWifiConfiguration },
  { "lg", HTTP_ANY, HandleLoggingConfiguration },
  { "tp", HTTP_ANY, HandleTemplateConfiguration },
  { "co", HTTP_ANY, HandleOtherConfiguration },
  { "dl", HTTP_ANY, HandleBackupConfiguration },
  { "rs", HTTP_ANY, HandleRestoreConfiguration },
  { "rt", HTTP_ANY, HandleResetConfiguration },
  { "in", HTTP_ANY, HandleInformation }
#endif  // Not FIRMWARE_MINIMAL
};

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void WebServer_on(const char * prefix, void (*func)(void), uint8_t method = HTTP_ANY) {
  if (Webserver == nullptr) { return; }
#ifdef ESP8266
  Webserver->on((const __FlashStringHelper *) prefix, (HTTPMethod) method, func);
#endif  // ESP8266
#ifdef ESP32
  Webserver->on(prefix, (HTTPMethod) method, func);
#endif  // ESP32
}

#ifdef ESP32
void WebServer_removeRoute(const char * prefix, uint8_t method = HTTP_ANY) {
  if (Webserver == nullptr) { return; }
  Webserver->removeRoute(prefix, (HTTPMethod) method);
}
#endif  // ESP32

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

// Always listens to all interfaces, so we don't need an IP address anymore
void StartWebserver(int type) {
  if (!Settings->web_refresh) { Settings->web_refresh = HTTP_REFRESH_TIME; }
  if (!Web.state) {

    for (uint32_t i = 0; i < LST_MAX; i++) {
      Web.slider[i] = -1;
    }
    for (uint32_t i = 0; i < sizeof(Web.shutter_slider); i++) {
      Web.shutter_slider[i] = -1;
    }

    if (!Webserver) {
      Webserver = new ESP8266WebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);

      const char* headerkeys[] = { "Referer", "Host" };
      size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
      Webserver->collectHeaders(headerkeys, headerkeyssize);

      // call `Webserver->on()` on each entry
      for (uint32_t i=0; i<nitems(WebServerDispatch); i++) {
        const WebServerDispatch_t & line = WebServerDispatch[i];
        // copy uri in RAM and prefix with '/'
        char uri[4];
        uri[0] = '/';
        uri[1] = pgm_read_byte(&line.uri[0]);
        uri[2] = pgm_read_byte(&line.uri[1]);
        uri[3] = '\0';
        // register
        WebServer_on(uri, line.handler, pgm_read_byte(&line.method));
      }
      Webserver->onNotFound(HandleNotFound);
//      Webserver->on(F("/u2"), HTTP_POST, HandleUploadDone, HandleUploadLoop);  // this call requires 2 functions so we keep a direct call
      Webserver->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);  // this call requires 2 functions so we keep a direct call
#ifndef FIRMWARE_MINIMAL
#ifdef USE_ALPINEJS
      Webserver->on("/alpinejs", HTTP_ANY, HandleAlpinejsRequest);
#endif // USE_ALPINEJS
      XdrvXsnsCall(FUNC_WEB_ADD_HANDLER);
#endif  // Not FIRMWARE_MINIMAL

      if (!Web.initial_config) {
        Web.initial_config = (!strlen(SettingsText(SET_STASSID1)) && !strlen(SettingsText(SET_STASSID2)));
        if (Web.initial_config) { AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Blank Device - Initial Configuration")); }
      }
    }
    Web.reset_web_log_flag = false;

    Webserver->begin(); // Web server start
  }
  if (Web.state != type) {
    AddLogServerActive(PSTR(D_LOG_HTTP "Web"));
    TasmotaGlobal.rules_flag.http_init = 1;
    Web.state = type;
  }
}

/*-------------------------------------------------------------------------------------------*/

void StopWebserver(void) {
  if (Web.state) {
    Webserver->close();
    Web.state = HTTP_OFF;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

/*-------------------------------------------------------------------------------------------*/

void WifiManagerBegin(bool reset_only) {
  // setup AP
  if (!Web.initial_config) { AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES)); }
  if (!TasmotaGlobal.global_state.wifi_down) {
    WifiSetMode(WIFI_AP_STA);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
    WifiSetMode(WIFI_AP);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
  }

  //StopWebserver();

  DnsServer = new DNSServer();

  int channel = WIFI_SOFT_AP_CHANNEL;
  if ((channel < 1) || (channel > 13)) { channel = 1; }

  // bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
  WiFi.softAP(TasmotaGlobal.hostname, WIFI_AP_PASSPHRASE, channel, 0, 1);
  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  StartWebserver((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER));
}

/*-------------------------------------------------------------------------------------------*/

void PollDnsWebserver(void) {
  if (DnsServer) { DnsServer->processNextRequest(); }
  if (Webserver) { Webserver->handleClient(); }
}

/*********************************************************************************************/

bool WebAuthenticate(void) {
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return Webserver->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  } else {
    return true;
  }
}

/*-------------------------------------------------------------------------------------------*/

bool HttpCheckPriviledgedAccess(bool autorequestauth = true) {
  if (HTTP_USER == Web.state) {
    HandleRoot();
    return false;
  }
  if (autorequestauth && !WebAuthenticate()) {
    Webserver->requestAuthentication();
    return false;
  }

  if (!Settings->flag5.disable_referer_chk && !WifiIsInManagerMode()) {
    String referer = Webserver->header(F("Referer"));  // http://demo/? or http://192.168.2.153/?
    if (referer.length()) {
      referer.toUpperCase();
      String hostname = TasmotaGlobal.hostname;
      hostname.toUpperCase();
      // TODO rework if IPv6
      if ((referer.indexOf(hostname) == 7) || (referer.indexOf(WiFi.localIP().toString()) == 7)) {
        return true;
      }
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
      hostname = EthernetHostname();
      hostname.toUpperCase();
      // TODO rework if IPv6
      if ((referer.indexOf(hostname) == 7) || (referer.indexOf(EthernetLocalIP().toString()) == 7)) {
        return true;
      }
#endif  // USE_ETHERNET
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "Referer '%s' denied. Use 'SO128 1' for HTTP API commands. 'Webpassword' is recommended."), referer.c_str());
    return false;
  } else {
#if defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)
    MI32suspendScanTask();
#endif // defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)
    return true;
  }
}

#ifdef USE_CORS
/*-------------------------------------------------------------------------------------------*/

void HttpHeaderCors(void) {
  if (strlen(SettingsText(SET_CORS))) {
    Webserver->sendHeader(F("Access-Control-Allow-Origin"), SettingsText(SET_CORS));
  }
}
#endif

/*-------------------------------------------------------------------------------------------*/

void WSHeaderSend(void) {
  char server[32];
  snprintf_P(server, sizeof(server), PSTR("Tasmota/%s (%s)"), TasmotaGlobal.version, GetDeviceHardware().c_str());
  Webserver->sendHeader(F("Server"), server);
  Webserver->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  Webserver->sendHeader(F("Pragma"), F("no-cache"));
  Webserver->sendHeader(F("Expires"), F("-1"));
#ifdef USE_CORS
  HttpHeaderCors();
#endif
}

/*********************************************************************************************\
 * HTTP Content Page handler
\*********************************************************************************************/

void WSSend(int code, int ctype, const String& content)
{
  char ct[25];  // strlen("application/octet-stream") +1 = Longest Content type string
  Webserver->send(code, GetTextIndexed(ct, sizeof(ct), ctype, kContentTypes), content);
}

/*********************************************************************************************\
 * HTTP Content Chunk handler
\*********************************************************************************************/

void WSContentBegin(int code, int ctype) {
  Webserver->client().flush();
  WSHeaderSend();
  Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN);
  WSSend(code, ctype, "");                         // Signal start of chunked content
  Web.chunk_buffer = "";
}

/*-------------------------------------------------------------------------------------------*/

void _WSContentSend(const char* content, size_t size) {  // Lowest level sendContent for all core versions
  Webserver->sendContent(content, size);

  SHOW_FREE_MEM(PSTR("WSContentSend"));
  DEBUG_CORE_LOG(PSTR("WEB: Chunk size %d"), size);
}

/*-------------------------------------------------------------------------------------------*/

void _WSContentSend(const String& content) {       // Low level sendContent for all core versions
  _WSContentSend(content.c_str(), content.length());
}

/*-------------------------------------------------------------------------------------------*/

void WSContentFlush(void) {
  if (Web.chunk_buffer.length() > 0) {
    _WSContentSend(Web.chunk_buffer);              // Flush chunk buffer
    Web.chunk_buffer = "";
  }
}

/*-------------------------------------------------------------------------------------------*/

void _WSContentSendBufferChunk_P(const char* content) {
  int len = strlen_P(content);
  if (len < CHUNKED_BUFFER_SIZE) {                 // Append chunk buffer with small content
    Web.chunk_buffer += (const __FlashStringHelper *)content;
    len = Web.chunk_buffer.length();
  }
  if (len >= CHUNKED_BUFFER_SIZE) {                // Either content or chunk buffer is oversize
    WSContentFlush();                              // Send chunk buffer before possible content oversize
  }
  len = strlen_P(content);
  if (len >= CHUNKED_BUFFER_SIZE) {                // Content is oversize
    _WSContentSend(content, len);                  // Send content
  }
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSendRaw_P(const char* content) {     // Content sent without formatting
  if (nullptr == content || !strlen_P(content)) { return; }

  WSContentSeparator(2);                           // Print separator on next WSContentSeparator(1)
  _WSContentSendBufferChunk_P(content);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend(const char* content, size_t size) {
  // To speed up transmission use chunked buffer if possible
  if (size < CHUNKED_BUFFER_SIZE) {
    // Terminate non-terminated content
    char buffer[size +1];
    strlcpy(buffer, content, sizeof(buffer));      // Terminate with '\0'
    _WSContentSendBufferChunk_P(buffer);
  } else {
    WSContentFlush();                              // Flush chunk buffer
    _WSContentSend(content, size);
  }
}

/*-------------------------------------------------------------------------------------------*/

void _WSContentSendBuffer(bool decimal, const char * formatP, va_list arg) {
  char* content = ext_vsnprintf_malloc_P(formatP, arg);
  if (content == nullptr) { return; }              // Avoid crash

  int len = strlen(content);
  if (decimal && (D_DECIMAL_SEPARATOR[0] != '.')) {
    for (uint32_t i = 0; i < len; i++) {
      if ('.' == content[i]) {
        content[i] = D_DECIMAL_SEPARATOR[0];
      }
    }
  }

  WSContentSendRaw_P(content);
  free(content);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_P(const char* formatP, ...) {   // Content send snprintf_P char data
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  _WSContentSendBuffer(false, formatP, arg);
  va_end(arg);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_PD(const char* formatP, ...) {  // Content send snprintf_P char data checked for decimal separator
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  _WSContentSendBuffer(true, formatP, arg);
  va_end(arg);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentStart_P(const char* title, bool auth) {
  if (auth && !WebAuthenticate()) {
    return Webserver->requestAuthentication();
  }

  WSContentBegin(200, CT_HTML);

  if (title != nullptr) {
    WSContentSend_P(HTTP_HEADER1, PSTR(D_HTML_LANGUAGE), SettingsTextEscaped(SET_DEVICENAME).c_str(), title);
  }
}

/*-------------------------------------------------------------------------------------------*/

void WSContentStart_P(const char* title) {
  WSContentStart_P(title, true);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSendStyle_P(const char* formatP, ...) {
  if ( WifiIsInManagerMode() && (!Web.initial_config) ) {
    if (WifiConfigCounter()) {
      WSContentSend_P(HTTP_SCRIPT_COUNTER);
    }
  }
#ifdef ESP32
  WSContentSend_P(HTTP_HEAD_LAST_SCRIPT32);
#else
  WSContentSend_P(HTTP_HEAD_LAST_SCRIPT);
#endif

  // Output style root colors by names
  WSContentSend_P(HTTP_HEAD_STYLE_ROOT_COLOR,
                  WebColor(COL_BACKGROUND),           // --c_bg
                  WebColor(COL_FORM),                 // --c_frm
                  WebColor(COL_TITLE),                // --c_ttl
                  WebColor(COL_TEXT),                 // --c_txt
                  WebColor(COL_TEXT_WARNING),         // --c_txtwrn
                  WebColor(COL_TEXT_SUCCESS),         // --c_txtscc
                  WebColor(COL_BUTTON),               // --c_btn
                  WebColor(COL_BUTTON_OFF),           // --c_btnoff
                  WebColor(COL_BUTTON_TEXT),          // --c_btntxt
                  WebColor(COL_BUTTON_HOVER),         // --c_btnhvr
                  WebColor(COL_BUTTON_RESET),         // --c_btnrst
                  WebColor(COL_BUTTON_RESET_HOVER),   // --c_btnrsthvr
                  WebColor(COL_BUTTON_SAVE),          // --c_btnsv
                  WebColor(COL_BUTTON_SAVE_HOVER),    // --c_btnsvhvr
                  WebColor(COL_INPUT),                // --c_in
                  WebColor(COL_INPUT_TEXT),           // --c_intxt
                  WebColor(COL_CONSOLE),              // --c_csl
                  WebColor(COL_CONSOLE_TEXT),         // --c_csltxt
                  WebColor(COL_TIMER_TAB_BACKGROUND), // --c_tab
                  WebColor(COL_TIMER_TAB_TEXT)        // --c_tabtxt
  );

  WSContentSendRaw_P(HTTP_HEAD_STYLE1);
  WSContentSendRaw_P(HTTP_HEAD_STYLE2);

#ifdef USE_WEB_STATUS_LINE_WIFI
  WSContentSendRaw_P(HTTP_HEAD_STYLE_WIFI);
#endif
#if defined(USE_ZIGBEE) || defined(USE_LORAWAN_BRIDGE)
  WSContentSendRaw_P(HTTP_HEAD_STYLE_ZIGBEE);
#endif // USE_ZIGBEE
  if (formatP != nullptr) {
    // This uses char strings. Be aware of sending %% if % is needed
    va_list arg;
    va_start(arg, formatP);
    _WSContentSendBuffer(false, formatP, arg);
    va_end(arg);
  }
  if (strlen(SettingsText(SET_CANVAS))) {
//    WSContentSend_P(PSTR("body{background:%s;background-repeat:no-repeat;background-attachment:fixed;background-size:cover;}"), SettingsText(SET_CANVAS));
    WSContentSend_P(PSTR("body{background:%s 0 0 / cover no-repeat fixed;}"), SettingsText(SET_CANVAS));
  }
#ifdef FIRMWARE_MINIMAL
  WSContentSend_P(HTTP_HEAD_STYLE3_MINIMAL,
    (Web.initial_config) ? "" : (Settings->flag5.gui_module_name) ? "" : ModuleName().c_str(),  // SetOption141 - (GUI) Disable display of GUI module name (1)
    (Settings->flag6.gui_device_name) ? "" : SettingsTextEscaped(SET_DEVICENAME).c_str());      // SetOption163 - (GUI) Disable display of GUI device name (1)
#else // FIRMWARE_MINIMAL
  WSContentSend_P(HTTP_HEAD_STYLE3,
    PSTR(D_NOSCRIPT),
    (Web.initial_config) ? "" : (Settings->flag5.gui_module_name) ? "" : ModuleName().c_str(),  // SetOption141 - (GUI) Disable display of GUI module name (1)
    (Settings->flag6.gui_device_name) ? "" : SettingsTextEscaped(SET_DEVICENAME).c_str());      // SetOption163 - (GUI) Disable display of GUI device name (1)

#endif // FIRMWARE_MINIMAL

  // SetOption53 - Show hostname and IP address in GUI main menu
#if (RESTART_AFTER_INITIAL_WIFI_CONFIG)
  if (Settings->flag3.gui_hostname_ip) {            // SetOption53 - (GUI) Show hostname and IP address in GUI main menu
#else
  if ( Settings->flag3.gui_hostname_ip || ( (WiFi.getMode() == WIFI_AP_STA) && (!Web.initial_config) )  ) {
#endif
    bool lip = WifiHasIP();
    bool sip = (static_cast<uint32_t>(WiFi.softAPIP()) != 0);
    bool eip = false;
    if (lip || sip) {
      WSContentSend_P(PSTR("<h4>%s%s (%s%s%s)"),    // tasmota.local (192.168.2.12, 192.168.4.1)
        TasmotaGlobal.hostname,
        (Mdns.begun) ? PSTR(".local") : "",
        (lip) ? WiFi.localIP().toString().c_str() : "",
        (lip && sip) ? ", " : "",
        (sip) ? WiFi.softAPIP().toString().c_str() : "");
    }
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
    eip = EthernetHasIP();
    if (eip) {
      WSContentSend_P(PSTR("%s%s%s (%s)"),          // tasmota-eth.local (192.168.2.13)
        (lip || sip) ? PSTR("</br>") : PSTR("<h4>"),
        EthernetHostname(),
        (Mdns.begun) ? PSTR(".local") : "",
        (eip) ? EthernetLocalIP().toString().c_str() : "");
    }
#endif
    if (lip || sip || eip) {
      WSContentSend_P(PSTR("</h4>"));
    }
  }
  WSContentSend_P(PSTR("</div>"));
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSendStyle(void) {
  WSContentSendStyle_P(nullptr);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentTextCenterStart(uint32_t color) {
  WSContentSend_P(PSTR("<div style='text-align:center;color:#%06x;'>"), color);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentButton(uint32_t title_index, bool show=true) {
  char action[4];
  char title[100];  // Large to accomodate UTF-16 as used by Russian

  WSContentSend_P(PSTR("<p></p><form id=but%d style=\"display:%s;\" action='%s' method='get'"),
    title_index,
    show ? "block":"none",
    GetTextIndexed(action, sizeof(action), title_index, kButtonAction));
  if (title_index <= BUTTON_RESET_CONFIGURATION) {
    char confirm[100];
    WSContentSend_P(PSTR(" onsubmit='return confirm(\"%s\");'><button name='%s' class='button bred'>%s</button></form>"),
      GetTextIndexed(confirm, sizeof(confirm), title_index, kButtonConfirm),
      (!title_index) ? PSTR("rst") : PSTR("non"),
      GetTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  } else {
    WSContentSend_P(PSTR("><button>%s</button></form>"),
      GetTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  }
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSpaceButton(uint32_t title_index, bool show=true) {
  WSContentSend_P(PSTR("<div></div>"));             // 5px padding
  WSContentButton(title_index, show);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSeparator(uint32_t state) {
  // Send two column separator
  static bool request = false;
  switch (state) {
    case 0:    // Print separator (fall through to WSContentSeparator(1))
      request = true;
    case 1:    // Print separator if needed
      if (request) {
        WSContentSend_P(HTTP_SNS_HR);  // <tr><td colspan=2><hr>{e}
        request = false;
      }
      break;
    case 2:    // Print separator on next WSContentSeparator(1)
      request = true;
      break;
    case 3:    // Don't print separator on next WSContentSeparator(1)
      request = false;
      break;
  }
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_Temp(const char *types, float f_temperature) {
  WSContentSend_PD(HTTP_SNS_F_TEMP, types, Settings->flag2.temperature_resolution, &f_temperature, TempUnit());
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_Voltage(const char *types, float f_voltage) {
  WSContentSend_PD(HTTP_SNS_F_VOLTAGE, types, Settings->flag2.voltage_resolution, &f_voltage);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_Current(const char *types, float f_current) {
  WSContentSend_PD(HTTP_SNS_F_CURRENT, types, Settings->flag2.current_resolution, &f_current);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSend_THD(const char *types, float f_temperature, float f_humidity) {
  WSContentSend_Temp(types, f_temperature);

  char parameter[FLOATSZ];
  dtostrfd(f_humidity, Settings->flag2.humidity_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_HUM, types, parameter);
  dtostrfd(CalcTempHumToDew(f_temperature, f_humidity), Settings->flag2.temperature_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_DEW, types, parameter, TempUnit());
#ifdef USE_HEAT_INDEX
  dtostrfd(CalcTemHumToHeatIndex(f_temperature, f_humidity), Settings->flag2.temperature_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_HEATINDEX, types, parameter, TempUnit());
#endif  // USE_HEAT_INDEX
}

/*-------------------------------------------------------------------------------------------*/

void WSContentEnd(void) {
  WSContentFlush();                                // Flush chunk buffer
//  _WSContentSend("");                              // Signal end of chunked content using multiple writes

  // Fix UDP response #23613
  const char *footer_empty = "0\r\n\r\n";
  Webserver->client().write(footer_empty, 5);      // Signal end of chunked content in one write (doesn't clear core _chunked)
  delay(5);

  Webserver->client().stop();
#if defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)
  MI32resumeScanTask();
#endif // defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)
}

/*-------------------------------------------------------------------------------------------*/

void WSContentStop(void) {
  if ( WifiIsInManagerMode() && (!Web.initial_config) ) {
    if (WifiConfigCounter()) {
      WSContentSend_P(HTTP_COUNTER);
    }
  }
  WSContentSend_P(HTTP_END, TasmotaGlobal.version, TasmotaGlobal.image_name);
  WSContentEnd();
}

#ifdef USE_WEB_STATUS_LINE
/*-------------------------------------------------------------------------------------------*/
// Display a Sticker in the Status bar (upper right) with a name (MQTT, TlS, VPN)
// and an optional tooltip text to indicate the duration of the connection (or -1 if none)
//
// Convert seconds to a string representing days, hours or minutes.
// The string will contain the most coarse time only, rounded down (61m == 01h, 01h37m == 01h).
void WSContentStatusSticker(const char *msg, const char *attr = NULL);
void WSContentStatusSticker(const char *msg, const char *attr)
{
  if (msg == NULL) { return; }
  if (attr == NULL) { attr = ""; }
  WSContentSend_P(HTTP_STATUS_STICKER, 0xAAAAAA, attr, msg);
}
#endif // USE_WEB_STATUS_LINE

/*********************************************************************************************/

void WebRestart(uint32_t type) {
  // type 0 = restart
  // type 1 = restart after config change
  // type 2 = Checking WiFi Connection - no restart, only refresh page.
  // type 3 = restart after WiFi Connection Test Successful
  // type 4 = type 0 without auto switch to production
  bool prep_switch_partition = false;
  if (0 == type) { prep_switch_partition = true; }
  if (4 == type) { type = 0; }

  bool reset_only = (HTTP_MANAGER_RESET_ONLY == Web.state);

  WSContentStart_P((type) ? PSTR(D_SAVE_CONFIGURATION) : PSTR(D_RESTART), !reset_only);
#if ((RESTART_AFTER_INITIAL_WIFI_CONFIG) && (AFTER_INITIAL_WIFI_CONFIG_GO_TO_NEW_IP))
  // In case of type 3 (New network has been configured) go to the new device's IP in the new Network
  if (3 == type) {
    WSContentSend_P("setTimeout(function(){location.href='http://%s';},%d);",
      IPForUrl(WiFi.localIP()).c_str(),
      HTTP_RESTART_RECONNECT_TIME
    );
  } else {
    WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_RESTART_RECONNECT_TIME);
  }
#else
  // In case of type 3 (New network has been configured) do not refresh the page. Just halt.
  // The IP of the device while was in AP mode, won't be the new IP of the newly configured Network.
  if (!(3 == type)) { WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_RESTART_RECONNECT_TIME); }
#endif
  WSContentSendStyle();
  if (type) {
    if (!(3 == type)) {
      WSContentSend_P(PSTR("<div style='text-align:center;'><b>%s</b><br><br></div>"), (type==2) ? PSTR(D_TRYING_TO_CONNECT) : PSTR(D_CONFIGURATION_SAVED) );
    } else {
#if (AFTER_INITIAL_WIFI_CONFIG_GO_TO_NEW_IP)
      WSContentTextCenterStart(WebColor(COL_TEXT_SUCCESS));
      WSContentSend_P(PSTR(D_SUCCESSFUL_WIFI_CONNECTION "<br><br></div><div style='text-align:center;'>" D_REDIRECTING_TO_NEW_IP "<br><br><a href='http://%_I'>%_I</a><br></div>"),(uint32_t)WiFi.localIP(),(uint32_t)WiFi.localIP());
#else
      WSContentTextCenterStart(WebColor(COL_TEXT_SUCCESS));
      WSContentSend_P(PSTR(D_SUCCESSFUL_WIFI_CONNECTION "<br><br></div><div style='text-align:center;'>" D_NOW_YOU_CAN_CLOSE_THIS_WINDOW "<br><br></div>"));
#endif
    }
  }
  if (type < 2) {
    WSContentSend_P(HTTP_MSG_RSTRT);
    if (HTTP_MANAGER == Web.state || reset_only) {
      Web.state = HTTP_ADMIN;
    } else {
      WSContentSpaceButton(BUTTON_MAIN);
    }
  }
  if (!(2 == type)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESTART));
    ShowWebSource(SRC_WEBGUI);
#ifdef ESP32
    if (prep_switch_partition) { EspPrepSwitchPartition(1); }  // Switch to production partition if on safeboot
#endif  // ESP32
    TasmotaGlobal.restart_flag = 2;
  }
  WSContentStop();
}

/*********************************************************************************************/

uint32_t WebUseManagementSubmenu(void) {
  static uint32_t management_count = 0;

  if (!management_count) {
    XdrvMailbox.index = 1;
    XdrvXsnsCall(FUNC_WEB_ADD_CONSOLE_BUTTON);
    XdrvCall(FUNC_WEB_ADD_MANAGEMENT_BUTTON);
    management_count = XdrvMailbox.index;
  }
  return management_count -1;
}

/*********************************************************************************************\
 * HandleRoot
\*********************************************************************************************/

void HandleWifiLogin(void) {
  WSContentStart_P(PSTR(D_CONFIGURE_WIFI), false);  // false means show page no matter if the client has or has not credentials
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_LOGIN);

  if (HTTP_MANAGER_RESET_ONLY == Web.state) {
    WSContentSpaceButton(BUTTON_RESTART);
#ifndef FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_RESET_CONFIGURATION);
#endif  // FIRMWARE_MINIMAL
  }

  WSContentStop();
}

#ifdef USE_SHUTTER
/*-------------------------------------------------------------------------------------------*/

int32_t IsShutterWebButton(uint32_t idx) {
  /* 0: Not a shutter, 1..4: shutter up idx, -1..-4: shutter down idx */
  int32_t ShutterWebButton = 0;
  if (Settings->flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    for (uint32_t i = 0; i < TasmotaGlobal.shutters_present ; i++) {
      if (ShutterGetStartRelay(i) && ((ShutterGetStartRelay(i) == idx) || (ShutterGetStartRelay(i) == (idx-1)))) {
        ShutterWebButton = (ShutterGetStartRelay(i) == idx) ? (i+1): (-1-i);
        break;
      }
    }
  }
  return ShutterWebButton;
}
#endif // USE_SHUTTER

/*-------------------------------------------------------------------------------------------*/

void WebGetDeviceCounts(void) {
  Web.buttons_non_light_non_shutter = TasmotaGlobal.devices_present;
  Web.light_shutter_button_mask = 0;       // Bitmask for each light and/or shutter button

#ifdef USE_LIGHT
  // Chk for reduced toggle buttons used by lights
  if (TasmotaGlobal.light_type) {
    // Find and skip light buttons
    uint32_t light_device = LightDevice();
    uint32_t light_devices = LightDevices();
    for (uint32_t button_idx = light_device; button_idx < (light_device + light_devices); button_idx++) {
      Web.buttons_non_light_non_shutter--;
      Web.light_shutter_button_mask |= (1 << (button_idx -1));  // Set button bit in bitmask
    }
  }
#endif  // USE_LIGHT

#ifdef USE_SHUTTER
  // Chk for reduced toggle buttons used by shutters
  if (Settings->flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    // Find and skip dedicated shutter buttons
    for (uint32_t button_idx = 1; button_idx <= TasmotaGlobal.devices_present; button_idx++) {
      if (IsShutterWebButton(button_idx) != 0) {
        Web.buttons_non_light_non_shutter--;
        Web.light_shutter_button_mask |= (1 << (button_idx -1));  // Set button bit in bitmask
      }
    }
  }
#endif  // USE_SHUTTER

//  AddLog(LOG_LEVEL_DEBUG, PSTR("HTP: DP %d, BNLNS %d, SB %08X"), TasmotaGlobal.devices_present, Web.buttons_non_light_non_shutter, Web.light_shutter_button_mask);
}

#ifdef USE_LIGHT
/*-------------------------------------------------------------------------------------------*/

void WebSliderColdWarm(void) {
  Web.slider[0] = LightGetColorTemp();
  WSContentSend_P(PSTR("<tr>"));
  WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
    2, 100,
    PSTR("a"),       // a - Unique HTML id
    PSTR("#eff"), PSTR("#f81"),  // 6500k in RGB (White) to 2500k in RGB (Warm Yellow)
    1,               // sl1 - used for slider updates
    153, 500,        // Range color temperature
    Web.slider[0],
    't', 0);         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));
  WSContentSend_P(PSTR("</tr>"));
}
#endif  // USE_LIGHT

/*-------------------------------------------------------------------------------------------*/

void HandleRoot(void) {
#ifndef NO_CAPTIVE_PORTAL
  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the page.
#endif  // NO_CAPTIVE_PORTAL

  if (Webserver->hasArg(F("rst"))) {
    WebRestart(0);
    return;
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    if (strlen(SettingsText(SET_WEBPWD)) && !(Webserver->hasArg(F("USER1"))) && !(Webserver->hasArg(F("PASS1"))) && HTTP_MANAGER_RESET_ONLY != Web.state) {
      HandleWifiLogin();
    } else {
      if (!strlen(SettingsText(SET_WEBPWD)) || (((Webserver->arg(F("USER1")) == WEB_USERNAME ) && (Webserver->arg(F("PASS1")) == SettingsText(SET_WEBPWD) )) || HTTP_MANAGER_RESET_ONLY == Web.state)) {
        HandleWifiConfiguration();
      } else {
        // wrong user and pass
        HandleWifiLogin();
      }
    }
#endif  // Not FIRMWARE_MINIMAL
    return;
  }

  if (HandleRootStatusRefresh()) {
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MAIN_MENU));

  /*
  Display GUI with items in following order:
  - Header with module name and device name
  - Dynamic ajax update region
  - Optional power toggle buttons for relays, display and iFan with feedback
  - Optional shutter buttons and slider with feedback
  - Optional light button and slider(s) with feedback
  - Call FUNC_WEB_ADD_MAIN_BUTTON
    - Optional buttons and sliders with feedback
  - Show default main buttons (Configuration, Information, Firmware Upgrade, Tools and Restart)
  */

  char stemp[33];

  WSContentStart_P(PSTR(D_MAIN_MENU));
#ifdef USE_SCRIPT_WEB_DISPLAY
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings->web_refresh, Settings->web_refresh);
#else
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings->web_refresh);
#endif
  WSContentSend_P(HTTP_SCRIPT_ROOT_PART2);
  WSContentSendStyle();

  WSContentSend_P(PSTR("<div style='padding:0;' id='l1' name='l1'></div><div></div>"));

#ifndef FIRMWARE_MINIMAL

  if (TasmotaGlobal.devices_present) {
    WebGetDeviceCounts();

    if (Web.buttons_non_light_non_shutter) {   // Any non light AND non shutter button - Show toggle buttons
      WSContentSend_P(HTTP_TABLE100);      // "<table style='width:100%%'>"
      WSContentSend_P(PSTR("<tr>"));

#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan()) {
        WSContentSend_P(HTTP_DEVICE_CONTROL, 36, 1, 1,
          (strlen(SettingsText(SET_BUTTON1))) ? SettingsTextEscaped(SET_BUTTON1).c_str() : PSTR(D_BUTTON_TOGGLE),
          "");
        for (uint32_t i = 0; i < MaxFanspeed(); i++) {
          snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i);
          WSContentSend_P(HTTP_DEVICE_CONTROL, 16, i +2, i +2,
            (strlen(SettingsText(SET_BUTTON2 + i))) ? SettingsTextEscaped(SET_BUTTON2 + i).c_str() : stemp,
            "");
        }
      } else {
#endif  // USE_SONOFF_IFAN

        const uint32_t max_columns = 8;
        uint32_t rows = Web.buttons_non_light_non_shutter / max_columns;
        if (Web.buttons_non_light_non_shutter % max_columns) { rows++; }
        uint32_t cols = Web.buttons_non_light_non_shutter / rows;
        if (Web.buttons_non_light_non_shutter % rows) { cols++; }

        uint32_t button_ptr = 0;
        for (uint32_t button_idx = 1; button_idx <= TasmotaGlobal.devices_present; button_idx++) {
          if (bitRead(Web.light_shutter_button_mask, button_idx -1)) { continue; }  // Skip non-sequential light and/or shutter button
          bool set_button = ((button_idx <= MAX_BUTTON_TEXT) && strlen(GetWebButton(button_idx -1)));
          snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), button_idx);
          WSContentSend_P(HTTP_DEVICE_CONTROL, 100 / cols, button_idx, button_idx,
            (set_button) ? HtmlEscape(GetWebButton(button_idx -1)).c_str() : (cols < 5) ? PSTR(D_BUTTON_TOGGLE) : "",
            (set_button) ? "" : (TasmotaGlobal.devices_present > 1) ? stemp : "");
          button_ptr++;
          if (0 == button_ptr % cols) { WSContentSend_P(PSTR("</tr><tr>")); }
        }
#ifdef USE_SONOFF_IFAN
      }
#endif  // USE_SONOFF_IFAN

      WSContentSend_P(PSTR("</tr></table>"));
    }

#ifdef USE_SHUTTER
    if (TasmotaGlobal.shutters_present) {  // Any shutter present - Show shutter buttons and slider
      WSContentSend_P(HTTP_TABLE100);      // "<table style='width:100%%'>"
      uint32_t shutter_button_idx;
      uint32_t shutter_button_idx_temp;
      for (uint32_t shutter_idx = 0; shutter_idx < TasmotaGlobal.shutters_present; shutter_idx++) {
        WSContentSend_P(PSTR("<tr>"));
        uint32_t shutter_options = ShutterGetOptions(shutter_idx);
        shutter_button_idx = ShutterGetStartRelay(shutter_idx) +1;  // Left button is next button first (down)
        for (uint32_t j = 0; j < 2; j++) {
          shutter_button_idx_temp = (shutter_options & 1) ? shutter_button_idx + (j * 2) - 1 : shutter_button_idx;  // Invert index
//          AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: j %d, shutter_idx %d, shutter_button_idx %d, shutter_idx %d, shutter_button_idx_temp %d"), j, shutter_idx, shutter_button_idx, shutter_idx, shutter_button_idx_temp);
          WSContentSend_P(HTTP_DEVICE_CONTROL, 15, shutter_button_idx_temp, shutter_button_idx_temp,
            ((shutter_options & 2) ? "-" :  // Is locked
            ((shutter_options & 1) ? (j ? "&#9660;" : "&#9650;") : (j ? "&#9650;" : "&#9660;"))),  // Invert web buttons
            "");

          if (1 == j) { break; }           // Both buttons shown

          shutter_button_idx--;            // Right button is previous button (up)
          bool set_button = ((shutter_button_idx <= MAX_BUTTON_TEXT) && strlen(GetWebButton(shutter_button_idx -1)));
          snprintf_P(stemp, sizeof(stemp), PSTR("Shutter %d"), shutter_idx +1);
          uint32_t shutter_real_to_percent_position = ShutterRealToPercentPosition(-9999, shutter_idx);
          Web.shutter_slider[shutter_idx] = (shutter_options & 1) ? (100 - shutter_real_to_percent_position) : shutter_real_to_percent_position;
          WSContentSend_P(HTTP_MSG_SLIDER_SHUTTER, 
            (set_button) ? HtmlEscape(GetWebButton(shutter_button_idx -1)).c_str() : stemp,
            shutter_idx +1,
            Web.shutter_slider[shutter_idx],
            shutter_idx +1);
        }
        WSContentSend_P(PSTR("</tr>"));
      }
      WSContentSend_P(PSTR("</table>"));
    }
#endif  // USE_SHUTTER

#ifdef USE_LIGHT
    if (TasmotaGlobal.light_type) {        // Any light - Show light button and slider(s)
      uint32_t light_device = LightDevice();
      uint32_t light_devices = LightDevices();
      uint32_t button_idx = light_device;

      WSContentSend_P(HTTP_TABLE100);      // "<table style='width:100%%'>"

      uint8_t light_subtype = TasmotaGlobal.light_type &7;
      if (!Settings->flag3.pwm_multi_channels) {  // SetOption68 0 - Enable multi-channels PWM instead of Color PWM
        bool split_white = ((LST_RGBW <= light_subtype) && (light_devices > 1) && (Settings->param[P_RGB_REMAP] & 128));  // Only on RGBW or RGBCW and SetOption37 128

        if ((LST_COLDWARM == light_subtype) || ((LST_RGBCW == light_subtype) && !split_white)) {
          WebSliderColdWarm();
        }

        if (light_subtype > 2) {           // No W or CW
          uint16_t hue;
          uint8_t sat;
          LightGetHSB(&hue, &sat, nullptr);

          Web.slider[1] = hue;
          WSContentSend_P(PSTR("<tr>"));
          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Hue
            2, 100,
            PSTR("b"),       // b - Unique HTML id
            PSTR("#800"), PSTR("#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800"),  // Hue colors
            2,               // sl2 - Unique range HTML id - Used as source for Saturation end color and slider updates
            0, 359,          // Range valid Hue
            Web.slider[1],
            'h', 0);         // h0 - Value id
          WSContentSend_P(PSTR("</tr>"));

          uint8_t dcolor = changeUIntScale(Settings->light_dimmer, 0, 100, 0, 255);
          char scolor[8];
          snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
          uint8_t red, green, blue;
          HsToRgb(hue, 255, &red, &green, &blue);
          snprintf_P(stemp, sizeof(stemp), PSTR("#%02X%02X%02X"), red, green, blue);  // Saturation end color

          Web.slider[2] = changeUIntScale(sat, 0, 255, 0, 100);
          WSContentSend_P(PSTR("<tr>"));
          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Saturation
            2, 100,
            PSTR("s"),       // s - Unique HTML id related to eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';
            scolor, stemp,   // Brightness to max current color
            3,               // sl3 - Unique range HTML id - Used for slider updates
            0, 100,          // Range 0 to 100%
            Web.slider[2],
            'n', 0);         // n0 - Value id
          WSContentSend_P(PSTR("</tr>"));
        }

        bool set_button = ((button_idx <= MAX_BUTTON_TEXT) && strlen(GetWebButton(button_idx -1)));
        char first[2];
        snprintf_P(first, sizeof(first), PSTR("%s"), PSTR(D_BUTTON_TOGGLE));
        char butt_txt[4];
        snprintf_P(butt_txt, sizeof(butt_txt), PSTR("%s"), (set_button) ? HtmlEscape(GetWebButton(button_idx -1)).c_str() : first);
        char number[8];
        WSContentSend_P(PSTR("<tr>"));
        WSContentSend_P(HTTP_DEVICE_CONTROL, 15, button_idx, button_idx,
          butt_txt,
          (set_button) ? "" : itoa(button_idx, number, 10));
        button_idx++;

        Web.slider[3] = Settings->light_dimmer;
        WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Brightness - Black to White
          1, 85,
          PSTR("c"),         // c - Unique HTML id
          PSTR("#000"), PSTR("#fff"),    // Black to White
          4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color and slider updates
          Settings->flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100% (SetOption77 - Do not power off if slider moved to far left)
          Web.slider[3],
          'd', 0);           // d0 - Value id is related to lc("d0", value) and WebGetArg("d0", tmp, sizeof(tmp));
        WSContentSend_P(PSTR("</tr>"));

        if (split_white) {   // SetOption37 128
          if (LST_RGBCW == light_subtype) {
            WebSliderColdWarm();
          }

          uint32_t width = 100;
          WSContentSend_P(PSTR("<tr>"));

          if (button_idx < (light_device + light_devices)) {
            bool set_button = ((button_idx <= MAX_BUTTON_TEXT) && strlen(GetWebButton(button_idx -1)));
            char first[2];
            snprintf_P(first, sizeof(first), PSTR("%s"), PSTR(D_BUTTON_TOGGLE));
            char butt_txt[4];
            snprintf_P(butt_txt, sizeof(butt_txt), PSTR("%s"), (set_button) ? HtmlEscape(GetWebButton(button_idx -1)).c_str() : first);
            char number[8];
            WSContentSend_P(HTTP_DEVICE_CONTROL, 15, button_idx, button_idx,
              butt_txt,
              (set_button) ? "" : itoa(button_idx, number, 10));
            button_idx++;
            width = 85;
          }

          Web.slider[4] = LightGetDimmer(2);
          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // White brightness - Black to White
            (100 == width) ? 2 : 1, width,
            PSTR("f"),       // f - Unique HTML id
            PSTR("#000"), PSTR("#fff"),    // Black to White
            5,               // sl5 - Unique range HTML id - Used for slider updates
            Settings->flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100% (SetOption77 - Do not power off if slider moved to far left)
            Web.slider[4],
            'w', 0);         // w0 - Value id is related to lc("w0", value) and WebGetArg("w0", tmp, sizeof(tmp));
          WSContentSend_P(PSTR("</tr>"));
        }
      } else {  // Settings->flag3.pwm_multi_channels - SetOption68 1 - Enable multi-channels PWM instead of Color PWM
        stemp[0] = 'e'; stemp[1] = '0'; stemp[2] = '\0';  // e0
        for (uint32_t i = 0; i < light_devices; i++) {
          bool set_button = ((button_idx <= MAX_BUTTON_TEXT) && strlen(GetWebButton(button_idx -1)));
          char first[2];
          snprintf_P(first, sizeof(first), PSTR("%s"), PSTR(D_BUTTON_TOGGLE));
          char butt_txt[4];
          snprintf_P(butt_txt, sizeof(butt_txt), PSTR("%s"),
            (set_button) ? HtmlEscape(GetWebButton(button_idx -1)).c_str() : first);
          char number[8];
          WSContentSend_P(PSTR("<tr>"));
          WSContentSend_P(HTTP_DEVICE_CONTROL, 15, button_idx, button_idx,
            butt_txt,
            (set_button) ? "" : itoa(button_idx, number, 10));
          button_idx++;

          stemp[1]++;        // e1 to e5 - Make unique ids
          Web.slider[i] = changeUIntScale(Settings->light_color[i], 0, 255, 0, 100);

          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Channel brightness - Black to White
            1, 85,
            stemp,           // e1 to e5 - Unique HTML id
            PSTR("#000"), PSTR("#fff"),  // Black to White
            i+1,             // sl1 to sl5 - Unique range HTML id - Used for slider updates
            1, 100,          // Range 1 to 100%
            Web.slider[i],
            'e', i+1);       // e1 to e5 - Value id

          WSContentSend_P(PSTR("</tr>"));
        }
      }  // Settings->flag3.pwm_multi_channels
      WSContentSend_P(PSTR("</table>"));
    }
#endif // USE_LIGHT

  }

  // Init buttons 
  uint32_t max_devices = TasmotaGlobal.devices_present;

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { 
    max_devices = MaxFanspeed() +1;  // 4 -> 5
  }
#endif  // USE_SONOFF_IFAN

  bool use_script = false;
  for (uint32_t idx = 1; idx <= max_devices; idx++) {
    bool not_active = !bitRead(TasmotaGlobal.power, idx -1);

#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan() && (idx > 1)) { 
      not_active = true;
    }
#endif  // USE_SONOFF_IFAN

    if (not_active) {
      if (!use_script) {
        use_script = true;
        WSContentSend_P(PSTR("<script>"));
      }
      WSContentSend_P(PSTR("eb('o%d').style.background='var(--c_btnoff)';"), idx);
    }
  }
  if (use_script) {
    WSContentSend_P(PSTR("</script>"));
  }

  XdrvXsnsCall(FUNC_WEB_ADD_MAIN_BUTTON);
#endif  // Not FIRMWARE_MINIMAL

  if (HTTP_ADMIN == Web.state) {
#ifdef FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_FIRMWARE_UPGRADE);
    WSContentButton(BUTTON_CONSOLE);
#else   // Not FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_CONFIGURATION);
    WSContentButton(BUTTON_INFORMATION);
    WSContentButton(BUTTON_FIRMWARE_UPGRADE);
    if (!WebUseManagementSubmenu()) {
      WSContentButton(BUTTON_CONSOLE);
    } else {
      WSContentButton(BUTTON_MANAGEMENT);
    }
#endif  // Not FIRMWARE_MINIMAL
    WSContentButton(BUTTON_RESTART);
  }
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*\
 * HandleRootStatusRefresh
\*-------------------------------------------------------------------------------------------*/

bool WebUpdateSliderTime(void) {
  uint32_t slider_update_time = millis();
  if (0 == Web.slider_update_time) {
    Web.slider_update_time = slider_update_time + Settings->web_refresh;  // Allow other users to sync screen
  }
  else if (slider_update_time > Web.slider_update_time) {
    Web.slider_update_time = 1;  // Allow multiple updates
    return true;
  }
  return false;
}

/*-------------------------------------------------------------------------------------------*/

bool HandleRootStatusRefresh(void) {
  if (!WebAuthenticate()) {
    Webserver->requestAuthentication();
    return true;
  }

  if (!Webserver->hasArg("m")) {     // Status refresh requested
    return false;
  }

#ifndef FIRMWARE_MINIMAL

  #ifdef USE_SCRIPT_WEB_DISPLAY
    Script_Check_HTML_Setvars();
  #endif

  char tmp[8];                       // WebGetArg numbers only
  char svalue[32];                   // Command and number parameter
  char webindex[5];                  // WebGetArg name

  WebGetArg(PSTR("o"), tmp, sizeof(tmp));  // 1 - 32 Device number for button Toggle or Fanspeed
  if (strlen(tmp)) {
    ShowWebSource(SRC_WEBGUI);
    uint32_t device = atoi(tmp);
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) {
      if (device < 2) {
        ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), device -2);
        ExecuteCommand(svalue, SRC_WEBGUI);
      }
    } else {
#endif  // USE_SONOFF_IFAN
#ifdef USE_SHUTTER
      int32_t ShutterWebButton;
      if (ShutterWebButton = IsShutterWebButton(device)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("ShutterPosition%d %s"), abs(ShutterWebButton), (ShutterWebButton>0) ? PSTR(D_CMND_SHUTTER_STOPOPEN) : PSTR(D_CMND_SHUTTER_STOPCLOSE));
        ExecuteWebCommand(svalue);
      } else {
#endif  // USE_SHUTTER
        ExecuteCommandPower(device, POWER_TOGGLE, SRC_IGNORE);
#ifdef USE_SHUTTER
      }
#endif  // USE_SHUTTER
#ifdef USE_SONOFF_IFAN
    }
#endif  // USE_SONOFF_IFAN
  }
#ifdef USE_LIGHT
  WebGetArg(PSTR("d0"), tmp, sizeof(tmp));  // 0 - 100 Dimmer value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_DIMMER " %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  WebGetArg(PSTR("w0"), tmp, sizeof(tmp));  // 0 - 100 White value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_WHITE " %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  uint32_t light_device = LightDevice();    // Channel number offset
  uint32_t light_devices = LightDevices();  // Number of channels
  for (uint32_t j = 0; j < light_devices; j++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("e%d"), j +1);
    WebGetArg(webindex, tmp, sizeof(tmp));  // 0 - 100 percent
    if (strlen(tmp)) {
      snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d %s"), j +light_device, tmp);
      ExecuteWebCommand(svalue);
    }
  }
  WebGetArg(PSTR("t0"), tmp, sizeof(tmp));  // 153 - 500 Color temperature
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_COLORTEMPERATURE " %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  WebGetArg(PSTR("h0"), tmp, sizeof(tmp));  // 0 - 359 Hue value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_HSBCOLOR  "1 %s"), tmp);
    ExecuteWebCommand(svalue);
  }
  WebGetArg(PSTR("n0"), tmp, sizeof(tmp));  // 0 - 99 Saturation value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_HSBCOLOR  "2 %s"), tmp);
    ExecuteWebCommand(svalue);
  }
#endif  // USE_LIGHT
#ifdef USE_SHUTTER
  for (uint32_t j = 1; j <= TasmotaGlobal.shutters_present; j++) {
    uint8_t percent;
    snprintf_P(webindex, sizeof(webindex), PSTR("u%d"), j);
    WebGetArg(webindex, tmp, sizeof(tmp));  // 0 - 100 percent
    percent = atoi(tmp);
    if (strlen(tmp)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("ShutterPosition%d %d"), j, (ShutterGetOptions(j-1) & 1) ? 100 - percent : percent);
      ExecuteWebCommand(svalue);
    }
  }
#endif  // USE_SHUTTER
#ifdef USE_ZIGBEE
  WebGetArg(PSTR("zbj"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("ZbPermitJoin"));
    ExecuteWebCommand(svalue);
  }
  WebGetArg(PSTR("zbr"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("ZbMap"));
    ExecuteWebCommand(svalue);
  }
#endif // USE_ZIGBEE

  XsnsXdrvCall(FUNC_WEB_GET_ARG);

#ifdef USE_WEB_SSE
  WSContentBegin(200, CT_STREAM);
  WSContentSend_P(PSTR("data: "));
#else
  WSContentBegin(200, CT_HTML);
#endif  // USE_WEB_SSE

  bool msg_exec_javascript = false;
  if (TasmotaGlobal.devices_present) {
    // Update changed web buttons
    uint32_t max_devices = TasmotaGlobal.devices_present;

#ifdef USE_SONOFF_IFAN
    uint32_t fanspeed;
    if (IsModuleIfan()) {
      // Single power relay and four virtual buttons
      max_devices = MaxFanspeed() +1;  // 4 -> 5
      fanspeed = GetFanspeed() +2;     // 0..3 -> 2..5
    }
#endif  // USE_SONOFF_IFAN

    WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);  // "<img style='display:none;' src onerror=\""
    msg_exec_javascript = true;
    for (uint32_t idx = 1; idx <= max_devices; idx++) {
      bool active = bitRead(TasmotaGlobal.power, idx -1);

#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan() && (idx > 1)) {
        active = (fanspeed == idx);
      }
#endif  // USE_SONOFF_IFAN

      WSContentSend_P(PSTR("eb('o%d').style.background='var(--c_btn%s)';"),
        idx, (active) ? PSTR("") : PSTR("off"));
    }
  }

#ifdef USE_SHUTTER
  for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    if (Web.shutter_slider[i] != -1) {
      uint32_t shutter_real_to_percent_position = ShutterRealToPercentPosition(-9999, i);
      uint32_t current_value = (ShutterGetOptions(i) & 1) ? (100 - shutter_real_to_percent_position) : shutter_real_to_percent_position;
      if (current_value != Web.shutter_slider[i]) {
        if (WebUpdateSliderTime()) {
          Web.shutter_slider[i] = current_value;
        }
        if (!msg_exec_javascript) {
          WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);  // "<img style='display:none;' src onerror=\""
          msg_exec_javascript = true;
        }
        WSContentSend_P(PSTR("eb('s27%d').value='%d';"), i +1, current_value);
      }
    }
  }
#endif  // USE_SHUTTER

#ifdef USE_LIGHT
  uint16_t hue;
  uint8_t sat;
  int current_value = -1;
  for (uint32_t i = 0; i < LST_MAX; i++) {
    if (Web.slider[i] != -1) {
      if (!Settings->flag3.pwm_multi_channels) {  // SetOption68 0 - Enable multi-channels PWM instead of Color PWM
        if (0 == i) {
          current_value = LightGetColorTemp();
        }
        else if (1 == i) {
          LightGetHSB(&hue, &sat, nullptr);
          current_value = hue;
        }
        else if (2 == i) {
          current_value = changeUIntScale(sat, 0, 255, 0, 100);
        }
        else if (3 == i) {
          current_value = Settings->light_dimmer;
        }
        else if (4 == i) {
          current_value = LightGetDimmer(2);
        }
      } else {
        current_value = changeUIntScale(Settings->light_color[i], 0, 255, 0, 100);
      }
      if (current_value != Web.slider[i]) {
        if (WebUpdateSliderTime()) {
          Web.slider[i] = current_value;
        }
        if (!msg_exec_javascript) {
          WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);  // "<img style='display:none;' src onerror=\""
          msg_exec_javascript = true;
        }
        WSContentSend_P(PSTR("eb('sl%d').value='%d';"), i +1, current_value);
      }
    }
  }
#endif  // USE_LIGHT

  if (msg_exec_javascript) {
    WSContentSend_P(PSTR("\">"));
  }

#ifdef USE_WEB_STATUS_LINE
  // create a first DIV for the upper left status bar, positioned left-justified
  // we use the same string literal for both lines to reduce Flash
  WSContentSend_P(PSTR("<div style='font-size:9px;font-weight:bold;text-align:%s;position:absolute;top:0;%s:0;display:inline-flex;'>"), PSTR("left"), PSTR("left"));
#ifdef USE_WEB_STATUS_LINE_WIFI
  if (Settings->flag4.network_wifi) {
    int32_t rssi = WiFi.RSSI();
    WSContentSend_P(PSTR("<div class='wifi' title='%s: " D_RSSI " %d%% (%d dBm)'><div class='arc a3%s'></div><div class='arc a2%s'></div><div class='arc a1%s'></div><div class='arc a0'></div></div>"),
                          SettingsTextEscaped(SET_STASSID1 + Settings->sta_active).c_str(),                      
                          WifiGetRssiAsQuality(rssi), rssi,
                          rssi < -55 ? " o30" : "",
                          rssi < -70 ? " o30" : "",
                          rssi < -85 ? " o30" : "");
  }
#endif // USE_WEB_STATUS_LINE_WIFI
#ifdef USE_WEB_STATUS_LINE_HEAP
  WSContentSend_P("<span>&nbsp;%ik</span>", ESP_getFreeHeap() / 1024);
#endif // USE_WEB_STATUS_LINE_HEAP
  // display here anything that goes on the left side
  XsnsXdrvCall(FUNC_WEB_STATUS_LEFT);
  WSContentSend_P(PSTR("</div>"));

  // create a second DIV for the upper right status bar, positioned right-justified
  WSContentSend_P(PSTR("<div style='font-size:9px;font-weight:bold;text-align:%s;position:absolute;top:0;%s:0;display:inline-flex;'>"), PSTR("right"), PSTR("right"));
  XsnsXdrvCall(FUNC_WEB_STATUS_RIGHT);
  WSContentSend_P(PSTR("</div>"));
#endif // USE_WEB_STATUS_LINE

  WSContentSend_P(PSTR("{t}"));        // <table style='width:100%'>
  WSContentSeparator(3);               // Reset seperator to ignore previous outputs 
  if (Settings->web_time_end) {
    WSContentSend_P(PSTR("{s}" D_TIMER_TIME "{m}%s{e}"), GetDateAndTime(DT_LOCAL).substring(Settings->web_time_start, Settings->web_time_end).c_str());
    WSContentSeparator(0);             // Print separator
  }
  XsnsXdrvCall(FUNC_WEB_SENSOR);
  WSContentSend_P(PSTR("</table>"));

  if (!Settings->flag6.gui_no_state_text) {          // SetOption161 - (GUI) Disable display of state text (1)
    if (!Web.buttons_non_light_non_shutter) {        // Might still be zero on restart so chk if we have at least one 
      WebGetDeviceCounts();
    }
    if ((Web.buttons_non_light_non_shutter > 0) &&
       ( Web.buttons_non_light_non_shutter <= 8)) {  // We need at least one non light AND non shutter button
      WSContentSend_P(PSTR("{t}<tr>"));
#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan()) {
        WSContentSend_P(HTTP_DEVICE_STATE, 36, (bitRead(TasmotaGlobal.power, 0)) ? PSTR("bold") : PSTR("normal"), 54, GetStateText(bitRead(TasmotaGlobal.power, 0)));
        uint32_t fanspeed = GetFanspeed();
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fanspeed);
        WSContentSend_P(HTTP_DEVICE_STATE, 64, (fanspeed) ? PSTR("bold") : PSTR("normal"), 54, (fanspeed) ? svalue : GetStateText(0));
      } else {
#endif  // USE_SONOFF_IFAN
        uint32_t cols = Web.buttons_non_light_non_shutter;
        uint32_t fontsize = (cols < 5) ? 70 - (cols * 8) : 32;
        uint32_t button_ptr = 0;
        for (uint32_t button_idx = 1; button_idx <= TasmotaGlobal.devices_present; button_idx++) {
          if (bitRead(Web.light_shutter_button_mask, button_idx -1)) { continue; }  // Skip non-sequential shutter button
          bool power_state = bitRead(TasmotaGlobal.power, button_idx -1);
          snprintf_P(svalue, sizeof(svalue), PSTR("%d"), power_state);
          WSContentSend_P(HTTP_DEVICE_STATE, 100 / cols, (power_state) ? PSTR("bold") : PSTR("normal"), fontsize, (cols < 5) ? GetStateText(power_state) : svalue);
          button_ptr++;
          if (button_ptr >= Web.buttons_non_light_non_shutter) { break; }
        }
#ifdef USE_SONOFF_IFAN
      }
#endif  // USE_SONOFF_IFAN
      WSContentSend_P(PSTR("</tr></table>"));
    }
  }

  if (1 == Web.slider_update_time) {
    Web.slider_update_time = 0;
  }

  WSContentSend_P(PSTR("\n\n"));  // Prep for SSE
  WSContentEnd();

#endif  // not FIRMWARE_MINIMAL

  return true;
}

#ifndef FIRMWARE_MINIMAL

#ifdef USE_ALPINEJS
/*********************************************************************************************\
 * Serve AlpineJS 2.8.2 in gzip format
 * Content-Encoding: gzip
 * Content-Type: text/javascript
\*********************************************************************************************/

void HandleAlpinejsRequest(void) {
  Webserver->client().flush();
  WSHeaderSend();
  Webserver->sendHeader(F("Content-Encoding"), F("gzip"));
  Webserver->sendHeader(F("Vary"), F("Accept-Encoding"));
  Webserver->setContentLength(sizeof(alpine_min_js_gz));
  Webserver->send(200, PSTR("text/javascript"), "");
  Webserver->sendContent_P(alpine_min_js_gz, sizeof(alpine_min_js_gz));
  Webserver->client().stop();
}
#endif  // USE_ALPINEJS

/*********************************************************************************************\
 * HandleConfiguration
\*********************************************************************************************/

void HandleConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURATION));

  WSContentStart_P(PSTR(D_CONFIGURATION));
  WSContentSendStyle();
  WSContentSend_P(HTTP_MENU_HEAD, D_CONFIGURATION);

  WSContentButton(BUTTON_MODULE);
  WSContentButton(BUTTON_WIFI);

  XdrvXsnsCall(FUNC_WEB_ADD_BUTTON);

  WSContentButton(BUTTON_LOGGING);
  WSContentButton(BUTTON_OTHER);
  WSContentButton(BUTTON_TEMPLATE);

  WSContentSpaceButton(BUTTON_RESET_CONFIGURATION);
  WSContentButton(BUTTON_BACKUP);
  WSContentButton(BUTTON_RESTORE);

  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

/*********************************************************************************************\
 * HandleTemplateConfiguration
\*********************************************************************************************/

void WSContentSendNiceLists(uint32_t option) {
  char stemp[30];                                             // Template number and Sensor name
  for (uint32_t i = 0; i < nitems(kGpioNiceList); i++) {  // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...
    if (option && (1 == i)) {
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), PSTR(D_SENSOR_USER));  // }2'255'>User}3
    }
    uint32_t ridx = pgm_read_word(&kGpioNiceList[i]) & 0xFFE0;
    uint32_t midx = BGPIO(ridx);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
  }
  WSContentSend_P(PSTR("\";"));

  WSContentSend_P(PSTR("hs=["));
  uint32_t midx;
  bool first_done = false;
  for (uint32_t i = 0; i < nitems(kGpioNiceList); i++) {  // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
    midx = pgm_read_word(&kGpioNiceList[i]);
    if (midx & 0x001F) {
      if (first_done) { WSContentSend_P(PSTR(",")); }
      WSContentSend_P(PSTR("%d"), midx);
      first_done = true;
    }
  }
#ifdef ESP8266
#ifdef USE_ADC
  for (uint32_t i = 0; i < nitems(kAdcNiceList); i++) {   // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
    midx = pgm_read_word(kAdcNiceList + i);
    if (midx & 0x001F) {
      if (first_done) { WSContentSend_P(PSTR(",")); }
      WSContentSend_P(PSTR("%d"), midx);
      first_done = true;
    }
  }
#endif  // USE_ADC
#endif  // ESP8266
  WSContentSend_P(PSTR("];"));
}

/*-------------------------------------------------------------------------------------------*/

#ifdef ESP8266
#ifdef USE_ADC
void WSContentSendAdcNiceList(uint32_t option) {
  char stemp[30];                                             // Template number and Sensor name
  WSContentSend_P(PSTR("os=\""));
  for (uint32_t i = 0; i < nitems(kAdcNiceList); i++) {   // GPIO: }2'0'>None}3}2'17'>Analog}3...
    if (option && (1 == i)) {
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), PSTR(D_SENSOR_USER));  // }2'15'>User}3
    }
    uint32_t ridx = pgm_read_word(kAdcNiceList + i) & 0xFFE0;
    uint32_t midx = BGPIO(ridx);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
  }
}
#endif  // USE_ADC
#endif  // ESP8266

/*-------------------------------------------------------------------------------------------*/

void HandleTemplateConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg(F("save"))) {
    TemplateSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[30];                                           // Template number and Sensor name

  WebGetArg(PSTR("t"), stemp, sizeof(stemp));                     // 0 - 69 Template number
  if (strlen(stemp)) {
    uint32_t module = atoi(stemp);
    uint32_t module_save = Settings->module;
    Settings->module = module;
    myio template_gp;
    TemplateGpios(&template_gp);
    gpio_flag flag = ModuleFlag();
    Settings->module = module_save;

    WSContentBegin(200, CT_PLAIN);
    WSContentSend_P(PSTR("%s}1"), AnyModuleName(module).c_str());  // NAME: Generic
    for (uint32_t i = 0; i < nitems(template_gp.io); i++) {        // 17,148,29,149,7,255,255,255,138,255,139,255,255
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32C6
      // ESP32C2/C3/C6 we always send all GPIOs, Flash are just hidden
      WSContentSend_P(PSTR("%s%d"), (i>0)?",":"", template_gp.io[i]);
#else
      if (!FlashPin(i)) {
        WSContentSend_P(PSTR("%s%d"), (i>0)?",":"", template_gp.io[i]);
      }
#endif  // CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6
    }
    WSContentSend_P(PSTR("}1%d}1%d"), flag, Settings->user_template_base);  // FLAG: 1  BASE: 17
    WSContentEnd();
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_TEMPLATE));

  WSContentStart_P(PSTR(D_CONFIGURE_TEMPLATE));
  WSContentSend_P(HTTP_SCRIPT_MODULE_TEMPLATE);

  WSContentSend_P(HTTP_SCRIPT_TEMPLATE);

  WSContentSendNiceLists(1);

  WSContentSend_P(HTTP_SCRIPT_TEMPLATE2);

#ifdef ESP8266
#ifdef USE_ADC
  WSContentSendAdcNiceList(1);
  WSContentSend_P(HTTP_SCRIPT_TEMPLATE3);
#endif  // USE_ADC
#endif  // ESP8266

  WSContentSend_P(HTTP_SCRIPT_TEMPLATE4);
  for (uint32_t i = 0; i < sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
    uint32_t midx = pgm_read_byte(kModuleNiceList + i);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_INDEX, midx, AnyModuleName(midx).c_str(), midx +1);
  }
  WSContentSend_P(HTTP_SCRIPT_TEMPLATE5);

  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_TEMPLATE);
  WSContentSend_P(HTTP_TABLE100);  // "<table style='width:100%%'>"
  WSContentSend_P(PSTR("<tr><td><b>" D_TEMPLATE_NAME "</b></td><td style='width:200px'><input id='s1' placeholder='" D_TEMPLATE_NAME "'></td></tr>"
                       "<tr><td><b>" D_BASE_TYPE "</b></td><td><select id='g99' onchange='st(this.value)'></select></td></tr>"
                       "</table>"
                       "<hr>"));
  WSContentSend_P(HTTP_TABLE100);  // "<table style='width:100%%'>"
  for (uint32_t i = 0; i < MAX_GPIO_PIN; i++) {
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6
    // ESP32C2/C3/C6 all gpios are in the template, flash are hidden
    bool hidden = FlashPin(i);
    WSContentSend_P(PSTR("<tr%s><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
      hidden ? PSTR(" hidden") : "",
      RedPin(i) ? WebColor(COL_TEXT_WARNING) : WebColor(COL_TEXT), i, (0==i) ? PSTR(" style='width:146px'") : "", i, i);
    WSContentSend_P(PSTR("<td style='width:54px'><select id='h%d'></select></td></tr>"), i);
#else // also works for ESP32S2
    if (!FlashPin(i)) {
      WSContentSend_P(PSTR("<tr><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
        RedPin(i) ? WebColor(COL_TEXT_WARNING) : WebColor(COL_TEXT), i, (0==i) ? PSTR(" style='width:146px'") : "", i, i);
      WSContentSend_P(PSTR("<td style='width:54px'><select id='h%d'></select></td></tr>"), i);
    }
#endif  // CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6
  }
  WSContentSend_P(PSTR("</table>"));

  gpio_flag flag = ModuleFlag();
  if (flag.data) {
    WSContentSend_P(HTTP_FORM_TEMPLATE_FLAG);
  }

  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

uint16_t WebGetGpioArg(uint32_t i) {
  char webindex[5];                                         // WebGetArg name
  snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), i);
  char tmp[8];                                              // WebGetArg numbers only
  WebGetArg(webindex, tmp, sizeof(tmp));                    // GPIO
  uint32_t gpio = (!strlen(tmp)) ? 0 : atoi(tmp);
  char webindex2[5];                                        // WebGetArg name
  snprintf_P(webindex2, sizeof(webindex2), PSTR("h%d"), i);
  char tmp2[8];                                             // WebGetArg numbers only
  WebGetArg(webindex2, tmp2, sizeof(tmp2));
  uint32_t value2 = (!strlen(tmp2)) ? 0 : atoi(tmp2) -1;
  gpio += value2;
  return gpio;
}

/*-------------------------------------------------------------------------------------------*/

void TemplateSaveSettings(void) {
  char tmp[TOPSZ];                                      // WebGetArg NAME and GPIO/BASE/FLAG byte value
#ifdef ESP8266
  char command[300];                                    // Template command string
#else
  char command[500];                                    // Template command string supporting P4 (55 GPIOs)
#endif

  WebGetArg(PSTR("s1"), tmp, sizeof(tmp));              // NAME
  snprintf_P(command, sizeof(command), PSTR(D_CMND_TEMPLATE " {\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), tmp);

  uint32_t j = 0;
  for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
/*    
#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32C3
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(i));
#elif defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
    if (22 == i) { j = 33; }    // skip 22-32
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(j));
    j++;
#elif defined(CONFIG_IDF_TARGET_ESP32)
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(Esp32TemplateToPhy[i]));
#else  // ESP8266
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(j));
    j++;
#endif
*/
#ifdef ESP8266
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(j));
    j++;
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32P4
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(i));
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    if (22 == i) { j = 33; }    // skip 22-32
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(j));
    j++;
#else  // ESP32
    snprintf_P(command, sizeof(command), PSTR("%s%s%d"), command, (i>0)?",":"", WebGetGpioArg(Esp32TemplateToPhy[i]));
#endif  // ESP32C2/C3/C6 and S2/S3
#endif  // ESP32
  }

  uint32_t flag = 0;
  char webindex[5];                                     // WebGetArg name
  for (uint32_t i = 0; i < GPIO_FLAG_USED; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
    uint32_t state = Webserver->hasArg(webindex) << i;  // FLAG
    flag += state;
  }
  WebGetArg(PSTR("g99"), tmp, sizeof(tmp));             // BASE
  uint32_t base = atoi(tmp) +1;

  snprintf_P(command, sizeof(command), PSTR("%s],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), command, flag, base);
  ExecuteWebCommand(command);
}

/*********************************************************************************************\
 * HandleModuleConfiguration
\*********************************************************************************************/

void HandleModuleConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg(F("save"))) {
    ModuleSaveSettings();
    WebRestart(1);
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_MODULE));

  char stemp[30];  // Sensor name
  uint32_t midx;
  myio template_gp;
  TemplateGpios(&template_gp);

  WSContentStart_P(PSTR(D_CONFIGURE_MODULE));
  WSContentSend_P(HTTP_SCRIPT_MODULE_TEMPLATE);

  WSContentSend_P(PSTR("function sl(){os=\""));
  uint32_t vidx = 0;
  for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
    if (0 == i) {
      midx = USER_MODULE;
      vidx = 0;
    } else {
      midx = pgm_read_byte(kModuleNiceList + i -1);
      vidx = midx +1;
    }
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_INDEX, midx, AnyModuleName(midx).c_str(), vidx);
  }
  WSContentSend_P(PSTR("\";sk(%d,99);os=\""), Settings->module);

  WSContentSendNiceLists(0);

  for (uint32_t i = 0; i < nitems(template_gp.io); i++) {
    if (ValidGPIO(i, template_gp.io[i])) {
      WSContentSend_P(PSTR("sk(%d,%d);"), TasmotaGlobal.my_module.io[i], i);  // g0 - g17
    }
  }

#ifdef ESP8266
#ifdef USE_ADC
  WSContentSendAdcNiceList(0);
  WSContentSend_P(PSTR("\";sk(%d," STR(ADC0_PIN) ");"), Settings->my_gp.io[(sizeof(myio) / 2) -1]);
#endif  // USE_ADC
#endif  // ESP8266

  WSContentSend_P(PSTR("}wl(sl);"));

  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_MODULE, AnyModuleName(MODULE).c_str());
  for (uint32_t i = 0; i < nitems(template_gp.io); i++) {
    if (ValidGPIO(i, template_gp.io[i])) {
      snprintf_P(stemp, 3, PINS_WEMOS +i*2);
      WSContentSend_P(PSTR("<tr><td style='width:116px'>%s <b>" D_GPIO "%d</b></td><td style='width:146px'><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
        (WEMOS==TasmotaGlobal.module_type)?stemp:"", i, i, i);
      WSContentSend_P(PSTR("<td style='width:54px'><select id='h%d'></select></td></tr>"), i);
    }
  }
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void ModuleSaveSettings(void) {
  char tmp[8];         // WebGetArg numbers only
  WebGetArg(PSTR("g99"), tmp, sizeof(tmp));  // Module
  uint32_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
  Settings->last_module = Settings->module;
  Settings->module = new_module;
  SetModuleType();
  myio template_gp;
  TemplateGpios(&template_gp);
  for (uint32_t i = 0; i < nitems(template_gp.io); i++) {
    if (Settings->last_module != new_module) {
      Settings->my_gp.io[i] = GPIO_NONE;
    } else {
      if (ValidGPIO(i, template_gp.io[i])) {
        Settings->my_gp.io[i] = WebGetGpioArg(i);  // Gpio
      }
    }
  }
  char command[32];
  snprintf_P(command, sizeof(command), PSTR(D_CMND_BACKLOG "0 " D_CMND_MODULE ";" D_CMND_GPIO));
  ExecuteWebCommand(command);
}

/*********************************************************************************************\
 * HandleWifiConfiguration
\*********************************************************************************************/

void HandleWifiConfiguration(void) {
  char tmp[TOPSZ];  // Max length is currently 150

  if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_WIFI));

  if (Webserver->hasArg(F("save")) && HTTP_MANAGER_RESET_ONLY != Web.state) {
    if ( WifiIsInManagerMode() ) {
      // Test WIFI Connection to Router
      // As Tasmota is in this case in AP mode, a STA connection can be established too at the same time

      if (WIFI_NOT_TESTING == Wifi.wifiTest) {
        if (MAX_WIFI_OPTION == Wifi.old_wificonfig) { Wifi.old_wificonfig = Settings->sta_config; }
        TasmotaGlobal.wifi_state_flag = Settings->sta_config = WIFI_MANAGER;
        Wifi.save_data_counter = TasmotaGlobal.save_data_counter;
      }

      Wifi.wifi_test_counter = 9;   // seconds to test user's proposed AP
      Wifi.wifiTest = WIFI_TESTING;
      TasmotaGlobal.save_data_counter = 0;               // Stop auto saving data - Updating Settings
      Settings->save_data = 0;
      TasmotaGlobal.sleep = 0;                           // Disable sleep
      TasmotaGlobal.restart_flag = 0;                    // No restart
      TasmotaGlobal.ota_state_flag = 0;                  // No OTA
//      TasmotaGlobal.blinks = 0;                          // Disable blinks initiated by WifiManager

      WebGetArg(PSTR("s1"), tmp, sizeof(tmp));   // SSID1
      SettingsUpdateText(SET_STASSID1, tmp);
      WebGetArg(PSTR("p1"), tmp, sizeof(tmp));   // PASSWORD1
      SettingsUpdateText(SET_STAPWD1, tmp);

      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP " %s " D_AS " %s ..."),
        SettingsText(SET_STASSID1), TasmotaGlobal.hostname);

      WiFiHelper::begin(SettingsText(SET_STASSID1), SettingsText(SET_STAPWD1));

      WebRestart(2);
    } else {
      // STATION MODE or MIXED
      // Save the config and restart
      WifiSaveSettings();
      WebRestart(1);
    }
    return;
  }

  if ( WIFI_TEST_FINISHED == Wifi.wifiTest ) {
    Wifi.wifiTest = WIFI_NOT_TESTING;
    if (Wifi.wifi_test_AP_TIMEOUT) {
      WebRestart(1); // Save credentials and Force Restart in STA only mode (11n-only routers)
    } else {
#if (RESTART_AFTER_INITIAL_WIFI_CONFIG)
      WebRestart(3);
#else
      HandleRoot();
#endif  // RESTART_AFTER_INITIAL_WIFI_CONFIG
    }
  }

  WSContentStart_P(PSTR(D_CONFIGURE_WIFI), !WifiIsInManagerMode());
  WSContentSend_P(HTTP_SCRIPT_WIFI);
  if (WifiIsInManagerMode()) { WSContentSend_P(HTTP_SCRIPT_HIDE); }
  if (WIFI_TESTING == Wifi.wifiTest) { WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_RESTART_RECONNECT_TIME); }
#ifdef USE_ENHANCED_GUI_WIFI_SCAN
  WSContentSendStyle_P("%s", HTTP_HEAD_STYLE_SSI);
#else
  WSContentSendStyle();
#endif  // USE_ENHANCED_GUI_WIFI_SCAN

  bool limitScannedNetworks = true;
  if (HTTP_MANAGER_RESET_ONLY != Web.state) {
    if (WIFI_TESTING == Wifi.wifiTest) {
      limitScannedNetworks = false;
    } else {
      if (Webserver->hasArg(F("scan"))) { limitScannedNetworks = false; }

      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Scanning..."));
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
      int n = WiFi.scanNetworks(true);
      while(n<0){
        delay(50); // some magic number - maybe non optimal
        n = WiFi.scanComplete();
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

      if (0 == n) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_NO_NETWORKS_FOUND));
        WSContentSend_P(PSTR(D_NO_NETWORKS_FOUND));
        limitScannedNetworks = false; // in order to show D_SCAN_FOR_WIFI_NETWORKS
      } else {
        //sort networks
        int indices[n];
        for (uint32_t i = 0; i < n; i++) {
          indices[i] = i;
        }

        // RSSI SORT
        for (uint32_t i = 0; i < n; i++) {
          for (uint32_t j = i + 1; j < n; j++) {
            if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
              std::swap(indices[i], indices[j]);
            }
          }
        }

        uint32_t networksToShow = n;
        if ((limitScannedNetworks) && (networksToShow > MAX_WIFI_NETWORKS_TO_SHOW)) { networksToShow = MAX_WIFI_NETWORKS_TO_SHOW; }

        if (WifiIsInManagerMode()) {
          WSContentTextCenterStart(WebColor(COL_TEXT));
          WSContentSend_P(PSTR(D_SELECT_YOUR_WIFI_NETWORK "</div><br>"));
        }

#ifdef USE_ENHANCED_GUI_WIFI_SCAN
        //display networks in page
        bool skipduplicated;
        int ssid_showed = 0;
        for (uint32_t i = 0; i < networksToShow; i++) {
          if (indices[i] < n) {
            int32_t rssi = WiFi.RSSI(indices[i]);
            String ssid = WiFi.SSID(indices[i]);
            DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"),
              ssid.c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), rssi);

            String ssid_copy = ssid;
            if (!ssid_copy.length()) { ssid_copy = F("no_name"); }
            // Print SSID
            if (!limitScannedNetworks) {
              WSContentSend_P(PSTR("<div><a href='#p' onclick='c(this)'>%s</a><br>"), HtmlEscape(ssid_copy).c_str());
            }
            skipduplicated = false;
            String nextSSID = "";
#ifdef USE_HIGHLIGHT_CONNECTED_AP
            bool HighlightAP;
#endif
            // Handle all APs with the same SSID
            for (uint32_t j = 0; j < n; j++) {
              if ((indices[j] < n) && ((nextSSID = WiFi.SSID(indices[j])) == ssid)) {
                if (!skipduplicated) {
                  // Update RSSI / quality
                  rssi = WiFi.RSSI(indices[j]);
                  uint8_t rssi_as_quality = WifiGetRssiAsQuality(rssi);
                  uint8_t num_bars = changeUIntScale(rssi_as_quality, 0, 100, 0, 4);

                  WSContentSend_P(PSTR("<div title='%d%% (%d dBm)'>"), rssi_as_quality, rssi);
                  if (limitScannedNetworks) {
                    // Print SSID and item
                    WSContentSend_P(PSTR("<a href='#p' onclick='c(this)'>%s</a><span class='q'><div class='si'>"), HtmlEscape(ssid_copy).c_str());
                    ssid_showed++;
                    skipduplicated = true; // For the simplified page, just show 1 SSID if there are many Networks with the same
#ifdef USE_HIGHLIGHT_CONNECTED_AP
                    HighlightAP = ssid_copy == WiFi.SSID();
#endif
                  } else {
                    // Print item
                    WSContentSend_P(PSTR("%s<span class='q'>(%d) <div class='si'>"), WiFi.BSSIDstr(indices[j]).c_str(), WiFi.channel(indices[j]));
#ifdef USE_HIGHLIGHT_CONNECTED_AP
                    HighlightAP = WiFi.BSSIDstr(indices[j]) == WiFi.BSSIDstr();
#endif
                  }
                  // Print signal strength indicator
                  for (uint8_t k = 0; k < 4; k++) {
#ifdef USE_HIGHLIGHT_CONNECTED_AP
                    WSContentSend_P(PSTR("<i class='b%d%s'%s></i>"), k, (k >= num_bars) ? PSTR(" o30") : PSTR(""), HighlightAP ? PSTR(" style='background-color:var(--c_btn);'") : PSTR(""));
#else
                    WSContentSend_P(PSTR("<i class='b%d%s'></i>"), k, (k >= num_bars) ? PSTR(" o30") : PSTR(""));
#endif
                  }
                  WSContentSend_P(PSTR("</div></span></div>"));
                } else {
                  if (ssid_showed <= networksToShow) { networksToShow++; }
                }
                indices[j] = n;
              }
              delay(0);
            }
            if (!limitScannedNetworks) {
              WSContentSend_P(PSTR("</div>"));
            }
          }
        }
#else  // No USE_ENHANCED_GUI_WIFI_SCAN
        // remove duplicates ( must be RSSI sorted )
        for (uint32_t i = 0; i < n; i++) {
          if (-1 == indices[i]) { continue; }
          String cssid = WiFi.SSID(indices[i]);
          uint32_t cschn = WiFi.channel(indices[i]);
          for (uint32_t j = i + 1; j < n; j++) {
            if ((cssid == WiFi.SSID(indices[j])) && (cschn == WiFi.channel(indices[j]))) {
              DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              indices[j] = -1;  // set dup aps to index -1
            }
          }
        }

        //display networks in page
        for (uint32_t i = 0; i < networksToShow; i++) {
          if (-1 == indices[i]) { continue; }  // skip dups
          int32_t rssi = WiFi.RSSI(indices[i]);
          DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"),
            WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), rssi);
          int quality = WifiGetRssiAsQuality(rssi);
          String ssid_copy = WiFi.SSID(indices[i]);
          if (!ssid_copy.length()) { ssid_copy = F("no_name"); }
          WSContentSend_P(PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%d%% (%d dBm)</span></div>"),
            HtmlEscape(ssid_copy).c_str(),
            WiFi.channel(indices[i]),
            quality, rssi
          );

          delay(0);
        }
#endif  // USE_ENHANCED_GUI_WIFI_SCAN

        WSContentSend_P(PSTR("<br>"));
      }
    }

    WSContentSend_P(PSTR("<div><a href='/wi?scan='>%s</a></div><br>"), (limitScannedNetworks) ? PSTR(D_SHOW_MORE_WIFI_NETWORKS) : PSTR(D_SCAN_FOR_WIFI_NETWORKS));
    WSContentSend_P(HTTP_FORM_WIFI_PART1, (WifiIsInManagerMode()) ? "" : PSTR(" (" STA_SSID1 ")"), SettingsTextEscaped(SET_STASSID1).c_str());
    if (WifiIsInManagerMode()) {
      // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
      WSContentSend_P(PSTR("></p>"));
    } else {
#ifdef USE_CORS
      WSContentSend_P(HTTP_FORM_WIFI_PART2, SettingsTextEscaped(SET_STASSID2).c_str(), WIFI_HOSTNAME, WIFI_HOSTNAME, SettingsTextEscaped(SET_HOSTNAME).c_str(), SettingsTextEscaped(SET_CORS).c_str());
#else
      WSContentSend_P(HTTP_FORM_WIFI_PART2, SettingsTextEscaped(SET_STASSID2).c_str(), WIFI_HOSTNAME, WIFI_HOSTNAME, SettingsTextEscaped(SET_HOSTNAME).c_str());
#endif  // USE_CORS
    }

    WSContentSend_P(HTTP_FORM_END);
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    WSContentTextCenterStart(WebColor(COL_TEXT_WARNING));
    WSContentSend_P(PSTR("<h3>"));

    if (WIFI_TESTING == Wifi.wifiTest) {
      WSContentSend_P(PSTR(D_TRYING_TO_CONNECT "<br>%s</h3></div>"), SettingsTextEscaped(SET_STASSID1).c_str());
    } else if (WIFI_TEST_FINISHED_BAD == Wifi.wifiTest) {
      WSContentSend_P(PSTR(D_CONNECT_FAILED_TO " %s<br>" D_CHECK_CREDENTIALS "</h3></div>"), SettingsTextEscaped(SET_STASSID1).c_str());
    }
    // More Options Button
    WSContentSend_P(PSTR("<div id=butmod style=\"display:%s;\"></div><p></p><form id=butmo style=\"display:%s;\"><button type='button' onclick='hidBtns()'>" D_SHOW_MORE_OPTIONS "</button></form>"),
      (WIFI_TEST_FINISHED_BAD == Wifi.wifiTest) ? "none" : Web.initial_config ? "block" : "none", Web.initial_config ? "block" : "none"
    );
    WSContentSpaceButton(BUTTON_RESTORE, !Web.initial_config);
    WSContentButton(BUTTON_RESET_CONFIGURATION, !Web.initial_config);
#endif  // FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_RESTART, !Web.initial_config);
  } else {
    WSContentSpaceButton(BUTTON_CONFIGURATION);
  }
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void WifiSaveSettings(void) {
  String cmnd = F(D_CMND_BACKLOG "0 ");
  cmnd += AddWebCommand(PSTR(D_CMND_HOSTNAME), PSTR("h"), PSTR("1"));
#ifdef USE_CORS
  cmnd += AddWebCommand(PSTR(D_CMND_CORS), PSTR("c"), PSTR("1"));
#endif  // USE_CORS
  cmnd += AddWebCommand(PSTR(D_CMND_SSID "1"), PSTR("s1"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_SSID "2"), PSTR("s2"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_PASSWORD "3"), PSTR("p1"), PSTR("\""));
  cmnd += AddWebCommand(PSTR(D_CMND_PASSWORD "4"), PSTR("p2"), PSTR("\""));
  ExecuteWebCommand((char*)cmnd.c_str());
}

/*********************************************************************************************\
 * HandleLoggingConfiguration
\*********************************************************************************************/

void HandleLoggingConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

  if (Webserver->hasArg("save")) {
    LoggingSaveSettings();
    HandleConfiguration();
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_LOGGING));
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_LOG1);
  char stemp1[45];
  char stemp2[32];
  uint8_t dlevel[4] = { LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_NONE, LOG_LEVEL_NONE };
  for (uint32_t idx = 0; idx < 4; idx++) {
    if ((2==idx) && !Settings->flag.mqtt_enabled) { continue; }  // SetOption3 - Enable MQTT
    uint32_t llevel = (0==idx)?Settings->seriallog_level:(1==idx)?Settings->weblog_level:(2==idx)?Settings->mqttlog_level:Settings->syslog_level;
    WSContentSend_P(PSTR("<p><b>%s</b> (%s)<br><select id='l%d'>"),
      GetTextIndexed(stemp1, sizeof(stemp1), idx, kLoggingOptions),
      GetTextIndexed(stemp2, sizeof(stemp2), dlevel[idx], kLoggingLevels),
      idx);
    for (uint32_t i = LOG_LEVEL_NONE; i <= LOG_LEVEL_DEBUG_MORE; i++) {
      WSContentSend_P(PSTR("<option%s value='%d'>%d %s</option>"),
        (i == llevel) ? PSTR(" selected") : "", i, i,
        GetTextIndexed(stemp1, sizeof(stemp1), i, kLoggingLevels));
    }
    WSContentSend_P(PSTR("</select></p>"));
  }
  WSContentSend_P(HTTP_FORM_LOG2, SettingsTextEscaped(SET_SYSLOG_HOST).c_str(), Settings->syslog_port, Settings->tele_period);
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void LoggingSaveSettings(void) {
  String cmnd = F(D_CMND_BACKLOG "0 ");
  cmnd += AddWebCommand(PSTR(D_CMND_SERIALLOG), PSTR("l0"), STR(SERIAL_LOG_LEVEL));
  cmnd += AddWebCommand(PSTR(D_CMND_WEBLOG), PSTR("l1"), STR(WEB_LOG_LEVEL));
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTLOG), PSTR("l2"), STR(MQTT_LOG_LEVEL));
  cmnd += AddWebCommand(PSTR(D_CMND_SYSLOG), PSTR("l3"), STR(SYS_LOG_LEVEL));
  cmnd += AddWebCommand(PSTR(D_CMND_LOGHOST), PSTR("lh"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_LOGPORT), PSTR("lp"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_TELEPERIOD), PSTR("lt"), PSTR("1"));
  ExecuteWebCommand((char*)cmnd.c_str());
}

/*********************************************************************************************\
 * HandleLoggingConfiguration
\*********************************************************************************************/

void HandleOtherConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_OTHER));

  if (Webserver->hasArg(F("save"))) {
    OtherSaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_OTHER));
  WSContentSendStyle();

  TemplateJson();
  WSContentSend_P(HTTP_FORM_OTHER, HtmlEscape(ResponseData()).c_str(), (USER_MODULE == Settings->module) ? PSTR(" checked disabled") : "",
    (Settings->flag5.disable_referer_chk) ? PSTR(" checked") : "",   // SetOption128 - Enable HTTP API
    (Settings->flag.mqtt_enabled) ? PSTR(" checked") : "",   // SetOption3 - Enable MQTT
    SettingsTextEscaped(SET_FRIENDLYNAME1).c_str(), SettingsTextEscaped(SET_DEVICENAME).c_str());

  char stemp[32];
  uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!TasmotaGlobal.devices_present) ? 1 : TasmotaGlobal.devices_present;
#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
  for (uint32_t i = 0; i < maxfn; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i +1);
    WSContentSend_P(PSTR("<b>" D_FRIENDLY_NAME " %d</b> (" FRIENDLY_NAME "%s)<br><input id='a%d' placeholder=\"" FRIENDLY_NAME "%s\" value=\"%s\"><p></p>"),
      i +1,
      (i) ? stemp : "",
      i,
      (i) ? stemp : "",
      SettingsTextEscaped(SET_FRIENDLYNAME1 + i).c_str());
  }

#ifdef USE_EMULATION
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
  WSContentSend_P(PSTR("<p></p><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend><p>"));  // Keep close to Friendlynames so do not use <br>
  for (uint32_t i = 0; i < EMUL_MAX; i++) {
#ifndef USE_EMULATION_WEMO
    if (i == EMUL_WEMO) { i++; }
#endif
#ifndef USE_EMULATION_HUE
    if (i == EMUL_HUE) { i++; }
#endif
    if (i < EMUL_MAX) {
      WSContentSend_P(PSTR("<label><input id='r%d' name='b2' type='radio' value='%d'%s><b>%s</b> %s</label><br>"),  // Different id only used for labels
        i, i,
        (i == Settings->flag2.emulation) ? PSTR(" checked") : "",
        GetTextIndexed(stemp, sizeof(stemp), i, kEmulationOptions),
        (i == EMUL_NONE) ? "" : (i == EMUL_WEMO) ? PSTR(D_SINGLE_DEVICE) : PSTR(D_MULTI_DEVICE));
    }
  }
  WSContentSend_P(PSTR("</p></fieldset>"));
#endif  // USE_EMULATION_WEMO || USE_EMULATION_HUE
#endif  // USE_EMULATION

  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void OtherSaveSettings(void) {
  String cmnd = F(D_CMND_BACKLOG "0 ");
  cmnd += AddWebCommand(PSTR(D_CMND_WEBPASSWORD "2"), PSTR("wp"), PSTR("\""));
  cmnd += F(";" D_CMND_SO "3 ");
  cmnd += Webserver->hasArg(F("b1"));
  cmnd += F(";" D_CMND_SO "128 ");
  cmnd += Webserver->hasArg(F("b3"));
  cmnd += AddWebCommand(PSTR(D_CMND_DEVICENAME), PSTR("dn"), PSTR("\""));
  char webindex[5];
  char cmnd2[24];                             // ";Module 0;Template "
  for (uint32_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("a%d"), i);
    snprintf_P(cmnd2, sizeof(cmnd2), PSTR(D_CMND_FN "%d"), i +1);
    cmnd += AddWebCommand(cmnd2, webindex, PSTR("\""));
  }

#ifdef USE_EMULATION
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
  cmnd += AddWebCommand(PSTR(D_CMND_EMULATION), PSTR("b2"), PSTR("0"));
#endif  // USE_EMULATION_WEMO || USE_EMULATION_HUE
#endif  // USE_EMULATION

  String tmpl = Webserver->arg(F("t1"));    // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255,"CMND":"SO123 1;SO99 0"}
  if (tmpl.length() && (tmpl.length() < MQTT_MAX_PACKET_SIZE)) {
    snprintf_P(cmnd2, sizeof(cmnd2), PSTR(";%s" D_CMND_TEMPLATE " "), (Webserver->hasArg(F("t2"))) ? PSTR(D_CMND_MODULE " 0;") : "");
    cmnd += cmnd2 + tmpl;
  }
  ExecuteWebCommand((char*)cmnd.c_str());
}

/*********************************************************************************************\
 * HandleBackupConfiguration
\*********************************************************************************************/

void HandleBackupConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  uint32_t config_len = SettingsConfigBackup();
  if (!config_len) { return; }    // Unable to allocate buffer

  WiFiClient myClient = Webserver->client();
  Webserver->setContentLength(config_len);

  char attachment[TOPSZ];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), SettingsConfigFilename().c_str());
  Webserver->sendHeader(F("Content-Disposition"), attachment);

  WSSend(200, CT_APP_STREAM, "");
  myClient.write((const char*)settings_buffer, config_len);

  SettingsBufferFree();
}

/*********************************************************************************************\
 * HandleResetConfiguration
\*********************************************************************************************/

void HandleResetConfiguration(void) {
  if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESET_CONFIGURATION));

  WSContentStart_P(PSTR(D_RESET_CONFIGURATION), !WifiIsInManagerMode());
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>"));
  WSContentSend_P(HTTP_MSG_RSTRT);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  char command[CMDSZ];
  snprintf_P(command, sizeof(command), PSTR(D_CMND_RESET " 1"));
  ExecuteWebCommand(command);
}

/*********************************************************************************************\
 * HandleRestoreConfiguration
\*********************************************************************************************/

void HandleRestoreConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESTORE_CONFIGURATION));

  WSContentStart_P(PSTR(D_RESTORE_CONFIGURATION));
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_RST);
  WSContentSend_P(HTTP_FORM_RST_UPG, PSTR(D_START_RESTORE));
  if (WifiIsInManagerMode()) {
    WSContentSpaceButton(BUTTON_MAIN);
  } else {
    WSContentSpaceButton(BUTTON_CONFIGURATION);
  }
  WSContentStop();

  Web.upload_file_type = UPL_SETTINGS;
}

/*********************************************************************************************\
 * HandleInformation
\*********************************************************************************************/

void WSContentSeparatorI(uint32_t size) {
  WSContentSend_P(PSTR("</td></tr><tr><td colspan=2><hr style='font-size:2px'%s>"), (1 == size)?" size=1":"");
//  WSContentSend_P(PSTR("</td></tr><tr><td colspan=2><hr style='font-size:%dpx'/>"), size);
//  WSContentSend_P(PSTR("</td></tr><tr><td colspan=2><hr style='border_top:%dpx solid'/>"), size);
//  WSContentSend_P(PSTR("</td></tr><tr><td colspan=2 style='border-bottom:%dpx solid #ccc;'>"), size);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSeparatorIFat(void) {
//  WSContentSend_P(PSTR("}1}2&nbsp;"));      // Empty line = </td></tr><tr><th></th><td>&nbsp;
  WSContentSeparatorI(2);
}

/*-------------------------------------------------------------------------------------------*/

void WSContentSeparatorIThin(void) {
  WSContentSeparatorI(1);
}

/*-------------------------------------------------------------------------------------------*/

void HandleInformation(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  float freemem = ((float)ESP_getFreeHeap()) / 1024;
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_INFORMATION));

  char stopic[TOPSZ];

  WSContentStart_P(PSTR(D_INFORMATION));
  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  WSContentSend_P(HTTP_SCRIPT_INFO_BEGIN);
  WSContentSend_P(HTTP_MENU_HEAD, D_INFORMATION);
  WSContentSend_P(PSTR("<table style='width:100%%'><tr><th>"));
  WSContentSend_P(PSTR(D_PROGRAM_VERSION "}2%s %s %s"), TasmotaGlobal.version, TasmotaGlobal.image_name, GetCodeCores().c_str());
  WSContentSend_P(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), GetBuildDateAndTime().c_str());
  WSContentSend_P(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_CORE_RELEASE "/%s"), ESP.getSdkVersion());
  WSContentSend_P(PSTR("}1" D_UPTIME "}2%s"), GetUptime().c_str());
#ifdef ESP8266
  WSContentSend_P(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d " D_AT " 0x%X"), Settings->save_flag, GetSettingsAddress());
#endif  // ESP8266
#ifdef ESP32
  WSContentSend_P(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d"), Settings->save_flag);
#endif  // ESP32
  WSContentSend_P(PSTR("}1" D_BOOT_COUNT "}2%d"), Settings->bootcount);
  WSContentSend_P(PSTR("}1" D_RESTART_REASON "}2%s"), GetResetReason().c_str());
  uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : TasmotaGlobal.devices_present;
#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
  for (uint32_t i = 0; i < maxfn; i++) {
    WSContentSend_P(PSTR("}1" D_FRIENDLY_NAME " %d}2%s"), i +1, SettingsTextEscaped(SET_FRIENDLYNAME1 +i).c_str());
  }
  WSContentSeparatorIFat();
#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED
  WSContentSend_P(PSTR("}1" D_HOSTED_MCU "}2%s (%s)"), GetHostedMCU().c_str(), GetHostedMCUFwVersion().c_str());
  WSContentSeparatorIFat();
#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED
  bool show_hr = false;
  if ((WiFi.getMode() >= WIFI_AP) && (static_cast<uint32_t>(WiFi.softAPIP()) != 0)) {
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (AP)}2%_I"), (uint32_t)WiFi.softAPIP());
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%_I"), (uint32_t)WiFi.softAPIP());
    WSContentSeparatorIThin();
  }
  if (Settings->flag4.network_wifi) {
    int32_t rssi = WiFi.RSSI();
    WSContentSend_P(PSTR("}1" D_AP "%d " D_INFORMATION "}2" D_SSID " %s<br>" D_RSSI " %d%% (%d dBm)<br>" D_MODE " %s<br>" D_CHANNEL " %d<br>" D_BSSID " %s"), 
      Settings->sta_active +1,
      SettingsTextEscaped(SET_STASSID1 + Settings->sta_active).c_str(),
      WifiGetRssiAsQuality(rssi), rssi,
      WifiGetPhyMode().c_str(),
      WiFi.channel(),
      WiFi.BSSIDstr().c_str());
    WSContentSeparatorIFat();
    WSContentSend_P(PSTR("}1" D_HOSTNAME "}2%s%s"), TasmotaGlobal.hostname, (Mdns.begun) ? PSTR(".local") : "");
#ifdef USE_IPV6
    String ipv6_addr = WifiGetIPv6Str();
    if (ipv6_addr != "") {
      WSContentSend_P(PSTR("}1 IPv6 Global (WiFi)}2%s"), ipv6_addr.c_str());
    }
    ipv6_addr = WifiGetIPv6LinkLocalStr();
    if (ipv6_addr != "") {
      WSContentSend_P(PSTR("}1 IPv6 Local (WiFi)}2%s"), ipv6_addr.c_str());
    }
#endif  // USE_IPV6
    if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
      WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFiHelper::macAddress().c_str());
      WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (WiFi)}2%_I"), (uint32_t)WiFi.localIP());
    }
    show_hr = true;
  }
  if (!TasmotaGlobal.global_state.wifi_down) {
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%_I"), Settings->ipv4_address[1]);
    WSContentSend_P(PSTR("}1" D_SUBNET_MASK "}2%_I"), Settings->ipv4_address[2]);
#ifdef USE_IPV6
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "1}2%s"), DNSGetIPStr(0).c_str());
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "2}2%s"), DNSGetIPStr(1).c_str());
#else // USE_IPV6
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "1}2%_I"), Settings->ipv4_address[3]);
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "2}2%_I"), Settings->ipv4_address[4]);
#endif // USE_IPV6
  }
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  if (EthernetHasIP()) {
    if (show_hr) {
      WSContentSeparatorIThin();
    }
    WSContentSend_P(PSTR("}1" D_HOSTNAME "}2%s%s"), EthernetHostname(), (Mdns.begun) ? PSTR(".local") : "");
#ifdef USE_IPV6
    String ipv6_eth_addr = EthernetGetIPv6Str();
    if (ipv6_eth_addr != "") {
      WSContentSend_P(PSTR("}1 IPv6 Global (eth)}2%s"), ipv6_eth_addr.c_str());
    }
    ipv6_eth_addr = EthernetGetIPv6LinkLocalStr();
    if (ipv6_eth_addr != "") {
      WSContentSend_P(PSTR("}1 IPv6 Local (eth)}2%s"), ipv6_eth_addr.c_str());
    }
#endif  // USE_IPV6
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), EthernetMacAddress().c_str());
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (eth)}2%_I"), (uint32_t)EthernetLocalIP());
  }
  if (!TasmotaGlobal.global_state.eth_down) {
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%_I"), Settings->eth_ipv4_address[1]);
    WSContentSend_P(PSTR("}1" D_SUBNET_MASK "}2%_I"), Settings->eth_ipv4_address[2]);
#ifdef USE_IPV6
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "1}2%s"), DNSGetIPStr(0).c_str());
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "2}2%s"), DNSGetIPStr(1).c_str());
#else // USE_IPV6
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "1}2%_I"), Settings->eth_ipv4_address[3]);
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "2}2%_I"), Settings->eth_ipv4_address[4]);
#endif // USE_IPV6
  }
#endif  // USE_ETHERNET
  WSContentSeparatorIFat();
  WSContentSend_P(PSTR("}1" D_HTTP_API "}2%s"), Settings->flag5.disable_referer_chk ? PSTR(D_ENABLED) : PSTR(D_DISABLED)); // SetOption 128
  WSContentSeparatorIFat();
  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    WSContentSend_P(PSTR("}1" D_MQTT_HOST "}2%s"), SettingsTextEscaped(SET_MQTT_HOST).c_str());
    WSContentSend_P(PSTR("}1" D_MQTT_PORT "}2%d"), Settings->mqtt_port);
#ifdef USE_MQTT_TLS
    WSContentSend_P(PSTR("}1" D_MQTT_TLS_ENABLE "}2%s"), Settings->flag4.mqtt_tls ? PSTR(D_ENABLED) : PSTR(D_DISABLED));
#endif  // USE_MQTT_TLS
    WSContentSend_P(PSTR("}1" D_MQTT_USER "}2%s"), SettingsTextEscaped(SET_MQTT_USER).c_str());
    WSContentSend_P(PSTR("}1" D_MQTT_CLIENT "}2%s"), TasmotaGlobal.mqtt_client);
    WSContentSend_P(PSTR("}1" D_MQTT_TOPIC "}2%s"), SettingsTextEscaped(SET_MQTT_TOPIC).c_str());
    uint32_t real_index = SET_MQTT_GRP_TOPIC;
    for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
      if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
      if (strlen(SettingsText(real_index +i))) {
        WSContentSend_P(PSTR("}1" D_MQTT_GROUP_TOPIC " %d}2%s"), 1 +i, GetGroupTopic_P(stopic, "", real_index +i));
      }
    }
    WSContentSend_P(PSTR("}1" D_MQTT_FULL_TOPIC "}2%s"), GetTopic_P(stopic, CMND, TasmotaGlobal.mqtt_topic, ""));
    WSContentSend_P(PSTR("}1" D_MQTT " " D_FALLBACK_TOPIC "}2%s"), GetFallbackTopic_P(stopic, ""));
    WSContentSend_P(PSTR("}1" D_MQTT_NO_RETAIN "}2%s"), Settings->flag4.mqtt_no_retain ? PSTR(D_ENABLED) : PSTR(D_DISABLED));
  } else {
    WSContentSend_P(PSTR("}1" D_MQTT "}2" D_DISABLED));
  }

#if defined(USE_EMULATION) || defined(USE_DISCOVERY)
  WSContentSeparatorIFat();
#endif  // USE_EMULATION or USE_DISCOVERY
#ifdef USE_EMULATION
  WSContentSend_P(PSTR("}1" D_EMULATION "}2%s"), GetTextIndexed(stopic, sizeof(stopic), Settings->flag2.emulation, kEmulationOptions));
#endif  // USE_EMULATION

#ifdef USE_DISCOVERY
  WSContentSend_P(PSTR("}1" D_MDNS_DISCOVERY "}2%s"), (Settings->flag3.mdns_enabled) ? D_ENABLED : D_DISABLED);  // SetOption55 - Control mDNS service
  if (Settings->flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
#ifdef WEBSERVER_ADVERTISE
    WSContentSend_P(PSTR("}1" D_MDNS_ADVERTISE "}2" D_WEB_SERVER));
#else
    WSContentSend_P(PSTR("}1" D_MDNS_ADVERTISE "}2" D_DISABLED));
#endif  // WEBSERVER_ADVERTISE
  }
#endif  // USE_DISCOVERY

  WSContentSeparatorIFat();
  WSContentSend_P(PSTR("}1" D_ESP_CHIP_ID "}2%d (%s)"), ESP_getChipId(), GetDeviceHardwareRevision().c_str());
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X (" D_TASMOTA_FLASHMODE ")"), ESP_getFlashChipId());
#ifdef ESP32
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_SIZE "}2%d KB"), ESP.getFlashChipSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%d KB"), ESP_getFlashChipMagicSize() / 1024);
#endif // ESP32
#ifdef ESP8266
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_SIZE "}2%d KB"), ESP.getFlashChipRealSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%d KB"), ESP_getFlashChipSize() / 1024);
#endif // ESP8266
  WSContentSend_P(PSTR("}1" D_PROGRAM_SIZE "}2%d KB"), ESP_getSketchSize() / 1024);
  WSContentSend_P(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%d KB"), ESP_getFreeSketchSpace() / 1024);
#ifdef ESP32
#ifdef USE_GT911
  WSContentSend_PD(PSTR("}1" D_FREE_MEMORY "}2%1_f KB"), &freemem);
#else
  WSContentSend_PD(PSTR("}1" D_FREE_MEMORY "}2%1_f KB (" D_FRAGMENTATION " %d%%)"), &freemem, ESP_getHeapFragmentation());
#endif // USE_GT911
  if (UsePSRAM()) {
    WSContentSend_P(PSTR("}1" D_PSR_MAX_MEMORY "}2%d KB"), ESP.getPsramSize() / 1024);
    WSContentSend_P(PSTR("}1" D_PSR_FREE_MEMORY "}2%d KB"), ESP.getFreePsram() / 1024);
  }
  WSContentSeparatorIFat();
  uint32_t cur_part = ESP_PARTITION_SUBTYPE_APP_FACTORY;   // 0
  const esp_partition_t *running_ota = esp_ota_get_running_partition();
  if (running_ota) { cur_part = running_ota->subtype; }    // 16 - 32
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  for (; it != NULL; it = esp_partition_next(it)) {
    const esp_partition_t *part = esp_partition_get(it);

//    AddLog(LOG_LEVEL_DEBUG, PSTR("PRT: Type %d, Subtype %d, Name %s, Size %d"), part->type, part->subtype, part->label, part->size);

    uint32_t part_size = part->size / 1024;
    if (ESP_PARTITION_TYPE_APP == part->type) {
      uint32_t prog_size = 0;                              // No active ota partition
      if (part->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY) {
        prog_size = EspProgramSize(part->label);           // safeboot partition (slow response)
      }
      else if ((part->subtype >= ESP_PARTITION_SUBTYPE_APP_OTA_MIN) && (part->subtype <= ESP_PARTITION_SUBTYPE_APP_OTA_MAX)) {
        if (cur_part == part->subtype) {
          prog_size = ESP_getSketchSize();                 // Active running ota partition (fast response)
        }
        else if (cur_part == ESP_PARTITION_SUBTYPE_APP_FACTORY) {
          prog_size = EspProgramSize(part->label);         // One app partition when safeboot partitions (slow response)
        }
      }
      char running[2] = { 0 };
      if (part->subtype == cur_part) { running[0] = '*'; }
      WSContentSend_PD(PSTR("}1" D_PARTITION " %s%s}2%d KB"), part->label, running, part_size);
      if (prog_size) {
        uint32_t part_used = ((prog_size / 1024) * 100) / part_size;
        WSContentSend_PD(PSTR(" (" D_USED " %d%%)"), part_used);
      }
    }
    if ((ESP_PARTITION_TYPE_DATA == part->type) && (ESP_PARTITION_SUBTYPE_DATA_SPIFFS == part->subtype)) {
      WSContentSend_PD(PSTR("}1" D_PARTITION " fs}2%d KB"), part_size);
    }
  }
  esp_partition_iterator_release(it);
#else   // not ESP32
  WSContentSend_PD(PSTR("}1" D_FREE_MEMORY "}2%1_f KB"), &freemem);
#ifdef USE_UFILESYS
  WSContentSeparatorIFat();
  WSContentSend_P(PSTR("}1" D_FILE_SYSTEM_SIZE "}2%d KB"), UfsSize());
#endif  // USE_UFILESYS
#endif  // ESP32
  WSContentSend_P(PSTR("</td></tr></table>"));

  WSContentSend_P(HTTP_SCRIPT_INFO_END);
  WSContentSendStyle();
  // WSContentSend_P(PSTR("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>"));
  WSContentSend_P(PSTR("<style>th{padding-right:5px;}</style>"
                       "<div id='i' name='i'></div>"));
  //   WSContentSend_P(PSTR("</fieldset>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

#endif  // Not FIRMWARE_MINIMAL

/*********************************************************************************************\
 * HandleUpgradeFirmware
\*********************************************************************************************/

#if defined(USE_ZIGBEE_EZSP) || defined(USE_TASMOTA_CLIENT) || defined(SHELLY_FW_UPGRADE) || defined(USE_RF_FLASH) || defined(USE_CCLOADER)
#define USE_WEB_FW_UPGRADE
#endif

#ifdef USE_WEB_FW_UPGRADE

struct {
  size_t spi_hex_size;
  size_t spi_sector_counter;
  size_t spi_sector_cursor;
  bool active;
  bool ready;
} BUpload;

/*-------------------------------------------------------------------------------------------*/

void BUploadInit(uint32_t file_type) {
  Web.upload_file_type = file_type;
  BUpload.spi_hex_size = 0;
  BUpload.spi_sector_counter = FlashWriteStartSector();
  BUpload.spi_sector_cursor = 0;
  BUpload.active = true;
  BUpload.ready = false;
}

/*-------------------------------------------------------------------------------------------*/

uint32_t BUploadWriteBuffer(uint8_t *buf, size_t size) {
  if (0 == BUpload.spi_sector_cursor) { // Starting a new sector write so we need to erase it first
    if (!ESP.flashEraseSector(BUpload.spi_sector_counter)) {
      return 7;  // Upload aborted - flash failed
    }
  }
  BUpload.spi_sector_cursor++;
  if (!ESP.flashWrite((BUpload.spi_sector_counter * SPI_FLASH_SEC_SIZE) + ((BUpload.spi_sector_cursor -1) * HTTP_UPLOAD_BUFLEN), (uint32_t*)buf, size)) {
    return 7;  // Upload aborted - flash failed
  }
  BUpload.spi_hex_size += size;
  if (2 == BUpload.spi_sector_cursor) {  // The web upload sends 2048 bytes at a time so keep track of the cursor position to reset it for the next flash sector erase
    BUpload.spi_sector_cursor = 0;
    BUpload.spi_sector_counter++;
    if (BUpload.spi_sector_counter > FlashWriteMaxSector()) {
      return 9;  // File too large - Not enough free space
    }
  }
  return 0;
}

#endif  // USE_WEB_FW_UPGRADE

/*-------------------------------------------------------------------------------------------*/

void HandleUpgradeFirmware(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_FIRMWARE_UPGRADE));

  WSContentStart_P(PSTR(D_FIRMWARE_UPGRADE));
  WSContentSendStyle();
  WSContentSend_P(HTTP_MENU_HEAD, D_FIRMWARE_UPGRADE);

  WSContentSend_P(HTTP_FORM_UPG, SettingsTextEscaped(SET_OTAURL).c_str());
#ifdef ESP32
  if (EspSingleOtaPartition() && !EspRunningFactoryPartition()) {
    WSContentSend_P(HTTP_FORM_RST_UPG_FCT, PSTR(D_START_UPGRADE));
  } else {
    WSContentSend_P(HTTP_FORM_RST_UPG, PSTR(D_START_UPGRADE));
  }
#else
  WSContentSend_P(HTTP_FORM_RST_UPG, PSTR(D_START_UPGRADE));
#endif  // ESP32
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  Web.upload_file_type = UPL_TASMOTA;
}

/*-------------------------------------------------------------------------------------------*/

void HandleUpgradeFirmwareStart(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  char command[TOPSZ + 10];  // OtaUrl

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  WifiConfigCounter();

  char otaurl[TOPSZ];
  WebGetArg(PSTR("o"), otaurl, sizeof(otaurl));
  if (strlen(otaurl)) {
    snprintf_P(command, sizeof(command), PSTR(D_CMND_OTAURL " %s"), otaurl);
    ExecuteWebCommand(command);
  }

  WSContentStart_P(PSTR(D_INFORMATION));
  WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_OTA_RESTART_RECONNECT_TIME);
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
  WSContentSend_P(HTTP_MSG_RSTRT);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  snprintf_P(command, sizeof(command), PSTR(D_CMND_UPGRADE " 1"));
  ExecuteWebCommand(command);
}

/*-------------------------------------------------------------------------------------------*/

void HandleUploadDone(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

#if defined(USE_ZIGBEE_EZSP)
  if ((UPL_EFR32 == Web.upload_file_type) && !Web.upload_error && BUpload.ready) {
    BUpload.ready = false;  //  Make sure not to follow thru again
    // GUI xmodem
    ZigbeeUploadStep1Done(FlashWriteStartSector(), BUpload.spi_hex_size);
    HandleZigbeeXfer();
    return;
  }
#endif  // USE_ZIGBEE_EZSP

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  WifiConfigCounter();
  UploadServices(1);

  WSContentStart_P(PSTR(D_INFORMATION));
  if (!Web.upload_error) {
    WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, (UPL_TASMOTA == Web.upload_file_type) ? HTTP_OTA_RESTART_RECONNECT_TIME : HTTP_RESTART_RECONNECT_TIME);  // Refesh main web ui after OTA upgrade
  }
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
  if (Web.upload_error) {
    WSContentSend_P(PSTR("%06x'>" D_FAILED "</font></b><br><br>"), WebColor(COL_TEXT_WARNING));
    char error[100];
    if (Web.upload_error < 10) {
      GetTextIndexed(error, sizeof(error), Web.upload_error -1, kUploadErrors);
    } else {
      snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), Web.upload_error);
    }
    WSContentSend_P(error);
    DEBUG_CORE_LOG(PSTR("UPL: %s"), error);
    TasmotaGlobal.stop_flash_rotate = Settings->flag.stop_flash_rotate;  // SetOption12 - Switch between dynamic or fixed slot flash save location
    Web.upload_error = 0;
  } else {
    WSContentSend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br>"), WebColor(COL_TEXT_SUCCESS));
    TasmotaGlobal.restart_flag = 2;  // Always restart to re-enable disabled features during update
    WSContentSend_P(HTTP_MSG_RSTRT);
    ShowWebSource(SRC_WEBGUI);
  }
  SettingsBufferFree();
  WSContentSend_P(PSTR("</div><br>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void UploadServices(uint32_t start_service) {
  if (Web.upload_services_stopped != start_service) { return; }
  Web.upload_services_stopped = !start_service;

  if (start_service) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("UPL: Services enabled"));

/*
    MqttRetryCounter(0);
*/
    AllowInterrupts(1);
  } else {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("UPL: Services disabled"));

    AllowInterrupts(0);
/*
    MqttRetryCounter(60);
    if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
      MqttDisconnect();
    }
*/
  }
}

/*-------------------------------------------------------------------------------------------*/

void HandleUploadLoop(void) {
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  static uint32_t upload_size;
  static bool upload_error_signalled;

  if (HTTP_USER == Web.state) { return; }

  if (Web.upload_error) {
    if (!upload_error_signalled) {
      if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
      UploadServices(1);

//      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Upload error %d"), Web.upload_error);

      upload_error_signalled = true;
    }
    return;
  }

  HTTPUpload& upload = Webserver->upload();

  // ***** Step1: Start upload file
  if (UPLOAD_FILE_START == upload.status) {
    Web.upload_error = 0;
    upload_error_signalled = false;
    char tmp[16];
    
    WebGetArg("fsz", tmp, sizeof(tmp));                    // filesize
    upload_size = (!strlen(tmp)) ? 0 : atoi(tmp);

    UploadServices(0);

    if (0 == upload.filename.c_str()[0]) {
      Web.upload_error = 1;  // No file selected
      return;
    }

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s (%d bytes)"), upload.filename.c_str(), upload_size);

#ifdef USE_UFILESYS
    if (UPL_UFSFILE == Web.upload_file_type) {
      const uint32_t freeBytes = (UfsFree() * 1024);
      if (upload_size > freeBytes) {
        Web.upload_error = 9;   // File too large
        return;
      }

      if (!UfsUploadFileOpen(upload.filename.c_str())) {
        Web.upload_error = 2;   // Not enough space
        return;
      }
    }
#endif  // USE_UFILESYS

    SettingsSave(1);  // Free flash for upload
  }

  // ***** Step2: Write upload file
  else if (UPLOAD_FILE_WRITE == upload.status) {
    if (0 == upload.totalSize) {  // First block received
      if (UPL_SETTINGS == Web.upload_file_type) {
        uint32_t set_size = sizeof(TSettings);
#ifdef USE_UFILESYS
        if (('s' == upload.buf[2]) && ('e' == upload.buf[3])) {  // /.settings
          set_size = upload.buf[14] + (upload.buf[15] << 8);
        }
#endif  // USE_UFILESYS
        if (!SettingsBufferAlloc(set_size)) {
          Web.upload_error = 2;  // Not enough space
          return;
        }
        Web.config_block_count = 0;
      }
#ifdef USE_WEB_FW_UPGRADE
#ifdef USE_RF_FLASH
      else if ((SONOFF_BRIDGE == TasmotaGlobal.module_type) && (':' == upload.buf[0])) {  // Check if this is a RF bridge FW file
        BUploadInit(UPL_EFM8BB1);
      }
#endif  // USE_RF_FLASH
#ifdef USE_TASMOTA_CLIENT
      else if (TasmotaClient_Available() && (':' == upload.buf[0])) {  // Check if this is a ARDUINO CLIENT hex file
        BUploadInit(UPL_TASMOTACLIENT);
      }
#endif  // USE_TASMOTA_CLIENT
#ifdef SHELLY_FW_UPGRADE
      else if (ShdPresent() && (0x00 == upload.buf[0]) && ((0x10 == upload.buf[1]) || (0x20 == upload.buf[1]))) {
        BUploadInit(UPL_SHD);
      }
#endif  // SHELLY_FW_UPGRADE
#ifdef USE_CCLOADER
      else if (CCLChipFound() && 0x02 == upload.buf[0]) { // the 0x02 is only an assumption!!
        BUploadInit(UPL_CCL);
      }
#endif  // USE_CCLOADER
#ifdef USE_ZIGBEE_EZSP
#ifdef ESP8266
      else if ((SONOFF_ZB_BRIDGE == TasmotaGlobal.module_type) && (0xEB == upload.buf[0])) {  // Check if this is a Zigbee bridge FW file
#endif  // ESP8266
#ifdef ESP32
      else if (PinUsed(GPIO_ZIGBEE_RX) && PinUsed(GPIO_ZIGBEE_TX) && (0xEB == upload.buf[0])) {  // Check if this is a Zigbee bridge FW file
#endif  // ESP32
        // Read complete file into ESP8266 flash
        // Current files are about 200k
        Web.upload_error = ZigbeeUploadStep1Init();  // 1
        if (Web.upload_error != 0) { return; }
        BUploadInit(UPL_EFR32);
      }
#endif  // USE_ZIGBEE_EZSP
#endif  // USE_WEB_FW_UPGRADE
      else if (UPL_TASMOTA == Web.upload_file_type) {
        if ((upload.buf[0] != 0xE9) && (upload.buf[0] != 0x1F)) {  // 0x1F is gzipped 0xE9
          Web.upload_error = 3;      // Invalid file signature - Magic byte is not 0xE9
          return;
        }
        if (0xE9 == upload.buf[0]) {
#ifdef ESP8266
          uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
          if (bin_flash_size > ESP.getFlashChipRealSize()) {
#else
          char tmp[16];
          WebGetArg("fsz", tmp, sizeof(tmp));                    // filesize
          uint32_t upload_size = (!strlen(tmp)) ? 0 : atoi(tmp);
          AddLog(LOG_LEVEL_DEBUG, D_LOG_UPLOAD "Freespace %i Filesize %i", ESP.getFreeSketchSpace(), upload_size);
          if (upload_size > ESP.getFreeSketchSpace()) {   // TODO revisit this test
#endif  // ESP8266
            Web.upload_error = 4;  // Program flash size is larger than real flash size
            return;
          }
  //            upload.buf[2] = 3;  // Force DOUT - ESP8285
        }
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) {         //start with max available size
          Web.upload_error = 2;  // Not enough space
          return;
        }
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "File type %d"), Web.upload_file_type);
    }  // First block received

    if (UPL_SETTINGS == Web.upload_file_type) {
      if (upload.currentSize > (settings_size - (Web.config_block_count * HTTP_UPLOAD_BUFLEN))) {
        Web.upload_error = 9;  // File too large
        return;
      }
      memcpy(settings_buffer + (Web.config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
      Web.config_block_count++;
    }
#ifdef USE_UFILESYS
    else if (!Web.upload_error && UPL_UFSFILE == Web.upload_file_type) {
      if (!UfsUploadFileWrite(upload.buf, upload.currentSize)) {
        Web.upload_error = 9;  // File too large
        return;
      }
    }
#endif  // USE_UFILESYS
#ifdef USE_WEB_FW_UPGRADE
    else if (BUpload.active) {
      // Write a block
//      AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Size %d, Data '%32_H'"), upload.currentSize, upload.buf);
      Web.upload_error = BUploadWriteBuffer(upload.buf, upload.currentSize);
      if (Web.upload_error != 0) { return; }
    }
#endif  // USE_WEB_FW_UPGRADE
    else if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
//      Web.upload_error = 5;  // Upload buffer miscompare
      Web.upload_error = 2;  // Not enough space
      return;
    }
    if (upload.totalSize && !(upload.totalSize % 102400)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Progress %d KB"), upload.totalSize / 1024);
    }
  }

  // ***** Step3: Finish upload file
  else if (UPLOAD_FILE_END == upload.status) {
    UploadServices(1);
    if (UPL_SETTINGS == Web.upload_file_type) {
      if (!SettingsConfigRestore()) {
        Web.upload_error = 8;  // File invalid
        return;
      }
    }
#ifdef USE_UFILESYS
    else if (!Web.upload_error && UPL_UFSFILE == Web.upload_file_type) {
      UfsUploadFileClose();
    }
#endif  // USE_UFILESYS
#ifdef USE_WEB_FW_UPGRADE
    else if (BUpload.active) {
      // Done writing the data to SPI flash
      BUpload.active = false;

      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Transfer %u bytes"), upload.totalSize);

      uint8_t* data = FlashDirectAccess();

//      uint32_t* values = (uint32_t*)(data);  // Only 4-byte access allowed
//      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Head 0x%08X"), values[0]);

      uint32_t error = 0;
#ifdef USE_RF_FLASH
      if (UPL_EFM8BB1 == Web.upload_file_type) {
        error = SnfBrUpdateFirmware(data, BUpload.spi_hex_size);
      }
#endif  // USE_RF_FLASH
#ifdef USE_TASMOTA_CLIENT
      if (UPL_TASMOTACLIENT == Web.upload_file_type) {
        error = TasmotaClient_Flash(data, BUpload.spi_hex_size);
      }
#endif  // USE_TASMOTA_CLIENT
#ifdef SHELLY_FW_UPGRADE
      if (UPL_SHD == Web.upload_file_type) {
        error = ShdFlash(data, BUpload.spi_hex_size);
      }
#endif  // SHELLY_FW_UPGRADE
#ifdef USE_CCLOADER
      if (UPL_CCL == Web.upload_file_type) {
        error = CLLFlashFirmware(data, BUpload.spi_hex_size);
      }
#endif  // SHELLY_FW_UPGRADE
#ifdef USE_ZIGBEE_EZSP
      if (UPL_EFR32 == Web.upload_file_type) {
        BUpload.ready = true;  // So we know on upload success page if it needs to flash hex or do a normal restart
      }
#endif  // USE_ZIGBEE_EZSP
      if (error != 0) {
//        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Transfer error %d"), error);
        Web.upload_error = error + (100 * (Web.upload_file_type -1));  // Add offset to discriminate transfer errors
        return;
      }
    }
#endif  // USE_WEB_FW_UPGRADE
    else if (!Update.end(true)) { // true to set the size to the current progress
      Web.upload_error = 6;  // Upload failed. Enable logging 3
      return;
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes"), upload.totalSize);
  }

  // ***** Step4: Abort upload file
  else {
    UploadServices(1);
    Web.upload_error = 7;  // Upload aborted
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
  }
  // do actually wait a little to allow ESP32 tasks to tick
  // fixes task timeout in ESP32Solo1 style unicore code.
  delay(10);
  OsWatchLoop();
//  Scheduler();          // Feed OsWatch timer to prevent restart on long uploads
}

/*********************************************************************************************\
 * HandlePreflightRequest
\*********************************************************************************************/

void HandlePreflightRequest(void) {
#ifdef USE_CORS
  HttpHeaderCors();
#endif
  Webserver->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  Webserver->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  WSSend(200, CT_HTML, "");
}

#ifdef ESP32
/*********************************************************************************************\
 * HandleSwitchBootPartition
\*********************************************************************************************/

// Switch boot partition
//
// Parameter `u4` is either `fct` or `ota` to switch to factory or ota
// If not in single-OTA mode
//
// The page can return the followinf (code 200)
// `false`: the current partition is not the target, but a restart to factory is triggered (polling required)
// `true`: the current partition is the one required
// `none`: there is no factory partition

// return a simple status page as text/plain code 200
static void WSReturnSimpleString(const char *msg) {
  if (nullptr == msg) { msg = ""; }
  Webserver->client().flush();
  WSHeaderSend();
  Webserver->send(200, "text/plain", msg);
}

/*-------------------------------------------------------------------------------------------*/

void HandleSwitchBootPartition(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  char tmp1[8];
  WebGetArg(PSTR("u4"), tmp1, sizeof(tmp1));

  bool switch_factory = false;      // trigger a restart to factory partition?
  bool switch_ota = false;          // switch back to OTA partition
  bool single_ota = EspSingleOtaPartition();
  bool api_mode = Webserver->hasArg("api");   // api-mode, returns `true`, `false` or `none`

  // switch to next OTA?
  if (strcmp("ota", tmp1) == 0) {
    switch_ota = true;
    if (single_ota && !EspRunningFactoryPartition()) {
      switch_ota = false;                     // if in single-OTA and already running OTA, nothing to do
    }
  }
  // switch to factory ?
  if (strcmp("fct", tmp1) == 0 && single_ota && !EspRunningFactoryPartition()) {
    switch_factory = true;
  }

  // apply the change in flash and return result
  if (switch_factory || switch_ota) {
    XsnsXdrvCall(FUNC_ABOUT_TO_RESTART);
    SettingsSaveAll();
    if (switch_factory) {
      EspPrepRestartToSafeBoot();
    } else {
      const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
      esp_ota_set_boot_partition(partition);
    }

    if (api_mode) {
      WSReturnSimpleString("false");
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESTART));
      EspRestart();
    } else {
      WebRestart(4);
    }
  } else {
    if (api_mode) {
      // return `none` or `true`
      WSReturnSimpleString(EspSingleOtaPartition() ? "true" : "none");
    } else {
      Webserver->sendHeader("Location", "/", true);
      Webserver->send(302, "text/plain", "");
    }
  }
  Web.upload_file_type = UPL_TASMOTA;
}
#endif // ESP32

/*********************************************************************************************\
 * HandleHttpCommand
\*********************************************************************************************/

void HandleHttpCommand(void) {
  if (!HttpCheckPriviledgedAccess(false)) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  if (!WebAuthenticate()) {
    // Prefer authorization via HTTP header (Basic auth), if it fails, use legacy method via GET parameters
    char tmp1[33];
    WebGetArg(PSTR("user"), tmp1, sizeof(tmp1));
    char tmp2[strlen(SettingsText(SET_WEBPWD)) + 2];  // Need space for an entered password longer than set password
    WebGetArg(PSTR("password"), tmp2, sizeof(tmp2));

    if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, SettingsText(SET_WEBPWD)))) {
      WSContentBegin(401, CT_APP_JSON);
      WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
      WSContentEnd();

      // https://github.com/arendst/Tasmota/discussions/15420
      ShowWebSource(SRC_WEBCOMMAND);
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Bad userid and/or password"));

      return;
    }
  }

  WSContentBegin(200, CT_APP_JSON);
  String svalue = Webserver->arg(F("cmnd"));
  if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
    uint32_t curridx = TasmotaGlobal.log_buffer_pointer;
    TasmotaGlobal.templog_level = LOG_LEVEL_INFO;
    ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCOMMAND);
    WSContentSend_P(PSTR("{"));
    bool cflg = false;
    uint32_t index = curridx;
    char* line;
    size_t len;
    while (GetLog(TasmotaGlobal.templog_level, &index, &line, &len)) {
      // [14:49:36.123 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
      char* JSON = (char*)memchr(line, '{', len);
      if (JSON) {  // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
        if (cflg) { WSContentSend_P(PSTR(",")); }
        uint32_t JSONlen = len - (JSON - line) -3;
        for( ++JSON ; JSONlen && JSON[JSONlen] != '}' ; JSONlen-- );
        WSContentSend(JSON, JSONlen);
        cflg = true;
      }
    }
    WSContentSend_P(PSTR("}"));
    TasmotaGlobal.templog_level = 0;
  } else {
    WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENTER_COMMAND " cmnd=\"}"));
  }
  WSContentEnd();
}

/*********************************************************************************************\
 * HandleManagement
\*********************************************************************************************/

void HandleManagement(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MANAGEMENT));

  WSContentStart_P(PSTR(D_MANAGEMENT));
  WSContentSendStyle();
  WSContentSend_P(HTTP_MENU_HEAD, D_MANAGEMENT);

  WSContentButton(BUTTON_CONSOLE);

  XdrvMailbox.index = 0;
  XdrvXsnsCall(FUNC_WEB_ADD_CONSOLE_BUTTON);
//  WSContentSend_P(PSTR("<div></div>"));     // 5px padding
  XdrvCall(FUNC_WEB_ADD_MANAGEMENT_BUTTON);

  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

/*********************************************************************************************\
 * HandleConsole
\*********************************************************************************************/

void HandleConsole(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg(F("c2"))) {      // Console refresh requested
    HandleConsoleRefresh();
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONSOLE));

  WSContentStart_P(PSTR(D_CONSOLE));
  WSContentSend_P(HTTP_SCRIPT_CONSOL, Settings->web_refresh);
#ifdef USE_CONSOLE_CSS_FLEX
  WSContentSendStyle_P(HTTP_CMND_STYLE);
#else
  WSContentSendStyle();
#endif  // USE_CONSOLE_CSS_FLEX
  WSContentSend_P(HTTP_FORM_CMND);
  WSContentSpaceButton((WebUseManagementSubmenu()) ? BUTTON_MANAGEMENT : BUTTON_MAIN);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void HandleConsoleRefresh(void) {
  String svalue = Webserver->arg(F("c1"));
  if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
    ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
  }

  char stmp[8];
  WebGetArg(PSTR("c2"), stmp, sizeof(stmp));
  uint32_t index = 0;                // Initial start, dump all
  if (strlen(stmp)) { index = atoi(stmp); }

  WSContentBegin(200, CT_PLAIN);
  WSContentSend_P(PSTR("%d}1%d}1"), TasmotaGlobal.log_buffer_pointer, Web.reset_web_log_flag);
  if (!Web.reset_web_log_flag) {
    index = 0;
    Web.reset_web_log_flag = true;
  }
  Web.cflg = (index);
  char* line;
  size_t len;
  while (GetLog(Settings->weblog_level, &index, &line, &len)) {
    if (!LogDataJsonPrettyPrint(line, len -1, WSContentSendLDJsonPPCb)) {
      WSContentSendLDJsonPPCb(line, len -1);
    }
  }
  WSContentSend_P(PSTR("}1"));
  WSContentEnd();
}

void WSContentSendLDJsonPPCb(const char* line, uint32_t len) {
  if (Web.cflg) { WSContentSend_P(PSTR("\n")); }
  WSContentSend(line, len);
  Web.cflg = true;
}

/********************************************************************************************/

void HandleNotFound(void) {
//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Not found (%s)"), Webserver->uri().c_str());
#ifndef NO_CAPTIVE_PORTAL
  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the error page.
#endif  // NO_CAPTIVE_PORTAL

#ifdef USE_EMULATION
#ifdef USE_EMULATION_HUE
  String path = Webserver->uri();
  if ((EMUL_HUE == Settings->flag2.emulation) && (path.startsWith(F("/api")))) {
    HandleHueApi(&path);
  } else
#endif  // USE_EMULATION_HUE
#endif  // USE_EMULATION
  {
    WSContentBegin(404, CT_PLAIN);
    WSContentSend_P(PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), Webserver->uri().c_str(), (Webserver->method() == HTTP_GET) ? PSTR("GET") : PSTR("POST"), Webserver->args());
    for (uint32_t i = 0; i < Webserver->args(); i++) {
      WSContentSend_P(PSTR(" %s: %s\n"), Webserver->argName(i).c_str(), Webserver->arg(i).c_str());
    }
    WSContentEnd();
  }
}

/********************************************************************************************/

#ifndef NO_CAPTIVE_PORTAL
/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
bool CaptivePortal(void) {
  // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
  if ((WifiIsInManagerMode()) && !ValidIpAddress(Webserver->hostHeader().c_str())) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    Webserver->sendHeader(F("Location"), String(F("http://")) + IPGetListeningAddressStr(), true);
    WSSend(302, CT_PLAIN, "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    Webserver->client().stop();  // Stop is needed because we sent no content length
    return true;
  }
  return false;
}
#endif  // NO_CAPTIVE_PORTAL

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifndef FIRMWARE_MINIMAL

enum {QUERY_DEFAULT=0, QUERY_RUN};
int WebQuery(char *buffer, int query_function);

#ifdef USE_WEBRUN
/*-------------------------------------------------------------------------------------------*/

char *WebRunBuffer = nullptr;
char *WebRunContext = nullptr;
bool WebRunMutex = false;

void WebRunLoop(void) {
  if (WebRunBuffer && !WebRunMutex && BACKLOG_EMPTY) {
    WebRunMutex = true;
    char *command = strtok_r(WebRunContext, "\n\r", &WebRunContext);
    if (command) {
      while (isspace(*command)) command++; // skip space
      if (*command && ';' != *command)
        ExecuteCommand(command, SRC_WEB);
    } else {
      free(WebRunBuffer);
      WebRunBuffer = WebRunContext = nullptr;
    }
    WebRunMutex = false;
  }
}

/*-------------------------------------------------------------------------------------------*/

void WebRunInit(const char *command_buffer) {
  if (!WebRunBuffer) {
    int len = strlen(command_buffer);
    WebRunContext = WebRunBuffer = (char*)malloc(len+1);
    if (WebRunBuffer) {
      memcpy(WebRunBuffer, command_buffer, len);
      WebRunBuffer[len] = 0;
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("WEBRUN: not enough memory"));
    }
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WEBRUN: previous not completed"));
  }
}
#endif // #ifdef USE_WEBRUN

/*-------------------------------------------------------------------------------------------*/

int WebQuery(char *buffer, int query_function = 0) {
  // http://192.168.1.1/path GET                                         -> Sends HTTP GET http://192.168.1.1/path
  // http://192.168.1.1/path POST {"some":"message"}                     -> Sends HTTP POST to http://192.168.1.1/path with body {"some":"message"}
  // http://192.168.1.1/path PUT [Autorization: Bearer abcdxyz] potato   -> Sends HTTP PUT to http://192.168.1.1/path with authorization header and body "potato"
  // http://192.168.1.1/path PATCH patchInfo                             -> Sends HTTP PATCH to http://192.168.1.1/path with body "potato"

  // Valid HTTP Commands: GET, POST, PUT, and PATCH
  // An unlimited number of headers can be sent per request, and a body can be sent for all command types
  // The body will be ignored if sending a GET command

#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  HTTPClientLight http;
#else // HTTP only
  WiFiClient http_client;
  HTTPClient http;
#endif

  int status = WEBCMND_WRONG_PARAMETERS;

  char *temp;
  const char *url = strtok_r(buffer, " ", &temp);
  const char *method = strtok_r(temp, " ", &temp);

  if (url) {
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
    if (http.begin(UrlEncode(url))) {
#else // HTTP only
    if (http.begin(http_client, UrlEncode(url))) {
#endif
      char empty_body[1] = { 0 };
      char *body = empty_body;
      if (temp) {                             // There is a body and/or header
        if (temp[0] == '[') {                 // Header information was sent; decode it
          temp += 1;
          temp = strtok_r(temp, "]", &body);
          bool headerFound = true;
          while (headerFound) {
            char *header = strtok_r(temp, ":", &temp);
            if (header) {
              char *headerBody = strtok_r(temp, "|", &temp);
              if (headerBody) {
                http.addHeader(header, headerBody);
              }
              else headerFound = false;
            }
            else headerFound = false;
          }
        } else {                              // No header information was sent, but there was a body
          body = temp;
        }
      }

      int http_code;
      if ((!method) || 0 == strcasecmp_P(method, PSTR("GET"))) { http_code = http.GET(); }
      else if (0 == strcasecmp_P(method, PSTR("POST"))) { http_code = http.POST(body); }
      else if (0 == strcasecmp_P(method, PSTR("PUT"))) { http_code = http.PUT(body); }
      else if (0 == strcasecmp_P(method, PSTR("PATCH"))) { http_code = http.PATCH(body); }
      else return status;

      if (http_code > 0) {                    // http_code will be negative on error
#if defined(USE_WEBSEND_RESPONSE) || defined(USE_WEBRUN)
        if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
          // Return received data to the user - Adds 900+ bytes to the code
          String response = http.getString(); // File found at server - may need lot of ram or trigger out of memory!
          const char* read = response.c_str();
//          uint32_t len = response.length() + 1;
//          AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Response '%*_H' = %s"), len, (uint8_t*)read, read);
#ifdef USE_WEBRUN
          if (QUERY_RUN == query_function)
            WebRunInit(read);
#endif
#ifdef USE_WEBSEND_RESPONSE
          char text[3] = { 0 };               // Make room foor double %
          text[0] = *read++;
          if (text[0] != '\0') {
            Response_P(PSTR("{\"" D_CMND_WEBQUERY "\":"));
            bool assume_json = (text[0] == '{') || (text[0] == '[');
            if (!assume_json) { ResponseAppend_P(PSTR("\"")); }
            while (text[0] != '\0') {
              if (text[0] > 31) {             // Remove control characters like linefeed
                if ('%' == text[0]) {         // Fix char string formatting for %
                  text[1] = '%';
                }
                if (assume_json) {
                  if (ResponseAppend_P(text) == ResponseSize()) { break; };
                } else {
                  if (ResponseAppend_P(EscapeJSONString(text).c_str()) == ResponseSize()) { break; };
                }
              }
              text[0] = *read++;
              text[1] = '\0';
            }
            if (!assume_json) { ResponseAppend_P(PSTR("\"")); }
            ResponseJsonEnd();
#ifdef USE_SCRIPT
            // recursive call must be possible in this case
            void script_setaflg(uint8_t flg);
            script_setaflg(0);
#endif  // USE_SCRIPT
            status = WEBCMND_VALID_RESPONSE;
          } else {
#endif  // USE_WEBSEND_RESPONSE
            status = WEBCMND_DONE;
          }
        } else
#endif  // USE_WEBSEND_RESPONSE || USE_WEBRUN
        status = WEBCMND_DONE;
      } else {
        status = WEBCMND_CONNECT_FAILED;
      }
      http.end();                             // Clean up connection data
    } else {
      status = WEBCMND_HOST_NOT_FOUND;
    }
  }
  return status;
}


/*-------------------------------------------------------------------------------------------*/

const char kWebCmndStatus[] PROGMEM = D_JSON_DONE "|" D_JSON_WRONG_PARAMETERS "|" D_JSON_CONNECT_FAILED "|" D_JSON_HOST_NOT_FOUND "|" D_JSON_MEMORY_ERROR "|" 
#ifdef USE_WEBGETCONFIG
  "|" D_JSON_FILE_NOT_FOUND "|" D_JSON_OTHER_HTTP_ERROR "|" D_JSON_CONNECTION_LOST "|" D_JSON_INVALID_FILE_TYPE
#endif // USE_WEBGETCONFIG
;

const char kWebCommands[] PROGMEM = "|"  // No prefix
  D_CMND_WEBLOG "|"
  D_CMND_WEBTIME "|"
#ifdef USE_EMULATION
  D_CMND_EMULATION "|"
#endif
#ifdef USE_SENDMAIL
  D_CMND_SENDMAIL "|"
#endif
  D_CMND_WEBSERVER "|" D_CMND_WEBPASSWORD "|" D_CMND_WEBREFRESH "|" D_CMND_WEBSEND "|" D_CMND_WEBQUERY "|"
  D_CMND_WEBCOLOR "|" D_CMND_WEBSENSOR "|" D_CMND_WEBBUTTON "|" D_CMND_WEBCANVAS
#ifdef USE_WEBGETCONFIG
  "|" D_CMND_WEBGETCONFIG
#endif
#ifdef USE_WEBRUN
  "|" D_CMND_WEBRUN
#endif
#ifdef USE_CORS
  "|" D_CMND_CORS
#endif
;

void (* const WebCommand[])(void) PROGMEM = {
  &CmndWeblog,
  &CmndWebTime,
#ifdef USE_EMULATION
  &CmndEmulation,
#endif
#ifdef USE_SENDMAIL
  &CmndSendmail,
#endif
  &CmndWebServer, &CmndWebPassword, &CmndWebRefresh, &CmndWebSend, &CmndWebQuery,
  &CmndWebColor, &CmndWebSensor, &CmndWebButton, &CmndWebCanvas
#ifdef USE_WEBGETCONFIG
  , &CmndWebGetConfig
#endif
#ifdef USE_WEBRUN
  , &CmndWebRun
#endif
#ifdef USE_CORS
  , &CmndCors
#endif
  };

/*********************************************************************************************/

void CmndWebTime(void) {
  // 2017-03-07T11:08:02-07:00
  // 0123456789012345678901234
  //
  // WebTime 0,16  = 2017-03-07T11:08 - No seconds
  // WebTime 11,19 = 11:08:02
  uint32_t values[2] = { 0 };
  String datetime = GetDateAndTime(DT_LOCAL);
  if (ParseParameters(2, values) > 1) {
    Settings->web_time_start = values[0];
    Settings->web_time_end = values[1];
    if (Settings->web_time_end > datetime.length()) { Settings->web_time_end = datetime.length(); }
    if (Settings->web_time_start >= Settings->web_time_end) { Settings->web_time_start = 0; }
  }
  Response_P(PSTR("{\"%s\":[%d,%d],\"Time\":\"%s\"}"),
    XdrvMailbox.command, Settings->web_time_start, Settings->web_time_end,
    datetime.substring(Settings->web_time_start, Settings->web_time_end).c_str());
}

#ifdef USE_EMULATION
/*-------------------------------------------------------------------------------------------*/

void CmndEmulation(void) {
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
#if defined(USE_EMULATION_WEMO) && defined(USE_EMULATION_HUE)
  if ((XdrvMailbox.payload >= EMUL_NONE) && (XdrvMailbox.payload < EMUL_MAX)) {
#else
#ifndef USE_EMULATION_WEMO
  if ((EMUL_NONE == XdrvMailbox.payload) || (EMUL_HUE == XdrvMailbox.payload)) {
#endif
#ifndef USE_EMULATION_HUE
  if ((EMUL_NONE == XdrvMailbox.payload) || (EMUL_WEMO == XdrvMailbox.payload)) {
#endif
#endif
    Settings->flag2.emulation = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
#endif
  ResponseCmndNumber(Settings->flag2.emulation);
}
#endif  // USE_EMULATION

#ifdef USE_SENDMAIL
/*-------------------------------------------------------------------------------------------*/

void CmndSendmail(void) {
  if (XdrvMailbox.data_len > 0) {
    uint8_t result = SendMail(XdrvMailbox.data);
    char stemp1[20];
    ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebCmndStatus));
  }
}
#endif  // USE_SENDMAIL

/*-------------------------------------------------------------------------------------------*/

void CmndWebServer(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings->webserver = XdrvMailbox.payload;
  }
  if (Settings->webserver) {
    Response_P(PSTR("{\"" D_CMND_WEBSERVER "\":\"" D_JSON_ACTIVE_FOR " %s " D_JSON_ON_DEVICE " %s " D_JSON_WITH_IP_ADDRESS " %_I\"}"),
      (2 == Settings->webserver) ? PSTR(D_ADMIN) : PSTR(D_USER), NetworkHostname(), (uint32_t)NetworkAddress());
  } else {
    ResponseCmndStateText(0);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndWebPassword(void) {
  bool show_asterisk = (2 == XdrvMailbox.index);
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_WEBPWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? WEB_PASSWORD : XdrvMailbox.data);
    if (!show_asterisk) {
      ResponseCmndChar(SettingsText(SET_WEBPWD));
    }
  } else {
    show_asterisk = true;
  }
  if (show_asterisk) {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndWeblog(void) {
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    Settings->weblog_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->weblog_level);
}

/*-------------------------------------------------------------------------------------------*/

void CmndWebRefresh(void) {
  if ((XdrvMailbox.payload > 399) && (XdrvMailbox.payload <= 65000)) {
    Settings->web_refresh = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->web_refresh);
}

/*-------------------------------------------------------------------------------------------*/

int WebSend(char *buffer) {
  // [tasmota] POWER1 ON                                               --> Sends http://tasmota/cm?cmnd=POWER1 ON
  // [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
  // [tasmota] /any/link/starting/with/a/slash.php?log=123             --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123
  // [tasmota,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123

  char *host;
  char *user;
  char *password;
  char *command;
  int status = WEBCMND_WRONG_PARAMETERS;

                                              // buffer = |  [  192.168.178.86  :  80  ,  admin  :  joker  ]    POWER1 ON   |
  host = strtok_r(buffer, "]", &command);     // host = |  [  192.168.178.86  :  80  ,  admin  :  joker  |, command = |    POWER1 ON   |
  if (host && command) {
    RemoveSpace(host);                        // host = |[192.168.178.86:80,admin:joker|
    host++;                                   // host = |192.168.178.86:80,admin:joker| - Skip [
    host = strtok_r(host, ",", &user);        // host = |192.168.178.86:80|, user = |admin:joker|
    String url = F("http://");                // url = |http://|
    url += host;                              // url = |http://192.168.178.86:80|

    command = Trim(command);                  // command = |POWER1 ON| or |/any/link/starting/with/a/slash.php?log=123|
    if (command[0] != '/') {
      url += F("/cm?");                       // url = |http://192.168.178.86/cm?|
      if (user) {
        user = strtok_r(user, ":", &password);  // user = |admin|, password = |joker|
        if (user && password) {
          char userpass[200];
          snprintf_P(userpass, sizeof(userpass), PSTR("user=%s&password=%s&"), user, password);
          url += userpass;                    // url = |http://192.168.178.86/cm?user=admin&password=joker&|
        }
      }
      url += F("cmnd=");                      // url = |http://192.168.178.86/cm?cmnd=| or |http://192.168.178.86/cm?user=admin&password=joker&cmnd=|
    }
    url += UrlEncode(command);                // url = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
    url += F(" GET");                         // url = |http://192.168.178.86/cm?cmnd=POWER1%20ON GET|

    DEBUG_CORE_LOG(PSTR("WEB: Uri '%s'"), url.c_str());
    status = WebQuery(const_cast<char*>(url.c_str()));
  }
  return status;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void CmndWebSend(void) {
  if (XdrvMailbox.data_len > 0) {
    uint32_t result = WebSend(XdrvMailbox.data);
    if (result != WEBCMND_VALID_RESPONSE) {
      char stemp1[20];
      ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebCmndStatus));
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndWebQuery(void) {
  if (XdrvMailbox.data_len > 0) {
    uint32_t result = WebQuery(XdrvMailbox.data);
    if (result != WEBCMND_VALID_RESPONSE) {
      char stemp1[20];
      ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebCmndStatus));
    }
  }
}

#ifdef USE_WEBRUN
/*-------------------------------------------------------------------------------------------*/

void CmndWebRun(void) {
  if (XdrvMailbox.data_len > 0) {
    uint32_t result = WebQuery(XdrvMailbox.data, QUERY_RUN);
    if (result != WEBCMND_VALID_RESPONSE) {
      char stemp1[20];
      ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebCmndStatus));
    }
  }
}
#endif // #ifdef USE_WEBRUN

#ifdef USE_WEBGETCONFIG
/*-------------------------------------------------------------------------------------------*/

int WebGetConfig(char *buffer) {
  // http://user:password@server:port/path/%id%.dmp  : %id% will be expanded to MAC address

  int status = WEBCMND_WRONG_PARAMETERS;

  RemoveSpace(buffer);                        // host = |[192.168.178.86:80,admin:joker|
  String url = ResolveToken(buffer);

  DEBUG_CORE_LOG(PSTR("WEB: Config Uri '%s'"), url.c_str());


#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  HTTPClientLight http;
  if (http.begin(UrlEncode(url))) {         // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
#else // HTTP only
  WiFiClient http_client;
  HTTPClient http;
  if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
#endif
    int http_code = http.GET();             // Start connection and send HTTP header
    if (http_code > 0) {                    // http_code will be negative on error
      status = WEBCMND_DONE;
      if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
        WiFiClient *stream = http.getStreamPtr();
        int len = http.getSize();
        if (len <= sizeof(TSettings)) { 
          len = sizeof(TSettings);
        }
        if (SettingsBufferAlloc(len)) {
          uint8_t *buff = settings_buffer;
          while (http.connected() && (len > 0)) {
            size_t size = stream->available();
            if (size) {
              int read = stream->readBytes(buff, len);
              len -= read;
            }
            delayMicroseconds(1);
          }
          if (len) {
            DEBUG_CORE_LOG(PSTR("WEB: Connection lost"));
            status = WEBCMND_CONNECTION_LOST;
          } else if (SettingsConfigRestore()) {
            AddLog(LOG_LEVEL_INFO, PSTR("WEB: Settings applied, restarting"));
            TasmotaGlobal.restart_flag = 2;  // Always restart to re-enable disabled features during update
          } else {
            DEBUG_CORE_LOG(PSTR("WEB: Settings file invalid"));
            status = WEBCMND_INVALID_FILE;
          }
        } else {
          DEBUG_CORE_LOG(PSTR("WEB: Memory error (%d) or invalid file length (%d)"), settings_buffer, len);
          status = WEBCMND_MEMORY_ERROR;
        }
      } else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("WEB: HTTP error %d"), http_code);
        status = (http_code == HTTP_CODE_NOT_FOUND) ? WEBCMND_FILE_NOT_FOUND : WEBCMND_OTHER_HTTP_ERROR;
      }
    } else {
      DEBUG_CORE_LOG(PSTR("WEB: Connection failed"));
      status = 2;                           // Connection failed
    }
    http.end();                             // Clean up connection data
  } else {
    status = 3;                             // Host not found or connection error
  }

  return status;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void CmndWebGetConfig(void) {
  // WebGetConfig http://myserver:8000/tasmota/conf/%id%.dmp where %id% is expanded to device mac address
  // WebGetConfig http://myserver:8000/tasmota/conf/Config_demo_9.5.0.8.dmp
  if (XdrvMailbox.data_len > 0) {
    uint32_t result = WebGetConfig(XdrvMailbox.data);
    char stemp1[20];
    ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebCmndStatus));
  }
}
#endif // USE_WEBGETCONFIG

/*-------------------------------------------------------------------------------------------*/

bool JsonWebColor(const char* dataBuf) {
  // Default (Dark theme)
  // {"WebColor":["#eaeaea","#252525","#4f4f4f","#000","#ddd","#65c115","#1f1f1f","#ff5661","#008000","#faffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#faffff","#999","#eaeaea","#08405e"]}
  // Default pre v7 (Light theme)
  // {"WebColor":["#000","#fff","#f2f2f2","#000","#fff","#000","#fff","#f00","#008000","#fff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#fff","#999","#000","#08405e"]}
  // {"WebColor":["#000000","#ffffff","#f2f2f2","#000000","#ffffff","#000000","#ffffff","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000","#08405e"]}

  JsonParser parser((char*) dataBuf);
  JsonParserObject root = parser.getRootObject();
  JsonParserArray arr = root[PSTR(D_CMND_WEBCOLOR)].getArray();
  if (arr) {  // if arr is valid, i.e. json is valid, the key D_CMND_WEBCOLOR was found and the token is an arra
    uint32_t i = 0;
    for (auto color : arr) {
      if (i < COL_LAST) {
        WebHexCode(i, color.getStr());
      } else {
        break;
      }
      i++;
    }
  }
  return true;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void CmndWebColor(void) {
  if (XdrvMailbox.data_len > 0) {
    if (strchr(XdrvMailbox.data, '{') == nullptr) {  // If no JSON it must be parameter
      if ((XdrvMailbox.data_len > 3) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= COL_LAST)) {
        WebHexCode(XdrvMailbox.index -1, XdrvMailbox.data);
      }
      else if (0 == XdrvMailbox.payload) {
        SettingsDefaultWebColor();
      }
    }
    else {
#ifndef FIRMWARE_MINIMAL      // if tasmota-minimal, read only and don't parse JSON
      JsonWebColor(XdrvMailbox.data);
#endif // FIRMWARE_MINIMAL
    }
  }
  Response_P(PSTR("{\"%s\":["), XdrvMailbox.command);
  for (uint32_t i = 0; i < COL_LAST; i++) {
    ResponseAppend_P(PSTR("%s\"#%06x\""), (i>0)?",":"", WebColor(i));
  }
  ResponseAppend_P(PSTR("]}"));
}

/*-------------------------------------------------------------------------------------------*/

void CmndWebSensor(void) {
  if (XdrvMailbox.index < MAX_XSNS_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings->sensors[1][XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
    }
  }
  Response_P(PSTR("{\"" D_CMND_WEBSENSOR "\":"));
  XsnsSensorState(1);
  ResponseJsonEnd();
}

/*-------------------------------------------------------------------------------------------*/

String *WebButton1732[16] = {0,};

void SetWebButton(uint8_t button_index, const char *text) {
  if (button_index < 16) 
    SettingsUpdateText(SET_BUTTON1 + button_index, text);
  else if (button_index < MAX_BUTTON_TEXT) {
    button_index -= 16;
    if (!WebButton1732[button_index]) 
      WebButton1732[button_index] = new String(text);
    else
      *WebButton1732[button_index] = text;
  }
}

const char* GetWebButton(uint8_t button_index) {
  static char empty[1] = {0};
  if (button_index < 16) 
    return SettingsText(SET_BUTTON1 + button_index);
  else if (button_index < MAX_BUTTON_TEXT) {
    button_index -= 16;
    if (WebButton1732[button_index])
      return WebButton1732[button_index]->c_str();
  }
  return empty;
}

void CmndWebButton(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_BUTTON_TEXT)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_BUTTON1, MAX_BUTTON_TEXT);
    } else {
      if (XdrvMailbox.data_len > 0) {
        SetWebButton(XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
      }
      ResponseCmndIdxChar(GetWebButton(XdrvMailbox.index -1));
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void CmndWebCanvas(void) {
  /*
  WebCanvas allows GUI body canvas configuration using a color, "url" or "gradient".
  The provided text overrules the body CSS background property "body{background:<WebCanvas> 0 0 / cover no-repeat fixed;}"
  - WebCanvas "                                                            // Set canvas to WebColor2
  - WebCanvas 0                                                            // Set canvas to WebColor2
  - WebCanvas red                                                          // Red canvas
  - WebCanvas linear-gradient(#F02 7%,#F93,#FF4,#082,#00F,#708 93%)        // Gradient pride flag
  - WebCanvas linear-gradient(#F02 16%,#F93 16% 33%,#FF4 33% 50%,#082 50% 67%,#00F 67% 84%,#708 84%)  // Pride flag
  - WebCanvas linear-gradient(90deg,#05A 33%,#FFF 33% 67%,#F43 67%)        // Flag France
  - WebCanvas linear-gradient(#000 33%,#D00 33% 67%,#FC0 67%)              // Flag Germany
  - WebCanvas linear-gradient(#059 33%,#FFF 33% 67%,#F43 67%)              // Flag The Netherlands
  - WebCanvas linear-gradient(#05B 50%,#FD0 50%)                           // Flag Ukraine
  - WebCanvas linear-gradient(#FFF 33%,#07D 33% 67%,#F34 67%)              // Flag Russia
  - WebCanvas url(http://ota.tasmota.com/tasmota/images/prf.png)           // Pride flag
  - WebCanvas url(http://ota.tasmota.com/tasmota/images/tasmota_logo.png)  // Tasmota logo
  */
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_CANVAS, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_CANVAS));
}

#ifdef USE_CORS
/*-------------------------------------------------------------------------------------------*/

void CmndCors(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_CORS, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? CORS_DOMAIN : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_CORS));
}
#endif  // USE_CORS

#endif  // not FIRMWARE_MINIMAL

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv01(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      PollDnsWebserver();
#ifdef USE_WEBRUN
      WebRunLoop();
#endif // #ifdef USE_WEBRUN
#ifdef USE_EMULATION
      if (Settings->flag2.emulation) { PollUdp(); }
#endif  // USE_EMULATION
      break;
    case FUNC_EVERY_SECOND:
      if (Web.initial_config) {
        Wifi.config_counter = 200;    // Do not restart the device if it has SSId Blank
      }
      if (Wifi.wifi_test_counter) {
        Wifi.wifi_test_counter--;
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_TRYING_TO_CONNECT " %s"), SettingsText(SET_STASSID1));
        IPAddress local_ip;
        if (WifiGetIP(&local_ip, true)) {            // Got IP - Connection Established (exclude AP address)
          Wifi.wifi_test_AP_TIMEOUT = false;
          Wifi.wifi_test_counter = 0;
          Wifi.wifiTest = WIFI_TEST_FINISHED;
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CMND_SSID " %s: " D_CONNECTED " - " D_IP_ADDRESS " %s"), SettingsText(Wifi.wifi_Test_Save_SSID2 ? SET_STASSID2 : SET_STASSID1), local_ip.toString().c_str());
//          TasmotaGlobal.blinks = 255;                    // Signal wifi connection with blinks
          if (MAX_WIFI_OPTION != Wifi.old_wificonfig) {
            TasmotaGlobal.wifi_state_flag = Settings->sta_config = Wifi.old_wificonfig;
          }
          TasmotaGlobal.save_data_counter = Wifi.save_data_counter;
          Settings->save_data = Wifi.save_data_counter;
          SettingsSaveAll();

          if ( Wifi.wifi_Test_Restart ) { TasmotaGlobal.restart_flag = 2; }

#if (!RESTART_AFTER_INITIAL_WIFI_CONFIG)
          Web.initial_config = false;
          Web.state = HTTP_ADMIN;
#endif
        } else if (!Wifi.wifi_test_counter) { // Test TimeOut
          Wifi.wifi_test_counter = 0;
          Wifi.wifiTest = WIFI_TEST_FINISHED_BAD;
          switch (WiFi.status()) {
            case WL_CONNECTED:
              AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS));
              Wifi.wifi_test_AP_TIMEOUT = false;
              break;
            case WL_NO_SSID_AVAIL:
              AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED));
              Wifi.wifi_test_AP_TIMEOUT = false;
              break;
            case WL_CONNECT_FAILED:
              AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD));
              Wifi.wifi_test_AP_TIMEOUT = false;
              break;
            default:  // WL_IDLE_STATUS and WL_DISCONNECTED - SSId in range but no answer from the router
              AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT));
              // If this error occurs twice, Tasmota will connect directly to the router without testing credentials.
              //   ESP8266 in AP+STA mode can manage only 11b and 11g, so routers that are 11n-ONLY won't respond.
              //   For this case, the user will see in the UI a message to check credentials. After that, if the user hits
              //   save and connect again, and the CONNECT_FAILED_AP_TIMEOUT is shown again, Credentials will be saved and
              //   Tasmota will restart and try to connect in STA mode only (11b/g/n).
              //
              //   If it fails again, depending on the WIFICONFIG settings, the user will need to wait or will need to
              //   push 6 times the button to enable Tasmota AP mode again.
              if (Wifi.wifi_test_AP_TIMEOUT) {
                Wifi.wifiTest = WIFI_TEST_FINISHED;
                AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CMND_SSID " %s: " D_ATTEMPTING_CONNECTION), SettingsText(Wifi.wifi_Test_Save_SSID2 ? SET_STASSID2 : SET_STASSID1) );
                if (MAX_WIFI_OPTION != Wifi.old_wificonfig) {
                  TasmotaGlobal.wifi_state_flag = Settings->sta_config = Wifi.old_wificonfig;
                }
                TasmotaGlobal.save_data_counter = Wifi.save_data_counter;
                Settings->save_data = Wifi.save_data_counter;
                SettingsSaveAll();
              }
              Wifi.wifi_test_AP_TIMEOUT = true;
          }
          WiFi.scanNetworks(); // restart scan
        }
      }
      break;
#ifndef FIRMWARE_MINIMAL
    case FUNC_COMMAND:
      result = DecodeCommand(kWebCommands, WebCommand);
      break;
#endif  // FIRMWARE_MINIMAL
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}
#endif  // USE_WEBSERVER
