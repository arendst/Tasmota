These files are needed for flashing Tasmota32c3 with esptool.py to an ESP32-C3.

Command syntax for flashing Tasmota32c3 firmware on ESP32-C3 via Esptool:

esptool.py --chip esp32c3 --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0x0000 bootloader_dio_80m.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 tasmota32c3.bin
