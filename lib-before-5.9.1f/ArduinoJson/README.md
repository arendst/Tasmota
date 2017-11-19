ArduinoJson - C++ JSON library for IoT
====================

[![Build status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/master?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/master) [![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoJson) [![Coverage Status](https://img.shields.io/coveralls/bblanchon/ArduinoJson.svg)](https://coveralls.io/r/bblanchon/ArduinoJson?branch=master) [![Star this project](http://githubbadges.com/star.svg?user=bblanchon&repo=ArduinoJson&style=flat&color=fff&background=007ec6)](https://github.com/bblanchon/ArduinoJson)

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

* All Arduino boards (Uno, Due, Mini, Micro, Yun...)
* ESP8266
* Teensy
* Intel Edison and Galileo
* PlatformIO
* Energia
* RedBearLab boards (BLE Nano...)
* Computers (Windows, Linux, OSX...)

See [FAQ: Compatibility issues](https://bblanchon.github.io/ArduinoJson/faq/compilation-fails-device-crashes-nothing-on-serial-console)

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

#### Encoding / Generating

```c++
StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.createObject();
root["sensor"] = "gps";
root["time"] = 1351824120;

JsonArray& data = root.createNestedArray("data");
data.add(48.756080, 6);  // 6 is the number of decimals to print
data.add(2.302038, 6);   // if not specified, 2 digits are printed

root.printTo(Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

[See JsonGeneratorExample.ino](examples/JsonGeneratorExample/JsonGeneratorExample.ino)


Documentation
-------------

The documentation is available online in the [ArduinoJson wiki](https://github.com/bblanchon/ArduinoJson/wiki).

The [ArduinoJson Assistant](https://bblanchon.github.io/ArduinoJson/assistant/) helps you get started with the library.


Testimonials
------------

From Arduino's Forum user `jflaplante`:
> I tried aJson json-arduino before trying your library. I always ran into memory problem after a while.
> I have no such problem so far with your library. It is working perfectly with my web services.

From StackOverflow user `thegreendroid`:
> It has a really elegant, simple API and it works like a charm on embedded and Windows/Linux platforms. We recently started using this on an embedded project and I can vouch for its quality.

From GitHub user `zacsketches`:
> Thanks for a great library!!!
> I've been watching you consistently develop this library over the past six months, and I used it today for a publish and subscribe architecture designed to help hobbyists move into more advanced robotics. Your library allowed me to implement remote subscription in order to facilitate multi-processor robots.
> ArduinoJson saved me a week's worth of time!!

[From Reddit user `erm_what_`](https://www.reddit.com/r/arduino/comments/3jj6ep/announcing_arduinojson_50/cusjk8c):
> This is a great library and I wouldn't be able to do the project I'm doing without it. I completely recommend it.

[From Reddit user `makerhacks`](https://www.reddit.com/r/arduino/comments/3jj6ep/announcing_arduinojson_50/cusqg7b):
> I am just starting an ESP8266 clock project and now I can output JSON from my server script and interpret it painlessly.

[From Twitter user `@hemalchevli`](https://twitter.com/hemalchevli/status/715788439397011456):
> ArduinoJson library should be used as a benchmark/reference for making libraries. Truly elegant.

[From GitHub user `sticilface`](https://github.com/bblanchon/ArduinoJson/issues/381#issuecomment-260203594):
> its a great lib:) and i use it in everything!

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
* Christoph Schmidt <img alt ='Germany' src='https://cdnjs.cloudflare.com/ajax/libs/emojione/2.1.4/assets/svg/1f1e9-1f1ea.svg' width='18' height='18'>

---

Found this library useful? Please star this project or [help me back with a donation!](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=donate%40benoitblanchon%2efr&lc=GB&item_name=Benoit%20Blanchon&item_number=Arduino%20JSON&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted) :smile:
