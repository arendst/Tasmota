# CHANGELOG

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.1.0] - 2025-7-1

### Fixed

- Wrong computation of sensor variant in method getSensorVariant


## [1.0.0] - 2025-1-30

### Added

- All commands according to data sheet

### Changed

Breaking changes
- The file and class name has changed from SensirionI2CScd4x to SensirionI2cScd4x (the "c" in I2c is now in lower case)
- begin(TwoWire& i2cBus, uint8_t i2cAddress); now takes a second argument for the i2c address
- all methods that have been named xxxTicks are now named xxxRaw
- getDataReadyFlag has been renamed to getDataReadyStatus
- get/setAmbientPressure(uint32_t& aAmbientPressure); now takes the ambient pressure in Pa as uint32_t
- getSerialNumber now returns uint64_t


## [0.4.0] - 2023-03-06

### Added
- Methods for ASC (Automatic Self Calibration)

### Breaking changes
- Renamed getDataReadyStatus to getDataReadyFlag 

### Fixed
- RH/T conversion now uses correct constant: `2^16 - 1`
- Fixed a few comments
- Initializing the output variables to zero 

## [0.3.1] - 2021-04-30

### Changed

* Increase timing for single shot from 1350ms to 5000ms
* Increase timing for self test from 5500ms to 10000ms


## [0.3.0] - 2021-03-01

### Added
- Convenience interfaces taking care of unit conversion to and from ticks.

### Fixed
- wake-up interface handles missing ACK from sensor on wake up.


## [0.2.0] - 2021-02-10

### Changed

* Updated Sensirion Core library version from 0.4.0 to 0.4.2. This includes the
  renaming of the library header file from `SensirionCoreArduinoLibrary.h` to
  `SensirionCore.h`.
* Define `SCD4X_I2C_ADDRESS` as hex instead of dec (unchanged value).

### Added

* Added warning about limited EEPROM write cycles.

## [0.1.0] - 2021-02-05

Initial release


[Unreleased]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/1.0.0...HEAD
[1.0.0]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/0.4.0...1.0.0
[0.4.0]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/0.3.1...0.4.0
[0.3.1]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/0.3.0...0.3.1
[0.3.0]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/0.2.0...0.3.0
[0.2.0]: https://github.com/Sensirion/arduino-i2c-scd4x/compare/0.1.0...0.2.0
[0.1.0]: https://github.com/Sensirion/arduino-i2c-scd4x/releases/tag/0.1.0