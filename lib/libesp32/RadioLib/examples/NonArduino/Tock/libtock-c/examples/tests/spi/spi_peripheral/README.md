SPI Peripheral Test
==============
This test echoes a buffer over the Spi bus, initially sending a buffer of
sequentially increasing integers, then continuing to echo the SPI controller.
This test also toggles the LED when the chip selected callback is issued 
(which occurs when the peripheral is selected by the controller).
