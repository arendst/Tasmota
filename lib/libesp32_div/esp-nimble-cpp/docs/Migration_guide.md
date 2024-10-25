# Migrating from Bluedroid to NimBLE

This guide describes the required changes to existing projects migrating from the original bluedroid API to NimBLE.

**The changes listed here are only the required changes that must be made**, and a short overview of options for migrating existing applications.

For more information on the improvements and additions please refer to the [class documentation](https://h2zero.github.io/NimBLE-Arduino/annotated.html) and [Improvements and updates](Improvements_and_updates.md)

* [General Changes](#general-information)
* [Server](#server-api)
    * [Services](#services)
    * [Characteristics](#characteristics)
    * [Characteristic Callbacks](#characteristic-callbacks)
    * [Descriptors](#descriptors)
    * [Descriptor Callbacks](#descriptor-callbacks)
    * [Security](#server-security)
* [Advertising](#advertising-api)
* [Client](#client-api)
    * [Remote Services](#remote-services)
    * [Remote characteristics](#remote-characteristics)
    * [Client Callbacks](#client-callbacks)
    * [Security](#client-security)
* [Scanning](#scan-api)
* [General Security](#security-api)
* [Configuration](#arduino-configuration)
<br/>

<a name="general-information"></a>
## General Information

### Header Files
All classes are accessible by including `NimBLEDevice.h` in your application, no further headers need to be included.

(Mainly for Arduino) You may choose to include `NimBLELog.h` in your application if you want to use the `NIMBLE_LOGx` macros for debugging. These macros are used the same way as the `ESP_LOGx` macros.  
<br/>

### Class Names
Class names remain the same as the original with the addition of a "Nim" prefix.
For example `BLEDevice` is now `NimBLEDevice` and `BLEServer` is now `NimBLEServer` etc.

For convenience definitions have been added to allow applications to use either name for all classes this means **no class names need to be changed in existing code** and makes migrating easier.  
<br/>

### BLE Addresses
`BLEAddress` (`NimBLEAddress`) When constructing an address the constructor now takes an *(optional)* `uint8_t type` parameter to specify the address type. Default is (0) Public static address.

For example `BLEAddress addr(11:22:33:44:55:66, 1)` will create the address object with an address type of: 1 (Random).

As this parameter is optional no changes to existing code are needed, it is mentioned here for information.

`BLEAddress::getNative` (`NimBLEAddress::getNative`) returns a uint8_t pointer to the native address byte array. In this library the address bytes are stored in reverse order from the original library. This is due to the way the NimBLE stack expects addresses to be presented to it. All other functions such as `toString` are not affected as the endian change is made within them.  
<br/>

<a name="server-api"></a>
## Server API
Creating a `BLEServer` instance is the same as original, no changes required.
For example `BLEDevice::createServer()` will work just as it did before.

`BLEServerCallbacks` (`NimBLEServerCallbacks`) has new methods for handling security operations.  
<br/>

`BLEServerCallbacks::onConnect` (`NimBLEServerCallbacks::onConnect`) only has a single callback declaration which takes an additional (required) parameter `NimBLEConnInfo & connInfo`, which has methods to get information about the connected peer.
```
void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo)`
```
<br/>

`BLEServerCallbacks::onDisconnect` (`NimBLEServerCallbacks::onDisconnect`) only has a single callback declaration which takes 2 additional (required) parameters `NimBLEConnInfo & connInfo`, which provides information about the peer and `int reason`, which gives the reason code for disconnection.

```
void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason)`
```
<br/>

`BLEServerCallbacks::onMtuChanged` (`NimBLEServerCallbacks::onMtuChanged`) takes the parameter `NimBLEConnInfo & connInfo` instead of `esp_ble_gatts_cb_param_t`, which has methods to get information about the connected peer.

```
onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo)
```

**Note:** All callback methods have default implementations which allows the application to implement only the methods applicable.  
<br/>

<a name="services"></a>
### Services
Creating a `BLEService` (`NimBLEService`) instance is the same as original, no changes required.
For example `BLEServer::createService(SERVICE_UUID)` will work just as it did before.  
<br/>

<a name="characteristics"></a>
### Characteristics
`BLEService::createCharacteristic` (`NimBLEService::createCharacteristic`) is used the same way as originally except the properties parameter has changed.

When creating a characteristic the properties are now set with `NIMBLE_PROPERTY::XXXX` instead of `BLECharacteristic::XXXX`.

#### Originally
> BLECharacteristic::PROPERTY_READ |  
BLECharacteristic::PROPERTY_WRITE

#### Is Now
> NIMBLE_PROPERTY::READ |  
NIMBLE_PROPERTY::WRITE
<br/>

#### The full list of properties
> NIMBLE_PROPERTY::READ  
NIMBLE_PROPERTY::READ_ENC  
NIMBLE_PROPERTY::READ_AUTHEN  
NIMBLE_PROPERTY::READ_AUTHOR  
NIMBLE_PROPERTY::WRITE  
NIMBLE_PROPERTY::WRITE_NR  
NIMBLE_PROPERTY::WRITE_ENC  
NIMBLE_PROPERTY::WRITE_AUTHEN  
NIMBLE_PROPERTY::WRITE_AUTHOR  
NIMBLE_PROPERTY::BROADCAST  
NIMBLE_PROPERTY::NOTIFY  
NIMBLE_PROPERTY::INDICATE  

<br/>

**Example:**
```
BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                     CHARACTERISTIC_UUID,
                                     BLECharacteristic::PROPERTY_READ   |
                                     BLECharacteristic::PROPERTY_WRITE
                                     );

```
Needs to be changed to:
```
BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                     CHARACTERISTIC_UUID,
                                     NIMBLE_PROPERTY::READ |
                                     NIMBLE_PROPERTY::WRITE
                                    );
```
<br/>

<a name="characteristic-callbacks"></a>
#### Characteristic callbacks
`BLECharacteristicCallbacks` (`NimBLECharacteristicCallbacks`) has a new method `NimBLECharacteristicCallbacks::onSubscribe` which is called when a client subscribes to notifications/indications.

`BLECharacteristicCallbacks::onRead` (`NimBLECharacteristicCallbacks::onRead`)  only has a single callback declaration, which takes an additional (required) parameter of `NimBLEConnInfo& connInfo`, which provides connection information about the peer.

`BLECharacteristicCallbacks::onWrite` (`NimBLECharacteristicCallbacks::onWrite`)  only has a single callback declaration, which takes an additional (required) parameter of `NimBLEConnInfo& connInfo`, which provides connection information about the peer.  

`BLECharacteristicCallbacks::onStatus` (`NimBLECharacteristicCallbacks::onStatus`) has had the status parameter removed as it was unnecessary since the status code from the BLE stack was also provided. The status code for success is 0 for notifications and BLE_HS_EDONE for indications, any other value is an error.

**Note:** All callback methods have default implementations which allows the application to implement only the methods applicable.  
<br/>

> BLECharacteristic::getData

**Has been removed from the API.**
Originally this returned a `uint8_t*` to the internal data, which is volatile.
To prevent possibly throwing exceptions this has been removed and `NimBLECharacteristic::getValue` should be used
to get a copy of the data first which can then safely be accessed via pointer.

**Example:**
```
std::string value = pCharacteristic->getValue();
uint8_t *pData = (uint8_t*)value.data();
```
Alternatively use the `getValue` template:
```
my_struct_t myStruct = pChr->getValue<my_struct_t>();
```
<br/>

<a name="descriptors"></a>
### Descriptors

Descriptors are now created using the `NimBLECharacteristic::createDescriptor` method.

BLE2902 or NimBLE2902 class has been removed.
NimBLE automatically creates the 0x2902 descriptor if a characteristic has a notification or indication property assigned to it.

It was no longer useful to have a class for the 0x2902 descriptor as a new callback `NimBLECharacteristicCallbacks::onSubscribe` was added
to handle callback functionality and the client subscription status is handled internally.

**Note:** Attempting to create a 0x2902 descriptor will trigger an assert to notify the error,
allowing the creation of it would cause a fault in the NimBLE stack.

All other descriptors are now created just as characteristics are by using the `NimBLECharacteristic::createDescriptor` method (except 0x2904, see below).
Which are defined as:
```
NimBLEDescriptor* createDescriptor(const char* uuid,
                                   uint32_t properties =
                                   NIMBLE_PROPERTY::READ |
                                   NIMBLE_PROPERTY::WRITE,
                                   uint16_t max_len = 100);

NimBLEDescriptor* createDescriptor(NimBLEUUID uuid,
                                   uint32_t properties =
                                   NIMBLE_PROPERTY::READ |
                                   NIMBLE_PROPERTY::WRITE,
                                   uint16_t max_len = 100);
```
##### Example
```
pDescriptor = pCharacteristic->createDescriptor("ABCD",
                                                NIMBLE_PROPERTY::READ |
                                                NIMBLE_PROPERTY::WRITE |
                                                NIMBLE_PROPERTY::WRITE_ENC,
                                                25);
```
Would create a descriptor with the UUID 0xABCD, publicly readable but only writable if paired/bonded (encrypted) and has a max value length of 25 bytes.
<br/>

For the 0x2904, there is a special class that is created when you call `createDescriptor("2904").

The pointer returned is of the base class `NimBLEDescriptor` but the call will create the derived class of `NimBLE2904` so you must cast the returned pointer to
`NimBLE2904` to access the specific class methods.

##### Example
```
p2904 = (NimBLE2904*)pCharacteristic->createDescriptor("2904");
```
<br/>

<a name="descriptor-callbacks"></a>
#### Descriptor callbacks

> `BLEDescriptorCallbacks::onRead` (`NimBLEDescriptorCallbacks::onRead`)
 `BLEDescriptorCallbacks::onwrite` (`NimBLEDescriptorCallbacks::onwrite`)

The above descriptor callbacks take an additional (required) parameter `NimBLEConnInfo& connInfo`, which contains the connection information of the peer.
<br/>

<a name="server-security"></a>
### Server Security
Security is set on the characteristic or descriptor properties by applying one of the following:
> NIMBLE_PROPERTY::READ_ENC  
NIMBLE_PROPERTY::READ_AUTHEN  
NIMBLE_PROPERTY::READ_AUTHOR  
NIMBLE_PROPERTY::WRITE_ENC  
NIMBLE_PROPERTY::WRITE_AUTHEN  
NIMBLE_PROPERTY::WRITE_AUTHOR  

<br/>

When a peer wants to read or write a characteristic or descriptor with any of these properties applied it will trigger the pairing process. By default the "just-works" pairing will be performed automatically.

This can be changed to use passkey authentication or numeric comparison. See [Security API](#security-api) for details.  
<br/>

<a name="advertising-api"></a>
## Advertising API
Advertising works the same as the original API except:

Calling `NimBLEAdvertising::setAdvertisementData` will entirely replace any data set with `NimBLEAdvertising::addServiceUUID`, or
`NimBLEAdvertising::setAppearance` or similar methods. You should set all the data you wish to advertise within the `NimBLEAdvertisementData` instead.  
<br/>

> BLEAdvertising::start (NimBLEAdvertising::start)

Now takes 2 optional parameters, the first is the duration to advertise for (in milliseconds), the second is a callback that is invoked when advertising ends and takes a pointer to a `NimBLEAdvertising` object (similar to the `NimBLEScan::start` API).
This provides an opportunity to update the advertisement data if desired.  
<br/>

<a name="client-api"></a>
## Client API

Client instances are created just as before with `BLEDevice::createClient` (`NimBLEDevice::createClient`).

Multiple client instances can be created, up to the maximum number of connections set in the config file (default: 3). To delete a client instance you must use `NimBLEDevice::deleteClient`.

`BLEClient::connect`(`NimBLEClient::connect`) Has had it's parameters altered.
Defined as:
> NimBLEClient::connect(bool deleteServices = true);
> NimBLEClient::connect(NimBLEAdvertisedDevice\* device, bool deleteServices = true);
> NimBLEClient::connect(NimBLEAddress address, bool deleteServices = true);

The type parameter has been removed and a new bool parameter has been added to indicate if the client should delete the attribute database previously retrieved (if applicable) for the peripheral, default value is true.

If set to false the client will use the attribute database it retrieved from the peripheral when previously connected.

This allows for faster connections and power saving if the devices dropped connection and are reconnecting.  
<br/>

> `BLEClient::getServices` (`NimBLEClient::getServices`)

This method now takes an optional (bool) parameter to indicate if the services should be retrieved from the server (true) or the currently known database returned (false : default).
Also now returns a pointer to `std::vector` instead of `std::map`.  
<br/>

**Removed:** the automatic discovery of all peripheral attributes as they consumed time and resources for data the user may not be interested in.

**Added:** `NimBLEClient::discoverAttributes` for the user to discover all the peripheral attributes to replace the the removed automatic functionality.  
<br/>

<a name="remote-services"></a>
### Remote Services
`BLERemoteService` (`NimBLERemoteService`) Methods remain mostly unchanged with the exceptions of:

> BLERemoteService::getCharacteristicsByHandle

This method has been removed.  
<br/>

> `BLERemoteService::getCharacteristics` (`NimBLERemoteService::getCharacteristics`)

This method now takes an optional (bool) parameter to indicate if the characteristics should be retrieved from the server (true) or
the currently known database returned (false : default).
Also now returns a pointer to `std::vector` instead of `std::map`.  
<br/>

<a name="remote-characteristics"></a>
### Remote Characteristics
`BLERemoteCharacteristic` (`NimBLERemoteCharacteristic`)
 There have been a few changes to the methods in this class:

> `BLERemoteCharacteristic::writeValue` (`NimBLERemoteCharacteristic::writeValue`)

Now returns true or false to indicate success or failure so you can choose to disconnect or try again.  
<br/>

> `BLERemoteCharacteristic::registerForNotify`

Has been removed.

> `NimBLERemoteCharacteristic::subscribe`  
> `NimBLERemoteCharacteristic::unsubscribe`  

Are the new methods added to replace it.  
<br/>

> `BLERemoteCharacteristic::readUInt8` (`NimBLERemoteCharacteristic::readUInt8`)
> `BLERemoteCharacteristic::readUInt16` (`NimBLERemoteCharacteristic::readUInt16`)
> `BLERemoteCharacteristic::readUInt32` (`NimBLERemoteCharacteristic::readUInt32`)
> `BLERemoteCharacteristic::readFloat` (`NimBLERemoteCharacteristic::readFloat`)

Are **deprecated** a template: `NimBLERemoteCharacteristic::readValue<type\>(time_t\*, bool)` has been added to replace them.  
<br/>

> `BLERemoteCharacteristic::readRawData`

**Has been removed from the API**
Originally it stored an unnecessary copy of the data and was returning a `uint8_t` pointer to volatile internal data.
The user application should use `NimBLERemoteCharacteristic::readValue` or `NimBLERemoteCharacteristic::getValue`.
To obtain a copy of the data, then cast the returned std::string to the type required such as:
```
std::string value = pChr->readValue();
uint8_t *data = (uint8_t*)value.data();
```
Alternatively use the `readValue` template:
```
my_struct_t myStruct = pChr->readValue<my_struct_t>();
```
<br/>

> `BLERemoteCharacteristic::getDescriptors` (`NimBLERemoteCharacteristic::getDescriptors`)

This method now takes an optional (bool) parameter to indicate if the descriptors should be retrieved from the server (true) or
the currently known database returned (false : default).
Also now returns a pointer to `std::vector` instead of `std::map`.  
<br/>

<a name="client-callbacks"></a>
### Client callbacks

> `BLEClientCallbacks::onDisconnect` (`NimBLEClientCallbacks::onDisconnect`)

This now takes a second parameter `int reason` which provides the reason code for disconnection.  
<br/>

<a name="client-security"></a>
### Client Security
The client will automatically initiate security when the peripheral responds that it's required.
The default configuration will use "just-works" pairing with no bonding, if you wish to enable bonding see below.  
<br/>

<a name="scan-api"></a>
## BLE Scan
The scan API is mostly unchanged from the original except for `NimBLEScan::start`, in which the duration parameter is now in milliseconds instead of seconds.  
<br/>

<a name="security-api"></a>
## Security API
Security operations have been moved to `BLEDevice` (`NimBLEDevice`).  
The security callback methods are now incorporated in the `NimBLEServerCallbacks` / `NimBLEClientCallbacks` classes.

The callback methods are:

> `bool onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pin)`

Receives the pin when using numeric comparison authentication.
Call `NimBLEDevice::injectConfirmPIN(connInfo, true);` to accept or `NimBLEDevice::injectConfirmPIN(connInfo, false);` to reject.
<br/>

> `void onPassKeyEntry(const NimBLEConnInfo& connInfo)`

Client callback; client should respond with the passkey (pin) by calling `NimBLEDevice::injectPassKey(connInfo, 123456);`
<br/>

> `uint32_t onPassKeyDisplay()`

Server callback; should return the passkey (pin) expected from the client.
<br/>

> `void onAuthenticationComplete(const NimBLEConnInfo& connInfo)`

Authentication complete, success or failed information is available from the `NimBLEConnInfo` methods.  
<br/>

Security settings and IO capabilities are now set by the following methods of NimBLEDevice.
> `NimBLEDevice::setSecurityAuth(bool bonding, bool mitm, bool sc)`
> `NimBLEDevice::setSecurityAuth(uint8_t auth_req)`

Sets the authorization mode for this device.  
<br/>

> `NimBLEDevice::setSecurityIOCap(uint8_t iocap)`

Sets the Input/Output capabilities of this device.  
<br/>

> `NimBLEDevice::setSecurityInitKey(uint8_t init_key)`

If we are the initiator of the security procedure this sets the keys we will distribute.  
<br/>

> `NimBLEDevice::setSecurityRespKey(uint8_t resp_key)`

Sets the keys we are willing to accept from the peer during pairing.  
<br/>

<a name="arduino-configuration"></a>
## Arduino Configuration

Unlike the original library pre-packaged in the esp32-arduino, this library has all the configuration options that are normally set in menuconfig available in the *src/nimconfig.h* file.

This allows Arduino users to fully customize the build, such as increasing max connections or loading the BLE stack into external PSRAM.

For details on the options, they are fully commented in *nimconfig.h*  
<br/>
