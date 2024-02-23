Console Receive Long
=======================

Receive 61 bytes from the console and display them.

When this test was written 64 is the max buffer size used in the kernel, and a
longer read would fail. Also, 64 is too easy of a number, so we choose an odd
size that is close to better test the kernel.
