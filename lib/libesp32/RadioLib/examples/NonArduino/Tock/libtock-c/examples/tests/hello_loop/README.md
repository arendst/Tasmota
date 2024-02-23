# Hello Loop Test App

Periodically prints "Hello\n" to the console. This is meant to help test
low-power operation when using the USART. On the SAM4L, with a board configured
to be able to enter deep sleep, power consumption should drop to microamps (in
WAIT mode) for a large majority of the time while the app continues to operate
normally.
