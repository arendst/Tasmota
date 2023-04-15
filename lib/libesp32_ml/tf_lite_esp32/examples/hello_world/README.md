<!-- mdformat off(b/169948621#comment2) -->

# Hello World Example

This example is designed to demonstrate the absolute basics of using [TensorFlow
Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers).
It includes the full end-to-end workflow of training a model, converting it for
use with TensorFlow Lite for Microcontrollers for running inference on a
microcontroller.

The model is trained to replicate a `sine` function and generates a pattern of
data to blink the built-in LED in a fade in/out pattern.

## Table of contents
<!--ts-->
* [Table of contents](#table-of-contents)
* [Deploy to Arduino](#deploy-to-arduino)
  * [Install the Arduino_TensorFlowLite library](#install-the-arduino_tensorflowlite-library)
  * [Load and run the example](#load-and-run-the-example)
<!--te-->

## Deploy to Arduino

The following instructions will help you build and deploy this sample
to [Arduino](https://www.arduino.cc/) devices.

The sample has been tested with the following devices:

- [Arduino Nano 33 BLE Sense](https://store.arduino.cc/usa/nano-33-ble-sense-with-headers)
- [Arduino Tiny Machine Learning Kit](https://store-usa.arduino.cc/products/arduino-tiny-machine-learning-kit)

The sample will use PWM to fade an LED on and off according to the model's
output. In the code, the `LED_BUILTIN` constant is used to specify the board's
built-in LED as the one being controlled. However, on some boards, this built-in
LED is not attached to a pin with PWM capabilities. In this case, the LED will
blink instead of fading.

![Animation on Nano 33 BLE Sense](../../docs/hello_world_animation.gif)

### Install the Arduino_TensorFlowLite library

To install the TensorFlow Lite Micro for Arduino library, see the
[how to install](../../README.md#how-to-install) instructions.

### Load and run the example

Once the library has been added, go to `File -> Examples`. You should see an
entry within the list named `Arduino_TensorFlowLite`. Select
it and click `hello_world` to load the example.

Use the Arduino IDE to build and upload the example. Once it is running,
you should see the built-in LED on your device flashing.

The Arduino Desktop IDE includes a plotter that we can use to display the sine
wave graphically. To view it, go to `Tools -> Serial Plotter`. You will see one
datapoint being logged for each inference cycle, expressed as a number between 0
and 255.

![Serial Plotter with Nano 33 BLE Sense](../../docs/hello_world_serial_plotter.png)
