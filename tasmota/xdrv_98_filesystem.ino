/*
  xdrv_98_filesystem.ino - unified file system for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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

#ifdef USE_UFILESYS
/*********************************************************************************************\
This driver adds universal file system support for ESP8266 (sd card or littlfs on  > 1 M devices
with special linker file e.g. eagle.flash.4m2m.ld) (makes no sense on 1M devices without sd card)
and ESP32 (sd card or little fs or sfatfile system).

The sd card chip select is the standard SDCARD_CS or when not found SDCARD_CS_PIN and initializes
the FS System Pointer ufsp which can be used by all standard file system calls.

The only specific call is ufs_fsinfo() which gets the total size (0) and free size (1).

A button is created in the setup section to show up the file directory to download and upload files
subdirectories are supported.

console calls :

ufs       fs info
ufstype   get filesytem type 0=none 1=SD  2=Flashfile
ufssize   total size in kB
ufsfree   free size in kB

The driver enabled by #define USE_UFILESYS
\*********************************************************************************************/

#define XDRV_98           98

#ifndef SDCARD_CS_PIN
#define SDCARD_CS_PIN     4
#endif

#ifdef ESP8266
#include <LittleFS.h>
#include <SPI.h>
#ifdef USE_SDCARD
#include <SD.h>
#include <SDFAT.h>
#endif  // USE_SDCARD
#endif  // ESP8266

#ifdef ESP32
#include <LITTLEFS.h>
#ifdef USE_SDCARD
#include <SD.h>
#endif  // USE_SDCARD
#include "FFat.h"
#include "FS.h"
#endif  // ESP32

#define UFS_FILE_WRITE "w"
#define UFS_FILE_READ "r"

// global file system pointer
FS *ufsp;
char ufs_path[48];
File ufs_upload_file;

// 0 = none, 1 = SD, 2 = ffat, 3 = littlefs
// spiffs should be obsolete
uint8_t ufs_type;
#define UFS_TNONE 0
#define UFS_TSDC 1
#define UFS_TFAT 2
#define UFS_TLFS 3

void UFSInit(void) {
  ufs_type = 0;
  // check for fs options,
  // 1. check for SD card
  // 2. check for littlefs or FAT


#ifdef USE_SDCARD
  if (TasmotaGlobal.spi_enabled) {
//  if (1) {
    int8_t cs = SDCARD_CS_PIN;
    if (PinUsed(GPIO_SDCARD_CS)) {
      cs = Pin(GPIO_SDCARD_CS);
    }

    if (SD.begin(cs)) {
#ifdef ESP8266
      ufsp = &SDFS;
#endif  // ESP8266
#ifdef ESP32
      ufsp = &SD;
#endif  // ESP32
      ufs_type = UFS_TSDC;
      return;
    }
  }
#endif // USE_SDCARD

// if no success with sd card try flash fs
#ifdef ESP8266
  ufsp = &LittleFS;
  if (!LittleFS.begin()) {
    return;
  }
#endif  // ESP8266
#ifdef ESP32
  // try lfs first
  ufsp = &LITTLEFS;
  if (!LITTLEFS.begin(true)) {
    // ffat is second
    ufsp = &FFat;
    if (!FFat.begin(true)) {
      return;
    }
    ufs_type = UFS_TFAT;
    return;
  }
#endif // ESP32
  ufs_type = UFS_TLFS;
  return;
}

uint32_t ufs_fsinfo(uint32_t sel) {
  uint32_t result = 0;

#ifdef ESP8266
  FSInfo64 fsinfo;
#endif  // ESP8266

  switch (ufs_type) {
    case UFS_TSDC:
#ifdef USE_SDCARD
#ifdef ESP8266
      ufsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = SD.totalBytes();
      } else {
        result = (SD.totalBytes() - SD.usedBytes());
      }
#endif
#endif //USE_SDCARD
      break;

    case UFS_TLFS:
#ifdef ESP8266
      ufsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = LITTLEFS.totalBytes();
      } else {
        result = LITTLEFS.totalBytes() - LITTLEFS.usedBytes();
      }
#endif // ESP32
      break;

    case UFS_TFAT:
#ifdef ESP32
      if (sel == 0) {
        result = FFat.totalBytes();
      } else {
        result = FFat.freeBytes();
      }
#endif // ESP32
      break;

  }
  return result / 1000;
}

#if USE_LONG_FILE_NAMES>0
#undef REJCMPL
#define REJCMPL 6
#else
#undef REJCMPL
#define REJCMPL 8
#endif

uint8_t ufs_reject(char *name) {
  char *lcp = strrchr(name,'/');
  if (lcp) {
    name = lcp + 1;
  }

  while (*name=='/') { name++; }
  if (*name=='_') { return 1; }
  if (*name=='.') { return 1; }

  if (!strncasecmp(name, "SPOTLI~1", REJCMPL)) { return 1; }
  if (!strncasecmp(name, "TRASHE~1", REJCMPL)) { return 1; }
  if (!strncasecmp(name, "FSEVEN~1", REJCMPL)) { return 1; }
  if (!strncasecmp(name, "SYSTEM~1", REJCMPL)) { return 1; }
  if (!strncasecmp(name, "System Volume", 13)) { return 1; }
  return 0;
}

// format number with thousand marker
void UFS_form1000(uint32_t number, char *dp, char sc) {
  char str[32];
  sprintf(str, "%d", number);
  char *sp = str;
  uint32_t inum = strlen(sp)/3;
  uint32_t fnum = strlen(sp)%3;
  if (!fnum) { inum--; }
  for (uint32_t count = 0; count <= inum; count++) {
    if (fnum) {
      memcpy(dp, sp, fnum);
      dp += fnum;
      sp += fnum;
      fnum = 0;
    } else {
      memcpy(dp, sp, 3);
      dp += 3;
      sp += 3;
    }
    if (count != inum) {
      *dp++ = sc;
    }
  }
  *dp = 0;
}

const char kUFSCommands[] PROGMEM = "Ufs" "|"  // Prefix
  "|" "Type" "|" "Size" "|" "Free";

void (* const kUFSCommand[])(void) PROGMEM = {
    &UFS_info, &UFS_type, &UFS_size, &UFS_free};

void UFS_info(void) {
  Response_P(PSTR("{\"Ufs\":{\"Type\":%d,\"Size\":%d,\"Free\":%d}}"), ufs_type, ufs_fsinfo(0), ufs_fsinfo(1));
}

void UFS_type(void) {
  ResponseCmndNumber(ufs_type);
}
void UFS_size(void) {
  ResponseCmndNumber(ufs_fsinfo(0));
}
void UFS_free(void) {
  ResponseCmndNumber(ufs_fsinfo(1));
}

const char UFS_WEB_DIR[] PROGMEM =
  "<p><form action='" "ufsd" "' method='get'><button>" "%s" "</button></form></p>";
const char UFS_FILE_UPLOAD[] PROGMEM = D_SDCARD_DIR;
const char UFS_FORM_FILE_UPLOAD[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;%s"  "&nbsp;</b></legend>";
const char UFS_FORM_FILE_UPG[] PROGMEM =
  "<form method='post' action='ufsu' enctype='multipart/form-data'>"
  "<br/><input type='file' name='ufsu'><br/>"
  "<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>";
const char UFS_FORM_FILE_UPGc[] PROGMEM =
  "<div style='text-align:left;color:green;'>total size: %s kB - free: %s kB</div>";
const char UFS_FORM_SDC_DIRa[] PROGMEM =
  "<div style='text-align:left'>";
const char UFS_FORM_SDC_DIRc[] PROGMEM =
  "</div>";
const char UFS_FORM_FILE_UPGb[] PROGMEM =
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char UFS_FORM_SDC_DIRd[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a></pre>";
const char UFS_FORM_SDC_DIRb[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a>     %s : %8d</pre>";
const char UFS_FORM_SDC_HREF[] PROGMEM =
  "http://%s/ufsd?download=%s/%s";

void UFSdirectory(void) {
  uint8_t depth = 0;

  strcpy(ufs_path, "/");
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("download")) {
    String stmp = Webserver->arg("download");
    char *cp = (char*)stmp.c_str();
    if (UFS_DownloadFile(cp)) {
      // is directory
      strcpy(ufs_path, cp);
    }
  }

  WSContentStart_P(UFS_FILE_UPLOAD);
  WSContentSendStyle();
  WSContentSend_P(UFS_FORM_FILE_UPLOAD,D_SDCARD_DIR);
  WSContentSend_P(UFS_FORM_FILE_UPG, D_SCRIPT_UPLOAD);
  char ts[16];
  char fs[16];
  UFS_form1000(ufs_fsinfo(0), ts, '.');
  UFS_form1000(ufs_fsinfo(1), fs, '.');
  WSContentSend_P(UFS_FORM_FILE_UPGc, ts, fs);
  WSContentSend_P(UFS_FORM_SDC_DIRa);
  if (ufs_type) {
    UFS_ListDir(ufs_path, depth);
  }
  WSContentSend_P(UFS_FORM_SDC_DIRc);
  WSContentSend_P(UFS_FORM_FILE_UPGb);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
  Web.upload_error = 0;
}

void UFS_ListDir(char *path, uint8_t depth) {
  char name[32];
  char npath[128];
  char format[12];
  sprintf(format, "%%-%ds", 24 - depth);

  File dir = ufsp->open(path, UFS_FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      snprintf_P(npath, sizeof(npath), PSTR("http://%s/ufsd?download=%s"), WiFi.localIP().toString().c_str(), path);
      for (uint32_t cnt = strlen(npath) - 1; cnt > 0; cnt--) {
        if (npath[cnt] == '/') {
          if (npath[cnt - 1] == '=') {
            npath[cnt + 1] = 0;
          } else {
            npath[cnt] = 0;
          }
          break;
        }
      }
      WSContentSend_P(UFS_FORM_SDC_DIRd, npath, path, "..");
    }
    char *ep;
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) {
        break;
      }
      // esp32 returns path here, shorten to filename
      ep = (char*)entry.name();
      if (*ep == '/') { ep++; }
      char *lcp = strrchr(ep,'/');
      if (lcp) {
        ep = lcp + 1;
      }
      time_t tm = entry.getLastWrite();
      char tstr[24];
      strftime(tstr, 22, "%d-%m-%Y - %H:%M:%S ", localtime(&tm));  // Theo note to me. Isn't strftime expensive? SHould use ISO Date/Time

      char *pp = path;
      if (!*(pp + 1)) { pp++; }
      char *cp = name;
      // osx formatted disks contain a lot of stuff we dont want
      if (!ufs_reject((char*)ep)) {

        for (uint8_t cnt = 0; cnt<depth; cnt++) {
          *cp++ = '-';
        }

        sprintf(cp, format, ep);
        if (entry.isDirectory()) {
          snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, WiFi.localIP().toString().c_str(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRd, npath, ep, name);
          uint8_t plen = strlen(path);
          if (plen > 1) {
            strcat(path, "/");
          }
          strcat(path, ep);
          UFS_ListDir(path, depth + 4);
          path[plen] = 0;
        } else {
          snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, WiFi.localIP().toString().c_str(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRb, npath, ep, name, tstr, entry.size());
        }
      }
      entry.close();
    }
    dir.close();
  }
}

uint8_t UFS_DownloadFile(char *file) {
  File download_file;
  WiFiClient download_Client;

  if (!ufsp->exists(file)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("UFS: File not found"));
    return 0;
  }

  download_file = ufsp->open(file, UFS_FILE_READ);
  if (!download_file) {
    AddLog_P(LOG_LEVEL_INFO, PSTR("UFS: Could not open file"));
    return 0;
  }

  if (download_file.isDirectory()) {
    download_file.close();
    return 1;
  }

  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp;
  for (uint32_t cnt = strlen(file); cnt >= 0; cnt--) {
    if (file[cnt] == '/') {
      cp = &file[cnt + 1];
      break;
    }
  }
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
  Webserver->sendHeader(F("Content-Disposition"), attachment);
  WSSend(200, CT_STREAM, "");

  uint8_t buff[512];
  uint32_t bread;

  // transfer is about 150kb/s
  uint32_t cnt = 0;
  while (download_file.available()) {
    bread = download_file.read(buff, sizeof(buff));
    uint32_t bw = download_Client.write((const char*)buff, bread);
    if (!bw) { break; }
    cnt++;
    if (cnt > 7) {
      cnt = 0;
      //if (glob_script_mem.script_loglevel & 0x80) {
        // this indeed multitasks, but is slower 50 kB/s
      //  loop();
      //}
    }
    delay(0);
  }
  download_file.close();
  download_Client.stop();
  return 0;
}

void UFS_Upload(void) {
  HTTPUpload& upload = Webserver->upload();
  if (upload.status == UPLOAD_FILE_START) {
    char npath[48];
    sprintf(npath, "%s/%s", ufs_path, upload.filename.c_str());
    ufsp->remove(npath);
    ufs_upload_file = ufsp->open(npath, UFS_FILE_WRITE);
    if (!ufs_upload_file) { Web.upload_error = 1; }
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (ufs_upload_file) {
      ufs_upload_file.write(upload.buf, upload.currentSize);
    }
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (ufs_upload_file) { ufs_upload_file.close(); }
    if (Web.upload_error) {
      AddLog_P(LOG_LEVEL_INFO, PSTR("HTP: upload error"));
    }
  } else {
    Web.upload_error = 1;
    WSSend(500, CT_PLAIN, F("500: couldn't create file"));
  }
}

void UFSFileUploadSuccess(void) {
  WSContentStart_P(PSTR(D_INFORMATION));
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
  WSContentSend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), WebColor(COL_TEXT_SUCCESS));
  WSContentSend_P(PSTR("</div><br/>"));
  WSContentSend_P(PSTR("<p><form action='%s' method='get'><button>%s</button></form></p>"), "/ufsd", D_UPL_DONE);
  WSContentStop();
}

#define D_UFSDIR "UFS directory"

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv98(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      UFSInit();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kUFSCommands, kUFSCommand);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      if (ufs_type) {
        WSContentSend_PD(UFS_WEB_DIR,D_UFSDIR);
      }
      break;
    case FUNC_WEB_ADD_HANDLER:
      Webserver->on("/ufsd", UFSdirectory);
      Webserver->on("/ufsu", HTTP_GET, UFSFileUploadSuccess);
      Webserver->on("/ufsu", HTTP_POST,[]() {
        Webserver->sendHeader("Location","/ufsu");
        Webserver->send(303);
      }, UFS_Upload);
      break;
#endif // USE_WEBSERVER
  }
  return result;
}
#endif  // USE_UFILESYS
