# RELEASE NOTES

18 March 2020 - Apache NimBLE v1.3.0

For full release notes, please visit the
[Apache Mynewt Wiki](https://cwiki.apache.org/confluence/display/MYNEWT/Release+Notes).

Apache NimBLE is an open-source Bluetooth 5.1 stack (both Host & Controller) that completely
replaces the proprietary SoftDevice on Nordic chipsets.

New features in this version of NimBLE include:

* Support for Bluetooth Core Specification 5.1
* New blestress test application
* Dialog DA1469x CMAC driver
* Support for LE Secure Connections out-of-band (OOB) association model
* Support for automated generation of syscfg for ports
* Qualification related bugfixes
* BLE Mesh improvements - fixes and resync with latest Zephyr code
* RIOT OS port fixes and improvements
* btshell sample application improvements
* improvements for bttester application
* Controller duplicates filtering improvements
* Multi PHY support improvements
* Memory and CPU usage optimizations
* Use of packed structs for HCI (code size reduction)
* Linux sample improvements
* PTS test instructions updates
* Clock managements improvements in controller

If working on next-generation RTOS and Bluetooth protocol stack
sounds exciting to you, get in touch, by sending a mail to the Apache Mynewt
Developer's list, dev@mynewt.apache.org.
