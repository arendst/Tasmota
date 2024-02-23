ADC Continuous Test App
=======================

Demonstrates asynchronous continuous collection of analog samples in Tock.
Manually provides buffers and callbacks to the ADC driver and then begins
continuously sampling at 44.1 kHz. The provided buffers are 4410 Bytes in
length, so a callback occurs every 100 milliseconds.

On each callback, the samples are converted to millivolts and statistics about
the data are collected (min, max, and average). Results are printed to the
console.

Every 100 samples, the ADC is stopped and the values of an entire buffer are
printed to the console synchronously. Once the printing is complete, the ADC is
started again.

Note that the ADC is not virtualized and can currently only be used by a single
application. Results are undefined if multiple applications (such as this and
the `hail` app) attempt to use the ADC at once. All applications on the system
can be erased with `tockloader erase-apps`.

Example Output
--------------

```
[Tock] ADC Continuous Test
ADC driver exists with 6 channels
Beginning continuous sampling on channel 0 at 44100 Hz
Channel: 0	Count: 4410	Avg: 2167	Min: 2119	Max: 2218
Channel: 0	Count: 4410	Avg: 2167	Min: 2114	Max: 2210
Channel: 0	Count: 4410	Avg: 2167	Min: 2121	Max: 2214
Channel: 0	Count: 4410	Avg: 2167	Min: 2120	Max: 2211
Channel: 0	Count: 4410	Avg: 2167	Min: 2116	Max: 2209
```

