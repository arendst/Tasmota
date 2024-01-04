# GPIOViewer Arduino Library to see live GPIO Pins on ESP32 boards

**Transforms the way you troubleshoot your microcontroller projects**.

### Youtube Tutorial
[<img src="https://github.com/thelastoutpostworkshop/images/blob/main/GPIO%20Viewer.png" width="300">](https://youtu.be/UxkOosaNohU)

### Installation Arduino IDE (Version 2)
>ℹ️ Make sure you have the latest ESP32 boards by Espressif Systems in your Board Manager<br>

- Install the **GPIOViewer Library with the Arduino IDE Library Manager** or Download the [latest release](https://github.com/thelastoutpostworkshop/gpio_viewer/releases/latest) and install the library in the Arduino IDE : `Sketch > Include Library > Add ZIP Library...`
- Download [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) and install the library in the Arduino IDE `Sketch > Include Library > Add ZIP Library...`
- Install the the [AsyncTCP](https://github.com/dvarrel/AsyncTCP) using the Arduino IDE Library Manager.

### Installation VSCode + PlatformIO

Add the following to your platformio.ini file:

```
lib_deps =
    https://github.com/dvarrel/AsyncTCP.git
    https://github.com/me-no-dev/ESPAsyncWebServer.git
    https://github.com/thelastoutpostworkshop/gpio_viewer.git
```

### Usage
>ℹ️ You can also use get examples provided with the library in the Arduino IDE through the menu `File > Examples > GPIOViewer`<br>
>ℹ️ You only need to include the library, declare the GPIOViewer and call begin() at the end of your setup, and that's it!<br>
>ℹ️ The URL to the web GPIO viewer application is printed on the serial monitor<br>
```c
#include <gpio_viewer.h> // Must me the first include in your project
GPIOViewer gpio_viewer;

void setup()
{
  Serial.begin(115200);

  // Comment the next line, If your code aleady include connection to Wifi
  gpio_viewer.connectToWifi("Your SSID network", "Your WiFi Password");
  // gpio_viewer.setPort(5555);   // You can set the http port, if not set default port is 8080

  // Your own setup code start here

  // Must be at the end of your setup
  // gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 100ms
  gpio_viewer.begin();
}
```
>ℹ️ The default HTTP port is **8080** and default sampling interval is **100ms**.

### GPIO Supported

- Digital
- Analog
- PWM

### Library Size

- The GPIOViewer Library adds 50 KB to your projects.
- No worries!  All the assets (ex. board images) of the web application are loaded from github pages and don't add to the size of your projects.

### Performance
- Ensure you have a strong Wifi signal with a good transfer rate.  25ms sampling interval works great on Wifi 6 with 125 Mbps.
- If you get "ERROR: Too many messages queued" on the Serial Monitor, this means the data is not read fast enough by the web application.  The data will still be displayed, but with some latency.  Reduce the sampling interval or try to improve your Wifi performance.

### ESP32 Boards Supported
>ℹ️ You can use the "Generic View" in the GPIO Web Application to see GPIO pin activites live even if your board image is not listed <br>
>ℹ️ You can also request an ESP32 board image addition by [creating a new issue](https://github.com/thelastoutpostworkshop/gpio_viewer/issues).


| Description        | Image                               | Pinout                               |
|--------------------|-------------------------------------|-------------------------------------|
| ESP32 VROOM 32D (38 pins)  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32-VROOM-32D.png" width="100">             | <img src="https://docs.espressif.com/projects/esp-idf/en/latest/esp32/_images/esp32-devkitC-v4-pinout.png" width="100">|
| ESP32 VROOM 32D (30 pins)   | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32-VROOM-32D-30pins.png" width="100">             |<img src="https://raw.githubusercontent.com/AchimPieters/esp32-homekit-camera/master/Images/ESP32-30PIN-DEVBOARD.png" width="100">|
| ESP32 D1 R32  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32_D1_R32.png" width="100">             |<img src="https://ito-iot.jp/sbc/WeMosD1R32-pinout.png" width="100">  |
| ESP32 C3 Wroom-02  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/esp32-C3-Wroom-02.png" width="100">             |<img src="https://m.media-amazon.com/images/S/aplus-media-library-service-media/a80e998c-f2b4-4c4a-9ee7-d97ede61e14c.__CR0,0,970,600_PT0_SX970_V1___.jpg" width="100">|
| ESP32 Wroom-32UE  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/esp32-Wroom-32UE.png" width="100">             |<img src="https://media.springernature.com/lw685/springer-static/image/chp%3A10.1007%2F978-3-031-02447-4_74/MediaObjects/519315_1_En_74_Fig2_HTML.png" width="100">|
| ESP32 EVB  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32-EVB.png" width="100">             |<img src="https://gitlab.com/gschorcht/RIOT.wiki-Images/raw/master/esp32/Olimex_ESP32-EVB_pinout.png" width="100">|
| ESP32 S3 Wroom-1  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/esp32-s3-wroom-1.png" width="100">             |<img src="https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout_v1.1.jpg" width="100">|
| Esp32 S2 Mini V1.0.0  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Esp32 S2 Mini V1.0.0.png" width="100">             |<img src="https://www.studiopieters.nl/wp-content/uploads/2022/08/WEMOS-ESP32-S2-MINI.png" width="100">|
| ESP32 POE  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32-POE.png" width="100">             |<img src="https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/resources/ESP32-POE-GPIO.png" width="100">|
| ESP32 C3 Mini  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/ESP32-C3-Mini.png" width="100">             |<img src="https://www.wemos.cc/en/latest/_static/boards/c3_mini_v2.1.0_4_16x9.png" width="100">|
| ESP32 Pico Kit v4.1  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/esp32-pico-kit-v4.1.png" width="100">             |<img src="https://docs.espressif.com/projects/esp-idf/en/latest/esp32/_images/esp32-pico-kit-v4-pinout.png" width="100">|
| Lilygo T7 Mini32 v1.5  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Lilygo-T7-Mini32-v1.5.png" width="100">             |<img src="https://ff.lnwfile.com/_/ff/_raw/cw/le/pb.jpg" width="100">|
| Freenove ESP32-S3 | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Freenove-esp32-s3.png" width="100">             |<img src="https://templates.blakadder.com/assets/device_images/wemos_D1_Mini_ESP32_pinout.webp" width="100">|
| Nano ESP32  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Nano-ESP32.png" width="100">             |<img src="https://docs.arduino.cc/static/8307a46e73a4b5b153aa9ec0a4082443/ABX00083-pinout.png" width="100">|
| StickLite-V3-ESP32S3  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/StickLite-V3-ESP32S3.png" width="100">             |<img src="https://heltec.org/wp-content/uploads/2023/09/HTIT-WSL_V3.png" width="100">|
| XIAO-ESP32-C3  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/XIAO-ESP32-C3.png" width="100">             |<img src="https://raw.githubusercontent.com/Freenove/Freenove_ESP32_S3_WROOM_Board/main/ESP32S3_Pinout.png" width="100">|
| T-Display S3 AMOLED | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/T-Display-S3-AMOLED.png" width="100">             |<img src="https://raw.githubusercontent.com/Xinyuan-LilyGO/T-Display-S3-AMOLED/main/image/T-Display-S3-AMOLED.jpg" width="100">|
| TinyPICO Nano  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/TinyPICO-Nano.png" width="100">             |<img src="https://d2j6dbq0eux0bg.cloudfront.net/images/90477757/3788332370.jpg" width="100">|
| TinyPico V3  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/TinyPico-V3.png" width="100">             |<img src="https://images.squarespace-cdn.com/content/v1/5c85d89877b903606126e6df/c4a98965-7415-414e-8358-339e3a0ce837/thumb-tinypico-v3_pinout_1200px.jpg?format=2500w" width="100">|
| Wemos Lolin32 Lite V1  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Wemos-Lolin32-Lite-V1.png" width="100">             |<img src="https://templates.blakadder.com/assets/device_images/wemos_LOLIN32_Lite_v1_pinout.webp" width="100">|
| Wemos D1 Mini ESP32  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/Wemos-D1-Mini-ESP32.png" width="100">             |<img src="https://templates.blakadder.com/assets/device_images/wemos_D1_Mini_ESP32_pinout.webp" width="100">|
| XIAO-ESP32-C3  | ! <img src="https://github.com/thelastoutpostworkshop/microcontroller_devkit/blob/main/gpio_viewer/assets/devboards_images/XIAO-ESP32-C3.png" width="100">             |<img src="https://files.seeedstudio.com/wiki/XIAO_WiFi/pin_map-2.png" width="100">|
