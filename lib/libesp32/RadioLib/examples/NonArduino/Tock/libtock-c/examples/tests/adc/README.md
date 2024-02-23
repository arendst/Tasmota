ADC Test App
============

Demonstrates synchronous analog sampling in Tock. Checks that the ADC driver
exists on the platform, and then iterates through each ADC channel. For each,
it first takes several single analog samples. Next it requests a buffer of 16
samples at 10 different frequencies from 25 Hz to 175000 Hz. Sample data
converted to millivolts is printed to console. If the analog channels are not
connected to any particular voltage, their values will vary.

Since the ADC currently is not virtualized, results are undefined if multiple 
applications (such as this and the `hail` app) attempt to use the ADC at once. 
All applications on the system can be erased with `tockloader erase-apps`.


Example Output
--------------

```
[Tock] ADC Test
ADC driver exists with 6 channels

Single Samples - Channel 0
ADC Reading: 2166 mV (raw: 0x0a80)
ADC Reading: 2167 mV (raw: 0x0a82)
ADC Reading: 2165 mV (raw: 0x0a7f)
ADC Reading: 2170 mV (raw: 0x0a85)
ADC Reading: 2167 mV (raw: 0x0a82)
ADC Reading: 2168 mV (raw: 0x0a83)
ADC Reading: 2169 mV (raw: 0x0a84)
ADC Reading: 2168 mV (raw: 0x0a83)
ADC Reading: 2168 mV (raw: 0x0a83)
ADC Reading: 2166 mV (raw: 0x0a80)

Buffered Samples - Channel 0
16 ADC samples at 25 Hz
	[ 2168 2166 2167 2167 2168 2164 2167 2181 2166 2169 2165 2170 2167 2166 2167 2166 ]
16 ADC samples at 100 Hz
	[ 2166 2168 2167 2169 2168 2168 2166 2167 2166 2170 2169 2171 2167 2168 2168 2168 ]
16 ADC samples at 500 Hz
	[ 2166 2164 2167 2167 2166 2164 2170 2168 2170 2167 2166 2168 2165 2165 2167 2167 ]
16 ADC samples at 1000 Hz
	[ 2170 2170 2168 2169 2164 2167 2167 2168 2172 2166 2169 2167 2167 2170 2171 2166 ]
16 ADC samples at 5000 Hz
	[ 2167 2167 2167 2169 2166 2166 2166 2166 2169 2167 2166 2166 2166 2168 2168 2163 ]
16 ADC samples at 10000 Hz
	[ 2167 2163 2167 2165 2169 2164 2166 2168 2168 2169 2170 2165 2169 2167 2167 2169 ]
16 ADC samples at 44100 Hz
	[ 2165 2170 2169 2168 2167 2171 2168 2169 2166 2169 2166 2165 2166 2166 2169 2169 ]
16 ADC samples at 100000 Hz
	[ 2166 2165 2162 2166 2166 2164 2167 2167 2165 2167 2166 2165 2166 2166 2168 2164 ]
16 ADC samples at 150000 Hz
	[ 2167 2167 2168 2168 2165 2168 2167 2166 2170 2169 2168 2164 2162 2166 2157 2167 ]
16 ADC samples at 175000 Hz
	[ 2168 2174 2169 2169 2166 2167 2164 2164 2165 2165 2165 2167 2167 2167 2165 2168 ]
```

