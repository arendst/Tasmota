UDP Button Press App
====================

Your client needs to keep track of sporadic events and record when those
events occur by pressing the 'user' button on the Imix.

This app is for platforms with an 802.15.4 radio.  It broadcasts button presses
over the network, so they can be recorded at a central server.
Currently, this app sends UDP packets using 6lowpan to a single neighbor with
an IP address known ahead of time.

## Running

To run this app, simply place the IP address of the destination node in the `dst_addr` struct.
Notably, until Tock has neighbor discovery implemented, you also have to configure
the destination MAC address in the kernel (in `boards/imix/src/main.rs`).
