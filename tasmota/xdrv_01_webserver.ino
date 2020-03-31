/*
  xdrv_01_webserver.ino - webserver for Tasmota

  Copyright (C) 2020  Theo Arends

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
#define WIFI_SOFT_AP_CHANNEL                  1          // Soft Access Point Channel number between 1 and 11 as used by WifiManager web GUI
#endif

const uint16_t CHUNKED_BUFFER_SIZE = (MESSZ / 2) - 100;  // Chunk buffer size (should be smaller than half mqtt_data size = MESSZ)

const uint16_t HTTP_REFRESH_TIME = 2345;                 // milliseconds
#define HTTP_RESTART_RECONNECT_TIME           9000       // milliseconds
#define HTTP_OTA_RESTART_RECONNECT_TIME       20000      // milliseconds

#include <ESP8266WebServer.h>
#include <DNSServer.h>

#ifdef USE_RF_FLASH
uint8_t *efm8bb1_update = nullptr;
#endif  // USE_RF_FLASH

enum UploadTypes { UPL_TASMOTA, UPL_SETTINGS, UPL_EFM8BB1, UPL_TASMOTASLAVE };

static const char * HEADER_KEYS[] = { "User-Agent", };

const char HTTP_HEADER[] PROGMEM =
  "<!DOCTYPE html><html lang=\"" D_HTML_LANGUAGE "\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<title>%s - %s</title>"

  "<script>"
  "var x=null,lt,to,tp,pc='';"            // x=null allow for abortion

#ifdef USE_JAVASCRIPT_ES6
// Following bytes saving ES6 syntax fails on old browsers like IE 11 - https://kangax.github.io/compat-table/es6/
  "eb=s=>document.getElementById(s);"     // Alias to save code space
  "qs=s=>document.querySelector(s);"      // Alias to save code space
  "sp=i=>eb(i).type=(eb(i).type==='text'?'password':'text');"  // Toggle password visibility
  "wl=f=>window.addEventListener('load',f);" // Execute multiple window.onload
  ;
#else
  "function eb(s){"
    "return document.getElementById(s);"  // Alias to save code space
  "}"
  "function qs(s){"                       // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function sp(i){"                       // Toggle password visibility
    "eb(i).type=(eb(i).type==='text'?'password':'text');"
  "}"
  "function wl(f){"                       // Execute multiple window.onload
    "window.addEventListener('load',f);"
  "}";
#endif

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

const char HTTP_SCRIPT_ROOT[] PROGMEM =
#ifdef USE_SCRIPT_WEB_DISPLAY
  "var rfsh=1;"
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "if (rfsh) {"
      "x.open('GET','.?m=1'+a,true);"       // ?m related to WebServer->hasArg("m")
      "x.send();"
      "lt=setTimeout(la,%d);"               // Settings.web_refresh
    "}"
  "}"
  "function seva(par,ivar){"
    "la('&sv='+ivar+'_'+par);"
  "}"
  "function siva(par,ivar){"
    "rfsh=1;"
    "la('&sv='+ivar+'_'+par);"
    "rfsh=0;"
  "}"
  "function pr(f){"
    "if (f) {"
      "lt=setTimeout(la,%d);"
      "rfsh=1;"
    "} else {"
      "clearTimeout(lt);"
      "rfsh=0;"
    "}"
  "}";
#else  // USE_SCRIPT_WEB_DISPLAY
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "x.open('GET','.?m=1'+a,true);"       // ?m related to WebServer->hasArg("m")
    "x.send();"
    "lt=setTimeout(la,%d);"               // Settings.web_refresh
  "}";
#endif  // USE_SCRIPT_WEB_DISPLAY

const char HTTP_SCRIPT_ROOT_PART2[] PROGMEM =
  "function lc(v,i,p){"
    "if(eb('s')){"                        // Check if Saturation is in DOM otherwise javascript fails on la()
      "if(v=='h'||v=='d'){"               // Hue or Brightness changed so change Saturation colors too
        "var sl=eb('sl4').value;"
        "eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';"
      "}"
    "}"
    "la('&'+v+i+'='+p);"
  "}"
  "wl(la);";

const char HTTP_SCRIPT_WIFI[] PROGMEM =
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}";

const char HTTP_SCRIPT_RELOAD[] PROGMEM =
  "setTimeout(function(){location.href='.';}," STR(HTTP_RESTART_RECONNECT_TIME) ");";

// Local OTA upgrade requires more time to complete cp: before web ui should be reloaded
const char HTTP_SCRIPT_RELOAD_OTA[] PROGMEM =
  "setTimeout(function(){location.href='.';}," STR(HTTP_OTA_RESTART_RECONNECT_TIME) ");";

const char HTTP_SCRIPT_CONSOL[] PROGMEM =
  "var sn=0,id=0;"                        // Scroll position, Get most of weblog initially
  "function l(p){"                        // Console log and command service
    "var c,o='',t;"
    "clearTimeout(lt);"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"                       // Console command id
      "o='&c1='+encodeURIComponent(c.value);"
      "c.value='';"
      "t.scrollTop=99999;"
      "sn=t.scrollTop;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var z,d;"
          "d=x.responseText.split(/}1/);"  // Field separator
          "id=d.shift();"
          "if(d.shift()==0){t.value='';}"
          "z=d.shift();"
          "if(z.length>0){t.value+=z;}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
        "}"
      "};"
      "x.open('GET','cs?c2='+id+o,true);"  // Related to WebServer->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
      "x.send();"
    "}"
    "lt=setTimeout(l,%d);"
    "return false;"
  "}"

  // Console command history - part 2
  "var hc=[],cn=0;"                       // hc = History commands, cn = Number of history being shown
  "function cH(a){"
    "var b=qs('#c1'),c=a.keyCode;"        // c1 = Console command id
    "if(38==c||40==c){b.autocomplete='off';}"  // Arrow up or down must be a keyboard so stop browser autocomplete
    "38==c?(++cn>hc.length&&(cn=hc.length),b.value=hc[cn-1]||''):"   // Arrow Up
    "40==c?(0>--cn&&(cn=0),b.value=hc[cn-1]||''):"                   // Arrow Down
    "13==c&&(hc.length>19&&hc.pop(),hc.unshift(b.value),cn=0)"       // Enter, 19 = Max number -1 of commands in history
  "}"

  "wl(l);";

const char HTTP_MODULE_TEMPLATE_REPLACE[] PROGMEM =
  "}2%d'>%s (%d}3";                       // }2 and }3 are used in below os.replace

const char HTTP_SCRIPT_MODULE_TEMPLATE[] PROGMEM =
  "var os;"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=s;"
  "}"
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}";

const char HTTP_SCRIPT_TEMPLATE[] PROGMEM =
  "var c;"                                // Need a global for BASE
  "function x1(b){"
    "var i,j,g,k,o;"
    "o=b.responseText.split(/}1/);"       // Field separator
    "k=o.shift();"                        // Template name
    "if(eb('s1').value==''){"
      "eb('s1').value=k;"                 // Set NAME if not yet set
    "}"
    "os=o.shift();"                       // Complete GPIO sensor list
    "as=o.shift();"                       // Complete ADC0 list
    "g=o.shift().split(',');"             // Array separator
    "j=0;"
    "for(i=0;i<13;i++){"                  // Supports 13 GPIOs
      "if(6==i){j=9;}"
      "if(8==i){j=12;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}"
    "g=o.shift();"                        // FLAG
    "os=as;"
    "sk(g&15,17);"                        // Set ADC0
    "g>>=4;"
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

  "function x2(a){"
    "os=a.responseText;"
    "sk(17,99);"                          // 17 = WEMOS
    "st(" STR(USER_MODULE) ");"
  "}"

#ifdef USE_JAVASCRIPT_ES6
  "sl=()=>ld('tp?m=1',x2);"               // ?m related to WebServer->hasArg("m")
#else
  "function sl(){"
    "ld('tp?m=1',x2);"                    // ?m related to WebServer->hasArg("m")
  "}"
#endif

  "wl(sl);";

const char HTTP_SCRIPT_MODULE1[] PROGMEM =
  "function x1(a){"                       // Module Type
    "os=a.responseText;"
    "sk(%d,99);"
  "}"
  "function x2(b){"                       // GPIOs
    "os=b.responseText;";
const char HTTP_SCRIPT_MODULE2[] PROGMEM =
  "}"
  "function x3(a){"                       // ADC0
    "os=a.responseText;"
    "sk(%d,17);"
  "}"
  "function sl(){"
    "ld('md?m=1',x1);"                    // ?m related to WebServer->hasArg("m")
    "ld('md?g=1',x2);"                    // ?g related to WebServer->hasArg("g")
    "if(eb('g17')){"
      "ld('md?a=1',x3);"                  // ?a related to WebServer->hasArg("a")
    "}"
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

const char HTTP_HEAD_LAST_SCRIPT[] PROGMEM =
  "function jd(){"                        // Add label name='' based on provided id=''
    "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
    "while(i.length>=t){"
      "if(i[t]){"
        "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
      "}"
      "t++;"
    "}"

    // Console command history - part 1
    "if(qs('#c1')){"                      // c1 = Console command id
      "qs('#c1').addEventListener('keydown',cH);"
    "}"

  "}"
  "wl(jd);"                               // Add name='' to any id='' in input,button,textarea,select
  "</script>";

const char HTTP_HEAD_STYLE1[] PROGMEM =
  "<style>"
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:#%06x;}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  "input{width:100%%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  "input[type=range]{width:99%%;}"
  "select{width:100%%;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "textarea{resize:vertical;width:98%%;height:318px;padding:5px;overflow:auto;background:#%06x;color:#%06x;}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:verdana,sans-serif;background:#%06x;}"  // COLOR_BACKGROUND
  "td{padding:0px;}";
const char HTTP_HEAD_STYLE2[] PROGMEM =
  "button{border:0;border-radius:0.3rem;background:#%06x;color:#%06x;line-height:2.4rem;font-size:1.2rem;width:100%%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  "button:hover{background:#%06x;}"  // COLOR_BUTTON_HOVER
  ".bred{background:#%06x;}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:#%06x;}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{background:#%06x;}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:#%06x;}"  // COLOR_BUTTON_SAVE_HOVER
  "a{color:#%06x;text-decoration:none;}"  // COLOR_BUTTON
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ".r{border-radius:0.3em;padding:2px;margin:6px 2px;}";
const char HTTP_HEAD_STYLE3[] PROGMEM =
  "</style>"

  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;'>"  // COLOR_TEXT
#ifdef FIRMWARE_MINIMAL
  "<div style='text-align:center;color:#%06x;'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"  // COLOR_TEXT_WARNING
#endif
  "<div style='text-align:center;color:#%06x;'><noscript>" D_NOSCRIPT "<br></noscript>" // COLOR_TITLE
#ifdef LANGUAGE_MODULE_NAME
  "<h3>" D_MODULE " %s</h3>"
#else
  "<h3>%s " D_MODULE "</h3>"
#endif
  "<h2>%s</h2>";

const char HTTP_MSG_SLIDER_GRADIENT[] PROGMEM =
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc(\"%c\",%d,value)'>"
  "</div>";
const char HTTP_MSG_SLIDER_SHUTTER[] PROGMEM =
  "<div><span class='p'>" D_CLOSE "</span><span class='q'>" D_OPEN "</span></div>"
  "<div><input type='range' min='0' max='100' value='%d' onchange='lc(\"u\",%d,value)'></div>";

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
//  "<input id='c0' name='c0' type='checkbox'><b>" D_OPTION_TEXT "</b><br>"
  "</p></fieldset>";

const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='md'>"
  "<p></p><b>" D_MODULE_TYPE "</b> (%s)<br><select id='g99'></select><br>"
  "<br><table>";

const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='wi'>"
  "<p><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br><input id='s1' placeholder='" STA_SSID1 "' value='%s'></p>"
  "<p><b>" D_AP1_PASSWORD "</b><input type='checkbox' onclick='sp(\"p1\")'><br><input id='p1' type='password' placeholder='" D_AP1_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
  "<p><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br><input id='s2' placeholder='" STA_SSID2 "' value='%s'></p>"
  "<p><b>" D_AP2_PASSWORD "</b><input type='checkbox' onclick='sp(\"p2\")'><br><input id='p2' type='password' placeholder='" D_AP2_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
  "<p><b>" D_HOSTNAME "</b> (%s)<br><input id='h' placeholder='%s' value='%s'></p>"
  "<p><b>" D_CORS_DOMAIN "</b><input id='c' placeholder='" CORS_DOMAIN "' value='%s'></p>";

const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b>"
  "</legend><form method='get' action='lg'>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br><input id='lh' placeholder='" SYS_LOG_HOST "' value='%s'></p>"
  "<p><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br><input id='lp' placeholder='" STR(SYS_LOG_PORT) "' value='%d'></p>"
  "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br><input id='lt' placeholder='" STR(TELE_PERIOD) "' value='%d'></p>";

const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='co'>"
  "<p></p>"
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE "&nbsp;</b></legend>"
  "<p><input id='t1' placeholder='" D_TEMPLATE "' value='%s'></p>"
  "<p><input id='t2' type='checkbox'%s><b>" D_ACTIVATE "</b></p>"
  "</fieldset>"
  "<br>"
  "<b>" D_WEB_ADMIN_PASSWORD "</b><input type='checkbox' onclick='sp(\"wp\")'><br><input id='wp' type='password' placeholder='" D_WEB_ADMIN_PASSWORD "' value='" D_ASTERISK_PWD "'><br>"
  "<br>"
  "<input id='b1' type='checkbox'%s><b>" D_MQTT_ENABLE "</b><br>"
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
  "<br><b>" D_OTA_URL "</b><br><input id='o' placeholder='OTA_URL' value='%s'><br>"
  "<br><button type='submit'>" D_START_UPGRADE "</button></form>"
  "</fieldset><br><br>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
const char HTTP_FORM_RST_UPG[] PROGMEM =
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br><input type='file' name='u2'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";

const char HTTP_FORM_CMND[] PROGMEM =
  "<br><textarea readonly id='t1' cols='340' wrap='off'></textarea><br><br>"
  "<form method='get' onsubmit='return l(1);'>"
  "<input id='c1' placeholder='" D_ENTER_COMMAND "' autofocus><br>"
  //  "<br><button type='submit'>Send command</button>"
  "</form>";

const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%%'>";

const char HTTP_COUNTER[] PROGMEM =
  "<br><div id='t' style='text-align:center;'></div>";

const char HTTP_END[] PROGMEM =
  "<div style='text-align:right;font-size:11px;'><hr/><a href='https://bit.ly/tasmota' target='_blank' style='color:#aaa;'>Tasmota %s " D_BY " Theo Arends</a></div>"
  "</div>"
  "</body>"
  "</html>";

const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg("o", tmp, sizeof(tmp));
const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:

enum ButtonTitle {
  BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
  BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_CONSOLE,
  BUTTON_MODULE, BUTTON_WIFI, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, BUTTON_RESTORE };
const char kButtonTitle[] PROGMEM =
  D_RESTART "|" D_RESET_CONFIGURATION "|"
  D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_CONSOLE "|"
  D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION;
const char kButtonAction[] PROGMEM =
  ".|rt|"
  ".|cn|in|up|cs|"
  "md|wi|lg|co|tp|dl|rs";
const char kButtonConfirm[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;

enum CTypes { CT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
const char kContentTypes[] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";

const char kLoggingOptions[] PROGMEM = D_SERIAL_LOG_LEVEL "|" D_WEB_LOG_LEVEL "|" D_MQTT_LOG_LEVEL "|" D_SYS_LOG_LEVEL;
const char kLoggingLevels[] PROGMEM = D_NONE "|" D_ERROR "|" D_INFO "|" D_DEBUG "|" D_MORE_DEBUG;

const char kEmulationOptions[] PROGMEM = D_NONE "|" D_BELKIN_WEMO "|" D_HUE_BRIDGE;

const char kUploadErrors[] PROGMEM =
  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "|" D_UPLOAD_ERR_5 "|" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9
#ifdef USE_RF_FLASH
  "|" D_UPLOAD_ERR_10 "|" D_UPLOAD_ERR_11 "|" D_UPLOAD_ERR_12 "|" D_UPLOAD_ERR_13
#endif
  "|" D_UPLOAD_ERR_14
  ;

const uint16_t DNS_PORT = 53;
enum HttpOptions {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER, HTTP_MANAGER_RESET_ONLY};

DNSServer *DnsServer;
ESP8266WebServer *WebServer;

struct WEB {
  String chunk_buffer = "";                         // Could be max 2 * CHUNKED_BUFFER_SIZE
  bool reset_web_log_flag = false;                  // Reset web console log
  uint8_t state = HTTP_OFF;
  uint8_t upload_error = 0;
  uint8_t upload_file_type;
  uint8_t upload_progress_dot_count;
  uint8_t config_block_count = 0;
  uint8_t config_xor_on = 0;
  uint8_t config_xor_on_set = CONFIG_FILE_XOR;
} Web;

// Helper function to avoid code duplication (saves 4k Flash)
static void WebGetArg(const char* arg, char* out, size_t max)
{
  String s = WebServer->arg(arg);
  strlcpy(out, s.c_str(), max);
//  out[max-1] = '\0';  // Ensure terminating NUL
}

static bool WifiIsInManagerMode(){
  return (HTTP_MANAGER == Web.state || HTTP_MANAGER_RESET_ONLY == Web.state);
}

void ShowWebSource(uint32_t source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SRC: %s from %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource), WebServer->client().remoteIP().toString().c_str());
  }
}

void ExecuteWebCommand(char* svalue, uint32_t source)
{
  ShowWebSource(source);
  last_source = source;
  ExecuteCommand(svalue, SRC_IGNORE);
}

void StartWebserver(int type, IPAddress ipweb)
{
  if (!Settings.web_refresh) { Settings.web_refresh = HTTP_REFRESH_TIME; }
  if (!Web.state) {
    if (!WebServer) {
      WebServer = new ESP8266WebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
      WebServer->on("/", HandleRoot);
      WebServer->onNotFound(HandleNotFound);
      WebServer->on("/up", HandleUpgradeFirmware);
      WebServer->on("/u1", HandleUpgradeFirmwareStart);  // OTA
      WebServer->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);
      WebServer->on("/u2", HTTP_OPTIONS, HandlePreflightRequest);
      WebServer->on("/cs", HTTP_GET, HandleConsole);
      WebServer->on("/cs", HTTP_OPTIONS, HandlePreflightRequest);
      WebServer->on("/cm", HandleHttpCommand);
#ifndef FIRMWARE_MINIMAL
      WebServer->on("/cn", HandleConfiguration);
      WebServer->on("/md", HandleModuleConfiguration);
      WebServer->on("/wi", HandleWifiConfiguration);
      WebServer->on("/lg", HandleLoggingConfiguration);
      WebServer->on("/tp", HandleTemplateConfiguration);
      WebServer->on("/co", HandleOtherConfiguration);
      WebServer->on("/dl", HandleBackupConfiguration);
      WebServer->on("/rs", HandleRestoreConfiguration);
      WebServer->on("/rt", HandleResetConfiguration);
      WebServer->on("/in", HandleInformation);
      XdrvCall(FUNC_WEB_ADD_HANDLER);
      XsnsCall(FUNC_WEB_ADD_HANDLER);
#endif  // Not FIRMWARE_MINIMAL
    }
    Web.reset_web_log_flag = false;

    // Collect User-Agent for Alexa Hue Emulation
    // This is used in xdrv_20_hue.ino in function findEchoGeneration()
    WebServer->collectHeaders(HEADER_KEYS, sizeof(HEADER_KEYS)/sizeof(char*));

    WebServer->begin(); // Web server start
  }
  if (Web.state != type) {
#if LWIP_IPV6
    String ipv6_addr = WifiGetIPv6();
    if(ipv6_addr!="") AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s and IPv6 global address %s "), my_hostname, (Wifi.mdns_begun) ? ".local" : "", ipweb.toString().c_str(),ipv6_addr.c_str());
    else AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"), my_hostname, (Wifi.mdns_begun) ? ".local" : "", ipweb.toString().c_str());
#else
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"), my_hostname, (Wifi.mdns_begun) ? ".local" : "", ipweb.toString().c_str());
#endif // LWIP_IPV6 = 1
    rules_flag.http_init = 1;
  }
  if (type) { Web.state = type; }
}

void StopWebserver(void)
{
  if (Web.state) {
    WebServer->close();
    Web.state = HTTP_OFF;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

void WifiManagerBegin(bool reset_only)
{
  // setup AP
  if (!global_state.wifi_down) {
//    WiFi.mode(WIFI_AP_STA);
    WifiSetMode(WIFI_AP_STA);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
//    WiFi.mode(WIFI_AP);
    WifiSetMode(WIFI_AP);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
  }

  StopWebserver();

  DnsServer = new DNSServer();

  int channel = WIFI_SOFT_AP_CHANNEL;
  if ((channel < 1) || (channel > 13)) { channel = 1; }

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
  // bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
  WiFi.softAP(my_hostname, WIFI_AP_PASSPHRASE, channel, 0);
#else
  // bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
  WiFi.softAP(my_hostname, WIFI_AP_PASSPHRASE, channel, 0, 1);
#endif

  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  StartWebserver((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER), WiFi.softAPIP());
}

void PollDnsWebserver(void)
{
  if (DnsServer) { DnsServer->processNextRequest(); }
  if (WebServer) { WebServer->handleClient(); }
}

/*********************************************************************************************/

bool WebAuthenticate(void)
{
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return WebServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  } else {
    return true;
  }
}

bool HttpCheckPriviledgedAccess(bool autorequestauth = true)
{
  if (HTTP_USER == Web.state) {
    HandleRoot();
    return false;
  }
  if (autorequestauth && !WebAuthenticate()) {
    WebServer->requestAuthentication();
    return false;
  }
  return true;
}

void HttpHeaderCors(void)
{
  if (strlen(SettingsText(SET_CORS))) {
    WebServer->sendHeader(F("Access-Control-Allow-Origin"), SettingsText(SET_CORS));
  }
}

void WSHeaderSend(void)
{
  WebServer->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  WebServer->sendHeader(F("Pragma"), F("no-cache"));
  WebServer->sendHeader(F("Expires"), F("-1"));
  HttpHeaderCors();
}

/**********************************************************************************************
* HTTP Content Page handler
**********************************************************************************************/

void WSSend(int code, int ctype, const String& content)
{
  char ct[25];  // strlen("application/octet-stream") +1 = Longest Content type string
  WebServer->send(code, GetTextIndexed(ct, sizeof(ct), ctype, kContentTypes), content);
}

/**********************************************************************************************
* HTTP Content Chunk handler
**********************************************************************************************/

void WSContentBegin(int code, int ctype)
{
  WebServer->client().flush();
  WSHeaderSend();
#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
  WebServer->sendHeader(F("Accept-Ranges"),F("none"));
  WebServer->sendHeader(F("Transfer-Encoding"),F("chunked"));
#endif
  WebServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  WSSend(code, ctype, "");                        // Signal start of chunked content
  Web.chunk_buffer = "";
}

void _WSContentSend(const String& content)        // Low level sendContent for all core versions
{
  size_t len = content.length();

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
  const char * footer = "\r\n";
  char chunk_size[11];
  sprintf(chunk_size, "%x\r\n", len);
  WebServer->sendContent(String() + chunk_size + content + footer);
#else
  WebServer->sendContent(content);
#endif

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("WSContentSend"));
#endif
  DEBUG_CORE_LOG(PSTR("WEB: Chunk size %d/%d"), len, sizeof(mqtt_data));
}

void WSContentFlush(void)
{
  if (Web.chunk_buffer.length() > 0) {
    _WSContentSend(Web.chunk_buffer);                  // Flush chunk buffer
    Web.chunk_buffer = "";
  }
}

void _WSContentSendBuffer(void)
{
  int len = strlen(mqtt_data);

  if (0 == len) {                                  // No content
    return;
  }
  else if (len == sizeof(mqtt_data)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: Content too large"));
  }
  else if (len < CHUNKED_BUFFER_SIZE) {            // Append chunk buffer with small content
    Web.chunk_buffer += mqtt_data;
    len = Web.chunk_buffer.length();
  }

  if (len >= CHUNKED_BUFFER_SIZE) {                // Either content or chunk buffer is oversize
    WSContentFlush();                              // Send chunk buffer before possible content oversize
  }
  if (strlen(mqtt_data) >= CHUNKED_BUFFER_SIZE) {  // Content is oversize
    _WSContentSend(mqtt_data);                     // Send content
  }
}

void WSContentSend_P(const char* formatP, ...)     // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  int len = vsnprintf_P(mqtt_data, sizeof(mqtt_data), formatP, arg);
  va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(mqtt_data) -1)) {
    mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(PSTR("ERROR: WSContentSend_P size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(mqtt_data), mqtt_data);
  }
#endif

  _WSContentSendBuffer();
}

void WSContentSend_PD(const char* formatP, ...)    // Content send snprintf_P char data checked for decimal separator
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  int len = vsnprintf_P(mqtt_data, sizeof(mqtt_data), formatP, arg);
  va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(mqtt_data) -1)) {
    mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(PSTR("ERROR: WSContentSend_PD size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(mqtt_data), mqtt_data);
  }
#endif

  if (D_DECIMAL_SEPARATOR[0] != '.') {
    for (uint32_t i = 0; i < len; i++) {
      if ('.' == mqtt_data[i]) {
        mqtt_data[i] = D_DECIMAL_SEPARATOR[0];
      }
    }
  }

  _WSContentSendBuffer();
}

void WSContentStart_P(const char* title, bool auth)
{
  if (auth && strlen(SettingsText(SET_WEBPWD)) && !WebServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD))) {
    return WebServer->requestAuthentication();
  }

  WSContentBegin(200, CT_HTML);

  if (title != nullptr) {
    char ctitle[strlen_P(title) +1];
    strcpy_P(ctitle, title);                       // Get title from flash to RAM
    WSContentSend_P(HTTP_HEADER, SettingsText(SET_FRIENDLYNAME1), ctitle);
  }
}

void WSContentStart_P(const char* title)
{
  WSContentStart_P(title, true);
}

void WSContentSendStyle_P(const char* formatP, ...)
{
  if (WifiIsInManagerMode()) {
    if (WifiConfigCounter()) {
      WSContentSend_P(HTTP_SCRIPT_COUNTER);
    }
  }
  WSContentSend_P(HTTP_HEAD_LAST_SCRIPT);

  WSContentSend_P(HTTP_HEAD_STYLE1, WebColor(COL_FORM), WebColor(COL_INPUT), WebColor(COL_INPUT_TEXT), WebColor(COL_INPUT),
                  WebColor(COL_INPUT_TEXT), WebColor(COL_CONSOLE), WebColor(COL_CONSOLE_TEXT), WebColor(COL_BACKGROUND));
  WSContentSend_P(HTTP_HEAD_STYLE2, WebColor(COL_BUTTON), WebColor(COL_BUTTON_TEXT), WebColor(COL_BUTTON_HOVER),
                  WebColor(COL_BUTTON_RESET), WebColor(COL_BUTTON_RESET_HOVER), WebColor(COL_BUTTON_SAVE), WebColor(COL_BUTTON_SAVE_HOVER),
                  WebColor(COL_BUTTON));
  if (formatP != nullptr) {
    // This uses char strings. Be aware of sending %% if % is needed
    va_list arg;
    va_start(arg, formatP);
    int len = vsnprintf_P(mqtt_data, sizeof(mqtt_data), formatP, arg);
    va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(mqtt_data) -1)) {
    mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(PSTR("ERROR: WSContentSendStyle_P size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(mqtt_data), mqtt_data);
  }
#endif

    _WSContentSendBuffer();
  }
  WSContentSend_P(HTTP_HEAD_STYLE3, WebColor(COL_TEXT),
#ifdef FIRMWARE_MINIMAL
    WebColor(COL_TEXT_WARNING),
#endif
    WebColor(COL_TITLE),
    ModuleName().c_str(), SettingsText(SET_FRIENDLYNAME1));
  if (Settings.flag3.gui_hostname_ip) {                // SetOption53 - Show hostanme and IP address in GUI main menu
    bool lip = (static_cast<uint32_t>(WiFi.localIP()) != 0);
    bool sip = (static_cast<uint32_t>(WiFi.softAPIP()) != 0);
    WSContentSend_P(PSTR("<h4>%s%s (%s%s%s)</h4>"),    // tasmota.local (192.168.2.12, 192.168.4.1)
      my_hostname,
      (Wifi.mdns_begun) ? ".local" : "",
      (lip) ? WiFi.localIP().toString().c_str() : "",
      (lip && sip) ? ", " : "",
      (sip) ? WiFi.softAPIP().toString().c_str() : "");
  }
  WSContentSend_P(PSTR("</div>"));
}

void WSContentSendStyle(void)
{
  WSContentSendStyle_P(nullptr);
}

void WSContentButton(uint32_t title_index)
{
  char action[4];
  char title[100];  // Large to accomodate UTF-16 as used by Russian

  if (title_index <= BUTTON_RESET_CONFIGURATION) {
    char confirm[100];
    WSContentSend_P(PSTR("<p><form action='%s' method='get' onsubmit='return confirm(\"%s\");'><button name='%s' class='button bred'>%s</button></form></p>"),
      GetTextIndexed(action, sizeof(action), title_index, kButtonAction),
      GetTextIndexed(confirm, sizeof(confirm), title_index, kButtonConfirm),
      (!title_index) ? "rst" : "non",
      GetTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  } else {
    WSContentSend_P(PSTR("<p><form action='%s' method='get'><button>%s</button></form></p>"),
      GetTextIndexed(action, sizeof(action), title_index, kButtonAction),
      GetTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  }
}

void WSContentSpaceButton(uint32_t title_index)
{
  WSContentSend_P(PSTR("<div></div>"));            // 5px padding
  WSContentButton(title_index);
}

void WSContentSend_THD(const char *types, float f_temperature, float f_humidity)
{
  char parameter[FLOATSZ];
  dtostrfd(f_temperature, Settings.flag2.temperature_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_TEMP, types, parameter, TempUnit());
  dtostrfd(f_humidity, Settings.flag2.humidity_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_HUM, types, parameter);
  dtostrfd(CalcTempHumToDew(f_temperature, f_humidity), Settings.flag2.temperature_resolution, parameter);
  WSContentSend_PD(HTTP_SNS_DEW, types, parameter, TempUnit());
}

void WSContentEnd(void)
{
  WSContentFlush();                                // Flush chunk buffer
  _WSContentSend("");                              // Signal end of chunked content
  WebServer->client().stop();
}

void WSContentStop(void)
{
  if (WifiIsInManagerMode()) {
    if (WifiConfigCounter()) {
      WSContentSend_P(HTTP_COUNTER);
    }
  }
  WSContentSend_P(HTTP_END, my_version);
  WSContentEnd();
}

/*********************************************************************************************/

void WebRestart(uint32_t type)
{
  // type 0 = restart
  // type 1 = restart after config change
  // type 2 = restart after config change with possible ip address change too
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTART);

  bool reset_only = (HTTP_MANAGER_RESET_ONLY == Web.state);

  WSContentStart_P((type) ? S_SAVE_CONFIGURATION : S_RESTART, !reset_only);
  WSContentSend_P(HTTP_SCRIPT_RELOAD);
  WSContentSendStyle();
  if (type) {
    WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br>"));
    if (2 == type) {
      WSContentSend_P(PSTR("<br>" D_TRYING_TO_CONNECT "<br>"));
    }
    WSContentSend_P(PSTR("</div>"));
  }
  WSContentSend_P(HTTP_MSG_RSTRT);
  if (HTTP_MANAGER == Web.state || reset_only) {
    Web.state = HTTP_ADMIN;
  } else {
    WSContentSpaceButton(BUTTON_MAIN);
  }
  WSContentStop();

  ShowWebSource(SRC_WEBGUI);
  restart_flag = 2;
}

/*********************************************************************************************/

void HandleWifiLogin(void)
{
  WSContentStart_P(S_CONFIGURE_WIFI, false);  // false means show page no matter if the client has or has not credentials
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

#ifdef USE_LIGHT
void WebSliderColdWarm(void)
{
  WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
    "a",             // a - Unique HTML id
    "#fff", "#ff0",  // White to Yellow
    1,               // sl1
    153, 500,        // Range color temperature
    LightGetColorTemp(),
    't', 0);         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));
}
#endif  // USE_LIGHT

void HandleRoot(void)
{
  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the page.

  if (WebServer->hasArg("rst")) {
    WebRestart(0);
    return;
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    if (strlen(SettingsText(SET_WEBPWD)) && !(WebServer->hasArg("USER1")) && !(WebServer->hasArg("PASS1")) && HTTP_MANAGER_RESET_ONLY != Web.state) {
      HandleWifiLogin();
    } else {
      if (!strlen(SettingsText(SET_WEBPWD)) || (((WebServer->arg("USER1") == WEB_USERNAME ) && (WebServer->arg("PASS1") == SettingsText(SET_WEBPWD) )) || HTTP_MANAGER_RESET_ONLY == Web.state)) {
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

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_MAIN_MENU);

  char stemp[33];

  WSContentStart_P(S_MAIN_MENU);
#ifdef USE_SCRIPT_WEB_DISPLAY
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings.web_refresh, Settings.web_refresh);
#else
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings.web_refresh);
#endif
  WSContentSend_P(HTTP_SCRIPT_ROOT_PART2);

  WSContentSendStyle();

  WSContentSend_P(PSTR("<div id='l1' name='l1'></div>"));
  if (devices_present) {
#ifdef USE_LIGHT
    if (light_type) {
      uint8_t light_subtype = light_type &7;
      if (!Settings.flag3.pwm_multi_channels) {  // SetOption68 0 - Enable multi-channels PWM instead of Color PWM
        bool split_white = ((LST_RGBW <= light_subtype) && (devices_present > 1));  // Only on RGBW or RGBCW and SetOption37 128

        if ((LST_COLDWARM == light_subtype) || ((LST_RGBCW == light_subtype) && !split_white)) {
          WebSliderColdWarm();
        }

        if (light_subtype > 2) {  // No W or CW
          uint16_t hue;
          uint8_t sat;
          LightGetHSB(&hue, &sat, nullptr);

          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Hue
            "b",             // b - Unique HTML id
            "#800", "#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800",  // Hue colors
            2,               // sl2 - Unique range HTML id - Used as source for Saturation end color
            1, 359,          // Range valid Hue
            hue,
            'h', 0);         // h0 - Value id

          uint8_t dcolor = changeUIntScale(Settings.light_dimmer, 0, 100, 0, 255);
          char scolor[8];
          snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
          uint8_t red, green, blue;
          LightHsToRgb(hue, 255, &red, &green, &blue);
          snprintf_P(stemp, sizeof(stemp), PSTR("#%02X%02X%02X"), red, green, blue);  // Saturation end color

          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Saturation
            "s",             // s - Unique HTML id related to eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';
            scolor, stemp,   // Brightness to max current color
            3,               // sl3 - Unique range HTML id - Not used
            0, 100,          // Range 0 to 100%
            changeUIntScale(sat, 0, 255, 0, 100),
            'n', 0);         // n0 - Value id
        }

        WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Brightness - Black to White
          "c",               // c - Unique HTML id
          "#000", "#fff",    // Black to White
          4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
          Settings.flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100%
          Settings.light_dimmer,
          'd', 0);           // d0 - Value id is related to lc("d0", value) and WebGetArg("d0", tmp, sizeof(tmp));

        if (split_white) {   // SetOption37 128
          if (LST_RGBCW == light_subtype) {
            WebSliderColdWarm();
          }
          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // White brightness - Black to White
            "f",             // f - Unique HTML id
            "#000", "#fff",  // Black to White
            5,               // sl5 - Unique range HTML id - Not used
            Settings.flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100%
            LightGetDimmer(2),
            'w', 0);         // w0 - Value id is related to lc("w0", value) and WebGetArg("w0", tmp, sizeof(tmp));
        }
      } else {  // Settings.flag3.pwm_multi_channels - SetOption68 1 - Enable multi-channels PWM instead of Color PWM
        uint32_t pwm_channels = light_subtype > LST_MAX ? LST_MAX : light_subtype;
        stemp[0] = 'e'; stemp[1] = '0'; stemp[2] = '\0';  // d0
        for (uint32_t i = 0; i < pwm_channels; i++) {
          stemp[1]++;        // e1 to e5 - Make unique ids

          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Channel brightness - Black to White
            stemp,           // e1 to e5 - Unique HTML id
            "#000", "#fff",  // Black to White
            i+1,             // sl1 to sl5 - Unique range HTML id - Not used
            1, 100,          // Range 1 to 100%
            changeUIntScale(Settings.light_color[i], 0, 255, 0, 100),
            'e', i+1);       // e1 to e5 - Value id
        }
      }  // Settings.flag3.pwm_multi_channels
    }
#endif // USE_LIGHT
#ifdef USE_SHUTTER
    if (Settings.flag3.shutter_mode) {  // SetOption80 - Enable shutter support
      for (uint32_t i = 0; i < shutters_present; i++) {
        WSContentSend_P(HTTP_MSG_SLIDER_SHUTTER, Settings.shutter_position[i], i+1);
      }
    }
#endif  // USE_SHUTTER
    WSContentSend_P(HTTP_TABLE100);
    WSContentSend_P(PSTR("<tr>"));
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) {
      WSContentSend_P(HTTP_DEVICE_CONTROL, 36, 1,
        (strlen(SettingsText(SET_BUTTON1))) ? SettingsText(SET_BUTTON1) : D_BUTTON_TOGGLE,
        "");
      for (uint32_t i = 0; i < MaxFanspeed(); i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i);
        WSContentSend_P(HTTP_DEVICE_CONTROL, 16, i +2,
          (strlen(SettingsText(SET_BUTTON2 + i))) ? SettingsText(SET_BUTTON2 + i) : stemp,
          "");
      }
    } else {
#endif  // USE_SONOFF_IFAN
      for (uint32_t idx = 1; idx <= devices_present; idx++) {
        bool set_button = ((idx <= MAX_BUTTON_TEXT) && strlen(SettingsText(SET_BUTTON1 + idx -1)));
#ifdef USE_SHUTTER
        int32_t ShutterWebButton;
        if (ShutterWebButton = IsShutterWebButton(idx)) {
          WSContentSend_P(HTTP_DEVICE_CONTROL, 100 / devices_present, idx,
            (set_button) ? SettingsText(SET_BUTTON1 + idx -1) : ((Settings.shutter_options[abs(ShutterWebButton)-1] & 2) /* is locked */ ? "-" : ((Settings.shutter_options[abs(ShutterWebButton)-1] & 8) /* invert web buttons */ ? ((ShutterWebButton>0) ? "▼" : "▲") : ((ShutterWebButton>0) ? "▲" : "▼"))),
            "");
          continue;
        }
#endif  // USE_SHUTTER
        snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
        WSContentSend_P(HTTP_DEVICE_CONTROL, 100 / devices_present, idx,
          (set_button) ? SettingsText(SET_BUTTON1 + idx -1) : (devices_present < 5) ? D_BUTTON_TOGGLE : "",
          (set_button) ? "" : (devices_present > 1) ? stemp : "");
      }
#ifdef USE_SONOFF_IFAN
    }
#endif  // USE_SONOFF_IFAN
    WSContentSend_P(PSTR("</tr></table>"));
  }
#ifdef USE_SONOFF_RF
  if (SONOFF_BRIDGE == my_module_type) {
    WSContentSend_P(HTTP_TABLE100);
    WSContentSend_P(PSTR("<tr>"));
    uint32_t idx = 0;
    for (uint32_t i = 0; i < 4; i++) {
      if (idx > 0) { WSContentSend_P(PSTR("</tr><tr>")); }
      for (uint32_t j = 0; j < 4; j++) {
        idx++;
        snprintf_P(stemp, sizeof(stemp), PSTR("%d"), idx);
        WSContentSend_P(PSTR("<td style='width:25%%'><button onclick='la(\"&k=%d\");'>%s</button></td>"), idx,  // &k is related to WebGetArg("k", tmp, sizeof(tmp));
          (strlen(SettingsText(SET_BUTTON1 + idx -1))) ? SettingsText(SET_BUTTON1 + idx -1) : stemp);
      }
    }
    WSContentSend_P(PSTR("</tr></table>"));
  }
#endif  // USE_SONOFF_RF

#ifndef FIRMWARE_MINIMAL
  XdrvCall(FUNC_WEB_ADD_MAIN_BUTTON);
  XsnsCall(FUNC_WEB_ADD_MAIN_BUTTON);
#endif  // Not FIRMWARE_MINIMAL

  if (HTTP_ADMIN == Web.state) {
#ifdef FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_FIRMWARE_UPGRADE);
#else
    WSContentSpaceButton(BUTTON_CONFIGURATION);
    WSContentButton(BUTTON_INFORMATION);
    WSContentButton(BUTTON_FIRMWARE_UPGRADE);
#endif  // Not FIRMWARE_MINIMAL
    WSContentButton(BUTTON_CONSOLE);
    WSContentButton(BUTTON_RESTART);
  }
  WSContentStop();
}

bool HandleRootStatusRefresh(void)
{
  if (!WebAuthenticate()) {
    WebServer->requestAuthentication();
    return true;
  }

  if (!WebServer->hasArg("m")) {     // Status refresh requested
    return false;
  }

  #ifdef USE_SCRIPT_WEB_DISPLAY
    Script_Check_HTML_Setvars();
  #endif

  char tmp[8];                       // WebGetArg numbers only
  char svalue[32];                   // Command and number parameter
  char webindex[5];                  // WebGetArg name

  WebGetArg("o", tmp, sizeof(tmp));  // 1 - 16 Device number for button Toggle or Fanspeed
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
        snprintf_P(svalue, sizeof(svalue), PSTR("ShutterPosition%d %s"), abs(ShutterWebButton), (ShutterWebButton>0) ? PSTR(D_CMND_SHUTTER_TOGGLEUP) : PSTR(D_CMND_SHUTTER_TOGGLEDOWN));
        ExecuteWebCommand(svalue, SRC_WEBGUI);
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
  WebGetArg("d0", tmp, sizeof(tmp));  // 0 - 100 Dimmer value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_DIMMER " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("w0", tmp, sizeof(tmp));  // 0 - 100 White value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_WHITE " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  uint32_t light_device = LightDevice();  // Channel number offset
  uint32_t pwm_channels = (light_type & 7) > LST_MAX ? LST_MAX : (light_type & 7);
  for (uint32_t j = 0; j < pwm_channels; j++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("e%d"), j +1);
    WebGetArg(webindex, tmp, sizeof(tmp));  // 0 - 100 percent
    if (strlen(tmp)) {
      snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d %s"), j +light_device, tmp);
      ExecuteWebCommand(svalue, SRC_WEBGUI);
    }
  }
  WebGetArg("t0", tmp, sizeof(tmp));  // 153 - 500 Color temperature
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_COLORTEMPERATURE " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("h0", tmp, sizeof(tmp));  // 0 - 359 Hue value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_HSBCOLOR  "1 %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("n0", tmp, sizeof(tmp));  // 0 - 99 Saturation value
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_HSBCOLOR  "2 %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
#endif  // USE_LIGHT
#ifdef USE_SHUTTER
  for (uint32_t j = 1; j <= shutters_present; j++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("u%d"), j);
    WebGetArg(webindex, tmp, sizeof(tmp));  // 0 - 100 percent
    if (strlen(tmp)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("ShutterPosition%d %s"), j, tmp);
      ExecuteWebCommand(svalue, SRC_WEBGUI);
    }
  }
#endif  // USE_SHUTTER
#ifdef USE_SONOFF_RF
  WebGetArg("k", tmp, sizeof(tmp));  // 1 - 16 Pre defined RF keys
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_RFKEY "%s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
#endif  // USE_SONOFF_RF
  WSContentBegin(200, CT_HTML);
  WSContentSend_P(PSTR("{t}"));
  XsnsCall(FUNC_WEB_SENSOR);
#ifdef USE_SCRIPT_WEB_DISPLAY
  XdrvCall(FUNC_WEB_SENSOR);
#endif

  WSContentSend_P(PSTR("</table>"));

  if (devices_present) {
    WSContentSend_P(PSTR("{t}<tr>"));
    uint32_t fsize = (devices_present < 5) ? 70 - (devices_present * 8) : 32;
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) {
      WSContentSend_P(HTTP_DEVICE_STATE, 36, (bitRead(power, 0)) ? "bold" : "normal", 54, GetStateText(bitRead(power, 0)));
      uint32_t fanspeed = GetFanspeed();
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fanspeed);
      WSContentSend_P(HTTP_DEVICE_STATE, 64, (fanspeed) ? "bold" : "normal", 54, (fanspeed) ? svalue : GetStateText(0));
    } else {
#endif  // USE_SONOFF_IFAN
      for (uint32_t idx = 1; idx <= devices_present; idx++) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), bitRead(power, idx -1));
        WSContentSend_P(HTTP_DEVICE_STATE, 100 / devices_present, (bitRead(power, idx -1)) ? "bold" : "normal", fsize, (devices_present < 5) ? GetStateText(bitRead(power, idx -1)) : svalue);
      }
#ifdef USE_SONOFF_IFAN
    }
#endif  // USE_SONOFF_IFAN
    WSContentSend_P(PSTR("</tr></table>"));
  }
  WSContentEnd();

  return true;
}

#ifdef USE_SHUTTER
int32_t IsShutterWebButton(uint32_t idx) {
  /* 0: Not a shutter, 1..4: shutter up idx, -1..-4: shutter down idx */
  int32_t ShutterWebButton = 0;
  if (Settings.flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
      if (Settings.shutter_startrelay[i] && ((Settings.shutter_startrelay[i] == idx) || (Settings.shutter_startrelay[i] == (idx-1)))) {
        ShutterWebButton = (Settings.shutter_startrelay[i] == idx) ? (i+1): (-1-i);
        break;
      }
    }
  }
  return ShutterWebButton;
}
#endif // USE_SHUTTER

/*-------------------------------------------------------------------------------------------*/

#ifndef FIRMWARE_MINIMAL

void HandleConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURATION);

  WSContentStart_P(S_CONFIGURATION);
  WSContentSendStyle();

  WSContentButton(BUTTON_MODULE);
  WSContentButton(BUTTON_WIFI);

  XdrvCall(FUNC_WEB_ADD_BUTTON);
  XsnsCall(FUNC_WEB_ADD_BUTTON);

  WSContentButton(BUTTON_LOGGING);
  WSContentButton(BUTTON_OTHER);
  WSContentButton(BUTTON_TEMPLATE);

  WSContentSpaceButton(BUTTON_RESET_CONFIGURATION);
  WSContentButton(BUTTON_BACKUP);
  WSContentButton(BUTTON_RESTORE);

  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

/*-------------------------------------------------------------------------------------------*/

void HandleTemplateConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (WebServer->hasArg("save")) {
    TemplateSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[30];                                           // Template number and Sensor name

  if (WebServer->hasArg("m")) {
    WSContentBegin(200, CT_PLAIN);
    for (uint32_t i = 0; i < sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
      uint32_t midx = pgm_read_byte(kModuleNiceList + i);
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, midx, AnyModuleName(midx).c_str(), midx +1);
    }
    WSContentEnd();
    return;
  }

  WebGetArg("t", stemp, sizeof(stemp));                     // 0 - 69 Template number
  if (strlen(stemp)) {
    uint32_t module = atoi(stemp);
    uint32_t module_save = Settings.module;
    Settings.module = module;
    myio cmodule;
    ModuleGpios(&cmodule);
    gpio_flag flag = ModuleFlag();
    Settings.module = module_save;

    WSContentBegin(200, CT_PLAIN);
    WSContentSend_P(PSTR("%s}1"), AnyModuleName(module).c_str());  // NAME: Generic
    for (uint32_t i = 0; i < sizeof(kGpioNiceList); i++) {   // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...
      if (1 == i) {
        WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, 255, D_SENSOR_USER, 255);  // }2'255'>User (255)}3
      }
      uint32_t midx = pgm_read_byte(kGpioNiceList + i);
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, midx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames), midx);
    }
    WSContentSend_P(PSTR("}1"));                                   // Field separator

    for (uint32_t i = 0; i < ADC0_END; i++) {                // FLAG: }2'0'>None (0)}3}2'17'>Analog (17)}3...
      if (1 == i) {
        WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, ADC0_USER, D_SENSOR_USER, ADC0_USER);  // }2'15'>User (15)}3
      }
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, i, GetTextIndexed(stemp, sizeof(stemp), i, kAdc0Names), i);
    }
    WSContentSend_P(PSTR("}1"));                                   // Field separator

    for (uint32_t i = 0; i < sizeof(cmodule); i++) {         // 17,148,29,149,7,255,255,255,138,255,139,255,255
      if ((i < 6) || ((i > 8) && (i != 11))) {              // Ignore flash pins GPIO06, 7, 8 and 11
        WSContentSend_P(PSTR("%s%d"), (i>0)?",":"", cmodule.io[i]);
      }
    }
    WSContentSend_P(PSTR("}1%d}1%d"), flag, Settings.user_template_base);  // FLAG: 1  BASE: 17
    WSContentEnd();
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_TEMPLATE);

  WSContentStart_P(S_CONFIGURE_TEMPLATE);
  WSContentSend_P(HTTP_SCRIPT_MODULE_TEMPLATE);
  WSContentSend_P(HTTP_SCRIPT_TEMPLATE);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_TEMPLATE);
  WSContentSend_P(HTTP_TABLE100);
  WSContentSend_P(PSTR("<tr><td><b>" D_TEMPLATE_NAME "</b></td><td style='width:200px'><input id='s1' placeholder='" D_TEMPLATE_NAME "'></td></tr>"
                       "<tr><td><b>" D_BASE_TYPE "</b></td><td><select id='g99' onchange='st(this.value)'></select></td></tr>"
                       "</table>"
                       "<hr/>"));
  WSContentSend_P(HTTP_TABLE100);
  for (uint32_t i = 0; i < 17; i++) {
    if ((i < 6) || ((i > 8) && (i != 11))) {                // Ignore flash pins GPIO06, 7, 8 and 11
      WSContentSend_P(PSTR("<tr><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d'></select></td></tr>"),
        ((9==i)||(10==i)) ? WebColor(COL_TEXT_WARNING) : WebColor(COL_TEXT), i, (0==i) ? " style='width:200px'" : "", i);
    }
  }
  WSContentSend_P(PSTR("<tr><td><b><font color='#%06x'>" D_ADC "0</font></b></td><td><select id='g17'></select></td></tr>"), WebColor(COL_TEXT));
  WSContentSend_P(PSTR("</table>"));
  gpio_flag flag = ModuleFlag();
  if (flag.data > ADC0_USER) {
    WSContentSend_P(HTTP_FORM_TEMPLATE_FLAG);
  }
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void TemplateSaveSettings(void)
{
  char tmp[TOPSZ];                                          // WebGetArg NAME and GPIO/BASE/FLAG byte value
  char webindex[5];                                         // WebGetArg name
  char svalue[200];                                         // Template command string

  WebGetArg("s1", tmp, sizeof(tmp));                        // NAME
  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_TEMPLATE " {\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), tmp);

  uint32_t j = 0;
  for (uint32_t i = 0; i < sizeof(Settings.user_template.gp); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), j);
    WebGetArg(webindex, tmp, sizeof(tmp));                  // GPIO
    uint8_t gpio = atoi(tmp);
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%s%d"), svalue, (i>0)?",":"", gpio);
    j++;
  }

  WebGetArg("g17", tmp, sizeof(tmp));                       // FLAG - ADC0
  uint32_t flag = atoi(tmp);
  for (uint32_t i = 0; i < GPIO_FLAG_USED; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
    uint32_t state = WebServer->hasArg(webindex) << i +4;   // FLAG
    flag += state;
  }
  WebGetArg("g99", tmp, sizeof(tmp));                       // BASE
  uint32_t base = atoi(tmp) +1;

  snprintf_P(svalue, sizeof(svalue), PSTR("%s],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), svalue, flag, base);
  ExecuteWebCommand(svalue, SRC_WEBGUI);
}

/*-------------------------------------------------------------------------------------------*/

void HandleModuleConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (WebServer->hasArg("save")) {
    ModuleSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[30];  // Sensor name
  uint32_t midx;
  myio cmodule;
  ModuleGpios(&cmodule);

  if (WebServer->hasArg("m")) {
    WSContentBegin(200, CT_PLAIN);
    uint32_t vidx = 0;
    for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
      if (0 == i) {
        midx = USER_MODULE;
        vidx = 0;
      } else {
        midx = pgm_read_byte(kModuleNiceList + i -1);
        vidx = midx +1;
      }
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, midx, AnyModuleName(midx).c_str(), vidx);
    }
    WSContentEnd();
    return;
  }

  if (WebServer->hasArg("g")) {
    WSContentBegin(200, CT_PLAIN);
    for (uint32_t j = 0; j < sizeof(kGpioNiceList); j++) {
      midx = pgm_read_byte(kGpioNiceList + j);
      if (!GetUsedInModule(midx, cmodule.io)) {
        WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, midx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames), midx);
      }
    }
    WSContentEnd();
    return;
  }

#ifndef USE_ADC_VCC
  if (WebServer->hasArg("a")) {
    WSContentBegin(200, CT_PLAIN);
    for (uint32_t j = 0; j < ADC0_END; j++) {
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE, j, GetTextIndexed(stemp, sizeof(stemp), j, kAdc0Names), j);
    }
    WSContentEnd();
    return;
  }
#endif  // USE_ADC_VCC

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MODULE);

  WSContentStart_P(S_CONFIGURE_MODULE);
  WSContentSend_P(HTTP_SCRIPT_MODULE_TEMPLATE);
  WSContentSend_P(HTTP_SCRIPT_MODULE1, Settings.module);
  for (uint32_t i = 0; i < sizeof(cmodule); i++) {
    if (ValidGPIO(i, cmodule.io[i])) {
      WSContentSend_P(PSTR("sk(%d,%d);"), my_module.io[i], i);  // g0 - g16
    }
  }
  WSContentSend_P(HTTP_SCRIPT_MODULE2, Settings.my_adc0);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_MODULE, AnyModuleName(MODULE).c_str());
  for (uint32_t i = 0; i < sizeof(cmodule); i++) {
    if (ValidGPIO(i, cmodule.io[i])) {
      snprintf_P(stemp, 3, PINS_WEMOS +i*2);
      char sesp8285[40];
      snprintf_P(sesp8285, sizeof(sesp8285), PSTR("<font color='#%06x'>ESP8285</font>"), WebColor(COL_TEXT_WARNING));
      WSContentSend_P(PSTR("<tr><td style='width:190px'>%s <b>" D_GPIO "%d</b> %s</td><td style='width:176px'><select id='g%d'></select></td></tr>"),
        (WEMOS==my_module_type)?stemp:"", i, (0==i)? D_SENSOR_BUTTON "1":(1==i)? D_SERIAL_OUT :(3==i)? D_SERIAL_IN :((9==i)||(10==i))? sesp8285 :(12==i)? D_SENSOR_RELAY "1":(13==i)? D_SENSOR_LED "1i":(14==i)? D_SENSOR :"", i);
    }
  }
#ifndef USE_ADC_VCC
  if (ValidAdc()) {
    WSContentSend_P(PSTR("<tr><td>%s <b>" D_ADC "0</b></td><td style='width:176px'><select id='g17'></select></td></tr>"), (WEMOS==my_module_type)?"A0":"");
  }
#endif  // USE_ADC_VCC
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void ModuleSaveSettings(void)
{
  char tmp[8];         // WebGetArg numbers only
  char webindex[5];    // WebGetArg name

  WebGetArg("g99", tmp, sizeof(tmp));
  uint32_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
  Settings.last_module = Settings.module;
  Settings.module = new_module;
  SetModuleType();
  myio cmodule;
  ModuleGpios(&cmodule);
  String gpios = "";
  for (uint32_t i = 0; i < sizeof(cmodule); i++) {
    if (Settings.last_module != new_module) {
      Settings.my_gp.io[i] = GPIO_NONE;
    } else {
      if (ValidGPIO(i, cmodule.io[i])) {
        snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), i);
        WebGetArg(webindex, tmp, sizeof(tmp));
        Settings.my_gp.io[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
        gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(Settings.my_gp.io[i]);
      }
    }
  }
#ifndef USE_ADC_VCC
  WebGetArg("g17", tmp, sizeof(tmp));
  Settings.my_adc0 = (!strlen(tmp)) ? 0 : atoi(tmp);
  gpios += F(", " D_ADC "0 "); gpios += String(Settings.my_adc0);
#endif  // USE_ADC_VCC

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), ModuleName().c_str(), gpios.c_str());
}

/*-------------------------------------------------------------------------------------------*/

const char kUnescapeCode[] = "&><\"\'";
const char kEscapeCode[] PROGMEM = "&amp;|&gt;|&lt;|&quot;|&apos;";

String HtmlEscape(const String unescaped) {
  char escaped[10];
  size_t ulen = unescaped.length();
  String result = "";
  for (size_t i = 0; i < ulen; i++) {
    char c = unescaped[i];
    char *p = strchr(kUnescapeCode, c);
    if (p != nullptr) {
      result += GetTextIndexed(escaped, sizeof(escaped), p - kUnescapeCode, kEscapeCode);
    } else {
      result += c;
    }
  }
  return result;
}

// Indexed by enum wl_enc_type in file wl_definitions.h starting from -1
const char kEncryptionType[] PROGMEM = "|||" D_WPA_PSK "||" D_WPA2_PSK "|" D_WEP "||" D_NONE "|" D_AUTO;

void HandleWifiConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_WIFI);

  if (WebServer->hasArg("save") && HTTP_MANAGER_RESET_ONLY != Web.state) {
    WifiSaveSettings();
    WebRestart(2);
    return;
  }

  WSContentStart_P(S_CONFIGURE_WIFI, !WifiIsInManagerMode());
  WSContentSend_P(HTTP_SCRIPT_WIFI);
  WSContentSendStyle();

  if (HTTP_MANAGER_RESET_ONLY != Web.state) {
    if (WebServer->hasArg("scan")) {
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
      int n = WiFi.scanNetworks();
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

      if (0 == n) {
        AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, S_NO_NETWORKS_FOUND);
        WSContentSend_P(S_NO_NETWORKS_FOUND);
        WSContentSend_P(PSTR(". " D_REFRESH_TO_SCAN_AGAIN "."));
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

        // remove duplicates ( must be RSSI sorted )
        String cssid;
        for (uint32_t i = 0; i < n; i++) {
          if (-1 == indices[i]) { continue; }
          cssid = WiFi.SSID(indices[i]);
          uint32_t cschn = WiFi.channel(indices[i]);
          for (uint32_t j = i + 1; j < n; j++) {
            if ((cssid == WiFi.SSID(indices[j])) && (cschn == WiFi.channel(indices[j]))) {
              DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              indices[j] = -1;  // set dup aps to index -1
            }
          }
        }

        //display networks in page
        for (uint32_t i = 0; i < n; i++) {
          if (-1 == indices[i]) { continue; }  // skip dups
          int32_t rssi = WiFi.RSSI(indices[i]);
          DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"),
            WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), rssi);
          int quality = WifiGetRssiAsQuality(rssi);
          int auth = WiFi.encryptionType(indices[i]);
          char encryption[20];
          WSContentSend_P(PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%s %d%% (%d dBm)</span></div>"),
            HtmlEscape(WiFi.SSID(indices[i])).c_str(),
            WiFi.channel(indices[i]),
            GetTextIndexed(encryption, sizeof(encryption), auth +1, kEncryptionType),
            quality, rssi
          );
          delay(0);

        }
        WSContentSend_P(PSTR("<br>"));
      }
    } else {
      WSContentSend_P(PSTR("<div><a href='/wi?scan='>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br>"));
    }

    // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
    WSContentSend_P(HTTP_FORM_WIFI, SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), WIFI_HOSTNAME, WIFI_HOSTNAME, SettingsText(SET_HOSTNAME), SettingsText(SET_CORS));
    WSContentSend_P(HTTP_FORM_END);
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_RESTORE);
    WSContentButton(BUTTON_RESET_CONFIGURATION);
#endif  // FIRMWARE_MINIMAL
    WSContentSpaceButton(BUTTON_RESTART);
  } else {
    WSContentSpaceButton(BUTTON_CONFIGURATION);
  }
  WSContentStop();
}

void WifiSaveSettings(void)
{
  char tmp[TOPSZ];  // Max length is currently 150

  WebGetArg("h", tmp, sizeof(tmp));
  SettingsUpdateText(SET_HOSTNAME, (!strlen(tmp)) ? WIFI_HOSTNAME : tmp);
  if (strstr(SettingsText(SET_HOSTNAME), "%") != nullptr) {
    SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
  }
  WebGetArg("c", tmp, sizeof(tmp));
  SettingsUpdateText(SET_CORS, (!strlen(tmp)) ? CORS_DOMAIN : tmp);
  WebGetArg("s1", tmp, sizeof(tmp));
  SettingsUpdateText(SET_STASSID1, (!strlen(tmp)) ? STA_SSID1 : tmp);
  WebGetArg("s2", tmp, sizeof(tmp));
  SettingsUpdateText(SET_STASSID2, (!strlen(tmp)) ? STA_SSID2 : tmp);
  WebGetArg("p1", tmp, sizeof(tmp));
  SettingsUpdateText(SET_STAPWD1, (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? SettingsText(SET_STAPWD1) : tmp);
  WebGetArg("p2", tmp, sizeof(tmp));
  SettingsUpdateText(SET_STAPWD2, (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? SettingsText(SET_STAPWD2) : tmp);
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_SSID "2 %s, " D_CMND_CORS " %s"),
    SettingsText(SET_HOSTNAME), SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), SettingsText(SET_CORS));
}

/*-------------------------------------------------------------------------------------------*/

void HandleLoggingConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_LOGGING);

  if (WebServer->hasArg("save")) {
    LoggingSaveSettings();
    HandleConfiguration();
    return;
  }

  WSContentStart_P(S_CONFIGURE_LOGGING);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_LOG1);
  char stemp1[45];
  char stemp2[32];
  uint8_t dlevel[4] = { LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_NONE, LOG_LEVEL_NONE };
  for (uint32_t idx = 0; idx < 4; idx++) {
    if ((2==idx) && !Settings.flag.mqtt_enabled) { continue; }  // SetOption3 - Enable MQTT
    uint32_t llevel = (0==idx)?Settings.seriallog_level:(1==idx)?Settings.weblog_level:(2==idx)?Settings.mqttlog_level:Settings.syslog_level;
    WSContentSend_P(PSTR("<p><b>%s</b> (%s)<br><select id='l%d'>"),
      GetTextIndexed(stemp1, sizeof(stemp1), idx, kLoggingOptions),
      GetTextIndexed(stemp2, sizeof(stemp2), dlevel[idx], kLoggingLevels),
      idx);
    for (uint32_t i = LOG_LEVEL_NONE; i <= LOG_LEVEL_DEBUG_MORE; i++) {
      WSContentSend_P(PSTR("<option%s value='%d'>%d %s</option>"),
        (i == llevel) ? " selected" : "", i, i,
        GetTextIndexed(stemp1, sizeof(stemp1), i, kLoggingLevels));
    }
    WSContentSend_P(PSTR("</select></p>"));
  }
  WSContentSend_P(HTTP_FORM_LOG2, SettingsText(SET_SYSLOG_HOST), Settings.syslog_port, Settings.tele_period);
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void LoggingSaveSettings(void)
{
  char tmp[TOPSZ];  // Max length is currently 33

  WebGetArg("l0", tmp, sizeof(tmp));
  SetSeriallog((!strlen(tmp)) ? SERIAL_LOG_LEVEL : atoi(tmp));
  WebGetArg("l1", tmp, sizeof(tmp));
  Settings.weblog_level = (!strlen(tmp)) ? WEB_LOG_LEVEL : atoi(tmp);
  WebGetArg("l2", tmp, sizeof(tmp));
  Settings.mqttlog_level = (!strlen(tmp)) ? MQTT_LOG_LEVEL : atoi(tmp);
  WebGetArg("l3", tmp, sizeof(tmp));
  SetSyslog((!strlen(tmp)) ? SYS_LOG_LEVEL : atoi(tmp));
  WebGetArg("lh", tmp, sizeof(tmp));
  SettingsUpdateText(SET_SYSLOG_HOST, (!strlen(tmp)) ? SYS_LOG_HOST : tmp);
  WebGetArg("lp", tmp, sizeof(tmp));
  Settings.syslog_port = (!strlen(tmp)) ? SYS_LOG_PORT : atoi(tmp);
  WebGetArg("lt", tmp, sizeof(tmp));
  Settings.tele_period = (!strlen(tmp)) ? TELE_PERIOD : atoi(tmp);
  if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) {
    Settings.tele_period = 10;   // Do not allow periods < 10 seconds
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_MQTTLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
    Settings.seriallog_level, Settings.weblog_level, Settings.mqttlog_level, Settings.syslog_level, SettingsText(SET_SYSLOG_HOST), Settings.syslog_port, Settings.tele_period);
}

/*-------------------------------------------------------------------------------------------*/

void HandleOtherConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_OTHER);

  if (WebServer->hasArg("save")) {
    OtherSaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(S_CONFIGURE_OTHER);
  WSContentSendStyle();

  TemplateJson();
  char stemp[strlen(mqtt_data) +1];
  strlcpy(stemp, mqtt_data, sizeof(stemp));  // Get JSON template
  WSContentSend_P(HTTP_FORM_OTHER, stemp, (USER_MODULE == Settings.module) ? " checked disabled" : "", (Settings.flag.mqtt_enabled) ? " checked" : "");  // SetOption3 - Enable MQTT

  uint32_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!devices_present) ? 1 : devices_present;
#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
  for (uint32_t i = 0; i < maxfn; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i +1);
    WSContentSend_P(PSTR("<b>" D_FRIENDLY_NAME " %d</b> (" FRIENDLY_NAME "%s)<br><input id='a%d' placeholder='" FRIENDLY_NAME "%s' value='%s'><p></p>"),
      i +1,
      (i) ? stemp : "",
      i,
      (i) ? stemp : "",
      SettingsText(SET_FRIENDLYNAME1 + i));
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
      WSContentSend_P(PSTR("<input id='r%d' name='b2' type='radio' value='%d'%s><b>%s</b> %s<br>"),  // Different id only used for labels
        i, i,
        (i == Settings.flag2.emulation) ? " checked" : "",
        GetTextIndexed(stemp, sizeof(stemp), i, kEmulationOptions),
        (i == EMUL_NONE) ? "" : (i == EMUL_WEMO) ? D_SINGLE_DEVICE : D_MULTI_DEVICE);
    }
  }
  WSContentSend_P(PSTR("</p></fieldset>"));
#endif  // USE_EMULATION_WEMO || USE_EMULATION_HUE
#endif  // USE_EMULATION

  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void OtherSaveSettings(void)
{
  char tmp[TOPSZ];
  char webindex[5];
  char friendlyname[TOPSZ];
  char message[LOGSZ];

  WebGetArg("wp", tmp, sizeof(tmp));
  SettingsUpdateText(SET_WEBPWD, (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? SettingsText(SET_WEBPWD) : tmp);
  Settings.flag.mqtt_enabled = WebServer->hasArg("b1");  // SetOption3 - Enable MQTT
#ifdef USE_EMULATION
  UdpDisconnect();
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
  WebGetArg("b2", tmp, sizeof(tmp));
  Settings.flag2.emulation = (!strlen(tmp)) ? 0 : atoi(tmp);
#endif  // USE_EMULATION_WEMO || USE_EMULATION_HUE
#endif  // USE_EMULATION

  snprintf_P(message, sizeof(message), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_CMND_EMULATION " %d, " D_CMND_FRIENDLYNAME), GetStateText(Settings.flag.mqtt_enabled), Settings.flag2.emulation);
  for (uint32_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("a%d"), i);
    WebGetArg(webindex, tmp, sizeof(tmp));
    snprintf_P(friendlyname, sizeof(friendlyname), PSTR(FRIENDLY_NAME"%d"), i +1);
    SettingsUpdateText(SET_FRIENDLYNAME1 +i, (!strlen(tmp)) ? (i) ? friendlyname : FRIENDLY_NAME : tmp);
    snprintf_P(message, sizeof(message), PSTR("%s%s %s"), message, (i) ? "," : "", SettingsText(SET_FRIENDLYNAME1 +i));
  }
  AddLog_P(LOG_LEVEL_INFO, message);

/*
  // This sometimes provides intermittent watchdog
  bool template_activate = WebServer->hasArg("t2");  // Try this to tackle intermittent watchdog after execution of Template command
  WebGetArg("t1", tmp, sizeof(tmp));
  if (strlen(tmp)) {  // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255}
    char svalue[128];
    snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_TEMPLATE " %s"), tmp);
    ExecuteWebCommand(svalue, SRC_WEBGUI);

    if (template_activate) {
      snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_MODULE " 0"));
      ExecuteWebCommand(svalue, SRC_WEBGUI);
    }
  }
  // Try async execution of commands
*/
  WebGetArg("t1", tmp, sizeof(tmp));
  if (strlen(tmp)) {  // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255}
    snprintf_P(message, sizeof(message), PSTR(D_CMND_BACKLOG " " D_CMND_TEMPLATE " %s%s"), tmp, (WebServer->hasArg("t2")) ? "; " D_CMND_MODULE " 0" : "");
    ExecuteWebCommand(message, SRC_WEBGUI);
  }
}

/*-------------------------------------------------------------------------------------------*/

void HandleBackupConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  if (!SettingsBufferAlloc()) { return; }

  WiFiClient myClient = WebServer->client();
  WebServer->setContentLength(sizeof(Settings));

  char attachment[TOPSZ];

//  char friendlyname[TOPSZ];
//  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), NoAlNumToUnderscore(friendlyname, SettingsText(SET_FRIENDLYNAME1)), my_version);

  char hostname[sizeof(my_hostname)];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), NoAlNumToUnderscore(hostname, my_hostname), my_version);

  WebServer->sendHeader(F("Content-Disposition"), attachment);

  WSSend(200, CT_STREAM, "");

  uint32_t cfg_crc32 = Settings.cfg_crc32;
  Settings.cfg_crc32 = GetSettingsCrc32();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

  memcpy(settings_buffer, &Settings, sizeof(Settings));
  if (Web.config_xor_on_set) {
    for (uint32_t i = 2; i < sizeof(Settings); i++) {
      settings_buffer[i] ^= (Web.config_xor_on_set +i);
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

  Settings.cfg_crc32 = cfg_crc32;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed
}

/*-------------------------------------------------------------------------------------------*/

void HandleResetConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESET_CONFIGURATION);

  WSContentStart_P(S_RESET_CONFIGURATION, !WifiIsInManagerMode());
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>"));
  WSContentSend_P(HTTP_MSG_RSTRT);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  char command[CMDSZ];
  snprintf_P(command, sizeof(command), PSTR(D_CMND_RESET " 1"));
  ExecuteWebCommand(command, SRC_WEBGUI);
}

void HandleRestoreConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_RESTORE_CONFIGURATION);

  WSContentStart_P(S_RESTORE_CONFIGURATION);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_RST);
  WSContentSend_P(HTTP_FORM_RST_UPG, D_RESTORE);
  if (WifiIsInManagerMode()) {
    WSContentSpaceButton(BUTTON_MAIN);
  } else {
    WSContentSpaceButton(BUTTON_CONFIGURATION);
  }
  WSContentStop();

  Web.upload_error = 0;
  Web.upload_file_type = UPL_SETTINGS;
}

/*-------------------------------------------------------------------------------------------*/

void HandleInformation(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_INFORMATION);

  char stopic[TOPSZ];

  int freeMem = ESP.getFreeHeap();

  WSContentStart_P(S_INFORMATION);
  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  WSContentSend_P(HTTP_SCRIPT_INFO_BEGIN);
  WSContentSend_P(PSTR("<table style='width:100%%'><tr><th>"));
  WSContentSend_P(PSTR(D_PROGRAM_VERSION "}2%s%s"), my_version, my_image);
  WSContentSend_P(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), GetBuildDateAndTime().c_str());
  WSContentSend_P(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_ESP8266_RELEASE "/%s"), ESP.getSdkVersion());
  WSContentSend_P(PSTR("}1" D_UPTIME "}2%s"), GetUptime().c_str());
  WSContentSend_P(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d at 0x%X"), Settings.save_flag, GetSettingsAddress());
  WSContentSend_P(PSTR("}1" D_BOOT_COUNT "}2%d"), Settings.bootcount);
  WSContentSend_P(PSTR("}1" D_RESTART_REASON "}2%s"), GetResetReason().c_str());
  uint32_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;
#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
  for (uint32_t i = 0; i < maxfn; i++) {
    WSContentSend_P(PSTR("}1" D_FRIENDLY_NAME " %d}2%s"), i +1, SettingsText(SET_FRIENDLYNAME1 +i));
  }
  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line
  int32_t rssi = WiFi.RSSI();
  WSContentSend_P(PSTR("}1" D_AP "%d " D_SSID " (" D_RSSI ")}2%s (%d%%, %d dBm)"), Settings.sta_active +1, SettingsText(SET_STASSID1 + Settings.sta_active), WifiGetRssiAsQuality(rssi), rssi);
  WSContentSend_P(PSTR("}1" D_HOSTNAME "}2%s%s"), my_hostname, (Wifi.mdns_begun) ? ".local" : "");
#if LWIP_IPV6
    String ipv6_addr = WifiGetIPv6();
    if(ipv6_addr != ""){
      WSContentSend_P(PSTR("}1 IPv6 Address }2%s"), ipv6_addr.c_str());
    }
#endif
  if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.localIP().toString().c_str());
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%s"), IPAddress(Settings.ip_address[1]).toString().c_str());
    WSContentSend_P(PSTR("}1" D_SUBNET_MASK "}2%s"), IPAddress(Settings.ip_address[2]).toString().c_str());
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "}2%s"), IPAddress(Settings.ip_address[3]).toString().c_str());
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.macAddress().c_str());
  }
  if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.softAPIP().toString().c_str());
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%s"), WiFi.softAPIP().toString().c_str());
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
  }
  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    WSContentSend_P(PSTR("}1" D_MQTT_HOST "}2%s"), SettingsText(SET_MQTT_HOST));
    WSContentSend_P(PSTR("}1" D_MQTT_PORT "}2%d"), Settings.mqtt_port);
    WSContentSend_P(PSTR("}1" D_MQTT_USER "}2%s"), SettingsText(SET_MQTT_USER));
    WSContentSend_P(PSTR("}1" D_MQTT_CLIENT "}2%s"), mqtt_client);
    WSContentSend_P(PSTR("}1" D_MQTT_TOPIC "}2%s"), SettingsText(SET_MQTT_TOPIC));
    uint32_t real_index = SET_MQTT_GRP_TOPIC;
    for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
      if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
      if (strlen(SettingsText(real_index +i))) {
        WSContentSend_P(PSTR("}1" D_MQTT_GROUP_TOPIC " %d}2%s"), 1 +i, GetGroupTopic_P(stopic, "", real_index +i));
      }
    }
    WSContentSend_P(PSTR("}1" D_MQTT_FULL_TOPIC "}2%s"), GetTopic_P(stopic, CMND, mqtt_topic, ""));
    WSContentSend_P(PSTR("}1" D_MQTT " " D_FALLBACK_TOPIC "}2%s"), GetFallbackTopic_P(stopic, ""));
  } else {
    WSContentSend_P(PSTR("}1" D_MQTT "}2" D_DISABLED));
  }
  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line

#ifdef USE_EMULATION
  WSContentSend_P(PSTR("}1" D_EMULATION "}2%s"), GetTextIndexed(stopic, sizeof(stopic), Settings.flag2.emulation, kEmulationOptions));
#else
  WSContentSend_P(PSTR("}1" D_EMULATION "}2" D_DISABLED));
#endif // USE_EMULATION

#ifdef USE_DISCOVERY
  WSContentSend_P(PSTR("}1" D_MDNS_DISCOVERY "}2%s"), (Settings.flag3.mdns_enabled) ? D_ENABLED : D_DISABLED);  // SetOption55 - Control mDNS service
  if (Settings.flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
#ifdef WEBSERVER_ADVERTISE
    WSContentSend_P(PSTR("}1" D_MDNS_ADVERTISE "}2" D_WEB_SERVER));
#else
    WSContentSend_P(PSTR("}1" D_MDNS_ADVERTISE "}2" D_DISABLED));
#endif // WEBSERVER_ADVERTISE
  }
#else
  WSContentSend_P(PSTR("}1" D_MDNS_DISCOVERY "}2" D_DISABLED));
#endif // USE_DISCOVERY

  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line
  WSContentSend_P(PSTR("}1" D_ESP_CHIP_ID "}2%d"), ESP.getChipId());
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), ESP.getFlashChipId());
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), ESP.getFlashChipRealSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%dkB"), ESP.getFlashChipSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_SIZE "}2%dkB"), ESP.getSketchSize() / 1024);
  WSContentSend_P(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%dkB"), ESP.getFreeSketchSpace() / 1024);
  WSContentSend_P(PSTR("}1" D_FREE_MEMORY "}2%dkB"), freeMem / 1024);
  WSContentSend_P(PSTR("</td></tr></table>"));

  WSContentSend_P(HTTP_SCRIPT_INFO_END);
  WSContentSendStyle();
  // WSContentSend_P(PSTR("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>"));
  WSContentSend_P(PSTR("<style>td{padding:0px 5px;}</style>"
                       "<div id='i' name='i'></div>"));
  //   WSContentSend_P(PSTR("</fieldset>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}
#endif  // Not FIRMWARE_MINIMAL

/*-------------------------------------------------------------------------------------------*/

void HandleUpgradeFirmware(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_FIRMWARE_UPGRADE);

  WSContentStart_P(S_FIRMWARE_UPGRADE);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_UPG, SettingsText(SET_OTAURL));
  WSContentSend_P(HTTP_FORM_RST_UPG, D_UPGRADE);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  Web.upload_error = 0;
  Web.upload_file_type = UPL_TASMOTA;
}

void HandleUpgradeFirmwareStart(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  char command[TOPSZ + 10];  // OtaUrl

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  WifiConfigCounter();

  char otaurl[TOPSZ];
  WebGetArg("o", otaurl, sizeof(otaurl));
  if (strlen(otaurl)) {
    snprintf_P(command, sizeof(command), PSTR(D_CMND_OTAURL " %s"), otaurl);
    ExecuteWebCommand(command, SRC_WEBGUI);
  }

  WSContentStart_P(S_INFORMATION);
  WSContentSend_P(HTTP_SCRIPT_RELOAD_OTA);
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
  WSContentSend_P(HTTP_MSG_RSTRT);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();

  snprintf_P(command, sizeof(command), PSTR(D_CMND_UPGRADE " 1"));
  ExecuteWebCommand(command, SRC_WEBGUI);
}

void HandleUploadDone(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  WifiConfigCounter();
  restart_flag = 0;
  MqttRetryCounter(0);

  WSContentStart_P(S_INFORMATION);
  if (!Web.upload_error) {
    WSContentSend_P(HTTP_SCRIPT_RELOAD_OTA);  // Refesh main web ui after OTA upgrade
  }
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
  if (Web.upload_error) {
    WSContentSend_P(PSTR("%06x'>" D_FAILED "</font></b><br><br>"), WebColor(COL_TEXT_WARNING));
#ifdef USE_RF_FLASH
    if (Web.upload_error < 15) {
#else
    if ((Web.upload_error < 10) || (14 == Web.upload_error)) {
      if (14 == Web.upload_error) { Web.upload_error = 10; }
#endif
      GetTextIndexed(error, sizeof(error), Web.upload_error -1, kUploadErrors);
    } else {
      snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), Web.upload_error);
    }
    WSContentSend_P(error);
    DEBUG_CORE_LOG(PSTR("UPL: %s"), error);
    stop_flash_rotate = Settings.flag.stop_flash_rotate;  // SetOption12 - Switch between dynamic or fixed slot flash save location
  } else {
    WSContentSend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br>"), WebColor(COL_TEXT_SUCCESS));
    WSContentSend_P(HTTP_MSG_RSTRT);
    ShowWebSource(SRC_WEBGUI);
#ifdef USE_TASMOTA_SLAVE
    if (TasmotaSlave_GetFlagFlashing()) {
      restart_flag = 0;
    } else { // It was a normal firmware file, or we are ready to restart device
      restart_flag = 2;
    }
#else
    restart_flag = 2;  // Always restart to re-enable disabled features during update
#endif
  }
  SettingsBufferFree();
  WSContentSend_P(PSTR("</div><br>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
#ifdef USE_TASMOTA_SLAVE
  if (TasmotaSlave_GetFlagFlashing()) {
    TasmotaSlave_Flash();
  }
#endif
}

void HandleUploadLoop(void)
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  bool _serialoutput = (LOG_LEVEL_DEBUG <= seriallog_level);

  if (HTTP_USER == Web.state) { return; }
  if (Web.upload_error) {
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
    return;
  }

  HTTPUpload& upload = WebServer->upload();

  if (UPLOAD_FILE_START == upload.status) {
    restart_flag = 60;
    if (0 == upload.filename.c_str()[0]) {
      Web.upload_error = 1;  // No file selected
      return;
    }
    SettingsSave(1);  // Free flash for upload
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    if (UPL_SETTINGS == Web.upload_file_type) {
      if (!SettingsBufferAlloc()) {
        Web.upload_error = 2;  // Not enough space
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
      if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
        MqttDisconnect();
      }
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {         //start with max available size

//        if (_serialoutput) Update.printError(Serial);
//        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
//          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
//        }

        Web.upload_error = 2;  // Not enough space
        return;
      }
    }
    Web.upload_progress_dot_count = 0;
  } else if (!Web.upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
    if (0 == upload.totalSize) {
      if (UPL_SETTINGS == Web.upload_file_type) {
        Web.config_block_count = 0;
      }
      else {
#ifdef USE_RF_FLASH
        if ((SONOFF_BRIDGE == my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
          Update.end();              // End esp8266 update session
          Web.upload_file_type = UPL_EFM8BB1;

          Web.upload_error = SnfBrUpdateInit();  // 10, 11
          if (Web.upload_error != 0) { return; }
        } else
#endif  // USE_RF_FLASH
#ifdef USE_TASMOTA_SLAVE
        if ((WEMOS == my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a ARDUINO SLAVE hex file
          Update.end();              // End esp8266 update session
          Web.upload_file_type = UPL_TASMOTASLAVE;
          Web.upload_error = TasmotaSlave_UpdateInit();  // 0
          if (Web.upload_error != 0) { return; }
        } else
#endif
        {
          if ((upload.buf[0] != 0xE9) && (upload.buf[0] != 0x1F)) {  // 0x1F is gzipped 0xE9
            Web.upload_error = 3;  // Magic byte is not 0xE9
            return;
          }
          if (0xE9 == upload.buf[0]) {
            uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
            if (bin_flash_size > ESP.getFlashChipRealSize()) {
              Web.upload_error = 4;  // Program flash size is larger than real flash size
              return;
            }
//            upload.buf[2] = 3;  // Force DOUT - ESP8285
          }
        }
      }
    }
    if (UPL_SETTINGS == Web.upload_file_type) {
      if (!Web.upload_error) {
        if (upload.currentSize > (sizeof(Settings) - (Web.config_block_count * HTTP_UPLOAD_BUFLEN))) {
          Web.upload_error = 9;  // File too large
          return;
        }
        memcpy(settings_buffer + (Web.config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
        Web.config_block_count++;
      }
    }
#ifdef USE_RF_FLASH
    else if (UPL_EFM8BB1 == Web.upload_file_type) {
      if (efm8bb1_update != nullptr) {    // We have carry over data since last write, i. e. a start but not an end
        ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
        free(efm8bb1_update);
        efm8bb1_update = nullptr;
        if (result != 0) {
          Web.upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid, 12, 13
          return;
        }
      }
      ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
      if (result < 0) {
        Web.upload_error = abs(result);  // 8, 12, 13
        return;
      } else if (result > 0) {
        if ((size_t)result > upload.currentSize) {
          // Offset is larger than the buffer supplied, this should not happen
          Web.upload_error = 9;  // File too large - Failed to decode RF firmware
          return;
        }
        // A remnant has been detected, allocate data for it plus a null termination byte
        size_t remnant_sz = upload.currentSize - result;
        efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
        if (efm8bb1_update == nullptr) {
          Web.upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
          return;
        }
        memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
        // Add null termination at the end of of remnant buffer
        efm8bb1_update[remnant_sz] = '\0';
      }
    }
#endif  // USE_RF_FLASH
#ifdef USE_TASMOTA_SLAVE
    else if (UPL_TASMOTASLAVE == Web.upload_file_type) {
      TasmotaSlave_WriteBuffer(upload.buf, upload.currentSize);
    }
#endif
    else {  // firmware
      if (!Web.upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
        Web.upload_error = 5;  // Upload buffer miscompare
        return;
      }
      if (_serialoutput) {
        Serial.printf(".");
        Web.upload_progress_dot_count++;
        if (!(Web.upload_progress_dot_count % 80)) { Serial.println(); }
      }
    }
  } else if(!Web.upload_error && (UPLOAD_FILE_END == upload.status)) {
    if (_serialoutput && (Web.upload_progress_dot_count % 80)) {
      Serial.println();
    }
    if (UPL_SETTINGS == Web.upload_file_type) {
      if (Web.config_xor_on_set) {
        for (uint32_t i = 2; i < sizeof(Settings); i++) {
          settings_buffer[i] ^= (Web.config_xor_on_set +i);
        }
      }
      bool valid_settings = false;
      unsigned long buffer_version = settings_buffer[11] << 24 | settings_buffer[10] << 16 | settings_buffer[9] << 8 | settings_buffer[8];
      if (buffer_version > 0x06000000) {
        uint32_t buffer_size = settings_buffer[3] << 8 | settings_buffer[2];
        if (buffer_version > 0x0606000A) {
          uint32_t buffer_crc32 = settings_buffer[4095] << 24 | settings_buffer[4094] << 16 | settings_buffer[4093] << 8 | settings_buffer[4092];
          valid_settings = (GetCfgCrc32(settings_buffer, buffer_size -4) == buffer_crc32);
        } else {
          uint16_t buffer_crc16 = settings_buffer[15] << 8 | settings_buffer[14];
          valid_settings = (GetCfgCrc16(settings_buffer, buffer_size) == buffer_crc16);
        }
      } else {
        valid_settings = (settings_buffer[0] == CONFIG_FILE_SIGN);
      }
      if (valid_settings) {
        SettingsDefaultSet2();
        memcpy((char*)&Settings +16, settings_buffer +16, sizeof(Settings) -16);
        Settings.version = buffer_version;  // Restore version and auto upgrade after restart
        SettingsBufferFree();
      } else {
        Web.upload_error = 8;  // File invalid
        return;
      }
    }
#ifdef USE_RF_FLASH
    else if (UPL_EFM8BB1 == Web.upload_file_type) {
      // RF FW flash done
      Web.upload_file_type = UPL_TASMOTA;
    }
#endif  // USE_RF_FLASH
#ifdef USE_TASMOTA_SLAVE
    else if (UPL_TASMOTASLAVE == Web.upload_file_type) {
      // Done writing the hex to SPI flash
      TasmotaSlave_SetFlagFlashing(true); // So we know on upload success page if it needs to flash hex or do a normal restart
      Web.upload_file_type = UPL_TASMOTA;
    }
#endif
    else {
      if (!Update.end(true)) { // true to set the size to the current progress
        if (_serialoutput) { Update.printError(Serial); }
        Web.upload_error = 6;  // Upload failed. Enable logging 3
        return;
      }
      if (!VersionCompatible()) {
        Web.upload_error = 14;  // Not compatible
        return;
      }
    }
    if (!Web.upload_error) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
    }
  } else if (UPLOAD_FILE_ABORTED == upload.status) {
    restart_flag = 0;
    MqttRetryCounter(0);
    Web.upload_error = 7;  // Upload aborted
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
  }
  delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void HandlePreflightRequest(void)
{
  HttpHeaderCors();
  WebServer->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  WebServer->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  WSSend(200, CT_HTML, "");
}

/*-------------------------------------------------------------------------------------------*/

void HandleHttpCommand(void)
{
  if (!HttpCheckPriviledgedAccess(false)) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  bool valid = true;
  if (strlen(SettingsText(SET_WEBPWD))) {
    char tmp1[33];
    WebGetArg("user", tmp1, sizeof(tmp1));
    char tmp2[strlen(SettingsText(SET_WEBPWD)) +1];
    WebGetArg("password", tmp2, sizeof(tmp2));
    if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, SettingsText(SET_WEBPWD)))) { valid = false; }
  }

  WSContentBegin(200, CT_JSON);
  if (valid) {
    uint32_t curridx = web_log_index;
    String svalue = WebServer->arg("cmnd");
    if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
      ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCOMMAND);
      if (web_log_index != curridx) {
        uint32_t counter = curridx;
        WSContentSend_P(PSTR("{"));
        bool cflg = false;
        do {
          char* tmp;
          size_t len;
          GetLog(counter, &tmp, &len);
          if (len) {
            // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
            char* JSON = (char*)memchr(tmp, '{', len);
            if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
              size_t JSONlen = len - (JSON - tmp);
              if (JSONlen > sizeof(mqtt_data)) { JSONlen = sizeof(mqtt_data); }
              char stemp[JSONlen];
              strlcpy(stemp, JSON +1, JSONlen -2);
              WSContentSend_P(PSTR("%s%s"), (cflg) ? "," : "", stemp);
              cflg = true;
            }
          }
          counter++;
          counter &= 0xFF;
          if (!counter) counter++;  // Skip 0 as it is not allowed
        } while (counter != web_log_index);
        WSContentSend_P(PSTR("}"));
      } else {
        WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENABLE_WEBLOG_FOR_RESPONSE "\"}"));
      }
    } else {
      WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENTER_COMMAND " cmnd=\"}"));
    }
  } else {
    WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
  }
  WSContentEnd();
}

/*-------------------------------------------------------------------------------------------*/

void HandleConsole(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (WebServer->hasArg("c2")) {      // Console refresh requested
    HandleConsoleRefresh();
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONSOLE);

  WSContentStart_P(S_CONSOLE);
  WSContentSend_P(HTTP_SCRIPT_CONSOL, Settings.web_refresh);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_CMND);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

void HandleConsoleRefresh(void)
{
  bool cflg = true;
  uint32_t counter = 0;                // Initial start, should never be 0 again

  String svalue = WebServer->arg("c1");
  if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
    ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
  }

  char stmp[8];
  WebGetArg("c2", stmp, sizeof(stmp));
  if (strlen(stmp)) { counter = atoi(stmp); }

  WSContentBegin(200, CT_PLAIN);
  WSContentSend_P(PSTR("%d}1%d}1"), web_log_index, Web.reset_web_log_flag);
  if (!Web.reset_web_log_flag) {
    counter = 0;
    Web.reset_web_log_flag = true;
  }
  if (counter != web_log_index) {
    if (!counter) {
      counter = web_log_index;
      cflg = false;
    }
    do {
      char* tmp;
      size_t len;
      GetLog(counter, &tmp, &len);
      if (len) {
        if (len > sizeof(mqtt_data) -2) { len = sizeof(mqtt_data); }
        char stemp[len +1];
        strlcpy(stemp, tmp, len);
        WSContentSend_P(PSTR("%s%s"), (cflg) ? "\n" : "", stemp);
        cflg = true;
      }
      counter++;
      counter &= 0xFF;
      if (!counter) { counter++; }  // Skip log index 0 as it is not allowed
    } while (counter != web_log_index);
  }
  WSContentSend_P(PSTR("}1"));
  WSContentEnd();
}

/********************************************************************************************/

void HandleNotFound(void)
{
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Not found (%s)"), WebServer->uri().c_str());

  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the error page.

#ifdef USE_EMULATION
#ifdef USE_EMULATION_HUE
  String path = WebServer->uri();
  if ((EMUL_HUE == Settings.flag2.emulation) && (path.startsWith("/api"))) {
    HandleHueApi(&path);
  } else
#endif  // USE_EMULATION_HUE
#endif  // USE_EMULATION
  {
    WSContentBegin(404, CT_PLAIN);
    WSContentSend_P(PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), WebServer->uri().c_str(), (WebServer->method() == HTTP_GET) ? "GET" : "POST", WebServer->args());
    for (uint32_t i = 0; i < WebServer->args(); i++) {
      WSContentSend_P(PSTR(" %s: %s\n"), WebServer->argName(i).c_str(), WebServer->arg(i).c_str());
    }
    WSContentEnd();
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
bool CaptivePortal(void)
{
  // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
  if ((WifiIsInManagerMode()) && !ValidIpAddress(WebServer->hostHeader().c_str())) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    WebServer->sendHeader(F("Location"), String("http://") + WebServer->client().localIP().toString(), true);
    WSSend(302, CT_PLAIN, "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    WebServer->client().stop();  // Stop is needed because we sent no content length
    return true;
  }
  return false;
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
    if ((' ' == decodedChar) || ('+' == decodedChar)) {
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
  // [tasmota] POWER1 ON                                               --> Sends http://tasmota/cm?cmnd=POWER1 ON
  // [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
  // [tasmota] /any/link/starting/with/a/slash.php?log=123             --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123
  // [tasmota,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123

  char *host;
  char *user;
  char *password;
  char *command;
  int status = 1;                             // Wrong parameters

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
    url += command;                           // url = |http://192.168.178.86/cm?cmnd=POWER1 ON|

    DEBUG_CORE_LOG(PSTR("WEB: Uri |%s|"), url.c_str());

#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
    HTTPClient http;
    if (http.begin(UrlEncode(url))) {         // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
#else
    WiFiClient http_client;
    HTTPClient http;
    if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
#endif
      int http_code = http.GET();             // Start connection and send HTTP header
      if (http_code > 0) {                    // http_code will be negative on error
        if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
#ifdef USE_WEBSEND_RESPONSE
          // Return received data to the user - Adds 900+ bytes to the code
          const char* read = http.getString().c_str();  // File found at server - may need lot of ram or trigger out of memory!
          uint32_t j = 0;
          char text = '.';
          while (text != '\0') {
            text = *read++;
            if (text > 31) {                  // Remove control characters like linefeed
              mqtt_data[j++] = text;
              if (j == sizeof(mqtt_data) -2) { break; }
            }
          }
          mqtt_data[j] = '\0';
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_WEBSEND));
#ifdef USE_SCRIPT
extern uint8_t tasm_cmd_activ;
          // recursive call must be possible in this case
          tasm_cmd_activ=0;
          XdrvRulesProcess();
#endif  // USE_SCRIPT
#endif  // USE_WEBSEND_RESPONSE
        }
        status = 0;                           // No error - Done
      } else {
        status = 2;                           // Connection failed
      }
      http.end();                             // Clean up connection data
    } else {
      status = 3;                             // Host not found or connection error
    }
  }
  return status;
}

bool JsonWebColor(const char* dataBuf)
{
  // Default (Dark theme)
  // {"WebColor":["#eaeaea","#252525","#4f4f4f","#000","#ddd","#65c115","#1f1f1f","#ff5661","#008000","#faffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#faffff","#999","#eaeaea"]}
  // Default pre v7 (Light theme)
  // {"WebColor":["#000","#fff","#f2f2f2","#000","#fff","#000","#fff","#f00","#008000","#fff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#fff","#999","#000"]}	  // {"WebColor":["#000000","#ffffff","#f2f2f2","#000000","#ffffff","#000000","#ffffff","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}

  char dataBufLc[strlen(dataBuf) +1];
  LowerCase(dataBufLc, dataBuf);
  RemoveSpace(dataBufLc);
  if (strlen(dataBufLc) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

  StaticJsonBuffer<450> jb;  // 421 from https://arduinojson.org/v5/assistant/
  JsonObject& obj = jb.parseObject(dataBufLc);
  if (!obj.success()) { return false; }

  char parm_lc[10];
  if (obj[LowerCase(parm_lc, D_CMND_WEBCOLOR)].success()) {
    for (uint32_t i = 0; i < COL_LAST; i++) {
      const char* color = obj[parm_lc][i];
      if (color != nullptr) {
        WebHexCode(i, color);
      }
    }
  }
  return true;
}

const char kWebSendStatus[] PROGMEM = D_JSON_DONE "|" D_JSON_WRONG_PARAMETERS "|" D_JSON_CONNECT_FAILED "|" D_JSON_HOST_NOT_FOUND "|" D_JSON_MEMORY_ERROR;

const char kWebCommands[] PROGMEM = "|"  // No prefix
#ifdef USE_EMULATION
  D_CMND_EMULATION "|"
#endif
#ifdef USE_SENDMAIL
  D_CMND_SENDMAIL "|"
#endif
  D_CMND_WEBSERVER "|" D_CMND_WEBPASSWORD "|" D_CMND_WEBLOG "|" D_CMND_WEBREFRESH "|" D_CMND_WEBSEND "|" D_CMND_WEBCOLOR "|"
  D_CMND_WEBSENSOR "|" D_CMND_WEBBUTTON "|" D_CMND_CORS;

void (* const WebCommand[])(void) PROGMEM = {
#ifdef USE_EMULATION
  &CmndEmulation,
#endif
#ifdef USE_SENDMAIL
  &CmndSendmail,
#endif
  &CmndWebServer, &CmndWebPassword, &CmndWeblog, &CmndWebRefresh, &CmndWebSend, &CmndWebColor,
  &CmndWebSensor, &CmndWebButton, &CmndCors };

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef USE_EMULATION
void CmndEmulation(void)
{
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
    Settings.flag2.emulation = XdrvMailbox.payload;
    restart_flag = 2;
  }
#endif
  ResponseCmndNumber(Settings.flag2.emulation);
}
#endif  // USE_EMULATION

#ifdef USE_SENDMAIL
void CmndSendmail(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t result = SendMail(XdrvMailbox.data);
    char stemp1[20];
    ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebSendStatus));
  }
}
#endif  // USE_SENDMAIL


void CmndWebServer(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings.webserver = XdrvMailbox.payload;
  }
  if (Settings.webserver) {
    Response_P(PSTR("{\"" D_CMND_WEBSERVER "\":\"" D_JSON_ACTIVE_FOR " %s " D_JSON_ON_DEVICE " %s " D_JSON_WITH_IP_ADDRESS " %s\"}"),
      (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str());
  } else {
    ResponseCmndStateText(0);
  }
}

void CmndWebPassword(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_WEBPWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? WEB_PASSWORD : XdrvMailbox.data);
    ResponseCmndChar(SettingsText(SET_WEBPWD));
  } else {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

void CmndWeblog(void)
{
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    Settings.weblog_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.weblog_level);
}

void CmndWebRefresh(void)
{
  if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload <= 10000)) {
    Settings.web_refresh = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.web_refresh);
}

void CmndWebSend(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint32_t result = WebSend(XdrvMailbox.data);
    char stemp1[20];
    ResponseCmndChar(GetTextIndexed(stemp1, sizeof(stemp1), result, kWebSendStatus));
  }
}

void CmndWebColor(void)
{
  if (XdrvMailbox.data_len > 0) {
    if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
      if ((XdrvMailbox.data_len > 3) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= COL_LAST)) {
        WebHexCode(XdrvMailbox.index -1, XdrvMailbox.data);
      }
      else if (0 == XdrvMailbox.payload) {
        SettingsDefaultWebColor();
      }
    }
    else {
      JsonWebColor(XdrvMailbox.data);
    }
  }
  Response_P(PSTR("{\"" D_CMND_WEBCOLOR "\":["));
  for (uint32_t i = 0; i < COL_LAST; i++) {
    if (i) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"#%06x\""), WebColor(i));
  }
  ResponseAppend_P(PSTR("]}"));
}

void CmndWebSensor(void)
{
  if (XdrvMailbox.index < MAX_XSNS_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings.sensors[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
    }
  }
  Response_P(PSTR("{\"" D_CMND_WEBSENSOR "\":"));
  XsnsSensorState();
  ResponseJsonEnd();
}

void CmndWebButton(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_BUTTON_TEXT)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_BUTTON1, MAX_BUTTON_TEXT);
    } else {
      if (XdrvMailbox.data_len > 0) {
        SettingsUpdateText(SET_BUTTON1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
      }
      ResponseCmndIdxChar(SettingsText(SET_BUTTON1 + XdrvMailbox.index -1));
    }
  }
}

void CmndCors(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_CORS, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? WEB_PASSWORD : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_CORS));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv01(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      PollDnsWebserver();
#ifdef USE_EMULATION
#ifdef USE_DEVICE_GROUPS
      if (Settings.flag2.emulation || Settings.flag4.device_groups_enabled) { PollUdp(); }
#else // USE_DEVICE_GROUPS
      if (Settings.flag2.emulation) { PollUdp(); }
#endif // USE_DEVICE_GROUPS
#endif  // USE_EMULATION
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWebCommands, WebCommand);
      break;
  }
  return result;
}
#endif  // USE_WEBSERVER
