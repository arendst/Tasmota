set version=6.4.1.9-mod-1.43.9

mkdir %version%

copy ..\.pioenvs\sonoff\firmware.bin %version%\EN-firmware-%version%.bin
copy ..\.pioenvs\sonoff-BG\firmware.bin %version%\BG-firmware-%version%.bin
copy ..\.pioenvs\sonoff-BR\firmware.bin %version%\BR-firmware-%version%.bin
copy ..\.pioenvs\sonoff-CN\firmware.bin %version%\CN-firmware-%version%.bin
copy ..\.pioenvs\sonoff-CZ\firmware.bin %version%\CZ-firmware-%version%.bin
copy ..\.pioenvs\sonoff-DE\firmware.bin %version%\DE-firmware-%version%.bin
copy ..\.pioenvs\sonoff-ES\firmware.bin %version%\ES-firmware-%version%.bin
copy ..\.pioenvs\sonoff-FR\firmware.bin %version%\FR-firmware-%version%.bin
copy ..\.pioenvs\sonoff-GR\firmware.bin %version%\GR-firmware-%version%.bin
copy ..\.pioenvs\sonoff-HU\firmware.bin %version%\HU-firmware-%version%.bin
copy ..\.pioenvs\sonoff-HE\firmware.bin %version%\HE-firmware-%version%.bin
copy ..\.pioenvs\sonoff-IT\firmware.bin %version%\IT-firmware-%version%.bin
copy ..\.pioenvs\sonoff-NL\firmware.bin %version%\NL-firmware-%version%.bin
copy ..\.pioenvs\sonoff-PL\firmware.bin %version%\PL-firmware-%version%.bin
copy ..\.pioenvs\sonoff-PT\firmware.bin %version%\PT-firmware-%version%.bin
copy ..\.pioenvs\sonoff-RU\firmware.bin %version%\RU-firmware-%version%.bin
copy ..\.pioenvs\sonoff-UK\firmware.bin %version%\UK-firmware-%version%.bin
copy ..\.pioenvs\sonoff-TR\firmware.bin %version%\TR-firmware-%version%.bin
copy ..\.pioenvs\sonoff-TW\firmware.bin %version%\TW-firmware-%version%.bin

copy ..\.pioenvs\sonoff-minimal\firmware.bin %version%\MINIMAL-firmware-%version%.bin
copy ..\.pioenvs\sonoff-classic\firmware.bin %version%\CLASSIC-firmware-%version%.bin
copy ..\.pioenvs\sonoff-knx\firmware.bin %version%\KNX-firmware-%version%.bin
copy ..\.pioenvs\sonoff-allsensors\firmware.bin %version%\ALL_SENSORS-firmware-%version%.bin


copy ..\.pioenvs\sonoff-DE-4MB\firmware.bin %version%\4MB-DE-firmware-%version%.bin
copy ..\.pioenvs\wemos-d1-mini-DE\firmware.bin %version%\wemos-d1-mini-DE-firmware-%version%.bin
