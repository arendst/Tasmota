// Uncomment the file system to use for FTP server

#define FS_LITTLEFS
//#define FS_SPIFFS
//#define FS_SD_MMC

#ifdef ESP32
#include <WiFi.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#include <WiFiClient.h>
#include <time.h>
#include "ESPFtpServer.h"

#if defined(FS_LITTLEFS)
#ifdef ESP32
#include "LITTLEFS.h"
#define FS_ID LITTLEFS
#endif
#ifdef ESP8266
#include "LittleFS.h"
#define FS_ID LittleFS
#endif
#define FS_NAME "LittleFS"
#elif defined(FS_SPIFFS)
#ifdef ESP32
#include "SPIFFS.h"
#endif
#define FS_ID SPIFFS
#define FS_NAME "SPIFFS"
#elif defined(FS_SD_MMC)
#include "SD_MMC.h"
#define FS_ID SD_MMC
#define FS_NAME "SD_MMC"
#else 
#define FS_ID SD
#define FS_NAME "UNDEF"
#endif

const char* ssid = "*********************";
const char* password = "*********************";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
struct tm   timeinfo;

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial

#ifdef ESP8266
bool getLocalTime (struct tm * info) {
  time_t now;

  time(&now);
  localtime_r (&now, info);

  if (info->tm_year > (2016 - 1900)) {
    return true;
  }
  else {
    return false;
  }
}
#endif

void setup (void) {
  Serial.begin (115200);

  WiFi.begin (ssid, password);
  Serial.println ("");

  // Wait for connection
  while (WiFi.status () != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }
  Serial.println ("");
  Serial.print ("Connected to ");
  Serial.println (ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP ());

  configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!getLocalTime (&timeinfo)) {
    delay (500);
    Serial.print (".");
  }
  Serial.printf ("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (timeinfo.tm_year) + 1900, (timeinfo.tm_mon) + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  
  //FS_ID.format ();
  if (FS_ID.begin ()) {
    Serial.println ("File system opened (" + String (FS_NAME) + ")");
    ftpSrv.begin ("esp32", "esp32");    //username, password for ftp.  set ports in ESPFtpServer.h  (default 21, 50009 for PASV)
  }
  else {
    Serial.println ("File system could not be opened; ftp server will not work");
  }
}

void loop (void){
  ftpSrv.handleFTP (FS_ID);        //make sure in loop you call handleFTP()!
}
