imix Test App
=============

This application tests the basic functionality of an imix board.


What to Expect
--------------

When this app starts, the "user" LED should begin blinking. When you press the
User Button, the "kernel" LED will turn on.

### More data from serial

If you make a serial connection to the board, you will see something like this:

    $ tockloader listen
    No device name specified. Using default "tock"
    Using "/dev/ttyUSB0 - imix development board - TockOS"
                       
    Listening for serial output.
    [imix] Test App!                    
    [imix] Samples all sensors.
    [imix] Transmits name over BLE.
    [imix] Button controls LED.
    [imix Sensor Reading]  
    Temperature:  2718 1/100 degrees C
    Humidity:     3723 0.01%
    Light:        593    
    Acceleration: 1007   
    A0:           2074 mV
    A1:           1812 mV
    A2:           1947 mV             
    A3:           1965 mV   
    A4:           1948 mV
    A5:           4 mV
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
    This means if you simply rotate the imix you won't see much change, however
    shaking the imix should result in some increased values. Check out
    [the accelerometer driver](../../../libtock/ninedof.h) for more
    fine-grained access to the accelerometer.
