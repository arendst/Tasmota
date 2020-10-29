<h1 align = "center">🌟LilyGO T-Watch🌟</h1>

**English | [中文](docs/details_cn.md)**


<h2 align = "left">⭐ News </h2>

1. In order to be compatible with multiple versions of T-Watch, all examples include a `config.h` file. For the first use, you need to define the **T-Watch** model you use in the `config.h` file
2. In the `config.h` file, you can also see similar definitions, such as **LILYGO_WATCH_LVGL**, **LILYGO_WATCH_HAS_MOTOR**, this type of definition, it will be responsible for opening the defined module function, all definitions Will be available here [View](./docs/defined_en.md)
3. Most of the examples are only used as hardware function demonstrations. This library only completes some initialization work and some demonstrations. For more advanced gameplay, please see [TTGO.h](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/src/TTGO.h), to understand how to initialize, after being familiar with it, you can completely leave this library for more advanced gameplay
- About API, please check the source code

<h2 align = "left">🚀 Characteristics</h2>

1. The library already contains all the hardware drivers for `T-Watch`
2. Using **TFT_eSPI** as the display driver, you can directly call **TFT_eSPI** through the construction object.
3. Using **lvgl 7.0.0** as the display graphics framework, the driver method has been implemented, you only need to call lvgl api according to your own needs.
4. For the use of lvgl please refer to **[lvgl docs](https://docs.lvgl.io/v7/en/html/)**


<h2 align = "left">🔷 Install</h2>

- Download a zipfile from github using the "Download ZIP" button and install it using the IDE ("Sketch" -> "Include Library" -> "Add .ZIP Library..."
- Clone this git repository into your sketchbook/libraries folder.
For more info, see https://www.arduino.cc/en/Guide/Libraries


<h2 align = "left">🔶 How to find the sample program</h2>

- In the Arduino board select `TTGO T-Watch`
- In the Arduino File -> Examples -> TTGO T-Watch

<h2 align = "left">🔷 Note</h2>

- If you don't have the `TTGO T-Watch` option in your board manager, please update the esp32 board as follows:
  - Using Arduino IDE Boards Manager (preferred)
    + [Instructions for Boards Manager](docs/arduino-ide/boards_manager.md)
  - Using Arduino IDE with the development repository
    + [Instructions for Windows](docs/arduino-ide/windows.md)
    + [Instructions for Mac](docs/arduino-ide/mac.md)
    + [Instructions for Debian/Ubuntu Linux](docs/arduino-ide/debian_ubuntu.md)
    + [Instructions for Fedora](docs/arduino-ide/fedora.md)
    + [Instructions for openSUSE](docs/arduino-ide/opensuse.md)

 <h2 align = "left">🔶 Precautions</h2>

- T-Watch-2019 : Since Twatch uses a special IO as the SD interface, please remove the SD card when downloading the program.


 <h2 align = "left">🔷 Other</h2>

- [Click for details of power consumption](docs/power.md)
- [Pin Map Click to View](docs/pinmap.md)
- [3D Shell](https://github.com/Xinyuan-LilyGO/LilyGo-TWatch-DataSheet/tree/master/3DSHELL)
- [Hardware Resource](https://github.com/Xinyuan-LilyGO/LilyGo-TWatch-DataSheet)
- [Acsip S76/78 Library&Examples](https://github.com/lewisxhe/Acsip-S7xG-Library)

 <h2 align = "left">🔶 More interesting projects</h2>

- [Flight Recorder](https://github.com/lyusupov/SoftRF/wiki/Flight-Recorder)
- [TTGO_T_Watch_Baidu_Rec](https://github.com/lixy123/TTGO_T_Watch_Baidu_Rec)
- [TTGO_T_Watch_Alarm_Clock](https://github.com/lixy123/TTGO_T_Watch_Alarm_Clock)
