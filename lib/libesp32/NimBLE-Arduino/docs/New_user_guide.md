# New User Guide

**Note:** If you are migrating an existing project from the original Bluedroid library please see the [Migration Guide.](Migration_guide.md)  

If you are a new user this will guide you through a simple server and client application.  

* [Creating a Server](#creating-a-server)
* [Creating a Client](#creating-a-client)  
<br/>

## Include Files
At the top of your application file add `#include NimBLEDevice.h`, this is the only header required and provides access to all classes.  
<br/>

## Using the Library
In order to perform any BLE tasks you must first initialize the library, this prepares the NimBLE stack to be ready for commands.  

To do this you must call `NimBLEDevice::init("your device name here")`, the parameter passed is a character string containing the name you want to advertise.  
If you're not creating a server or do not want to advertise a name, simply pass an empty string for the parameter.  

This can be called any time you wish to use BLE functions and does not need to be called from app_main(IDF) or setup(Arduino) but usually is.  
<br/>

<a name="creating-a-server"></a>
## Creating a Server
BLE servers perform 2 tasks, they advertise their existance for clients to find them and they provide services which contain information for the connecting client.  

After initializing the NimBLE stack we create a server by calling `NimBLEDevice::createServer()`, this will create a server instance and return a pointer to it.  

Once we have created the server we need to tell it the services it hosts.  
To do this we call `NimBLEServer::createService(const char* uuid)`. Which returns a pointer to an instance of `NimBLEService`.  
The `uuid` parameter is a hexadecimal string with the uuid we want to give the service, it can be 16, 32, or 128 bits.  

For this example we will keep it simple and use a 16 bit value: ABCD.  
<br/>

**Example code:**  
```
#include "NimBLEDevice.h"

// void setup() in Arduino
void app_main(void)  
{
    NimBLEDevice::init("NimBLE");
    
    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService("ABCD");
}
```

Now we have NimBLE initialized, a server created and a service assigned to it.  
We can't do much with this yet so now we should add a characteristic to the service to provide some data.  

Next we call `NimBLEService::createCharacteristic` which returns a pointer to an instance of `NimBLECharacteristic`, and takes two parameters: 
A `uuid` to specify the UUID of the characteristic and a bitmask of the properties we want applied to it.  

Just as with the service UUID we will use a simple 16 bit value: 1234.  
The properties bitmask is a little more involved. It is a combination of NIMBLE_PROPERTY:: values.  

Here is the list of options:  
> NIMBLE_PROPERTY\::READ  
> NIMBLE_PROPERTY\::READ_ENC  
> NIMBLE_PROPERTY\::READ_AUTHEN  
> NIMBLE_PROPERTY\::READ_AUTHOR  
> NIMBLE_PROPERTY\::WRITE  
> NIMBLE_PROPERTY\::WRITE_NR  
> NIMBLE_PROPERTY\::WRITE_ENC  
> NIMBLE_PROPERTY\::WRITE_AUTHEN  
> NIMBLE_PROPERTY\::WRITE_AUTHOR  
> NIMBLE_PROPERTY\::BROADCAST  
> NIMBLE_PROPERTY\::NOTIFY  
> NIMBLE_PROPERTY\::INDICATE  

For this example we won't need to specify these as the default value is `NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE`  
which will allow reading and writing values to the characteristic without encryption or security.  
The function call will simply be `pService->createCharacteristic("1234");`  
<br/>

**Our example code now is:**  
```
#include "NimBLEDevice.h"

// void setup() in Arduino
void app_main(void)
{
    NimBLEDevice::init("NimBLE");
    
    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService("ABCD");
    NimBLECharacteristic *pCharacteristic = pService->createCharacteristic("1234");
}
```

All that's left to do now is start the sevice, give the characteristic a value and start advertising for clients.  

Fist we start the service by calling `NimBLEService::start()`.

Next we need to call `NimBLECharacteristic::setValue` to set the characteristic value that the client will read.  
There are many different types you can send as parameters for the value but for this example we will use a simple string.
`pCharacteristic->setValue("Hello BLE");`  

Next we need to advertise for connections.  
To do this we create an instance of `NimBLEAdvertising` add our service to it (optional) and start advertisng.  

**The code for this will be:**  
```
NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising(); // create advertising instance
pAdvertising->addServiceUUID("ABCD"); // tell advertising the UUID of our service
pAdvertising->start(); // start advertising
```
That's it, this will be enough to create a BLE server with a service and a characteristic and advertise for client connections.  

**The full example code:**  
```
#include "NimBLEDevice.h"

// void setup() in Arduino
void app_main(void)
{
    NimBLEDevice::init("NimBLE");
    
    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService("ABCD");
    NimBLECharacteristic *pCharacteristic = pService->createCharacteristic("1234");
    
    pService->start();
    pCharacteristic->setValue("Hello BLE");
    
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("ABCD"); 
    pAdvertising->start(); 
}
```

Now if you scan with your phone using nRFConnect or any other BLE app you should see a device named "NimBLE" with a service of "ABCD".  

For more advanced features and options please see the server examples in the examples folder.  
<br/>

<a name="creating-a-client"></a>
## Creating a Client 

BLE clients perform 2 tasks, they scan for advertising servers and form connections to them to read and write to their characteristics/descriptors.

After initializing the NimBLE stack we create a scan instance by calling `NimBLEDevice::getScan()`, this will create a `NimBLEScan` instance and return a pointer to it.  

Once we have created the scan we can start looking for advertising servers.  

To do this we call `NimBLEScan::start(duration)`, the duration parameter is a uint32_t that specifies the number of seconds to scan for,  
passing 0 will scan forever.  

In this example we will scan for 10 seconds. This is a blocking function (a non blocking overload is also available).  
This call returns an instance of `NimBLEScanResults` when the scan completes which can be parsed for advertisers we are interested in.  

**Example Code:**  
```
#include "NimBLEDevice.h"

// void setup() in Arduino
void app_main(void)  
{
    NimBLEDevice::init("");
    
    NimBLEScan *pScan = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->start(10);
}
```
<br/>

Now that we have scanned we need to check the results for any advertisers we are interested in connecting to.  

To do this we iterate through the results and check if any of the devices found are advertising the service we want `ABCD`.  
Each result in `NimBLEScanResults` is a `NimBLEAdvertisedDevice` instance that we can access data from.

We will check each device found for the `ABCD` service by calling `NimBLEAdvertisedDevice::isAdvertisingService`.  
This takes an instance of `NimBLEUUID` as a parameter so we will need to create one.  

**The code for this looks like:**
```
NimBLEUUID serviceUuid("ABCD");

for(int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    
    if (device.isAdvertisingService(serviceUuid)) {
    // create a client and connect
    }
}
```
<br/>

Now that we can scan and parse advertisers we need to be able to create a `NimBLEClient` instance and use it to connect.  

To do this we call `NimBLEDevice::createClient` which creates the `NimBLEClient` instance and returns a pointer to it.  

After this we call `NimBLEClient::connect` to connect to the advertiser.  
This takes a pointer to the `NimBLEAdvertisedDevice` and returns `true` if successful.

**Lets do that now:**
```
NimBLEUUID serviceUuid("ABCD");

for(int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    
    if (device.isAdvertisingService(serviceUuid)) {
        NimBLEClient *pClient = NimBLEDevice::createClient();
        
        if(pClient->connect(&device)) {
        //success
        } else {
        // failed to connect
        }
    }
}
```
As shown, the call to `NimBLEClient::connect` should have it's eturn value tested to make sure it succeeded before proceeding to get data.  
<br/>

Next we need to access the servers data by asking it for the service and the characteristic we are interested in, then read the characteristic value. 

To do this we call `NimBLEClient::getService`, which takes as a parameter the UUID of the service and returns  
a pointer an instance to `NimBLERemoteService` or `nullptr` if the service was not found.  

Next we will call `NimBLERemoteService::getCharateristic` which takes as a parameter the UUID of the service and returns  
a pointer to an instance of `NimBLERemoteCharacteristic` or `nullptr` if not found.  

Finally we will read the characteristic value with `NimBLERemoteCharacteristic::readValue()`.  

**Here is what that looks like:**
```
NimBLEUUID serviceUuid("ABCD");

for(int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    
    if (device.isAdvertisingService(serviceUuid)) {
        NimBLEClient *pClient = NimBLEDevice::createClient();
        
        if (pClient->connect(&device)) {
            NimBLERemoteService *pService = pClient->getService(serviceUuid);
            
            if (pService != nullptr) {
                NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic("1234");
                
                if (pCharacteristic != nullptr) {
                    std::string value = pCharacteristic->readValue();
                    // print or do whatever you need with the value
                }
            }
        } else {
        // failed to connect
        }
    }
}
```
<br/>

The last thing we should do is clean up once we are done with the connection.  
Because multiple clients are supported and can be created we should delete them when finished with them to conserve resources.  
This is done by calling `NimBLEDevice::deleteClient`.

**Lets add that now:**
```
NimBLEUUID serviceUuid("ABCD");

for(int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    
    if (device.isAdvertisingService(serviceUuid)) {
        NimBLEClient *pClient = NimBLEDevice::createClient();
        
        if (pClient->connect(&device)) {
            NimBLERemoteService *pService = pClient->getService(serviceUuid);
            
            if (pService != nullptr) {
                NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic("1234");
                
                if (pCharacteristic != nullptr) {
                    std::string value = pCharacteristic->readValue();
                    // print or do whatever you need with the value
                }
            }
        } else {
        // failed to connect
        }
        
        NimBLEDevice::deleteClient(pClient);
    }
}
```
Note that there is no need to disconnect as that will be done when deleting the client instance.  
<br/>

**Here is the full example code:**  
```
#include "NimBLEDevice.h"

// void setup() in Arduino
void app_main(void)  
{
    NimBLEDevice::init("");
    
    NimBLEScan *pScan = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->start(10);
    
    NimBLEUUID serviceUuid("ABCD");
    
    for(int i = 0; i < results.getCount(); i++) {
        NimBLEAdvertisedDevice device = results.getDevice(i);
        
        if (device.isAdvertisingService(serviceUuid)) {
            NimBLEClient *pClient = NimBLEDevice::createClient();
            
            if (pClient->connect(&device)) {
                NimBLERemoteService *pService = pClient->getService(serviceUuid);
                
                if (pService != nullptr) {
                    NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic("1234");
                    
                    if (pCharacteristic != nullptr) {
                        std::string value = pCharacteristic->readValue();
                        // print or do whatever you need with the value
                    }
                }
            } else {
            // failed to connect
            }
            
            NimBLEDevice::deleteClient(pClient);
        }
    }
}
```
<br/>

For more advanced features and options please see the client examples in the examples folder.  
<br/>

