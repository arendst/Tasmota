/*
  xdrv_43_mlx90640.ino - MLX90640 support for Tasmota

  Copyright (C) 2021  Christian Baars and Theo Arends

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


  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  0.9.0.0 20200827  started - based on https://github.com/melexis/mlx90640-library
*/

#ifdef USE_I2C
#ifdef USE_MLX90640

#define MLX90640_ADDRESS        0x33
#define MLX90640_POI_NUM        6       //some parts of the JS are hardcoded for 6!!

/*********************************************************************************************\
* MLX90640
\*********************************************************************************************/

#define XDRV_43             43
#define XI2C_53             53 // See I2CDEVICES.md
#include <MLX90640_API.h>

const char MLX90640type[] PROGMEM = "MLX90640";

#ifdef USE_WEBSERVER
#define WEB_HANDLE_MLX90640 "mlx"
const char HTTP_BTN_MENU_MLX90640[] PROGMEM = "<p><form action='" WEB_HANDLE_MLX90640 "' method='get'><button>MLX90640</button></form></p>";
#endif // USE_WEBSERVER

struct {
  uint32_t type:1;
  uint32_t ready:1;
  uint32_t dumpedEE:1;
  uint32_t extractedParams:1;
  paramsMLX90640 *params;
  float Ta;
  uint16_t Frame[834];
  float To[768];
  uint8_t pois[2*MLX90640_POI_NUM] = {2,1, 30,1, 10,12, 22,12, 2,23, 30,23}; // {x1,y1,x2,y2,...,x6,y6}
} MLX90640;

/*********************************************************************************************\
 * commands
\*********************************************************************************************/

#define D_CMND_MLX90640 "MLX"

const char S_JSON_MLX90640_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_MLX90640 "%s\":%d}";
const char S_JSON_MLX90640_COMMAND[] PROGMEM        = "{\"" D_CMND_MLX90640 "%s\"}";
const char kMLX90640_Commands[] PROGMEM             = "POI";

enum MLX90640_Commands {                            // commands useable in console or rules
  CMND_MLX90640_POI                                 // MLXPOIn xxyy - set POI number n to x,y
  };

/************************************************************************\
 * Web GUI
\************************************************************************/
#ifdef USE_WEBSERVER

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_1_SNS_SIZE = 389;
const char HTTP_MLX90640_1_SNS_COMPRESSED[] PROGMEM = "\x3D\x3C\x1F\xF4\x65\x2A\x2B\x32\x18\xCF\x87\xDD\x33\x65\x1D\x86\xBB\x33\xB0\x41"
                             "\xA4\x7D\x9F\x81\xE7\x7A\x90\xDB\x18\x7C\x3B\xA6\x76\x10\xB6\x75\x1B\x0E\x43\xA8"
                             "\x8C\x8E\x43\xA8\x8D\x87\x28\xEA\x23\x23\x94\x77\x8F\x87\xE1\x02\x0D\x13\xAC\xD8"
                             "\x72\x1D\xE3\xD6\x77\x48\xC8\xE5\x1D\x64\x6C\x39\x47\x78\xEC\x3B\xA4\x64\x72\x1D"
                             "\x64\x6C\x39\x0E\xF1\xDB\x23\x61\xCA\x3C\x10\x20\xE3\x3A\x36\xC7\x9A\x3E\x2E\x63"
                             "\xE8\xB4\x6D\x8F\x33\xC1\x9D\xFD\x07\x7C\x67\x7E\x3A\x83\xA3\x61\xD4\x3D\xF1\x0F"
                             "\x06\x77\xF4\x3C\x43\x0D\x87\x50\xCC\xD3\xE1\xEF\x1E\xF9\xE0\xCE\xFE\xBE\x56\x7C"
                             "\x3D\xE3\xDF\x3C\x18\x17\xC1\xD6\xE7\x21\xE7\x44\x37\x05\xF9\x90\xCC\xF1\xDD\x04"
                             "\x2C\x65\x33\x3A\x3B\xC8\xF6\x82\x0E\x87\xF6\x1D\x23\xE0\x21\x66\x87\x41\xE7\x44"
                             "\x3B\x05\xF0\x9B\xC3\xC4\x18\x5A\xFA\x8B\xEC\x3A\x3B\xA7\x78\xF0\x67\x7F\x46\xC4"
                             "\x7C\x4C\xCE\x8E\x81\x85\xAF\xA8\x8D\x87\x5F\xD8\x74\x74\x09\x98\xA3\xC6\x98\x3B"
                             "\xA6\xC3\xF0\xE5\xD3\x3B\xC7\xB4\x8D\x87\xC3\x97\x11\xE0\xF7\x17\xDD\x0B\xFF\x23"
                             "\xDA\x6C\x3C\xD1\x0D\xBA\x14\x74\x30\x16\x67\xCE\xE8\xDB\x18\x77\x4D\x87\x51\xC6"
                             "\x75\x5D\x33\xA9\x9D\x57\x0E\x88\xEF\x1D\xE3\xA8\x8C\x81\x32\xF9\xDD\x04\x5D\x04"
                             "\x8C\x91\xD6\xBE\xC3\xA3\xA5\x60\xC3\xBC\x75\x1C\x67\x55\x63\x3A\x99\xD5\x56\x74"
                             "\x47\x78\xEF\x1E\xE3\xC1\xEE";
#define  HTTP_MLX90640_1_SNS       Decompress(HTTP_MLX90640_1_SNS_COMPRESSED,HTTP_MLX90640_1_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_1_SNS[] PROGMEM =
    "<script type='text/javascript'>"
        "const map=(value,x1,y1,x2,y2)=>(value-x1)*(y2-x2)/(y1-x1)+x2;"
        "const image = new Image;"
        "var canvas,ctx,grd;"
        "var gPx,poi=[];var rA=[];" //gradient pixel, POI's, rawArray
    "function getMousePos(canvas, evt) {"
        "const rect = canvas.getBoundingClientRect();"
        "var x = evt.clientX-rect.left;"
        "if(x>320){"
          "x=319;"
        "}"
        "return {"
          "x: Math.floor(map(x,0,320,0,31.9)),"
          "y: Math.floor(map((evt.clientY - rect.top),0,240,0,23.9))};"
        "}"
  ;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_2a_SNS_SIZE = 632;
const char HTTP_MLX90640_2a_SNS_COMPRESSED[] PROGMEM = "\x33\xBF\xA0\xB7\x9A\x3E\x23\x8C\xF0\x5E\x74\x5B\xD4\xFE\x67\x61\x1D\xD3\x02\xF8"
                             "\x3A\xDC\xE3\xBA\x77\x91\xED\xF8\x47\x74\xFB\x16\x11\xF6\x75\x05\xBC\xCE\xF1\xE0"
                             "\xF7\x1D\x47\x29\xB3\xBC\x78\x20\x43\xBA\xBE\x11\xDD\xF1\xD4\x66\x77\x8F\x69\x9D"
                             "\xFD\x1B\x3E\x7C\xE6\x3E\x88\xD8\x43\x48\x22\x15\x54\x30\xBE\xCD\x42\xDF\xA8\xEE"
                             "\x9D\xE3\xC1\xB3\xE7\x4C\xEF\xBB\x10\xCB\xD5\x74\xC3\x15\x7C\x3C\xCF\x80\x8B\xA0"
                             "\x1E\xDD\x30\x77\x4D\x9F\x3A\x7D\xD8\x86\x45\xEA\xBA\x67\xC3\xE1\xCC\x3F\x47\xE8"
                             "\x8D\x9F\x3A\x7A\xAE\x85\xF8\xF8\x7C\x39\x4D\x90\x20\xE7\xD6\x43\x91\xF1\x1B\x3E"
                             "\x74\xFB\xF0\xCC\xEF\x33\xC1\x9D\xFD\x69\xE3\x4C\x23\xBB\x64\x38\xE8\x38\xCA\x99"
                             "\x04\xF8\x7A\x85\x1F\x0F\x87\x2B\x99\xDE\x3B\x87\xB4\x8C\xEF\xE8\xC1\x5A\x3E\x2E"
                             "\x63\xE8\x8C\x05\x97\x47\x2E\x88\xAF\xFF\xB3\x23\xBB\x64\x38\xEF\x1E\x02\xDE\x67"
                             "\xC3\x05\x67\xBC\x71\x9E\xF9\xE0\xB4\xC1\xDD\x0B\x79\x9F\x87\x23\x6C\xEF\x1E\xD2"
                             "\x0B\x79\x9F\x02\xDE\x67\x59\xC8\xDB\x3C\x13\x1C\x77\x4F\xBB\x39\x0F\xB3\xBC\x74"
                             "\x2D\xEE\x7F\x21\x45\x44\x34\x82\x3E\x05\xBC\xCE\x95\x84\x63\x4D\x8C\x43\xBA\x72"
                             "\x88\x10\xB2\x73\x8C\xF7\x11\x8C\xFA\x3B\xBA\x7C\x39\x0F\x07\x70\xB5\x1E\x88\xC1"
                             "\x59\xD0\x27\xC3\xD4\x28\xF0\xB4\xED\x9D\xB3\xBC\x8F\x6A\xF9\x59\xEF\x69\xDB\x3B"
                             "\xA1\x6F\x33\xD6\x73\xB0\xEF\x1D\x70\xF7\xCE\xE1\xF0\xEE\x11\x82\xB3\xDE\xD3\xDF"
                             "\x3C\x1E\xE0\x42\xE3\x90\x2D\xE6\x76\xCE\x42\x04\x5D\xB0\xE4\x3B\xC7\x70\x81\x07"
                             "\x6B\x38\xCF\x04\x37\x3C\x77\x4E\xF1\xE0\xF7\x1E\xE0\x4E\xE1\x28\xE4\xA2\x04\x1E"
                             "\x1F\x0C\x8F\x9C\xC7\xD1\x0B\xDE\xA3\x9F\x20\x45\xE1\x0C\x10\xB7\x13\x8C\x81\x07"
                             "\x71\x32\x04\x8D\xC0\xF6\x8C\xCD\x3D\xED\x3B\x0E\x51\xEF\x9F\x0F\x78\x8C\x8F\x7B"
                             "\x4F\x7C\xEA\x46\x47\xBD\xA8\xED\xA3\x90\xF7\xCF\x7C\xF0\x77\x0E\xD9\xDB\x2D\x3C"
                             "\x1E\xE3\xDC\x7B\x8F\x07\xB8\xF0\x08\xDC\x67\x15\x19\x0C\x68\xF8\x8F\xBB\xFF\xEC"
                             "\xC8\x70\xB3\x06\x1F\xCF\xB3\xC1\xB3\xE7\x4C\x18\xF8\xEE\x9F\x64\x3C\x4C\xA8\xFB"
                             "\x3A\x8F\xB3\xB0\x68\x46\xC3\xB4\x7D\x9D\xBF\x1D\xB3\xEC\xF8\x7D\x9D\xB3\x33\xAA"
                             "\xBE\x2D\x9D\xE3\xC1\xB3\xE7\x4F\x3E\x10\xEE\x9D\xE3\xC1\xEE\x3C\x1B";
#define  HTTP_MLX90640_2a_SNS       Decompress(HTTP_MLX90640_2a_SNS_COMPRESSED,HTTP_MLX90640_2a_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_2a_SNS[] PROGMEM =
  "var line = 0;"
  "setInterval(function() {"
  "rl('ul',line);"   // 0 = do NOT force refresh
  "},200);"
  "function rl(s,v){"         //source, value
    "var xr=new XMLHttpRequest();"
    "xr.onreadystatechange=function(){"
      "if(xr.readyState==4&&xr.status==200){"
            "var aB = xr.response;" // arrayBuffer
            "var i;"
            "if (aB.byteLength==260) {" // 2 lines of pixel data
              "var fA = new Float32Array(aB);" //floatArray
              "line=fA[0];"
              "if(line>1000){line=line-1000;eb('a1').innerHTML=line.toFixed(2);line=0}" //ambient hack
              "for (i=1; i < fA.length; i++) { "
                "rA[i+(line*64)-1] = fA[i];"
              "}"
              "line = line+1;"
              "if(line>11) {line=0;mos();"
              //"console.log(rA);"
              "}"
            "}"
            "if (aB.byteLength==12)"
              "{var y=new Uint8Array(aB);"
            	"for (i=0; i < y.length; i++){"
                "poi[i/2]=[y[i], y[i + 1]]; ++i;"
              "}"
            "}"
        "};"
      "};"
    "xr.responseType = 'arraybuffer';"
    "xr.open('GET','/mlx?'+s+'='+v,true);"
    "xr.send();"
    "};"
  ;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_2b_SNS_SIZE = 389;
const char HTTP_MLX90640_2b_SNS_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x43\x73\xC7\x74\xEF\x1E\xD3\x3B\xFA\xCF\x06\x8F\x88\x4C\x0C"
                             "\x58\xD7\xD4\x74\x0F\xEE\xE9\x93\x09\x8D\x7D\x47\x74\xFB\x0E\xF8\xCE\xFC\x7D\x9D"
                             "\xE3\xC6\x78\x33\xA0\xFE\x89\x42\x91\xF1\x1C\xBA\x3C\x16\x78\x33\xA0\xA7\xA3\xC2"
                             "\xA9\x1F\x11\xCA\xC3\xC1\x19\xDF\xD6\x07\x46\xC4\x7C\x59\xE0\xCE\x83\xCE\x88\x3C"
                             "\xEA\x66\xCA\x3B\xA7\xD9\xCA\x21\x0F\xB3\xBC\x78\x31\x9F\x47\x74\xCE\xFE\xB4\xF8"
                             "\x71\x9E\x0B\x4F\x43\x95\x87\x82\xD3\xB6\x76\xCE\xF1\xED\x04\x4A\x66\xB3\xE1\xC6"
                             "\x78\x23\x59\xE8\x72\xE8\xF0\x46\xB3\xB6\x76\xC1\x07\x74\x32\x47\xC4\x72\xAD\x1D"
                             "\xC3\xAC\xEE\x11\x0D\xBA\x14\x74\x30\x16\x67\xCE\xE8\xDB\x18\x77\x7E\x56\x7B\xC7"
                             "\x77\x4F\x59\xCB\xA3\xBC\x76\xC8\xD6\x7B\xE7\x51\xC6\x75\x63\x3A\x99\xD5\x56\x8E"
                             "\xF1\xDE\x3C\x16\x07\x46\xC3\xA1\x8D\x08\x22\xF5\x19\x04\xD1\xF1\x1F\x7F\x1E\x1C"
                             "\x77\x4F\xB4\x76\xD0\xF1\x0C\x36\x1D\x04\xBA\xB3\xDE\x3B\xA6\x47\xAC\xE6\x1D\xE3"
                             "\xDF\x3B\x87\x6C\xEE\x1F\x67\x51\x02\x6D\x43\xB6\x72\x1E\xF9\xDC\x3B\x64\x0A\x15"
                             "\x4E\x51\xEF\x9D\xC3\xB6\x77\x0F\xB3\xBC\x7D\x90\x22\xE7\xE5\xF6\x1D\x1D\xD3\x59"
                             "\xEB\x39\x0E\xA2\xD3\xD6\x72\x1D\x50\xEA\x87\x78\xF0\x7B\x8F\x71\x68\xDB\x1E\x67"
                             "\x4F\x7C\x34\x7C\xCF\x06\x74\xAC\x21\x2E\xAC\x85\x97\xC8\x23\xBA\x7D\xE8\xDB\x1E"
                             "\x67\x60\xCE\x1E\x3E\xCE\xF1\xE0\xF7\x1B";
#define  HTTP_MLX90640_2b_SNS       Decompress(HTTP_MLX90640_2b_SNS_COMPRESSED,HTTP_MLX90640_2b_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_2b_SNS[] PROGMEM =
  "function mos(){" //map off screen"
      "var osc = document.createElement('canvas');" //offscreen
      "osc.width = 32;"
      "osc.height = 24;"
          "var octx = osc.getContext('2d');"
          "for (var i=0;i<24;i++){"
              "for (var j=0;j<32;j++){"
                  "var y = 239 - Math.floor(map(rA[(i*32)+j],0,40,0,239));" // 40 is max. temp for heat map
                  // console.log(gPx.data[y],gPx.data[y+1],gPx.data[y+2]);
                  "octx.fillStyle = 'rgb(' + gPx.data[(y*4)] + ',' + gPx.data[(y*4)+1] +',' + gPx.data[(y*4)+2] + ')';"
                  "octx.fillRect(j*1,i*1,1,1);"
              "}"
      "}"
      "image.src =osc.toDataURL('image/png');"
  "}"
   ;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_3a_SNS_SIZE = 664;
const char HTTP_MLX90640_3a_SNS_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x43\x33\x48\x43\xBA\x77\x8F\x68\xF7\xC4\x21\x0E\xE9\xDE\x3C"
                             "\x07\x46\xC3\xA0\xF0\x58\x3A\xC2\x20\xF0\x68\xCC\xF6\xD3\x2C\x18\xFF\x75\xB9\xC8"
                             "\xF8\x8F\xBF\x60\xBF\x86\xCE\xBC\x33\x7F\x3E\xCF\x06\x77\xF5\xF2\xC4\x7C\x5C\xC7"
                             "\xD1\x7E\xF8\x79\x9D\xD3\xBC\x78\xF9\x61\xD3\xCC\x26\x1D\x17\x60\x8C\x83\xCE\xA7"
                             "\xD5\xE3\xBA\xC7\x1D\xD3\xEC\x69\xCA\x3E\xCE\xF1\xDE\x3C\x17\xCB\x0E\x82\x30\x9D"
                             "\x02\x2D\x83\xBC\x78\x31\x9F\x3B\xA6\x77\xF4\x31\x6F\x21\x99\xA7\x78\xF6\x99\xDF"
                             "\xD0\x67\xC3\x93\x59\xE0\xB4\xC1\xDD\x63\x8E\xE9\xF6\x33\x34\x82\x3E\xCE\xF1\xD0"
                             "\xCE\xC2\x16\xCF\x87\xC1\x87\x78\xF6\x86\x7C\x39\x5B\xA7\x83\xDC\x41\xD1\xB0\xE8"
                             "\x63\x42\x08\xBD\x46\x41\x34\x7C\x47\xDF\xC7\x87\x59\xDD\x3E\xCE\xD8\x67\x6C\xFB"
                             "\x3A\x81\x06\x10\x20\xC2\x38\xCE\x9C\x77\x8F\xB3\xC1\x07\x46\xC3\xA0\xE6\x3D\xBC"
                             "\x43\x2E\x85\x02\x17\x09\x77\xF0\xCE\xE8\xCC\xD3\xDE\x18\x7B\xE7\xBC\x71\x9E\xF9"
                             "\xDB\x67\x4D\x3A\x8E\xE1\x02\x14\xB3\x90\x81\x06\x59\xC8\x75\x33\xAA\x8F\x59\x10"
                             "\xDB\xA1\x47\x42\x18\x5A\x08\x38\x81\x8D\x08\x20\x42\xC6\xCC\x67\x52\x3E\x23\xEC"
                             "\xE4\x3A\x69\x0C\x36\x22\x33\x7F\x12\xFA\xCF\xB3\xC1\xB0\xF8\x32\xFF\xE6\x5B\xD4"
                             "\x77\x46\x1D\xE3\xB6\x72\x10\x22\x62\x45\x4C\xD9\x47\x74\xD8\x08\x99\xF9\xC6\x7B"
                             "\xE7\x6E\x10\x24\xE9\xC7\x21\xD2\x8E\xF1\xE0\x8C\xEF\xEB\xB0\x46\x8F\x88\x4C\x0C"
                             "\x58\xD7\xD4\x74\x0F\xEE\xE9\x93\x09\x8D\x7D\x47\x74\xFB\x20\x8B\x4F\xB0\x41\xC1"
                             "\x29\x9B\x28\x14\x70\x82\xA6\x6C\xA2\xEC\x11\x9D\xD3\xEC\x86\x16\x16\x9D\x67\xDA"
                             "\x3B\x68\xD8\x8E\xDA\x3E\xCF\x34\x8F\xB4\x76\xD2\x3B\xBF\x2B\x3D\xE0\x43\xE1\x58"
                             "\xE4\x3D\xF3\xD7\x74\x77\x8E\xD9\x02\x26\xE4\x7B\xE7\x78\xE9\x58\x46\x34\xD8\xC4"
                             "\x3B\xA7\x28\xEF\x1D\xC3\xB6\x77\x0F\xB3\xB8\x53\x1D\x99\x79\x06\xAE\x91\x0C\xCF"
                             "\x1E\x68\xFB\x47\x6C\x11\x78\x1D\x47\x6D\x1F\x68\xEA\x04\x7A\x07\x21\xEF\x9D\xE3"
                             "\xC1\x76\x08\xCE\x96\x30\x63\xE1\x08\x31\x5A\x10\x87\x74\x10\xF8\x03\x3B\xC7\x80"
                             "\x43\xE5\xA0\x12\x2D\x82\x0D\x73\xDC\x7B\x8D";
#define  HTTP_MLX90640_3a_SNS       Decompress(HTTP_MLX90640_3a_SNS_COMPRESSED,HTTP_MLX90640_3a_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_3a_SNS[] PROGMEM =
  	"function poiD(){" //poi draw
          "grdD();"
          "ctx.globalCompositeOperation = 'source-over';"
			    "var rO = new Range();" //rangeObject
			    "rO.selectNodeContents(eb('m2'));"
			    "rO.deleteContents();"
  				"for(var p in poi){"
  					//"console.log('poi:'+ poi[p][0]);"
            "var c=150;"
            "if(eb('poiL').value==p){c=255;}"
            // "console.log(c);"
            "ctx.fillStyle = 'rgba('+c+','+c+','+c+',0.6)';"
            "ctx.beginPath();"
            "ctx.arc(poi[p][0]+0.5, poi[p][1]+0.5,1,0,2*Math.PI);"
            "ctx.fill();"
            "ctx.font = '1.5px Verdana';"
            "x=parseInt(p)+1;"
            "ctx.fillText(x, poi[p][0]+1.5, poi[p][1]+1.2);"
            "var node = document.createElement('LI');"
            "var textnode = document.createTextNode('POI-' + x + ':  ' +  (rA[(poi[p][1]*32)+poi[p][0]]).toFixed(2) + ' °C at Pos: ' + poi[p][0] + ' , ' + poi[p][1]);"
            "node.appendChild(textnode);"
            "eb('m2').appendChild(node);"
  				"}"
  			"}"
    ;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_3b_SNS_SIZE = 477;
const char HTTP_MLX90640_3b_SNS_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x5E\x74\x2C\x61\xDD\x3B\xC7\xB7\xE1\x1D\xD3\xEC\x58\xC3\xEC"
                             "\xEA\x38\xCE\xF1\xE0\x83\xBE\x33\xBF\x23\xE2\x63\x8E\xE9\xF6\x34\x23\x61\xF6\x77"
                             "\x8F\x01\xD1\xB1\x1F\x10\x20\xD5\x3A\x0F\x3A\x20\xF3\xA9\x9B\x28\xEE\x9F\x67\x28"
                             "\x84\x3E\xC1\x0F\x0D\x3A\x58\x82\x13\x33\x7D\x44\x16\xFA\x9F\x3F\x9D\xD3\xEC\x6E"
                             "\x0B\xF3\x1B\x86\x6C\xFB\x3A\x90\x21\xE9\xC7\x75\x99\xD1\xDE\x47\xB4\xCE\xFE\x86"
                             "\x90\xC4\x7C\x43\xCE\x88\x6E\x0B\xF3\x21\x99\xE3\xBA\x08\x39\x29\xD4\x99\x9D\x1D"
                             "\xE3\xC3\x1C\x77\x4F\xB1\xA7\x21\xF6\x77\x8E\x85\xBD\xCF\xE4\x28\xA8\x86\x90\x48"
                             "\xF8\x8F\xB2\xA6\x34\x63\xFD\xD0\xBF\xB3\xCD\x1F\x68\xED\xA3\xBB\xF2\xB3\xDE\x3B"
                             "\xA3\x48\x61\xD0\xC8\xF5\x9C\xBA\x3B\xC7\x6C\x86\x90\xC3\xA1\xB0\xF7\xCE\x95\x84"
                             "\x63\x4D\x8C\x43\xBA\x72\x8E\xF1\xDE\x3B\x87\x6C\xEE\x1F\x67\x70\xAE\x91\x0C\xCF"
                             "\x02\x0E\x18\x34\x86\x1D\x0D\x88\xED\xA3\xED\x1D\x40\x87\x2C\xC8\xF0\x7B\x81\x6F"
                             "\x82\x01\x30\x7F\x81\x4B\x82\x02\x10\x15\xF8\x20\x33\xBF\xAD\x10\xD8\x08\x5C\x54"
                             "\x0C\xCD\x20\x8F\xB3\xBC\x74\x33\xB0\x85\xB3\xC0\xCC\xD3\xDE\xD1\x0D\x87\xBE\x7B"
                             "\xC7\x19\xEF\x9F\x08\x69\x08\x74\x36\x02\x2C\xD3\x90\xF7\xCF\x84\x34\x84\x3A\x19"
                             "\x1E\x06\xE7\x8E\xE9\xDE\x3C\x02\x1F\x1E\xA0\x97\x8E\x9E\xB3\x91\xB6\xCE\xD9\xDD"
                             "\x21\xA4\x21\xD0\xD8\x7A\xCE\x46\xCE\xF1\xDB\x21\xA4\x21\xD0\xC8\xEF\x1E\x0F\x71"
                             "\xDE\x3C\x1B";
#define  HTTP_MLX90640_3b_SNS       Decompress(HTTP_MLX90640_3b_SNS_COMPRESSED,HTTP_MLX90640_3b_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_3b_SNS[] PROGMEM =
      "function setup(){"
        "rl('up',0);"
        "canvas = eb('mlx');"
		    "ctx = canvas.getContext('2d');"
        "canvas.addEventListener('mousemove', function(evt) {"
          "var mP = getMousePos(canvas, evt);"
          // console.log((mP.y*32)+mP.x);
          "eb('m1').innerHTML = 'Temperature: ' + (rA[(mP.y*32)+mP.x].toFixed(2)) + ' at Pos: ' + mP.x + ' , ' + mP.y;"
        "});"
        "canvas.addEventListener('mousedown', function(evt) {"
          "var mD = getMousePos(canvas, evt);"
          "var idx = eb('poiL').value;"
          "poi[idx][0]=mD.x;"
          "poi[idx][1]=mD.y;"
		      //"console.log(poi);"
		      "mos();"
          "rl('up',eb('poiL').value*10000+(mD.x*100)+mD.y);" //poi-1: 2,14 -> 10214
        "});"
  ;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_4a_SNS_SIZE = 468;
const char HTTP_MLX90640_4a_SNS_COMPRESSED[] PROGMEM = "\xD3\x08\xEE\x87\x7C\x67\x7E\x3A\x0F\x3A\x20\xF3\xA9\x9B\x28\xEF\x23\xDA\x1D\x1B"
                             "\x0E\x9E\x0E\xC2\x67\x74\xE4\x67\x54\x67\x78\xF0\x41\xD1\xB0\xE9\xA3\x6C\x79\x97"
                             "\x86\xE6\x50\xAD\xE1\xE2\x7D\x63\x82\x62\x23\xE2\xAF\x8B\x67\x81\xEF\x88\x8F\x88"
                             "\x3A\x36\x1D\x03\xFB\xBA\x64\x16\xF3\xBF\x90\xF7\xEC\x4D\x7D\x47\x74\xE3\x3A\x8E"
                             "\xE3\x3A\x8E\xE3\x3A\x8E\xE5\x61\xDE\x3C\x10\xF7\xC4\x3A\x58\x82\x10\x78\x16\x7C"
                             "\xBD\x58\x30\xEE\x9C\x67\x51\xDC\x3E\xC8\xC9\x84\x16\x0F\x9F\x60\x9D\x68\xE8\x71"
                             "\xFB\x4E\xA3\xB8\x7D\x96\x7E\xF8\x79\x82\x85\xD3\x95\xA7\x51\xDC\x3E\xC8\xCF\x70"
                             "\x27\x40\xA1\x70\xE6\x69\xD4\x77\x0F\xB2\x12\xFE\x68\x38\x21\x60\xA1\x8F\x1C\x87"
                             "\x51\xDC\x3E\xC8\x70\x56\x19\xA0\x20\xD9\x21\x0E\xE9\xDE\x3C\x0F\x10\xC3\x60\x21"
                             "\x70\x5A\x3C\xE8\xB4\x6D\x8F\x32\x12\xEA\xCE\xE9\xCB\xAD\x3A\x8E\xE3\x3A\x8E\xE4"
                             "\x3A\xAA\xD1\xDE\x3C\x10\xDC\xF1\xDD\x3B\xC7\x8D\x1B\x63\xCC\xE9\x9C\x16\x58\x88"
                             "\xF8\x8C\x0B\xE0\xEB\x73\x91\xDD\x04\x2E\x29\xC4\xFD\x8F\x96\x8D\xB1\xE6\x77\x74"
                             "\x6D\x8F\x33\xA8\xE3\x3A\x99\xD5\x74\x75\x56\x1D\xE3\xC1\x0C\xCD\x21\x0E\xE9\xDE"
                             "\x3C\x1E\xE3\xDC\x7B\x81\x13\x12\x04\x1D\x74\xF6\x87\x46\xC3\xA1\x8D\x08\x22\xF5"
                             "\x19\x04\xD1\xF1\x0F\x7C\x43\xC0\x21\xD0\x2F\xB0\xE8\xEE\x9C\xBA\x20\x41\xE2\xB8"
                             "\xEA\x39\x58\x77\x8F\x07\xB8\xF4\x3B\x0B\xC1\xFF\x46\x51\xF8";
#define  HTTP_MLX90640_4a_SNS       Decompress(HTTP_MLX90640_4a_SNS_COMPRESSED,HTTP_MLX90640_4a_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_4a_SNS[] PROGMEM =
        "if (canvas.getContext) {"
          "ctx.scale(10,10);"
          "ctx.imageSmoothingEnabled = true;"
          "grd = ctx.createLinearGradient(0, 0, 0, 24);" // gradient
          "grd.addColorStop(0, 'yellow');"
          "grd.addColorStop(.075, 'orange');"
          "grd.addColorStop(.25, 'violet');"
          "grd.addColorStop(.45, 'darkblue');"
          "grd.addColorStop(1, 'black');"
          "grdD();"
          "gPx = ctx.getImageData(325, 0, 1,239);"
          "mos();"
            "image.onload = function () {"
                "ctx.drawImage(image,0,0,32,24);"
                "poiD();"
            "}"
        "}"
      "}"
    "function grdD(){"  //gradient draw()
      "ctx.fillStyle = grd;"
      "ctx.fillRect(32, 0, 2,24);}"
    "</script>"
;
#endif //USE_UNISHOX_COMPRESSION
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_MLX90640_4b_SNS_SIZE = 418;
const char HTTP_MLX90640_4b_SNS_COMPRESSED[] PROGMEM = "\x3D\x07\x60\x86\x4B\x38\x2C\xB1\x0F\x87\xDF\x9D\x0B\x18\x77\x4E\xF1\xE0\xFB\x3F"
                             "\x0F\x40\xEF\x8C\xEF\xCB\x44\x3E\x1F\x63\x42\x36\x1F\x68\x7F\x44\xA1\x47\xC3\xEC"
                             "\xE5\xE3\x3E\xCE\xE1\x0A\x7A\x3C\x2A\x2B\x8F\x87\xD9\xCA\xC6\x7D\x9F\x87\xA1\xD8"
                             "\x40\x83\x83\x9F\x87\xA0\x9A\x66\x7E\x1E\x87\x60\x9A\x66\x7E\x1E\x9E\x61\x30\xE9"
                             "\x68\x87\xC3\xEC\x66\x69\x04\x7D\xAC\xE0\xC5\x5F\x0F\x33\xE1\xF6\x37\x3C\x77\x4E"
                             "\xF1\xF6\x7E\x1E\x98\x32\xB7\x39\x19\xD8\x42\xD9\xF0\xFB\x38\xCF\xB3\xF0\x88\x61"
                             "\x61\x69\xD6\x72\x1E\x87\x61\x02\x0D\x40\x4B\xB8\x72\x10\x20\xDC\x39\x44\x0A\x77"
                             "\x0E\x51\x02\x0D\xC3\x96\x40\xA7\x70\xE5\x90\x20\xDC\x39\x84\x0A\x77\x0E\x61\x02"
                             "\x0D\xC3\x9A\x40\xA7\x70\xE6\x90\x20\xDC\x39\xC4\x08\xB7\x0E\xC0\x41\xE1\x2A\x01"
                             "\xFC\x3D\x04\xD3\x30\x41\xE2\x0C\xE4\x3E\xC8\x10\xF8\x5B\x13\x4C\xCF\xC2\x18\x58"
                             "\x5A\x75\x9C\x67\x99\xDC\x3D\x0B\xC3\x2F\x96\x88\x7C\x3E\xEC\xE4\x3E\xCF\xC3\xD0"
                             "\xEC\x2F\x0C\xBE\x3F\x26\x3B\x32\xF2\x0D\x1D\xDF\x3E\xF6\x7C\xEF\x02\x2E\x1E\x08"
                             "\x39\x11\xCA\x20\x44\xC8\x8E\xC1\xD8\x21\x91\xF8";
#define  HTTP_MLX90640_4b_SNS       Decompress(HTTP_MLX90640_4b_SNS_COMPRESSED,HTTP_MLX90640_4b_SNS_SIZE).c_str()
#else
const char HTTP_MLX90640_4b_SNS[] PROGMEM =
  "<body onload='setup();'>"
  "<canvas id='mlx' width='340' height='240'></canvas>"
  "<div></div>"
  "<select id='poiL' onchange='mos()'>"
	  "<option value='0'>POI-1</option>"
	  "<option value='1'>POI-2</option>"
	  "<option value='2'>POI-3</option>"
	  "<option value='3'>POI-4</option>"
	  "<option value='4'>POI-5</option>"
	  "<option value='5'>POI-6</option>"
	"</select>"
    "<div id='m1'></div>"
    "<div>POI-0: <span id='a1'></span>°C (sensor)</div>"
    "<div id='m2'></div>"
  "</body>"
  ;
#endif //USE_UNISHOX_COMPRESSION
void MLX90640UpdateGUI(void){
  WSContentStart_P("mlx");
  WSContentSendStyle();
  WSContentSend_P(HTTP_MLX90640_1_SNS);
  WSContentSend_P(HTTP_MLX90640_2a_SNS);
  WSContentSend_P(HTTP_MLX90640_2b_SNS);
  WSContentSend_P(HTTP_MLX90640_3a_SNS);
  WSContentSend_P(HTTP_MLX90640_3b_SNS);
  WSContentSend_P(HTTP_MLX90640_4a_SNS);
  WSContentSend_P(HTTP_MLX90640_4b_SNS);
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

void MLX90640HandleWebGuiResponse(void){
  char tmp[(MLX90640_POI_NUM*2)+4];
  WebGetArg("ul", tmp, sizeof(tmp));                  // update line
  if (strlen(tmp)) {
    uint8_t _line = atoi(tmp);
    // AddLog(LOG_LEVEL_DEBUG, "MLX90640: send line %u", _line);
    float _buf[65];
    if(_line==0){_buf[0]=1000+MLX90640.Ta;} //ambient temperature modulation hack
    else{_buf[0]=(float)_line;}
    memcpy((char*)&_buf[1],(char*)&MLX90640.To[_line*64],64*4);
    Webserver->send_P(200,PSTR("application/octet-stream"),(const char*)&_buf,65*4);
    return;
    }
  WebGetArg("up", tmp, sizeof(tmp));                  // update POI to browser
  if (strlen(tmp)==1) {
    Webserver->send_P(200,PSTR("application/octet-stream"),(const char*)&MLX90640.pois,MLX90640_POI_NUM*2);
    return;
    }
  else if (strlen(tmp)>2) {                           // receive updated POI from browser
    uint32_t _poi = atoi(tmp);
    uint32_t _poiNum = (_poi-(_poi%10000))/10000;
    MLX90640.pois[_poiNum*2] = (_poi%10000)/100;
    MLX90640.pois[(_poiNum*2)+1] = _poi%100;
    // AddLog(LOG_LEVEL_DEBUG, PSTR("RAW: %u, POI-%u: x: %u, y: %u"),_poi,_poiNum,MLX90640.pois[_poiNum],MLX90640.pois[_poiNum+1]);
    for(int i = 0;i<MLX90640_POI_NUM;i++){
    AddLog(LOG_LEVEL_DEBUG, PSTR("POI-%u: x: %u, y: %u"),i+1,MLX90640.pois[i*2],MLX90640.pois[(i*2)+1]);
    }
    return;
  }
}

void MLX90640HandleWebGui(void){
  if (!HttpCheckPriviledgedAccess()) { return; }
  MLX90640HandleWebGuiResponse();
  MLX90640UpdateGUI();
}
#endif // USE_WEBSERVER

/************************************************************************\
 * Command
\************************************************************************/
bool MLX90640Cmd(void){
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_MLX90640);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_MLX90640), disp_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kMLX90640_Commands);
    uint32_t _idx;
    switch (command_code) {
      case CMND_MLX90640_POI:
        if(XdrvMailbox.index>(MLX90640_POI_NUM-1)&&XdrvMailbox.index<1) return false;
        _idx = (XdrvMailbox.index-1)*2;
        if (XdrvMailbox.data_len > 0) {
          uint32_t _coord = TextToInt(XdrvMailbox.data);
          MLX90640.pois[_idx] = (_coord%10000)/100;
          if(MLX90640.pois[_idx]>31) MLX90640.pois[_idx]=31;
          MLX90640.pois[_idx+1] = _coord%100;
          if(MLX90640.pois[_idx+1]>23) MLX90640.pois[_idx+1]=23;
        }
        AddLog(LOG_LEVEL_INFO, PSTR("POI-%u = x:%u,y:%u"),XdrvMailbox.index,MLX90640.pois[_idx],MLX90640.pois[_idx+1]);
        Response_P(S_JSON_MLX90640_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      default:
    	  // else for Unknown command
    	  serviced = false;
    	break;
    }
  } else {
    return false;
  }
  return serviced;
}

/************************************************************************\
 * Init
\************************************************************************/
void MLX90640init()
{
  if (MLX90640.type || !I2cSetDevice(MLX90640_ADDRESS)) { return; }

  Wire.setClock(400000);
  int status = -1;
  if(!MLX90640.dumpedEE){
    status = MLX90640_DumpEE(MLX90640_ADDRESS, MLX90640.Frame);
    if (status != 0){
      AddLog(LOG_LEVEL_INFO, PSTR("Failed to load system parameters"));
    }
    else {
      AddLog(LOG_LEVEL_INFO, PSTR("MLX90640: started"));
      MLX90640.type = true;
    }
  MLX90640.params = new paramsMLX90640;
  }
}

/************************************************************************\
 * Run loop
\************************************************************************/
void MLX90640every100msec(){
    static uint32_t _job = 0;
    int status;
    uint32_t _time;

    if(!MLX90640.extractedParams){
      static uint32_t _chunk = 0;
      AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: will read chunk: %u"), _chunk);
      _time = millis();
      status = MLX90640_ExtractParameters(MLX90640.Frame, MLX90640.params, _chunk);
      if (status == 0){
        AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: parameter received after: %u msec, status: %u"), TimePassedSince(_time), status);
      }
      if (_chunk == 5) MLX90640.extractedParams = true;
      _chunk++;
      return;
    }

    switch(_job){
        case 0:
        if(MLX90640_SynchFrame(MLX90640_ADDRESS)!=0){
            _job=-1;
            AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: frame not ready"));
            break;
            }
        // _time = millis();
        status = MLX90640_GetFrameData(MLX90640_ADDRESS, MLX90640.Frame);
        // AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: got frame 0 in %u msecs, status: %i"), TimePassedSince(_time), status);
        break;
        case 1:
        MLX90640.Ta = MLX90640_GetTa(MLX90640.Frame, MLX90640.params);
        break;
        case 2:
        // _time = millis();
        MLX90640_CalculateTo(MLX90640.Frame, MLX90640.params, 0.95f, MLX90640.Ta - 8, MLX90640.To, 0);
        // AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: calculated temperatures in %u msecs"), TimePassedSince(_time));
        break;
        case 5:
        if(MLX90640_SynchFrame(MLX90640_ADDRESS)!=0){
            _job=4;
            break;
        }
        // _time = millis();
        status = MLX90640_GetFrameData(MLX90640_ADDRESS, MLX90640.Frame);
        // // AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: got frame 1 in %u msecs, status: %i"), TimePassedSince(_time), status);
        break;
        case 7:
        // _time = millis();
        MLX90640_CalculateTo(MLX90640.Frame, MLX90640.params, 0.95f, MLX90640.Ta - 8, MLX90640.To, 1);
        // AddLog(LOG_LEVEL_DEBUG, PSTR("MLX90640: calculated temperatures in %u msecs"), TimePassedSince(_time));
        break;
        default:
        break;
    }
    _job++;
    if(_job>10) _job=0;
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void MLX90640Show(uint8_t json)
{
  char amb_tstr[FLOATSZ];
  dtostrfd(MLX90640.Ta, Settings.flag2.temperature_resolution, amb_tstr);
  if (json) {
    ResponseAppend_P(PSTR(",\"MLX90640\":{\"" D_JSON_TEMPERATURE "\":[%s"), amb_tstr);
  for(int i = 0;i<MLX90640_POI_NUM;i++){
    char obj_tstr[FLOATSZ];
    dtostrfd(MLX90640.To[MLX90640.pois[i*2]+(MLX90640.pois[(i*2)+1]*32)], Settings.flag2.temperature_resolution, obj_tstr);
    ResponseAppend_P(PSTR(",%s"),obj_tstr);
    // AddLog(LOG_LEVEL_DEBUG, PSTR("Array pos: %u"),MLX90640.pois[i*2]+(MLX90640.pois[(i*2)+1]*32));
    AddLog(LOG_LEVEL_DEBUG, PSTR("POI-%u: x: %u, y: %u"),i+1,MLX90640.pois[i*2],MLX90640.pois[(i*2)+1]);
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv43(uint8_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    MLX90640init();
  }
  if(MLX90640.type){
    switch (function) {
      case FUNC_EVERY_100_MSECOND:
      MLX90640every100msec();
      break;
      case FUNC_JSON_APPEND:
        MLX90640Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_MAIN_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_MLX90640);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/mlx"), MLX90640HandleWebGui);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = MLX90640Cmd();
        break;
    }
  }
  return result;
}

#endif  // USE_MLX90640_SENSOR
#endif // USE_I2C
