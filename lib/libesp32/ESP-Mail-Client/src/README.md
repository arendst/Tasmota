# ESP Mail Client Arduino Library


The description of the available functions in the current reease are shown below.


## Global functions


#### Sending Email through the SMTP server.

param **`smtp`** The pointer to SMTP session object which holds the data and the TCP client.

param **`msg`** The pointer to SMTP_Message class which contains the header, body, and attachments.

param **`closeSession`** The option to Close the SMTP session after sent.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool sendMail(SMTPSession *smtp, SMTP_Message *msg, bool closeSession = true);
```



#### Append message to the mailbox

param **`imap`** The pointer to IMAP sesssion object which holds the data and the TCP client.

param **`msg`** The pointer to SMTP_Message class which contains the header, body, and attachments.

param **`lastAppend`** The last message to append (optional). In case MULTIAPPEND extension 

is supported, set this to false will append messages in single APPEND command.

param **`flags`** The flags to set to this message.

 param **`dateTime`** The date/time to set to this message (optional).

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool appendMessage(IMAPSession *imap, SMTP_Message *msg, T flags = "");
```


#### Reading Email through IMAP server.

param **`imap`** The pointer to IMAP sesssion object which holds the data and the TCP client.

param **`closeSession`** The option to close the IMAP session after fetching or searching the Email.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool readMail(IMAPSession *imap, bool closeSession = true);
```





#### Set the argument to the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`msgUID`** The UID of the message.

param **`flags`** The flag list to set.

param **`closeSession`** The option to close the IMAP session after set flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool setFlag(IMAPSession *imap, int msgUID, <string> flags, bool closeSession, bool silent = false, int32_t modsequence = -1);
```





#### Set the argument to the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`flags`** The flag list to set.

param **`closeSession`** The option to close the IMAP session after set flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool setFlag(IMAPSession *imap, <string> sequenceSet, bool UID, <string> flags, bool closeSession, bool silent = false, int32_t modsequence = -1);
```





#### Add the argument to the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`msgUID`** The UID of the message.

param **`flags`** The flag list to add.

param **`closeSession`** The option to close the IMAP session after add flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool addFlag(IMAPSession *imap, int msgUID, <string> flags, bool closeSession, int32_t modsequence = -1);
```





#### Add the argument to the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`flags`** The flag list to add.

param **`closeSession`** The option to close the IMAP session after set flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.
 
```cpp
bool addFlag(IMAPSession *imap, <string> sequenceSet, bool UID, <string> flags, bool closeSession, bool silent = false, int32_t modsequence = -1);
```





#### Remove the argument from the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`msgUID`** The UID of the message that flags to be removed.

param **`flags`** The flag list to remove.

param **`closeSession`** The option to close the IMAP session after remove flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool removeFlag(IMAPSession *imap, int msgUID, <string> flags, bool closeSession, int32_t modsequence = -1);
```





#### Remove the argument from the Flags for the specified message.

param **`imap`** The pointer to IMAP session object which holds the data and the TCP client.

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`flags`** The flag list to remove.

param **`closeSession`** The option to close the IMAP session after set flag.

param **`silent`** The option to ignore the response.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.
 
```cpp
bool removeFlag(IMAPSession *imap, <string> sequenceSet, bool UID, <string> flags, bool closeSession, bool silent = false, int32_t modsequence = -1);
```






#### Reconnect WiFi or network if lost connection.

param **`reconnect`** The boolean to set/unset WiFi AP reconnection.

```cpp
void networkReconnect(bool reconnect);
```




#### Initiate SD card with SPI port configuration.

param **`ss`** The SPI Chip/Slave Select pin.

param **`sck`** The SPI Clock pin.

param **`miso`** The SPI MISO pin.

param **`mosi`** The SPI MOSI pin.

aram **`frequency`** The SPI frequency.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool sdBegin(int8_t ss = -1, int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, uint32_t frequency = 4000000);
```





#### Initiate SD card with SD FS configurations (ESP8266 only).

param **`ss`** SPI Chip/Slave Select pin.

param **`sdFSConfig`** The pointer to SDFSConfig object (ESP8266 only).

return **`boolean`** type status indicates the success of the operation.

```cpp
  bool sdBegin(SDFSConfig *sdFSConfig);
```





#### Initiate SD card with chip select and SPI configuration (ESP32 only).

param **`ss`** The SPI Chip/Slave Select pin.

param **`spiConfig`** The pointer to SPIClass object for SPI configuartion.

param **`frequency`** The SPI frequency.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool sdBegin(int8_t ss, SPIClass *spiConfig = nullptr, uint32_t frequency = 4000000);
```





#### Initiate SD card with SdFat SPI and pins configurations (with SdFat included only).

param **`sdFatSPIConfig`** The pointer to SdSpiConfig object for SdFat SPI configuration.

param **`ss`** The SPI Chip/Slave Select pin.

param **`sck`** The SPI Clock pin.

param **`miso`** The SPI MISO pin.

param **`mosi`** The SPI MOSI pin.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
 bool sdBegin(SdSpiConfig *sdFatSPIConfig, int8_t ss = -1, int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1);
```





#### Initiate SD card with SdFat SDIO configuration (with SdFat included only).

param **`sdFatSDIOConfig`** The pointer to SdioConfig object for SdFat SDIO configuration.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
 bool sdBegin(SdioConfig *sdFatSDIOConfig);
```




#### Initialize the SD_MMC card (ESP32 only).

param **`mountpoint`** The mounting point.

param **`mode1bit`** Allow 1 bit data line (SPI mode).

param **`format_if_mount_failed`** Format SD_MMC card if mount failed.

return **`Boolean`** type status indicates the success of the operation.

```cpp
bool sdMMCBegin(const char *mountpoint = "/sdcard", bool mode1bit = false, bool format_if_mount_failed = false);
```




#### Get free Heap memory.

return **`int`** Free memory amount in byte.

```cpp
int getFreeHeap();
```





## IMAPSession class functions


The following functions are available from the IMAP Session class.

This class used for controlling IMAP transports and retrieving the data from the IMAP server.




#### Set the tcp timeout.

param **`timeoutSec`** The tcp timeout in seconds.

```cpp
void setTCPTimeout(unsigned long timeoutSec);
```



#### Assign custom Client from Arduino Clients.

param **`client`** The pointer to Arduino Client derived class e.g. WiFiClient, WiFiClientSecure, EthernetClient or GSMClient.

```cpp
void setClient(Client *client);
```


#### Assign TinyGsm Clients.

param **`client`** The pointer to TinyGsmClient.

param **`modem`** The pointer to TinyGsm modem object. Modem should be initialized and/or set mode before transfering data.

param **`pin`** The SIM pin.

param **`apn`** The GPRS APN (Access Point Name).

param **`user`** The GPRS user.

param **`password`** The GPRS password.

```cpp
void setGSMClient(Client *client, void *modem, const char *pin, const char *apn, const char *user, const char *password);
```


#### Assign the callback function to handle the network connection for custom Client.

param **`networkConnectionCB`** The function that handles the network connection.

```cpp
void networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB);
```



#### Assign the callback function to handle the network connection status acknowledgement.

param **`networkStatusCB`** The function that handle the network connection status acknowledgement.

```cpp
void networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB);
```



#### Set the network status acknowledgement.

param **`status`** The network status.

```cpp
void setNetworkStatus(bool status);
```



#### Set the BearSSL IO buffer size.

param **`rx`** The BearSSL receive buffer size in bytes.

param **`tx`** The BearSSL trasmit buffer size in bytes.

```cpp
void setSSLBufferSize(int rx = -1, int tx = -1);
```



#### Set system time with timestamp.

param **`ts`** timestamp in seconds from midnight Jan 1, 1970.

param **`gmtOffset`** The GMT offset.

This function allows the internal time setting by timestamp i.e. timestamp from external RTC. 

```cpp
void setSystemTime(time_t ts, float gmtOffset = 0);
```



#### Begin the IMAP server connection.

param **`session_config`** The pointer to Session_Config structured data that keeps the server and log in details.

param **`imap_data`** The pointer to IMAP_Data structured data that keeps the operation options.

param **`login`** The bool option for login after server connection.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool connect(Session_Config *session_config, IMAP_Data *imap_data, bool login = true);
```



#### Login to IMAP server using Email and password.

param **`email`** The IMAP server account email.

param **`password`** The IMAP server account password.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bbool loginWithPassword(<string> email, <string> password);
```



#### Login to IMAP server using Email and access token.

param **`email`** The IMAP server account email.

param **`token`** The Access token to log in.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bbool loginWithAccessToken(<string> email, <string> token);
```


#### Send the client identification to the server

param **`identification`** The pointer to IMAP_Identification structured data that keeps

the key properties e.g., name, version, os, os_version, vendor, support_url, address,

date, command, arguments, and environment.

```cpp
bool id(IMAP_Identification *identification);
```


#### Return the server ID returns from ID command.

return **`The server ID string.`**

```cpp
String serverID();
```


#### Return the SASL authentication status.

return **`boolean`** The boolean value indicates SASL authentication status.

```cpp
bool isAuthenticated();
```


#### Return the log in status.

return **`boolean`** The boolean value indicates log in status.

```cpp
bool isLoggedIn();
```


#### Return firmware update result when attachment filename matches.

return **`boolean`** The boolean value indicates the firmware update status.

```cpp
bool isFirmwareUpdateSuccess();
```


#### Close the IMAP session.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool closeSession();
```


#### Setup TCP KeepAlive for internal TCP client.

param **`tcpKeepIdleSeconds`** lwIP TCP Keepalive idle in seconds.

param **`tcpKeepIntervalSeconds`** lwIP TCP Keepalive interval in seconds.

param **`tcpKeepCount`** lwIP TCP Keepalive count.

For the TCP (KeepAlive) options, see [this doc](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#tcp-options).

If value of one of these parameters is zero, the TCP KeepAlive will be disabled.

You can check the server connecting status, by exexuting `<IMAPSession>.connected()` which will return true when connection to the server is still alive. 

```cpp
void keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount);
```



#### Get TCP KeepAlive status.

return **`Boolean`** status of TCP Keepalive.

```cpp
bool isKeepAlive();
```



#### Get TCP connection status.

return **`boolean`** The boolean value indicates the connection status.

```cpp
bool connected();
```






#### Set to enable the debug.

param **`level`** The level to enable the debug message

level = 0, no debugging

level = 1, basic level debugging

```cpp
void debug(int level);
```





#### Get the list of all the mailbox folders since the TCP session was opened and user was authenticated.

param **`folders`** The FoldersCollection class that contains the collection of the 
FolderInfo structured data.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getFolders(FoldersCollection &folders);
```





#### Select or open the mailbox folder to search or fetch the message inside.

param **`folderName`** The known mailbox folder name. The default name is INBOX.

param **`readOnly`** The option to open the mailbox for read only. Set this option to false when you wish 
to modify the Flags using the setFlag, addFlag and removeFlag functions.

return **`boolean`** The boolean value which indicates the success of operation.

note: the function will exit immediately and return true if the time since previous success folder selection (open) with the same readOnly mode, is less than 5 seconds.

```cpp
bool selectFolder(<string> folderName, bool readOnly = true);
```





#### Open the mailbox folder to read or search the mesages. 

param **`folderName`** The name of known mailbox folder to be opened.

param **`readOnly`** The option to open the mailbox for reading only. Set this option to false when you wish 
to modify the flags using the setFlag, addFlag and removeFlag functions.

return **`boolean`** The boolean value which indicates the success of operation.

note: the function will exit immediately and return true if the time since previous success folder selection (open) with the same readOnly mode, is less than 5 seconds.

```cpp
bool openFolder(<string> folderName, bool readOnly = true);
```





#### Close the mailbox folder that was opened. 

param **`expunge`**  The option to allow emty the deleted flag set messages in case folder was open with editable mode.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool closeFolder(bool expunge = false);
```






#### Create folder. 

param **`folderName`** The name of folder to create.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool createFolder(<string> folderName);
```





#### Get subscribes mailboxes. 

param **`reference`** The reference name.

param **`mailbox`** The mailbox name with possible wildcards.

param **`folders`** The return FoldersCollection that contains the folder info e.g., name, attribute and delimiter.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getSubscribesMailboxes(<string> reference, <string> mailbox, FoldersCollection &folders);
```




#### Subscribe mailbox.

param **`folderName`** The name of folder to subscribe.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool subscribe(<string> folderName)
```




#### Unsubscribe mailbox.

param **`folderName`** The name of folder to unsubscribe.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool unSubscribe(<string> folderName)
```





#### Rename folder. 

param **`currentFolderName`** The name of folder to create.

param **`newFolderName`** The new name of folder to create.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool renameFolder(<string> currentFolderName, <string> newFolderName);
```



#### Delete folder. 

param **`folderName`** The name of folder to delete..

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool deleteFolder(<string> folderName);
```






#### Get UID number in selected or opened mailbox. 

param **`msgNum`** The message number or order in the total message numbers.

return **`boolean`** The boolean value which indicates the success of operation.

Returns 0 when fail to get UID.

```cpp
int getUID(int msgNum);
```







#### Get message flags in selected or opened mailbox. 

param **`msgNum`** The message number or order in the total message numbers.

return **`string`** Message flags in selected or opened mailbox.

empty string when fail to get flags.

```cpp
const char *getFlags(int msgNum);
```






#### Send the custom IMAP command and get the result via callback.

param **`cmd`** The command string.

param **`callback`** The function that accepts IMAP_Response as parameter.

param **`tag`** The tag string to pass to the callback function.

return **`boolean`** The boolean value which indicates the success of operation.

imap.connect and imap.selectFolder or imap.openFolder are needed to call once prior to call this function.

```cpp
bool sendCustomCommand(<string> cmd, imapResponseCallback callback, <string> tag);
```





#### Send the custom IMAP command data string.

param **`data`** The string data.

param **`lastData`** The flag represents the last data to send (optional).

return **`boolean`** The boolean value which indicates the success of operation.

Should be used after calling sendCustomCommand("APPEND xxxxxx");

```cpp
bool sendCustomData(T data, bool lastData = false);
```





#### Send the custom IMAP command data.

param **`data`** The byte data.

param **`size`** The data size.

param **`lastData`** The flag represents the last data to send (optional).

return **`boolean`** The boolean value which indicates the success of operation.

Should be used after calling ssendCustomCommand("APPEND xxxxxx");

```cpp
bool sendCustomData(uint8_t *data, size_t size, bool lastData = false);
```




#### Begin the IMAP server connection without authentication.

param **`session_config`** The pointer to Session_Config structured data that keeps the server and log in details.

param **`callback`** The callback function that accepts IMAP_Response as parameter.

param **`tag`** The tag that pass to the callback function.

return **`The boolean`** value indicates the success of operation.

```cpp
bool customConnect(Session_Config *session_config, imapResponseCallback callback, <string> tag);
```




#### Copy the messages to the defined mailbox folder. 

param **`toCopy`** The pointer to the MessageList class that contains the list of messages to copy.

param **`dest`** The destination folder that the messages to copy to.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool copyMessages(MessageList *toCopy, <string> dest);
```





#### Copy the messages to the defined mailbox folder. 

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`dest`** The destination folder that the messages to copy to.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool copyMessages(<string> sequenceSet, bool UID, <string> dest);
```






#### Move the messages to the defined mailbox folder. 

param **`toMove`** The pointer to the MessageList class that contains the list of messages to move.

param **`dest`** The destination folder that the messages to move to.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool moveMessages(MessageList *toMove, <string> dest);
```





#### Move the messages to the defined mailbox folder. 

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`dest`** The destination folder that the messages to move to.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool moveMessages(<string> sequenceSet, bool UID, <string> dest);
```






#### Delete the messages in the opened mailbox folder. 

param **`toDelete`** The pointer to the MessageList class that contains the list of messages to delete.

param **`expunge`** The boolean option to expunge all messages.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool deleteMessages(MessageList *toDelete, bool expunge = false, int32_t modsequence = -1);
```






#### Delete the messages in the opened mailbox folder. 

param **`sequenceSet`** The sequence set string i.g., unique identifier (UID) or 
message sequence number or ranges of UID or sequence number.

param **`UID`** The option for sequenceSet whether it is UID or message sequence number.

param **`expunge`** The boolean option to expunge all messages.

param **`modsequence`** The int32_t option for UNCHANGESINCE conditional test.

return **`boolean`** The boolean value indicates the success of operation.

The modsequence value can be used only if IMAP server supports Conditional STORE extension and the selected mailbox supports modsequences.

```cpp
bool deleteMessages(<string> sequenceSet, bool UID, bool expunge = false, int32_t modsequence = -1);
```






#### Get the quota root's resource usage and limits.

param **`quotaRoot`** The quota root to get.

param **`info`** The pointer to `IMAP_Quota_Root_Info` that contains quota root's resource name, usage and limit.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getQuota(<string> quotaRoot, IMAP_Quota_Root_Info *info);
```





#### Set the quota root's resource usage and limits.

param **`quotaRoot`** The quota root to set.

param **`data`** The pointer to `IMAP_Quota_Root_Info` that contains quota root's resource name, usage and limit.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool setQuota(<string> quotaRoot, IMAP_Quota_Root_Info *data);
```





#### Get the list of quota roots for the named mailbox.

param **`mailbox`** The mailbox name.

param **`quotaRootsList`** The pointer to `IMAP_Quota_Roots_List` that contains the list of `IMAP_Quota_Root_Info`.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getQuotaRoot(<string> mailbox, IMAP_Quota_Roots_List *quotaRootsList);
```





#### Get the ACLs for a mailbox.

param **`mailbox`** The mailbox name.

param **`aclList`** The pointer to the returning `IMAP_Rights_List` object.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getACL(<string> mailbox, IMAP_Rights_List *aclList);
```






#### Get the ACLs for a mailbox.

param **`mailbox`** The mailbox name.

param **`acl`** The pointer to the acl IMAP_Rights_Info to set.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool setACL(<string> mailbox, IMAP_Rights_Info *acl);
```






#### Delete the ACLs set for identifier on mailbox.

param **`mailbox`** The mailbox name.

param **`identifier`** The identifier (user) to remove the rights.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool deleteACL(<string> mailbox, <string> identifier);
```





#### Show my ACLs for a mailbox.

param **`mailbox`** The mailbox name.

param **`acl`** The pointer to the returning `IMAP_Rights_Info` object.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool myRights(<string> mailbox, IMAP_Rights_Info *acl);
```





#### Returns IMAP namespaces.

param **`mailbox`** The mailbox name.

param **`ns`** The pointer to the returning `IMAP_Namespaces_List` object.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool getNamespace(IMAP_Namespaces_List *ns);
```






#### Enable IMAP capability.

param **`capability`** The mailbox name.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool enable(<string> capability);
```





#### Listen for the selected or open mailbox for updates.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool listen();
```






#### Stop listen for the mailbox for updates.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool stopListen();
```







#### Check for the selected or open mailbox updates.

return **`boolean`** The boolean value which indicates the changes status of mailbox.

```cpp
bool folderChanged();
```






#### Assign the callback function that returns the operating status when fetching or reading the Email.

param **`imapCallback`** The function that accepts the `imapStatusCallback` as parameter.

```cpp
void callback(imapStatusCallback imapCallback);
```




#### Assign the callback function to decode the string based on the character set.

param **`callback`** The function that accepts the pointer to `IMAP_Decoding_Info` as parameter.

```cpp
void characterDecodingCallback(imapCharacterDecodingCallback callback);
```



#### Assign the callback function that returns the MIME data stream from fetching or reading the Email.

param **`mimeDataStreamCallback`** The function that accepts the `MIME_Stream_Info` as parameter.

```cpp
void mimeDataStreamCallback(MIMEDataStreamCallback mimeDataStreamCallback);
```





#### Determine if no message body contained in the search result and only the message header is available.

```cpp
bool headerOnly();
```





#### Get the message list from search or fetch the Emails

return **`The IMAP_MSG_List structured`** data which contains the text and html contents, 
attachments, inline images, embedded rfc822 messages details for each message.

```cpp
IMAP_MSG_List data();
```




#### Get the details of the selected or opned mailbox folder

return **`The SelectedFolderInfo class`** instance which contains the info about flags, total messages, next UID,  
earch count and the available messages count.

```cpp
SelectedFolderInfo selectedFolder();
```





#### Get the error details when readingg the Emails.

return **`String`** The string of error details.

```cpp
String errorReason();
```


#### Get the operating status error code.

return **`int`** The value of operating status error code.

The negative value indicated error.

See src/ESP_Mail_Error.h and extras/MB_FS.h

```cpp
int errorCode();
```



#### Clear all the cache data stored in the IMAP session object.

```cpp
void empty();
```



#### Get the status of message fetching and searching.
  
return **`IMAP_Status`** The IMAP_Status object contains the fetching and searching statuses.

```cpp
IMAP_Status status();
```



#### Get the JSON string of file name list of files that stored in SD card.

return **`The JSON string`** of filenames.

note This will available only when standard SD library was used and file storage is SD.

```cpp
String fileList();
```


## SMTPSession class functions


The following functions are available from the SMTP Session class.

This class is similar to the IMAP session class, used for controlling SMTP transports 
and retrieving the data from the SMTP server.




#### Set the tcp timeout.

param **`timeoutSec`** The tcp timeout in seconds.

```cpp
void setTCPTimeout(unsigned long timeoutSec);
```



#### Assign custom Client from Arduino Clients.

param **`client`** The pointer to Arduino Client derived class e.g. WiFiClient, WiFiClientSecure, EthernetClient or GSMClient.

```cpp
void setClient(Client *client);
```


#### Assign TinyGsm Clients.

param **`client`** The pointer to TinyGsmClient.

param **`modem`** The pointer to TinyGsm modem object. Modem should be initialized and/or set mode before transfering data.

param **`pin`** The SIM pin.

param **`apn`** The GPRS APN (Access Point Name).

param **`user`** The GPRS user.

param **`password`** The GPRS password.

```cpp
void setGSMClient(Client *client, void *modem, const char *pin, const char *apn, const char *user, const char *password);
```



#### Assign the callback function to handle the network connection for custom Client.

param **`networkConnectionCB`** The function that handles the network connection.

```cpp
void networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB);
```


#### Assign the callback function to handle the network connection status acknowledgement.

param **`networkStatusCB`** The function that handle the network connection status acknowledgement.

```cpp
void networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB);
```



#### Set the network status acknowledgement.

param **`status`** The network status.

```cpp
void setNetworkStatus(bool status);
```



#### Set the BearSSL IO buffer size.

param **`rx`** The BearSSL receive buffer size in bytes.

param **`tx`** The BearSSL trasmit buffer size in bytes.

```cpp
void setSSLBufferSize(int rx = -1, int tx = -1);
```



#### Set system time with timestamp.

param **`ts`** timestamp in seconds from midnight Jan 1, 1970.

param **`gmtOffset`** The GMT offset.

This function allows the internal time setting by timestamp i.e. timestamp from external RTC. 

```cpp
void setSystemTime(time_t ts, float gmtOffset = 0);
```



#### Begin the SMTP server connection.

param **`session_config`** The pointer to Session_Config structured data that keeps the server and log in details.

param **`login`** The bool option for login after server connection.

return **`boolean`** The boolean value indicates the success of operation.

```cpp
bool connect(Session_Config *session_config, bool login = true);
```



#### Login to SMTP server using Email and password.

param **`email`** The SMTP server account email.

param **`password`** The SMTP server account password.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool loginWithPassword(<string> email, <string> password);
```



#### Login to SMTP server using Email and access token.

param **`email`** The SMTP server account email.

param **`token`** The Access token to log in.

return **`boolean`** The boolean value which indicates the success of operation.

```cpp
bool loginWithAccessToken(<string> email, <string> token);
```


#### Return the SASL authentication status.

return **`boolean`** The boolean value indicates SASL authentication status.

```cpp
bool isAuthenticated();
```



#### Return the log in status.

return **`boolean`** The boolean value indicates log in status.

```cpp
bool isLoggedIn();
```


#### Close the SMTP session.

```cpp
bool closeSession();
```



#### Setup TCP KeepAlive for internal TCP client.

param **`tcpKeepIdleSeconds`** lwIP TCP Keepalive idle in seconds.

param **`tcpKeepIntervalSeconds`** lwIP TCP Keepalive interval in seconds.

param **`tcpKeepCount`** lwIP TCP Keepalive count.

For the TCP (KeepAlive) options, see [this doc](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#tcp-options).

If value of one of these parameters is zero, the TCP KeepAlive will be disabled.

You can check the server connecting status, by exexuting `<SMTPSession>.connected()` which will return true when connection to the server is still alive. 

```cpp
void keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount);
```



#### Get TCP KeepAlive status.

return **`Boolean`** status of TCP Keepalive.

```cpp
bool isKeepAlive();
```



#### Get TCP connection status.

return **`boolean`** The boolean value indicates the connection status.

```cpp
bool connected();
```




#### Begin the SMTP server connection without authentication.

param **`session_config`** The pointer to Session_Config structured data that keeps the server and log in details.

param **`callback`** The callback function that accepts the SMTP_Response as parameter.

param **`commandID`** The command identifier number that will pass to the callback.

return **`The int`** value of response code.

If commandID was not set or set to -1, the command identifier will be auto increased started from zero.

```cpp
int customConnect(Session_Config *session_config, smtpResponseCallback callback, int commandID = -1);
```






#### Send the custom SMTP command and get the result via callback.

param **`cmd`** The command string.

param **`callback`** The function that accepts the SMTP_Response as parameter.

return **`boolean`** The boolean value which indicates the success of operation.

smtp.connect or smtp.customConnect is needed to call once prior to call this function.

```cpp
bool sendCustomCommand(<string> cmd, smtpResponseCallback callback);
```





#### Send the custom SMTP command data string.

param **`data`** The string data.

return **`The boolean`** value which indicates the success of operation.

Should be used after calling sendCustomCommand("DATA");

```cpp
bool sendCustomData(<string> data);
```




#### Send the custom SMTP command data.

param **`data`**  The byte data.

param **`size`**  The data size.

return **`The boolean`** value which indicates the success of operation.

Should be used after calling sendCustomCommand("DATA");

```cpp
bool sendCustomData(uint8_t *data, size_t size);
```





#### Set to enable the debug.

param **`level`** The level to enable the debug message

level = 0, no debugging

level = 1, basic level debugging

```cpp
void debug(int level);
```





#### Get the error details when sending the Email

return **`String`** The string of error details.

```cpp
String errorReason();
```


#### Get the operating status error code.

return **`int`** The value of operating status error code.

The negative value indicated error.

See src/ESP_Mail_Error.h and extras/MB_FS.h

```cpp
int errorCode();
```



#### Get the SMTP server response status code.

return **`int`** The value of SMTP server response status code.

See RFC 5321 standard's documentation.

```cpp
int statusCode();
```



#### Get the SMTP server response status message.

return **`String`** The value of SMTP server response status message.

```cpp
String statusMessage();
```



#### Set the Email sending status callback function.

param **`smtpCallback`** The callback function that accept the `smtpStatusCallback` param.

```cpp
void callback(smtpStatusCallback smtpCallback);
```


#### Get the status of message fetching and searching.

return **`SMTP_Status`** The SMTP_Status object contains the sending status.

```cpp
SMTP_Status status();
```

## SMTP_Message class functions


The following functions are available from the SMTP Message class.

This class is used for storing the message data including headers, body and attachments
which will be processed with the SMTP session class.




#### To reset the SMTP_Attachment item data

param **`att`** The SMTP_Attachment class that stores the info about attachment

This function was used for clear the internal data of attachment item to be ready for reuse.

```cpp
void resetAttachItem(SMTP_Attachment &att);
```



#### To clear all data in SMTP_Message class included headers, bodies and attachments

```cpp
void clear();
```




#### To clear all the inline images in SMTP_Message class.

```cpp
void clearInlineimages();
```





#### To clear all the attachments.

```cpp
void clearAttachments();
```





#### To clear all rfc822 message attachment.

```cpp
void clearRFC822Messages();
```





#### To clear the primary recipient mailboxes.

```cpp
void clearRecipients();
```





#### To clear the Carbon-copy recipient mailboxes.

```cpp
void clearCc();
```





#### To clear the Blind-carbon-copy recipient mailboxes.

```cpp
void clearBcc();
```


#### To clear the custom message headers.

```cpp
void clearHeader();
```




#### To add attachment to the message.

param **`att`** The SMTP_Attachment data item

```cpp
void addAttachment(SMTP_Attachment &att);
```





#### To add parallel attachment to the message. 

param **`att`** The SMTP_Attachment data item

```cpp
void addParallelAttachment(SMTP_Attachment &att);
```





#### To add inline image to the message.

param **`att`** The SMTP_Attachment data item

```cpp
void addInlineImage(SMTP_Attachment &att);
```





#### To add rfc822 message to the message.

param **`msg`** The RFC822_Message class object

```cpp
void addMessage(SMTP_Message &msg);
```





#### To add the primary recipient mailbox to the message.

param **`name`** The name of primary recipient

param **`email`** The Email address of primary recipient

```cpp
void addRecipient(<string> name, <string> email);
```





#### To add Carbon-copy recipient mailbox.

param **`email`** The Email address of secondary recipient

```cpp
void addCc(<string> email);
```





#### To add Blind-carbon-copy recipient mailbox.

param **`email`** The Email address of the tertiary recipient

```cpp
void addBcc(<string> email);
```





#### To add the custom header to the message.

param **`hdr`** The header name and value

```cpp
void addHeader(<string> hdr);
```




##### [properties] The message author config

This property has the sub properties

###### [const char*] name - The sender name.

###### [const char*] email - The sender Email address.

```cpp
esp_mail_email_info_t sender;
```


##### [properties] The topic of message

```cpp
const char *subject;
```


##### [properties] The message type

```cpp
byte type;
```


##### [properties] The PLAIN text message

This property has the sub properties

###### [esp_mail_smtp_embed_message_body_t] embed - The option to embed this message content as a file.

###### [const char*] content - The PLAIN text content of the message.

###### [esp_mail_blob_message_content_t] blob - The blob that contins PLAIN text content of the message.

###### [esp_mail_file_message_content_t] file - The file that contins PLAIN text content of the message.

###### [const char*] charSet - The character transcoding of the PLAIN text content of the message.

###### [const char*] content_type - The content type of message.

###### [const char*] transfer_encoding - The option to encode the content for data transfer.

###### [boolean] flowed - The option to send the PLAIN text with wrapping.

```cpp
esp_mail_plain_body_t text;
```


##### [properties] The HTML text message

This propery has the sub properties

###### [const char*] content - The HTML content of the message.

###### [esp_mail_blob_message_content_t] blob - The blob that contins HTML content of the message.

###### [esp_mail_file_message_content_t] file - The file that contins HTML content of the message.

###### [const char*] charSet - The character transcoding of the HTML content of the message.

###### [const char*] content_type - The content type of message.

###### [const char*] transfer_encoding - The option to encode the content for data transfer.

```cpp
esp_mail_html_body_t html;
```


##### [properties] The response config

This propery has the sub properties

###### [const char*] reply_to - The author Email address to reply.

###### [const char*] return_path - The sender Email address to return the message.

###### [int] notify - The Delivery Status Notifications enumeration e.g. 

esp_mail_smtp_notify_never = 0,

esp_mail_smtp_notify_success = 1,

esp_mail_smtp_notify_failure = 2, and

esp_mail_smtp_notify_delay = 4 

```cpp
esp_mail_smtp_msg_response_t response;
```


##### [properties] The priority of the message

This property has the enumeration values

esp_mail_smtp_priority_high = 1,

esp_mail_smtp_priority_normal = 3,

esp_mail_smtp_priority_low = 5

```cpp
esp_mail_smtp_priority priority;
```


##### [properties] The enable options

This propery has the sub property

###### [boolean] chunking - enable chunk data sending for large message.

```cpp
esp_mail_smtp_enable_option_t enable;
```


##### [properties] The message from config

This property has the sub properties

###### [const char*] name - The messsage author name.

###### [const char*] email - The message author Email address.

```cpp
esp_mail_email_info_t from;
```


##### [properties] The message identifier

```cpp
const char *ID;
```

##### [properties] The keywords or phrases, separated by commas

```cpp
const char *keywords;
```


##### [properties] The comments about message

```cpp
const char *comments;
```


##### [properties] The date of message

```cpp
const char *date;
```


##### [properties] The return recipient of the message

```cpp
const char *return_path;
```



##### [properties] The field that contains the parent's message ID of the message to which this one is a reply

```cpp
const char *in_reply_to;
```



##### [properties] The field that contains the parent's references (if any) and followed by the parent's message ID (if any) of the message to which this one is a reply

```cpp
const char *references;
```


## IMAP_Status class functions


The following functions are available from the IMAP Status class.

This class is used as the callback parameter for retrieving the status while reading the Email.




#### Get the information of each process in the reading operation.

return **`string`** The info for each process

```cpp
const char *info();
```




#### Get the status of completion.

return **`boolean`** The bool value indicates that all reading processes are finished

```cpp
bool success();
```





#### To clear all data store in this class.

```cpp
void empty();
```






## SMTP_Status class functions


The following functions are available from the SMTP Status class.

This class is used as the callback parameter for retrieving the status while sending the Email.




#### Get the information of each process in the sending operation.

return **`string`** The info for each process

```cpp
const char *info();
```




#### Get the status of completion.

return **`boolean`** The bool value indicates that all sending processes are finished

```cpp
bool success();
```





#### To clear all data store in this class.

```cpp
void empty();
```




#### Get the number of complete sending message.

return **`number`** The number of message that was sent

```cpp
size_t completedCount();
```





#### Get the number of failed sending message.

return **`number`** The number of message that was not sent

```cpp
size_t failedCount();
```




## SendingResult class functions


The following functions are available from the SendingResult class.

This class is used for retrieving the info about the result of sending the messages.




#### Get the information of a message sending status.

param **`index`** The index number of a message sending status

return **`SMTP_Result`** The SMTP_Result type data that provides these properties

##### [bool] completed - The status of the message

#### [MB_String] recipients - The primary recipient mailbox of the message

#### [MB_String] subject - The topic of the message

#### [time_t] timesstamp - The timestamp of the message

```cpp
SMTP_Result getItem(size_t index);
```





#### Get the amount of the result data.

return **`number`** The number of result item

```cpp
size_t size();
```




## SMTP_Response type data

The following properties are available from the SMTP_Response data type.

This data type obtains the response from sending custom SMTP commands.


#### [Properties] The command identifier

```cpp
int id;
```

#### [Properties] The response text

```cpp
MB_String text;
```


#### [Properties] The response code

```cpp
int respCode;
```




## FoldersCollection class functions


The following functions are available from the FoldersCollection class.

This class is used for retrieving the info about the mailbox folders which available to read or serach
in the user Email mailbox.




#### Get the information of a folder in a folder collection.

param **`index`** The index number of folders

return **`esp_mail_folder_info_item_t`** The esp_mail_folder_info_item_t structured data that provides these properties

#### [MB_String] name - The name of folder

#### [MB_String] attributes - The attributes of folder

#### [MB_String] delimiter - The delimeter of folder

```cpp
esp_mail_folder_info_item_t info(size_t index);
```





#### Get the number of folders in the collection.

return **`number`** The number of folder in the collection

```cpp
size_t size();
```






## SelectedFolderInfo class functions


The following functions are available from the SelectedFolderInfo class.

This class is used for retrieving the info about the sselected or opened mailbox folder.




#### Get the numbers of flags in the user Email mailbox.

return **`number`** The numbers of flags

```cpp
size_t flagCount();
```





#### Get the numbers of messages in this mailbox.

return **`number`** The numbers of messages in the selected mailbox folder

```cpp
size_t msgCount();
```






#### Get the numbers of messages in this mailbox that recent flag was set.

return **`number`** The numbers of messages in the selected mailbox folder that recent flag was set

```cpp
size_t recentCount();
```






#### Get the order of message in number of message in this mailbox that reoved.

return **`IMAP_Polling_Status`** The data that holds the polling status.

The IMAP_Polling_Status has the properties e.g. type, messageNum, and argument.

The type property is the type of status e.g.imap_polling_status_type_undefined, imap_polling_status_type_new_message,
imap_polling_status_type_remove_message, and imap_polling_status_type_fetch_message.

The messageNum property is message number or order from the total number of message that added, fetched or deleted.

The argument property is the argument of commands e.g. FETCH

```cpp
struct IMAP_Polling_Status pollingStatus();
```








#### Get the predicted next message UID in the sselected folder.

return **`number`** The number represents the next message UID number

```cpp
size_t nextUID();
```







#### Get the numbers of messages from search result based on the search criteria.

return **`number`** The total number of messsages from search

```cpp
size_t searchCount();
```





#### Get the numbers of messages to be stored in the ressult.

return **`number`** The number of messsage stored from search

```cpp
size_t availableMessages();
```





#### Get the flag argument at the specified index.

return **`index`** The index of flag in the flags list

return **`String`** The argument of selected flag

```cpp
String flag(size_t index);
```




## Session_Config type data


The following properties are available from the Session_Config data type.

This data type is used for storing the session info about the server and login credentials.


#### [Properties] The server config

This property has the sub properties

##### [MB_String] host_name - The hostName of the server.

##### [uint16_t] port - The port on the server to connect to.

```cpp
esp_mail_sesson_sever_config_t server;
```


#### [Properties] The log in config

This property has the sub properties

##### [MB_String] email - The user Email address to log in.

##### [consst char *] password - The user password to log in.

##### [consst char *] accessToken - The OAuth2.0 access token to log in.

##### [consst char *] user_domain - The host name or public IP of client system.

```cpp
esp_mail_sesson_login_config_t login;
```


#### [Properties] The secure config

This property has the sub properties

##### [bool] startTLS - The option to send the command to start the TLS connection.

```cpp
esp_mail_sesson_secure_config_t secure;
```


#### [Properties] The ports and protocols config

This property has the sub properties

##### [port_function *] list - The list (aray) of port_function.

##### [size_t] size - The size of array.

```cpp
esp_mail_ports_functions ports_functions;
```




#### [Properties] The certificate config

##### [const char *] cert_data - The certificate data (base64 data).

##### [const char *] cert_file - The certificate file (DER format).

##### [esp_mail_file_storage_type] cert_file_storage_type - The storage type.

##### [bool] verify - The cerificate verification option.

```cpp
esp_mail_sesson_cert_config_t certificate;
```



#### [Properties] SPI Ethernet Module config for ESP8266

##### [ENC28J60lwIP *] enc28j60 - The ENC28J60 Ethernet module lwip interfac.

##### [Wiznet5100lwIP *] w5100 - The W5100 Ethernet module lwip interfac.

##### [Wiznet5500lwIP *] w5500 - The W5500 Ethernet module lwip interfac.

```cpp
esp_mail_spi_ethernet_module_t spi_ethernet_module;
```



#### [Properties] The callback function for WiFi connection

```cpp
NetworkConnectionHandler network_connection_handler;
```




## port_function type data

#### [Properties] The port number

```cpp
uint16_t port;
```

#### [Properties] The protocol

The port protocol e.g. esp_mail_protocol_plain_text, esp_mail_protocol_ssl and esp_mail_protocol_tls.

```cpp
esp_mail_protocol protocol;
```




## IMAP_Data type data


The following properties are available from the IMAP_Data data type.

This data type is used for storing the IMAP transport and operating options to 
control and store the operation result e.g. the messahe contents from search and fetch.




#### [Properties] The config for fetching

This property has the sub properties

##### [MB_String] uid - The UID of message to fetch.

##### [MB_String] number - The message sequence number to fetch.

##### [esp_mail_imap_sequence_set_t] sequence_set - The sequence set options.

##### [MB_String] set_seen - Set the message flag as seen.

```cpp
esp_mail_imap_fetch_config_t fetch;
```


#### [Properties] The config for IMAP sequence set fetching

This property has the sub properties

##### [MB_String] string - The sequence set string i.g., unique identifier (UID) 
or message sequence number or ranges of UID or sequence number.

##### [bool] UID - The option for sequenceSet whether it is UID or message sequence number.

##### [bool] headerOnly - The option for header only fetching.

```cpp
esp_mail_imap_sequence_set_t fetch;
```


#### [Properties] The config for search

This property has the sub properties

##### [MB_String] criteria - The search criteria.

##### [boolean] unseen_msg - The option to search the unseen message.

```cpp
esp_mail_imap_search_config_t search;
```


#### [Properties] The config about the limits

This property has the sub properties

##### [size_t] search - The maximum messages from the search result.

##### [size_t] fetch - The maximum messages from the sequence set fetching result.

##### [size_t] msg_size - The maximum size of the memory buffer to store the message content.

This is only limit for data to be stored in the IMAPSession. 

##### [size_t] attachment_size - The maximum size of each attachment to download.

The IMAP idle (polling) timeout in ms (1 min to 29 min). Default is 10 min.

##### [size_t] imap_idle_timeout - The IMAP idle timeout in ms.

The IMAP idle (polling) host check interval in ms (30 sec to imap_idle_timeout) 
for internet availability checking to ensure the connection is active. 
 
Default is 1 min.

##### [size_t] imap_idle_host_check_interval - The IMAP idle host check interval in ms.


```cpp
esp_mail_imap_limit_config_t limit;
```



#### [Properties] The config to enable the features

This property has the sub properties

##### [boolean] text - To store the PLAIN text of the message in the IMAPSession.

##### [boolean] html - To store the HTML of the message in the IMAPSession.

##### [boolean] rfc822 - To store the rfc822 messages in the IMAPSession.

##### [boolean] download_status - To enable the download status via the serial port.

##### [boolean] recent_sort - To sort the message UID of the search result in descending order.

##### [boolean] header_case_sesitive - To allow case sesitive in header parsing.

```cpp
esp_mail_imap_enable_config_t enable;
```



#### [Properties] The config about downloads

This property has the sub properties

##### [boolean] text - To download the PLAIN text content of the message.

##### [boolean] html - To download the HTML content of the message.

##### [boolean] attachment - To download the attachments of the message.

##### [boolean] inlineImg - To download the inline image of the message.

##### [boolean] rfc822 - To download the rfc822 mesages in the message.

##### [boolean] header - To download the message header.

```cpp
esp_mail_imap_download_config_t download;
```



#### [Properties] The config about the storage and path to save the downloaded file.

This property has the sub properties

##### [const char*] saved_path - The path to save the downloaded file.

##### [esp_mail_file_storage_type] type - The type of file storages enumeration e.g.

esp_mail_file_storage_type_none = 0,

esp_mail_file_storage_type_flash = 1, and

esp_mail_file_storage_type_sd = 2

```cpp
esp_mail_imap_storage_config_t storage;
```


#### [Properties] The config about firmware updates and downloads for ESP32, ESP8266 and Raspberry Pi Pico.

This property has the sub properties

##### [string] attach_filename - Update firmware using message attachments if one of its filename matches.

##### [bool] save_to_file - Save firmware file option.

```cpp
esp_mail_imap_firmware_config_t firmware_update;
```



## esp_mail_smtp_embed_message_body_t structured data


The following properties are available from the IMAP_Data data type.

This data type is used for storing the IMAP transport and operating options to 
control and store the operation result e.g. the messahe contents from search and fetch.




##### [Properties] Enable to send this message body as file

```cpp
bool enable;
```


##### [Properties] The name of embedded file

```cpp
const char* enable;
```


##### [Properties] The embedded type enumeration

esp_mail_smtp_embed_message_type_attachment = 0

sp_mail_smtp_embed_message_type_inline = 1

```cpp
esp_mail_smtp_embed_message_type type;
```

## esp_mail_blob_message_content_t structured data


The following properties are available from the esp_mail_blob_message_content_t data type.

This data type is used for storing the blob info of message body.



##### [Properties] The array of content in flash memory.

```cpp
const uint8_t * data;
```


##### [Properties] The array size in bytes.

```cpp
size_t size;
```





## esp_mail_file_message_content_t structured data


The following properties are available from the esp_mail_file_message_content_t data type.

This data type is used for storing the file info of message body.


##### [Properties] The file path include its name.

```cpp
const char *name;
```


##### [Properties] The type of file storages.

```cpp
esp_mail_file_storage_type type;
```





## IMAP_MSG_Item type data


The following properties are available from the IMAP_MSG_Item data type.

This data type is used for message item info and its contents from search and fetch.




#### [Properties] The message number

```cpp
int msgNo;
```


#### [Properties] The message UID

```cpp
int UID;
```


#### [Properties] The message identifier

```cpp
const char *ID;
```



#### [Properties] The language(s) for auto-responses

```cpp
const char *acceptLang;
```



#### [Properties] The language of message content

```cpp
const char *contentLang;
```



#### [Properties] The mailbox of message author

```cpp
const char *from;
```


#### [Properties] The charset of the mailbox of message author (deprecate)

```cpp
const char *fromCharset;
```


#### [Properties] The primary recipient mailbox

```cpp
const char *to;
```


#### [Properties] The charset of the primary recipient mailbox (deprecate)

```cpp
const char *toCharset;
```


#### [Properties] The Carbon-copy recipient mailboxes

```cpp
const char *cc;
```


#### [Properties] The charset of the Carbon-copy recipient mailbox header (deprecate)

```cpp
const char *ccCharset;
```

#### [Properties] The message date and time

```cpp
const char *date;
```

#### [Properties] The topic of message

```cpp
const char *subject;
```

#### [Properties] The topic of message charset (deprecate)

```cpp
const char *subjectCharset;
```

#### [Properties] The PLAIN text content of the message

```cpp
esp_mail_plain_body_t text;
```

#### [Properties] The HTML content of the message

```cpp
esp_mail_html_body_t html;
```

#### [Properties] The sender Email

```cpp
const char *sender;
```


#### [Properties] The charset of the sender Email (obsoleted)

```cpp
const char *senderCharset;
```


#### [Properties] The keywords or phrases, separated by commas

```cpp
const char *keyword;
```

#### [Properties] The comments about message

```cpp
const char *comments;
```


#### [Properties] The return recipient of the message

```cpp
const char *return_path;
```


#### [Properties] The Email address to reply

```cpp
const char *reply_to;
```


#### [Properties] The field that contains the parent's message ID of the message to which this one is a reply

```cpp
const char *in_reply_to;
```


#### [Properties] The field that contains the parent's references (if any) and followed by the parent's message ID (if any) of the message to which this one is a reply

```cpp
const char *references;
```


#### [Properties] The Blind carbon-copy recipients

```cpp
const char *bcc;
```

#### [Properties] The charset of the Blind carbon-copy recipient mailbox header (obsoleted)

```cpp
const char *bccCharset;
```


#### [Properties] The error description from fetching the message

```cpp
const char *fetchError;
```


#### [Properties] The info about the attachments in the message

```cpp
std::vector<IMAP_Attach_Item> attachments;
```

#### [Properties] The info about the rfc822 messages included in the message

```cpp
std::vector<IMAP_MSG_Item> rfc822;
```




## IMAP_Response type data

The following properties are available from the IMAP_Response data type.

This data type obtains the response from sending custom IMAP commands.


#### [Properties] The status tag

```cpp
MB_String tag;
```

#### [Properties] The status text

```cpp
MB_String text;
```


#### [Properties] The completion of response

```cpp
bool completed;
```






## MIME_Data_Stream_Info type data

The following properties are available from the MIME_Data_Stream_Info data type.

This data type obtains the IMAP multipart body or MIME stream data via the callback function.


#### [Properties] The message UID

```cpp
uint32_t uid;
```

#### [Properties] The content type of the message part

```cpp
const char *type;
```

#### [Properties] The content disposition of the message part

```cpp
const char *disposition;
```


#### [Properties] The content character set of the message part

```cpp
const char *charSet;
```


#### [Properties] The text content flowed format parameter of the message part

```cpp
bool flowed;
```


#### [Properties] The text content format DelSp parameter of the message part

```cpp
bool delsp;
```


#### [Properties] The content transfer encoding of the message part

```cpp
const char *transfer_encoding;
```


#### [Properties] The content ID of the inline attachment type message part

```cpp
const char *cid;
```


#### [Properties] The content description of the message part

```cpp
const char *description;
```

#### [Properties] The file name of the attachment type message part

```cpp
const char *filename;
```


#### [Properties] The name of the attachment type message part

```cpp
const char *name;
```


#### [Properties] The creation date of the message part

```cpp
const char *date;
```

#### [Properties] The size of the attachment (unencoded) type message part

```cpp
size_t size;
```


#### [Properties] The total octet of the message part

```cpp
size_t octet_size;
```

#### [Properties] The current octet count of the message part that currently parses

```cpp
int octet_count;
```

#### [Properties] The size of content (unencoded) of message part

```cpp
size_t data_size;
```


#### [Properties] The data buffer of message part that currently parses

```cpp
void *data;
```


#### [Properties] The flag that states the first chunk data of message part that currently parses

```cpp
bool isFirstData;
```

#### [Properties] The flag that states the final chunk data of message part that currently parses

```cpp
bool isLastData;
```




## IMAP_Decoding_Info type data

The following properties are available from the IMAP_Decoding_Info data type.

This data type obtains the IMAP header and text (plain and html) strings to be decoded via the callback function based on its character set.


#### [Properties] The character set of the string to decode

```cpp
const char *charset;
```

#### [Properties] The string to decode

```cpp
const char *data;
```


#### [Properties] The type of data that currently processed

0 or IMAP_Decoding_Info::message_part_type_header

1 or IMAP_Decoding_Info::message_part_type_text

```cpp
message_part_type type;
```





## Search Criteria

Search crieria is used for searching the mailbox for messages that match
the given searching criteria.  

Searching criteria consist of one or more search keys. When multiple keys are 
specified, the result is the intersection (AND function) of all the messages 
that match those keys.  

Example:

 **`DELETED FROM "SMITH" SINCE 1-Feb-1994`** refers 
to all deleted messages from Smith that were placed in the mailbox since 
February 1, 1994.  

A search key can also be a parenthesized list of one or more search keys 
(e.g., for use with the OR and NOT keys).

**`SINCE 10-Feb-2019`**  will search all messages that received since 10 Feb 2019

**`UID SEARCH ALL`**  will seach all message which will return the message UID 
that can be use later for fetch one or more messages.

 
The following keywords can be used for the search criteria.


**ALL** - All messages in the mailbox; the default initial key for	ANDing.

**ANSWERED** - Messages with the \Answered flag set.

**BCC** - Messages that contain the specified string in the envelope structure's BCC field.

**BEFORE** - Messages whose internal date (disregarding time and timezone) is earlier than the specified date.

**BODY** - Messages that contain the specified string in the body of the message.

**CC** - Messages that contain the specified string in the envelope structure's CC field.

**DELETED** - Messages with the \Deleted flag set.

**DRAFT** - Messages with the \Draft flag set.

**FLAGGED** - Messages with the \Flagged flag set.

**FROM** - Messages that contain the specified string in the envelope	structure's FROM field.

**HEADER** - Messages that have a header with the specified field-name (as defined in [RFC-2822])

and that contains the specified string	in the text of the header (what comes after the colon).

If the string to search is zero-length, this matches all messages that have a header line with 

the specified field-name regardless of	the contents.

**KEYWORD** - Messages with the specified keyword flag set.

**LARGER** - Messages with an (RFC-2822) size larger than the specified number of octets.

**NEW** -  Messages that have the \Recent flag set but not the \Seen flag.

This is functionally equivalent to **"(RECENT UNSEEN)"**.

**NOT** - Messages that do not match the specified search key.

**OLD** - Messages that do not have the \Recent flag set.  This is	functionally equivalent to

**"NOT RECENT"** (as opposed to **"NOT NEW"**).

**ON** - Messages whose internal date (disregarding time and timezone) is within the specified date.

**OR** - Messages that match either search key.

**RECENT** - Messages that have the \Recent flag set.

**SEEN** - Messages that have the \Seen flag set.

**SENTBEFORE** - Messages whose (RFC-2822) Date: header (disregarding time and	timezone) is earlier than the specified date.

**SENTON** - Messages whose (RFC-2822) Date: header (disregarding time and timezone) is within the specified date.

**SENTSINCE** - Messages whose (RFC-2822) Date: header (disregarding time and timezone) is within or later than the specified date.

**SINCE** - Messages whose internal date (disregarding time and timezone) is within or later than the specified date.

**SMALLER** - Messages with an (RFC-2822) size smaller than the specified number of octets.

**SUBJECT** - Messages that contain the specified string in the envelope structure's SUBJECT field.

**TEXT** - Messages that contain the specified string in the header or body of the message.

**TO** - Messages that contain the specified string in the envelope structure's TO field.

**UID** - Messages with unique identifiers corresponding to the specified unique identifier set.  

Sequence set ranges are permitted.

**UNANSWERED** - Messages that do not have the \Answered flag set.

**UNDELETED** - Messages that do not have the \Deleted flag set.

**UNDRAFT** - Messages that do not have the \Draft flag set.

**UNFLAGGED** - Messages that do not have the \Flagged flag set.

**UNKEYWORD** - Messages that do not have the specified keyword flag set.

**UNSEEN** - Messages that do not have the \Seen flag set.









## MailClient.Time functions


The helper function to set and get the system time.





#### Set the system time from the NTP server.

param **`gmtOffset`** The GMT time offset in hour.

param **`daylightOffset`** The Daylight time offset in hour.

return **`boolean`** The status indicates the success of operation.

This requires internet connection

```cpp
bool setClock(float gmtOffset, float daylightOffset);
```





#### Get the timestamp from the year, month, date, hour, minute, and second provided.

param **`year`** The year.

param **`mon`** The months from 1 to 12.

param **`date`** The dates.

param **`hour`** The hours.

param **`mins`** The minutes.

param **`sec`** The seconds.

return **`time_t`** The value of timestamp.

```cpp
time_t getTimestamp(int year, int mon, int date, int hour, int mins, int sec);
```




#### Get the timestamp from the time string.

param **`gmt`** Return GMT time.

return **`timestamp`** of time string.

```cpp
time_t getTimestamp(const char* timeString, bool gmt = false);
```






#### Get the current timestamp.

return **`uint64_t`** The value of current timestamp.

```cpp
uint64_t getCurrentTimestamp();
```






#### Get the current date time string that valid for Email

return **`String`** The current date time string.

```cpp
String getDateTimeString();
```






## License

The MIT License (MIT)

Copyright (c) 2023 K. Suwatchai (Mobizt)


Permission is hereby granted, free of charge, to any person returning a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

