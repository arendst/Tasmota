I2C Master/Slave Ping Pong
==========================

This test sends a message from an I2C master to an I2C slave.

To test, program one device with this application and another
with the `i2c_master_slave_ping_pong` applcation and press the user
button on the master device. It will send a message to the slave device
and print out information on whether the send was successful. By
inspecting the slave you can see if the write was received.


