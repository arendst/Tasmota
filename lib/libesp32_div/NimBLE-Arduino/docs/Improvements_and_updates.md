# Improvements and updates

Many improvements have been made to this library vs the original, this is a brief overview of the most significant changes.  
Refer to the [class documentation](https://h2zero.github.io/esp-nimble-cpp/annotated.html) for futher information on class specifics.  

* [Server](#server)
* [Advertising](#advertising)
* [Client](#client)
* [General](#general)  
<br/>

<a name="server"></a>
# Server  

`NimBLECharacteristic::setValue(const T &s)`  
`NimBLEDescriptor::setValue(const T &s)`  

Now use a template to accomodate standard and custom types/values.  

**Example**
```
struct my_struct{
    uint8_t  one;
    uint16_t two;
    uint32_t four;
    uint64_t eight;
    float    flt;
}myStruct;
        
    myStruct.one = 1;
    myStruct.two = 2;
    myStruct.four = 4;
    myStruct.eight = 8;
    myStruct.flt = 1234.56;

    pCharacteristic->setValue(myStruct);
 ```
This will send the struct to the recieving client when read or a notification sent.  

`NimBLECharacteristic::getValue` now takes an optional timestamp parameter which will update it's value with  
the time the last value was recieved. In addition an overloaded template has been added to retrieve the value  
as a type specified by the user.  

**Example**
```
    time_t timestamp;
    myStruct = pCharacteristic->getValue<myStruct>(&timestamp); // timestamp optional
```
<br/>

**Advertising will automatically start when a client disconnects.**  

A new method `NimBLEServer::advertiseOnDisconnect(bool)` has been implemented to control this, true(default) = enabled.  
<br/>

`NimBLEServer::removeService` takes an additional parameter `bool deleteSvc` that if true will delete the service  
and all characteristics / descriptors belonging to it and invalidating any pointers to them.  

If false the service is only removed from visibility by clients. The pointers to the service and  
it's characteristics / descriptors will remain valid and the service can be re-added in the future  
using `NimBLEServer::addService`.  
<br/>

<a name="advertising"></a>
# Advertising
`NimBLEAdvertising::start`

Now takes 2 optional parameters, the first is the duration to advertise for (in seconds), the second is a callback  
that is invoked when advertsing ends and takes a pointer to a `NimBLEAdvertising` object (similar to the `NimBLEScan::start` API).  

This provides an opportunity to update the advertisment data if desired.  
<br/>

<a name="client"></a>
# Client  

`NimBLERemoteCharacteristic::readValue(time_t\*, bool)`   
`NimBLERemoteDescriptor::readValue(bool)`  

Have been added as templates to allow reading the values as any specified type.   

**Example**
```
struct my_struct{
    uint8_t  one;
    uint16_t two;
    uint32_t four;
    uint64_t eight;
    float    flt;
}myStruct;

    time_t timestamp;
    myStruct = pRemoteCharacteristic->readValue<myStruct>(&timestamp); // timestamp optional
```  
<br/>

`NimBLERemoteCharacteristic::registerForNotify`  
Has been **deprecated** as now the internally stored characteristic value is updated when notification/indication is recieved.  

`NimBLERemoteCharacteristic::subscribe` and `NimBLERemoteCharacteristic::unsubscribe` have been implemented to replace it.  
A callback is no longer requred to get the most recent value unless timing is important. Instead, the application can call `NimBLERemoteCharacteristic::getValue` to  
get the last updated value any time.  

In addition `NimBLERemoteCharacteristic::readValue` and `NimBLERemoteCharacteristic::getValue` take an optional timestamp parameter which will update it's value with  
the time the last value was recieved.  

> NimBLEClient::getService  
> NimBLERemoteService::getCharacteristic  
> NimBLERemoteCharacteristic::getDescriptor  

These methods will now check the respective vectors for the attribute object and, if not found, will retrieve (only)  
the specified attribute from the peripheral.  

These changes allow more control for the user to manage the resources used for the attributes.    
<br/>

`NimBLEClient::connect()` can now be called without an address or advertised device parameter. This will connect to the  
device with the address previously set when last connected or set with `NimBLEDevice::setPeerAddress()`. 

<a name="general"></a>
# General  
To reduce resource use all instances of `std::map` have been replaced with `std::vector`.  

Use of `FreeRTOS::Semaphore` has been removed as it was consuming too much ram, the related files have been left in place to accomodate application use.  

Operators `==`, `!=` and `std::string` have been added to `NimBLEAddress` and `NimBLEUUID` for easier comparison and logging.  

New constructor for `NimBLEUUID(uint32_t, uint16_t, uint16_t, uint64_t)` added to lower memory use vs string construction. See: [#21](https://github.com/h2zero/NimBLE-Arduino/pull/21).   

Security/pairing operations are now handled in the respective `NimBLEClientCallbacks` and `NimBLEServerCallbacks` classes, `NimBLESecurity`(deprecated) remains for backward compatibility.  

Configuration options have been added to add or remove debugging information, when disabled (default) significatly reduces binary size.  
In ESP-IDF the options are in menuconfig: `Main menu -> ESP-NimBLE-cpp configuration`.  
For Arduino the options must be commented / uncommented in nimconfig.h.   
<br/>  

