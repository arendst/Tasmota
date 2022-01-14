# Changelog

All notable changes to this project will be documented in this file.

## [1.3.5] - 2022-01-14

### Added
- CONFIG_NIMBLE_CPP_DEBUG_LEVEL macro in nimconfig.h to allow setting the log level separately from the Arduino core log level.

### Fixed
- Memory leak when initializing/deinitializing the BLE stack caused by new FreeRTOS timers be created on each initialization.

## [1.3.4] - 2022-01-09

### Fixed
- Workaround for latest Arduino-esp32 core that causes tasks not to block when required, which caused functions to return prematurely resulting in exceptions/crashing.
- The wrong length value was being used to set the values read from peer attributes. This has been corrected to use the proper value size.

## [1.3.3] - 2021-11-24

### Fixed
- Workaround added for FreeRTOS bug that affected timers, causing scan and advertising timer expirations to not correctly trigger callbacks.

## [1.3.2] - 2021-11-20

### Fixed
- Added missing macros for scan filter.

### Added
- `NimBLEClient::getLastError` : Gets the error code of the last function call that produces a return code from the stack.

## [1.3.1] - 2021-08-04

### Fixed
- Corrected a compiler/linker error when an application or a library uses bluetooth classic due to the redefinition of `btInUse`.

## [1.3.0] - 2021-08-02

### Added
- `NimBLECharacteristic::removeDescriptor`: Dynamically remove a descriptor from a characterisic. Takes effect after all connections are closed and sends a service changed indication.
- `NimBLEService::removeCharacteristic`: Dynamically remove a characteristic from a service. Takes effect after all connections are closed and sends a service changed indication
- `NimBLEServerCallbacks::onMTUChange`: This is callback is called when the MTU is updated after connection with a client.
- ESP32C3 support

- Whitelist API:
  - `NimBLEDevice::whiteListAdd`: Add a device to the whitelist.
  - `NimBLEDevice::whiteListRemove`: Remove a device from the whitelist.
  - `NimBLEDevice::onWhiteList`: Check if the device is on the whitelist.
  - `NimBLEDevice::getWhiteListCount`: Gets the size of the whitelist
  - `NimBLEDevice::getWhiteListAddress`: Get the address of a device on the whitelist by index value.

- Bond management API:
  - `NimBLEDevice::getNumBonds`: Gets the number of bonds stored.
  - `NimBLEDevice::isBonded`: Checks if the device is bonded.
  - `NimBLEDevice::deleteAllBonds`: Deletes all bonds.
  - `NimBLEDevice::getBondedAddress`: Gets the address of a bonded device by the index value.

- `NimBLECharacteristic::getCallbacks` to retrieve the current callback handler.
- Connection Information class: `NimBLEConnInfo`.
- `NimBLEScan::clearDuplicateCache`: This can be used to reset the cache of advertised devices so they will be immediately discovered again.

### Changed
- FreeRTOS files have been removed as they are not used by the library.
- Services, characteristics and descriptors can now be created statically and added after.
- Excess logging and some asserts removed.
- Use ESP_LOGx macros to enable using local log level filtering.

### Fixed
- `NimBLECharacteristicCallbacks::onSubscribe` Is now called after the connection is added to the vector.
- Corrected bonding failure when reinitializing the BLE stack.
- Writing to a characterisic with a std::string value now correctly writes values with null characters.
- Retrieving remote descriptors now uses the characterisic end handle correctly.
- Missing data in long writes to remote descriptors.
- Hanging on task notification when sending an indication from the characteristic callback.
- BLE controller memory could be released when using Arduino as a component.
- Complile errors with NimBLE release 1.3.0.

## [1.2.0] - 2021-02-08

### Added
- `NimBLECharacteristic::getDescriptorByHandle`: Return the BLE Descriptor for the given handle.

- `NimBLEDescriptor::getStringValue`: Get the value of this descriptor as a string.

- `NimBLEServer::getServiceByHandle`: Get a service by its handle.

- `NimBLEService::getCharacteristicByHandle`: Get a pointer to the characteristic object with the specified handle.

- `NimBLEService::getCharacteristics`: Get the vector containing pointers to each characteristic associated with this service.  
Overloads to get a vector containing pointers to all the characteristics in a service with the UUID. (supports multiple same UUID's in a service)
  - `NimBLEService::getCharacteristics(const char *uuid)`
  - `NimBLEService::getCharacteristics(const NimBLEUUID &uuid)`

- `NimBLEAdvertisementData` New methods:
  - `NimBLEAdvertisementData::addTxPower`: Adds transmission power to the advertisement.
  - `NimBLEAdvertisementData::setPreferredParams`: Adds connection parameters to the advertisement.
  - `NimBLEAdvertisementData::setURI`: Adds URI data to the advertisement.

- `NimBLEAdvertising` New methods:
  - `NimBLEAdvertising::setName`: Set the name advertised.
  - `NimBLEAdvertising::setManufacturerData`: Adds manufacturer data to the advertisement.
  - `NimBLEAdvertising::setURI`: Adds URI data to the advertisement.
  - `NimBLEAdvertising::setServiceData`: Adds service data to the advertisement.
  - `NimBLEAdvertising::addTxPower`: Adds transmission power to the advertisement.
  - `NimBLEAdvertising::reset`: Stops the current advertising and resets the advertising data to the default values.

- `NimBLEDevice::setScanFilterMode`: Set the controller duplicate filter mode for filtering scanned devices.

- `NimBLEDevice::setScanDuplicateCacheSize`: Sets the number of advertisements filtered before the cache is reset.

- `NimBLEScan::setMaxResults`:  This allows for setting a maximum number of advertised devices stored in the results vector.

- `NimBLEAdvertisedDevice` New data retrieval methods added:
  - `haveAdvInterval/getAdvInterval`: checks if the interval is advertised / gets the advertisement interval value.

  - `haveConnParams/getMinInterval/getMaxInterval`: checks if the parameters are advertised / get min value / get max value.

  - `haveURI/getURI`: checks if a URI is advertised / gets the URI data.

  - `haveTargetAddress/getTargetAddressCount/getTargetAddress(index)`: checks if a target address is present / gets a count of the addresses targeted / gets the address of the target at index.

### Changed
- `nimconfig.h` (Arduino) is now easier to use.

- `NimBLEServer::getServiceByUUID` Now takes an extra parameter of instanceID to support multiple services with the same UUID.

- `NimBLEService::getCharacteristic` Now takes an extra parameter of instanceID to support multiple characteristics with the same UUID.

- `NimBLEAdvertising` Transmission power is no longer advertised by default and can be added to the advertisement by calling `NimBLEAdvertising::addTxPower`

- `NimBLEAdvertising` Custom scan response data can now be used without custom advertisment.

- `NimBLEScan` Now uses the controller duplicate filter.  

- `NimBLEAdvertisedDevice` Has been refactored to store the complete advertisement payload and no longer parses the data from each advertisement.  
Instead the data will be parsed on-demand when the user application asks for specific data.  

### Fixed
- `NimBLEHIDDevice` Characteristics now use encryption, this resolves an issue with communicating with devices requiring encryption for HID devices.


## [1.1.0] - 2021-01-20

### Added
- `NimBLEDevice::setOwnAddrType` added to enable the use of random and random-resolvable addresses, by asukiaaa  

- New examples for securing and authenticating client/server connections, by mblasee.  

- `NimBLEAdvertising::SetMinPreferred` and `NimBLEAdvertising::SetMinPreferred` re-added.  

- Conditional checks added for command line config options in `nimconfig.h` to support custom configuration in platformio.  

- `NimBLEClient::setValue` Now takes an extra bool parameter `response` to enable the use of write with response (default = false).  

- `NimBLEClient::getCharacteristic(uint16_t handle)` Enabling the use of the characteristic handle to be used to find 
the NimBLERemoteCharacteristic object.  

- `NimBLEHIDDevice` class added by wakwak-koba.  

- `NimBLEServerCallbacks::onDisconnect` overloaded callback added to provide a ble_gap_conn_desc parameter for the application  
to obtain information about the disconnected client.  

- Conditional checks in `nimconfig.h` for command line defined macros to support platformio config settings.  

### Changed
- `NimBLEAdvertising::start` now returns a bool value to indicate success/failure.  

- Some asserts were removed in `NimBLEAdvertising::start` and replaced with better return code handling and logging.    

- If a host reset event occurs, scanning and advertising will now only be restarted if their previous duration was indefinite.  

- `NimBLERemoteCharacteristic::subscribe` and `NimBLERemoteCharacteristic::registerForNotify` will now set the callback
regardless of the existance of the CCCD and return true unless the descriptor write operation failed.  

- Advertising tx power level is now sent in the advertisement packet instead of scan response.  

- `NimBLEScan` When the scan ends the scan stopped flag is now set before calling the scan complete callback (if used)  
this allows the starting of a new scan from the callback function.  

### Fixed
- Sometimes `NimBLEClient::connect` would hang on the task block if no event arrived to unblock.  
A time limit has been added to timeout appropriately.  

- When getting descriptors for a characterisic the end handle of the service was used as a proxy for the characteristic end  
handle. This would be rejected by some devices and has been changed to use the next characteristic handle as the end when possible.  

- An exception could occur when deleting a client instance if a notification arrived while the attribute vectors were being  
deleted. A flag has been added to prevent this.  
  
- An exception could occur after a host reset event when the host re-synced if the tasks that were stopped during the event did  
not finish processing. A yield has been added after re-syncing to allow tasks to finish before proceeding.  
  
- Occasionally the controller would fail to send a disconnected event causing the client to indicate it is connected  
and would be unable to reconnect. A timer has been added to reset the host/controller if it expires.  
  
- Occasionally the call to start scanning would get stuck in a loop on BLE_HS_EBUSY, this loop has been removed.  

- 16bit and 32bit UUID's in some cases were not discovered or compared correctly if the device  
advertised them as 16/32bit but resolved them to 128bits. Both are now checked.  
  
- `FreeRTOS` compile errors resolved in latest Ardruino core and IDF v3.3.  

- Multiple instances of `time()` called inside critical sections caused sporadic crashes, these have been moved out of critical regions.  

- Advertisement type now correctly set when using non-connectable (advertiser only) mode.  

- Advertising payload length correction, now accounts for appearance.  

- (Arduino) Ensure controller mode is set to BLE Only.  


## [1.0.2] - 2020-09-13

### Changed

- `NimBLEAdvertising::start` Now takes 2 optional parameters, the first is the duration to advertise for (in seconds), the second is a  
callback that is invoked when advertsing ends and takes a pointer to a `NimBLEAdvertising` object (similar to the `NimBLEScan::start` API).

- (Arduino) Maximum BLE connections can now be altered by only changing the value of `CONFIG_BT_NIMBLE_MAX_CONNECTIONS` in `nimconfig.h`.
Any changes to the controller max connection settings in `sdkconfig.h` will now have no effect when using this library.

- (Arduino) Revert the previous change to fix the advertising start delay. Instead a replacement fix that routes all BLE controller commands from  
a task running on core 0 (same as the controller) has been implemented. This improves response times and reliability for all BLE functions.


## [1.0.1] - 2020-09-02

### Added

- Empty `NimBLEAddress` constructor: `NimBLEAddress()` produces an address of 00:00:00:00:00:00 type 0.
- Documentation of the difference of NimBLEAddress::getNative vs the original bluedroid library.

### Changed

- notify_callback typedef is now defined as std::function to enable the use of std::bind to call a class member function.

### Fixed

- Fix advertising start delay when first called.


## [1.0.0] - 2020-08-22

First stable release.

All the original library functionality is complete and many extras added with full documentation.
