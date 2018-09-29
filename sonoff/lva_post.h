/*
 здесь последними пробуем переопределить ранее назначенне определения.
 включаем ключем -D_LVA при компиляции

этот файл подключается последним в sonoff.ino

переопределяет 

#include "sonoff_post.h"                    // Configuration overrides for all previous includes
#include "i18n.h"                           // Language support configured by user_config.h
#include "sonoff_template.h"   
*/
#ifndef _LVA_POST_H_
#define _LVA_POST_H_
#ifdef LVA_POST
// довыключаем, что не выключило  BE_MINIMAL
#undef USE_HLW8012       // Use energy sensor for Sonoff Pow and WolfBlitz
#undef USE_CSE7766       // Use energy sensor for Sonoff S31 and Pow R2
// безсмысленно #undef USE_KNX_WEB_MENU

/*
#undef USE_DHT           // Default DHT11 sensor needs no external library
#undef USE_ENERGY_SENSOR // Use energy sensors (+14k code)
*/
// выключаем лишнее что есть в USE_CLASSIC
#undef USE_WPS
#undef USE_SMARTCONFIG
#undef USE_DOMOTICZ
#undef USE_HOME_ASSISTANT
#undef USE_EMULATION // Disable Belkin WeMo and Hue Bridge emulation for Alexa (-16k code, -2k mem)
#undef USE_DISCOVERY // Disable Discovery services for both MQTT and web server
#undef WEBSERVER_ADVERTISE // Provide access to webserver by name <Hostname>.local/
#undef MQTT_HOST_DISCOVERY
#undef USE_ADC_VCC
#undef USE_DS18x20        // Disable DS18x20 sensor
#undef USE_DS18x20_LEGACY // Disable DS18x20 sensor
#undef USE_DS18B20        // Disable internal DS18B20 sensor
#undef USE_WS2812
#undef USE_DHT
#undef USE_ENERGY_SENSOR
#undef USE_MCP39F501
/*
-------------------------------------------
это минимальная конфигурация, все что можно и нельзя отключили

DATA:    [======    ]  60.2% (used 49296 bytes from 81920 bytes)
PROGRAM: [====      ]  35.6% (used 373079 bytes from 1048576 bytes)
-------------------------------------------
*/

/*
КАМИН
*/
#ifdef KAMIN
    //#warning "KAMIN configuratung. only DS18b20"
    // подключаем модернизированную мною  библиотеку. 
    #define USE_DS18x20_LEGACY                       // Optional for more than one DS18x20 sensors with dynamic scan using library OneWire
    //DATA:    [======    ]  60.4% (used 49520 bytes from 81920 bytes) +k224
    //PROGRAM : [== == ] 35.8 % (used 375783 bytes from 1048576 bytes) +2k704
    //#define USE_DS18x20

#endif //KAMIN

/*
 крыша
*/

#ifdef ROOF
//#warning "KAMIN configuratung. only DS18b20"
// подключаем модернизированную мною  библиотеку.
#define USE_DS18x20_LEGACY // Optional for more than one DS18x20 sensors with dynamic scan using library OneWire
//DATA:    [======    ]  60.4% (used 49520 bytes from 81920 bytes) +k224
//PROGRAM : [== == ] 35.8 % (used 375783 bytes from 1048576 bytes) +2k704
//#define USE_DS18x20
#define USE_I2C // автоматически она не включается
#define USE_ADS1115 // для обратной связи приводов, мною доработанный родной дравер

//#define USE_MCP23017 // мой модуль, он умеет сам искать адрес 
// управление портами de-icing, включать блок <-
#define USE_MCP230xx                           // Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+4k7 code)
#define USE_MCP230xx_ADDR 0x20               // Enable MCP23008/MCP23017 I2C Address to use (Must be within range 0x20 through 0x27 - set according to your wired setup)
#define USE_MCP230xx_OUTPUT                  // Enable MCP23008/MCP23017 OUTPUT support through sensor29 commands (+1k5 code)
#define USE_MCP230xx_DISPLAYOUTPUT           // Enable MCP23008/MCP23017 to display state of OUTPUT pins on Web UI (+0k2 code)
// ->

// PWM драйвер, над овключать обе опции
#define USE_PCA9685
#define USE_PCA9685_ADDR 0x41 // от 40 до 47 у меня на тесте распаянно на 41


#endif //ROOF

#ifdef _LVA_DEBUG
    #define USE_DEBUG_DRIVER
//#define DEBUG_THEO //неработает т.к. нет RtcSettingsDump() я открыл проблему #3917
#endif

//#undef USE_CLASSIC
//#define USE_SENSORS

#endif ///LVA_POST
#endif                   // _LVA_POST_H_