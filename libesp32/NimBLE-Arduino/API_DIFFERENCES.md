# Server API differnces:

### Characteristics:
When creating a characteristic the properties are now set with `NIMBLE_PROPERTY::XXXX` instead of `BLECharacteristic::XXXX`.

#### Previous:
```
BLECharacteristic::PROPERTY_READ |
BLECharacteristic::PROPERTY_WRITE
```

#### Changed to:
```
NIMBLE_PROPERTY::READ | 
NIMBLE_PROPERTY::WRITE
```    

#### The full list of properties:
```
NIMBLE_PROPERTY::READ
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
```

### Descriptors:
Descriptors are now created using the NimBLEcharacteristic method `createDescriptor()`.
 
The previous method `addDescriptor()` is now a private function in the library.

This was done because the NimBLE host automatically creates a 0x2902 descriptor if a characteristic has notify or indicate properties applied.   
Due to this fact, this library also creates one automatically for your application.    
The only reason to manually create this descriptor now is to assign callback functions.   
If you do not require this functionality you can safely exclude the manual creation of that descriptor.   


For any other descriptor, (except 0x2904, see below) it should now be created just as characteristics are    
by invoking the `NimBLECharacteristic::createDescriptor` methods.   
Which are defined as:
```
NimBLEDescriptor* createDescriptor(const char* uuid, 
						uint32_t properties = NIMBLE_PROPERTY::READ | 
											  NIMBLE_PROPERTY::WRITE,
                                     uint16_t max_len = 100);
                                     
NimBLEDescriptor* createDescriptor(NimBLEUUID uuid,
						uint32_t properties = NIMBLE_PROPERTY::READ | 
											  NIMBLE_PROPERTY::WRITE,
                                     uint16_t max_len = 100);
```
##### Example:
```
pDescriptor = pCharacteristic->createDescriptor("ABCD", 
                            NIMBLE_PROPERTY::READ | 
                            NIMBLE_PROPERTY::WRITE |
                            NIMBLE_PROPERTY::WRITE_ENC,
                            25);`
```
Would create a descriptor with the UUID 0xABCD, publicly readable but only writable if paired/bonded (encrypted) and has a max value length of 25 bytes.

For the 0x2904 descriptor, there is a special class that is created when you call `createDescriptor("2904")`.

The pointer returned is of the base class `NimBLEDescriptor` but the call will create the derived class of `NimBLE2904` so you must cast the returned pointer to `NimBLE2904*` to access the specific class methods.

##### Example:
```
p2904 = (NimBLE2904*)pCharacteristic->createDescriptor("2904");
```

#### Server Security:
Security is set on the characteristic or descriptor properties by applying one of the following:
```
NIMBLE_PROPERTY::READ_ENC
NIMBLE_PROPERTY::READ_AUTHEN
NIMBLE_PROPERTY::READ_AUTHOR
NIMBLE_PROPERTY::WRITE_ENC
NIMBLE_PROPERTY::WRITE_AUTHEN
NIMBLE_PROPERTY::WRITE_AUTHOR
```
When a peer wants to read or write a characteristic or descriptor with any of these properties applied    
it will trigger the pairing process. By default the "just-works" pairing will be performed automatically.   
This can be changed to use passkey authentication or numeric confirmation. See below for details.


# Client API Differences:
The `BLEAdvertisedDeviceCallbacks` class `onResult()` method now receives a pointer to the    
`NimBLEAdvertisedDevice` object instead of a copy.

`NimBLEClient::connect()` now takes an extra parameter to indicate if the client should download the services   
 database from the peripheral, default value is true. 

Defined as:
```
bool connect(NimBLEAdvertisedDevice* device, bool refreshServices = true);
bool connect(NimBLEAddress address, uint8_t type = BLE_ADDR_TYPE_PUBLIC, bool refreshServices = true);
```
If set to false the client will use the services database it retrieved from the peripheral last time it connected.    
This allows for faster connections and power saving if the devices just dropped connection and want to reconnect.

```
NimBLERemoteCharacteristic::writeValue();
NimBLERemoteCharacteristic::registerForNotify();
```
Now return true or false to indicate success or failure so you can choose to disconnect or try again.

#### Client Security:
The client will automatically initiate security when the peripheral responds that it's required.    
The default configuration will use "just-works" pairing with no bonding, if you wish to enable bonding see below.


# Security:
Security callback functions are now incorporated in the client/server Callbacks class.   
However backward compatibility with the `BLESecurity` class is retained to minimize app code changes.

The relevant server callbacks are defined as:
```
bool    onConfirmPIN(uint32_t pin); // accept or reject the passkey
void    onAuthenticationComplete(ble_gap_conn_desc* desc); // auth complete - details in desc 
bool    onPassKeyNotify(uint32_t pass_key); // receive the passkey sent by the client, accept or reject
```
The relevant client callbacks are defined as:
```
bool     onConfirmPIN(uint32_t pin); // accept or reject the passkey
void     onAuthenticationComplete(ble_gap_conn_desc* desc); // auth complete - details in desc 
uint32_t onPassKeyRequest(); // return the passkey to send to the server
```

Security settings and IO capabilities are now set by the corresponding method of `NimBLEDevice::`.
```
static void             setSecurityAuth(bool bonding, bool mitm, bool sc);
static void             setSecurityAuth(uint8_t auth_req);
static void             setSecurityIOCap(uint8_t iocap);
static void             setSecurityInitKey(uint8_t init_key);
static void             setSecurityRespKey(uint8_t init_key);


/**
 * @brief Set the authorization mode for this device.
 * @param bonding, if true we allow bonding, false no bonding will be performed.
 * @param mitm, if true we are capable of man in the middle protection, false if not.
 * @param sc, if true we will perform secure connection pairing, false we will use legacy pairing.
 */   
void NimBLEDevice::setSecurityAuth(bool bonding, bool mitm, bool sc) 



/**
 * @brief Set the authorization mode for this device.
 * @param A bitmap indicating what modes are supported.
 * The bits are defined as follows:
 ** 0x01 BLE_SM_PAIR_AUTHREQ_BOND                
 ** 0x04 BLE_SM_PAIR_AUTHREQ_MITM                
 ** 0x08 BLE_SM_PAIR_AUTHREQ_SC                  
 ** 0x10 BLE_SM_PAIR_AUTHREQ_KEYPRESS  - not yet supported.         
 ** 0xe2 BLE_SM_PAIR_AUTHREQ_RESERVED  - for reference only.
 */ 
void NimBLEDevice::setSecurityAuth(uint8_t auth_req) 



/**
 * @brief Set the Input/Output capabilities of this device.
 * @param One of the following:
 ** 0x00 BLE_HS_IO_DISPLAY_ONLY         DisplayOnly IO capability
 ** 0x01 BLE_HS_IO_DISPLAY_YESNO        DisplayYesNo IO capability
 ** 0x02 BLE_HS_IO_KEYBOARD_ONLY        KeyboardOnly IO capability 
 ** 0x03 BLE_HS_IO_NO_INPUT_OUTPUT      NoInputNoOutput IO capability 
 ** 0x04 BLE_HS_IO_KEYBOARD_DISPLAY     KeyboardDisplay Only IO capability
 */
void NimBLEDevice::setSecurityIOCap(uint8_t iocap) 



/**
 * @brief If we are the initiator of the security procedure this sets the keys we will distribute.
 * @param A bitmap indicating which keys to distribute during pairing.
 * The bits are defined as follows:           
 ** 0x01: BLE_SM_PAIR_KEY_DIST_ENC  - Distribute the encryption key.
 ** 0x02: BLE_SM_PAIR_KEY_DIST_ID   - Distribute the ID key (IRK).
 ** 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 ** 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
void NimBLEDevice::setSecurityInitKey(uint8_t init_key) 


/**
 * @brief Set the keys we are willing to accept during pairing.
 * @param A bitmap indicating which keys to accept during pairing.
 * The bits are defined as follows:           
 ** 0x01: BLE_SM_PAIR_KEY_DIST_ENC  -  Accept the encryption key.
 ** 0x02: BLE_SM_PAIR_KEY_DIST_ID   -  Accept the ID key (IRK).
 ** 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 ** 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
void NimBLEDevice::setSecurityRespKey(uint8_t init_key) 
```

 I'm sure there are more things I have forgotten but this is all the majors.   
 I will update this document as necessary.
