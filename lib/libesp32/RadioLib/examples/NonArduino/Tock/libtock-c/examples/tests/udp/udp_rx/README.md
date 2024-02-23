UDP Rx App
=============

Test to receive UDP packets. It should be run alongside udp_send. Full instructions
for this test can be found in the README for the udp_send test app.

Notably, the ability to set the destination MAC address from userspace requires
that the ieee802154 driver be present *in addition to* the UDP driver. Because some boards
(like Imix) do not include both, you need to set the destination MAC address
in the kernel of the sender (or set the SRC in the kernel of the receiver)
for this app to work correctly.

