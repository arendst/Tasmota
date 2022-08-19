# Bluetooth 5.x features

## About extended advertising
Extended advertising allows for much more capability and flexibility.

* Allows for 251 bytes of advertisement data and up to 1650 bytes when chained (configuration dependant) vs 31.

* New PHY's (physical layers) that allow for faster data rate (2M PHY) or long range/slower data rates (CODED PHY) as well as the original 1M PHY.

* New periodic advertising, allowing the scanning device to sync with the advertisements of a beacon. This allows for the scanning device to sleep or perform other tasks before the next expected advertisement is sent, preserving cpu cycles and power (To be implemented).  
<br/>

## Enabling extended advertising
Extended advertising is supported when enabled with the config option `CONFIG_BT_NIMBLE_EXT_ADV` set to a value of 1. This is done in menuconfig under `Component config > Bluetooth > NimBLE options > Enable extended advertising`, or set in `nimconfig.h` for Arduino, or in `build_flags` in PlatformIO.

When enabled the following will occur:
* `NimBLEScan::start` method will scan on both the 1M PHY and the coded PHY standards automatically.

* `NimBLEClient::connect` will use the primary PHY the device is listening on, unless specified (see below).

* `NimBLEClient::setConnectPhy` becomes available to specify the PHY's to connect with (default is all).

* `NimBLEAdvertising` is no longer available for use and is replaced by `NimBLEExtAdvertising`. `NimBLEDevice::getAdvertising` will now return an instance of `NimBLEExtAdvertising`.

* `NimBLEAdvertisementData` is no longer available for use and is replaced by `NimBLEExtAdvertisement`. This new class is where everything about the advertisement is configured, including the advertisement intervals and advertisement ended callback.



