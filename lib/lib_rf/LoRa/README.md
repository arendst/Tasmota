# Arduino LoRa

[![Build Status](https://travis-ci.org/sandeepmistry/arduino-LoRa.svg?branch=master)](https://travis-ci.org/sandeepmistry/arduino-LoRa)

An [Arduino](https://arduino.cc/) library for sending and receiving data using [LoRa](https://www.lora-alliance.org/) radios.

## Compatible Hardware

 * [Semtech SX1276/77/78/79](https://www.semtech.com/products/wireless-rf/lora-connect/sx1276) based boards including:
   * [Dragino Lora Shield](https://www.dragino.com/products/lora/item/102-lora-shield.html)
   * [HopeRF](https://www.hoperf.com/modules/lora/index.html) [RFM95W](https://www.hoperf.com/modules/lora/RFM95.html), [RFM96W](https://www.hoperf.com/modules/lora/RFM96.html), and [RFM98W](https://www.hoperf.com/modules/lora/RFM98.html)
   * [Modtronix](http://modtronix.com/) [inAir4](http://modtronix.com/inair4.html), [inAir9](http://modtronix.com/inair9.html), and [inAir9B](http://modtronix.com/inair9b.html)
 * [Arduino MKR WAN 1300](https://store.arduino.cc/usa/mkr-wan-1300)
   * **NOTE:** Requires firmware v1.1.6 or later on the on-board Murata module. Please use the [MKRWANFWUpdate_standalone example](https://github.com/arduino-libraries/MKRWAN/blob/master/examples/MKRWANFWUpdate_standalone/MKRWANFWUpdate_standalone.ino) from latest [MKRWAN library](https://github.com/arduino-libraries/MKRWAN) release to update the firmware.
   * **WARNING**: [LoRa.onReceive(...)](https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md#register-callback) and [LoRa.recieve()](https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md#receive-mode) is not compatible with this board!

### Semtech SX1276/77/78/79 wiring

| Semtech SX1276/77/78/79 | Arduino |
| :---------------------: | :------:|
| VCC | 3.3V |
| GND | GND |
| SCK | SCK |
| MISO | MISO |
| MOSI | MOSI |
| NSS | 10 |
| NRESET | 9 |
| DIO0 | 2 |


`NSS`, `NRESET`, and `DIO0` pins can be changed by using `LoRa.setPins(ss, reset, dio0)`. `DIO0` pin is optional, it is only needed for receive callback mode. If `DIO0` pin is used, it **must** be interrupt capable via [`attachInterrupt(...)`](https://www.arduino.cc/en/Reference/AttachInterrupt).

**NOTES**:
 * Some boards (like the Arduino Nano), cannot supply enough current for the SX127x in TX mode. This will cause lockups when sending, be sure to use an external 3.3V supply that can provide at least 120mA's when using these boards.
 * If your Arduino board operates at 5V, like the Arduino Uno, Leonardo or Mega, you will need to use a level converter for the wiring to the Semtech SX127x module. Most Semtech SX127x breakout boards do not have logic level converters built-in.

## Installation

### Using the Arduino IDE Library Manager

1. Choose `Sketch` -> `Include Library` -> `Manage Libraries...`
2. Type `LoRa` into the search box.
3. Click the row to select the library.
4. Click the `Install` button to install the library.

### Using Git

```sh
cd ~/Documents/Arduino/libraries/
git clone https://github.com/sandeepmistry/arduino-LoRa LoRa
```

## API

See [API.md](API.md).

## Examples

See [examples](examples) folder.

## FAQ

**1) Initilizating the LoRa radio is failing**

Please check the wiring you are using matches what's listed in [Semtech SX1276/77/78/79 wiring](#semtech-sx1276777879-wiring). You can also use `LoRa.setPins(ss, reset, dio0)` to change the default pins used. Some logic level converters cannot operate at 8 MHz, you can call `LoRa.setSPIFrequency(frequency)` to lower the SPI frequency used by the library. Both API's must be called before `LoRa.begin(...)`.

**2) Can other radios see the packets I'm sending?**

Yes, any LoRa radio that are configured with the same radio parameters and in range can see the packets you send.

**3) Is the data I'm sending encrypted?**

No, all data is sent unencrypted. If want your packet data to be encrypted, you must encrypt it before passing it into this library, followed by decrypting on the receiving end.

**4) How does this library differ from LoRaWAN libraries?**

This library exposes the LoRa radio directly, and allows you to send data to any radios in range with same radio parameters. All data is broadcasted and there is no addressing. LoRaWAN builds on top of LoRA, but adds addressing, encryption, and additional layers. It also requires a LoRaWAN gateway and LoRaWAN network and application server.

**5) Does this library honor duty cycles?**

No, you have to manage it by your self.

**6) Which frequencies can I use?**

You can use [this table](https://www.thethingsnetwork.org/wiki/LoRaWAN/Frequencies/By-Country) to lookup the available frequencies by your country. The selectable frequency also depends on your hardware. You can lookup the data sheet or ask your supplier.

Please also notice the frequency dependent duty cycles for legal reasons!

## License

This libary is [licensed](LICENSE) under the [MIT Licence](https://en.wikipedia.org/wiki/MIT_License).
