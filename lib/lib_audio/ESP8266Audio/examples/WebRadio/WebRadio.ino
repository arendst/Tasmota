/*
  WebRadio Example
  Very simple HTML app to control web streaming
  
  Copyright (C) 2017  Earle F. Philhower, III

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

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include <EEPROM.h>

// Custom web server that doesn't need much RAM
#include "web.h"

// To run, set your ESP8266 build to 160MHz, update the SSID info, and upload.

// Enter your WiFi setup here:
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

AudioGenerator *decoder = NULL;
AudioFileSourceICYStream *file = NULL;
AudioFileSourceBuffer *buff = NULL;
AudioOutputI2S *out = NULL;

int volume = 100;
char title[64];
char url[96];
char status[64];
bool newUrl = false;
bool isAAC = false;
int retryms = 0;

typedef struct {
  char url[96];
  bool isAAC;
  int16_t volume;
  int16_t checksum;
} Settings;

// C++11 multiline string constants are neato...
static const char HEAD[] PROGMEM = R"KEWL(
<head>
<title>ESP8266 Web Radio</title>
<script type="text/javascript">
  function updateTitle() {
    var x = new XMLHttpRequest();
    x.open("GET", "title");
    x.onload = function() { document.getElementById("titlespan").innerHTML=x.responseText; setTimeout(updateTitle, 5000); }
    x.onerror = function() { setTimeout(updateTitle, 5000); }
    x.send();
  }
  setTimeout(updateTitle, 1000);
  function showValue(n) {
    document.getElementById("volspan").innerHTML=n;
    var x = new XMLHttpRequest();
    x.open("GET", "setvol?vol="+n);
    x.send();
  }
  function updateStatus() {var x = new XMLHttpRequest();
    x.open("GET", "status");
    x.onload = function() { document.getElementById("statusspan").innerHTML=x.responseText; setTimeout(updateStatus, 5000); }
    x.onerror = function() { setTimeout(updateStatus, 5000); }
    x.send();
  }
  setTimeout(updateStatus, 2000);
</script>
</head>)KEWL";

static const char BODY[] PROGMEM = R"KEWL(
<body>
ESP8266 Web Radio!
<hr>
Currently Playing: <span id="titlespan">%s</span><br>
Volume: <input type="range" name="vol" min="1" max="150" steps="10" value="%d" onchange="showValue(this.value)"/> <span id="volspan">%d</span>%%
<hr>
Status: <span id="statusspan">%s</span>
<hr>
<form action="changeurl" method="GET">
Current URL: %s<br>
Change URL: <input type="text" name="url">
<select name="type"><option value="mp3">MP3</option><option value="aac">AAC</option></select>
<input type="submit" value="Change"></form>
<form action="stop" method="POST"><input type="submit" value="Stop"></form>
</body>)KEWL";

void HandleIndex(WiFiClient *client)
{
  char buff[sizeof(BODY) + sizeof(title) + sizeof(status) + sizeof(url) + 3*2];
  
  Serial.printf_P(PSTR("Sending INDEX...Free mem=%d\n"), ESP.getFreeHeap());
  WebHeaders(client, NULL);
  WebPrintf(client, DOCTYPE);
  client->write_P( PSTR("<html>"), 6 );
  client->write_P( HEAD, strlen_P(HEAD) );
  sprintf_P(buff, BODY, title, volume, volume, status, url);
  client->write(buff, strlen(buff) );
  client->write_P( PSTR("</html>"), 7 );
  Serial.printf_P(PSTR("Sent INDEX...Free mem=%d\n"), ESP.getFreeHeap());
}

void HandleStatus(WiFiClient *client)
{
  WebHeaders(client, NULL);
  client->write(status, strlen(status));
}

void HandleTitle(WiFiClient *client)
{
  WebHeaders(client, NULL);
  client->write(title, strlen(title));
}

void HandleVolume(WiFiClient *client, char *params)
{
  char *namePtr;
  char *valPtr;
  
  while (ParseParam(&params, &namePtr, &valPtr)) {
    ParamInt("vol", volume);
  }
  Serial.printf_P(PSTR("Set volume: %d\n"), volume);
  out->SetGain(((float)volume)/100.0);
  RedirectToIndex(client);
}

void HandleChangeURL(WiFiClient *client, char *params)
{
  char *namePtr;
  char *valPtr;
  char newURL[sizeof(url)];
  char newType[4];

  newURL[0] = 0;
  newType[0] = 0;
  while (ParseParam(&params, &namePtr, &valPtr)) {
    ParamText("url", newURL);
    ParamText("type", newType);
  }
  if (newURL[0] && newType[0]) {
    newUrl = true;
    strncpy(url, newURL, sizeof(url)-1);
    url[sizeof(url)-1] = 0;
    if (!strcmp_P(newType, PSTR("aac"))) {
      isAAC = true;
    } else {
      isAAC = false;
    }
    strcpy_P(status, PSTR("Changing URL..."));
    Serial.printf_P(PSTR("Changed URL to: %s(%s)\n"), url, newType);
    RedirectToIndex(client);
  } else {
    WebError(client, 404, NULL, false);
  }
}

void RedirectToIndex(WiFiClient *client)
{
  WebError(client, 301, PSTR("Location: /\r\n"), true);
}

void StopPlaying()
{
  if (decoder) {
    decoder->stop();
    delete decoder;
    decoder = NULL;
  }
  if (buff) {
    buff->close();
    delete buff;
    buff = NULL;
  }
  if (file) {
    file->close();
    delete file;
    file = NULL;
  }
  strcpy_P(status, PSTR("Stopped"));
  strcpy_P(title, PSTR("Stopped"));
}

void HandleStop(WiFiClient *client)
{
  Serial.printf_P(PSTR("HandleStop()\n"));
  StopPlaying();
  RedirectToIndex(client);
}

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *str)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) isUnicode; // Punt this ball for now
  (void) ptr;
  if (strstr_P(type, PSTR("Title"))) { 
    strncpy(title, str, sizeof(title));
    title[sizeof(title)-1] = 0;
  } else {
    // Who knows what to do?  Not me!
  }
}
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) code;
  (void) ptr;
  strncpy_P(status, string, sizeof(status)-1);
  status[sizeof(status)-1] = 0;
}

#ifdef ESP8266
const int preallocateBufferSize = 5*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
#else
const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed
#endif
void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;

void setup()
{
  // First, preallocate all the memory needed for the buffering and codecs, never to be freed
  preallocateBuffer = malloc(preallocateBufferSize);
  preallocateCodec = malloc(preallocateCodecSize);
  if (!preallocateBuffer || !preallocateCodec) {
    Serial.begin(115200);
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize+preallocateCodecSize);
    while (1) delay(1000); // Infinite halt
  }

  Serial.begin(115200);

  delay(1000);
  Serial.printf_P(PSTR("Connecting to WiFi\n"));

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);

  // Try forever
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf_P(PSTR("...Connecting to WiFi\n"));
    delay(1000);
  }
  Serial.printf_P(PSTR("Connected\n"));
  
  Serial.printf_P(PSTR("Go to http://"));
  Serial.print(WiFi.localIP());
  Serial.printf_P(PSTR("/ to control the web radio.\n"));

  server.begin();
  
  strcpy_P(url, PSTR("none"));
  strcpy_P(status, PSTR("OK"));
  strcpy_P(title, PSTR("Idle"));

  audioLogger = &Serial;
  file = NULL;
  buff = NULL;
  out = new AudioOutputI2S();
  decoder = NULL;

  LoadSettings();
}

void StartNewURL()
{
  Serial.printf_P(PSTR("Changing URL to: %s, vol=%d\n"), url, volume);

  newUrl = false;
  // Stop and free existing ones
  Serial.printf_P(PSTR("Before stop...Free mem=%d\n"), ESP.getFreeHeap());
  StopPlaying();
  Serial.printf_P(PSTR("After stop...Free mem=%d\n"), ESP.getFreeHeap());
  SaveSettings();
  Serial.printf_P(PSTR("Saved settings\n"));
  
  file = new AudioFileSourceICYStream(url);
  Serial.printf_P(PSTR("created icystream\n"));
  file->RegisterMetadataCB(MDCallback, NULL);
  buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
  Serial.printf_P(PSTR("created buffer\n"));
  buff->RegisterStatusCB(StatusCallback, NULL);
  decoder = isAAC ? (AudioGenerator*) new AudioGeneratorAAC(preallocateCodec, preallocateCodecSize) : (AudioGenerator*) new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  Serial.printf_P(PSTR("created decoder\n"));
  decoder->RegisterStatusCB(StatusCallback, NULL);
  Serial.printf_P("Decoder start...\n");
  decoder->begin(buff, out);
  out->SetGain(((float)volume)/100.0);
  if (!decoder->isRunning()) {
    Serial.printf_P(PSTR("Can't connect to URL"));
    StopPlaying();
    strcpy_P(status, PSTR("Unable to connect to URL"));
    retryms = millis() + 2000;
  }
  Serial.printf_P("Done start new URL\n");
}

void LoadSettings()
{
  // Restore from EEPROM, check the checksum matches
  Settings s;
  uint8_t *ptr = reinterpret_cast<uint8_t *>(&s);
  EEPROM.begin(sizeof(s));
  for (size_t i=0; i<sizeof(s); i++) {
    ptr[i] = EEPROM.read(i);
  }
  EEPROM.end();
  int16_t sum = 0x1234;
  for (size_t i=0; i<sizeof(url); i++) sum += s.url[i];
  sum += s.isAAC;
  sum += s.volume;
  if (s.checksum == sum) {
    strcpy(url, s.url);
    isAAC = s.isAAC;
    volume = s.volume;
    Serial.printf_P(PSTR("Resuming stream from EEPROM: %s, type=%s, vol=%d\n"), url, isAAC?"AAC":"MP3", volume);
    newUrl = true;
  }
}

void SaveSettings()
{
  // Store in "EEPROM" to restart automatically
  Settings s;
  memset(&s, 0, sizeof(s));
  strcpy(s.url, url);
  s.isAAC = isAAC;
  s.volume = volume;
  s.checksum = 0x1234;
  for (size_t i=0; i<sizeof(url); i++) s.checksum += s.url[i];
  s.checksum += s.isAAC;
  s.checksum += s.volume;
  uint8_t *ptr = reinterpret_cast<uint8_t *>(&s);
  EEPROM.begin(sizeof(s));
  for (size_t i=0; i<sizeof(s); i++) {
    EEPROM.write(i, ptr[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

void PumpDecoder()
{
  if (decoder && decoder->isRunning()) {
    strcpy_P(status, PSTR("Playing")); // By default we're OK unless the decoder says otherwise
    if (!decoder->loop()) {
      Serial.printf_P(PSTR("Stopping decoder\n"));
      StopPlaying();
      retryms = millis() + 2000;
    }
}

}

void loop()
{
  static int lastms = 0;
  if (millis()-lastms > 1000) {
    lastms = millis();
    Serial.printf_P(PSTR("Running for %d seconds%c...Free mem=%d\n"), lastms/1000, !decoder?' ':(decoder->isRunning()?'*':' '), ESP.getFreeHeap());
  }

  if (retryms && millis()-retryms>0) {
    retryms = 0;
    newUrl = true;
  }
  
  if (newUrl) {
    StartNewURL();
  }

  PumpDecoder();
  
  char *reqUrl;
  char *params;
  WiFiClient client = server.available();
  PumpDecoder();
  char reqBuff[384];
  if (client && WebReadRequest(&client, reqBuff, 384, &reqUrl, &params)) {
    PumpDecoder();
    if (IsIndexHTML(reqUrl)) {
      HandleIndex(&client);
    } else if (!strcmp_P(reqUrl, PSTR("stop"))) {
      HandleStop(&client);
    } else if (!strcmp_P(reqUrl, PSTR("status"))) {
      HandleStatus(&client);
    } else if (!strcmp_P(reqUrl, PSTR("title"))) {
      HandleTitle(&client);
    } else if (!strcmp_P(reqUrl, PSTR("setvol"))) {
      HandleVolume(&client, params);
    } else if (!strcmp_P(reqUrl, PSTR("changeurl"))) {
      HandleChangeURL(&client, params);
    } else {
      WebError(&client, 404, NULL, false);
    }
    // web clients hate when door is violently shut
    while (client.available()) {
      PumpDecoder();
      client.read();
    }
  }
  PumpDecoder();
  if (client) {
    client.flush();
    client.stop();
  }
}

