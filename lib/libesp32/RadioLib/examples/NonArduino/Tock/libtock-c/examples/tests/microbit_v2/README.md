Micro:bit v2 Test App
=====================

This app tests the sensors of the Micro:bit v2.

On startup
----------

When this app starts, the top left LED should start blinking. When you
press the A button, the LEDs should display an A, and a note should play. The 
B button behaves similarly, but plays a different note. 

### More data printed to console

If you run the command "tockloader listen" while this app is running, you will
see a printout like this: 

    $ tockloader listen
    No device name specified. Using default "tock"
    Using "/dev/ttyACM0 - BBC Micro:bit CMSIS-DAP - mbed Serial Port"
    Listening for serial output.
    ----------------------------
    [Micro:bit] Test App!
    [Micro:bit] Samples all sensors.
    [Micro:bit] Transmits name over BLE.
    [Micro:bit] Button controls LED.
    [Micro:bit Sensor Reading]
      Temperature:  23.0 degrees C
      Acceleration: 992
      Magnetometer: X: 3024, Y: 269, Z: -1048
      Ambient sound: 67
    ADC:
      P0:           1434 mV
      P1:           880 mV
      P2:           783 mV
    Digital:
      P8:           0
      P9:           0
      p16:          0

### Experimenting with some of the sensors

  * The temperature sensor will not change significantly,
    but a warm finger can usually move it a few degrees.

  * The acceleration should be around 900-1100 when the Micro:bit isn't moving. 
    if you shake the Micro:bit, it should jump to the high 2000s.
   
  * When the Micro:bit is close to an electronic device, the X, Y, and Z values
    from the magnetometer should get bigger.
  
  * The ambient sound should be around 70 in a quiet room. Playing music loudly
    next to the Micro:bit should raise it to 80 or 90.
  
  * Connecting P0, P1, or P2 with the GND pin should drop the corresponding pin's mV
    to zero or almost zero. Additionally, the bottom-left LED should turn on.
  
  * Pressing the A button and B button should make the Micro:bit play a
    different note for each button and display a letter (A or B).
