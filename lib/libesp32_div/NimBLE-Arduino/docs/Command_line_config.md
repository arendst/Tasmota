# Arduino command line and platformio config options  

`CONFIG_BT_NIMBLE_MAX_CONNECTIONS`  

Sets the number of simultaneous connections (esp controller max is 9)  
- Default value is 3  
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

