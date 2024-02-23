UDP Send App
=============

An testing app for platforms with an 802.15.4 radio that transmits
UDP packets to a single end node over the network. Currently, it sends UDP packets
using 6lowpan to a single neighbor with an IP address known ahead of time.
As Imix is currently the only board with tested 15.4 support, this test is
intended to be run on Imix. If this app is run on a platform without 15.4
support, it will simply print that 15.4 is not supported for that platform.

## Running

This application should be tested using the associated `udp_rx` app. Flash one
Imix with this application, and flash a second Imix with `udp_rx`. If both
apps are working correctly, the second Imix will blink its user LED every
time an app is received, and will print the payload of each received packet
to the console. If you wish to use this app to send to a board with a different
address than the one hardcoded in this application, change the destination IPv6 address
to match the source IPv6 address of your intended receiver. Note that in order
to send to a different receiver you must also change the destination MAC address,
which is currently configured in the kernel (in `boards/imix/src/main.rs`)
