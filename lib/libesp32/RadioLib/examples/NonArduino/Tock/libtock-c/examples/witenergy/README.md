Wit Energy BLE App
==================

This Tock app uses BLE (via nRF serialization available on Hail and imix) to
read energy measurements from and control the Wit Energy BLE power meter.

Supported Meters
----------------

- [WitEnergy](https://www.amazon.com/WiTenergy-Bluetooth-Electricity-Monitor-Programmable/dp/B00OOHWFYE)
- [OORT](https://www.amazon.com/SmartSocket-Enabled-Energy-Android-Compatible/dp/B00OG897MY)

These meters are basically identical and either will work.

Usage
-----

The app basically allows you to toggle the internal relay of the meter. Pressing
the first user button on the hail/imix will toggle the relay. While the board
is connected over BLE with the meter it will print over UART the measurements
collected by the meter. The measurements are the correct values multiplied by
10^4.
