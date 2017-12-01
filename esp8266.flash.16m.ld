/* Flash Split for 16M chips /
/ sketch 999KB /
/ eeprom 20KB /
/ spiffs 15MB */
MEMORY
{
dport0_0_seg : org = 0x3FF00000, len = 0x10
dram0_0_seg : org = 0x3FFE8000, len = 0x14000
iram1_0_seg : org = 0x40100000, len = 0x8000
irom0_0_seg : org = 0x40201010, len = 0xfeff0
}
PROVIDE ( _SPIFFS_start = 0x402FB000 );
PROVIDE ( _SPIFFS_end = 0x402FB000 );
PROVIDE ( _SPIFFS_page = 0x100 );
PROVIDE ( _SPIFFS_block = 0x2000 );
INCLUDE "esp8266.flash.common.ld"
