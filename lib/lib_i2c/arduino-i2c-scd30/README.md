# Sensirion I²C SCD30 Arduino Library

This is the Sensirion SCD30 library for Arduino allowing you to
communicate with a SCD30 sensor
over I²C.

<img src="images/sensor_scd30_image.jpg" width="300px">

Click [here](https://sensirion.com/products/catalog/SCD30/) to learn more about the Sensirion SCD30 sensor.



The default I²C address of [SCD30](https://sensirion.com/products/catalog/SCD30/) is **0x61**.



## Installation of the library

This library can be installed using the Arduino Library manager:
Start the [Arduino IDE](http://www.arduino.cc/en/main/software) and open
the Library Manager via

`Sketch` ➔ `Include Library` ➔ `Manage Libraries...`

Search for the `Sensirion I2C SCD30` library in the `Filter
your search...` field and install it by clicking the `install` button.

If you cannot find it in the library manager, download the latest release as .zip file
and add it to your [Arduino IDE](http://www.arduino.cc/en/main/software) via

`Sketch` ➔ `Include Library` ➔ `Add .ZIP Library...`

Don't forget to **install the dependencies** listed below the same way via library
manager or `Add .ZIP Library`

#### Dependencies
* [Sensirion Core](https://github.com/Sensirion/arduino-core)

## Connect the sensor

Use the following pin description to connect your SCD30 to the standard I²C bus of your Arduino board:

<img src="images/scd30_pinout.jpg" width="300px">

| *Pin* | *Cable Color* | *Name* | *Description*  | *Comments* |
|-------|---------------|:------:|----------------|------------|
| 1 | red | VDD | Supply Voltage | 3.3V to 5.5V
| 2 | black | GND | Ground |
| 3 | yellow | SCL | I2C: Serial clock input |
| 4 | green | SDA | I2C: Serial data input / output |
| 5 |  | RDY |  | High when data is available - do not connect
| 6 |  | PWM |  | do not connect
| 7 | blue | SEL | Interface select | Pull to ground or floating for I2C




The recommended voltage is 3.3V.

### Board specific wiring
You will find pinout schematics for recommended board models below:



<details><summary>Arduino Uno</summary>
<p>

| *SCD30* | *SCD30 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| VDD | 1 | red | 3.3V |
| GND | 2 | black | GND |
| SCL | 3 | yellow | D19/SCL |
| SDA | 4 | green | D18/SDA |
| RDY | 5 |  |  |
| PWM | 6 |  |  |
| SEL | 7 | blue | GND |



<img src="images/Arduino-Uno-Rev3-i2c-pinout-3.3V-SEL.png" width="600px">
</p>
</details>




<details><summary>Arduino Nano</summary>
<p>

| *SCD30* | *SCD30 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| VDD | 1 | red | 3.3V |
| GND | 2 | black | GND |
| SCL | 3 | yellow | A5 |
| SDA | 4 | green | A4 |
| RDY | 5 |  |  |
| PWM | 6 |  |  |
| SEL | 7 | blue | GND |



<img src="images/Arduino-Nano-i2c-pinout-3.3V-SEL.png" width="600px">
</p>
</details>




<details><summary>Arduino Micro</summary>
<p>

| *SCD30* | *SCD30 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| VDD | 1 | red | 3.3V |
| GND | 2 | black | GND |
| SCL | 3 | yellow | ~D3/SCL |
| SDA | 4 | green | D2/SDA |
| RDY | 5 |  |  |
| PWM | 6 |  |  |
| SEL | 7 | blue | GND |



<img src="images/Arduino-Micro-i2c-pinout-3.3V-SEL.png" width="600px">
</p>
</details>




<details><summary>Arduino Mega 2560</summary>
<p>

| *SCD30* | *SCD30 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| VDD | 1 | red | 3.3V |
| GND | 2 | black | GND |
| SCL | 3 | yellow | D21/SCL |
| SDA | 4 | green | D20/SDA |
| RDY | 5 |  |  |
| PWM | 6 |  |  |
| SEL | 7 | blue | GND |



<img src="images/Arduino-Mega-2560-Rev3-i2c-pinout-3.3V-SEL.png" width="600px">
</p>
</details>




<details><summary>ESP32 DevKitC</summary>
<p>

| *SCD30* | *SCD30 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| VDD | 1 | red | 3V3 |
| GND | 2 | black | GND |
| SCL | 3 | yellow | GPIO 22 |
| SDA | 4 | green | GPIO 21 |
| RDY | 5 |  |  |
| PWM | 6 |  |  |
| SEL | 7 | blue | GND |



<img src="images/esp32-devkitc-i2c-pinout-3.3V-SEL.png" width="600px">
</p>
</details>



## Quick Start

1. Install the libraries and dependencies according to [Installation of the library](#installation-of-the-library)

2. Connect the SCD30 sensor to your Arduino as explained in [Connect the sensor](#connect-the-sensor)

3. Open the `exampleUsage` sample project within the Arduino IDE:

   `File` ➔ `Examples` ➔ `Sensirion I2C SCD30` ➔ `exampleUsage`



4. Click the `Upload` button in the Arduino IDE or `Sketch` ➔ `Upload`

5. When the upload process has finished, open the `Serial Monitor` or `Serial
   Plotter` via the `Tools` menu to observe the measurement values. Note that
   the `Baud Rate` in the used tool has to be set to `115200 baud`.

## Contributing

**Contributions are welcome!**

This Sensirion library uses
[`clang-format`](https://releases.llvm.org/download.html) to standardize the
formatting of all our `.cpp` and `.h` files. Make sure your contributions are
formatted accordingly:

The `-i` flag will apply the format changes to the files listed.

```bash
clang-format -i src/*.cpp src/*.h
```

Note that differences from this formatting will result in a failed build until
they are fixed.
:

## Known issues

* *softReset()*:
   After using the ``softReset()`` function on an Arduino MKR WIFI 1010, subsequent commands are no longer acknowledged.
   The I2C line remains low after receiving the first command byte.

   To make the provided example work on the Arduino MKR WIFI 1010, the call to ``softReset()`` and the subsequent ``delay()`` can be removed.

## License

See [LICENSE](LICENSE).