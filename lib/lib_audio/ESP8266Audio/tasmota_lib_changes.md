Change in file `lib/lib_audio/ESP8266Audio/src/AudioOutputULP.cpp`
```
from
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
to
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
```

remove in file `lib/lib_audio/ESP8266Audio/src/AudioFileSourceFS.cpp`
```
#ifdef ESP32
#include "SPIFFS.h"
#endif
```

add in file `lib/lib_audio/ESP8266Audio/src/spiram-fast.h`
```
#ifndef SPECIAL
#define SPECIAL 0xF0
#endif
```

Files to delete:
```
lib/lib_audio/ESP8266Audio/src/AudioFileSourceSPIFFS.h
lib/lib_audio/ESP8266Audio/src/AudioOutputSPIFFSWAV.cpp
lib/lib_audio/ESP8266Audio/src/AudioOutputSPIFFSWAV.h
```
