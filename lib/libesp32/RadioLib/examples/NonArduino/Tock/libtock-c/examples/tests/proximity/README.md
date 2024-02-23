Proximity Sensor Test App
=============

This app tests the complete hil interface (all driver functions) for the proximity sensor.
This interface is called by `proximity_read_on_interrupt_sync()` and `proximity_read_sync()` synchronous
syscall wrapper functions implemented in libtock/proximity.c.

Hardware Requirements (your board must include):
    - a proximity sensor (ex: apds9960 found on the nano33ble)

Kernel Requirements:
    - the proximity sensor on the board must have an associated capsule driver
    - this capsule driver must implement the `kernel::hil::sensors::ProximityDriver` driver interface


Example Shell Output:
---------------------

Initialization complete. Entering main loop.
Proximity Sensor Test
Driver exists
Number of LEDs on the board: 3

Examples Visual Output:
----------------------

Bring your finger/object very close to the proximity sensor and the LED's should start blinking in sequential order.
This tests the `proximity_read_on_interrupt_sync()`
Now as you move the object farther away from the sensor, the LEDs should start blinking at a slower rate.
This tests the `proximity_read_sync()`





