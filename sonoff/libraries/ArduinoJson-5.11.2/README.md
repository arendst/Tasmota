[![Build status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/master?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/master) [![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoJson) [![Coverage Status](https://img.shields.io/coveralls/bblanchon/ArduinoJson.svg)](https://coveralls.io/r/bblanchon/ArduinoJson?branch=master) [![Star this project](http://githubbadges.com/star.svg?user=bblanchon&repo=ArduinoJson&style=flat&color=fff&background=007ec6)](https://github.com/bblanchon/ArduinoJson)

![ArduinoJson's logo](banner.svg)

ArduinoJson - C++ JSON library for IoT
====================

*An elegant and efficient JSON library for embedded systems.*

It's designed to have the most intuitive API, the smallest footprint and is able to work without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.
For instance, it supports Aduino's `String` and `Stream`, but also `std::string`, `std::istream` and `std::ostream`.

Features
--------

* JSON decoding (comments are supported)
* JSON encoding (with optional indentation)
* Elegant API, very easy to use
* Fixed memory allocation (zero malloc)
* No data duplication (zero copy)
* Portable (written in C++98)
* Self-contained (no external dependency)
* Small footprint
* Header-only library
* MIT License

Works on
--------

* Arduino boards: Uno, Due, Mini, Micro, Yun...
* ESP8266, ESP32
* Teensy
* RedBearLab boards (BLE Nano...)
* Intel Edison and Galileo
* WeMos boards: D1...
* Computers: Windows, Linux, OSX...
* PlatformIO
* Particle
* Energia

Quick start
-----------

#### Decoding / Parsing

```c++
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

const char* sensor = root["sensor"];
long time          = root["time"];
double latitude    = root["data"][0];
double longitude   = root["data"][1];
```

[See JsonParserExample.ino](examples/JsonParserExample/JsonParserExample.ino)

Use [ArduinoJson Assistant](https://bblanchon.github.io/ArduinoJson/assistant/) to compute the buffer size.

#### Encoding / Generating

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

[See JsonGeneratorExample.ino](examples/JsonGeneratorExample/JsonGeneratorExample.ino)

Use [ArduinoJson Assistant](https://bblanchon.github.io/ArduinoJson/assistant/) to compute the buffer size.


Documentation
-------------

The documentation is available online in the [ArduinoJson Website](https://bblanchon.github.io/ArduinoJson/).

The [ArduinoJson Assistant](https://bblanchon.github.io/ArduinoJson/assistant/) helps you get started with the library.


Donators
--------

Special thanks to the following persons and companies who made generous donations to the library author:

* Robert Murphy <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Surge Communications <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Alex Scott <img alt='United Kingdom' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1ec-1f1e7.svg' width='18' height='18'>
* Firepick Services LLC <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* A B Doodkorte <img alt='Netherlands' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f3-1f1f1.svg' width='18' height='18'>
* Scott Smith <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Johann Stieger <img alt='Austria' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1e6-1f1f9.svg' width='18' height='18'>
* Gustavo Donizeti Gini <img alt='Brazil' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1e7-1f1f7.svg' width='18' height='18'>
* Charles-Henri Hallard <img alt='France' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1eb-1f1f7.svg' width='18' height='18'>
* Martijn van den Burg <img alt='Netherlands' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f3-1f1f1.svg' width='18' height='18'>
* Nick Koumaris <img alt='Greece' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1ec-1f1f7.svg' width='18' height='18'>
* Jon Williams <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Kestutis Liaugminas <img alt='Lithuania' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f1-1f1f9.svg' width='18' height='18'>
* Darlington Adibe <img alt='Nigeria' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f3-1f1ec.svg' width='18' height='18'>
* Yoeri Kroon <img alt='Netherlands' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f3-1f1f1.svg' width='18' height='18'>
* Andrew Melvin <img alt='United Kingdom' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1ec-1f1e7.svg' width='18' height='18'>
* Doanh Luong <img alt ='Vietnam' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fb-1f1f3.svg' width='18' height='18'>
* Christoph Schmidt <img alt='Germany' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1e9-1f1ea.svg' width='18' height='18'>
* OpenEVSE LLC <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Prokhoryatov Alexey <img alt='Russia' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f7-1f1fa.svg' width='18' height='18'>
* Google Inc. <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Charles Haynes <img alt='Australia' src='https://d1j8pt39hxlh3d.cloudfront.net/development/emojione/2.2/989/2546.svg' width='18' height='18'>
* Charles Walker <img alt='USA' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1fa-1f1f8.svg' width='18' height='18'>
* Günther Jehle <img alt='Liechtenstein' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1f1-1f1ee.svg' width='18' height='18'>
* Patrick Elliott


---

Found this library useful? Please star this project or [help me back with a donation!](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=donate%40benoitblanchon%2efr&lc=GB&item_name=Benoit%20Blanchon&item_number=Arduino%20JSON&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted) :smile:
