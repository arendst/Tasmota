# TasmotaSerial

Implementation of software serial library for the ESP8266 at 9600 baud

Allows for several instances to be active at the same time.

Please note that due to the fact that the ESP always have other activities ongoing, there will be some inexactness in interrupt
timings. This may lead to bit errors when having heavy data traffic.
