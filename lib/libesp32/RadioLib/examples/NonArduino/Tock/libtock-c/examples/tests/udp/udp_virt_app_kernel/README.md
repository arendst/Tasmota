UDP App / Kernel Virtualization Test
=============

An testing app for Imix that verifies that
virtulizing UDP between capsules and apps simultaneously works as expected;
i.e. that capsules and apps cannot bind to the same ports, and that simultaneous
sending from apps and capsules works as expected.

## Running

This application should be tested using the associated in-kernel test,
`udp_lowpan_test.rs`, which is an Imix component. This application
should be loaded onto a version of the kernel with this test enabled.
At startup, the app and kernel test will both print messages indicating whether
the test has been successful. A panic in the kernel indicates a failure in the kernel
portion of the test, an assert failure in the app indicates a failure in the app
portion of the test. Further description of this test is found in `udp_lowpan_test.rs`.
