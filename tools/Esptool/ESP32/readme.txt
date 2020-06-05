These files are needed for flashing Tasmota32 with esptool.py to an ESP32.

Command syntax for flashing Tasmota32 firmware on ESP32 via Esptool (replace COM Port Number!):

esptool.py --chip esp32 --port COM5 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dout --flash_freq 40m --flash_size detect 0x1000 bootloader_dout_40m.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 tasmota32.bin
