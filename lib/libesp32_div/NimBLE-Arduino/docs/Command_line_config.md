# Arduino command line and platformio config options  

`CONFIG_BT_NIMBLE_MAX_CONNECTIONS`  

Sets the number of simultaneous connections (esp controller max is 9)  
- Default value is 3  
<br/>

`CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED`

Enable/disable storing the timestamp when an attribute value is updated  
This allows for checking the last update time using getTimeStamp() or getValue(time_t*)  
If disabled, the timestamp returned from these functions will be 0.  
Disabling timestamps will reduce the memory used for each value.  
1 = Enabled, 0 = Disabled; Default = Disabled  
<br/>

`CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH`

Set the default allocation size (bytes) for each attribute.  
If not specified when the constructor is called. This is also the size used when a remote  
characteristic or descriptor is constructed before a value is read/notifed.  
Increasing this will reduce reallocations but increase memory footprint.  
Default value is 20. Range: 1 : 512 (BLE_ATT_ATTR_MAX_LEN)  
 <br/>

`CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU`  

Sets the default MTU size.  
- Default value is 255  
<br/>

`CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME`  

Set the default device name  
- Default value is "nimble"  
<br/>

`CONFIG_BT_NIMBLE_DEBUG`   

If defined, enables debug log messages from the NimBLE host  
- Uses approx. 32kB of flash memory.  
<br/>

`CONFIG_NIMBLE_CPP_LOG_LEVEL`   

Define to set the debug log message level from the NimBLE CPP Wrapper.  
If not defined it will use the same value as the Arduino core debug level.  
Values: 0 = NONE, 1 = ERROR, 2 = WARNING, 3 = INFO, 4+ = DEBUG  
<br/>

`CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT`  

If defined, NimBLE host return codes will be printed as text in debug log messages.  
- Uses approx. 7kB of flash memory.  
<br/>

`CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT`  

If defined, GAP event codes will be printed as text in debug log messages.  
- Uses approx. 1kB of flash memory.  
<br/>

`CONFIG_NIMBLE_CPP_ENABLE_ADVERTISMENT_TYPE_TEXT`  

If defined, advertisment types will be printed as text while scanning in debug log messages.  
- Uses approx. 250 bytes of flash memory.  
<br/>

`CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE`  

Set the default appearance.  
- Default value is 0x00  
<br/>

`CONFIG_BT_NIMBLE_ROLE_CENTRAL_DISABLED`  
 
If defined, NimBLE Client functions will not be included.  
- Reduces flash size by approx. 7kB.  
<br/>

`CONFIG_BT_NIMBLE_ROLE_OBSERVER_DISABLED`  

If defined, NimBLE Scan functions will not be included.  
- Reduces flash size by approx. 26kB.  
<br/>

`CONFIG_BT_NIMBLE_ROLE_PERIPHERAL_DISABLED`  

If defined NimBLE Server functions will not be included.  
- Reduces flash size by approx. 16kB.  
<br/>

`CONFIG_BT_NIMBLE_ROLE_BROADCASTER_DISABLED`  

If defined, NimBLE Advertising functions will not be included.  
- Reduces flash size by approx. 5kB.  
<br/>

`CONFIG_BT_NIMBLE_MAX_BONDS`  

Sets the number of devices allowed to store/bond with  
- Default value is 3  
<br/>

`CONFIG_BT_NIMBLE_MAX_CCCDS`  

Sets the maximum number of CCCD subscriptions to store  
- Default value is 8  
<br/>

`CONFIG_BT_NIMBLE_RPA_TIMEOUT`  

Sets the random address refresh time in seconds.  
- Default value is 900  
<br/>

`CONFIG_BT_NIMBLE_MSYS1_BLOCK_COUNT`  

Set the number of msys blocks For prepare write & prepare responses. This may need to be increased if  
you are sending large blocks of data with a low MTU. E.g: 512 bytes with 23 MTU will fail.  
- Default value is 12  
<br/>

`CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL`  

Sets the NimBLE stack to use external PSRAM will be loaded  
- Must be defined with a value of 1; Default is CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL 1  
<br/>

`CONFIG_BT_NIMBLE_PINNED_TO_CORE`  

Sets the core the NimBLE host stack will run on   
- Options: 0 or 1
<br/>

`CONFIG_BT_NIMBLE_TASK_STACK_SIZE`  

Set the task stack size for the NimBLE core.  
- Default is 4096  
<br/>

