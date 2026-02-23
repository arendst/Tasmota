# CHANGELOG

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] 

## [1.2.0] - 2025-3-13

### Added

- get version command to read the firmware version
- get SHT heater measurement to read humidity and temperature measurement after heater activation. This command is available for firmware version >= 4.0.
### Changed

- Activate SHT heater duration reduced to 20ms. For firmware version >= 4.0 the get heater measurements command can be used to check if the heater has finished. With firmware versions < 4.0 one must wait at least 1300ms before issuing another command.
## [1.1.0] - 2025-2-12

### Changed

- Serial number and product name interpreted as string (changed from uint8[32] to int8[32])
## [1.0.1] - 2024-12-1

### Fixed

- Fix naming from mass concentration to number concentration for read number concentration method (returned values were correct).
## [1.0.0] - 2024-11-25

### Added

- Add all public interfaces
### Changed

- Return type of CO2 value in read_measured_values changed from float to uint16
## [0.1.0] - 2024-10-31

### Added

- Add product picture
- Add interfaces to start, stop and read measurements.
- Add interfaces to read product name, serial number and version

[Unreleased]: https://github.com/Sensirion/arduino-i2c-sen66/compare/1.2.0...HEAD
[1.2.0]: https://github.com/Sensirion/arduino-i2c-sen66/compare/1.1.0...1.2.0
[1.1.0]: https://github.com/Sensirion/arduino-i2c-sen66/compare/1.0.1...1.1.0
[1.0.1]: https://github.com/Sensirion/arduino-i2c-sen66/compare/1.0.0...1.0.1
[1.0.0]: https://github.com/Sensirion/arduino-i2c-sen66/compare/0.1.0...1.0.0
[0.1.0]: https://github.com/Sensirion/arduino-i2c-sen66/releases/tag/0.1.0