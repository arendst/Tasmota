/* Copyright 2019 Motea Marius

  This example code will create a webserver that will provide basic control to AC units using the web application
  build with javascript/css. User config zone need to be updated if a different class than Collix need to be used.
  Javasctipt file may also require minor changes as in current version it will not allow to set fan speed if Auto mode
  is selected (required for Coolix).

*/
#ifndef EXAMPLES_WEB_AC_CONTROL_WEB_AC_CONTROL_H_
#define EXAMPLES_WEB_AC_CONTROL_WEB_AC_CONTROL_H_
#if defined(ESP8266)
#include <LittleFS.h>
#else
#include <SPIFFS.h>
#endif

// Uncomment one of the following to manually override what
//    type of persistent storage is used.
// Warning: Changing filesystems will cause all previous locally
//    saved configuration data to be lost.
// #define FILESYSTEM SPIFFS
// #define FILESYSTEM LittleFS
#ifndef FILESYSTEM
// Set the default filesystem if none was specified.
#ifdef ESP8266
#define FILESYSTEM LittleFS
#else
#define FILESYSTEM SPIFFS
#endif  // defined(ESP8266)
#endif  // FILESYSTEM

#if (FILESYSTEM == LittleFS)
#define FILESYSTEMSTR "LittleFS"
#else
#define FILESYSTEMSTR "SPIFFS"
#endif
#endif  // EXAMPLES_WEB_AC_CONTROL_WEB_AC_CONTROL_H_
