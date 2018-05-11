@echo off

echo ***************************************************************
echo:
echo                  Copying Firmwares for Release
echo:
echo ***************************************************************

cd %systemdrive%\users\%username%\Desktop\Sonoff-Tasmota-development\.pioenvs

copy sonoff-BG\firmware.bin sonoff_KNX-BG.bin
copy sonoff-CN\firmware.bin sonoff_KNX-CN.bin
copy sonoff-CZ\firmware.bin sonoff_KNX-CZ.bin
copy sonoff-DE\firmware.bin sonoff_KNX-DE.bin
copy sonoff-DE_4MB\firmware.bin sonoff_KNX-DE-4MB.bin
copy sonoff-EN\firmware.bin sonoff_KNX-EN.bin
copy sonoff-EN_4MB\firmware.bin sonoff_KNX-EN-4MB.bin
copy sonoff-ES\firmware.bin sonoff_KNX-ES.bin
copy sonoff-ES_4MB\firmware.bin sonoff_KNX-ES-4MB.bin
copy sonoff-FR\firmware.bin sonoff_KNX-FR.bin
copy sonoff-GR\firmware.bin sonoff_KNX-GR.bin
copy sonoff-HU\firmware.bin sonoff_KNX-HU.bin
copy sonoff-IT\firmware.bin sonoff_KNX-IT.bin
copy sonoff-minimal\firmware.bin sonoff_minimal.bin
copy sonoff-NL\firmware.bin sonoff_KNX-NL.bin
copy sonoff-PL\firmware.bin sonoff_KNX-PL.bin
copy sonoff-PT_PT\firmware.bin sonoff_KNX-PT_PT.bin
copy sonoff-PT_BR\firmware.bin sonoff_KNX-PT_BR.bin
copy sonoff-RU\firmware.bin sonoff_KNX-RU.bin
copy sonoff-TW\firmware.bin sonoff_KNX-TW.bin

echo:
echo:
echo DONE...
echo:
pause
