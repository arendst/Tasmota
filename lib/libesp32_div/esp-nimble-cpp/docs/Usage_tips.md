# Usage Tips

## Threadsafety

This library is threadsafe. Attribues can be manipulated freely.

## Do not delete client instances unless necessary or unused

When a client instance has been created and has connected to a peer device and it has retrieved service/characteristic information it will store that data for the life of the client instance.  
If you are periodically connecting to the same devices and you have deleted the client instance or the services when connecting again it will cause a retrieval of that information from the peer again.  
This results in significant energy drain on the battery of the devices, fragments heap, and reduces connection performance.  
  
Client instances in this library use approximately 20% of the original bluedroid library, deleting them will provide much less gain than it did before.  

It is recommended to retain the client instance in cases where the time between connecting to the same device is less than 5 minutes.  
<br/>  

## Only retrieve the services and characteristics needed

As a client the use of `NimBLEClient::getServices` or `NimBLERemoteService::getCharacteristics` and using `true` for the parameter should be limited to devices that are not known.  
Instead `NimBLEClient::getService(NimBLEUUID)` or `NimBLERemoteService::getCharacteristic(NimBLEUUID)` should be used to access certain attributes that are useful to the application.  
This reduces energy consumed, heap allocated, connection time and improves overall efficiency.  
<br/>  

## Check return values

Many user issues can be avoided by checking if a function returned successfully, by either testing for true/false such as when calling `NimBLEClient::connect`,  
or nullptr such as when  calling `NimBLEClient::getService`. The latter being a must, as calling a method on a nullptr will surely result in a crash.  
Most of the functions in this library return something that should be checked before proceeding.  
<br/>  

## Persisted bonds can be lost due to low MAX_CCCDS

The symptom: CONFIG_BT_NIMBLE_MAX_BONDS is set to N, but a smaller number of bonds is preserved, perhaps even a single bond. The limitation of persisted bonds can be observed via NimBLEDevice::getNumBonds(). The number may not reach CONFIG_BT_NIMBLE_MAX_BONDS.

Cause: For each bond, NimBLE persists each of the CCCD (client characteristic configuration descriptor) values that have been subscribed
to by the client. If CONFIG_BT_NIMBLE_MAX_CCCDS is too low, the older CCCD values are overwritten by the newer ones. The loss of the older
CCCDs values results in those bonds being lost.

Fix: Increase CONFIG_BT_NIMBLE_MAX_CCCDS. These take approximately 40 bytes in NVS, 2 bytes for the CCCD value and the NVS metadata overhead. The value of
CONFIG_BT_NIMBLE_MAX_CCCDS should conservatively be no less than (CONFIG_BT_NIMBLE_MAX_BONDS * {maximum number of characteristics that can be subscribed to}).

## Device 'Local Name'

'Local name' refers to how the device is seen and displayed.

A devices 'Local name' can be thought of as coming from two places, the <i>Advertising "Local name"</i> and the <i>the GATT Device Name</i>.

### Advertising "Local name"

Field found in the advertising data payload. Value is set via NimBLEAdvertising::setName().

### GATT Device Name

Characteristic UUID 0x2A00 in the Generic Access service. Set via NimBLEDevice::init() or NimBLEDevice::setDeviceName().

This characteristic is read <b>after</b> connecting to the device.

### Important considerations

* OSes cache the <i>'GATT Device Name'</i>.
* OSes update the device name based on the <i>'GATT Device Name'</i> after connecting to a device. This means that if you set the <i>Advertising 'Local name'</i> to "ABCD" but the <i>'GATT Device Name'</i> to "12345", the device will be seen as "ABCD" until connecting to the device, at which time the devices name will change to "12345".
* If no <i>'Advertising "Local name"'</i> is set, OSes, such as iOS, may display the devices name as 'Unnamed' until the device is connected to, at which time the <i>'GATT Device Name'</i> is read and used instead.

It is recommended that both <i>'Advertising "Local name"'</i> <b>and</b> <i>'GATT Device Name'</i> be set appropriately, after considering the above described behavior.

## There will be bugs - please report them

No code is bug free and unit testing will not find them all on it's own. If you encounter a bug, please report it along with any logs and decoded backtrace if applicable.  
Best efforts will be made to correct any errors ASAP.  

Bug reports can be made at https://github.com/h2zero/NimBLE-Arduino/issues or https://github.com/h2zero/esp-nimble-cpp/issues.  
Questions and suggestions will be happily accepted there as well.
