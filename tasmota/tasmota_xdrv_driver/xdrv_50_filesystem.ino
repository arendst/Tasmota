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

// saves 80 bytes of flash, makes the UI cleaner for folders containing lots of files.
// disables recursive folder listing in file UI
//#define UFILESYS_NO_RECURSE_GUI

// Enables serving of static files on /fs/
// costs 1844 bytes of flash and 40 bytes of RAM
// probably not useful on esp8266, but useful on esp32
// You could serve a whole webapp from Tas itself.
//#define UFILESYS_STATIC_SERVING

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
    ffsp = nullptr;
    return;
  }
#endif  // ESP8266

#ifdef ESP32
  // try lfs first
  ffsp = &LittleFS;
 if (!LittleFS.begin(true, "") && !LittleFS.begin(true, "", 5, "fs_1")) {         // force empty mount point to make it the fallback FS
    // ffat is second
    ffsp = &FFat;
   if (!FFat.begin(true, "")) {
      ffsp = nullptr;
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

// simple put a zero at last '/'
// modifies input string
char *folderOnly(char *fname){
  for (int i = strlen(fname)-1; i >= 0; i--){
    if (fname[i] == '/'){
      fname[i] = 0;
      break;
    }
    fname[i] = 0;
  }
  if (!fname[0]){
    fname[0] = '/';
    fname[1] = 0;
  }
  return fname;
}

// returns everything after last '/' of whiole input if no '/'
char *fileOnly(char *fname){
  char *cp = fname;
  for (uint32_t cnt = strlen(fname); cnt >= 0; cnt--) {
    if (fname[cnt] == '/') {
      cp = &fname[cnt + 1];
      break;
    }
  }
  return cp;
}


#ifdef USE_SDCARD
void UfsCheckSDCardInit(void) {
  // Try SPI mode first
  // SPI mode requires SDCARD_CS to be configured
  if (TasmotaGlobal.spi_enabled && PinUsed(GPIO_SDCARD_CS)) {
    int8_t cs = Pin(GPIO_SDCARD_CS);

#ifdef ESP8266
    SPI.begin();
#endif // ESP8266
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
      AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted (SPI mode) with %d kB free"), UfsInfo(1, 0));
#endif // ESP32
    }
  }
#if defined(ESP32) && defined(SOC_SDMMC_HOST_SUPPORTED)     // ESP32 and SDMMC supported (not Esp32C3)
  // check if SDIO is configured
  else if (PinUsed(GPIO_SDIO_CLK) && PinUsed(GPIO_SDIO_CMD) && PinUsed(GPIO_SDIO_D0)) {
    int32_t sdio_cmd = Pin(GPIO_SDIO_CMD);
    int32_t sdio_clk = Pin(GPIO_SDIO_CLK);
    int32_t sdio_d0 = Pin(GPIO_SDIO_D0);
    int32_t sdio_d1 = Pin(GPIO_SDIO_D1);
    int32_t sdio_d2 = Pin(GPIO_SDIO_D2);
    int32_t sdio_d3 = Pin(GPIO_SDIO_D3);
    bool bit_4_mode = (sdio_d1 >= 0) && (sdio_d2 >= 0) && (sdio_d3 >= 0);   // enable 4-bit mode if possible
    if (bit_4_mode) {
      // AddLog(LOG_LEVEL_DEBUG, "UFS: trying SDIO 4-bit clk=%i cmd=%i d0=%i d1=%i d2=%i d3=%i", sdio_clk, sdio_cmd, sdio_d0, sdio_d1, sdio_d2, sdio_d3);
      SD_MMC.setPins(sdio_clk, sdio_cmd, sdio_d0, sdio_d1, sdio_d2, sdio_d3);
    } else {
      // AddLog(LOG_LEVEL_DEBUG, "UFS: trying SDIO 1-bit clk=%i cmd=%i d0=%i", sdio_clk, sdio_cmd, sdio_d0);
      SD_MMC.setPins(sdio_clk, sdio_cmd, sdio_d0);
    }
    if (SD_MMC.begin("/sd", !bit_4_mode /*mode 1 bit*/, false /*format_if_failed*/)) {    // mount under "/sd" to be consistent with SD SPI
      ufsp = &SD_MMC;

      ufs_type = UFS_TSDC;
      dfsp = ufsp;
      if (ffsp) {ufs_dir = 1;}
      // make sd card the global filesystem
      AddLog(LOG_LEVEL_INFO, PSTR("UFS: SDCard mounted (SDIO %i-bit) with %d kB free"), bit_4_mode ? 4 : 1, UfsInfo(1, 0));
    }
  }
#endif
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
        result = LittleFS.totalBytes();
      } else {
        result = LittleFS.totalBytes() - LittleFS.usedBytes();
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

uint32_t UfsSize(void) {
  return UfsInfo(0, ufs_dir == 2 ? 1:0);
}

uint32_t UfsFree(void) {
  return UfsInfo(1, ufs_dir == 2 ? 1:0);
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
    AddLog(LOG_LEVEL_DEBUG, PSTR("TFS: File '%s' not found"), fname +1);  // Skip leading slash
  }
  return yes;
}

size_t TfsFileSize(const char *fname){
  if (!ffs_type) { return 0; }

  File file = ffsp->open(fname, "r");
  if (!file) { return 0; }
  size_t flen = file.size();
  file.close();
  return flen;
}

bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) {
  if (!ffs_type) { return false; }
#ifdef USE_WEBCAM
  WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
#endif
  bool result = false;
  File file = ffsp->open(fname, "w");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Save failed"));
  } else {
    // This will timeout on ESP32-webcam
    // But now solved with WcInterrupt(0) in support_esp.ino
    file.write(buf, len);
  /*
    // This will still timeout on ESP32-webcam when wcresolution 10
    uint32_t count = len / 512;
    uint32_t chunk = len / count;
    for (uint32_t i = 0; i < count; i++) {
      file.write(buf + (i * chunk), chunk);
      // do actually wait a little to allow ESP32 tasks to tick
      // fixes task timeout in ESP32Solo1 style unicore code and webcam.
      delay(10);
      OsWatchLoop();
    }
    uint32_t left = len % count;
    if (left) {
      file.write(buf + (count * chunk), left);
    }
  */
    file.close();
    result = true;
  }
#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif
  return result;
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

  File file = ffsp->open(fname, "r");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: File '%s' not found"), fname +1);  // Skip leading slash
    return false;
  }

  size_t flen = file.size();
  if (len > flen) { len = flen; }           // Adjust requested length to smaller file length
  file.read(buf, len);
  file.close();
  return true;
}

String TfsLoadString(const char *fname) {
  // Use a reasonable amount of stack space considering 4k/8k available on ESP8266/ESP32 and manageable string length
  char buf[2048] = { 0 };                   // Prepare empty string of max 2047 characters on stack
  TfsLoadFile(fname, (uint8_t*)buf, 2047);  // Leave last position as end of string ('\0')
  return String(buf);                       // Received string or empty on error
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

  if (BACKLOG_EMPTY && strlen(UfsData.run_file) && !UfsData.run_file_mutex) {
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
//        else if (index && !comment && (buf[0] == ';')) {
//          break;                       // End of command on multi command line
//        }
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
      ExecuteCommand(cmd_line, SRC_FILE);
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
  "|Type|Size|Free|Delete|Rename|Run"
#ifdef UFILESYS_STATIC_SERVING
  "|Serve"
#endif
  ;

void (* const kUFSCommand[])(void) PROGMEM = {
  &UFSInfo, &UFSType, &UFSSize, &UFSFree, &UFSDelete, &UFSRename, &UFSRun
#ifdef UFILESYS_STATIC_SERVING
  ,&UFSServe
#endif  
  };

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

#ifdef UFILESYS_STATIC_SERVING
/*
* Serves a filesystem folder at a web url.
* NOTE - this is expensive on flash -> +2.5kbytes.
* like "UFSServe <fs path>,<url>[,<noauth>]"
* e.g. "UFSServe /sd/,/mysdcard/,1" - will serve the /sd/ fs folder as https://<ip>/mysdcard/ with no auth required
* e.g. "UFSServe /www/,/" - will serve the /www/ fs folder as https://<ip>/ with auth required if TAS has a password setup
* <noauth> defaults to 0 - i.e. the default is to require auth if configured
* it WILL serve on / - so conflicting urls could occur.  I beleive native TAS urls will have priority.
* you can serve multiple folders, and they can each be auth or noauth
*
* by default, it also enables cors on the webserver - this allows you to have 
* a website external to TAS which can access the files, else the browser refuses.
*/
#include "detail/RequestHandlersImpl.h"

// class to allow us to request auth when required.
// StaticRequestHandler is in the above header
class StaticRequestHandlerAuth : public StaticRequestHandler {
public:
    StaticRequestHandlerAuth(FS& fs, const char* path, const char* uri, const char* cache_header):
      StaticRequestHandler(fs, path, uri, cache_header)
    {
    }

    // we replace the handle method, 
    // and look for authentication only if we would serve the file.
    // if we check earlier, then we will reject as unauth even though a later
    // handler may be public, and so fail to serve public files.
    bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) override {
        if (!canHandle(requestMethod, requestUri))
            return false;

        log_v("StaticRequestHandler::handle: request=%s _uri=%s\r\n", requestUri.c_str(), _uri.c_str());

        String path(_path);

        if (!_isFile) {
            // Base URI doesn't point to a file.
            // If a directory is requested, look for index file.
            if (requestUri.endsWith("/")) 
              requestUri += "index.htm";

            // Append whatever follows this URI in request to get the file path.
            path += requestUri.substring(_baseUriLength);
        }
        log_v("StaticRequestHandler::handle: path=%s, isFile=%d\r\n", path.c_str(), _isFile);

        String contentType = getContentType(path);

        // look for gz file, only if the original specified path is not a gz.  So part only works to send gzip via content encoding when a non compressed is asked for
        // if you point the the path to gzip you will serve the gzip as content type "application/x-gzip", not text or javascript etc...
        if (!path.endsWith(FPSTR(mimeTable[gz].endsWith)) && !_fs.exists(path))  {
            String pathWithGz = path + FPSTR(mimeTable[gz].endsWith);
            if(_fs.exists(pathWithGz))
                path += FPSTR(mimeTable[gz].endsWith);
        }

        File f = _fs.open(path, "r");
        if (!f || !f.available())
            return false;

        if (!WebAuthenticate()) {
          AddLog(LOG_LEVEL_ERROR, PSTR("UFS: serv of %s denied"), requestUri.c_str());
          server.requestAuthentication();
          return true;
        }

        if (_cache_header.length() != 0)
            server.sendHeader("Cache-Control", _cache_header);

        server.streamFile(f, contentType);
        return true;
    }
};

void UFSServe(void) {
  if (XdrvMailbox.data_len > 0) {
    bool result = false;
    char *fpath = strtok(XdrvMailbox.data, ",");
    char *url = strtok(nullptr, ",");
    char *noauth = strtok(nullptr, ",");
    if (fpath && url) {
      char t[] = "";
      StaticRequestHandlerAuth *staticHandler;
      if (noauth && *noauth == '1'){
        staticHandler = (StaticRequestHandlerAuth *) new StaticRequestHandler(*ffsp, fpath, url, (char *)nullptr);
      } else {
        staticHandler = new StaticRequestHandlerAuth(*ffsp, fpath, url, (char *)nullptr);
      }
      if (staticHandler) {
        //Webserver->serveStatic(url, *ffsp, fpath);
        Webserver->addHandler(staticHandler);
        Webserver->enableCORS(true);
        result = true;
      } else {
        // could this happen?  only lack of memory.
        result = false;
      }
    }
    if (!result) {
      ResponseCmndFailed();
    } else {
      ResponseCmndDone();
    }
  }
}
#endif // UFILESYS_STATIC_SERVING

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
  "<p><form action='ufsd' method='get'><input type='hidden' name='download' value='%s' /> <button>%s</button></form></p>";

const char UFS_CURRDIR[] PROGMEM = 
  "<p>%s: %s</p>";

#ifndef D_CURR_DIR
  #define D_CURR_DIR "Folder"
#endif

const char UFS_FORM_FILE_UPLOAD[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_MANAGE_FILE_SYSTEM "&nbsp;</b></legend>";
const char UFS_FORM_FILE_UPGc[] PROGMEM =
  "<div style='text-align:left;color:#%06x;'>" D_FS_SIZE " %s MB - " D_FS_FREE " %s MB";

const char UFS_FORM_FILE_UPGc1[] PROGMEM =
    " &nbsp;&nbsp;<a href='/ufsd?dir=%d'>%s</a>";

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
  "<form method='get' action='ufse'><input type='hidden' name='file' value='%s/" D_NEW_FILE "'>"
  "<button type='submit'>" D_CREATE_NEW_FILE "</button></form>";
const char UFS_FORM_FILE_UPGb1[] PROGMEM =
  "<input type='checkbox' id='shf' onclick='sf(eb(\"shf\").checked);' name='shf'>" D_SHOW_HIDDEN_FILES "</input>";

const char UFS_FORM_FILE_UPGb2[] PROGMEM =
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char UFS_FORM_SDC_DIR_NORMAL[] PROGMEM =
  "";
const char UFS_FORM_SDC_DIR_HIDDABLE[] PROGMEM =
  " class='hf'";
const char UFS_FORM_SDC_DIRd[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a></pre>";
const char UFS_FORM_SDC_DIRb[] PROGMEM =
  "<pre%s><a href='%s' file='%s'>%s</a> %19s %8d %s %s</pre>";
const char UFS_FORM_SDC_HREF[] PROGMEM =
  "ufsd?download=%s/%s";

#ifdef GUI_TRASH_FILE
const char UFS_FORM_SDC_HREFdel[] PROGMEM =
  //"<a href=ufsd?delete=%s/%s>&#128465;</a>"; // 🗑️
  "<a href='ufsd?delete=%s/%s&download=%s' onclick=\"return confirm('" D_CONFIRM_FILE_DEL "')\">&#128293;</a>"; // 🔥
#endif // GUI_TRASH_FILE

#ifdef GUI_EDIT_FILE

#define FILE_BUFFER_SIZE  1024

const char UFS_FORM_SDC_HREFedit[] PROGMEM =
  "<a href='ufse?file=%s/%s'>&#x1F4DD;</a>"; // 📝

const char HTTP_EDITOR_FORM_START[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_EDIT_FILE "&nbsp;</b></legend>"
  "<form>"
  "<label for='name'>" D_FILE ":</label><input type='text' id='name' name='name' value='%s'><br><hr width='98%%'>"
  "<textarea id='content' name='content' wrap='off' rows='8' cols='80' style='font-size: 12pt'>";

const char HTTP_EDITOR_FORM_END[] PROGMEM =
  "</textarea>"
  "<button name='save' type='submit' formmethod='post' formenctype='multipart/form-data' formaction='/ufse' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";

#endif  // #ifdef GUI_EDIT_FILE

void UfsDirectory(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MANAGE_FILE_SYSTEM));

  uint8_t depth = 0;
  uint8_t isdir = 0;

  strcpy(ufs_path, "/");

  if (Webserver->hasArg(F("dir"))) {
    String stmp = Webserver->arg(F("dir"));
    ufs_dir = atoi(stmp.c_str());
  }

  if (ufs_dir == 1) {
    dfsp = ufsp;
  } else {
    if (ffsp) {
      dfsp = ffsp;
    }
  }

  if (Webserver->hasArg(F("delete"))) {
    String stmp = Webserver->arg(F("delete"));
    char *cp = (char*)stmp.c_str();
    File download_file = dfsp->open(cp, UFS_FILE_READ);
    if (download_file) {
      if (download_file.isDirectory()) {
        download_file.close();
        dfsp->rmdir(cp);
      } else {
        download_file.close();
        dfsp->remove(cp);
      }
    }
  }

  if (Webserver->hasArg(F("download"))) {
    String stmp = Webserver->arg(F("download"));
    char *cp = (char*)stmp.c_str();
    if (UfsDownloadFile(cp)) {
      // is directory
      strcpy(ufs_path, cp);
      isdir = 1;
    } else {
      return;
    }
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
    WSContentSend_P(UFS_FORM_FILE_UPGc1, (ufs_dir == 1)?2:1, (ufs_dir == 1)?PSTR("SDCard"):PSTR("FlashFS"));
  }
  WSContentSend_P(UFS_FORM_FILE_UPGc2);

  WSContentSend_P(UFS_FORM_FILE_UPG, PSTR(D_SCRIPT_UPLOAD));

  if (isdir){
    // if a folder, show 'folder: xxx' if not '/'
    if (ufs_path[0] != '/' || strlen(ufs_path) != 1){
      WSContentSend_P(UFS_CURRDIR, PSTR(D_CURR_DIR), ufs_path);
    }
  }

  WSContentSend_P(UFS_FORM_SDC_DIRa);
  if (ufs_type) {
    UfsListDir(ufs_path, depth);
  }
  WSContentSend_P(UFS_FORM_SDC_DIRc);
#ifdef GUI_EDIT_FILE
  WSContentSend_P(UFS_FORM_FILE_UPGb, ufs_path);
#endif
  if (!isSDC()) {
    WSContentSend_P(UFS_FORM_FILE_UPGb1);
  }
  WSContentSend_P(UFS_FORM_FILE_UPGb2);

  WSContentSpaceButton(BUTTON_MANAGEMENT);
  WSContentStop();

  Web.upload_file_type = UPL_UFSFILE;
}

// return true if SDC
bool isSDC(void) {
#ifndef SDC_HIDE_INVISIBLES
  return false;
#else
  if (((uint32_t)ufsp != (uint32_t)ffsp) && ((uint32_t)ffsp == (uint32_t)dfsp)) return false;
  if (((uint32_t)ufsp == (uint32_t)ffsp) && (ufs_type != UFS_TSDC)) return false;
  return true;
#endif
}

void UfsListDir(char *path, uint8_t depth) {
  char name[48];
  char npath[128];
  char format[12];
  sprintf(format, PSTR("%%-%ds"), 24 - depth);

  File dir = dfsp->open(path, UFS_FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      ext_snprintf_P(npath, sizeof(npath), PSTR("ufsd?download=%s"), path);
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
      WiFiClient client = Webserver->client();
      // abort if the client disconnected
      // if there is a huge folder, then this gives a way out by refresh of browser
      if (!client.connected()){
        break;
      }

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
      bool hiddable = UfsReject((char*)ep);

      if (!hiddable || !isSDC() ) {

        for (uint8_t cnt = 0; cnt<depth; cnt++) {
          *cp++ = '-';
        }

        String pp_escaped_string = UrlEscape(pp);
        String ep_escaped_string = UrlEscape(ep);
        const char* ppe = pp_escaped_string.c_str();    // this can't be merged on a single line otherwise the String object can be freed
        const char* epe = ep_escaped_string.c_str();
        sprintf(cp, format, ep);
#ifdef GUI_TRASH_FILE
        char delpath[128+UFS_FILENAME_SIZE];
        ext_snprintf_P(delpath, sizeof(delpath), UFS_FORM_SDC_HREFdel, ppe, epe, ppe[0]?ppe:"/");
#else
        char delpath[2] = " ";
#endif // GUI_TRASH_FILE
        if (entry.isDirectory()) {
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, ppe, epe);

          WSContentSend_P(UFS_FORM_SDC_DIRb, hiddable ? UFS_FORM_SDC_DIR_HIDDABLE : UFS_FORM_SDC_DIR_NORMAL, npath, epe,
                          HtmlEscape(name).c_str(), "", 0, delpath, " ");
          //WSContentSend_P(UFS_FORM_SDC_DIRd, npath, ep, name);
#ifdef UFILESYS_RECURSEFOLDERS_GUI
          uint8_t plen = strlen(path);
          if (plen > 1) {
            strcat(path, "/");
          }
          strcat(path, ep);
          UfsListDir(path, depth + 4);
          path[plen] = 0;
#endif          
        } else {
  #ifdef GUI_EDIT_FILE
          char editpath[128];
          ext_snprintf_P(editpath, sizeof(editpath), UFS_FORM_SDC_HREFedit, ppe, epe);
  #else
          char editpath[2];
          editpath[0]=0;
  #endif // GUI_TRASH_FILE
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, ppe, epe);
          WSContentSend_P(UFS_FORM_SDC_DIRb, hiddable ? UFS_FORM_SDC_DIR_HIDDABLE : UFS_FORM_SDC_DIR_NORMAL, npath, epe,
                          HtmlEscape(name).c_str(), tstr.c_str(), entry.size(), delpath, editpath);
        }
        entry.close();
      }
    }
    dir.close();
  }
}

#ifdef ESP32
// this actually does not work reliably, as the 
// webserver can close the connection before the download task completes
//#define ESP32_DOWNLOAD_TASK
#endif // ESP32

uint8_t UfsDownloadFile(char *file) {
  File download_file;

  AddLog(LOG_LEVEL_INFO, PSTR("UFS: File '%s' download"), file);

  if (!dfsp->exists(file)) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: File '%s' not found"), file);
    return 0;
  }

  download_file = dfsp->open(file, UFS_FILE_READ);
  if (!download_file) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Could not open file '%s'"), file);
    return 0;
  }

  if (download_file.isDirectory()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: File '%s' to download is directory"), file);
    download_file.close();
    return 1;
  }

#ifndef ESP32_DOWNLOAD_TASK
  WiFiClient download_Client;
  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp = fileOnly(file);
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



// to make this work I thing you wouold need to duplicate the client 
// BEFORE starting the task, so that the webserver does not close it's
// copy of the client.
#ifdef ESP32_DOWNLOAD_TASK
  download_file.close();

  if (UfsData.download_busy == true) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download is busy"));
    return 0;
  }

  UfsData.download_busy = true;
  char *path = (char*)malloc(128);
  strcpy(path,file);
  BaseType_t ret = xTaskCreatePinnedToCore(download_task, "DT", 6000, (void*)path, 3, nullptr, 1);
  if (ret != pdPASS)
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download task failed with %d"), ret);
  yield();
#endif // ESP32_DOWNLOAD_TASK

  return 0;
}


#ifdef ESP32_DOWNLOAD_TASK
#ifndef DOWNLOAD_SIZE
#define DOWNLOAD_SIZE 4096
#endif // DOWNLOAD_SIZE
void download_task(void *path) {
  File download_file;
  WiFiClient download_Client;
  char *file = (char*) path;

  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: ESP32 File '%s' to download"), file);

  download_file = dfsp->open(file, UFS_FILE_READ);
  uint32_t flen = download_file.size();

  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: len %d to download"), flen);

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp = fileOnly(file);
  //snprintf_P(attachment, sizeof(attachment), PSTR("download file '%s' as '%s'"), file, cp);
  //Webserver->sendHeader(F("X-Tasmota-Debug"), attachment);
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
  free(path);
  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: esp32 sent file"));
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

//******************************************************************************************
//  File Editor
//******************************************************************************************

#ifdef GUI_EDIT_FILE

void UfsEditor(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor GET"));

  char fname_input[UFS_FILENAME_SIZE];
  if (Webserver->hasArg(F("file"))) {
    WebGetArg(PSTR("file"), fname_input, sizeof(fname_input));
  } else {
    snprintf_P(fname_input, sizeof(fname_input), PSTR(D_NEW_FILE));
  }
  char fname[UFS_FILENAME_SIZE];
  UfsFilename(fname, fname_input);                  // Trim spaces and add slash

  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: file=%s, ffs_type=%d, TfsFileExist=%d"), fname, ffs_type, dfsp->exists(fname));

  WSContentStart_P(PSTR(D_EDIT_FILE));
  WSContentSendStyle();
  char *bfname = fname +1;
  WSContentSend_P(HTTP_EDITOR_FORM_START, bfname);  // Skip leading slash

  if (ffs_type && dfsp->exists(fname)) {
    File fp = dfsp->open(fname, "r");
    if (!fp) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: file open failed"));
      WSContentSend_P(D_NEW_FILE);
    } else {
      uint8_t *buf = (uint8_t*)malloc(FILE_BUFFER_SIZE+1);
      size_t filelen = fp.size();
      AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: file len=%d"), filelen);
      while (filelen > 0) {
        size_t l = fp.read(buf, FILE_BUFFER_SIZE);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("UFS: UfsEditor: read=%d"), l);
        if (l < 0) { break; }
        buf[l] = '\0';
        WSContentSend_P(PSTR("%s"), buf);
        filelen -= l;
      }
      fp.close();
      free(buf);
      AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: read done"));
    }
  } else {
    WSContentSend_P(D_NEW_FILE);
  }

  WSContentSend_P(HTTP_EDITOR_FORM_END);
  folderOnly(fname);
  WSContentSend_P(UFS_WEB_DIR, fname, PSTR(D_MANAGE_FILE_SYSTEM));
  WSContentStop();
}

void UfsEditorUpload(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: file upload"));

  if (!HttpCheckPriviledgedAccess()) { return; }

  if (!Webserver->hasArg("name")) {
    AddLog(LOG_LEVEL_ERROR, PSTR("UFS: UfsEditor: file upload - no filename"));
    WSSend(400, CT_PLAIN, F("400: Bad request - no filename"));
    return;
  }

  char fname_input[UFS_FILENAME_SIZE];
  WebGetArg(PSTR("name"), fname_input, sizeof(fname_input));
  char fname[UFS_FILENAME_SIZE];
  UfsFilename(fname, fname_input);                  // Trim spaces and add slash
  AddLog(LOG_LEVEL_DEBUG, PSTR("UFS: UfsEditor: file '%s'"), fname);

  if (!Webserver->hasArg("content")) {
    AddLog(LOG_LEVEL_ERROR, PSTR("UFS: UfsEditor: file upload - no content"));
    WSSend(400, CT_PLAIN, F("400: Bad request - no content"));
    return;
  }
  String content = Webserver->arg("content");

  if (!dfsp) {
    Web.upload_error = 1;
    AddLog(LOG_LEVEL_ERROR, PSTR("UFS: UfsEditor: 507: no storage available"));
    WSSend(507, CT_PLAIN, F("507: no storage available"));
    return;
  }

  // recursively create folder(s)
  char tmp[UFS_FILENAME_SIZE];
  char folder[UFS_FILENAME_SIZE] = "";
  strcpy(tmp, fname);
  // zap file name off the end
  folderOnly(tmp);
  char *tf = strtok(tmp, "/");
  while(tf){
    if (*tf){
      strcat(folder, "/");
      strcat(folder, tf);
    }
    // we don;t care if it fails - it may already exist.
    dfsp->mkdir(folder);
    tf = strtok(nullptr, "/");
  }

  File fp = dfsp->open(fname, "w");
  if (!fp) {
    Web.upload_error = 1;
    AddLog(LOG_LEVEL_ERROR, PSTR("UFS: UfsEditor: 400: invalid file name '%s'"), fname);
    WSSend(400, CT_PLAIN, F("400: bad request - invalid filename"));
    return;
  }

  if (*content.c_str()) {
    content.replace("\r\n", "\n");
    content.replace("\r", "\n");
  }

  if (!fp.print(content)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("UFS: UfsEditor: write error on '%s'"), fname);
  }

  fp.close();

  // zap file name off the end
  folderOnly(fname);
  char t[20+UFS_FILENAME_SIZE] = "/ufsu?download=";
  strcat(t, fname);
  Webserver->sendHeader(F("Location"), t);
  Webserver->send(303);
}

#endif  // GUI_EDIT_FILE

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv50(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      UfsExecuteCommandFileLoop();
      break;
/*
// Moved to support_tasmota.ino for earlier init to be used by scripter
#ifdef USE_SDCARD
    case FUNC_PRE_INIT:
      UfsCheckSDCardInit();
      break;
#endif // USE_SDCARD
*/
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
        if (XdrvMailbox.index) {
          XdrvMailbox.index++;
        } else {
          WSContentSend_PD(UFS_WEB_DIR, "/", PSTR(D_MANAGE_FILE_SYSTEM));
        }
      }
      break;
    case FUNC_WEB_ADD_HANDLER:
//      Webserver->on(F("/ufsd"), UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      Webserver->on("/ufsd", UfsDirectory);
      Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
      Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
#ifdef GUI_EDIT_FILE
      Webserver->on("/ufse", HTTP_GET, UfsEditor);
      Webserver->on("/ufse", HTTP_POST, UfsEditorUpload);
#endif
      break;
#endif // USE_WEBSERVER
  }
  return result;
}
#endif  // USE_UFILESYS
