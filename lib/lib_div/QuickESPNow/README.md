# Quick ESP-NOW

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gmag11/library/QuickEspNow.svg)](https://registry.platformio.org/libraries/gmag11/QuickEspNow)

## Introduction

This fork includes a change to the rx_cb signature for compatibility with the 5.x Espressif IDF.

This is a library for Arduino framework to be used with Espressif ESP8266 and ESP32 series microcontrollers.

ESP-NOW is a wireless communication protocol that allows two devices to send data to each other without the need for a wireless network. You can read more about it here [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)

## Benefits

Usage of ESP-NOW may not be straightforward. There are some restrictions that has to be considered, as limit of number of devices, WiFi channel selection, and other factors.

This library pretends to hide all that restrictions so that it can be used with just a few lines of code.

Besides, it removes some limitations:

- No more 20 devices limit. You can use ESP-NOW with **any number of devices**. Library takes control of peer registration and makes it transparent to you.
- Channel selection is not required for WiFi coexistence.
- No need to assign a role to each device. Just use it for peer to peer communication.
- **RSSI** information of every message, so that receiver can estimate how close the sender is.
- Receiver can distiguish between broadcast and unicast messages.
- Tested maximum througput, about 600 kbps continuous with default parameters.
- Encryption is not supported. Usage of ESP-NOW encryption restrict system to 6 devices. You can implement data encryption in a higher layer.
- Asynchronous Sending Mode: The send method now supports a blocking mode, returning a value only after the message send has been confirmed. It returns 0 if the message was sent successfully and a different value if there was an error. This feature ensures reliable message delivery by waiting for confirmation before proceeding withour the user needing to implement a TX callback function.
It is importatnt to know that the best performance is achieved when using asynchronous mode, as it allows the library to send messages in the background while the user code continues to run. The synchronous mode is provided for users who require a blocking send method for ease of use.
- Default Mode Change: For ease of use, the asynchronous mode is now the default configuration. This mode can be set during the `begin` function call, streamlining the setup process for new projects. This change aims to simplify the initial configuration for developers by automatically opting for the more user-friendly synchronous sending mode. If you require best performance, you can still change the mode to asynchronous by calling `begin` function with the proper parameter.

## Performance

I include this table to show the performance of the library. It was tested with ESP32 and ESP8266, sending messages as fastest as possible. The test has been repeated with different message lengths and using synchronous and asynchronous modes.

| Device | broadcast/unicast | sync/async | 250 bytes | 125 bytes | 75 bytes | 35 bytes |  12 bytes |
|--------|-------------------|------------|-----------|-----------|----------|----------|-----------|
| ESP32  |  broadcast        | async      | 640 kbps  | 450 kbps  | 340 kbps | 190 kbps |  75 kbps  |
| ESP32  |  broadcast        | sync       | 615 kbps  | 440 kbps  | 320 kbps | 180 kbps |  73 kbps  |
| ESP8266|  broadcast        | async      | 200 kbps  | 100 kbps  |  60 kbps |  28 kbps | 9.5 kbps  |
| ESP8266|  broadcast        | sync       | 200 kbps  | 100 kbps  |  60 kbps |  28 kbps | 9.5 kbps  |
| ESP32  |  unicast          | async      | 570 kbps  | 400 kbps  | 285 kbps | 160 kbps |  60 kbps  |
| ESP32  |  unicast          | sync       | 550 kbps  | 375 kbps  | 270 kbps | 150 kbps |  57 kbps  |
| ESP8266|  unicast          | async      | 200 kbps  | 100 kbps  |  60 kbps |  28 kbps | 9.5 kbps  |
| ESP8266|  unicast          | sync       | 200 kbps  | 100 kbps  |  60 kbps |  28 kbps | 9.5 kbps  |

**Note** : In previous versions of the library, esp8266 was able to send messages at 600 kBps, but it was a mistake. The actual performance is 200 kbps. The table has been updated to reflect the correct values. It was due a to a missing check to avoid sending a message before the previous one was confirmed. This check has been added in version 0.8.1.
It seems that this check is not completely mandarory and both ESP8266 and ESP32 are able to send messages correctly even if latest one has not been confirmed. I will investigate what implications this may have and if it is possible to (optionally) remove this check in future versions.

Please note that these maximum values represent the best-case scenario without any message loss, assuming the microcontroller is not running any other tasks.

However, it's important to consider that in synchronous mode, where the user code is blocked until the message is sent (which can take from 1 to 20 ms), the actual performance may be significantly lower depending on the rest of the code.

On the other hand, in asynchronous mode, the `send` function returns in just 22us for both ESP32 and ESP8266, so it is not expected to have a significant impact on the rest of the code.

Please note that the performance of ESP8266 is lower than ESP32. This may cause problems if an ESP32 is sending messages at a higher rate than the ESP8266 can handle. In such cases, the receiver may lose messages or even crash. If you need to use both devices in the same network, it is recommended to keep the message rate at a safe level for the slowest device.

## Usage

To get started with Quick ESP-NOW, simply call the `begin` function and set a receiving callback function if you need to receive data. Then, you can use the `send` function to send data to a specific device or use `ESPNOW_BROADCAST_ADDRESS` to send data to all listening devices on the same channel.

```C++
void dataReceived (uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
    Serial.printf("Received message: %.*s\n", len, data);
}

void setup () {
    Serial.begin (115200);
    WiFi.mode (WIFI_MODE_STA);
    WiFi.disconnect (false);
    quickEspNow.onDataRcvd (dataReceived);
    quickEspNow.begin (1); // If you are not connected to WiFi, channel should be specified
}

void loop () {
    String message = "Hello, world!";
    quickEspNow.send (DEST_ADDR, (uint8_t*)message.c_str (), message.length ());
    delay (1000);
}
```
