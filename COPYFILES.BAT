@echo off

echo ***************************************************************
echo:
echo                  Copying Firmwares for Release
echo:
echo ***************************************************************

md %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release
cd %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\.pioenvs

copy sonoff-minimal\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_minimal.bin
copy sonoff-BG\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BG.bin
copy sonoff-BR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BR.bin
copy sonoff-CN\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CN.bin
copy sonoff-CZ\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CZ.bin
copy sonoff-DE\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE.bin
copy sonoff-EN\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN.bin
copy sonoff-ES\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES.bin
copy sonoff-FR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-FR.bin
copy sonoff-GR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-GR.bin
copy sonoff-HE\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HE.bin
copy sonoff-HU\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HU.bin
copy sonoff-IT\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-IT.bin
copy sonoff-NL\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-NL.bin
copy sonoff-PL\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PL.bin
copy sonoff-PT\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PT.bin
copy sonoff-RU\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-RU.bin
copy sonoff-TW\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TW.bin
copy sonoff-TR\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TR.bin
copy sonoff-UK\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-UK.bin
copy sonoff-DE_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE-4MB.bin
copy sonoff-EN_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN-4MB.bin
copy sonoff-ES_4MB\firmware.bin %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES-4MB.bin

copy sonoff-minimal\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_minimal.map
copy sonoff-BG\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BG.map
copy sonoff-BR\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-BR.map
copy sonoff-CN\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CN.map
copy sonoff-CZ\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-CZ.map
copy sonoff-DE\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE.map
copy sonoff-EN\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN.map
copy sonoff-ES\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES.map
copy sonoff-FR\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-FR.map
copy sonoff-GR\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-GR.map
copy sonoff-HE\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HE.map
copy sonoff-HU\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-HU.map
copy sonoff-IT\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-IT.map
copy sonoff-NL\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-NL.map
copy sonoff-PL\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PL.map
copy sonoff-PT\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-PT.map
copy sonoff-RU\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-RU.map
copy sonoff-TW\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TW.map
copy sonoff-TR\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-TR.map
copy sonoff-UK\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-UK.map
copy sonoff-DE_4MB\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-DE-4MB.map
copy sonoff-EN_4MB\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-EN-4MB.map
copy sonoff-ES_4MB\firmware.map %USERPROFILE%\Documents\GitHub\Sonoff-Tasmota_KNX\release\sonoff_KNX-ES-4MB.map

echo:
echo:
echo DONE...
echo:
pause
