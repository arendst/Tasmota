# LittleFS_esp32

#### ***Notice: The Library is been integrated to [Arduino esp32 core idf-release/v4.2 branch](https://github.com/espressif/arduino-esp32/tree/idf-release/v4.2 ) for future major core release. On built-in library, #define tweaks below will be unavailable.***


## LittleFS library for arduino-esp32

- A LittleFS wrapper for Arduino ESP32 of [littlefs-project](https://github.com/littlefs-project/littlefs)
- Based on [ESP-IDF port of joltwallet/esp_littlefs](https://github.com/joltwallet/esp_littlefs) , thank you Brian!
- As a reference, see [LillteFS library for ESP8266 core](https://github.com/esp8266/Arduino/tree/master/libraries/LittleFS)
- [PR](https://github.com/espressif/arduino-esp32/pull/4096) and [merge](https://github.com/espressif/arduino-esp32/pull/4483) at esp32 core development
- [PR](https://github.com/espressif/esp-idf/pull/5469) at esp-idf development
- The functionality is similar to SPIFFS

### Installation

- Use **Arduino Library Manager** 
- Or download / use **git** to have latest repository of **LITTLEFS** added to Arduino IDE **/libraries** folder  
(File > Preferences > Sketchbook location).
- See ``` #define CONFIG_LITTLEFS_FOR_IDF_3_2 ``` in **esp_littlefs.c**.  
Now it is defined / undefined automatically by detecting the IDF version and core version.
When defined, the library works with old and new IDFs 3.2 - 4.x but file timestamp feature is removed.
See LITTLEFS_time example.
- See ``` #define CONFIG_LITTLEFS_SPIFFS_COMPAT ``` in **esp_littlefs.c**.  
When set to 1, folders are recursively created or deleted if empty on creating/deleting a new file like SPIFFS. Default is 0.
- The other ``` #define CONFIG_LITTLEFS_xxxxx ``` are set to optimal default values.  
Read [here](https://github.com/joltwallet/esp_littlefs/blob/master/Kconfig) and [here](https://github.com/littlefs-project//littlefs/blob/master/README.md) if you want to modify.
- For low-level default error reporting modifications, see the defines at beginning of the **lfs.c** here.

### Usage

- Use LITTLEFS [with identical methods as SPIFFS](https://diyprojects.io/esp32-get-started-spiff-library-read-write-modify-files/) plus mkdir() and rmdir()
- A quick startup based on your existing code you can re-define SPIFFS
``` 
#define USE_LittleFS

#include <FS.h>
#ifdef USE_LittleFS
  #define SPIFFS LITTLEFS
  #include <LITTLEFS.h> 
#else
  #include <SPIFFS.h>
#endif 
 ```
 - Note, this may not work if your sketch uses other libraries that use SPIFFS themselves.
 - See also [esp_partition.h](https://github.com/espressif/esp-idf/blob/master/components/spi_flash/include/esp_partition.h) . LITTLEFS re-uses same type and subtype as SPIFFS

### Differences with SPIFFS 

- LittleFS has folders, you need to iterate files in folders unless you set  ``` #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1 ```  
- At root a "/folder" = "folder"
- Requires a label for mount point, NULL will not work. Recommended is to use default LITTLEFS.begin()
- maxOpenFiles parameter is unused, kept for compatibility
- LITTLEFS.mkdir(path) and LITTLEFS.rmdir(path) are available
- file.seek() behaves like on FFat see [more details](https://github.com/lorol/LITTLEFS/issues/11)
- file.write() and file.print() when partition space is ending may return different than really written bytes (on other FS is also inconsistent).
- Speed comparison based on **LittleFS_test.ino** sketch (for a file 1048576 bytes):

|Filesystem|Read time [ms]|Write time [ms]|
|----|----|----|
|FAT|276|14493|
|LITTLEFS|446*|16387|
|SPIFFS|767|65622|

*The read speed improved by changing ```#define CONFIG_LITTLEFS_CACHE_SIZE``` from 128 to 512


### Arduino ESP32 LittleFS filesystem upload tool 

- Use (replace if exists) [arduino-esp32fs-plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin/pull/23 ) with [this variant](https://github.com/lorol/arduino-esp32fs-plugin), which supports SPIFFS, LittleFS and FatFS
- Requires [mklittlefs executable](https://github.com/earlephilhower/mklittlefs) which is available [in releases section here](https://github.com/lorol/arduino-esp32fs-plugin ) or download the zipped binary [here](https://github.com/earlephilhower/mklittlefs/releases) or from **esp-quick-toolchain** releases [here](https://github.com/earlephilhower/esp-quick-toolchain/releases) 
- Copy it to **/tools** folder of esp32 platform where **espota** and **esptool** (.py or.exe) tools are located
- Restart Arduino IDE. 

### PlatformIO

- See [LITTLEFS_PlatformIO example here](https://github.com/lorol/LITTLEFS/tree/master/examples/LITTLEFS_PlatformIO)  
 ( based on notes below from [BlueAndi](https://github.com/BlueAndi) )
- Add to _platformio.ini_:
 `extra_scripts = replace_fs.py`
 
- Add _replace_fs.py_ to project root directory (where platformio.ini is located):
 
 ```python
 Import("env")
 print("Replace MKSPIFFSTOOL with mklittlefs.exe")
 env.Replace (MKSPIFFSTOOL = "mklittlefs.exe")
 ```
 
- Add _mklittlefs.exe_ to project root directory as well.

## Credits and license

- This work is based on [littlefs-project](https://github.com/littlefs-project/littlefs) , [ESP-IDF port of joltwallet/esp_littlefs](https://github.com/joltwallet/esp_littlefs) , [Espressif Arduino core for the ESP32, the ESP-IDF - SPIFFS Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS)
- Licensed under GPL v2 ([text](LICENSE))

## To Do
  - [x] Submit to be added to Arduino Library Manager
  - [x] Optional drop-in compatibility with SPIFFS' lack of folders, similar to esp8266' way - implemented as a choice by a #define
    - [recursive folders auto creation](https://github.com/esp8266/Arduino/blob/master/libraries/LittleFS/src/LittleFS.cpp#L60) when a new file is created at non-existing path
    - [recursive folders auto deletion](https://github.com/esp8266/Arduino/blob/master/libraries/LittleFS/src/LittleFS.h#L149) on "last file" deletion (SPIFFS cannot have "folder w/o file")
    - review other differences: opendir(), rmdir(), unlink()
  - [ ] Retire this library when released by esp32 Arduino core
  - [ ] Cleanup examples 
