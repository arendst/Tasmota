# Модернизированный [Sonoff-Tasmota](https://github.com/arendst/Sonoff-Tasmota)
текущий номер релиза Sonoff-Tasmota 6.2.1.8 20180926
Прошивка обновляется по мере наличия свободного времени.

Для облегчения поддержки вносятся минимальные изменения в оригинальные файлы и добавляются новые.

Необходимость данного форка вызвана тем, что для автоматизации дома на базе **OpenHub** потребовалось изменить некоторые модули и написать новые модули для поддержки новых устройств.
Основные задачи:
* увеличено количество датчиков DS18B20 на одном устройстве. надо добавить поддержку двех ветвей дастчиков на одном устройстве.
* сокращены поля вывода MQTT, т.к. не хватало длины стандартного MQTT сообщения
* добавлена поддержка нескольких ADS1115 (для автоматизации требуется 16 ЦАП - обработка обратной связи с приводов воздушных заслонок)
* добавлена поддержка расширителя портов MCP23017 (в последней версии _**Sonoff-Tasmota**_) появилась поддержка. надо разбиратся.
* добавлена поддержка PWM драйвеа PCA9685 (тоже штатно появилась поддержка)
---
* (надо добавить конвертер MODBUS RTU - MQTT для Danfoss FC51)
* (надо добавить конвертер MODBUS RTU - MQTT для счетчика Меркурий 231)

---

## Основные изменения к **Sonoff-Tasmota**

**Список измененных файлов и причина их изменения:**

* ***patformio. ini*** - отключены лишние варианты сборок, в команде сборки включен ключ оределения ***_LVA*** активизирующий все мои изменения, настроен ком.порт.

* ***sanoff/i18n.h***  	- сокращен вывод сообщений MQTT. надо переделать.

* ***sanoff/xdrv_interface.ino***  (был добавлен дебуг вызова модулей, требуется при отладке собственных модулей)

* ***sanoff/xsns_12_ads1115_ada.ino***	- переделан вывод строк.
* ***sanoff/sonoff.ino*** - добавлен DEBUG для MQTT сообщений подключается _lva_post.h_, файлом _lva_post.h_ уточняем  последними определения (если чего-то надо переопределить)

* ***user_config.h***  не все  можно выключить (#define USE_I2C ) через ***user_config_override.h***, по этому правим user_config.h

  ***user_config_override.h*** 	- мои переопределение #define по умолчанию. здесь надо указать вариант моей сборки. включаем стандарную сборку tasmota USE_CLASSIC
* ***sanoff/lva_post.h*** -  здесь отключаем все лишнее из USE_CLASSIC, и делаем свои компоновки.

* ***sanoff/xdrv_interface.ino*** -добавлен LVA_DEBUG на сериал при  выполнении функции _XdrvCommand_
* ***sanoff/xsns_05_ds18x20_legacy.ino*** - увеличено количество датчиков до 24 (нахрена столько, если их всего у меня максимум на шине 16), сокращен вывод информации на webpage и MQTT. (_надо довести до ума, бросил_.)
* ***sanoff/xsns_12_ads1115.ino*** - стандартная библиотека передаланая на сокращенный вывод. только одину микросхему поддерживает.
* ***sanoff/xsns_12_ads1115_lva.ino*** - собственная библиотека поддержки 4 микросхем, надо проверять на совместимость с последней версией Тамоты.
* ***sanoff/xsns_91mcp_lva.ino*** - собственная библиотека для MCP23017, ранее в Тамоте не было поддержки MCP23017, сейчас появилась (***xsns_29_mcp230xx.ino***) надо разбиратся. Возможно надо убирать.


___
 Во всех откорректированных оригинальных файлах  в пятой строке добавлен текст **UPDATED LVA**
  изменения в сам тексте обрамлены строками:
>  // LVA <--

>#ifndef \_LVA

>#else

>#endif

>//  LVA  -->


### В весенне-летний период занимался стройкой дома не поддерживал проект, много воды утекло за это время. много чего появилось.

Пробую актализировать проект с версии (	5.12.0a) и понять, что изменилось в _**Sonoff-Tasmota**_ и провести синхронизаию с данным проектом.

---
#MODBUS

Новая версия _**Sonoff-Tasmota**_ научилась пуллить по MODBUS  разные устройства используя програмномый сериал на скорости 9600.

 **xsns_23_ sdm120 .ino** - Eastron SDM120 -Modbus-измеритель мощности. по умолчанию включено варианте сборки **sensors**. поддерка фключается в **sonoff_post.h**
функция SDM120250ms опрашивает устройство и читает 8 адрессов попорядку, приетом в функции сначала проверяется чтение, а только потом посылается запрос устройству. только чтение и публикация 8 параметров.

**xnrg_05_pzem2.ino** - PZEM-003,017 и PZEM-014,016 Поддержка датчика энергопотребления Modbus.

**xsns_17_senseair.ino** - Поддержка датчика CO2 SenseAir K30, K70 и S8 
все эти датчики не плохой вариант считывать меркурий 230, думаю должны быть очень похожие таблицы.

Сравнивая эти модули, думаю самым аккуратно написанным является  **xsns_23_ sdm120 .ino**, берем его за основу для Danfoss FC51
текущая основная поблема, у меня max485, она требует переключения прием/передача, надо дергать третью ногу намомент передачи.
в приципе бибилотеку TasmotaSerial не надо, достаточно перед записью в порт опускать ногу, а по окончании поднимать.
осталосб разобратся как дергать нагами.
 irsend = new IRsend(pin[GPIO_IRSEND]); // an IR led is at GPIO_IRSEND
 определение пина  брать надо от сюда  _sonoff_template.h_  "enum UserSelectablePins", только проверять по номеру из вебинтерфейса, что он доступен для определения.


---
Добавились функции вызова модулей файл xdrv_interface.ino

 * Function call to all xdrv
 *
 * FUNC_PRE_INIT
 * FUNC_INIT
 * FUNC_LOOP
 * FUNC_MQTT_SUBSCRIBE
 * FUNC_MQTT_INIT
 * return FUNC_MQTT_DATA
 * return FUNC_COMMAND
 * FUNC_SET_POWER
 * FUNC_SHOW_SENSOR
 * FUNC_EVERY_SECOND
 * FUNC_EVERY_50_MSECOND
 * FUNC_EVERY_100_MSECOND
 * FUNC_EVERY_250_MSECOND
 * FUNC_RULES_PROCESS
 * FUNC_FREE_MEM

4.разбираем как работает _**Sonoff-Tasmota**_ (шпаргалка, чтобы запомнить)
4.1 - сначала обрабатывается _sonoff.ino_, где включается _user_config.h_ и т.д.

4.2 название модулей 
 * xdrv_ - драйвера
 * xdsp_ - модули для работы с дисплеями
 * xnrg_ - модули для работы с сенсорами мощности
 * xsns  - модули для работы с прочими сенсорами



5. [Правила оформления файла README.MD на GITHUB](http://webdesign.ru.net/article/pravila-oformleniya-fayla-readmemd-na-github.html)




7. Сейчас рабает два модуля ESP (WEMOS d1 mini V2) 
Камин  (Датчики теспературы (5 шт.) подулючены к D4 )
отопление  (Датчики теспературы (11 шт) подулючены к D4 ) модуль ополения постоянно висит, горит светодиод GPIO2 (D4) или он всетаки на ТХ (GPI01)пине?

надо сделать по принципу оригинала: разные ключи под прошивки для разных устройств:

* камин (температурные датчики 5 шт, электрический счетчик отопления)
* отопление (температурны датчики, расширительн портов для контроля напряжений, управления АВР дизеля)
* Меркурий (для счетчика на столбе)
* чердак (данфосы, de-icing, температурные датчики, управление приводами вентиляции)


99. Шпаргалки
* Как найти в файлах всей папки -  сочетание клавиш Shift + Ctrl + F. Замена во всех файлах проекта также делается.


BE_MINIMAL + user_config_overgrive

DATA:    [======    ]  58.5% (used 47952 bytes from 81920 bytes)
PROGRAM: [===       ]  33.2% (used 347915 bytes from 1048576 bytes)

+ выключили USE_KNX_WEB_MENU

---
## xsns_29_mcp230xx.ino

Взято от [сюда](https://github.com/arendst/Sonoff-Tasmota/wiki/MCP23008-MCP23017).

Формат команды в консоле (как в MQTT пока не понятно) 

спросить состояние пина D9 
>sensor29 9,?

>15:06:25 RSL: RESULT = {"Sensor29_D9":{"MODE":1,"PULL_UP":"OFF","INT_MODE":"DISABLED","STATE":"OFF"}}

1. Настройка пинов, общий синтаксис:

>sensor29 pin,pinmode,pullup
где
- `pin`:  от 0 до 15
- `pinmode`:
    - 1 = `INPUT` (данные телеметрической передачи с плавающей точкой будут отправляться в соответствии с интервалами конфигурации прошивки Tasmota
    - 2 = `INPUT` с `INTERRUPT` на `CHANGE` (Будет отправлен вывод MQTT при изменении состояния от LOW до HIGH и HIGH до LOW)
    - 3 = `INPUT` с `INTERRUPT` на CHANGE до LOW (отправит вывод MQTT с изменением состояния только от `HIGH до LOW`)
    - 4 = `INPUT` с `INTERRUPT` на CHANGE до HIGH (выведет выход MQTT с изменением состояния только от `LOW до HIGH`)
    - 5 = `OUTPUT` (если включено с #define USE_MCP230xx_OUTPUT)

- `pullup` = Рабочий режим штыря следующим образом:
  - 0 = подтяжка ОТКЛЮЧЕНА
  - 1 = подтяжка ВКЛЮЧЕНА

- `intmode` = Необязательно указать режим представления прерываний следующим образом - по умолчанию 0, если не указано
  - 0 = Отчет о немедленном прерывании с использованием TELEMETRY и EVENT
  - 1 = Только немедленное СОБЫТИЕ (телеметрия не указана)
  - 2 = Непосредственная ТЕЛЕМЕТРИЯ (не вызвано событием)

Примеры некоторых вариантов конфигурации контактов:

_sensor29 4,1,0_ - Включит `D4` для `INPUT` без внутреннего подтягивающего резистора

_sensor29 3,1,1_ - Включит `D3` для `INPUT` с внутренним подтягивающим резистором `ENABLED`

_sensor29 5,2,1_ - Включит `D5` для `INPUT` и сообщит о состоянии изменения от `LOW до HIGH` и `HIGH до LOW` через MQTT

_sensor29 6,3,1_ - Включит `D6` для `INPUT` и сообщит о состоянии изменения от `HIGH` до `LOW` (также включена подсветка)

_sensor29 2,4,0_ - Включит `D2` для `INPUT` и сообщит о состоянии изменения от `LOW до HIGH` (заметка подтягивания не включена)


2. `Сброс и настройка всех пинов сразу`

_sensor29 reset_    // Reset all pins INPUT, no interrupt, no pull-up by default

_sensor29 reset1_   // Reset all pins INPUT, no interrupt, no pull-up by default

_sensor29 reset2_   // Reset all pins INT on CHANGE, with pull-up enabled by default

_sensor29 reset3_   // Reset all pins INT on LOW, with pull-up enabled by default

_sensor29 reset4_   // Reset all pins INT on HIGH, with pull-up enabled by default

_sensor29 reset5_   // Reset all pins to OUTPUT mode (if enabled by #define USE_MCP230xx_OUTPUT)

3. на отладочной плате конпка D4 светодиоды D8 (снят резестор),D9,D10 

4. `Примеры`:

>sensor29 0,5,0  // Configure pin 0 as OUTPUT and default to OFF on reset/power-up

>sensor29 0,5,1  // Configure pin 0 as OUTPUT and default to ON on reset/power-up

>sensor29 0,6,0  // Configure pin 0 as INVERTED OUTPUT and default to ON on reset/power-up

>sensor29 0,6,1  // Configure pin 0 as INVERTED OUTPUT and default to OFF on reset/power-up

>sensor29 9,5,0 - пин на OUT по умолчанию выключен

>sensor29 9,5,1 - пин на OUT по умолчанию включен

>sensor29 9,ON   // Turn pin ON (HIGH if pinmode 5 or LOW if pinmode 6(inverted))

>sensor29 9,OFF  // Turn pin OFF (LOW if pinmode 5 or HIGH if pinmode 6(inverted))

>sensor29 9,T    // Toggle the current state of pin from ON to OFF, or OFF to ON


>sensor29 reset2

>sensor29 9,5,0

>sensor29 9,ON

>sensor29 9,OFF

>sensor29 9,T

-----
# полезная опция I2Cscan
----

# [PCA9685: 16-channel, 12-bit PWM I²C-bus LED controller](https://github.com/arendst/Sonoff-Tasmota/wiki/PCA9685)



driver15 pwmf,frequency   // where frequency is the PWM frequency from 24 to 1526 in Hz
driver15 pwm,pin,pwmvalue // where pin = LED pin 0 through 15 and pwmvalue is the pulse width between 0 and 4096
driver15 pwm,pin,ON       // Fully turn a specific pin/LED ON
driver15 pwm,pin,OFF      // Fully turn a specific pin/LED OFF
driver15 reset            // Reset to power-up settings - i.e. F=50hz and all pins in OFF state


driver15 pwm,15,OFF
driver15 pwm,15,ON
driver15 pwm,15,1024
driver15 pwm,15,10
driver15 
sensor33 pwm,15,ON


sensor33 pwm,15,3900

sensor33 pwm,15,OFF
sensor33 pwm,15,ON
sensor33 pwm,15,ON
sensor33 pwm,15,4096
sensor33 pwm,15,4095
sensor33 pwm,15,4095

sensor33 pwm,13,4096
sensor33 pwm,14,40

