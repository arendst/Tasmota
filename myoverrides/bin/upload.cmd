rem only erase sketch space
rem C:\Users\Public\arduino-1.8.5\portable\packages\esp8266\tools\esptool\2.5.0-3-20ed2b9/esptool.exe -vv -cd ck -cb 115200 -cp COM3 -ca 0x00000 -cf  sonoff.ino.6.4.1.19.bin
rem erase all flash - sketch and wifi
C:\Users\Public\arduino-1.8.5\portable\packages\esp8266\tools\esptool\2.5.0-3-20ed2b9/esptool.exe -vv -cd ck -cb 115200 -cp COM3 -ca 0x0 -cz 0x100000 -ca 0x00000 -cf  sonoff.ino.6.4.1.19.1kflash.bin