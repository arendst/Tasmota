# Uncompilable CMake File to enable project editing with CLion IDE

cmake_minimum_required(VERSION 2.8.4)
project(arduino-mqtt)

include_directories(
        /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/
        /Users/256dpi/Development/Arduino/libraries/Ethernet/src
        /Users/256dpi/Development/Arduino/libraries/WiFi101/src
        /Users/256dpi/Development/Arduino/libraries/MKRGSM/src
        /Applications/Arduino.app/Contents/Java/libraries/Bridge/src
        /Users/256dpi/Library/Arduino15/packages/esp8266/hardware/esp8266/2.3.0/libraries/ESP8266WiFi/src
        /Users/256dpi/Library/Arduino15/packages/esp32/libraries/WiFi/src
        /Users/256dpi/Library/Arduino15/packages/esp32/libraries/WiFiClientSecure/src
        src/)

include_directories(src/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    examples/AdafruitHuzzahESP8266/AdafruitHuzzahESP8266.ino
        examples/AdafruitHuzzahESP8266Secure/AdafruitHuzzahESP8266Secure.ino
    examples/ArduinoEthernetShield/ArduinoEthernetShield.ino
    examples/ArduinoMKRGSM1400/ArduinoMKRGSM1400.ino
        examples/ArduinoMKRGSM1400Secure/ArduinoMKRGSM1400Secure.ino
    examples/ArduinoWiFi101/ArduinoWiFi101.ino
        examples/ArduinoWiFi101Secure/ArduinoWiFi101Secure.ino
    examples/ArduinoWiFiShield/ArduinoWiFiShield.ino
    examples/ArduinoYun/ArduinoYun.ino
        examples/ArduinoYunSecure/ArduinoYunSecure.ino
    examples/ESP32DevelopmentBoard/ESP32DevelopmentBoard.ino
        examples/ESP32DevelopmentBoardSecure/ESP32DevelopmentBoardSecure.ino
    src/lwmqtt
    src/MQTT.h
    src/MQTTClient.h)

add_executable(arduino-mqtt ${SOURCE_FILES})
