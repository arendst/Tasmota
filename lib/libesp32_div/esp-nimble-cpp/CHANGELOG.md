# Changelog
All notable changes to this project will be documented in this file.

## [2.3.0] 2025-05-19

## Fixed
- Incorrect `NimBLECharacteristic::onSubscribe` value when indications are set.
- `NimBLECharacteristic::onRead` callback not called in some cases.
- Clear attribute value when zero length value is written.
- Notify/Indicate incorrectly returning success with custom value.
- Corrected NimBLEClient array initialization.
- Prevent potential exception when scan is restarted.
- Attribute getValue failing with some data types
- Incorrectly passing a pointer to a function taking const reference.

## Added
- Support for esp32c5
- L2CAP infrastructure.
- Scan duplicate cache reset time.

## Changed
- Cleaned up examples.
- Allow PHY updates without enabling extended advertising.

## [2.2.1] 2025-02-28

## Fixed
- Added back `NimBLEClient::connect` overload with `NimBLEAdvertisedDevice` parameter to resolve connection error due to NULL address.
- Crash caused by returning invalid vector entry when retrieving remote descriptors.

## [2.2.0] 2025-02-24

## Fixed
- Crash when calling `NimBLEClient::DiscoverAttributes`.

## Added
- Conditional macros for logging.
- `NimBLEDeviceCallbacks` class with a callback for handling bond storage.

## [2.1.1] 2025-01-26

## Fixed
- remote descriptor discovery error when no descriptors exist.
- scan filter settings not enabled for esp32s3/c3.
- remote descriptor discovery returning more than the desired descriptor.

## [2.1.0] 2025-01-12

## Fixed
- Crash when retrieving descriptors if more than one exists.
- Incorrect TX power value being advertised.
- New user guide code for 2.x
- Potential race condition if `NimBLEScan::clearResults1 is called from multiple tasks.

## Changed
- If privacy is not enabled identity keys will not be shared.
- `NimBLEDevice::setPower` and `NimBLEDevice::getPower` now take an additional parameter `NimBLETxPowerType` to set/get the power level for different operations.

## Added
- Config option `CONFIG_NIMBLE_CPP_ADDR_FMT_EXCLUDE_DELIMITER`, if defined will remove the ":" delimiter from the BLE address string.
- Config option `CONFIG_NIMBLE_CPP_ADDR_FMT_UPPERCASE` if defined will make the BLE address strings uppercase.

## [2.0.3] 2025-01-05

## Fixed
- Unused variable warning when log level is below info.
- Build error missing definition of CONFIG_NIMBLE_CPP_FREERTOS_TASK_BLOCK_BIT in platformio.
- Race condition in `NimBLEScan` that can cause a crash due to heap corruption if `NimBLEScan::stop` is called from the `onResult` callback.
- Advertisement data not set if scan response is enabled after the data is set.
- `NimBLECharacteristic`/`NimBLEDescriptor` not able to update their values in the `onRead` callback.
- Too short of a timeout being requested in NimBLE_Server example leading to frequent disconnects.

## Changed
- `NimBLEHIDDevice` now allows for the same report ID in multiple input/output/feature reports.

## Added
- Config for custom log colors pre level.
- Error logs in the case that NIMBLE_CPP_DEBUG_ASSERT is not defined.
- Error logs when setting advertisement data fails.
- Missing documentation in the migration guide about enabling automatic advertising on disconnect, which was disabled by default in 2.x.

## [2.0.2] 2024-12-21

## Fixed
- Compile error when only advertising role is enabled.
- Possible crash if bonded client reconnects.

## Changed
- `NimBLEHIDDevice` can now create more than one in/out/feature report map.

## [2.0.1] 2024-12-16

## Fixed
- `NimBLEHIDDevice::getOutputReport` will now return the correct characteristic.
- Compile error when central is disabled, class `NimBLEServer` has no member named `m_pClient`.

## Changed
- Added missing includes for `NimBLEConnInfo` and `NimBLEUtils` to `NimBLEDevice.h`.

## [2.0.0] 2024-12-14

## **Breaking changes**
- All connection oriented callbacks now receive a reference to `NimBLEConnInfo`, the `ble_gap_conn_desc` has also been replace with `NimBLEConnInfo` in the functions that received it.
- All functions that take a time input parameter now expect the value to be in milliseconds instead of seconds.
- Removed Eddystone URL as it has been shutdown by google since 2021.
- NimBLESecurity class removed.
- `NimBLEDevice` Ignore list functions removed.
- `NimBLEDevice::startSecurity` now returns a `bool`, true on success, instead of an int to be consistent with the rest of the library.
- `NimBLEDevice::getInitialized` renamed to `NimBLEDevice::isInitialized`.
- `NimBLEDevice::setPower` no longer takes the `esp_power_level_t` and `esp_ble_power_type_t`, instead only an integer value in dbm units is accepted.
- `NimBLEDevice::setOwnAddrType` no longer takes a `bool nrpa` parameter.
- `NimBLEDevice::getClientListSize` replaced with `NimBLEDevice::getCreatedClientCount`.
- `NimBLEDevice::getClientList` was removed.
- `NimBLEServer::disconnect` now returns `bool`, true = success,  instead of `int` to be consistent with the rest of the library.
- `NimBLEServerCallbacks::onMTUChanged` renamed to `NimBLEServerCallbacks::onMTUChange` to be consistent with the client callback.
- `NimBLEServer::getPeerIDInfo` renamed to `NimBLEServer::getPeerInfoByHandle` to better describe it's use.
- `NimBLEServerCallbacks::onPassKeyRequest` has been replaced with `NimBLEServer::onPassKeyDisplay` which should display the pairing pin that the client is expected to send.
- `NimBLEServerCallbacks::onAuthenticationComplete` now takes a `NimBLEConnInfo&` parameter.
- `NimBLEClient::getServices` now returns a const reference to std::vector<NimBLERemoteService*> instead of a pointer to the internal vector.
- `NimBLEClient::getConnId` has been renamed to `getConnHandle` to be consistent with bluetooth terminology.
- `NimBLEClient::disconnect` now returns a `bool`, true on success, instead of an int to be consistent with the rest of the library.
- `NimBLEClientCallbacks::onDisconnect` now takes an additional `int reason` parameter to let the application know why the disconnect occurred.
- `NimBLEClientCallbacks::onPassKeyRequest` has been changed to `NimBLEClientCallbacks::onPassKeyEntry` which takes a `NimBLEConnInfo&` parameter and does not return a value. Instead or returning a value this callback should prompt a user to enter a pin number which is sent later via `NimBLEDevice::injectPassKey`.
- `NimBLEClientCallbacks::onConfirmPIN` renamed to `NimBLEClientCallbacks::onConfirmPasskey` and no longer returns a value and now takes a `NimBLEConnInfo&` parameter. This should be used to prompt a user to confirm the pin on the display (YES/NO) after which the response should be sent with `NimBLEDevice::injectConfirmPasskey`.
- `NimBLEAdvertising::setMinPreferred` and `NimBLEAdvertising::setMaxPreferred` have been removed, use `NimBLEAdvertising::setPreferredParams` instead.
- Advertising the name and TX power of the device will no longer happen by default and should be set manually by the application.
- `NimBLEAdvertising::setAdvertisementType` has been renamed to `NimBLEAdvertising::setConnectableMode` to better reflect it's function.
- `NimBLEAdvertising::setScanResponse` has been renamed to `NimBLEAdvertising::enableScanResponse` to better reflect it's function.
- `NimBLEAdvertising`; Scan response is no longer enabled by default.
- `NimBLEAdvertising::start` No longer takes a callback pointer parameter, instead the new method `NimBLEAdvertising::setAdvertisingCompleteCallback` should be used.
- `NimBLEAdvertisementData::addData` now takes either a `std::vector<uint8_t>` or `uint8_t* + length` instead of `std::string` or `char + length`.
- `NimBLEAdvertisementData::getPayload` now returns `std::vector<uint8_t>` instead of `std::string`.
- The callback parameter for `NimBLEScan::start` has been removed and the blocking overload of `NimBLEScan::start` has been replaced by an overload of `NimBLEScan::getResults` with the same parameters.
- `NimBLEAdvertisedDeviceCallbacks` Has been replaced by `NimBLEScanCallbacks` which contains the following methods: `onResult`, `onScanEnd`, and `onDiscovered
- - `NimBLEScanCallbacks::onResult`, functions the same as the old `NimBLEAdvertisedDeviceCallbacks::onResult` but now takes aa `const NimBLEAdvertisedDevice*` instead of non-const.
- - `NimBLEScanCallbacks::onScanEnd`, replaces the scanEnded callback passed to `NimBLEScan::start` and now takes a `const NimBLEScanResults&` and `int reason` parameter.
- - `NimBLEScanCallbacks::onDiscovered`, This is called immediately when a device is first scanned, before any scan response data is available and takes a `const NimBLEAdvertisedDevice*` parameter.
- `NimBLEScan::stop` will no longer call the `onScanEnd` callback as the caller should know its been stopped when this is called.
- `NimBLEScan::clearDuplicateCache` has been removed as it was problematic and only for the esp32. Stop and start the scanner for the same effect.
- `NimBLEScanResults::getDevice` methods now return `const NimBLEAdvertisedDevice*`.
- `NimBLEScanResults` iterators are now `const_iterator`.
- `NimBLEAdvertisedDevice::hasRSSI` removed as redundant, RSSI is always available.
- `NimBLEAdvertisedDevice::getPayload` now returns `const std::vector<uint8_t>` instead of a pointer to internal memory.
- `NimBLEAdvertisedDevice` Timestamp removed, if needed then the app should track the time from the callback.
- `NimBLECharacteristic::notify` no longer takes a `bool is_notification` parameter, instead `indicate()` should be called to send indications.
- `NimBLECharacteristicCallbacks::onNotify` removed as unnecessary, the library does not call notify without app input.
- `NimBLECharacteristicCallbacks::onStatus` No longer takes a `status` parameter, refer to the return code for success/failure.
- `NimBLERemoteCharacteristic::getRemoteService` now returns a `const NimBLERemoteService*` instead of non-const.
- `NimBLERemoteCharacteristic::readUInt32` Has been removed.
- `NimBLERemoteCharacteristic::readUInt16` Has been removed.
- `NimBLERemoteCharacteristic::readUInt8` Has been removed.
- `NimBLERemoteCharacteristic::readFloat` Has been removed.
- `NimBLERemoteCharacteristic::registerForNotify` Has been removed.
- `NimBLERemoteService::getCharacteristics` now returns a `const std::vector<NimBLERemoteCharacteristic*>&` instead of non-const `std::vector<NimBLERemoteCharacteristic*>*`.
- `NimBLERemoteService::getValue` now returns `NimBLEAttValue` instead of `std::string`.
- `NimBLEService::getCharacteristics` now returns a `const std::vector<NimBLECharacteristic*>&` instead of std::vector<NimBLECharacteristic *>.
- `NimBLEUUID::getNative` method replaced with `NimBLEUUID::getBase` which returns a read-only pointer to the underlying `ble_uuid_t` struct.
- `NimBLEUUID`; `msbFirst` parameter has been removed from constructor, caller should reverse the data first or call the new `reverseByteOrder` method after.
- `NimBLEAddress` constructor; default value for the `type` parameter removed, caller should know the address type and specify it.
- `NimBLEAddress::getNative` replaced with `NimBLEAddress::getBase` and now returns a pointer to `const ble_addr_t` instead of a pointer to the address value.
- `NimBLEAddress::equals` method and `NimBLEAddress::== operator` will now also test if the address types are the same.
- `NimBLEUtils::dumpGapEvent` function removed.
- `NimBLEUtils::buildHexData` replaced with `NimBLEUtils::dataToHexString`, which returns a `std::string` containing the hex string.
- `NimBLEEddystoneTLM::setTemp` now takes an `int16_t` parameter instead of float to be friendly to devices without floating point support.
- `NimBLEEddystoneTLM::getTemp` now returns `int16_t` to work with devices that don't have floating point support.
- `NimBLEEddystoneTLM::setData` now takes a reference to * `NimBLEEddystoneTLM::BeaconData` instead of `std::string`.
- `NimBLEEddystoneTLM::getData` now returns a reference to * `NimBLEEddystoneTLM::BeaconData` instead of `std::string`.
- `NimBLEBeacon::setData` now takes `const NimBLEBeacon::BeaconData&` instead of `std::string`.
- `NimBLEBeacon::getData` now returns `const NimBLEBeacon::BeaconData&` instead of `std::string`.
- `NimBLEHIDDevice::reportMap` renamed to `NimBLEHIDDevice::getReportMap`.
- `NimBLEHIDDevice::hidControl` renamed to `NimBLEHIDDevice::getHidControl`.
- `NimBLEHIDDevice::inputReport`renamed to `NimBLEHIDDevice::getInputReport`.
- `NimBLEHIDDevice::outputReport`renamed to `NimBLEHIDDevice::getOutputReport`.
- `NimBLEHIDDevice::featureReport`renamed to `NimBLEHIDDevice::getFeatureReport`.
- `NimBLEHIDDevice::protocolMode`renamed to `NimBLEHIDDevice::getProtocolMode`.
- `NimBLEHIDDevice::bootOutput`renamed to `NimBLEHIDDevice::getBootOutput`.
- `NimBLEHIDDevice::pnp`renamed to `NimBLEHIDDevice::setPnp`.
- `NimBLEHIDDevice::hidInfo`renamed to `NimBLEHIDDevice::setHidInfo`.
- `NimBLEHIDDevice::deviceInfo`renamed to `NimBLEHIDDevice::getDeviceInfoService`.
- `NimBLEHIDDevice::hidService`renamed to `NimBLEHIDDevice::getHidService`.
- `NimBLEHIDDevice::batteryService`renamed to `NimBLEHIDDevice::getBatteryService`.

## Fixed
- `NimBLEDevice::getPower` and `NimBLEDevice::getPowerLevel` bug worked around for the esp32s3 and esp32c3.
- `NimBLEDevice::setPower` and `NimBLEDevice::getPower` now support the full power range for all esp devices.
- `NimBLEDevice::setOwnAddrType` will now correctly apply the provided address type for all devices.
- `NimBLEDevice::getPower` (esp32) return value is now calculated to support devices with different min/max ranges.
- Crash if `NimBLEDevice::deinit` is called when the stack has not been initialized.
- `NimBLEServer` service changed notifications will now wait until the changes have taken effect and the server re-started before indicating the change to peers, reducing difficultly for some clients to update their data.
- `NimBLEService::getHandle` will now fetch the handle from the stack if not valid to avoid returning an invalid value.
- `std::vector` input to set/write values template.
- `NimBLEHIDDevice::pnp` will now set the data correctly.
- Check for Arduino component 
- Fixed building with esp-idf version 5.x.
- Fixed pairing failing when the process was started by the peer first.
- Fixed building with esp-idf and Arduino component.
- Workaround for esp32s3 and esp32c3 not returning the correct txPower with some IDF versions.

### Changed
- `NimBLEClient::secureConnection` now takes an additional parameter `bool async`, if true, will send the secure command and return immediately with a true value for successfully sending the command, else false. This allows for asynchronously securing a connection.
- Deleting the client instance from the `onDisconnect` callback is now supported.
- `NimBLEClient::connect` will no longer cancel already in progress connections.
- `NimBLEClient::setDataLen` now returns bool, true if successful.
- `NimBLEClient::updateConnParams` now returns bool, true if successful.
- `NimBLEClient::setPeerAddress` now returns a bool, true on success.
- `NimBLEDevice::startSecurity` now takes and additional parameter `int* rcPtr` which will provide the return code from the stack if provided.
- `NimBLEDevice::deleteClient` no longer blocks tasks.
- `NimBLEDevice::getAddress` will now return the address currently in use.
- `NimBLEDevice::init` now returns a bool with `true` indicating success.
- `NimBLEDevice::deinit` now returns a bool with `true` indicating success.
- `NimBLEDevice::setDeviceName` now returns a bool with `true` indicating success.
- `NimBLEDevice::setCustomGapHandler` now returns a bool with `true` indicating success.
- `NimBLEDevice::setOwnAddrType` now returns a bool with `true` indicating success and works with non-esp32 devices.
- `NimBLEDevice::setPower` now returns a bool value, true = success.
- `NimBLEDevice::setMTU` now returns a bool value, true = success.
- `NimBLEDevice::deleteAllBonds` now returns true on success, otherwise false.
- `NimBLEEddystoneTLM` internal data struct type `BeaconData` is now public and usable by the application.
- `NimBLEBeacon` internal data struct type `BeaconData` is now public and can be used by the application.
- Removed tracking of client characteristic subscription status from `NimBLEServer` and `NimBLECharacteristic` and instead uses
the functions and tracking in the host stack.
- `NimBLECharacteristic::indicate` now takes the same parameters as `notify`.
- `NimBLECharacteristic::notify` and `NimBLECharacteristic::indicate` now return a `bool`, true = success.
- Added optional `connHandle` parameter to `NimBLECharacteristic::notify` to allow for sending notifications to specific clients.
- `NimBLEServer` Now uses a std::array to store client connection handles instead of std::vector to reduce memory allocation.
- `NimBLEExtAdvertisement` : All functions that set data now return `bool`, true = success.
- `NimBLEAdvertising` Advertising data is now stored in instances of `NimBLEAdvertisingData` and old vectors removed.
- `NimBLEAdvertising::setAdvertisementData` and `NimBLEAdvertising::setScanResponseData` now return `bool`, true = success.
- Added optional `NimBLEAddress` parameter to `NimBLEAdvertising::start` to allow for directed advertising to a peer.
- All `NimBLEAdvertising` functions that change data values now return `bool`, true = success.
- All `NimBLEAdvertisementData` functions that change data values now return `bool`, true = success.
- `NimBLEAdvertising` advertising complete callback is now defined as std::function to allow for using std::bind for callback functions.
- `NimBLEAdvertisementData::setName` now takes an optional `bool` parameter to indicate if the name is complete or incomplete, default = complete.
- `NimBLEAdvertisementData` moved to it's own .h and .cpp files.
- `NimBLEScan::start` takes a new `bool restart` parameter, default `true`, that will restart an already in progress scan and clear the duplicate filter so all devices will be discovered again.
- Scan response data that is received without advertisement first will now create the device and send a callback.
- `NimBLEScan::start` will no longer clear cache or results if scanning is already in progress.
- `NimBLEScan::start` will now allow the start command to be sent with updated parameters if already scanning.
- `NimBLEScan::clearResults` will now reset the vector capacity to 0.
- Host reset will now no longer restart scanning and instead will call `NimBLEScanCallbacks::onScanEnd`.
- Added optional `index` parameter to `NimBLEAdvertisedDevice::getPayloadByType`
- `NimBLEAdvertisedDevice::getManufacturerData` now takes an optional index parameter for use in the case of multiple manufacturer data fields.
- `NimBLEUtils`: Add missing GAP event strings.
- `NimBLEUtils`: Add missing return code strings.
- `NimBLEUtils`: Event/error code strings optimized.
- `NimBLEAttValue` cleanup and optimization.
- cleaned up code, removed assert/abort calls, replaced with a configurable option to enable debug asserts.

### Added
- (esp32 specific) `NimBLEDevice::setPowerLevel` and `NimBLEDevice::getPowerLevel` which take and return the related `esp_power_level* ` types.
- `NimBLEDevice::setDefaultPhy` which will set the default preferred PHY for all connections.
- `NimBLEDevice::getConnectedClients`, which returns a vector of pointers to the currently connected client instances.
- `NimBLEDevice::setOwnAddr` function added, which takes a `uint8_t*` or `NimBLEAddress&` and will set the mac address of the device, returns `bool` true= success.
- `NimBLEDevice::injectPassKey` Used to send the pairing passkey instead of a return value from the client callback.
- `NimBLEDevice::injectConfirmPasskey` Used to send the numeric comparison pairing passkey confirmation instead of a return value from the client callback.
- `NimBLEDevice::setDeviceName` to change the device name after initialization.
- `NimBLECharacteristic::create2904` which will specifically create a Characteristic Presentation Format (0x2904) descriptor.
- `NimBLEAdvertising::refreshAdvertisingData` refreshes the advertisement data while still actively advertising.
- `NimBLEClient::updatePhy` to request a PHY change with a peer.
- `NimBLEClient::getPhy` to read the current connection PHY setting.
- `Config` struct to `NimBLEClient` to efficiently set single bit config settings.
- `NimBLEClient::setSelfDelete` that takes the bool parameters `deleteOnDisconnect` and `deleteOnConnectFail`, which will configure the client to delete itself when disconnected or the connection attempt fails.
- `NimBLEClient::setConfig` and `NimBLEClient::getConfig` which takes or returns a `NimBLEClient::Config` object respectively.
- `NimBLEClient::cancelConnect()` to cancel an in-progress connection, returns `bool`, true = success.
- Non-blocking `NimBLEClient::connect` option added via 2 new `bool` parameters added to the function:
- * `asyncConnect`; if true, will send the connect command and return immediately.
- * `exchangeMTU`; if true will send the exchange MTU command upon connection.
- `NimBLEClientCallbacks::onConnectFail` callback that is called when the connection attempt fail while connecting asynchronously.
- `NimBLEClientCallbacks::onMTUChange` callback which will be called when the MTU exchange completes and takes a `NimBLEClient*` and `uint16_t MTU` parameter.
- `NimBLEClientCallbacks::onPhyUpdate` and -`NimBLEServerCallbacks::onPhyUpdate` Which are called when the PHY update is complete.
- Extended scan example.
- `NimBLEServer::updatePhy` to request a PHY change with a peer.
- `NimBLEServer::getPhy` to read the PHY of a peer connection.
- `NimBLEServer::getClient` which will create a client instance from the provided peer connHandle or connInfo to facilitate reading/write from the connected client.
- `NimBLEServerCallbacks::onConnParamsUpdate` callback.
- `NimBLEScan::erase` overload that takes a `const NimBLEAdvertisedDevice*` parameter.
- `NimBLEScan::setScanPhy` to enable/disable the PHY's to scan on (extended advertising only).
- `NimBLEScan::setScanPeriod` which will allow for setting a scan restart timer in the controller (extended advertising only).
- `NimBLEAdvertisedDevice::isScannable()` that returns true if the device is scannable.
- `NimBLEAdvertisedDevice::begin` and `NimBLEAdvertisedDevice::end` read-only iterators for convenience and use in range loops.
- `NimBLEAdvertisedDevice::getAdvFlags` returns the advertisement flags of the advertiser.
- `NimBLEAdvertisedDevice::getPayloadByType` Generic use function that returns the data from the advertisement with the specified type.
- `NimBLEAdvertisedDevice::haveType` Generic use function that returns true if the advertisement data contains a field with the specified type.
- Support for esp32c6, esp32c2, esp32h2, and esp32p4.
- `NimBLEExtAdvertisement::removeData`, which will remove the data of the specified type from the advertisement.
- `NimBLEExtAdvertisement::addServiceUUID`, which will append to the service uuids advertised.
- `NimBLEExtAdvertisement::removeServiceUUID`, which will remove the service from the uuids advertised.
- `NimBLEExtAdvertisement::removeServices`, which will remove all service uuids advertised.
- New overloads for `NimBLEExtAdvertisement::setServiceData` with the parameters `const NimBLEUUID& uuid, const uint8_t* data, size_t length` and `const NimBLEUUID& uuid, const std::vector<uint8_t>& data`.
- `NimBLEExtAdvertisement::getDataLocation`, which returns the location in the advertisement data of the type requested in parameter `uint8_t type`.
- `NimBLEExtAdvertisement::toString` which returns a hex string representation of the advertisement data.
- `NimBLEAdvertising::getAdvertisementData`, which returns a reference to the currently set advertisement data.
- `NimBLEAdvertising::getScanData`, which returns a reference to the currently set scan response data.
- New overloads for `NimBLEAdvertising::removeServiceUUID` and `NimBLEAdvertisementData::removeServiceUUID` to accept a `const char*`
- `NimBLEAdvertising::clearData`, which will clear the advertisement and scan response data.
- `NimBLEAdvertising::setManufacturerData` Overload that takes a `const uint8_t*` and , size_t` parameter.
- `NimBLEAdvertising::setServiceData` Overload that takes `const NimBLEUUID& uuid`, ` const uint8_t* data`, ` size_t length` as parameters.
- `NimBLEAdvertising::setServiceData` Overload that takes `const NimBLEUUID& uuid`, `const std::vector<uint8_t>&` as parameters.
- `NimBLEAdvertising::setDiscoverableMode` to allow applications to control the discoverability of the advertiser.
- `NimBLEAdvertising::setAdvertisingCompleteCallback` sets the callback to call when advertising ends.
- `NimBLEAdvertising::setPreferredParams` that takes the min and max preferred connection parameters as an alternative for `setMinPreferred` and `setMaxPreferred`.
- `NimBLEAdvertising::setAdvertisingInterval` Sets the advertisement interval for min and max to the same value instead of calling `setMinInterval` and `setMaxInterval` separately if there is not value difference.
- `NimBLEAdvertisementData::removeData`, which takes a parameter `uint8_t type`, the data type to remove.
- `NimBLEAdvertisementData::toString`, which will print the data in hex.
- `NimBLEUtils::taskWait` which causes the calling task to wait for an event.
- `NimBLEUtils::taskRelease` releases the task from and event.
- `NimBLEUtils::generateAddr` function added with will generate a random address and takes a `bool` parameter, true = create non-resolvable private address, otherwise a random static address is created, returns `NimBLEAddress`.
- `NimBLEUUID::getValue` which returns a read-only `uint8_t` pointer to the UUID value.
- `NimBLEUUID::reverseByteOrder`, this will reverse the bytes of the UUID, which can be useful for advertising/logging.
- `NimBLEUUID` constructor overload that takes a reference to `ble_uuid_any_t`.
- `NimBLEAddress::isNrpa` method to test if an address is random non-resolvable.
- `NimBLEAddress::isStatic` method to test if an address is random static.
- `NimBLEAddress::isPublic` method to test if an address is a public address.
- `NimBLEAddress::isNull` methods to test if an address is NULL.
- `NimBLEAddress::getValue` method which returns a read-only pointer to the address value.
- `NimBLEAddress::reverseByteOrder` method which will reverse the byte order of the address value.
- `NimBLEHIDDevice::batteryLevel` returns the HID device battery level characteristic.
- `NimBLEBeacon::setData` overload that takes `uint8_t* data` and `uint8_t length`.
- `NimBLEHIDDevice::getPnp` function added to access the pnp characteristic.
- `NimBLEHIDDevice::getHidInfo` function added to access the hid info characteristic.

## [1.4.1] - 2022-10-30

### Fixed
 - NimBLEDevice::getPower incorrect value when power level is -3db.
 - Failed pairing when already in progress.

### Changed
 - Revert previous change that forced writing with response when subscribing in favor of allowing the application to decide.

### Added
 - Added NimBLEHIDDevice::batteryLevel.
 - Added NimBLEDevice::setDeviceName allowing for changing the device name while the BLE stack is active.
 - CI Builds

## [1.4.0] - 2022-07-31

### Fixed
- Fixed missing data from long notification values.
- Fixed NimbleCharacteristicCallbacks::onRead not being called when a non-long read command is received.
- Prevent a potential crash when retrieving characteristics from a service if the result was successful but no characteristics found.
- logs/typos.

### Changed
- AD flags are no longer set in the advertisements of non-connectable beacons, freeing up 3 bytes of advertisement room.
- Save resources when retrieving descriptors if the characteristic handle is the same as the end handle (no descriptors).
- Subscribing to characteristic notifications/indications will now always use write with response, as per BLE specifications.
- `NimBLEClient::discoverAttributes` now returns a bool value to indicate success/failure.
- Scan result callbacks are no longer called when the scan response data is updated in order to reduce duplicates.

### Added
- Preliminary support for non-esp devices, NRF51 and NRF52 devices supported with [n-able arduino core](https://github.com/h2zero/n-able-Arduino)
- Alias added for  `NimBLEServerCallbacks::onMTUChange` to `onMtuChanged` in order to support porting code from original library.
- `NimBLEAttValue` Class added to reduce and control RAM footprint of characteristic/descriptor values and support conversions from Arduino Strings and many other data types.
- Bluetooth 5 extended advertising support for capable devices. CODED Phy, 2M Phy, extended advertising data, and multi-advertising are supported, periodic advertising will be implemented in the future.

## [1.3.3] - 2022-02-15

### Changed
- If attribute retrieval fails with a "not found" try again with the 16 bit version if a 128 bit base uuid is used.

### Fixed
- Memory leak when deleting client instance.
- IDf version check for data length extension.
- Memory leak when server services changed.
- Compiler warnings for non-esp32 devices.

## [1.3.2] - 2022-01-15

### Fixed
- Initialize advertising complete callback in NimBLEAdvertising constructor.
- Clear client disconnect timer in constructor before initializing.
- Fix missing data when reading large values.
- Fix missing data in notifications when using a large MTU size and more than 270 bytes of data are sent.
- Workaround fix added for cases when the task notification value is not cleared, causing various functions that should block not to block.

### Added
- `NimBLEClient::getLastError` : Gets the error code of the last function call that produces a return code from the stack.
- `NimBLECharacteristic::notify` : Overload method to send notifications/indications with custom values.
- Added conditional checks for ESP32 specific functions/values to support use of the library on non-esp32 devices.
- Added an alias to use the callback name from the original library `onMtuChanged`.
- `NimBLEClient::setDataLen` and `NimBLEServer::setDataLen`:  Data length extension support (IDF version >= 4.3.2 only)
- Config option to set logging level for esp-nimble-cpp

### Changed
- Critical section calls now use the NimBLE API instead of FreeRTOS directly. This removes the need for a `portMUX_TYPE` variable in the class definitions.
- Removed unnecessary variables in `NimBLEService` and changed the constructor no no longer accept `numHandles` and `inst_id` parameters.

## [1.3.1] - 2021-08-04

### Fixed
- Corrected a compiler/linker error when an application or a library uses bluetooth classic due to the redefinition of `btInUse`.

## [1.3.0] - 2021-08-02

### Added
- `NimBLECharacteristic::removeDescriptor`: Dynamically remove a descriptor from a characteristic. Takes effect after all connections are closed and sends a service changed indication.
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
- Writing to a characteristic with a std::string value now correctly writes values with null characters.
- Retrieving remote descriptors now uses the characteristic end handle correctly.
- Missing data in long writes to remote descriptors.
- Hanging on task notification when sending an indication from the characteristic callback.
- BLE controller memory could be released when using Arduino as a component.
- Compile errors with NimBLE release 1.3.0.

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

- `NimBLEAdvertising` Custom scan response data can now be used without custom advertisement.

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
regardless of the existence of the CCCD and return true unless the descriptor write operation failed.

- Advertising tx power level is now sent in the advertisement packet instead of scan response.

- `NimBLEScan` When the scan ends the scan stopped flag is now set before calling the scan complete callback (if used)
this allows the starting of a new scan from the callback function.

### Fixed
- Sometimes `NimBLEClient::connect` would hang on the task block if no event arrived to unblock.
A time limit has been added to timeout appropriately.

- When getting descriptors for a characteristic the end handle of the service was used as a proxy for the characteristic end
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

- `FreeRTOS` compile errors resolved in latest Arduino core and IDF v3.3.

- Multiple instances of `time()` called inside critical sections caused sporadic crashes, these have been moved out of critical regions.

- Advertisement type now correctly set when using non-connectable (advertiser only) mode.

- Advertising payload length correction, now accounts for appearance.

- (Arduino) Ensure controller mode is set to BLE Only.


## [1.0.2] - 2020-09-13

### Changed

- `NimBLEAdvertising::start` Now takes 2 optional parameters, the first is the duration to advertise for (in seconds), the second is a
callback that is invoked when advertising ends and takes a pointer to a `NimBLEAdvertising` object (similar to the `NimBLEScan::start` API).

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
