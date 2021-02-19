/*
  xdrv_50_filesystem.ino - unified file system for Tasmota

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
This driver adds universal file system support for
- ESP8266 (sd card or littlefs on  > 1 M devices with special linker file e.g. eagle.flash.4m2m.ld)
  (makes no sense on 1M devices without sd card)
- ESP32 (sd card or littlefs or sfatfile system).

The sd card chip select is the standard SDCARD_CS or when not found SDCARD_CS_PIN and initializes
the FS System Pointer ufsp which can be used by all standard file system calls.

The only specific call is UfsInfo() which gets the total size (0) and free size (1).

A button is created in the setup section to show up the file directory to download and upload files
subdirectories are supported.

Supported commands:
ufs       fs info
ufstype   get filesytem type 0=none 1=SD  2=Flashfile
ufssize   total size in kB
ufsfree   free size in kB
\*********************************************************************************************/

#define XDRV_50           50

#ifndef SDCARD_CS_PIN
#define SDCARD_CS_PIN     4
#endif

#define UFS_TNONE         0
#define UFS_TSDC          1
#define UFS_TFAT          2
#define UFS_TLFS          3

/*
// In tasmota.ino
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
*/

// Global file system pointer
FS *ufsp;
// Flash file system pointer
FS *ffsp;
// Local pointer for file managment
FS *dfsp;

char ufs_path[48];
File ufs_upload_file;
uint8_t ufs_dir;
// 0 = None, 1 = SD, 2 = ffat, 3 = littlefs
uint8_t ufs_type;
uint8_t ffs_type;

struct {
  char run_file[48];
  int run_file_pos = -1;
  bool run_file_mutex = 0;
  bool download_busy;
} UfsData;

/*********************************************************************************************/

// Init flash file system
void UfsInitOnce(void) {
  ufs_type = 0;
  ffsp = 0;
  ufs_dir = 0;

#ifdef ESP8266
  ffsp = &LittleFS;
  if (!LittleFS.begin()) {
    ffsp = 0;
    return;
  }
#endif  // ESP8266

#ifdef ESP32
  // try lfs first
  ffsp = &LITTLEFS;
  if (!LITTLEFS.begin(true)) {
    // ffat is second
    ffsp = &FFat;
    if (!FFat.begin(true)) {
      return;
    }
    ffs_type = UFS_TFAT;
    ufs_type = ffs_type;
    ufsp = ffsp;
    dfsp = ffsp;
    return;
  }
#endif // ESP32
  ffs_type = UFS_TLFS;
  ufs_type = ffs_type;
  ufsp = ffsp;
  dfsp = ffsp;
}

// Called from tasmota.ino at restart. This inits flash file only
void UfsInit(void) {
  UfsData.run_file_pos = -1;
  UfsInitOnce();
  if (ufs_type) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: FlashFS mounted with %d kB free"), UfsInfo(1, 0));
  }
}

#ifdef USE_SDCARD
void UfsCheckSDCardInit(void) {
  if (TasmotaGlobal.spi_enabled) {
    int8_t cs = SDCARD_CS_PIN;
    if (PinUsed(GPIO_SDCARD_CS)) {
      cs = Pin(GPIO_SDCARD_CS);
    }

#ifdef EPS8266
    SPI.begin();
#endif // EPS8266

#ifdef ESP32
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif // ESP32

    if (SD.begin(cs)) {
#ifdef ESP8266
      ufsp = &SDFS;
#endif  // ESP8266

#ifdef ESP32
      ufsp = &SD;
#endif  // ESP32
      ufs_type = UFS_TSDC;
      dfsp = ufsp;
      if (ffsp) {ufs_dir = 1;}
      // make sd card the global filesystem
#ifdef ESP8266
      // on esp8266 sdcard info takes several seconds !!!, so we ommit it here
      AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted"));
#endif // ESP8266
#ifdef ESP32
      AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted with %d kB free"), UfsInfo(1, 0));
#endif // ESP32
    }
  }
}
#endif // USE_SDCARD

uint32_t UfsInfo(uint32_t sel, uint32_t type) {
  uint64_t result = 0;
  FS *ifsp = ufsp;
  uint8_t itype = ufs_type;
  if (type) {
    ifsp = ffsp;
    itype = ffs_type;
  }

#ifdef ESP8266
  FSInfo64 fsinfo;
#endif  // ESP8266

  switch (itype) {
    case UFS_TSDC:
#ifdef USE_SDCARD
#ifdef ESP8266
      ifsp->info64(fsinfo);
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
#endif  // ESP32
#endif  // USE_SDCARD
      break;

    case UFS_TLFS:
#ifdef ESP8266
      ifsp->info64(fsinfo);
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
#endif  // ESP32
      break;

    case UFS_TFAT:
#ifdef ESP32
      if (sel == 0) {
        result = FFat.totalBytes();
      } else {
        result = FFat.freeBytes();
      }
#endif  // ESP32
      break;

  }
  return result / 1024;
}

#if USE_LONG_FILE_NAMES>0
#undef REJCMPL
#define REJCMPL 6
#else
#undef REJCMPL
#define REJCMPL 8
#endif

uint8_t UfsReject(char *name) {
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

/*********************************************************************************************\
 * Tfs low level functions
\*********************************************************************************************/

bool TfsFileExists(const char *fname){
  if (!ffs_type) { return false; }

  bool yes = ffsp->exists(fname);
  if (!yes) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TFS: File not found"));
  }
  return yes;
}

bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) {
  if (!ffs_type) { return false; }

  File file = ffsp->open(fname, "w");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Save failed"));
    return false;
  }

  file.write(buf, len);
  file.close();
  return true;
}

bool TfsInitFile(const char *fname, uint32_t len, uint8_t init_value) {
  if (!ffs_type) { return false; }

  File file = ffsp->open(fname, "w");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Erase failed"));
    return false;
  }

  for (uint32_t i = 0; i < len; i++) {
    file.write(&init_value, 1);
  }
  file.close();
  return true;
}

bool TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len) {
  if (!ffs_type) { return false; }
  if (!TfsFileExists(fname)) { return false; }

  File file = ffsp->open(fname, "r");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: File not found"));
    return false;
  }

  file.read(buf, len);
  file.close();
  return true;
}

bool TfsDeleteFile(const char *fname) {
  if (!ffs_type) { return false; }

  if (!ffsp->remove(fname)) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Delete failed"));
    return false;
  }
  return true;
}

bool TfsRenameFile(const char *fname1, const char *fname2) {
  if (!ffs_type) { return false; }

  if (!ffsp->rename(fname1, fname2)) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Rename failed"));
    return false;
  }
  return true;
}

/*********************************************************************************************\
 * File command execute support
\*********************************************************************************************/

bool UfsExecuteCommandFileReady(void) {
  return (UfsData.run_file_pos < 0);   // Check file ready to disable concurrency
}

void UfsExecuteCommandFileLoop(void) {
  if (UfsExecuteCommandFileReady() || !ffs_type) { return; }

  if (TimeReached(TasmotaGlobal.backlog_timer) && strlen(UfsData.run_file) && !UfsData.run_file_mutex) {
    File file = ffsp->open(UfsData.run_file, "r");
    if (!file || !file.seek(UfsData.run_file_pos)) {
      UfsData.run_file_pos = -1;       // Signal file ready
      return;
    }

    UfsData.run_file_mutex = true;

    char cmd_line[512];
    cmd_line[0] = '\0';                // Clear in case of re-entry
    while (file.available()) {
      uint16_t index = 0;
      bool comment = false;
      while (file.available()) {
        uint8_t buf[1];
        file.read(buf, 1);
        if ((buf[0] == '\n') || (buf[0] == '\r')) {
          break;                       // End of command with linefeed or carriage return
        }
        else if (index && !comment && (buf[0] == ';')) {
          break;                       // End of command on multi command line
        }
        else if ((0 == index) && isspace(buf[0])) {
                                       // Skip leading spaces (' ','\t','\n','\v','\f','\r')
        }
        else if ((0 == index) && (buf[0] == ';')) {
          comment = true;              // Ignore comment lines until linefeed or carriage return
        }
        else if (!comment && (index < sizeof(cmd_line) - 2)) {
          cmd_line[index++] = buf[0];  // Build command
        }
      }
      if ((index > 0) && (index < sizeof(cmd_line) - 1)) {
        cmd_line[index] = '\0';        // Valid command received
        break;
      }
    }
    UfsData.run_file_pos = (file.available()) ? file.position() : -1;
    file.close();
    if (strlen(cmd_line)) {
      bool nodelay = (!(!strncasecmp_P(cmd_line, PSTR(D_CMND_DELAY), strlen(D_CMND_DELAY))));
      ExecuteCommand(cmd_line, SRC_FILE);
      if (nodelay) {
        TasmotaGlobal.backlog_timer = millis();  // Reset backlog_timer which has been set by ExecuteCommand (CommandHandler)
      }
    }

    UfsData.run_file_mutex = false;
  }
}

bool UfsExecuteCommandFile(const char *fname) {
  // Check for non-concurrency and file existance
  if (UfsExecuteCommandFileReady() && TfsFileExists(fname)) {
    snprintf(UfsData.run_file, sizeof(UfsData.run_file), fname);
    UfsData.run_file_pos = 0;          // Signal start of file
    return true;
  }
  return false;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const int UFS_FILENAME_SIZE = 48;

char* UfsFilename(char* fname, char* fname_in) {
  fname_in = Trim(fname_in);  // Remove possible leading spaces
  snprintf_P(fname, UFS_FILENAME_SIZE, PSTR("%s%s"), ('/' == fname_in[0]) ? "" : "/", fname_in);
  return fname;
}

const char kUFSCommands[] PROGMEM = "Ufs|"  // Prefix
  "|Type|Size|Free|Delete|Rename|Run";

void (* const kUFSCommand[])(void) PROGMEM = {
  &UFSInfo, &UFSType, &UFSSize, &UFSFree, &UFSDelete, &UFSRename, &UFSRun};

void UFSInfo(void) {
  Response_P(PSTR("{\"Ufs\":{\"Type\":%d,\"Size\":%d,\"Free\":%d}"), ufs_type, UfsInfo(0, 0), UfsInfo(1, 0));
  if (ffs_type && (ffs_type != ufs_type)) {
    ResponseAppend_P(PSTR(",{\"Type\":%d,\"Size\":%d,\"Free\":%d}"), ffs_type, UfsInfo(0, 1), UfsInfo(1, 1));
  }
  ResponseJsonEnd();
}

void UFSType(void) {
  if (ffs_type && (ffs_type != ufs_type)) {
    Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, ufs_type, ffs_type);
  } else {
    ResponseCmndNumber(ufs_type);
  }
}

void UFSSize(void) {
  if (ffs_type && (ffs_type != ufs_type)) {
    Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, UfsInfo(0, 0), UfsInfo(0, 1));
  } else {
    ResponseCmndNumber(UfsInfo(0, 0));
  }
}

void UFSFree(void) {
  if (ffs_type && (ffs_type != ufs_type)) {
    Response_P(PSTR("{\"%s\":[%d,%d]}"), XdrvMailbox.command, UfsInfo(1, 0), UfsInfo(1, 1));
  } else {
    ResponseCmndNumber(UfsInfo(1, 0));
  }
}

void UFSDelete(void) {
  // UfsDelete  sdcard or flashfs file if only one of them available
  // UfsDelete2 flashfs file if available
  if (XdrvMailbox.data_len > 0) {
    char fname[UFS_FILENAME_SIZE];
    UfsFilename(fname, XdrvMailbox.data);
    bool result = false;
    if (ffs_type && (ffs_type != ufs_type) && (2 == XdrvMailbox.index)) {
      result = TfsDeleteFile(fname);
    } else {
      result = (ufs_type && ufsp->remove(fname));
    }
    if (!result) {
      ResponseCmndFailed();
    } else {
      ResponseCmndDone();
    }
  }
}

void UFSRename(void) {
  // UfsRename  sdcard or flashfs file if only one of them available
  // UfsRename2 flashfs file if available
  if (XdrvMailbox.data_len > 0) {
    bool result = false;
    char *fname1 = strtok(XdrvMailbox.data, ",");
    char *fname2 = strtok(nullptr, ",");
    if (fname1 && fname2) {
      char fname_old[UFS_FILENAME_SIZE];
      UfsFilename(fname_old, fname1);
      char fname_new[UFS_FILENAME_SIZE];
      UfsFilename(fname_new, fname2);
      if (ffs_type && (ffs_type != ufs_type) && (2 == XdrvMailbox.index)) {
        result = TfsRenameFile(fname_old, fname_new);
      } else {
        result = (ufs_type && ufsp->rename(fname_old, fname_new));
      }
    }
    if (!result) {
      ResponseCmndFailed();
    } else {
      ResponseCmndDone();
    }
  }
}

void UFSRun(void) {
  if (XdrvMailbox.data_len > 0) {
    char fname[UFS_FILENAME_SIZE];
    if (UfsExecuteCommandFile(UfsFilename(fname, XdrvMailbox.data))) {
      ResponseClear();
    } else {
      ResponseCmndFailed();
    }
  } else {
    bool not_active = UfsExecuteCommandFileReady();
    UfsData.run_file_pos = -1;
    ResponseCmndChar(not_active ? PSTR(D_JSON_DONE) : PSTR(D_JSON_ABORTED));
  }
}

/*********************************************************************************************\
 * Web support
\*********************************************************************************************/

#ifdef USE_WEBSERVER

const char UFS_WEB_DIR[] PROGMEM =
  "<p><form action='" "ufsd" "' method='get'><button>" "%s" "</button></form></p>";

const char UFS_FORM_FILE_UPLOAD[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_MANAGE_FILE_SYSTEM "&nbsp;</b></legend>";
const char UFS_FORM_FILE_UPGc[] PROGMEM =
  "<div style='text-align:left;color:#%06x;'>" D_FS_SIZE " %s MB - " D_FS_FREE " %s MB";

const char UFS_FORM_FILE_UPGc1[] PROGMEM =
    " &nbsp;&nbsp;<a href='http://%_I/ufsd?dir=%d'>%s</a>";

const char UFS_FORM_FILE_UPGc2[] PROGMEM =
  "</div>";

const char UFS_FORM_FILE_UPG[] PROGMEM =
  "<form method='post' action='ufsu' enctype='multipart/form-data'>"
  "<br><input type='file' name='ufsu'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
  "<br>";
const char UFS_FORM_SDC_DIRa[] PROGMEM =
  "<div style='text-align:left;overflow:auto;height:250px;'>";
const char UFS_FORM_SDC_DIRc[] PROGMEM =
  "</div>";
const char UFS_FORM_FILE_UPGb[] PROGMEM =
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char UFS_FORM_SDC_DIRd[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a></pre>";
const char UFS_FORM_SDC_DIRb[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a> %s %8d %s</pre>";
const char UFS_FORM_SDC_HREF[] PROGMEM =
  "http://%_I/ufsd?download=%s/%s";
#ifdef GUI_TRASH_FILE
const char UFS_FORM_SDC_HREFdel[] PROGMEM =
  //"<a href=http://%_I/ufsd?delete=%s/%s>&#128465;</a>";
  "<a href=http://%_I/ufsd?delete=%s/%s>&#128293;</a>"; // 🔥
#endif // GUI_TRASH_FILE

void UfsDirectory(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MANAGE_FILE_SYSTEM));

  uint8_t depth = 0;

  strcpy(ufs_path, "/");

  if (Webserver->hasArg(F("download"))) {
    String stmp = Webserver->arg(F("download"));
    char *cp = (char*)stmp.c_str();
    if (UfsDownloadFile(cp)) {
      // is directory
      strcpy(ufs_path, cp);
    } else {
      return;
    }
  }

  if (Webserver->hasArg(F("dir"))) {
    String stmp = Webserver->arg(F("dir"));
    ufs_dir = atoi(stmp.c_str());
    if (ufs_dir == 1) {
      dfsp = ufsp;
    } else {
      if (ffsp) {
        dfsp = ffsp;
      }
    }
  }

  if (Webserver->hasArg(F("delete"))) {
    String stmp = Webserver->arg(F("delete"));
    char *cp = (char*)stmp.c_str();
    dfsp->remove(cp);
  }

  WSContentStart_P(PSTR(D_MANAGE_FILE_SYSTEM));
  WSContentSendStyle();
  WSContentSend_P(UFS_FORM_FILE_UPLOAD);

  char ts[FLOATSZ];
  dtostrfd((float)UfsInfo(0, ufs_dir == 2 ? 1:0) / 1000, 3, ts);
  char fs[FLOATSZ];
  dtostrfd((float)UfsInfo(1, ufs_dir == 2 ? 1:0) / 1000, 3, fs);
  WSContentSend_PD(UFS_FORM_FILE_UPGc, WebColor(COL_TEXT), ts, fs);

  if (ufs_dir) {
    WSContentSend_P(UFS_FORM_FILE_UPGc1, (uint32_t)WiFi.localIP(), (ufs_dir == 1)?2:1, (ufs_dir == 1)?PSTR("SDCard"):PSTR("FlashFS"));
  }
  WSContentSend_P(UFS_FORM_FILE_UPGc2);

  WSContentSend_P(UFS_FORM_FILE_UPG, PSTR(D_SCRIPT_UPLOAD));

  WSContentSend_P(UFS_FORM_SDC_DIRa);
  if (ufs_type) {
    UfsListDir(ufs_path, depth);
  }
  WSContentSend_P(UFS_FORM_SDC_DIRc);
  WSContentSend_P(UFS_FORM_FILE_UPGb);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();

  Web.upload_file_type = UPL_UFSFILE;
}

void UfsListDir(char *path, uint8_t depth) {
  char name[32];
  char npath[128];
  char format[12];
  sprintf(format, PSTR("%%-%ds"), 24 - depth);

  File dir = dfsp->open(path, UFS_FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      ext_snprintf_P(npath, sizeof(npath), PSTR("http://%_I/ufsd?download=%s"), (uint32_t)WiFi.localIP(), path);
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
      WSContentSend_P(UFS_FORM_SDC_DIRd, npath, path, PSTR(".."));
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

      uint32_t tm = entry.getLastWrite();
      String tstr = GetDT(tm);

      char *pp = path;
      if (!*(pp + 1)) { pp++; }
      char *cp = name;
      // osx formatted disks contain a lot of stuff we dont want
      if (!UfsReject((char*)ep)) {

        for (uint8_t cnt = 0; cnt<depth; cnt++) {
          *cp++ = '-';
        }

        sprintf(cp, format, ep);
        if (entry.isDirectory()) {
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRd, npath, ep, name);
          uint8_t plen = strlen(path);
          if (plen > 1) {
            strcat(path, "/");
          }
          strcat(path, ep);
          UfsListDir(path, depth + 4);
          path[plen] = 0;
        } else {
#ifdef GUI_TRASH_FILE
          char delpath[128];
          ext_snprintf_P(delpath, sizeof(delpath), UFS_FORM_SDC_HREFdel, (uint32_t)WiFi.localIP(), pp, ep);
#else
          char delpath[2];
          delpath[0]=0;
#endif // GUI_TRASH_FILE
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRb, npath, ep, name, tstr.c_str(), entry.size(), delpath);
        }
      }
      entry.close();
    }
    dir.close();
  }
}

#ifdef ESP32
#define ESP32_DOWNLOAD_TASK
#endif // ESP32

uint8_t UfsDownloadFile(char *file) {
  File download_file;

  if (!dfsp->exists(file)) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: File not found"));
    return 0;
  }

  download_file = dfsp->open(file, UFS_FILE_READ);
  if (!download_file) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Could not open file"));
    return 0;
  }

  if (download_file.isDirectory()) {
    download_file.close();
    return 1;
  }

#ifndef ESP32_DOWNLOAD_TASK
  WiFiClient download_Client;
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
  WSSend(200, CT_APP_STREAM, "");

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
    OsWatchLoop();
  }
  download_file.close();
  download_Client.stop();
#endif // ESP32_DOWNLOAD_TASK


#ifdef ESP32_DOWNLOAD_TASK
  download_file.close();

  if (UfsData.download_busy == true) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download is busy"));
    return 0;
  }

  UfsData.download_busy = true;
  char *path = (char*)malloc(128);
  strcpy(path,file);
  xTaskCreatePinnedToCore(donload_task, "DT", 6000, (void*)path, 3, NULL, 1);
#endif // ESP32_DOWNLOAD_TASK

  return 0;
}


#ifdef ESP32_DOWNLOAD_TASK
#ifndef DOWNLOAD_SIZE
#define DOWNLOAD_SIZE 4096
#endif // DOWNLOAD_SIZE
void donload_task(void *path) {
  File download_file;
  WiFiClient download_Client;
  char *file = (char*) path;

  download_file = dfsp->open(file, UFS_FILE_READ);
  free(file);

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
  WSSend(200, CT_APP_STREAM, "");

  uint8_t *buff = (uint8_t*)malloc(DOWNLOAD_SIZE);
  if (buff) {
    uint32_t bread;
    while (download_file.available()) {
      bread = download_file.read(buff, DOWNLOAD_SIZE);
      uint32_t bw = download_Client.write((const char*)buff, bread);
      if (!bw) { break; }
    }
    free(buff);
  }
  download_file.close();
  download_Client.stop();
  UfsData.download_busy = false;
  vTaskDelete( NULL );
}
#endif //  ESP32_DOWNLOAD_TASK


bool UfsUploadFileOpen(const char* upload_filename) {
  char npath[48];
  snprintf_P(npath, sizeof(npath), PSTR("%s/%s"), ufs_path, upload_filename);
  dfsp->remove(npath);
  ufs_upload_file = dfsp->open(npath, UFS_FILE_WRITE);
  return (ufs_upload_file);
}

bool UfsUploadFileWrite(uint8_t *upload_buf, size_t current_size) {
  if (ufs_upload_file) {
    ufs_upload_file.write(upload_buf, current_size);
  } else {
    return false;
  }
  return true;
}

void UfsUploadFileClose(void) {
  ufs_upload_file.close();
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv50(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      UfsExecuteCommandFileLoop();
      break;
#ifdef USE_SDCARD
    case FUNC_PRE_INIT:
      UfsCheckSDCardInit();
      break;
#endif // USE_SDCARD
    case FUNC_MQTT_INIT:
      if (!TasmotaGlobal.no_autoexec) {
        UfsExecuteCommandFile(TASM_FILE_AUTOEXEC);
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kUFSCommands, kUFSCommand);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_MANAGEMENT_BUTTON:
      if (ufs_type) {
        WSContentSend_PD(UFS_WEB_DIR, PSTR(D_MANAGE_FILE_SYSTEM));
      }
      break;
    case FUNC_WEB_ADD_HANDLER:
//      Webserver->on(F("/ufsd"), UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      Webserver->on("/ufsd", UfsDirectory);
      Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
      Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      break;
#endif // USE_WEBSERVER
  }
  return result;
}
#endif  // USE_UFILESYS
