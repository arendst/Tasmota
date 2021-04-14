# RELEASE NOTES

16 July 2019 - Apache NimBLE v1.2.0

For full release notes, please visit the
[Apache Mynewt Wiki](https://cwiki.apache.org/confluence/display/MYNEWT/Release+Notes).

Apache NimBLE is an open-source Bluetooth 5.0 stack (both Host & Controller) that completely
replaces the proprietary SoftDevice on Nordic chipsets.

New features in this version of NimBLE include:

* Perdiodic Advertising support with up to 1650 bytes of data (scanner and advertiser)
* Support for scan request notification in GAP API
* Updated host qualification ID
* Qualification related bugfixes
* GAP API doxygen documentation update
* BLE Mesh improvements - fixes and resync with latest Zephyr code
* RIOT OS port fixes and improvements
* btshell sample application improvements
* improvements for bttester application
* Controller duplicates filtering improvements
* Memory and CPU usage optimizations in controller

If working on next-generation RTOS and Bluetooth protocol stack
sounds exciting to you, get in touch, by sending a mail to the Apache Mynewt
Developer's list, dev@mynewt.apache.org.
