@echo off

echo ***************************************************************
echo:
echo                  Copying Firmwares for Release
echo:
echo ***************************************************************

md %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release
cd %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\.pioenvs

copy sonoff-BG\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BG.bin
copy sonoff-BR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BR.bin
copy sonoff-CN\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CN.bin
copy sonoff-CZ\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CZ.bin
copy sonoff-DE\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE.bin
copy sonoff-DE_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE-4MB.bin
copy sonoff-EN\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN.bin
copy sonoff-EN_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN-4MB.bin
copy sonoff-ES\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES.bin
copy sonoff-ES_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES-4MB.bin
copy sonoff-FR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-FR.bin
copy sonoff-GR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-GR.bin
copy sonoff-HE\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HE.bin
copy sonoff-HU\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HU.bin
copy sonoff-IT\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-IT.bin
copy sonoff-minimal\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_minimal.bin
copy sonoff-NL\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-NL.bin
copy sonoff-PL\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PL.bin
copy sonoff-PT\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PT.bin
copy sonoff-RU\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-RU.bin
copy sonoff-TW\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TW.bin
copy sonoff-TR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TR.bin
copy sonoff-UK\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-UK.bin

echo:
echo:
echo DONE...
echo:
pause
