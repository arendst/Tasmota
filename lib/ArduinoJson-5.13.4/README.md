![ArduinoJson](banner.svg)

---

[![Build status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/master?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/master) [![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoJson) [![Coverage Status](https://img.shields.io/coveralls/bblanchon/ArduinoJson.svg)](https://coveralls.io/r/bblanchon/ArduinoJson?branch=master) [![Star this project](http://githubbadges.com/star.svg?user=bblanchon&repo=ArduinoJson&style=flat&color=fff&background=007ec6)](https://github.com/bblanchon/ArduinoJson)

ArduinoJson is a C++ JSON library for Arduino and IoT (Internet Of Things).

## Features

* JSON decoding (comments are supported)
* JSON encoding (with optional indentation)
* Elegant API, easy to use
* Fixed memory allocation (zero malloc)
* No data duplication (zero copy)
* Portable (written in C++98, can be used in any C++ project)
* Self-contained (no external dependency)
* Small footprint
* Input and output streams
* [100% code coverage](https://coveralls.io/github/bblanchon/ArduinoJson)
* [Header-only library](https://en.wikipedia.org/wiki/Header-only)
* [MIT License](https://en.wikipedia.org/wiki/MIT_License)
* [Comprehensive documentation](https://arduinojson.org?utm_source=github&utm_medium=readme)

## Compatibility

ArduinoJson works on the following hardware:

* <img src="https://www.arduino.cc/favicon.ico" height="16" width="16"> Arduino boards: [Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno), [Due](https://www.arduino.cc/en/Main/ArduinoBoardDue), [Mini](https://www.arduino.cc/en/Main/ArduinoBoardMini), [Micro](https://www.arduino.cc/en/Main/ArduinoBoardMicro), [Yun](https://www.arduino.cc/en/Main/ArduinoBoardYun)...
* <img src="http://espressif.com/sites/all/themes/espressif/favicon.ico" height="16" width="16"> Espressif chips: [ESP8266](https://en.wikipedia.org/wiki/ESP8266), [ESP32](https://en.wikipedia.org/wiki/ESP32)
* <img src="https://www.wemos.cc/themes/martin-materialize-parallax/assets/favicon.ico" height="16" width="16"> WeMos boards: [D1](https://wiki.wemos.cc/products:d1:d1), [D1 mini](https://wiki.wemos.cc/products:d1:d1_mini),  ...
* <img src="http://redbearlab.com/favicon.ico" height="16" width="16"> RedBearLab boards: [BLE Nano](http://redbearlab.com/blenano/), [BLE Mini](http://redbearlab.com/blemini/), [WiFi Micro](https://redbear.cc/product/wifi/wifi-micro.html), [LOLIN32](https://wiki.wemos.cc/products:lolin32:lolin32)...
* <img src="https://www.pjrc.com/favicon.ico" height="16" width="16"> [Teensy](https://www.pjrc.com/teensy/) boards
* <img src="https://software.intel.com/sites/all/themes/zero/favicon.ico" height="16" width="16"> Intel boards: Edison, Galileo...
* <img src="https://www-assets.particle.io/images/favicon.png"  height="16" width="16"> Particle boards: [Photon](https://www.particle.io/products/hardware/photon-wifi-dev-kit), [Electron](https://www.particle.io/products/hardware/electron-cellular-dev-kit)...
* <img src="http://www.ti.com/favicon.ico" height="16" width="16"> Texas Instruments boards: [MSP430](http://www.ti.com/microcontrollers/msp430-ultra-low-power-mcus/overview/overview.html)...

ArduinoJson compiles with zero warning on the following compilers, IDEs, and platforms:

* <img src="https://www.arduino.cc/favicon.ico" height="16" width="16"> [Arduino IDE](https://www.arduino.cc/en/Main/Software)
* <img src="http://cdn.platformio.org/favicon.ico" height="16" width="16"> [PlatformIO](http://platformio.org/)
* <img src="http://energia.nu/img/favicon.ico" height="16" width="16"> [Energia](http://energia.nu/)
* <img src="http://www.visualmicro.com/pics/arduino-visual-studio-ld.png" height="16" width="16"> [Visual Micro](http://www.visualmicro.com/)
* <img src="http://www.atmel.com/Images/favicon.ico" height="16" width="16"> [Atmel Studio](http://www.atmel.com/microsite/atmel-studio/)
* <img src="https://www.iar.com/favicon.ico" height="16" width="16"> [IAR Embedded Workbench](https://www.iar.com/iar-embedded-workbench/)
* <img src="http://www.st.com/etc/clientlibs/st-site/media/app/images/favicon.png" height="16" width="16"> [Atollic TrueSTUDIO](https://atollic.com/truestudio/)
* <img src="http://www.keil.com/favicon.ico" height="16" width="16"> [Keil uVision](http://www.keil.com/)
* <img src="http://www.microchip.com/favicon.ico" height="16" width="16"> [MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide)
* <img src="https://gcc.gnu.org/favicon.ico" height="16" width="16"> [GCC](https://gcc.gnu.org/)
* <img src="https://clang.llvm.org/favicon.ico" height="16" width="16"> [Clang](https://clang.llvm.org/)
* <img src="https://www.visualstudio.com/favicon.ico" height="16" width="16"> [Visual Studio](https://www.visualstudio.com/)

## Quickstart

### Deserialization

Here is a program that parses a JSON document with ArduinoJson.

```c++
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

const char* sensor = root["sensor"];
long time          = root["time"];
double latitude    = root["data"][0];
double longitude   = root["data"][1];
```

See the [tutorial on arduinojson.org](https://arduinojson.org/doc/decoding/?utm_source=github&utm_medium=readme)

### Serialization

Here is a program that generates a JSON document with ArduinoJson:

```c++
StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.createObject();
root["sensor"] = "gps";
root["time"] = 1351824120;

JsonArray& data = root.createNestedArray("data");
data.add(48.756080);
data.add(2.302038);

root.printTo(Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

See the [tutorial on arduinojson.org](https://arduinojson.org/doc/encoding/?utm_source=github&utm_medium=readme)

## Documentation

The documentation is available on [arduinojson.org](https://arduinojson.org/?utm_source=github&utm_medium=readme), here are some shortcuts:

* The [Examples](https://arduinojson.org/example/?utm_source=github&utm_medium=readme) show how to use the library in various situations.
* The [API Reference](https://arduinojson.org/api/?utm_source=github&utm_medium=readme) contains the description of each class and function.
* The [FAQ](https://arduinojson.org/faq/?utm_source=github&utm_medium=readme) has the answer to virtually every question.
* The [ArduinoJson Assistant](https://arduinojson.org/assistant/?utm_source=github&utm_medium=readme) writes programs for you!

---

Do you like this library? Please [star this project on GitHub](https://github.com/bblanchon/ArduinoJson/stargazers)!

What? You don't like it but you *love* it?
We don't take donations anymore, but [we sell a book](https://arduinojson.org/book/?utm_source=github&utm_medium=readme), so you can help and learn at the same time!