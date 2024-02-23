GPIO Async Test App
===================

This is a basic test application for asynchronous GPIO support. The
`async_gpio` interface mirrors the `gpio` interface, but is designed for
uses where there is an asynchronous request needed to control the GPIO, as would
be common with an I2C GPIO extender.

Example Output
--------------

```
GPIO Async Test App
Enabling rising edge interrupt on port 0 pin 1
Toggling port 0 pin 0
INTERRUPT
INTERRUPT
INTERRUPT
INTERRUPT
INTERRUPT
INTERRUPT
Now disabling interrupt
```
