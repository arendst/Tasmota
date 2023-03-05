# TensorFlow Lite Micro Library for Arduino-Espressif32

This repository has the code (including examples) needed to use Tensorflow Lite Micro on an Arduino.

## Table of contents
<!--ts-->
* [Table of contents](#table-of-contents)
* [Build Status](#build-status)
* [How to Install](#how-to-install)
  * [GitHub](#github)
  * [Checking your Installation](#checking-your-installation)
* [Compatibility](#compatibility)
* [License](#license)
* [Contributing](#contributing)
<!--te-->

## Build Status

Build Type          |     Status    |
---------------     | ------------- |
Arduino CLI on Linux  | [![Arduino](https://github.com/tensorflow/tflite-micro-arduino-examples/actions/workflows/ci.yml/badge.svg?event=schedule)](https://github.com/tensorflow/tflite-micro-arduino-examples/actions/workflows/ci.yml)
Sync from tflite-micro  | [![Sync from tflite-micro](https://github.com/tensorflow/tflite-micro-arduino-examples/actions/workflows/sync.yml/badge.svg)](https://github.com/tensorflow/tflite-micro-arduino-examples/actions/workflows/sync.yml)

## How to Install

### GitHub

The officially supported TensorFlow Lite Micro library for Arduino resides
in the [tflite-micro-arduino-examples](https://github.com/tensorflow/tflite-micro-arduino-examples)
GitHub repository.
To install the in-development version of this library, you can use the
latest version directly from the GitHub repository. This requires you clone the
repo into the folder that holds libraries for the Arduino IDE. The location for
this folder varies by operating system, but typically it's in
`~/Arduino/libraries` on Linux, `~/Documents/Arduino/libraries/` on MacOS, and
`My Documents\Arduino\Libraries` on Windows.

Once you're in that folder in the terminal, you can then grab the code using the
git command line tool:

```
git clone https://github.com/tensorflow/tflite-micro-arduino-examples Arduino_TensorFlowLite
```

To update your clone of the repository to the latest code, use the following terminal commands:
```
cd Arduino_TensorFlowLite
git pull
```

### Checking your Installation

Once the library has been installed, you should then start the Arduino IDE.
You will now see an `Arduino_TensorFlowLite`
entry in the `File -> Examples` menu of the Arduino IDE. This submenu contains a list
of sample projects you can try out.

![Hello World](docs/hello_world_screenshot.png)

## Compatibility

This library is designed for the `Arduino Nano 33 BLE Sense` board. The framework
code for running machine learning models should be compatible with most Arm Cortex
M-based boards, such as the `Raspberry Pi Pico`, but the code to access peripherals
like microphones, cameras, and accelerometers is specific to the `Nano 33 BLE Sense`.

## License

This code is made available under the Apache 2 license.

## Contributing

Forks of this library are welcome and encouraged. If you have bug reports or
fixes to contribute, the source of this code is at [https:://github.com/tensorflow/tflite-micro](github.com/tensorflow/tflite-micro)
and all issues and pull requests should be directed there.

The code here is created through an automatic project generation process
and may differ from
that source of truth, since it's cross-platform and needs to be modified to
work within the Arduino IDE.