I2C Master/Slave Ping Pong
==========================

This test sends a message between two multi-master I2C devices.

To test, program two devices with this application and press the user
button on one device. A device powers on its LED when sending a message
and powers off its LED when it receives a message.


Test Operation
--------------

At startup, each device will act as an I2C slave with address 0x40.

Upon button press, a device will change its address to 0x41, change to
master mode, and issue a write to device 0x40. Once complete, the device
will return to slave mode, now with address 0x41.

In steady state operation, when a device receives a message, it will
send a message in response 2500 ms later.
