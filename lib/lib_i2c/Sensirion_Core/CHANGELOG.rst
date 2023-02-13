Changelog
=========

All notable changes to this project will be documented in this file.

The format is based on `Keep a Changelog <https://keepachangelog.com/en/1.0.0/>`_
and this project adheres to `Semantic Versioning <https://semver.org/spec/v2.0.0.html>`_.

`Unreleased`_
-------------


`0.6.0`_ 2022-06-22
-------------------

- Fix compiler warnings in SensirionErrors.cpp
- Allow drivers to choose CRC function

`0.5.3`_ 2021-10-19
-------------------

- Add support for sensor specific errors
- Update keywords.txt


`0.5.2`_ 2021-08-03
-------------------

Fixed
.....

- Fix CRC insertion in ``SensirionI2CTxFrame`` when more then one parameter
  is sent to the sensor.

`0.5.1`_ 2021-07-08
-------------------

Changed
.......

- Adjusted deprecation warnings

`0.5.0`_ 2021-07-07
-------------------

Added
.....

- Enable SensirionTxFrame to incorporate Uint8 and Uint16 commands


`0.4.3`_ 2021-02-12
-------------------

Added
.....

- Added ``const`` modifier to functions which process MOSI array data.

`0.4.2`_ 2021-01-29
-------------------

Changed
.......

- Renamed the library header from ``SensirionCoreArduinoLibrary.h`` to ``SensirionCore.h``.
  We keep the old header for legacy support.

`0.4.1`_ 2021-01-28
-------------------

Fixed
.....

- Properly handle I2C write errors


`0.4.0`_ 2021-01-20
-------------------

Added
.....

- Documentation for all functions.

Breaking
........

- Change interface of ``errorToString()`` function to include length of the
  provided buffer.

Removed
.......

- Removed ``reset()`` function from ``SensirionI2CTxFrame`` since the
  functionality is not needed.


`0.3.0`_ 2021-01-13
-------------------

Added
.....

- Core implementation for I2C communication. This includes a RX and TX frame
  and a I2C communication class.

Changed
.......

- SHDLC and I2C RX frame inherit from a RX frame base class.
- ESP8266 test board from esp8266:esp8266:arduino to esp8266:esp8266:generic.
- Sorted errors into general, SHDLC and I2C errors.
- Replace C style casts with ``static_cast``.


`0.2.0`_ 2021-01-11
-------------------

Added
.....

- Explanation what SHDLC is in README.
- ``SensirionErrors.h`` to ``SensirionCoreArduinoLibrary.h``.
- ``sendAndReceiveFrame()`` function to ``SensirionShdlcCommunication``. This
  function combines ``sendFrame()`` and ``receiveFrame()`` into one function and
  adds additional error checking.

Changed
.......

- Rename DeviceError to ExecutionError.
- Move check for execution error after the whole frame is read and checksum is
  checked. This prevents that a wrong checksum can't be displayed as an
  execution error.

Removed
.......

- ``reset()`` function from ``SensirionShdlcTxFrame`` and ``SensirionShdlcRxFrame``,
  since one can just create a new frame object which has the same effect.

`0.1.0`_ 2021-01-07
-------------------

- Initial release


.. _Unreleased: https://github.com/Sensirion/arduino-core/compare/0.6.0...main
.. _0.6.0: https://github.com/Sensirion/arduino-core/compare/0.6.0...0.5.3
.. _0.5.3: https://github.com/Sensirion/arduino-core/compare/0.5.2...0.5.3
.. _0.5.2: https://github.com/Sensirion/arduino-core/compare/0.5.1...0.5.2
.. _0.5.1: https://github.com/Sensirion/arduino-core/compare/0.5.0...0.5.1
.. _0.5.0: https://github.com/Sensirion/arduino-core/compare/0.4.3...0.5.0
.. _0.4.3: https://github.com/Sensirion/arduino-core/compare/0.4.2...0.4.3
.. _0.4.2: https://github.com/Sensirion/arduino-core/compare/0.4.1...0.4.2
.. _0.4.1: https://github.com/Sensirion/arduino-core/compare/0.4.0...0.4.1
.. _0.4.0: https://github.com/Sensirion/arduino-core/compare/0.3.0...0.4.0
.. _0.3.0: https://github.com/Sensirion/arduino-core/compare/0.2.0...0.3.0
.. _0.2.0: https://github.com/Sensirion/arduino-core/compare/0.1.0...0.2.0
.. _0.1.0: https://github.com/Sensirion/arduino-core/releases/tag/0.1.0
