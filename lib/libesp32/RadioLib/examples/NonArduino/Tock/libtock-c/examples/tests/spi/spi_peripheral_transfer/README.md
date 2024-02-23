SPI Peripheral Transfer Test
========================
This test passes one of two buffers over the SPI bus; either an all-zero
buffer, or a buffer with sequentially increasing integers. This test assumes
that the transmitter (the attached Spi Master) first sends a buffer with
sequentially increasing integers, before alternating with the zero buffer.

Note that this test is intended to be used in tandem with the SPI Controller
Transfer Test. To use this test, connect two boards together, using one as an
SPI controller and one as a SPI peripheral, and load the SPI Controller 
or Peripheral Test code. On the peripheral board, every time a transfer is 
received, the received buffer is compared with the expected buffer, and a 
new buffer is queued. On failure (when an unexpected buffer is received), 
the LED is turned on.
