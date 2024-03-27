TasmotaAlert

Action to take in case of new release of RadioLib

20240327

1 - Remove folder `RadioLib\examples\NonArduino` (Fixes Github vulnerability alerts)
2 - Change in file `RadioLib\src\modules\SX126x\SX126x.cpp` all occurences of ``this->mod->hal->yield();`` into ``this->mod->hal->delay(0);`` (Fixes ESP8266 Panic core_esp8266_main.cpp:133 __yield)
3 - Change in file `RadioLib\src\Module.cpp` all occurences of ``this->hal->yield();`` into ``this->hal->delay(0);`` (Fixes ESP8266 Panic core_esp8266_main.cpp:133 __yield)


