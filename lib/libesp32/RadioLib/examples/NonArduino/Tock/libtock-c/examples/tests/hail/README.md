Hail Test App
=============

This application tests the basic functionality of a hail board.


What to Expect
--------------

When this app starts, the LED should begin blinking blue. When you press the
User Button, the green LED will turn on.

_Note:_ The blue LED continues blinking.  If you hold the User Button,
the LED will alternate green and turquoise.

### More data from serial

If you make a serial connection to the board, you will see something like this:

    $ tockloader listen
    No device name specified. Using default "tock"
    Using "/dev/cu.usbserial-c098e5130006 - Hail IoT Module - TockOS"
    Listening for serial output.
    ----------------------------
    [Hail] Test App!
    [Hail] Samples all sensors.
    [Hail] Transmits name over BLE.
    [Hail] Button controls LED.
    [Hail Sensor Reading]
      Temperature:  2382 1/100 degrees C
      Humidity:     4219 0.01%
      Light:        140
      Acceleration: 992
      A0:           1434 mV
      A1:           880 mV
      A2:           783 mV
      A3:           736 mV
      A4:           837 mV
      A5:           741 mV
      D0:           0
      D1:           0
      D6:           0
      D7:           0

### Experimenting with some of the sensors

  * The light sensor reports a value in the low hundreds for office lighting,
    nearly zero when covered with a hand, and several thousand when a light is
    shined directly at the board.

  * Exhaling on the temperature and humidity sensor should show a fairly quick
    rise in humidity. The temperature sensor will not change as significantly,
    but a warm finger can usually move it a few degrees.

  * The acceleration value is the Euclidean sum of all acceleration vectors.
    This means if you simply rotate the hail you won't see much change, however
    shaking the hail should result in some increased values. Check out
    [the accelerometer driver](../../../libtock/ninedof.h) for more
    fine-grained access to the accelerometer.
