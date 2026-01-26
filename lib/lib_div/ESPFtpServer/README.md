# ESPFtpServer

The history of this code is as follows:

* https://github.com/gallegojm/Arduino-Ftp-Server/tree/master/FtpServer: FTP server for Arduino Mega2560 and Due with ethernet module W5100, W5200 or W5500
* https://github.com/nailbuster/esp8266FTPServer: ported to ESP8266 and SPIFFS file system
* https://github.com/robo8080/ESP32_FTPServer_SD: ported to ESP32 and SD card file system
* https://github.com/fa1ke5/ESP32_FTPServer_SD_MMC: use SD card in SD_MMC mode (No SD lib, SD_MMC lib). Added fully fuctional passive mode ftp server, browse dir, change dir, rename dir/file, delete dir/file, upload and download files, dirs.

The current repository is forked from fa1ke5, and has the following changes:
* use of any file system file system (like SPIFFS/LittleFS/SD_MMC)
* codebase to work for both ESP8266 and ESP32
* clean-up of code layout and English
* addition of library description files
