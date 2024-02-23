ACIFC Test App
============

Demonstrates the use of an analog comparator in Tock. Checks that the analog comparator driver exists on the platform, and does a polling or an interrupt-based comparison depending on the `mode` variable set by the user.

Example Output
--------------

```
Analog Comparator test application
Analog Comparator driver exists with 2 channels
Interrupt received on channel 1, Vinp > Vinn!
Interrupt received on channel 1, Vinp > Vinn!
Interrupt received on channel 0, Vinp > Vinn!
```
