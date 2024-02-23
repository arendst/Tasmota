IPv6 Sensor App
=============

An example app for platforms with sensors and an 802.15.4 radio that broadcasts
periodic sensor readings over the network. Currently, it sends UDP packets
using 6lowpan to a single neighbor with an IP address known ahead of time.

## Running

This application should be tested using the `udp_rx` app in `examples/tests/udp/udp_rx`.
Flash one Imix with this application, and flash a second Imix with `udp_rx`. If both
apps are working correctly, the second Imix will blink its user LED every
time an app is received, and will print the payload of each received packet
to the console. If you wish to use this app to send to a board with a different
address than the one hardcoded in this application, change the destination IPv6 address
to match the source IPv6 address of your intended receiver. Note that in order
to send to a different receiver you must also change the destination MAC address,
which is currently configured in the kernel (in `boards/imix/src/main.rs`)
