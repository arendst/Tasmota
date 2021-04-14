## Eddystone URL beacon
EddystoneURL beacon by BeeGee based on
[pcbreflux ESP32 Eddystone URL deepsleep](https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook/ESP32_Eddystone_URL_deepsleep)

[EddystoneURL frame specification](https://github.com/google/eddystone/blob/master/eddystone-url/README.md)

   Create a BLE server that will send periodic Eddystone URL frames.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create advertising data
   3. Start advertising.
   4. wait
   5. Stop advertising.
   6. deep sleep
