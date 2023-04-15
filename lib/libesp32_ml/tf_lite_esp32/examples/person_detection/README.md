# Person detection example

This example shows how you can use [Tensorflow Lite Micro](https://www.tensorflow.org/lite/microcontrollers) to run a 300.5 kilobyte neural
network to recognize people in images.

## Table of contents
<!--ts-->
   * [Deploy to Arduino](#deploy-to-arduino)
      * [Install the Arduino_TensorFlowLite library](#install-the-arduino_tensorflowlite-library)
      * [Load and run the example](#load-and-run-the-example)
   * [Training your own model](#training-your-own-model)
<!--te-->

## Deploy to Arduino

The following instructions will help you build and deploy this example to
[Arduino](https://www.arduino.cc/) devices.

The example has been tested with the following devices:

- [Tiny Machine Learning Kit](https://store.arduino.cc/products/arduino-tiny-machine-learning-kit)

The Arduino Tiny Machine Learning Kit uses the OV7675 camera attachment. The OV7675 is currently not supported, and the code will simply generate a blank image (support coming _**soon**_). If you're using a different Arduino board and attaching your own
camera, you'll need to implement your own `arduino_image_provider.cpp` code. It also has a
set of LEDs, which are used to indicate whether a person has been recognized.

### Install the Arduino_TensorFlowLite library

This example application is included as part of the official TensorFlow Lite Micro
Arduino library.
To install the TensorFlow Lite Micro for Arduino library, see the
[how to install](../../README.md#how-to-install) instructions.

### Load and run the example

Once the library has been added, go to `File -> Examples`. You should see an
entry within the list named `Arduino_TensorFlowLite`. Select
it and click `person_detection` to load the example.

Use the Arduino IDE to build and upload the example. Once it is running, you
should see the built-in LED on your device flashing. The built-in LED will flash on/off for each inference cycle.  The green LED will be lit if a person is predicted, 
The blue LED will be lit if the prediction is not-a-person.

The program also outputs inference results to the serial port, which appear as
follows:

```
Cropping image and quantizing
Image cropped and quantized
Person score: 39.6% No person score: 60.93%
```

When the program is run, it waits several seconds for a USB-serial connection to be
available. If there is no connection available, it will not output data. To see
the serial output in the Arduino desktop IDE, do the following:

1. Open the Arduino IDE
1. Connect the Arduino board to your computer via USB
1. Press the reset button on the Arduino board
1. Within 5 seconds, go to `Tools -> Serial Monitor` in the Arduino IDE. You may
   have to try several times, since the board will take a moment to connect.

If you don't see any output, repeat the process again.

## Training your own model

You can train your own model with some easy-to-use scripts. See
[training_a_model.md](https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/micro/examples/person_detection/training_a_model.md) for instructions.

