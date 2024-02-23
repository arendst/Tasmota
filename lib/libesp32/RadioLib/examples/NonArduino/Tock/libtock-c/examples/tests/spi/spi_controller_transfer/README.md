Spi  Controller Transfer Test
========================
This test passes one of two buffers over the SPI bus; either an all-zero
buffer, or a buffer with sequentially increasing integers. This test assumes
that the receiver (the attached SPI peripheral) echoes the buffers back.
On the first transfer, the SPI controller sends a buffer with sequentially
increasing integers, and expects a buffer of all zeroes. On the next call to
'spi_read_write', the controller sends a buffer of all zeroes, and expects a 
buffer of sequentially increasing integers.

Note that this test is intended to be used in tandem with the SPI peripheral 
transfer test. To use this test, connect two boards together, using one as 
an SPI controller and one as a SPI peripheral, and load the test applications.
On the controller board, every time the user button is pressed, a buffer is 
sent. On failure (when an unexpected buffer is received), the LED is turned on.
