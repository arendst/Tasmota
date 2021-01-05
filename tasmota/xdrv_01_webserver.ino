/*
  xdrv_01_webserver.ino - webserver for Tasmota

  Copyright (C) 2021  Theo Arends

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

#include "UnishoxStrings.h"

#ifndef WIFI_SOFT_AP_CHANNEL
#define WIFI_SOFT_AP_CHANNEL                  1          // Soft Access Point Channel number between 1 and 11 as used by WifiManager web GUI
#endif

const uint16_t CHUNKED_BUFFER_SIZE = (MESSZ / 2) - 100;  // Chunk buffer size (should be smaller than half mqtt_data size = MESSZ)

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

enum UploadTypes { UPL_TASMOTA, UPL_SETTINGS, UPL_EFM8BB1, UPL_TASMOTACLIENT, UPL_EFR32, UPL_SHD, UPL_CCL };

#ifdef USE_UNISHOX_COMPRESSION
  #ifdef USE_JAVASCRIPT_ES6
    #include "./html_compressed/HTTP_HEADER1_ES6.h"
  #else
    #include "./html_compressed/HTTP_HEADER1_NOES6.h"
  #endif
#else
  #ifdef USE_JAVASCRIPT_ES6
    #include "./html_uncompressed/HTTP_HEADER1_ES6.h"
  #else
    #include "./html_uncompressed/HTTP_HEADER1_NOES6.h"
  #endif
#endif

///////////////////////////////////////////////////////
const char HTTP_SEND_STYLE_U[] PROGMEM =
  //=HTTP_SCRIPT_COUNTER
  "var cn=180;"                           // seconds
  "function u(){"
    "if(cn>=0){"
      "eb('t').innerHTML='%s '+cn+' %s';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "wl(u);"
  "\0"
  //=HTTP_HEAD_LAST_SCRIPT
  "function jd(){"                        // Add label name='' based on provided id=''
    "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
    "while(i.length>=t){"
      "if(i[t]){"
        "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
      "}"
      "t++;"
    "}"
  "}"
  "wl(jd);"                               // Add name='' to any id='' in input,button,textarea,select
  "</script>"
  "\0"
  //=HTTP_HEAD_STYLE1
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
  "td{padding:0px;}"
  "\0"
  //=HTTP_HEAD_STYLE2
  "button{border:0;border-radius:0.3rem;background:#%06x;color:#%06x;line-height:2.4rem;font-size:1.2rem;width:100%%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  "button:hover{background:#%06x;}"  // COLOR_BUTTON_HOVER
  ".bred{background:#%06x;}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:#%06x;}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{background:#%06x;}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:#%06x;}"  // COLOR_BUTTON_SAVE_HOVER
  "a{color:#%06x;text-decoration:none;}"  // COLOR_BUTTON
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ".r{border-radius:0.3em;padding:2px;margin:6px 2px;}"
  "\0"
  //=HTTP_HEAD_ERROR_TOO_LONG
  "ERROR: WSContentSendStyle_P size %d > mqtt_data size %d. Start of data [%s...]"
  "\0"
  //=HTTP_HEAD_STYLE3
  "</style>"
  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;'>"  // COLOR_TEXT
  "<div style='text-align:center;color:#%06x;'><noscript>%s<br></noscript>" // COLOR_TITLE
  "<h3>%s %s</h3>"
  "<h2>%s</h2>"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_SCRIPT_COUNTER=0,
  HTTP_HEAD_LAST_SCRIPT=100,
  HTTP_HEAD_STYLE1=327,
  HTTP_HEAD_STYLE2=919,
  HTTP_HEAD_ERROR_TOO_LONG=1398,
  HTTP_HEAD_STYLE3=1477,
};

// Compressed from 1674 to 981, -41.4%
const char HTTP_SEND_STYLE[] PROGMEM = "\x00\x69\x33\xBF\xA0\xF8\xF8\x72\x7D\x9E\x08\xC0\xBE\x0E\xB7\x39\x0B\x3B\xA7\x78"
                             "\xF6\xE9\x83\xBA\x1F\x1F\x87\xC3\x8C\xEF\x1E\xD2\x63\x8E\xE9\xF7\x47\xD9\xDE\x3A"
                             "\x6F\x73\xF9\x0A\x2A\x2B\x21\xA4\x11\xF0\xFB\x3E\xBC\x8F\xB3\xB6\x1F\x1D\xB3\xED"
                             "\x1F\x5E\x3E\xCF\x01\xF1\xD6\x75\x9E\x3C\xE8\xAB\x46\xBC\x17\x47\x74\x59\xD4\x72"
                             "\x36\xCE\xF1\xE0\xF7\x1E\xE2\x1F\x08\xEE\x8B\x3B\xC7\x89\xC2\xF8\x22\x66\xA6\xB1"
                             "\x0E\xE9\xDE\x3D\xA6\x77\xF5\x47\xC3\x8C\xEA\x2D\x3E\x09\x81\x8B\x1A\xFA\x8E\x86"
                             "\xA1\x6F\xE6\x45\xE6\x13\x0E\xB3\xE5\x61\x04\x77\x4F\xBD\xE1\x82\xE8\xEA\x1C\x2E"
                             "\xAB\x38\xEA\xA6\x6C\xAB\xFB\xB3\xAB\xCC\x26\x1D\x1F\x67\x78\xF0\x3E\x2B\x42\x67"
                             "\x77\x4E\x81\x3E\x1E\xA1\x47\xE1\xF2\x8E\xF1\xED\xD3\x07\x77\x4F\x7A\x8F\x7C\xEF"
                             "\x1E\xDD\x3D\xEA\x3D\xF3\xDE\x3E\xFA\xC6\xB3\xEC\xF7\xCF\x87\x77\x4F\x7A\x8F\x7C"
                             "\xE8\x2A\x2B\xFC\x57\x55\xFD\x1C\x2E\x99\xDD\x3E\xF4\x43\xEC\xEF\x1F\xA3\xF4\x77"
                             "\x4F\xE0\x27\x57\xEB\x1A\xCF\xB3\xBC\x77\x8E\xF1\xDA\x04\x1C\x87\x44\x3E\xCF\x7C"
                             "\xF3\x04\x7C\xB0\xF0\x7B\xA8\xED\x9D\xB0\x41\xE0\xD4\xD6\x21\xDE\x3C\x1E\x87\x67"
                             "\x83\xFE\x8C\xA3\xF2\x70\xBE\x7A\x7A\x8C\x82\x67\xE0\x9A\x66\x75\x18\xD6\x10\x9E"
                             "\x74\x75\x02\x0F\x04\x2A\x01\x83\xC0\x81\xED\x19\x62\x09\xBC\x3C\x79\x9C\xD2\x18"
                             "\x6C\x3C\x18\xCE\xA3\xAF\xDA\x6D\x67\x99\xC8\x4C\x69\xE0\xF7\x02\x1E\x1A\x7B\x47"
                             "\x58\x66\x83\xDF\xC1\x7C\x21\xE6\x79\x1F\x47\x1F\x11\xB0\xF0\x7B\x86\x1E\xD1\xB7"
                             "\xF1\xED\xE3\xCC\xE3\x3A\x69\x31\xA8\xE3\x3C\x1E\xE2\xDE\x18\x2E\x8F\x68\xFE\x89"
                             "\x42\x8F\x33\x91\xB3\xE8\xFA\x3C\x10\xEC\x36\x1D\x7E\xD3\x6E\xF0\xF1\xE6\x3B\x3E"
                             "\x23\xF9\xD6\x3B\x0D\x87\x83\xAC\x7D\x8E\x34\xDA\x3A\xC1\x4A\xD0\xDC\x36\x82\x95"
                             "\x70\x4C\xD8\x43\xC0\xB3\xE0\x87\xB6\xEF\x0C\x17\x47\xBD\x46\x43\x19\xF0\x31\x4C"
                             "\x33\x41\xD8\x6C\x3D\xF0\x41\xE1\x14\x10\x68\xFE\xC4\xDC\x3D\xF0\x43\xDF\x18\xD3"
                             "\xC0\x20\xF0\x38\x75\xFF\x47\x85\x51\xE6\x73\x88\x61\xB0\xF0\x66\xFF\x5A\x1D\x84"
                             "\x75\xD8\x5A\x3D\xC7\x99\xD6\x72\x10\xC0\x49\xCC\xFF\x7C\x3C\xC1\x0F\x1E\x38\x91"
                             "\xF4\x7D\x1E\x0F\x71\x02\x17\x13\xE0\x91\xC2\x88\x2C\x70\x04\x08\x7C\xC6\x9E\xDF"
                             "\xBF\x69\xB5\x9E\x60\x87\xA8\x1E\x01\x0B\x32\x39\xF3\xE8\xFA\x3C\x10\xA7\xA3\xC2"
                             "\xA8\xF3\x39\x73\xE4\x30\xD8\x78\x04\x7E\x3E\xB0\xCD\xFC\xC0\x58\x3E\x79\xD8\xBA"
                             "\xC0\x5A\xD1\x47\x60\x86\x47\xB6\x99\xB2\x81\x0B\x82\xA0\xDF\x53\xF8\x20\xF2\x7A"
                             "\x62\xC6\xE8\x46\x47\x99\x9B\xF8\x97\xD6\x75\x7A\xFB\xC7\x5F\x9F\xF4\xC0\x26\xE6"
                             "\x07\xBA\x84\x04\x4E\x5F\x4E\x32\x18\x6C\x3C\x1E\xE9\xC2\xF8\x20\xF4\x15\x8F\x68"
                             "\x20\xF1\xE8\x79\x9C\x67\x82\x04\x2E\x3F\x7F\x62\x68\xBF\x1E\x67\x19\xD2\xCB\xEC"
                             "\x68\x2C\xEF\x01\x6F\x33\xAC\x10\xB8\x41\x39\x47\x4C\x2F\xB1\xA0\x8F\xCE\xA1\xD0"
                             "\xE5\x17\xD8\xD0\x42\xE1\xAC\x10\x78\xAE\x04\x3E\x63\x6B\xF7\xDE\xDA\xDC\xE3\xAC"
                             "\x41\x7F\xBA\xDC\xE3\xCC\xE3\x3A\x61\x78\xF0\x0A\x95\x43\x17\xFF\x67\xCF\x31\x99"
                             "\xBD\x4F\xE7\x83\xDC\x08\x3C\x0C\x1E\x62\xB0\xCD\xFC\x13\xBA\x06\x07\x41\xDF\x62"
                             "\x02\x9D\x40\x56\xC4\x87\xBF\xC0\xA7\x50\x15\x31\x2B\x3D\xA0\x8F\xC3\x7D\x33\x65"
                             "\x1D\x62\x30\xF0\x10\xB8\x1A\xEC\xE6\x78\x3D\xC7\x41\x87\xB4\xC0\x59\x74\x79\x84"
                             "\xCC\x50\x20\xE0\x2A\x01\x83\xC9\x48\x4C\xC5\x1E\x0F\x71\xD0\xD4\x08\x56\xFF\xA3"
                             "\xC2\x81\x22\xE0\x20\xCD\x3D\xC7\x4F\x82\x17\x20\x60\x8D\xC7\xD3\x1A\x08\x9C\xD7"
                             "\x9C\xA2\x18\x6C\x04\x2E\x83\x09\xE6\x73\x88\x61\xB1\x1C\xA2\x18\x6C\x3C\x1E\xE9"
                             "\xC2\xF9\x32\xF9\x7C\xB0\xBE\x79\xA2\x1F\x2F\x10\x79\xD4\xFA\x8B\xCF\x84\x2F\x51"
                             "\x90\x4C\xFD\x90\xC5\xED\x36\xB4\x7D\x08\x8F\xC4\x34\xD5\x54\x7E\xC4\xBA\xB0\x43"
                             "\xA0\x74\x45\xEA\xBF\xD2\xC3\x08\x4B\xAB\x47\xBC\x7D\x78\xE8\x74\x3A\x1E\xFC\xE1"
                             "\x7C\xF4\x3B\x01\x07\xA3\xB6\x7A\x1D\x82\x9D\x88\x7E\x1E\x83\xB0\x43\x23\xF0\xF4"
                             "\x13\x4C\xD7\xA8\xC8\x26\x7C\x3E\xC1\x2F\x83\x21\x37\xC3\x02\xB3\x23\xCF\x78\x2D"
                             "\xE6\x75\x8E\x0B\x03\x34\x04\x8E\x66\x86\xEF\x1D\x60\x83\xCB\xD1\xCB\xC6\x43\x0D"
                             "\x87\x83\xEC\x14\xF2\xB0\x42\xE8\x27\x02\x3E\x30\x7D\x9F\x87\xA7\x60\x21\x74\x9D"
                             "\x4F\xAF\x1E\x83\xBE\x7E\x1E\x87\x60\x22\x4D\x3D\x05\x1C\xB3\xF0\xFA\x2F\x23\xEB"
                             "\xC7\xA1\xD8\x28\xE5\x9F\x87\xA1\x0A\x39\x47\xE1\xF4\x5E\x3D\x0E\xC1\x47\x28\xFC"
                             "\x29\xC2\xF9";
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
const char HTTP_SEND_STYLE_ZIGBEE_U[] PROGMEM =
  //=HTTP_HEAD_STYLE_ZIGBEE
  ".bt{box-sizing:border-box;position:relative;display:inline-block;width:20px;height:12px;border:2px solid;border-radius:3px;margin-left:-3px}"
  ".bt::after,.bt::before{content:\"\";display:block;box-sizing:border-box;position:absolute;height:6px;background:currentColor;top:1px}"
  ".bt::before{right:-4px;border-radius:3px;width:4px}"
  ".bt::after{width:var(--bl,14px);left:1px}"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_HEAD_STYLE_ZIGBEE=0,
};

// Compressed from 364 to 242, -33.5%
const char HTTP_SEND_STYLE_ZIGBEE[] PROGMEM = "\x00\x17\x3A\x0E\xA3\xDA\x3B\x0D\x87\x5F\xB4\xDB\xBC\x3C\x79\x8E\xCF\x88\xFE\x75"
                             "\x8E\xC3\x61\xE0\x66\x7B\x6B\x73\x8F\x3F\xB0\xAE\xB4\xCD\x9E\x04\xDF\x0C\x0A\xCC"
                             "\x8F\x3D\xE0\xB7\x99\xD6\x38\x2C\x0C\xD0\xF0\x3F\xA2\x50\xA3\xCC\xE5\x32\x18\x6C"
                             "\x3C\x0A\x7A\x3C\x2A\x2B\x8F\x33\x92\x88\x61\xB0\xF0\x08\x39\x29\xE6\x72\x88\x61"
                             "\xB1\x7B\x02\xD1\x01\x0A\x69\xD7\xFB\x13\x45\xF8\xF3\x39\x64\x30\xD8\x78\x1B\x7F"
                             "\x1E\xDE\x3A\xC2\x66\x28\xF3\x3A\xCE\x59\x0C\x36\x1E\xE3\xA0\xEA\x3C\xCF\x3B\x31"
                             "\x4F\xE7\x51\xD0\x75\x1E\x67\x98\xE6\x63\x3E\xCF\x68\x79\xD4\xFA\x8F\x33\xD8\x7B"
                             "\x01\x13\x5E\x04\x1D\x5C\x16\xB8\x14\xB1\xDE\xC0\x85\xD3\x04\x3D\xD0\xE7\x10\xC3"
                             "\x61\xE0\x75\x86\x68\x3D\xFC\x17\xC2\x1E\x61\x8B\xFF\xDF\x51\x07\x81\x67\xCF\x15"
                             "\x83\x0F\x33\x90\x81\x0F\x5F\x04\x2D\x53\xFA\x3C\x2A\x2B\x8F\x33\xAC\xE6\x10\x22"
                             "\x70\x54\x08\xFC\x0C\x82\x0F\x0A\x67\x30\x81\x23\x81\x23\xDA\x08\x34\x4C\xEF\xE7"
                             "\x74\xEB\x3A\xC7\x04\x75\x1C\x98\x43\x0D\x87\x78\xF0\x13\x31\x47\x99\xC8\x43\x0D"
                             "\x87\xBA\x70\xBE";
///////////////////////////////////////////////////////

// const char HTTP_SCRIPT_COUNTER[] PROGMEM =
//   "var cn=180;"                           // seconds
//   "function u(){"
//     "if(cn>=0){"
//       "eb('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
//       "cn--;"
//       "setTimeout(u,1000);"
//     "}"
//   "}"
//   "wl(u);";


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
    #include "./html_uncompressed/HTTP_SCRIPT_ROOT_NO_WEB_DISPLAY.h"
  #endif
  #include "./html_uncompressed/HTTP_SCRIPT_ROOT_PART2.h"
#endif

#ifdef USE_ENHANCED_GUI_WIFI_SCAN
///////////////////////////////////////////////////////
const char HTTP_WIFI_CONF_ENHANCED_U[] PROGMEM =
  //=HTTP_HEAD_STYLE_SSI
  // Signal Strength Indicator
  ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0}"
  ".si i{width:3px;margin-right:1px;border-radius:3px;background-color:#%06x}"
  ".si .b0{height:25%%}.si .b1{height:50%%}.si .b2{height:75%%}.si .b3{height:100%%}.o30{opacity:.3}"
  "\0"
  //=HTTP_PRINT_SSID
  "<div><a href='#p' onclick='c(this)'>%s</a><br>"
  "\0"
  //=HTTP_PRINT_ITEM
  "<div title='%d dBm (%d%%)'>%s<span class='q'>(%d) <div class='si'>"
  "\0"
  //=HTTP_PRINT_SSI
  "<i class='b%d%s'></i>"
  "\0"
  //=HTTP_PRINT_END
  "</span></div></div>"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_HEAD_STYLE_SSI=0,
  HTTP_PRINT_SSID=239,
  HTTP_PRINT_ITEM=286,
  HTTP_PRINT_SSI=353,
  HTTP_PRINT_END=375,
};

// Compressed from 395 to 310, -21.5%
const char HTTP_WIFI_CONF_ENHANCED[] PROGMEM = "\x00\x19\x3A\x7B\x4F\x68\x9B\xE1\x81\x59\x91\xE7\xBC\x16\xF3\x3A\xCC\x04\xCD\x87"
                             "\x8B\x0B\x47\xB8\xEB\xDA\x63\x7C\x79\x98\x09\x9B\x0E\xB7\xC2\x1E\x05\x3D\x1E\x15"
                             "\x47\x99\xC9\xA4\x30\xD8\x78\x19\x62\x09\xBC\x3C\x79\x9C\x67\xB8\xE8\x5E\xD5\xA7"
                             "\xB4\x7F\x44\xA1\x47\x99\xCB\x21\x86\xC3\xC0\xDB\xF8\xF6\xF1\xD7\xF0\x41\xC2\x06"
                             "\x1B\x0F\x03\xB3\xE2\x3F\x9D\x7F\xB1\x34\x5F\x8F\x33\x96\x43\x0D\x87\x81\xD6\x19"
                             "\xA0\xF7\xF0\x5F\x08\x75\x87\x81\x67\xCF\x33\xC8\xFA\x38\xF8\x8D\x87\xB8\xE9\xED"
                             "\x47\x41\xC7\x19\xED\x20\x42\xD1\x0E\x56\x9F\x47\xD1\x02\x15\x03\x90\x81\x0E\x81"
                             "\xCD\x64\x08\x94\x0E\x51\x02\x1D\x03\x9E\x20\x45\xC1\x0E\x59\x02\x1D\x03\x91\xB3"
                             "\xE8\xFA\x3D\xC7\x42\xC3\x96\xCF\x69\x60\xCB\x0F\x68\xC8\xF3\x3A\x1C\xB3\xDC\x53"
                             "\x85\xF3\xD0\x4D\x33\x3F\x0F\x4B\x42\xBE\xCC\x1F\x0F\xB3\xC8\x61\xF6\xB3\x83\x0B"
                             "\x43\x34\x3E\x1F\x61\x9D\xDA\x15\xBE\x3B\xC7\xD9\xF8\x7D\x78\xF4\x3B\x2C\xFC\x3D"
                             "\x07\x7C\xFC\x9C\x2F\x9E\x82\x69\x9A\xAD\xA0\x99\xF0\xFB\x3E\x84\x42\x10\xE1\xA8"
                             "\xEE\x9F\x42\x1F\x47\xD0\x20\xE0\x5E\x19\x7C\x83\x0A\xFD\xE3\xE1\xF6\x6A\x3E\xCF"
                             "\xC3\xBA\x7D\x08\x77\x91\xE8\x26\x99\x82\x1D\x0F\x69\xF6\x7E\x4E\x17\xCF\x4D\x04"
                             "\x39\x63\x8F\xA1\x0F\xAF\x1F\x67\xE1\xE8\x76\x69\xF9\x38\x5F\x3D\x0E\xCF\x0C\xBE"
                             "\x3F\x0F\x43\xB0\x4D\x33\x04\x18\x47\xE4\xE1\x7C";
///////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////
const char HTTP_WIFI_CONF_U[] PROGMEM =
  //=HTTP_SCRIPT_WIFI
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}"
  "\0"
  //=HTTP_SCRIPT_ON_CLICK
  "<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%d%% (%d dBm)</span></div>"
  "\0"
  //=HTTP_SCRIPT_BR
  "<br>"
  "\0"
  //=HTTP_SCRIPT_SCAN
  "<div><a href='/wi?scan='>%s</a></div><br>"
  "\0"
  //=HTTP_FORM_WIFI
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>"
  "<form method='get' action='wi'>"
  "<p><b>%s</b> (%s)<br><input id='s1' placeholder=\"%s\" value=\"%s\"></p>"  // Need \" instead of ' to be able to use ' in text (#8489)
  "<p><label><b>%s</b><input type='checkbox' onclick='sp(\"p1\")'></label><br><input id='p1' type='password' placeholder=\"" D_AP1_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>%s</b> (%s)<br><input id='s2' placeholder=\"%s\" value=\"%s\"></p>"
  "<p><label><b>%s</b><input type='checkbox' onclick='sp(\"p2\")'></label><br><input id='p2' type='password' placeholder=\"" D_AP2_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>%s</b> (%s)<br><input id='h' placeholder=\"%s\" value=\"%s\"></p>"
  "<p><b>%s</b><input id='c' placeholder=\"%s\" value=\"%s\"></p>"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_SCRIPT_WIFI=0,
  HTTP_SCRIPT_ON_CLICK=75,
  HTTP_SCRIPT_BR=175,
  HTTP_SCRIPT_SCAN=180,
  HTTP_FORM_WIFI=222,
};

// Compressed from 827 to 405, -51.0%
const char HTTP_WIFI_CONF[] PROGMEM = "\x00\x34\x30\x2F\x83\xAD\xCE\x41\x9D\xD0\x8E\xF1\xED\x63\x8E\xE9\xF7\xE3\x90\xFB"
                             "\x3B\xC7\x42\x33\xB0\x85\xB3\xE0\x47\x4D\xEE\x7F\x2A\x2B\x66\xCA\x3F\xC7\xF8\x23"
                             "\xA5\x33\x65\x10\x79\xD4\xFA\x8F\x0C\x71\xDD\x3E\xC6\x1C\x87\xD9\xDE\x3A\x11\x8C"
                             "\x0C\x5F\x8E\xE9\xDE\x3C\x1E\xE9\xC2\xF9\xE8\x26\x99\x9F\x87\xA5\xA1\x5F\x66\x0F"
                             "\x87\xD9\xE4\x30\xFB\x59\xC1\x85\xA1\x9A\x1F\x0F\xB0\xCE\xED\x0A\xDF\x1D\xE3\xEC"
                             "\xFC\x3E\xBC\x7A\x1D\x96\x7E\x1F\xAE\x1D\xE1\x87\x83\xBA\x7D\x08\x77\x8F\xD7\x0E"
                             "\xF0\xC3\xD3\xC3\x2F\x90\x61\x5F\xBC\x7C\x3E\xCD\x47\xD9\xF8\x7D\x08\x7D\x1F\x48"
                             "\xEE\x9F\x42\x21\x08\x70\xD3\xBC\x7A\x1D\x9E\x19\x7C\x7E\x1E\x87\x60\x9A\x66\x7E"
                             "\x4E\x17\xCF\x41\xDF\x3F\x01\x2B\x45\x3B\x07\xF4\xED\x78\x3B\xE3\xE0\x21\xE7\x40"
                             "\x83\x83\x82\x0E\x0C\x63\x58\x42\x79\xD1\xF8\x7A\x04\xC7\x9F\x08\x7E\x1E\x83\x81"
                             "\x0B\x5A\x3E\xBC\x08\x31\x0F\x43\xB0\x71\xF8\x7A\x1D\x80\x87\x68\xC6\x7C\x6A\x1A"
                             "\xE8\x56\x08\x7C\x3E\xC7\x9D\x1F\x6A\xC3\xAD\xCE\x3E\x1F\x63\xFA\x7D\x9F\x87\xA0"
                             "\xC3\xF0\xF4\x1C\x7E\x1F\x5E\x3D\x0E\xC1\xC7\xE2\x3B\xA7\xD7\x8E\xF1\xE8\x3B\xE7"
                             "\xE1\xE9\xBC\x30\x5D\x2D\x10\xF8\x7D\xF8\xE4\x3E\xCE\xE1\x0C\x0A\xC3\x62\xB0\x21"
                             "\x1F\xCF\x87\xB0\xFA\xF1\xEC\x40\x83\xC5\x39\xEC\x3E\xBC\x7B\x0F\xC3\xD0\xEC\x18"
                             "\x7E\x1E\x83\x0F\xC3\xD0\x2B\x1C\xC2\x04\x5C\x9C\x10\xB1\xEA\x32\x18\xCF\x87\xD8"
                             "\x62\x98\x66\x83\xB0\xD8\x08\xDC\x4D\x78\x61\xDD\x3D\x83\x0E\x43\xD8\x77\x8F\xB3"
                             "\xF0\xF4\x3B\x08\x10\xF1\x70\x48\xD5\x06\x1C\x87\xD9\x02\x16\x34\x32\xFD\xE1\xFC"
                             "\xF8\x80\x95\xB2\x02\x26\xC4\x08\xBB\x2A\x01\x2C\x70\x48\x72\x88\x19\xAF\x04\x87"
                             "\x28\x81\x5F\x82\x43\x94\x40\xCA\x78\x24\x14\x0C\x07\x82\x30\x4A\xE0\x73\x44\x3E"
                             "\x1F\x61\x82\xEE\x2D\x38\x5F";
///////////////////////////////////////////////////////
// const char HTTP_SCRIPT_WIFI[] PROGMEM =
//   "function c(l){"
//     "eb('s1').value=l.innerText||l.textContent;"
//     "eb('p1').focus();"
//   "}";


///////////////////////////////////////////////////////
const char HTTP_UPGRADE_U[] PROGMEM =
  //=HTTP_FORM_UPG
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>"
  "<form method='get' action='u1'>"
  "<br><b>%s</b><br><input id='o' placeholder=\"OTA_URL\" value=\"%s\"><br>"
  "<br><button type='submit'>%s</button></form>"
  "</fieldset><br><br>"
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>"
  "\0"
  //=HTTP_FORM_RST_UPG
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br><input type='file' name='u2'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>%s %s</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' style='display:none;text-align:center;'><b>%s ...</b></div>"
  "\0"
  //=HTTP_FORM_RST
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_FORM_UPG=0,
  HTTP_FORM_RST_UPG=295,
  HTTP_FORM_RST=622,
};

// Compressed from 707 to 374, -47.1%
const char HTTP_UPGRADE[] PROGMEM = "\x00\x2D\x3D\x04\xD3\x35\xA2\x1F\x0F\xB3\x07\x21\xF6\x77\x0B\xD4\x64\x13\x3E\x1F"
                             "\x62\x6F\x86\x05\x66\x47\x98\xE0\xB0\x33\x43\xC1\xF6\x7E\x1E\x86\x35\x84\x27\x9D"
                             "\x1F\x87\xA0\x4C\x79\xF0\x87\xE1\xE8\x38\xFC\x3F\x5C\x3B\xC3\x0F\x07\xD7\x81\x06"
                             "\x21\xE8\x76\x0E\x3F\x0F\x43\xB0\x10\xED\x18\xCF\x8D\x43\x5D\x0A\xC1\x0F\x87\xD8"
                             "\xF3\xA3\xED\x58\x75\xB9\xC7\xC3\xEC\x59\xC8\x7D\x9F\x87\xA1\x0E\xF9\xF8\x7A\x0E"
                             "\x3F\x0F\xAF\x1E\x87\x60\xE0\x41\x93\xBC\x30\x5D\x2D\x10\xF8\x7D\xE1\xF6\x86\x05"
                             "\x61\xB1\x58\x10\x8F\xE7\xC3\xD8\x58\x54\x56\x7E\xC8\x59\x7C\x82\x3D\x88\xCE\xC2"
                             "\x16\xCF\x87\xB0\xFA\xF1\xEC\x04\x2C\x73\xE7\xE1\xE8\x38\x5D\x56\x72\xA3\x21\x8C"
                             "\xF8\x7D\xF8\x58\xE1\xBB\x47\xD8\x20\xE6\x62\xEA\xB3\x8F\xC3\xD0\xEC\x31\x9F\x1A"
                             "\x7E\x1E\x87\x60\x22\xF0\x38\x08\x78\xB8\x30\xAE\x0B\xA7\x0B\xE0\x93\xC1\x78\xCC"
                             "\xF0\x23\xF0\x60\x72\x8F\xB3\xB8\x4F\x83\x04\x1D\x8C\x68\xB0\xAB\x46\x5F\xE8\xEC"
                             "\x31\x9F\x1A\x75\x89\x75\x60\x85\xC2\x70\x20\xF0\x82\x08\x38\x11\x8D\x09\x9F\x6B"
                             "\xAC\x6B\x3E\x1F\x62\xCE\x51\x02\x1F\x0D\xE0\xA5\xC1\x7A\xCE\x0C\x2D\x0C\xD0\xF8"
                             "\x7D\xB1\xC7\x74\xF6\x18\x39\x0F\x61\xDE\x3A\x17\xA8\xC8\x26\x74\x04\x2E\x41\x0F"
                             "\x87\xB3\xB3\x99\xEC\x3C\x31\xC7\x74\xF6\x18\x39\x44\x09\xB6\x87\x05\x81\x9A\x1E"
                             "\xC3\xC5\x0A\xDF\x1D\x0C\x67\xC6\x9D\x01\x07\x34\x3B\xA7\x78\xF0\x7D\x9F\x87\xD7"
                             "\x90\x2C\xF1\x1A\x76\x09\xA6\x67\xE0\x22\xF3\x02\x72\x88\x13\x39\x81\xEC\xE6\x78"
                             "\xA6\x6C\xA3\xAE\xC2\xD1\xEE\x3C\xC3\x7D\x4F\xE7\x83\xEC\x10\x79\x3F\x47\x43\xA1"
                             "\xD0\x10\x78\xA6\x13\x4C\xCF\xC9\xC2\xF8\x32\xFE\x72\xA7\x0B\xE6";
///////////////////////////////////////////////////////

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
  // #include "./html_compressed/HTTP_SCRIPT_TEMPLATE.h"
#else
  #include "./html_uncompressed/HTTP_SCRIPT_MODULE_TEMPLATE.h"
  // #include "./html_uncompressed/HTTP_SCRIPT_TEMPLATE.h"
#endif



///////////////////////////////////////////////////////
const char HTTP_TEMPLATE_U[] PROGMEM =
  //=HTTP_SCRIPT_TEMPLATE
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}"
  "var c;"                                // Need a global for BASE
  "function x1(b){"
    "var i,j,g,k,o;"
    "o=b.responseText.split(/}1/);"       // Field separator
    "k=o.shift();"                        // Template name
    "if(eb('s1').value==''){"
      "eb('s1').value=k;"                 // Set NAME if not yet set
    "}"
    "g=o.shift().split(',');"             // GPIO - Array separator
    "os=\""                              // }2'0'>None (0)}3}2'17'>Button1 (17)}3...
  "\0"
  //=HTTP_SCRIPT_TEMPLATE2
  "j=0;"
  "for(i=0;i<%d;i++){"  // Supports 13 GPIOs
    "if(6==i){j=9;}"
    "if(8==i){j=12;}"
    "sk(g[i],j);"                       // Set GPIO
    "j++;"
  "}"
  "\0"
  //=HTTP_SCRIPT_TEMPLATE4
    "g=o.shift();"                        // FLAG
    "for(i=0;i<%d;i++){"
      "p=(g>>i)&1;"
      "eb('c'+i).checked=p;"              // Set FLAG checkboxes
    "}"
    "if(%d==c){"
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
    "os=\""                              // }2'0'>Sonoff Basic (1)}3...
  "\0"
  //=HTTP_SCRIPT_TEMPLATE5
    "\";"
    "sk(%d,99);"       // 17 = WEMOS
    "st(%d);"
  "}"
  "wl(sl);"
  "\0"
  //=HTTP_FORM_TEMPLATE
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='tp'>"
  "\0"
  //=HTTP_FORM_TEMPLATE6
  "<tr><td><b>%s</b></td><td style='width:200px'><input id='s1' placeholder='%s'></td></tr>"
  "<tr><td><b>%s</b></td><td><select id='g99' onchange='st(this.value)'></select></td></tr>"
  "</table>"
  "<hr/>"
  "\0"
  //=HTTP_FORM_TEMPLATE7
  "<tr><td><b><font color='#%06x'>%s%d</font></b></td><td%s><select id='g%d' onchange='ot(%d,this.value)'></select></td>"
  "\0"
  //=HTTP_FORM_TEMPLATE8
  "<td style='width:50px'><select id='h%d'></select></td></tr>"
  "\0"
  //=HTTP_FORM_TEMPLATE_FLAG
  "<p></p>"  // Keep close so do not use <br>
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend><p>"
  "</p></fieldset>"
  "\0"
  ;
///////////////////////////////////////////////////////
enum {
  HTTP_SCRIPT_TEMPLATE=0,
  HTTP_SCRIPT_TEMPLATE2=304,
  HTTP_SCRIPT_TEMPLATE4=372,
  HTTP_SCRIPT_TEMPLATE5=540,
  HTTP_FORM_TEMPLATE=568,
  HTTP_FORM_TEMPLATE6=646,
  HTTP_FORM_TEMPLATE7=836,
  HTTP_FORM_TEMPLATE8=954,
  HTTP_FORM_TEMPLATE_FLAG=1014,
};

// Compressed from 1088 to 745, -31.5%
const char HTTP_TEMPLATE[] PROGMEM = "\x00\x44\x30\x2F\x83\xAD\xCE\x41\x08\x77\x45\x9D\x46\x0E\xF1\xED\x33\xBF\xA3\x61"
                             "\xF3\x98\xFA\x23\x61\x0D\x20\x88\x55\x50\xC2\xFB\x35\x0B\x7E\xA3\xBA\x77\x8F\x06"
                             "\xC3\xA6\x77\xDD\x88\x65\xEA\xBA\x61\x8A\xBE\x1E\x67\xC0\x43\xC7\x4E\xE9\xDE\x3D"
                             "\xBA\x60\xEE\xD0\xAD\xF1\xD3\xEE\xC4\x32\x2F\x55\xD3\x3E\x1F\x0E\x61\xFA\x3F\x45"
                             "\x42\xB7\xC7\x4F\x55\xD0\xBF\x1F\x0F\x87\x29\xB3\xBC\x7B\x48\x10\x70\x43\xBC\x78"
                             "\x3D\xC7\xB8\xF0\x6C\x3A\x60\xC7\xC7\x74\xFB\x21\xE2\x65\x47\xD9\xD4\x2C\xEA\xAF"
                             "\x8B\x67\x78\xF0\x6C\x3A\x79\xF0\x87\x74\xEF\x1E\x0F\x71\x9D\xFD\x06\x78\x04\x4E"
                             "\x2A\x01\x4D\x87\x21\xDD\x21\xC0\x83\xBF\xE9\xD4\x6B\x3A\x87\x8E\xA3\x43\xAB\x0F"
                             "\x18\x7C\x1C\x74\xFB\xF0\xCC\xEF\x32\xA6\x6C\xA3\xA7\x86\x05\xB4\x77\x4E\xC3\xDC"
                             "\x72\x1D\x87\x78\xF0\x46\x87\xCC\x3A\x78\x56\x98\xA3\xBA\x77\x8F\x1A\x60\xEE\xB1"
                             "\xC7\x74\xFB\xF1\xC8\x7D\x9D\xE3\xA1\x19\xD8\x42\xD9\xF0\xF8\x7D\x9F\x67\x78\xF6"
                             "\x82\x55\x03\x43\xC1\xEE\x1E\x04\x5C\x44\x10\xB2\x93\xEC\xEA\x3E\xCE\xF1\xE3\x3C"
                             "\x7C\x3D\x93\x85\xF3\x59\xF0\xE3\x3C\x11\x8C\xF9\xDD\xD3\xE1\xC6\x78\x2D\x3D\x0F"
                             "\xA1\x0F\x1A\x76\xCE\xD9\xDE\x3D\xBA\x60\xEE\x9C\xE3\xE1\xF0\xB4\xEF\x1E\xD3\x59"
                             "\xF0\xE2\x3C\x1E\xE2\xD3\x07\x74\xE7\xC8\x10\xA5\x1C\x94\x78\x3D\xC5\xE3\x43\xBA"
                             "\x3C\x7B\xDA\x7B\xE7\x51\xAC\xEF\x1E\x0D\x67\x6C\xED\x9E\x0F\x74\xE1\x7C\x11\xB3"
                             "\xC0\x4E\xCA\x06\x1F\x0E\xE8\xF1\xF8\x7E\x69\xDE\x3F\x47\x21\xE0\x98\xE3\xBA\x7D"
                             "\x86\x7D\x9D\xBD\x3B\xC7\x40\xC5\x30\xCD\x18\x87\xC1\x87\x83\xDD\xA6\x0E\xE9\xF4"
                             "\x21\xF0\xF8\x19\xDE\x3D\xA0\x8F\x92\x31\xC7\x74\xFB\x1E\x38\x91\x02\x27\x04\x63"
                             "\xC7\x83\xDC\x7B\x81\x13\x89\x3F\x51\xDD\xA3\xBC\x7B\x43\x3E\x51\xE0\xCE\xFE\xAC"
                             "\xF8\x7D\xD0\xC3\xB5\x47\xC3\xEC\xED\xD1\xE0\x21\x0E\xED\x9D\x46\xC3\x90\xEF\x1E"
                             "\x08\x11\xB0\x90\x8E\xE9\xDE\x3D\xB9\xE3\xE1\xEC\x9C\x2F\x9E\xC3\xC7\x8D\x0E\xE9"
                             "\xF4\x21\xD4\x71\x23\xBC\x78\x2F\x51\xDD\x3E\x84\x3B\xC7\x83\xDC\x3E\x11\xDD\xF0"
                             "\x47\x78\xF1\x38\x5F\x3D\x0C\x6B\x08\x4F\x3A\x3F\x0F\x40\x98\xF3\xE1\x0F\xC3\xD0"
                             "\x71\xF8\x7E\xB8\x77\x86\x1E\x01\x06\x11\xE8\x76\x0E\x3F\x0F\x43\xB0\x10\xEC\x18"
                             "\xCF\x8D\x43\x5D\x0A\xC1\x0F\x87\xD8\xF3\xA3\xED\x58\x75\xB9\xC7\xC3\xEE\x86\x1F"
                             "\x67\xE4\xE1\x7C\xF4\xAF\x9F\x87\xA5\x08\x7E\x1E\x83\x8F\xC3\xEB\xC0\x83\x8B\x50"
                             "\x87\xE1\xE9\x42\x2F\x51\x90\x4C\xF8\x7D\x8F\xE8\x94\x28\xF3\x39\x4D\x90\xC3\x61"
                             "\xF6\x7E\x1E\x9B\xC3\x05\xD2\xD1\x0F\x87\xDF\x8E\x43\xEC\xEE\x10\xC0\xAC\x36\x2B"
                             "\x02\x11\xFC\xF8\x7D\x9F\x5E\x3E\xC1\x0B\x19\x3B\x2B\xE7\xE0\x2A\x2B\x66\x87\xE1"
                             "\xE9\xE6\x13\x0C\x10\x72\x11\xE3\x89\x1F\x67\x70\xB3\x81\x0B\xA0\x5A\x7D\xFA\x81"
                             "\x07\xA0\x46\x67\x61\x0B\x67\x78\xFB\x3F\x0F\x43\xB0\x10\x70\x80\x47\xCD\x0E\xCA"
                             "\xB1\xC1\x33\xF0\xF4\x15\xF3\xB0\x12\x38\x1A\x3D\x0C\x67\x52\x0F\x02\xCF\x9F\x0F"
                             "\xB3\xC8\xFA\x38\xF8\x8D\x87\xD9\xF8\x7D\x78\xFA\x10\xF4\x3B\x0C\x67\x51\xF8\x08"
                             "\xFA\xF9\xF5\xE0\x49\xD8\x4F\xA1\x01\x1F\x61\xCA\x3B\xA7\xD0\x87\x50\x2B\x6C\x93"
                             "\x85\xF0\x4C\xE1\xE8\xE6\x90\x20\xF0\xF2\x08\xD9\x80\xA3\xE8\x40\x50\xE0\x6E\x70"
                             "\xBE\x7A\x0C\x3F\x0F\x43\xB0\x61\xF8\x0A\xDC\x90\x9F\x5E\x04\xFE\x48\xC1\x0B\x13"
                             "\x3B\x01\x13\x14\x9C\x2F\x9B";
///////////////////////////////////////////////////////


// const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
//     "j=0;"
//     "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"  // Supports 13 GPIOs
//       "if(6==i){j=9;}"
//       "if(8==i){j=12;}"
//       "sk(g[i],j);"                       // Set GPIO
//       "j++;"
//     "}";
const char HTTP_SCRIPT_TEMPLATE3[] PROGMEM =
    "\";"
    "sk(g[13]," STR(ADC0_PIN) ");";       // Set ADC0

// const char HTTP_SCRIPT_TEMPLATE4[] PROGMEM =
//     "g=o.shift();"                        // FLAG
//     "for(i=0;i<" STR(GPIO_FLAG_USED) ";i++){"
//       "p=(g>>i)&1;"
//       "eb('c'+i).checked=p;"              // Set FLAG checkboxes
//     "}"
//     "if(" STR(USER_MODULE) "==c){"
//       "g=o.shift();"
//       "eb('g99').value=g;"                // Set BASE for initial select
//     "}"
//   "}"
//   "function st(t){"
//     "c=t;"                                // Needed for initial BASE select
//     "var a='tp?t='+t;"
//     "ld(a,x1);"                           // ?t related to WebGetArg("t", stemp, sizeof(stemp));
//   "}"
//   "function sl(){"
//     "os=\"";                              // }2'0'>Sonoff Basic (1)}3...
// const char HTTP_SCRIPT_TEMPLATE5[] PROGMEM =
//     "\";"
//     "sk(" STR(WEMOS_MODULE) ",99);"       // 17 = WEMOS
//     "st(" STR(USER_MODULE) ");"
//   "}"
//   "wl(sl);";

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
  // #include "./html_compressed/HTTP_HEAD_LAST_SCRIPT.h"
  // #include "./html_compressed/HTTP_HEAD_STYLE1.h"
  // #include "./html_compressed/HTTP_HEAD_STYLE2.h"
#else
  // #include "./html_uncompressed/HTTP_HEAD_LAST_SCRIPT.h"
  // #include "./html_uncompressed/HTTP_HEAD_STYLE1.h"
  // #include "./html_uncompressed/HTTP_HEAD_STYLE2.h"
#endif


#ifdef USE_ZIGBEE
// Styles used for Zigbee Web UI
// Battery icon from https://css.gg/battery
//
  #ifdef USE_UNISHOX_COMPRESSION
    #include "./html_compressed/HTTP_HEAD_STYLE_ZIGBEE.h"
  #else
    #include "./html_uncompressed/HTTP_HEAD_STYLE_ZIGBEE.h"
  #endif
#endif // USE_ZIGBEE

// const char HTTP_HEAD_STYLE_SSI[] PROGMEM =
//   // Signal Strength Indicator
//   ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0}"
//   ".si i{width:3px;margin-right:1px;border-radius:3px;background-color:#%06x}"
//   ".si .b0{height:25%%}.si .b1{height:50%%}.si .b2{height:75%%}.si .b3{height:100%%}.o30{opacity:.3}";

const char HTTP_HEAD_STYLE3_MINIMAL[] PROGMEM =
  "</style>"

  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;'>"  // COLOR_TEXT
  "<div style='text-align:center;color:#%06x;'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"  // COLOR_TEXT_WARNING
  "<div style='text-align:center;color:#%06x;'><noscript>%s<br></noscript>" // COLOR_TITLE
  "<h3>%s %s</h3>"
  "<h2>%s</h2>";

// const char HTTP_HEAD_STYLE3[] PROGMEM =
//   "</style>"
//   "</head>"
//   "<body>"
//   "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;'>"  // COLOR_TEXT
//   "<div style='text-align:center;color:#%06x;'><noscript>%s<br></noscript>" // COLOR_TITLE
//   "<h3>%s %s</h3>"
//   "<h2>%s</h2>";

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

// const char HTTP_FORM_TEMPLATE[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" D_TEMPLATE_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='get' action='tp'>";
// const char HTTP_FORM_TEMPLATE_FLAG[] PROGMEM =
//   "<p></p>"  // Keep close so do not use <br>
//   "<fieldset><legend><b>&nbsp;" D_TEMPLATE_FLAGS "&nbsp;</b></legend><p>"
// //  "<label><input id='c0' name='c0' type='checkbox'><b>" D_OPTION_TEXT "</b></label><br>"
//   "</p></fieldset>";

const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='md'>"
  "<p></p><b>" D_MODULE_TYPE "</b> (%s)<br><select id='g99'></select><br>"
  "<br><table>";

// const char HTTP_FORM_WIFI[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='get' action='wi'>"
//   "<p><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br><input id='s1' placeholder=\"" STA_SSID1 "\" value=\"%s\"></p>"  // Need \" instead of ' to be able to use ' in text (#8489)
//   "<p><label><b>" D_AP1_PASSWORD "</b><input type='checkbox' onclick='sp(\"p1\")'></label><br><input id='p1' type='password' placeholder=\"" D_AP1_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
//   "<p><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br><input id='s2' placeholder=\"" STA_SSID2 "\" value=\"%s\"></p>"
//   "<p><label><b>" D_AP2_PASSWORD "</b><input type='checkbox' onclick='sp(\"p2\")'></label><br><input id='p2' type='password' placeholder=\"" D_AP2_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
//   "<p><b>" D_HOSTNAME "</b> (%s)<br><input id='h' placeholder=\"%s\" value=\"%s\"></p>"
//   "<p><b>" D_CORS_DOMAIN "</b><input id='c' placeholder=\"" CORS_DOMAIN "\" value=\"%s\"></p>";

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
  "<label><input id='b1' type='checkbox'%s><b>" D_MQTT_ENABLE "</b></label><br>"
  "<br>"
  "<label><b>" D_DEVICE_NAME "</b> (%s)</label><br><input id='dn' placeholder=\"\" value=\"%s\"><br>"
  "<br>";

const char HTTP_FORM_END[] PROGMEM =
  "<br>"
  "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";

// const char HTTP_FORM_RST[] PROGMEM =
//   "<div id='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_RESTORE_CONFIGURATION "&nbsp;</b></legend>";
// const char HTTP_FORM_UPG[] PROGMEM =
//   "<div id='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
//   "<form method='get' action='u1'>"
//   "<br><b>" D_OTA_URL "</b><br><input id='o' placeholder=\"OTA_URL\" value=\"%s\"><br>"
//   "<br><button type='submit'>" D_START_UPGRADE "</button></form>"
//   "</fieldset><br><br>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
// const char HTTP_FORM_RST_UPG[] PROGMEM =
//   "<form method='post' action='u2' enctype='multipart/form-data'>"
//   "<br><input type='file' name='u2'><br>"
//   "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
//   "</fieldset>"
//   "</div>"
//   "<div id='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";

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
  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "|" D_UPLOAD_ERR_5 "|" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9;

const uint16_t DNS_PORT = 53;
enum HttpOptions {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER, HTTP_MANAGER_RESET_ONLY};

DNSServer *DnsServer;
ESP8266WebServer *Webserver;

struct WEB {
  String chunk_buffer = "";                         // Could be max 2 * CHUNKED_BUFFER_SIZE
  uint16_t upload_progress_dot_count;
  uint16_t upload_error = 0;
  uint8_t state = HTTP_OFF;
  uint8_t upload_file_type;
  uint8_t config_block_count = 0;
  uint8_t config_xor_on = 0;
  uint8_t config_xor_on_set = CONFIG_FILE_XOR;
  bool reset_web_log_flag = false;                  // Reset web console log
} Web;

// Helper function to avoid code duplication (saves 4k Flash)
static void WebGetArg(const char* arg, char* out, size_t max)
{
  String s = Webserver->arg(arg);
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
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("SRC: %s from %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource), Webserver->client().remoteIP().toString().c_str());
  }
}

void ExecuteWebCommand(char* svalue, uint32_t source)
{
  ShowWebSource(source);
  TasmotaGlobal.last_source = source;
  ExecuteCommand(svalue, SRC_IGNORE);
}

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
  { "in", HTTP_ANY, HandleInformation },
#endif  // Not FIRMWARE_MINIMAL
};

void WebServer_on(const char * prefix, void (*func)(void), uint8_t method = HTTP_ANY) {
#ifdef ESP8266
  Webserver->on((const __FlashStringHelper *) prefix, (HTTPMethod) method, func);
#endif  // ESP8266
#ifdef ESP32
  Webserver->on(prefix, (HTTPMethod) method, func);
#endif  // ESP32
}

void StartWebserver(int type, IPAddress ipweb)
{
  if (!Settings.web_refresh) { Settings.web_refresh = HTTP_REFRESH_TIME; }
  if (!Web.state) {
    if (!Webserver) {
      Webserver = new ESP8266WebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
      // call `Webserver->on()` on each entry
      for (uint32_t i=0; i<ARRAY_SIZE(WebServerDispatch); i++) {
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
      Webserver->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);  // this call requires 2 functions so we keep a direct call
#ifndef FIRMWARE_MINIMAL
      XdrvCall(FUNC_WEB_ADD_HANDLER);
      XsnsCall(FUNC_WEB_ADD_HANDLER);
#endif  // Not FIRMWARE_MINIMAL
    }
    Web.reset_web_log_flag = false;

    Webserver->begin(); // Web server start
  }
  if (Web.state != type) {
#if LWIP_IPV6
    String ipv6_addr = WifiGetIPv6();
    if (ipv6_addr!="") {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s and IPv6 global address %s "),
        NetworkHostname(), (Mdns.begun) ? ".local" : "", ipweb.toString().c_str(), ipv6_addr.c_str());
    } else {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"),
        NetworkHostname(), (Mdns.begun) ? ".local" : "", ipweb.toString().c_str());
    }
#else
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s%s " D_WITH_IP_ADDRESS " %s"),
      NetworkHostname(), (Mdns.begun) ? ".local" : "", ipweb.toString().c_str());
#endif // LWIP_IPV6 = 1
    TasmotaGlobal.rules_flag.http_init = 1;
  }
  if (type) { Web.state = type; }
}

void StopWebserver(void)
{
  if (Web.state) {
    Webserver->close();
    Web.state = HTTP_OFF;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
  }
}

void WifiManagerBegin(bool reset_only)
{
  // setup AP
  if (!TasmotaGlobal.global_state.wifi_down) {
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

  // bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
  WiFi.softAP(TasmotaGlobal.hostname, WIFI_AP_PASSPHRASE, channel, 0, 1);
  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  StartWebserver((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER), WiFi.softAPIP());
}

void PollDnsWebserver(void)
{
  if (DnsServer) { DnsServer->processNextRequest(); }
  if (Webserver) { Webserver->handleClient(); }
}

/*********************************************************************************************/

bool WebAuthenticate(void)
{
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return Webserver->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
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
    Webserver->requestAuthentication();
    return false;
  }
  return true;
}

void HttpHeaderCors(void)
{
  if (strlen(SettingsText(SET_CORS))) {
    Webserver->sendHeader(F("Access-Control-Allow-Origin"), SettingsText(SET_CORS));
  }
}

void WSHeaderSend(void)
{
  Webserver->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  Webserver->sendHeader(F("Pragma"), F("no-cache"));
  Webserver->sendHeader(F("Expires"), F("-1"));
  HttpHeaderCors();
}

/**********************************************************************************************
* HTTP Content Page handler
**********************************************************************************************/

void WSSend(int code, int ctype, const String& content)
{
  char ct[25];  // strlen("application/octet-stream") +1 = Longest Content type string
  Webserver->send(code, GetTextIndexed(ct, sizeof(ct), ctype, kContentTypes), content);
}

/**********************************************************************************************
* HTTP Content Chunk handler
**********************************************************************************************/

void WSContentBegin(int code, int ctype)
{
  Webserver->client().flush();
  WSHeaderSend();
  Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN);
  WSSend(code, ctype, "");                        // Signal start of chunked content
  Web.chunk_buffer = "";
}

void _WSContentSend(const String& content)        // Low level sendContent for all core versions
{
  size_t len = content.length();
  Webserver->sendContent(content);

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("WSContentSend"));
#endif
  DEBUG_CORE_LOG(PSTR("WEB: Chunk size %d/%d"), len, sizeof(TasmotaGlobal.mqtt_data));
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
  int len = strlen(TasmotaGlobal.mqtt_data);

  if (0 == len) {                                  // No content
    return;
  }
  else if (len == sizeof(TasmotaGlobal.mqtt_data)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: Content too large"));
  }
  else if (len < CHUNKED_BUFFER_SIZE) {            // Append chunk buffer with small content
    Web.chunk_buffer += TasmotaGlobal.mqtt_data;
    len = Web.chunk_buffer.length();
  }

  if (len >= CHUNKED_BUFFER_SIZE) {                // Either content or chunk buffer is oversize
    WSContentFlush();                              // Send chunk buffer before possible content oversize
  }
  if (strlen(TasmotaGlobal.mqtt_data) >= CHUNKED_BUFFER_SIZE) {  // Content is oversize
    _WSContentSend(TasmotaGlobal.mqtt_data);                     // Send content
  }
}

void WSContentSend_P(const char* formatP, ...)     // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
  va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
    TasmotaGlobal.mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(PSTR("ERROR: WSContentSend_P size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
  }
#endif

  _WSContentSendBuffer();
}

void WSContentSend_PD(const char* formatP, ...)    // Content send snprintf_P char data checked for decimal separator
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, formatP);
  int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
  va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
    TasmotaGlobal.mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(PSTR("ERROR: WSContentSend_PD size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
  }
#endif

  if (D_DECIMAL_SEPARATOR[0] != '.') {
    for (uint32_t i = 0; i < len; i++) {
      if ('.' == TasmotaGlobal.mqtt_data[i]) {
        TasmotaGlobal.mqtt_data[i] = D_DECIMAL_SEPARATOR[0];
      }
    }
  }

  _WSContentSendBuffer();
}

void WSContentStart_P(const char* title, bool auth)
{
  if (auth && strlen(SettingsText(SET_WEBPWD)) && !Webserver->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD))) {
    return Webserver->requestAuthentication();
  }

  WSContentBegin(200, CT_HTML);

  if (title != nullptr) {
    WSContentSend_P(HTTP_HEADER1, D_HTML_LANGUAGE, SettingsText(SET_DEVICENAME), title);
  }
}

void WSContentStart_P(const char* title)
{
  WSContentStart_P(title, true);
}

void WSContentSendStyle_P(const char* formatP, ...)
{
  UnishoxStrings msg(HTTP_SEND_STYLE);
  if (WifiIsInManagerMode()) {
    if (WifiConfigCounter()) {
      WSContentSend_P(msg[HTTP_SCRIPT_COUNTER], PSTR(D_RESTART_IN), PSTR(D_SECONDS));
    }
  }
  WSContentSend_P(msg[HTTP_HEAD_LAST_SCRIPT]);

  WSContentSend_P(msg[HTTP_HEAD_STYLE1], WebColor(COL_FORM), WebColor(COL_INPUT), WebColor(COL_INPUT_TEXT), WebColor(COL_INPUT),
                  WebColor(COL_INPUT_TEXT), WebColor(COL_CONSOLE), WebColor(COL_CONSOLE_TEXT), WebColor(COL_BACKGROUND));
  WSContentSend_P(msg[HTTP_HEAD_STYLE2], WebColor(COL_BUTTON), WebColor(COL_BUTTON_TEXT), WebColor(COL_BUTTON_HOVER),
                  WebColor(COL_BUTTON_RESET), WebColor(COL_BUTTON_RESET_HOVER), WebColor(COL_BUTTON_SAVE), WebColor(COL_BUTTON_SAVE_HOVER),
                  WebColor(COL_BUTTON));
#ifdef USE_ZIGBEE
  {
    UnishoxStrings msg_z(HTTP_SEND_STYLE_ZIGBEE);
    WSContentSend_P(msg_z[HTTP_HEAD_STYLE_ZIGBEE]);
  }
#endif // USE_ZIGBEE
  if (formatP != nullptr) {
    // This uses char strings. Be aware of sending %% if % is needed
    va_list arg;
    va_start(arg, formatP);
    int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
    va_end(arg);

#ifdef DEBUG_TASMOTA_CORE
  if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
    TasmotaGlobal.mqtt_data[33] = '\0';
    DEBUG_CORE_LOG(msg[HTTP_HEAD_ERROR_TOO_LONG], len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
  }
#endif

    _WSContentSendBuffer();
  }
  WSContentSend_P(
#ifdef FIRMWARE_MINIMAL
    HTTP_HEAD_STYLE3_MINIMAL, WebColor(COL_TEXT),
    WebColor(COL_TEXT_WARNING),
#else
    msg[HTTP_HEAD_STYLE3], WebColor(COL_TEXT),
#endif
    WebColor(COL_TITLE), PSTR(D_NOSCRIPT),
#ifdef LANGUAGE_MODULE_NAME
    PSTR(D_MODULE), ModuleName().c_str(),
#else // LANGUAGE_MODULE_NAME
    ModuleName().c_str(), PSTR(D_MODULE),
#endif // LANGUAGE_MODULE_NAME
    SettingsText(SET_DEVICENAME));

  if (Settings.flag3.gui_hostname_ip) {                // SetOption53 - Show hostanme and IP address in GUI main menu
    bool lip = (static_cast<uint32_t>(WiFi.localIP()) != 0);
    bool sip = (static_cast<uint32_t>(WiFi.softAPIP()) != 0);
    WSContentSend_P(PSTR("<h4>%s%s (%s%s%s)</h4>"),    // tasmota.local (192.168.2.12, 192.168.4.1)
      NetworkHostname(),
      (Mdns.begun) ? ".local" : "",
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
  Webserver->client().stop();
}

void WSContentStop(void)
{
  if (WifiIsInManagerMode()) {
    if (WifiConfigCounter()) {
      WSContentSend_P(HTTP_COUNTER);
    }
  }
  WSContentSend_P(HTTP_END, TasmotaGlobal.version);
  WSContentEnd();
}

/*********************************************************************************************/

void WebRestart(uint32_t type)
{
  // type 0 = restart
  // type 1 = restart after config change
  // type 2 = restart after config change with possible ip address change too
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESTART));

  bool reset_only = (HTTP_MANAGER_RESET_ONLY == Web.state);

  WSContentStart_P((type) ? PSTR(D_SAVE_CONFIGURATION) : PSTR(D_RESTART), !reset_only);
  WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_RESTART_RECONNECT_TIME);
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
  TasmotaGlobal.restart_flag = 2;
}

/*********************************************************************************************/

void HandleWifiLogin(void)
{
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

#ifdef USE_LIGHT
void WebSliderColdWarm(void)
{
  WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
    "a",             // a - Unique HTML id
    "#eff", "#f81",  // 6500k in RGB (White) to 2500k in RGB (Warm Yellow)
    1,               // sl1
    153, 500,        // Range color temperature
    LightGetColorTemp(),
    't', 0);         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));
}
#endif  // USE_LIGHT

void HandleRoot(void)
{
  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the page.

  if (Webserver->hasArg("rst")) {
    WebRestart(0);
    return;
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    if (strlen(SettingsText(SET_WEBPWD)) && !(Webserver->hasArg("USER1")) && !(Webserver->hasArg("PASS1")) && HTTP_MANAGER_RESET_ONLY != Web.state) {
      HandleWifiLogin();
    } else {
      if (!strlen(SettingsText(SET_WEBPWD)) || (((Webserver->arg("USER1") == WEB_USERNAME ) && (Webserver->arg("PASS1") == SettingsText(SET_WEBPWD) )) || HTTP_MANAGER_RESET_ONLY == Web.state)) {
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

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MAIN_MENU));

  char stemp[33];

  WSContentStart_P(PSTR(D_MAIN_MENU));
#ifdef USE_SCRIPT_WEB_DISPLAY
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings.web_refresh, Settings.web_refresh);
#else
  WSContentSend_P(HTTP_SCRIPT_ROOT, Settings.web_refresh);
#endif
  WSContentSend_P(HTTP_SCRIPT_ROOT_PART2);

  WSContentSendStyle();

  WSContentSend_P(PSTR("<div id='l1' name='l1'></div>"));
  if (TasmotaGlobal.devices_present) {
#ifdef USE_LIGHT
    if (TasmotaGlobal.light_type) {
      uint8_t light_subtype = TasmotaGlobal.light_type &7;
      if (!Settings.flag3.pwm_multi_channels) {  // SetOption68 0 - Enable multi-channels PWM instead of Color PWM
        bool split_white = ((LST_RGBW <= light_subtype) && (TasmotaGlobal.devices_present > 1));  // Only on RGBW or RGBCW and SetOption37 128

        if ((LST_COLDWARM == light_subtype) || ((LST_RGBCW == light_subtype) && !split_white)) {
          WebSliderColdWarm();
        }

        if (light_subtype > 2) {  // No W or CW
          uint16_t hue;
          uint8_t sat;
          LightGetHSB(&hue, &sat, nullptr);

          WSContentSend_P(HTTP_MSG_SLIDER_GRADIENT,  // Hue
            "b",             // b - Unique HTML id
            "#800", PSTR("#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800"),  // Hue colors
            2,               // sl2 - Unique range HTML id - Used as source for Saturation end color
            1, 359,          // Range valid Hue
            hue,
            'h', 0);         // h0 - Value id

          uint8_t dcolor = changeUIntScale(Settings.light_dimmer, 0, 100, 0, 255);
          char scolor[8];
          snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
          uint8_t red, green, blue;
          HsToRgb(hue, 255, &red, &green, &blue);
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
          Settings.flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100% (SetOption77 - Do not power off if slider moved to far left)
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
            Settings.flag3.slider_dimmer_stay_on, 100,  // Range 0/1 to 100% (SetOption77 - Do not power off if slider moved to far left)
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
      for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
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
      for (uint32_t idx = 1; idx <= TasmotaGlobal.devices_present; idx++) {
        bool set_button = ((idx <= MAX_BUTTON_TEXT) && strlen(SettingsText(SET_BUTTON1 + idx -1)));
#ifdef USE_SHUTTER
        int32_t ShutterWebButton;
        if (ShutterWebButton = IsShutterWebButton(idx)) {
          WSContentSend_P(HTTP_DEVICE_CONTROL, 100 / TasmotaGlobal.devices_present, idx,
            (set_button) ? SettingsText(SET_BUTTON1 + idx -1) : ((Settings.shutter_options[abs(ShutterWebButton)-1] & 2) /* is locked */ ? "-" : ((Settings.shutter_options[abs(ShutterWebButton)-1] & 8) /* invert web buttons */ ? ((ShutterWebButton>0) ? "&#9660;" : "&#9650;") : ((ShutterWebButton>0) ? "&#9650;" : "&#9660;"))),
            "");
          continue;
        }
#endif  // USE_SHUTTER
        snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
        WSContentSend_P(HTTP_DEVICE_CONTROL, 100 / TasmotaGlobal.devices_present, idx,
          (set_button) ? SettingsText(SET_BUTTON1 + idx -1) : (TasmotaGlobal.devices_present < 5) ? D_BUTTON_TOGGLE : "",
          (set_button) ? "" : (TasmotaGlobal.devices_present > 1) ? stemp : "");
      }
#ifdef USE_SONOFF_IFAN
    }
#endif  // USE_SONOFF_IFAN
    WSContentSend_P(PSTR("</tr></table>"));
  }
#ifdef USE_TUYA_MCU
  if (IsModuleTuya()) {
    if (AsModuleTuyaMS()) {
      WSContentSend_P(HTTP_TABLE100);
      WSContentSend_P(PSTR("<tr><div></div>"));
      snprintf_P(stemp, sizeof(stemp), PSTR("" D_JSON_IRHVAC_MODE ""));
      WSContentSend_P(HTTP_DEVICE_CONTROL, 26, TasmotaGlobal.devices_present + 1,
        (strlen(SettingsText(SET_BUTTON1 + TasmotaGlobal.devices_present))) ? SettingsText(SET_BUTTON1 + TasmotaGlobal.devices_present) : stemp, "");
      WSContentSend_P(PSTR("</tr></table>"));
    }
  }
#endif  // USE_TUYA_MCU
#ifdef USE_SONOFF_RF
  if (SONOFF_BRIDGE == TasmotaGlobal.module_type) {
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
    Webserver->requestAuthentication();
    return true;
  }

  if (!Webserver->hasArg("m")) {     // Status refresh requested
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
#ifdef USE_TUYA_MCU
    if (IsModuleTuya()) {
      if (device <= TasmotaGlobal.devices_present) {
        ExecuteCommandPower(device, POWER_TOGGLE, SRC_IGNORE);
      } else {
        if (AsModuleTuyaMS() && device == TasmotaGlobal.devices_present + 1) {
          uint8_t dpId = TuyaGetDpId(TUYA_MCU_FUNC_MODESET);
          snprintf_P(svalue, sizeof(svalue), PSTR("Tuyasend4 %d,%d"), dpId, !TuyaModeSet());
          ExecuteCommand(svalue, SRC_WEBGUI);
        }
      }
    } else {
#endif  // USE_TUYA_MCU
#ifdef USE_SHUTTER
      int32_t ShutterWebButton;
      if (ShutterWebButton = IsShutterWebButton(device)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("ShutterPosition%d %s"), abs(ShutterWebButton), (ShutterWebButton>0) ? PSTR(D_CMND_SHUTTER_STOPOPEN) : PSTR(D_CMND_SHUTTER_STOPCLOSE));
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
#ifdef USE_TUYA_MCU
    }
#endif  // USE_TUYA_MCU
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
  uint32_t pwm_channels = (TasmotaGlobal.light_type & 7) > LST_MAX ? LST_MAX : (TasmotaGlobal.light_type & 7);
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
  for (uint32_t j = 1; j <= TasmotaGlobal.shutters_present; j++) {
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
#ifdef USE_ZIGBEE
  WebGetArg("zbj", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("ZbPermitJoin"));
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
  WebGetArg("zbr", tmp, sizeof(tmp));
  if (strlen(tmp)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("ZbMap"));
    ExecuteWebCommand(svalue, SRC_WEBGUI);
  }
#endif // USE_ZIGBEE
  WSContentBegin(200, CT_HTML);
  WSContentSend_P(PSTR("{t}"));
  XsnsCall(FUNC_WEB_SENSOR);
  XdrvCall(FUNC_WEB_SENSOR);

  WSContentSend_P(PSTR("</table>"));

  if (TasmotaGlobal.devices_present) {
    WSContentSend_P(PSTR("{t}<tr>"));
    uint32_t fsize = (TasmotaGlobal.devices_present < 5) ? 70 - (TasmotaGlobal.devices_present * 8) : 32;
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) {
      WSContentSend_P(HTTP_DEVICE_STATE, 36, (bitRead(TasmotaGlobal.power, 0)) ? "bold" : "normal", 54, GetStateText(bitRead(TasmotaGlobal.power, 0)));
      uint32_t fanspeed = GetFanspeed();
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fanspeed);
      WSContentSend_P(HTTP_DEVICE_STATE, 64, (fanspeed) ? "bold" : "normal", 54, (fanspeed) ? svalue : GetStateText(0));
    } else {
#endif  // USE_SONOFF_IFAN
      for (uint32_t idx = 1; idx <= TasmotaGlobal.devices_present; idx++) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), bitRead(TasmotaGlobal.power, idx -1));
        WSContentSend_P(HTTP_DEVICE_STATE, 100 / TasmotaGlobal.devices_present, (bitRead(TasmotaGlobal.power, idx -1)) ? "bold" : "normal", fsize, (TasmotaGlobal.devices_present < 5) ? GetStateText(bitRead(TasmotaGlobal.power, idx -1)) : svalue);
      }
#ifdef USE_SONOFF_IFAN
    }
#endif  // USE_SONOFF_IFAN

    WSContentSend_P(PSTR("</tr></table>"));
  }
#ifdef USE_TUYA_MCU
  if (IsModuleTuya()) {
    uint32_t modeset = TuyaModeSet();
    if (AsModuleTuyaMS()) {
      WSContentSend_P(PSTR("<div style='text-align:center;font-size:25px;'>" D_JSON_IRHVAC_MODE ": %d</div>"), modeset);
    }
  }
#endif  // USE_TUYA_MCU
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

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURATION));

  WSContentStart_P(PSTR(D_CONFIGURATION));
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

void WSContentSendNiceLists(uint32_t option) {
  char stemp[30];                                             // Template number and Sensor name
  for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {  // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...
    if (option && (1 == i)) {
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), D_SENSOR_USER);  // }2'255'>User}3
    }
    uint32_t ridx = pgm_read_word(kGpioNiceList + i) & 0xFFE0;
    uint32_t midx = BGPIO(ridx);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
  }
  WSContentSend_P(PSTR("\";"));

  WSContentSend_P(PSTR("hs=["));
  uint32_t midx;
  bool first_done = false;
  for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {  // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
    midx = pgm_read_word(kGpioNiceList + i);
    if (midx & 0x001F) {
      if (first_done) { WSContentSend_P(PSTR(",")); }
      WSContentSend_P(PSTR("%d"), midx);
      first_done = true;
    }
  }
#ifdef ESP8266
#ifdef USE_ADC
  for (uint32_t i = 0; i < ARRAY_SIZE(kAdcNiceList); i++) {   // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
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

#ifdef ESP8266
#ifdef USE_ADC
void WSContentSendAdcNiceList(uint32_t option) {
  char stemp[30];                                             // Template number and Sensor name
  WSContentSend_P(PSTR("os=\""));
  for (uint32_t i = 0; i < ARRAY_SIZE(kAdcNiceList); i++) {   // GPIO: }2'0'>None}3}2'17'>Analog}3...
    if (option && (1 == i)) {
      WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), D_SENSOR_USER);  // }2'15'>User}3
    }
    uint32_t ridx = pgm_read_word(kAdcNiceList + i) & 0xFFE0;
    uint32_t midx = BGPIO(ridx);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, GetTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
  }
}
#endif  // USE_ADC
#endif  // ESP8266

/*-------------------------------------------------------------------------------------------*/

void HandleTemplateConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("save")) {
    TemplateSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[30];                                           // Template number and Sensor name

  WebGetArg("t", stemp, sizeof(stemp));                     // 0 - 69 Template number
  if (strlen(stemp)) {
    uint32_t module = atoi(stemp);
    uint32_t module_save = Settings.module;
    Settings.module = module;
    myio template_gp;
    TemplateGpios(&template_gp);
    gpio_flag flag = ModuleFlag();
    Settings.module = module_save;

    WSContentBegin(200, CT_PLAIN);
    WSContentSend_P(PSTR("%s}1"), AnyModuleName(module).c_str());  // NAME: Generic
    for (uint32_t i = 0; i < ARRAY_SIZE(template_gp.io); i++) {        // 17,148,29,149,7,255,255,255,138,255,139,255,255
      if (!FlashPin(i)) {
        WSContentSend_P(PSTR("%s%d"), (i>0)?",":"", template_gp.io[i]);
      }
    }
    WSContentSend_P(PSTR("}1%d}1%d"), flag, Settings.user_template_base);  // FLAG: 1  BASE: 17
    WSContentEnd();
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_TEMPLATE));

  UnishoxStrings msg(HTTP_TEMPLATE);
  WSContentStart_P(PSTR(D_CONFIGURE_TEMPLATE));
  WSContentSend_P(HTTP_SCRIPT_MODULE_TEMPLATE);

  WSContentSend_P(msg[HTTP_SCRIPT_TEMPLATE]);

  WSContentSendNiceLists(1);

  WSContentSend_P(msg[HTTP_SCRIPT_TEMPLATE2], MAX_USER_PINS);

#ifdef ESP8266
#ifdef USE_ADC
  WSContentSendAdcNiceList(1);
  WSContentSend_P(HTTP_SCRIPT_TEMPLATE3);
#endif  // USE_ADC
#endif  // ESP8266

  WSContentSend_P(msg[HTTP_SCRIPT_TEMPLATE4], GPIO_FLAG_USED, USER_MODULE);
  for (uint32_t i = 0; i < sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
    uint32_t midx = pgm_read_byte(kModuleNiceList + i);
    WSContentSend_P(HTTP_MODULE_TEMPLATE_REPLACE_INDEX, midx, AnyModuleName(midx).c_str(), midx +1);
  }
  WSContentSend_P(msg[HTTP_SCRIPT_TEMPLATE5], WEMOS_MODULE, USER_MODULE);

  WSContentSendStyle();
  WSContentSend_P(msg[HTTP_FORM_TEMPLATE], PSTR(D_TEMPLATE_PARAMETERS));
  WSContentSend_P(HTTP_TABLE100);
  WSContentSend_P(msg[HTTP_FORM_TEMPLATE6], PSTR(D_TEMPLATE_NAME), PSTR(D_TEMPLATE_NAME), PSTR(D_BASE_TYPE));
  WSContentSend_P(HTTP_TABLE100);
  for (uint32_t i = 0; i < MAX_GPIO_PIN; i++) {
    if (!FlashPin(i)) {
      WSContentSend_P(msg[HTTP_FORM_TEMPLATE7],
        ((9==i)||(10==i)) ? WebColor(COL_TEXT_WARNING) : WebColor(COL_TEXT), PSTR(D_GPIO),
        i, (0==i) ? " style='width:150px'" : "", i, i);
      WSContentSend_P(msg[HTTP_FORM_TEMPLATE8], i);
    }
  }
  WSContentSend_P(PSTR("</table>"));

  gpio_flag flag = ModuleFlag();
  if (flag.data) {
    WSContentSend_P(msg[HTTP_FORM_TEMPLATE_FLAG], PSTR(D_TEMPLATE_FLAGS));
  }

  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

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

void TemplateSaveSettings(void)
{
  char tmp[TOPSZ];                                          // WebGetArg NAME and GPIO/BASE/FLAG byte value
  char svalue[300];                                         // Template command string

  WebGetArg("s1", tmp, sizeof(tmp));                        // NAME
  snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_TEMPLATE " {\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), tmp);

  uint32_t j = 0;
  for (uint32_t i = 0; i < ARRAY_SIZE(Settings.user_template.gp.io); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    snprintf_P(svalue, sizeof(svalue), PSTR("%s%s%d"), svalue, (i>0)?",":"", WebGetGpioArg(j));
    j++;
  }

  uint32_t flag = 0;
  char webindex[5];                                         // WebGetArg name
  for (uint32_t i = 0; i < GPIO_FLAG_USED; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
    uint32_t state = Webserver->hasArg(webindex) << i;      // FLAG
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

  if (Webserver->hasArg("save")) {
    ModuleSaveSettings();
    WebRestart(1);
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_MODULE));

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
  WSContentSend_P(PSTR("\";sk(%d,99);os=\""), Settings.module);

  WSContentSendNiceLists(0);

  for (uint32_t i = 0; i < ARRAY_SIZE(template_gp.io); i++) {
    if (ValidGPIO(i, template_gp.io[i])) {
      WSContentSend_P(PSTR("sk(%d,%d);"), TasmotaGlobal.my_module.io[i], i);  // g0 - g17
    }
  }

#ifdef ESP8266
#ifdef USE_ADC
  WSContentSendAdcNiceList(0);
  WSContentSend_P(PSTR("\";sk(%d," STR(ADC0_PIN) ");"), Settings.my_gp.io[(sizeof(myio) / 2) -1]);
#endif  // USE_ADC
#endif  // ESP8266

  WSContentSend_P(PSTR("}wl(sl);"));

  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_MODULE, AnyModuleName(MODULE).c_str());
  for (uint32_t i = 0; i < ARRAY_SIZE(template_gp.io); i++) {
    if (ValidGPIO(i, template_gp.io[i])) {
      snprintf_P(stemp, 3, PINS_WEMOS +i*2);
      WSContentSend_P(PSTR("<tr><td style='width:116px'>%s <b>" D_GPIO "%d</b></td><td style='width:150px'><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
        (WEMOS==TasmotaGlobal.module_type)?stemp:"", i, i, i);
      WSContentSend_P(PSTR("<td style='width:50px'><select id='h%d'></select></td></tr>"), i);
    }
  }
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void ModuleSaveSettings(void)
{
  char tmp[8];         // WebGetArg numbers only

  WebGetArg("g99", tmp, sizeof(tmp));
  uint32_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
  Settings.last_module = Settings.module;
  Settings.module = new_module;
  SetModuleType();
  myio template_gp;
  TemplateGpios(&template_gp);
  String gpios = "";
  for (uint32_t i = 0; i < ARRAY_SIZE(template_gp.io); i++) {
    if (Settings.last_module != new_module) {
      Settings.my_gp.io[i] = GPIO_NONE;
    } else {
      if (ValidGPIO(i, template_gp.io[i])) {
        Settings.my_gp.io[i] = WebGetGpioArg(i);
        gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(Settings.my_gp.io[i]);
      }
    }
  }

  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), ModuleName().c_str(), gpios.c_str());
}

/*-------------------------------------------------------------------------------------------*/

const char kUnescapeCode[] = "&><\"\'\\";
const char kEscapeCode[] PROGMEM = "&amp;|&gt;|&lt;|&quot;|&apos;|&#92;";

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

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_WIFI));

  if (Webserver->hasArg("save") && HTTP_MANAGER_RESET_ONLY != Web.state) {
    WifiSaveSettings();
    WebRestart(2);
    return;
  }

  UnishoxStrings msg(HTTP_WIFI_CONF);
  WSContentStart_P(PSTR(D_CONFIGURE_WIFI), !WifiIsInManagerMode());
  WSContentSend_P(msg[HTTP_SCRIPT_WIFI]);
#ifdef USE_ENHANCED_GUI_WIFI_SCAN
  UnishoxStrings msg_e(HTTP_WIFI_CONF_ENHANCED);
  WSContentSendStyle_P(msg_e[HTTP_HEAD_STYLE_SSI], WebColor(COL_TEXT));
#else
  WSContentSendStyle();
#endif  // USE_ENHANCED_GUI_WIFI_SCAN

  if (HTTP_MANAGER_RESET_ONLY != Web.state) {
    if (Webserver->hasArg("scan")) {
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
      int n = WiFi.scanNetworks();
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

      if (0 == n) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_NO_NETWORKS_FOUND));
        WSContentSend_P(PSTR(D_NO_NETWORKS_FOUND));
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

#ifdef USE_ENHANCED_GUI_WIFI_SCAN
        //display networks in page
        for (uint32_t i = 0; i < n; i++) {
          if (indices[i] < n) {
            int32_t rssi = WiFi.RSSI(indices[i]);
            String ssid = WiFi.SSID(indices[i]);
            DEBUG_CORE_LOG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"),
              ssid.c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), rssi);

            // Print SSID
            WSContentSend_P(msg_e[HTTP_PRINT_SSID], HtmlEscape(ssid).c_str());

            String nextSSID = "";
            // Handle all APs with the same SSID
            for (uint32_t j = 0; j < n; j++) {
              if ((indices[j] < n) && ((nextSSID = WiFi.SSID(indices[j])) == ssid)) {
                // Update RSSI / quality
                rssi = WiFi.RSSI(indices[j]);
                uint32_t rssi_as_quality = WifiGetRssiAsQuality(rssi);
                uint32_t num_bars = changeUIntScale(rssi_as_quality, 0, 100, 0, 4);

                // Print item
                WSContentSend_P(msg_e[HTTP_PRINT_ITEM],
                  rssi, rssi_as_quality,
                  WiFi.BSSIDstr(indices[j]).c_str(),
                  WiFi.channel(indices[j])
                );
                // Print signal strength indicator
                for (uint32_t k = 0; k < 4; ++k) {
                  WSContentSend_P(msg_e[HTTP_PRINT_SSI], k, (num_bars < k) ? PSTR(" o30") : PSTR(""));
                }
                WSContentSend_P(msg_e[HTTP_PRINT_END]);

                indices[j] = n;
              }
              delay(0);
            }
            WSContentSend_P(PSTR("</div>"));
          }
        }
#else  // No USE_ENHANCED_GUI_WIFI_SCAN
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
/*
          int auth = WiFi.encryptionType(indices[i]);
          char encryption[20];
          WSContentSend_P(PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%s %d%% (%d dBm)</span></div>"),
            HtmlEscape(WiFi.SSID(indices[i])).c_str(),
            WiFi.channel(indices[i]),
            GetTextIndexed(encryption, sizeof(encryption), auth +1, kEncryptionType),
            quality, rssi
          );
*/
          WSContentSend_P(msg[HTTP_SCRIPT_ON_CLICK],
            HtmlEscape(WiFi.SSID(indices[i])).c_str(),
            WiFi.channel(indices[i]),
            quality, rssi
          );

          delay(0);
        }
#endif  // USE_ENHANCED_GUI_WIFI_SCAN

        WSContentSend_P(msg[HTTP_SCRIPT_BR]);
      }
    } else {
      WSContentSend_P(msg[HTTP_SCRIPT_SCAN], D_SCAN_FOR_WIFI_NETWORKS);
    }

    // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
    WSContentSend_P(msg[HTTP_FORM_WIFI],
      PSTR(D_WIFI_PARAMETERS),
      PSTR(D_AP1_SSID), PSTR(STA_SSID1), PSTR(STA_SSID1), SettingsText(SET_STASSID1), PSTR(D_AP1_PASSWORD),
      PSTR(D_AP2_SSID), PSTR(STA_SSID2), PSTR(STA_SSID2), SettingsText(SET_STASSID2), PSTR(D_AP2_PASSWORD),
      PSTR(D_HOSTNAME), WIFI_HOSTNAME, WIFI_HOSTNAME, SettingsText(SET_HOSTNAME),
      PSTR(D_CORS_DOMAIN), PSTR(CORS_DOMAIN), SettingsText(SET_CORS));
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
  if (strchr(SettingsText(SET_HOSTNAME), '%') != nullptr) {
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
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_SSID "2 %s, " D_CMND_CORS " %s"),
    SettingsText(SET_HOSTNAME), SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), SettingsText(SET_CORS));
}

/*-------------------------------------------------------------------------------------------*/

void HandleLoggingConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

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
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_MQTTLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
    Settings.seriallog_level, Settings.weblog_level, Settings.mqttlog_level, Settings.syslog_level, SettingsText(SET_SYSLOG_HOST), Settings.syslog_port, Settings.tele_period);
}

/*-------------------------------------------------------------------------------------------*/

void HandleOtherConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_OTHER));

  if (Webserver->hasArg("save")) {
    OtherSaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_OTHER));
  WSContentSendStyle();

  TemplateJson();
  char stemp[strlen(TasmotaGlobal.mqtt_data) +1];
  strlcpy(stemp, TasmotaGlobal.mqtt_data, sizeof(stemp));  // Get JSON template
  WSContentSend_P(HTTP_FORM_OTHER, stemp, (USER_MODULE == Settings.module) ? " checked disabled" : "",
    (Settings.flag.mqtt_enabled) ? " checked" : "",   // SetOption3 - Enable MQTT
    SettingsText(SET_FRIENDLYNAME1), SettingsText(SET_DEVICENAME));

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
  char tmp[300];   // Needs to hold complete ESP32 template of minimal 230 chars
  char webindex[5];
  char friendlyname[TOPSZ];
  char message[MAX_LOGSZ];

  WebGetArg("dn", tmp, sizeof(tmp));
  SettingsUpdateText(SET_DEVICENAME, (!strlen(tmp)) ? "" : (!strcmp(tmp,"1")) ? SettingsText(SET_FRIENDLYNAME1) : tmp);
  WebGetArg("wp", tmp, sizeof(tmp));
  SettingsUpdateText(SET_WEBPWD, (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? SettingsText(SET_WEBPWD) : tmp);
  Settings.flag.mqtt_enabled = Webserver->hasArg("b1");  // SetOption3 - Enable MQTT
#ifdef USE_EMULATION
  UdpDisconnect();
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
  WebGetArg("b2", tmp, sizeof(tmp));
  Settings.flag2.emulation = (!strlen(tmp)) ? 0 : atoi(tmp);
#endif  // USE_EMULATION_WEMO || USE_EMULATION_HUE
#endif  // USE_EMULATION

  snprintf_P(message, sizeof(message), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_CMND_EMULATION " %d, " D_CMND_DEVICENAME " %s, " D_CMND_FRIENDLYNAME),
    GetStateText(Settings.flag.mqtt_enabled), Settings.flag2.emulation, SettingsText(SET_DEVICENAME));
  for (uint32_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
    snprintf_P(webindex, sizeof(webindex), PSTR("a%d"), i);
    WebGetArg(webindex, tmp, sizeof(tmp));
    snprintf_P(friendlyname, sizeof(friendlyname), PSTR(FRIENDLY_NAME"%d"), i +1);
    SettingsUpdateText(SET_FRIENDLYNAME1 +i, (!strlen(tmp)) ? (i) ? friendlyname : FRIENDLY_NAME : tmp);
    snprintf_P(message, sizeof(message), PSTR("%s%s %s"), message, (i) ? "," : "", SettingsText(SET_FRIENDLYNAME1 +i));
  }
  AddLogData(LOG_LEVEL_INFO, message);

  WebGetArg("t1", tmp, sizeof(tmp));
  if (strlen(tmp)) {  // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255}
    snprintf_P(message, sizeof(message), PSTR(D_CMND_BACKLOG " " D_CMND_TEMPLATE " %s%s"), tmp, (Webserver->hasArg("t2")) ? "; " D_CMND_MODULE " 0" : "");
    ExecuteWebCommand(message, SRC_WEBGUI);
  }
}

/*-------------------------------------------------------------------------------------------*/

void HandleBackupConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

  if (!SettingsBufferAlloc()) { return; }

  WiFiClient myClient = Webserver->client();
  Webserver->setContentLength(sizeof(Settings));

  char attachment[TOPSZ];

//  char friendlyname[TOPSZ];
//  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), NoAlNumToUnderscore(friendlyname, SettingsText(SET_FRIENDLYNAME1)), TasmotaGlobal.version);

  char hostname[sizeof(TasmotaGlobal.hostname)];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), NoAlNumToUnderscore(hostname, TasmotaGlobal.hostname), TasmotaGlobal.version);

  Webserver->sendHeader(F("Content-Disposition"), attachment);

  WSSend(200, CT_STREAM, "");

  uint32_t cfg_crc32 = Settings.cfg_crc32;
  Settings.cfg_crc32 = GetSettingsCrc32();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

  memcpy(settings_buffer, &Settings, sizeof(Settings));
  if (Web.config_xor_on_set) {
    for (uint32_t i = 2; i < sizeof(Settings); i++) {
      settings_buffer[i] ^= (Web.config_xor_on_set +i);
    }
  }

  myClient.write((const char*)settings_buffer, sizeof(Settings));

  SettingsBufferFree();

  Settings.cfg_crc32 = cfg_crc32;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed
}

/*-------------------------------------------------------------------------------------------*/

void HandleResetConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESET_CONFIGURATION));

  WSContentStart_P(PSTR(D_RESET_CONFIGURATION), !WifiIsInManagerMode());
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

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_RESTORE_CONFIGURATION));

  UnishoxStrings msg(HTTP_UPGRADE);
  WSContentStart_P(PSTR(D_RESTORE_CONFIGURATION));
  WSContentSendStyle();
  WSContentSend_P(msg[HTTP_FORM_RST], PSTR(D_RESTORE_CONFIGURATION));
  WSContentSend_P(msg[HTTP_FORM_RST_UPG], PSTR(D_START), PSTR(D_UPGRADE), PSTR(D_UPLOAD_STARTED));
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

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_INFORMATION));

  char stopic[TOPSZ];

  int freeMem = ESP_getFreeHeap();

  WSContentStart_P(PSTR(D_INFORMATION));
  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  WSContentSend_P(HTTP_SCRIPT_INFO_BEGIN);
  WSContentSend_P(PSTR("<table style='width:100%%'><tr><th>"));
  WSContentSend_P(PSTR(D_PROGRAM_VERSION "}2%s%s"), TasmotaGlobal.version, TasmotaGlobal.image_name);
  WSContentSend_P(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), GetBuildDateAndTime().c_str());
  WSContentSend_P(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_CORE_RELEASE "/%s"), ESP.getSdkVersion());
  WSContentSend_P(PSTR("}1" D_UPTIME "}2%s"), GetUptime().c_str());
#ifdef ESP8266
  WSContentSend_P(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d at 0x%X"), Settings.save_flag, GetSettingsAddress());
#endif  // ESP8266
#ifdef ESP32
  WSContentSend_P(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d"), Settings.save_flag);
#endif  // ESP32
  WSContentSend_P(PSTR("}1" D_BOOT_COUNT "}2%d"), Settings.bootcount);
  WSContentSend_P(PSTR("}1" D_RESTART_REASON "}2%s"), GetResetReason().c_str());
  uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : TasmotaGlobal.devices_present;
#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
  for (uint32_t i = 0; i < maxfn; i++) {
    WSContentSend_P(PSTR("}1" D_FRIENDLY_NAME " %d}2%s"), i +1, SettingsText(SET_FRIENDLYNAME1 +i));
  }
  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line
#ifdef ESP32
#ifdef USE_ETHERNET
  if (static_cast<uint32_t>(EthernetLocalIP()) != 0) {
    WSContentSend_P(PSTR("}1" D_HOSTNAME "}2%s%s"), EthernetHostname(), (Mdns.begun) ? ".local" : "");
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), EthernetMacAddress().c_str());
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (eth)}2%s"), EthernetLocalIP().toString().c_str());
    WSContentSend_P(PSTR("}1<hr/>}2<hr/>"));
  }
#endif
#endif
  if (Settings.flag4.network_wifi) {
    int32_t rssi = WiFi.RSSI();
    WSContentSend_P(PSTR("}1" D_AP "%d " D_SSID " (" D_RSSI ")}2%s (%d%%, %d dBm)"), Settings.sta_active +1, HtmlEscape(SettingsText(SET_STASSID1 + Settings.sta_active)).c_str(), WifiGetRssiAsQuality(rssi), rssi);
    WSContentSend_P(PSTR("}1" D_HOSTNAME "}2%s%s"), TasmotaGlobal.hostname, (Mdns.begun) ? ".local" : "");
#if LWIP_IPV6
    String ipv6_addr = WifiGetIPv6();
    if (ipv6_addr != "") {
      WSContentSend_P(PSTR("}1 IPv6 Address }2%s"), ipv6_addr.c_str());
    }
#endif
    if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
      WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.macAddress().c_str());
      WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (wifi)}2%s"), WiFi.localIP().toString().c_str());
      WSContentSend_P(PSTR("}1<hr/>}2<hr/>"));
    }
  }
  if (!TasmotaGlobal.global_state.network_down) {
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%s"), IPAddress(Settings.ip_address[1]).toString().c_str());
    WSContentSend_P(PSTR("}1" D_SUBNET_MASK "}2%s"), IPAddress(Settings.ip_address[2]).toString().c_str());
    WSContentSend_P(PSTR("}1" D_DNS_SERVER "}2%s"), IPAddress(Settings.ip_address[3]).toString().c_str());
  }
  if ((WiFi.getMode() >= WIFI_AP) && (static_cast<uint32_t>(WiFi.softAPIP()) != 0)) {
    WSContentSend_P(PSTR("}1<hr/>}2<hr/>"));
    WSContentSend_P(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
    WSContentSend_P(PSTR("}1" D_IP_ADDRESS " (AP)}2%s"), WiFi.softAPIP().toString().c_str());
    WSContentSend_P(PSTR("}1" D_GATEWAY "}2%s"), WiFi.softAPIP().toString().c_str());
  }
  WSContentSend_P(PSTR("}1}2&nbsp;"));  // Empty line
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    WSContentSend_P(PSTR("}1" D_MQTT_HOST "}2%s"), SettingsText(SET_MQTT_HOST));
    WSContentSend_P(PSTR("}1" D_MQTT_PORT "}2%d"), Settings.mqtt_port);
#ifdef USE_MQTT_TLS
    WSContentSend_P(PSTR("}1" D_MQTT_TLS_ENABLE "}2%s"), Settings.flag4.mqtt_tls ? PSTR(D_ENABLED) : PSTR(D_DISABLED));
#endif // USE_MQTT_TLS
    WSContentSend_P(PSTR("}1" D_MQTT_USER "}2%s"), SettingsText(SET_MQTT_USER));
    WSContentSend_P(PSTR("}1" D_MQTT_CLIENT "}2%s"), TasmotaGlobal.mqtt_client);
    WSContentSend_P(PSTR("}1" D_MQTT_TOPIC "}2%s"), SettingsText(SET_MQTT_TOPIC));
    uint32_t real_index = SET_MQTT_GRP_TOPIC;
    for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
      if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
      if (strlen(SettingsText(real_index +i))) {
        WSContentSend_P(PSTR("}1" D_MQTT_GROUP_TOPIC " %d}2%s"), 1 +i, GetGroupTopic_P(stopic, "", real_index +i));
      }
    }
    WSContentSend_P(PSTR("}1" D_MQTT_FULL_TOPIC "}2%s"), GetTopic_P(stopic, CMND, TasmotaGlobal.mqtt_topic, ""));
    WSContentSend_P(PSTR("}1" D_MQTT " " D_FALLBACK_TOPIC "}2%s"), GetFallbackTopic_P(stopic, ""));
    WSContentSend_P(PSTR("}1" D_MQTT_NO_RETAIN "}2%s"), Settings.flag4.mqtt_no_retain ? PSTR(D_ENABLED) : PSTR(D_DISABLED));
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
  WSContentSend_P(PSTR("}1" D_ESP_CHIP_ID "}2%d"), ESP_getChipId());
#ifdef ESP8266
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), ESP.getFlashChipId());
#endif
  WSContentSend_P(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), ESP.getFlashChipRealSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%dkB"), ESP.getFlashChipSize() / 1024);
  WSContentSend_P(PSTR("}1" D_PROGRAM_SIZE "}2%dkB"), ESP_getSketchSize() / 1024);
  WSContentSend_P(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%dkB"), ESP.getFreeSketchSpace() / 1024);
  WSContentSend_P(PSTR("}1" D_FREE_MEMORY "}2%dkB"), freeMem / 1024);
#ifdef ESP32
  if (psramFound()) {
    WSContentSend_P(PSTR("}1" D_PSR_MAX_MEMORY "}2%dkB"), ESP.getPsramSize() / 1024);
    WSContentSend_P(PSTR("}1" D_PSR_FREE_MEMORY "}2%dkB"), ESP.getFreePsram() / 1024);
  }
#endif
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

void BUploadInit(uint32_t file_type) {
  Web.upload_file_type = file_type;
  BUpload.spi_hex_size = 0;
  BUpload.spi_sector_counter = FlashWriteStartSector();
  BUpload.spi_sector_cursor = 0;
  BUpload.active = true;
  BUpload.ready = false;
}

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

void HandleUpgradeFirmware(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_FIRMWARE_UPGRADE));

  UnishoxStrings msg(HTTP_UPGRADE);
  WSContentStart_P(PSTR(D_FIRMWARE_UPGRADE));
  WSContentSendStyle();
  WSContentSend_P(msg[HTTP_FORM_UPG], PSTR(D_UPGRADE_BY_WEBSERVER), PSTR(D_OTA_URL), SettingsText(SET_OTAURL),
                  PSTR(D_START_UPGRADE), PSTR(D_UPGRADE_BY_FILE_UPLOAD));
  WSContentSend_P(msg[HTTP_FORM_RST_UPG], PSTR(D_START), PSTR(D_UPGRADE), PSTR(D_UPLOAD_STARTED));
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

  WSContentStart_P(PSTR(D_INFORMATION));
  WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, HTTP_OTA_RESTART_RECONNECT_TIME);
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

#if defined(USE_ZIGBEE_EZSP)
  if ((UPL_EFR32 == Web.upload_file_type) && !Web.upload_error && BUpload.ready) {
    BUpload.ready = false;  //  Make sure not to follow thru again
    // GUI xmodem
    ZigbeeUploadStep1Done(FlashWriteStartSector(), BUpload.spi_hex_size);
    HandleZigbeeXfer();
    return;
  }
#endif  // USE_ZIGBEE_EZSP

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  WifiConfigCounter();
  TasmotaGlobal.restart_flag = 0;
  MqttRetryCounter(0);
#ifdef USE_COUNTER
  CounterInterruptDisable(false);
#endif  // USE_COUNTER

  WSContentStart_P(PSTR(D_INFORMATION));
  if (!Web.upload_error) {
    WSContentSend_P(HTTP_SCRIPT_RELOAD_TIME, (UPL_TASMOTA == Web.upload_file_type) ? HTTP_OTA_RESTART_RECONNECT_TIME : HTTP_RESTART_RECONNECT_TIME);  // Refesh main web ui after OTA upgrade
  }
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
  if (Web.upload_error) {
    WSContentSend_P(PSTR("%06x'>" D_FAILED "</font></b><br><br>"), WebColor(COL_TEXT_WARNING));
    if (Web.upload_error < 10) {
      GetTextIndexed(error, sizeof(error), Web.upload_error -1, kUploadErrors);
    } else {
      snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), Web.upload_error);
    }
    WSContentSend_P(error);
    DEBUG_CORE_LOG(PSTR("UPL: %s"), error);
    TasmotaGlobal.stop_flash_rotate = Settings.flag.stop_flash_rotate;  // SetOption12 - Switch between dynamic or fixed slot flash save location
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

void HandleUploadLoop(void)
{
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
  bool _serialoutput = (LOG_LEVEL_DEBUG <= TasmotaGlobal.seriallog_level);

  if (HTTP_USER == Web.state) { return; }
  if (Web.upload_error) {
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
    return;
  }

  HTTPUpload& upload = Webserver->upload();

  // ***** Step1: Start upload file
  if (UPLOAD_FILE_START == upload.status) {
    TasmotaGlobal.restart_flag = 60;
    if (0 == upload.filename.c_str()[0]) {
      Web.upload_error = 1;  // No file selected
      return;
    }
    SettingsSave(1);  // Free flash for upload
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    if (UPL_SETTINGS == Web.upload_file_type) {
      if (!SettingsBufferAlloc()) {
        Web.upload_error = 2;  // Not enough space
        return;
      }
    } else {
      MqttRetryCounter(60);
#ifdef USE_COUNTER
      CounterInterruptDisable(true);  // Prevent OTA failures on 100Hz counter interrupts
#endif  // USE_COUNTER
#ifdef USE_EMULATION
      UdpDisconnect();
#endif  // USE_EMULATION
#ifdef USE_ARILUX_RF
      AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
      if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
        MqttDisconnect();
      }
    }
    Web.upload_progress_dot_count = 0;
  }

  // ***** Step2: Write upload file
  else if (UPLOAD_FILE_WRITE == upload.status) {
    if (0 == upload.totalSize) {  // First block received
      if (UPL_SETTINGS == Web.upload_file_type) {
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
      else if (ShdPresent() && (0x00 == upload.buf[0]) && (0x10 == upload.buf[1])) {
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
      else if ((upload.buf[0] != 0xE9) && (upload.buf[0] != 0x1F)) {  // 0x1F is gzipped 0xE9
        Web.upload_error = 3;      // Invalid file signature - Magic byte is not 0xE9
        return;
      }
      if (UPL_TASMOTA == Web.upload_file_type) {
        if (0xE9 == upload.buf[0]) {
          uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
          if (bin_flash_size > ESP.getFlashChipRealSize()) {
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
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "File type %d"), Web.upload_file_type);
    }  // First block received

    if (UPL_SETTINGS == Web.upload_file_type) {
      if (upload.currentSize > (sizeof(Settings) - (Web.config_block_count * HTTP_UPLOAD_BUFLEN))) {
        Web.upload_error = 9;  // File too large
        return;
      }
      memcpy(settings_buffer + (Web.config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
      Web.config_block_count++;
    }
#ifdef USE_WEB_FW_UPGRADE
    else if (BUpload.active) {
      // Write a block
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR("DBG: Size %d"), upload.currentSize);
//      AddLogBuffer(LOG_LEVEL_DEBUG, upload.buf, 32);
      Web.upload_error = BUploadWriteBuffer(upload.buf, upload.currentSize);
      if (Web.upload_error != 0) { return; }
    }
#endif  // USE_WEB_FW_UPGRADE
    else if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Web.upload_error = 5;  // Upload buffer miscompare
      return;
    }
    if (_serialoutput) {
      Serial.printf(".");
      Web.upload_progress_dot_count++;
      if (!(Web.upload_progress_dot_count % 80)) { Serial.println(); }
    }
  }

  // ***** Step3: Finish upload file
  else if (UPLOAD_FILE_END == upload.status) {
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
#ifdef ESP8266
        valid_settings = (0 == settings_buffer[0xF36]);  // Settings.config_version
#endif  // ESP8266
#ifdef ESP32
        valid_settings = (1 == settings_buffer[0xF36]);  // Settings.config_version
#endif  // ESP32
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
#ifdef USE_WEB_FW_UPGRADE
    else if (BUpload.active) {
      // Done writing the data to SPI flash
      BUpload.active = false;

      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Transfer %u bytes"), upload.totalSize);

      uint8_t* data = FlashDirectAccess();

//      uint32_t* values = (uint32_t*)(data);  // Only 4-byte access allowed
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Head 0x%08X"), values[0]);

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
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Transfer error %d"), error);
        Web.upload_error = error + (100 * Web.upload_file_type);  // Add offset to discriminate transfer errors
        return;
      }
    }
#endif  // USE_WEB_FW_UPGRADE
    else if (!Update.end(true)) { // true to set the size to the current progress
      if (_serialoutput) { Update.printError(Serial); }
      Web.upload_error = 6;  // Upload failed. Enable logging 3
      return;
    }
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes"), upload.totalSize);
  }

  // ***** Step4: Abort upload file
  else if (UPLOAD_FILE_ABORTED == upload.status) {
    TasmotaGlobal.restart_flag = 0;
    MqttRetryCounter(0);
#ifdef USE_COUNTER
    CounterInterruptDisable(false);
#endif  // USE_COUNTER
    Web.upload_error = 7;  // Upload aborted
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
  }
  delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void HandlePreflightRequest(void)
{
  HttpHeaderCors();
  Webserver->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  Webserver->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  WSSend(200, CT_HTML, "");
}

/*-------------------------------------------------------------------------------------------*/

void HandleHttpCommand(void)
{
  if (!HttpCheckPriviledgedAccess(false)) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  if (strlen(SettingsText(SET_WEBPWD))) {
    char tmp1[33];
    WebGetArg("user", tmp1, sizeof(tmp1));
    char tmp2[strlen(SettingsText(SET_WEBPWD)) +1];
    WebGetArg("password", tmp2, sizeof(tmp2));
    if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, SettingsText(SET_WEBPWD)))) {
      WSContentBegin(401, CT_JSON);
      WSContentSend_P(PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
      WSContentEnd();
      return;
    }
  }

  WSContentBegin(200, CT_JSON);
  String svalue = Webserver->arg("cmnd");
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
        size_t JSONlen = len - (JSON - line);
        if (JSONlen > sizeof(TasmotaGlobal.mqtt_data)) { JSONlen = sizeof(TasmotaGlobal.mqtt_data); }
        char stemp[JSONlen];
        strlcpy(stemp, JSON +1, JSONlen -2);
        WSContentSend_P(PSTR("%s%s"), (cflg) ? "," : "", stemp);
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

/*-------------------------------------------------------------------------------------------*/

void HandleConsole(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("c2")) {      // Console refresh requested
    HandleConsoleRefresh();
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONSOLE));

  WSContentStart_P(PSTR(D_CONSOLE));
  WSContentSend_P(HTTP_SCRIPT_CONSOL, Settings.web_refresh);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_CMND);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

void HandleConsoleRefresh(void)
{
  String svalue = Webserver->arg("c1");
  if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
    ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
  }

  char stmp[8];
  WebGetArg("c2", stmp, sizeof(stmp));
  uint32_t index = 0;                // Initial start, dump all
  if (strlen(stmp)) { index = atoi(stmp); }

  WSContentBegin(200, CT_PLAIN);
  WSContentSend_P(PSTR("%d}1%d}1"), TasmotaGlobal.log_buffer_pointer, Web.reset_web_log_flag);
  if (!Web.reset_web_log_flag) {
    index = 0;
    Web.reset_web_log_flag = true;
  }
  bool cflg = (index);
  char* line;
  size_t len;
  while (GetLog(Settings.weblog_level, &index, &line, &len)) {
    if (len > sizeof(TasmotaGlobal.mqtt_data) -2) { len = sizeof(TasmotaGlobal.mqtt_data); }
    char stemp[len +1];
    strlcpy(stemp, line, len);
    WSContentSend_P(PSTR("%s%s"), (cflg) ? "\n" : "", stemp);
    cflg = true;
  }
  WSContentSend_P(PSTR("}1"));
  WSContentEnd();
}

/********************************************************************************************/

void HandleNotFound(void)
{
//  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Not found (%s)"), Webserver->uri().c_str());

  if (CaptivePortal()) { return; }  // If captive portal redirect instead of displaying the error page.

#ifdef USE_EMULATION
#ifdef USE_EMULATION_HUE
  String path = Webserver->uri();
  if ((EMUL_HUE == Settings.flag2.emulation) && (path.startsWith("/api"))) {
    HandleHueApi(&path);
  } else
#endif  // USE_EMULATION_HUE
#endif  // USE_EMULATION
  {
    WSContentBegin(404, CT_PLAIN);
    WSContentSend_P(PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), Webserver->uri().c_str(), (Webserver->method() == HTTP_GET) ? "GET" : "POST", Webserver->args());
    for (uint32_t i = 0; i < Webserver->args(); i++) {
      WSContentSend_P(PSTR(" %s: %s\n"), Webserver->argName(i).c_str(), Webserver->arg(i).c_str());
    }
    WSContentEnd();
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
bool CaptivePortal(void)
{
  // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
  if ((WifiIsInManagerMode()) && !ValidIpAddress(Webserver->hostHeader().c_str())) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));

    Webserver->sendHeader(F("Location"), String("http://") + Webserver->client().localIP().toString(), true);
    WSSend(302, CT_PLAIN, "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    Webserver->client().stop();  // Stop is needed because we sent no content length
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

    WiFiClient http_client;
    HTTPClient http;
    if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
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
              TasmotaGlobal.mqtt_data[j++] = text;
              if (j == sizeof(TasmotaGlobal.mqtt_data) -2) { break; }
            }
          }
          TasmotaGlobal.mqtt_data[j] = '\0';
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
    TasmotaGlobal.restart_flag = 2;
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
      (2 == Settings.webserver) ? D_ADMIN : D_USER, NetworkHostname(), NetworkAddress().toString().c_str());
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
      if (Settings.flag2.emulation) { PollUdp(); }
#endif  // USE_EMULATION
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWebCommands, WebCommand);
      break;
  }
  return result;
}
#endif  // USE_WEBSERVER
