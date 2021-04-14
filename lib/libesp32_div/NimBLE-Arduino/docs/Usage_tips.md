# Usage Tips

## Put BLE functions in a task running on the NimBLE stack core

When commands are sent to the stack from a differnt core they can experience delays in execution.  
This library detects this and invokes the esp32 IPC to reroute these commands through the correct core but this also increases overhead.  
Therefore it is highly recommended to create tasks for BLE to run on the same core, the macro `CONFIG_BT_NIMBLE_PINNED_TO_CORE` can be used to set the core.
<br/>  

## Do not delete client instances unless necessary or unused

When a client instance has been created and has connected to a peer device and it has retrieved service/characteristic information it will store that data for the life of the client instance.  
If you are periodically connecting to the same devices and you have deleted the client instance or the services when connecting again it will cause a retrieval of that information from the peer again.  
This results in significant energy drain on the battery of the devices, fragments heap, and reduces connection performance.  
  
Client instances in this library use approximately 20% of the original bluedroid library, deleteing them will provide much less gain than it did before.  

It is recommended to retain the client instance in cases where the time between connecting to the same device is less than 5 minutes.  
<br/>  

## Only retrieve the services and characteriscs needed

As a client the use of `NimBLEClient::getServices` or `NimBLERemoteService::getCharacteristics` and using `true` for the parameter should be limited to devices that are not known.  
Instead `NimBLEClient::getService(NimBLEUUID)` or `NimBLERemoteService::getCharacteristic(NimBLEUUID)` should be used to access certain attributes that are useful to the application.  
This reduces energy consumed, heap allocated, connection time and improves overall efficiency.  
<br/>  

## Check return values

Many user issues can be avoided by checking if a function returned successfully, by either testing for true/false such as when calling `NimBLEClient::connect`,  
or nullptr such as when  calling `NimBLEClient::getService`. The latter being a must, as calling a method on a nullptr will surely result in a crash.  
Most of the functions in this library return something that should be checked before proceeding.  
<br/>  

## There will be bugs - please report them

No code is bug free and unit testing will not find them all on it's own. If you encounter a bug, please report it along with any logs and decoded backtrace if applicable.  
Best efforts will be made to correct any errors ASAP.  

Bug reports can be made at https://github.com/h2zero/NimBLE-Arduino/issues or https://github.com/h2zero/esp-nimble-cpp/issues.  
Questions and suggestions will be happily accepted there as well.
