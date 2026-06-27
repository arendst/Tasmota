# Sensirion I²C STCC4 Arduino Library

This is the Sensirion STCC4 library for Arduino allowing you to
communicate with a STCC4 sensor
over I²C.

<img src="images/STCC4.png" width="300px">

Click [here](https://sensirion.com/products/catalog/STCC4) to learn more about the Sensirion STCC4 sensor.



The default I²C address of [STCC4](https://sensirion.com/products/catalog/STCC4) is **0x64**.

> [!NOTE]
> The SEK-STCC4 board from Sensirion includes a STCC4 and a SHT4x for temperature and humidity compensation, which is controlled by the STCC4 through the integrated I2C controller interface. The provided examples are designed considering this sensor configuration.


## Installation of the library

This library can be installed using the Arduino Library manager:
Start the [Arduino IDE](http://www.arduino.cc/en/main/software) and open
the Library Manager via

`Sketch` ➔ `Include Library` ➔ `Manage Libraries...`

Search for the `Sensirion I2C STCC4` library in the `Filter
your search...` field and install it by clicking the `install` button.

If you cannot find it in the library manager, download the latest release as .zip file
and add it to your [Arduino IDE](http://www.arduino.cc/en/main/software) via

`Sketch` ➔ `Include Library` ➔ `Add .ZIP Library...`

Don't forget to **install the dependencies** listed below the same way via library
manager or `Add .ZIP Library`

#### Dependencies
* [Sensirion Core](https://github.com/Sensirion/arduino-core)

## Connect the sensor

Use the following pin description to connect your STCC4 to the standard I²C bus of your Arduino board:

<img src="images/STCC4_pinout.png" width="300px">

| *Pin* | *Cable Color* | *Name* | *Description*  | *Comments* |
|-------|---------------|:------:|----------------|------------|
| 1 | black | GND | Ground |
| 2 | red | VDD | Supply Voltage | 2.7V to 5.5V
| 3 | green | SDA | I2C: Serial data input / output |
| 4 | yellow | SCL | I2C: Serial clock input |




The recommended voltage is 3.3V.

### Board specific wiring
You will find pinout schematics for recommended board models below:



<details><summary>Arduino Uno</summary>
<p>

| *STCC4* | *STCC4 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| GND | 1 | black | GND |
| VDD | 2 | red | 3.3V |
| SDA | 3 | green | D18/SDA |
| SCL | 4 | yellow | D19/SCL |



<img src="images/Arduino-Uno-Rev3-i2c-pinout-3.3V.png" width="600px">
</p>
</details>




<details><summary>Arduino Nano</summary>
<p>

| *STCC4* | *STCC4 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| GND | 1 | black | GND |
| VDD | 2 | red | 3.3V |
| SDA | 3 | green | A4 |
| SCL | 4 | yellow | A5 |



<img src="images/Arduino-Nano-i2c-pinout-3.3V.png" width="600px">
</p>
</details>




<details><summary>Arduino Micro</summary>
<p>

| *STCC4* | *STCC4 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| GND | 1 | black | GND |
| VDD | 2 | red | 3.3V |
| SDA | 3 | green | D2/SDA |
| SCL | 4 | yellow | ~D3/SCL |



<img src="images/Arduino-Micro-i2c-pinout-3.3V.png" width="600px">
</p>
</details>




<details><summary>Arduino Mega 2560</summary>
<p>

| *STCC4* | *STCC4 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| GND | 1 | black | GND |
| VDD | 2 | red | 3.3V |
| SDA | 3 | green | D20/SDA |
| SCL | 4 | yellow | D21/SCL |



<img src="images/Arduino-Mega-2560-Rev3-i2c-pinout-3.3V.png" width="600px">
</p>
</details>




<details><summary>ESP32 DevKitC</summary>
<p>

| *STCC4* | *STCC4 Pin* | *Cable Color* | *Board Pin* |
| :---: | --- | --- | --- |
| GND | 1 | black | GND |
| VDD | 2 | red | 3V3 |
| SDA | 3 | green | GPIO 21 |
| SCL | 4 | yellow | GPIO 22 |



<img src="images/esp32-devkitc-i2c-pinout-3.3V.png" width="600px">
</p>
</details>



## Quick Start

1. Install the libraries and dependencies according to [Installation of the library](#installation-of-the-library)

2. Connect the STCC4 sensor to your Arduino as explained in [Connect the sensor](#connect-the-sensor)

3. Open the `exampleUsage` sample project within the Arduino IDE:

   `File` ➔ `Examples` ➔ `Sensirion I2C STCC4` ➔ `exampleUsage`



5. Click the `Upload` button in the Arduino IDE or `Sketch` ➔ `Upload`

4. When the upload process has finished, open the `Serial Monitor` or `Serial
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

## License

See [LICENSE](LICENSE).
