GPIO Loopback Test
=============

This app tests pin output and input by setting the value of one pin and reading
it on another.

Setup
-----

Place a jumper between GPIO pins 0 and 1 on the board.

> Note: these may not be explicitly labeled as GPIO 0 and 1 on the physical
> board. The pins you should use are the first two as specified by the board
> configuration in the Tock kernel.

Expected Output
---------------

```
******************************
GPIO Loopback Test Application
******************************

> Setup:
> Place a jumper between GPIO pin 0 and GPIO pin 1 on the board.

Checking loopback test pin 0 -> pin 1...SUCCESS
Checking loopback test pin 1 -> pin 0...SUCCESS
```
