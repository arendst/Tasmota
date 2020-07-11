# Mail Client Arduino Library for ESP32 v 2.1.4

This library allows ESP32 to send Email with/without attachment and receive Email with/without attachment download via SMTP and IMAP servers. 

The library was test and work well with ESP32s based module.

Copyright (c) 2019 K. Suwatchai (Mobizt).

![ESP32 Mail](/media/images/esp32-mail.png)

## Tested Devices

This following devices were tested and work well.

 * Sparkfun ESP32 Thing
 * NodeMCU-32
 * WEMOS LOLIN32
 * TTGO T8 V1.8
 * M5Stack ESP32


 
## Features

* Support Email sending with or without attachment via IMAP server.

* Support SSL/TLS and STARTTLS protocols.

* Working with SD card allows large file attachment supported or SPIFFS for small file size attachment.

* Support Email reading via search and fetch modes (with or without attachment downloads).

* Support large attachment download via SD card or SPIFFS for small file size attachment.

* Message text and its header are able to download and save to SD card or SPIFFS.

* Support Email message fetch and search via IMAP command as in RFC 3501 (depending on IMAP server implementation).

* Support Ethernet.

* Built-in Time function.




## Prerequisites


For library version 1.2.0 or newer, STARTTLS was supported and can be enable automatically when port 587 for SMTP was used or can set manually thrugh smtpData.setSTARTTLS(true) and for IMAP through imapData.setSTARTTLS(true).



## Installing


Click on **Clone or download** dropdown at the top of repository, select **Download ZIP** and save file on your computer.

From Arduino IDE, goto menu **Sketch** -> **Include Library** -> **Add .ZIP Library...** and choose **ESP32-Mail-Client-master.zip** that previously downloaded.

Go to menu **Files** -> **Examples** -> **ESP32-Mail-Client-master** and choose one from examples



## Usages


__Declaration and Initialization__



**The first thing to do to use this library.**

```C++


//1. Include ESP32 Mail Client library (this library)

#include "ESP32_MailClient.h"


//2. For sending Email, declare Email Sending data object in global scope.
SMTPData smtpData;

//Or

//For receiving Email, declare Email receiving data object in global scope.
IMAPData imapData;


//3 Setup SMTP server login credential in setup()

smtpData.setLogin("smtp.gmail.com", 587, "YOUR_EMAIL_ACCOUNT@gmail.com", "YOUR_EMAIL_PASSWORD");

//Or

//Setup IMAP server login credential in setup()

imapData.setLogin("imap.gmail.com", 993, "YOUR_EMAIL_ACCOUNT@gmail.com", "YOUR_EMAIL_PASSWORD");


//4 For SMTP, set some custom message header (optional)
smtpData.addCustomMessageHeader("Date: Sat, 10 Aug 2019 21:39:56 -0700 (PDT)");

smtpData.addCustomMessageHeader("Message-ID: <10000.30000@gmail.com>");


//5 To debug for SMTP

smtpData.setDebug(true);

//Or IMAP
imapData.setDebug(true);


//6. Send Email
MailClient.sendMail(smtpData));

//Or Receive Email

MailClient.readdMail(imapData));





```

___


__Send and Receive Email__


**Compose Email**

This library allows you to set sender, recipient, importance (priority), CC, BCC and attachment data (binary or from SD card file).

To set sender, use `smtpData.setSender` e.g. `smtpData.setSender("Jarvis", "SOME_EMAIL_ACCOUNT@SOME_EMAIL.com")`.

To set priority, use `smtpData.setPriority` e.g. `smtpData.setPriority("High")`.

To set message subject, use `smtpData.setSubject` e.g. `smtpData.setSubject("ESP32 Send Mail Test")`.

To set message text, use `smtpData.setMessage` e.g. `smtpData.setMessage("This is plain text message", false);`.

To set sender, use `smtpData.addRecipient` e.g. `smtpData.addRecipient("SOME_RECIPIENT@SOME_MAIL.com")`.

To add attachment, use `smtpData.addAttachData` e.g. `smtpData.addAttachData("test.png", "image/png", (uint8_t *)imageData, sizeof imageData);`.


When completed all required message data, sending Email `MailClient.sendMail(smtpData)`.



**Get Email**

To read or receive Email, mailbox folder should be assigned via `imapData.setFolder` e.g. `imapData.setFolder("INBOX")`.

Then set search criteria to search specified mailbox folder via `imapData.setSearchCriteria` e.g. `imapData.setSearchCriteria("UID SEARCH ALL")`.

Then set search limit to limut the memory and time usages `imapData.setSearchLimit`.

From search criteria, UID of message will be available to fetch or read.

Whit search, body message and attachment can be ignore to reduce the network data usage.

Begin receive Email `MailClient.readMail(imapData)`.

From above settings, you will get the following header information

Messsage UID via `imapData.getUID`.

Messsage ID via `imapData.getMessageID`.

Accept Language via `imapData.getAcceptLanguage`.

Content Language via `imapData.getContentLanguage`.

Sender via `imapData.getFrom`.

Sender Charset via `imapData.getFromCharset`.

Recipient via `imapData.getTo`.

Recipient Charset via `imapData.getToCharset`.

CC via `imapData.getCC`.

CC Charset via `imapData.getCCCharset`.

Date via `imapData.getDate`.

Subject via `imapData.getSubject`.

Subject Charset via `imapData.getSubjectCharset`.

In addition, by setting search criteria, the following infomation are available.

Mailbox folder count via `imapData.getFolderCount`.

Mailbox folder name via `imapData.getFolder`.

Supported flags count via `imapData.getFlagCount`.

Supported flags name via `imapData.getFlag`.

Total message in folder via `imapData.totalMessages`.

Total message from search result via `imapData.searchCount`.

Available message from search result (limited by `imapData.setSearchLimit`) via `imapData.availableMessages`.

When fetch specific message via `imapData.setFetchUID`,  availability of attachment file can be determined via 
`imapData.getAttachmentCount` for that message which will be automatically download by setting `imapData.setDownloadAttachment(true)` 
prior to `MailClient.readMail`.



See [full examples](https://github.com/mobizt/ESP32-Mail-Client/tree/master/examples) for all features usages.





## All Supported Functions


**These are all functions available from the library and the descriptions.**


__Global functions__


**Sending Email via SMTP server.**

param *`smtpData`* - SMTP Data object to hold data and instances.

return - *`Boolean`* type status indicates the success of operation.

```C++
bool sendMail(SMTPData &smtpData);
```




**Reading Email via IMAP server.**

param *`imapData`* - IMAP Data object to hold data and instances.

return - *`Boolean`* type status indicates the success of operation.

```C++
bool readMail(IMAPData &imapData);
```





**Set the argument to the Flags for message.**

param *`imapData`* - IMAP Data object to hold data and instances.

param *`msgUID`* - The UID of message.

param *`flags`* - The flag list.

return - *`Boolean`* type status indicates the success of operation.

```C++
bool setFlag(IMAPData &imapData, int msgUID, const String &flags);
```





**Add the argument to the Flags for message.**

param *`imapData`* - IMAP Data object to hold data and instances.

param *`msgUID`* - The UID of message.

param *`flags`* - The flag list.

return - *`Boolean`* type status indicates the success of operation.

```C++
bool addFlag(IMAPData &imapData, int msgUID, const String &flags);
```






**Remove the argument from the Flags for message.**

param *`imapData`* - IMAP Data object to hold data and instances.

param *`msgUID`* - The UID of message.

param *`flags`* - The flag list.

return - *`Boolean`* type status indicates the success of operation.

```C++
bool removeFlag(IMAPData &imapData, int msgUID, const String &flags);
```





**Get the Email sending error details.**

return - *`Error details string (String object).`*

```C++
String smtpErrorReason();
```




**Get the Email reading error details.**

return - *`Error details string (String object).`*

```C++
String imapErrorReason();
```





**Init SD card with GPIO pins.**

param *`sck`* -SPI Clock pin.

param *`miso`* - SPI MISO pin.

param *`m0si`* - SPI MOSI pin.

param *`ss`* - SPI Chip/Slave Select pin.

return *`Boolean`* type status indicates the success of operation.


```C++
bool sdBegin(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss);
```






**Init SD card with default GPIO pins.**

return *`Boolean`* type status indicates the success of operation.


```C++
bool sdBegin(void);
```







### IMAPData object call for receiving Email.


**Set the IMAP server login credentials.**

param *`host`* - IMAP server e.g. imap.gmail.com.

param *`port`* - IMAP port e.g. 993 for gmail.

param *`loginEmail`* - The Email address of account.

param *`loginPassword`* - The account password.

param *`rootCA`* - Root CA certificate base64 string

```C++
void setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword);

void setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword, const char *rootCA);

```





**Set STARTTLS mode to enable STARTTLS protocol.**

param *`starttls`* - bool flag that enables STARTTLS mode

```C++
void setSTARTTLS(bool starttls);
```






**Set debug print to serial.**

param *`debug`* - bool flag to enable debug

```C++
void setDebug(bool debug);
```


 



**Set the mailbox folder to search or fetch.**

param *`folderName`* - Known mailbox folder. Default value is INBOX

```C++
void setFolder(const String &folderName);
```



**Set the maximum message buffer size for text/html result from search or fetch the message.**

param *`size`* - The message size in byte.

```C++
void setMessageBufferSize(size_t size);
```



**Set the maximum attachment file size to be downloaded.**

param *`size`* - The attachement file size in byte.

```C++
void setAttachmentSizeLimit(size_t size);
```



**Set the search criteria used in selected mailbox search.**

In case of message UID was set via setFetchUID function, search operation will not process,

you need to clear message UID by calling imapData.setFetchUID("") to clear.

param *`criteria`* - Search criteria String.

If folder is not set, the INBOX folder will be used

Example:

*`SINCE 10-Feb-2019`*  will search all messages that received since 10 Feb 2019

*`UID SEARCH ALL`*  will seach all message which will return the message UID that can be use later for fetch one or more messages.

 
Search criteria can be consisted these keywords


*`ALL`* - All messages in the mailbox; the default initial key for	ANDing.

*`ANSWERED`* - Messages with the \Answered flag set.

*`BCC`* - Messages that contain the specified string in the envelope structure's BCC field.

*`BEFORE`* - Messages whose internal date (disregarding time and timezone) is earlier than the specified date.

*`BODY`* - Messages that contain the specified string in the body of the message.

*`CC`* - Messages that contain the specified string in the envelope structure's CC field.

*`DELETED`* - Messages with the \Deleted flag set.

*`DRAFT`* - Messages with the \Draft flag set.

*`FLAGGED`* - Messages with the \Flagged flag set.

*`FROM`* - Messages that contain the specified string in the envelope	structure's FROM field.

*`HEADER`* - Messages that have a header with the specified field-name (as defined in [RFC-2822])

and that contains the specified string	in the text of the header (what comes after the colon).

If the string to search is zero-length, this matches all messages that have a header line with 

the specified field-name regardless of	the contents.

*`KEYWORD`* - Messages with the specified keyword flag set.

*`LARGER`* - Messages with an (RFC-2822) size larger than the specified number of octets.

*`NEW`* -  Messages that have the \Recent flag set but not the \Seen flag.

This is functionally equivalent to `*"(RECENT UNSEEN)"*`.

*`NOT`* - Messages that do not match the specified search key.

*`OLD`* - Messages that do not have the \Recent flag set.  This is	functionally equivalent to

*`"NOT RECENT"`* (as opposed to *`"NOT NEW"`*).

*`ON`* - Messages whose internal date (disregarding time and timezone) is within the specified date.

*`OR`* - Messages that match either search key.

*`RECENT`* - Messages that have the \Recent flag set.

*`SEEN`* - Messages that have the \Seen flag set.

*`SENTBEFORE`* - Messages whose (RFC-2822) Date: header (disregarding time and	timezone) is earlier than the specified date.

*`SENTON`* - Messages whose (RFC-2822) Date: header (disregarding time and timezone) is within the specified date.

*`SENTSINCE`* - Messages whose (RFC-2822) Date: header (disregarding time and timezone) is within or later than the specified date.

*`SINCE`* - Messages whose internal date (disregarding time and timezone) is within or later than the specified date.

*`SMALLER`* - Messages with an (RFC-2822) size smaller than the specified number of octets.

*`SUBJECT`* - Messages that contain the specified string in the envelope structure's SUBJECT field.

*`TEXT`* - Messages that contain the specified string in the header or body of the message.

*`TO`* - Messages that contain the specified string in the envelope structure's TO field.

*`UID`* - Messages with unique identifiers corresponding to the specified unique identifier set.  

Sequence set ranges are permitted.

*`UNANSWERED`* - Messages that do not have the \Answered flag set.

*`UNDELETED`* - Messages that do not have the \Deleted flag set.

*`UNDRAFT`* - Messages that do not have the \Draft flag set.

*`UNFLAGGED`* - Messages that do not have the \Flagged flag set.

*`UNKEYWORD`* - Messages that do not have the specified keyword flag set.

*`UNSEEN`* - Messages that do not have the \Seen flag set.

```C++
void setSearchCriteria(const String &criteria);
```





**Set to search the unseen message.**

param *`unseenSearch`* - Boolean flag to enable unseen message search.

This function will be overridden (omitted) by setFetchUID as setSearchCriteria.

```C++
void setSearchUnseenMessage(bool unseenSearch);
```






**Set the download folder.**

param *`path`* - Path in SD card.

All text/html message and attachemnts will be saved to message UID folder which created in defined path

e.g. *`"/{DEFINED_PATH}/{MESSAGE_UID}/{ATTACHMENT_FILE...}"`*.

```C++
void setSaveFilePath(const String &path);
```




**Specify message UID to fetch or read.**

param *`fetchUID`* - The message UID.

Specify the message UID to fetch (read) only specific message instead of search.

```C++
void setFetchUID(const String &fetchUID);
```





**Set storage type to save download attached file or messages.**

param *`storageType`* - The storage type to save file, MailClientStorageType::SD or MailClientStorageType::SPIFFS

```C++
void setFileStorageType(uint8_t storageType);
```






**Enable/disable attachment download.**

param *`download`* - Boolean flag to enable/disable attachment download.

```C++
void setDownloadAttachment(bool download);
```



**Enable/disable html message result.**

param *`htmlFormat`* - Boolean flag to enable/disable html message result.

The default value is false.

```C++
void setHTMLMessage(bool htmlFormat);
```




**Enable/disable plain text message result.**

param *`textFormat`* - Boolean flag to enable/disable plain text message result.

The default value is true.

```C++
void setTextMessage(bool textFormat);
```





**Set the maximum message to search.**

param *`limit`* - Any number from 0 to 65535.

The default value is 20.

```C++
void setSearchLimit(uint16_t limit);
```



**Enable/disable recent sort result.**

param *`recentSort`* - Boolean flag to enable/disable recent message sort result.

The default value is true.

```C++
void setRecentSort(bool recentSort);
```



**Assign callback function that return status of message fetching or reading.**

param *`readCallback`* - The function that accept readStatusCallback as parameter.

```C++
void setReadCallback(readStatusCallback readCallback);
```



**Enable/disable attachement download progress while fetching or receiving message.**

param *`report`* - Boolean flag to enable/disable attachement download progress while fetching or receiving message.

To get the download status, Callback function should be set via setReadCallback.

```C++
void setDownloadReport(bool report);
```



**Determine only message header is return when search.**

```C++
bool isHeaderOnly();
```



**Get the sender name/Email for selected message from search result.**

param *`messageIndex`* - The index of message.

return *`Sender name/Email String.`*

```C++
String getFrom(uint16_t messageIndex);
```



**Get the sender name/Email charactor encoding.**

param *`messageIndex`* - The index of message.

return *`Sender name/Email charactor encoding which use for decoding.`*

```C++
String getFromCharset(uint16_t messageIndex);
```



**Get the recipient name/Email for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Recipient name/Email String.`*

```C++
String getTo(uint16_t messageIndex);
```




**Get the recipient name/Email charactor encoding.**

param *`messageIndex`* - The index of message.

return *`Recipient name/Email charactor encoding which use in decoding to local language.`*

```C++
String getToCharset(uint16_t messageIndex);
```




**Get the CC name/Email for selected message index of IMAPData result.**

param *`messageIndex`* - The index of message.

return *`CC name/Email String.`*

```C++
String getCC(uint16_t messageIndex);
```



**Get the CC name/Email charactor encoding.**

param *`messageIndex`* - The index of message.

return *`CC name/Email charactor encoding which use in decoding to local language.`*

```C++
String getCCCharset(uint16_t messageIndex);
```




**Get the message subject for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Message subject name/Email String.`*

```C++
String getSubject(uint16_t messageIndex);
```




**Get the message subject charactor encoding.**

param *`messageIndex`* - The index of message.

return *`Message subject charactor encoding which use in decoding to local language.`*

```C++
String getSubjectCharset(uint16_t messageIndex);
```



**Get the html message for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The html message String or empty String upon the setHTMLMessage was set.`*

```C++
String getHTMLMessage(uint16_t messageIndex);
```



**Get the plain text message for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The plain text message String or empty String upon the setTextMessage was set.`*

```C++
String getTextMessage(uint16_t messageIndex);
```


**Get the html message charactor encoding.**

param *`messageIndex`* - The index of message.

return *`Html message charactor encoding which use in decoding to local language.`*

```C++
String getHTMLMessgaeCharset(uint16_t messageIndex);
```




**Get the text message charactor encoding.**

param *`messageIndex`* - The index of message.

return *`The text message charactor encoding which use in decoding to local language.`*

```C++
String getTextMessgaeCharset(uint16_t messageIndex);
```




**Get the date of received message for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The date String.`*

```C++
String getDate(uint16_t messageIndex);
```




**Get the message UID for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`UID String that can be use in setFetchUID.`*

```C++
String getUID(uint16_t messageIndex);
```




**Get the message number for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The message number which vary upon search criteria and sorting.`*

```C++
String getNumber(uint16_t messageIndex);
```




**Get the message ID for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The message ID String.`*

```C++
String getMessageID(uint16_t messageIndex);
```




**Get the accept language for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The accept language String.`*

```C++
String getAcceptLanguage(uint16_t messageIndex);
```



**Get the content language of text or html for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`The content language String.`*

```C++
String getContentLanguage(uint16_t messageIndex);
```




**Determine fetch error status for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Fetch error status.`*

```C++
bool isFetchMessageFailed(uint16_t messageIndex);
```



**Get fetch error reason for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Fetch error reason String for selected message index.`*

```C++
String getFetchMessageFailedReason(uint16_t messageIndex);
```




**Determine the attachment download error for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Fetch status.`*

```C++
bool isDownloadAttachmentFailed(uint16_t messageIndex, size_t attachmentIndex);
```




**Get the attachment download error reason for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Download error reason String for selected message index.`*

```C++
String getDownloadAttachmentFailedReason(uint16_t messageIndex, size_t attachmentIndex);
```



**Determine the downloaded/saved text message error status for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Text message download status.`*

```C++
bool isDownloadMessageFailed(uint16_t messageIndex);
```





**Get the attachment or message downloadeds error reason for selected message index from search result.**

param *`messageIndex`* - The index of message.

return *`Downloaded error reason String for selected message index.`*

```C++
String getDownloadMessageFailedReason(uint16_t messageIndex);
```




**Assign the download and decode flags for html message download.**

param *`download`* - Boolean flag to enable/disable message download.

return *`decoded`* - Boolean flag to enable/disable html message decoding (support utf8 and base64 encoding).

```C++
void saveHTMLMessage(bool download, bool decoded);
```




**Assign the download and decode flags for plain text message download.**

param *`download`* - Boolean flag to enable/disable message download.

return *`decoded`* - Boolean flag to enable/disable plain text message decoding (support utf8 and base64 encoding).

```C++
void saveTextMessage(bool download, bool decoded);
```




**Determine the mailbox folder count.**

return *`Folder count number.`* 

```C++
uint16_t getFolderCount();
```




**Get the mailbox folder name at selected index.**

param *`folderIndex`* - Index of folder.

return *`Folder name String.`* 

Use folder name from this function for fetch or search.

```C++
String getFolder(uint16_t folderIndex);
```



**Determin the number of supported flags count.**

return *`Flag count number.`* 

```C++
uint16_t getFlagCount();
```




**Get the flag name for selected index.**

param *`folderIndex`* - Index of folder.

return *`Flag name String.`* 

Use flags from this function for fetch or search.

```C++
String getFlag(uint16_t flagIndex);
```




**Get the number of message in selected mailbox folder.**

return *`Total message number.`* 

```C++
size_t totalMessages();
```




**Get the number of message from search result.**

return *`Search result number.`* 

```C++
size_t searchCount();
```




**Get the number of message available from search result which less than search limit.**

return *`Available message number.`* 

```C++
size_t availableMessages();
```





**Get the number of attachments for selected message index from search result.**

param *`messageIndex`* - Index of message.

return *`Number of attachments`* 

```C++
size_t getAttachmentCount(uint16_t messageIndex);
```





**Get file name of attachment for selected attachment index and message index from search result.**

param *`messageIndex`* - Index of message.

param *`attachmentIndex`* - Index of attachment.

return *`The attachment file name String at the selected index.`* 

```C++
String getAttachmentFileName(size_t messageIndex, size_t attachmentIndex);
```




**Get the name of attachment for selected attachment index and message index from search result.**

param *`messageIndex`* - Index of message.

param *`attachmentIndex`* - Index of attachment.

return *`The attachment name String at the selected index.`* 

```C++
String getAttachmentName(size_t messageIndex, size_t attachmentIndex);
```




**Get attachment file size for selected attachment index and message index from search result.**

param *`messageIndex`* - Index of message.

param *`attachmentIndex`* - Index of attachment.

return *`The attachment file size in byte at the selected index.`* 

```C++
int getAttachmentFileSize(size_t messageIndex, size_t attachmentIndex);
```




**Get creation date of attachment for selected attachment index and message index from search result.**

param *`messageIndex`* - Index of message.

param *`attachmentIndex`* - Index of attachment.

return *`The attachment creation date String at the selected index.`* 

```C++
String getAttachmentCreationDate(size_t messageIndex, size_t attachmentIndex);
```




**Get attachment file type for selected attachment index and message index from search result.**

param *`messageIndex`* - Index of message.

param *`attachmentIndex`* - Index of attachment.

return *`File MIME String at the selected index`* e.g. image/jpeg.

```C++
String getAttachmentType(size_t messageIndex, size_t attachmentIndex);
```




**Clear IMAPData object data.**

```C++
void empty();
```



    
### SMTPData object call for sending Email.


**Set SMTP server login credentials**

param *`host`* - SMTP server e.g. smtp.gmail.com

param *`port`* - SMTP port.

param *`loginEmail`* - The account Email.

param *`loginPassword`* - The account password.

param *`rootCA`* - Root CA certificate base64 string

```C++
void setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword);

void setLogin(const String &host, uint16_t port, const String &loginEmail, const String &loginPassword, const char *rootCA);

```





**Set STARTTLS mode to enable STARTTLS protocol.**

param *`starttls`* - bool flag that enables STARTTLS mode

```C++
void setSTARTTLS(bool starttls);
```






**Set debug print to serial.**

param *`debug`* - bool flag to enable debug

```C++
void setDebug(bool debug);
```






**Set Sender info**

param *`fromName`* - Sender's name

param *`senderEmail`* - Sender's Email.

```C++
void setSender(const String &fromName, const String &senderEmail);
```





**Get Sender's name**

return *`Sender's name String.`*

```C++
String getFromName();
```





**Get Sender's Email.**

return *`Sender's Email String.`*

```C++
String getSenderEmail();
```




**Set Email priority or importance**

param *`priority`* - Number from 1 to 5, 1 for highest, 3 for normal and 5 for lowest priority

```C++
void setPriority(int priority);
```





**Set Email priority or importance.**

param *`priority`* - String (High, Normal or Low)

```C++
void setPriority(const String &priority);
```




**Get Email priority**

return *`Number`* represents Email priority (1 for highest, 3 for normal, 5 for low priority).

```C++
uint8_t getPriority();
```




**Add one or more recipient**

param *`email`* - Recipient Email String of one recipient.

To add multiple recipients, call addRecipient for each recipient.

```C++
void addRecipient(const String &email);
```




**Remove recipient**

param *`email`* - Recipient Email String.

```C++
void removeRecipient(const String &email);
```
 
 


**Remove recipient**

param *`index`* - Index of recipients in Email object that previously added.

```C++
void removeRecipient(uint8_t index);
```




**Clear all recipients**
```C++
void clearRecipient();
```
 
 
 
 
**Get one recipient**

param *`index`* - Index of recipients.

return *`Recipient Email`* String at the index.

```C++
String getRecipient(uint8_t index);
```





**Get number of recipients**

return *`Number`* of recipients.

```C++
uint8_t recipientCount();
```

  


**Set the Email subject.**

param *`subject`* - The subject.

```C++
void setSubject(const String &subject);
```




**Get the Email subject.**

return *`Subject String.`*

```C++
String getSubject();
```




**Set the Email message**

param *`message`* - The message can be in normal text or html format.

param *`htmlFormat`* - The html format flag, True for send the message as html format
  
```C++
void setMessage(const String &message, bool htmlFormat);
```




**Get the message**

return *`Message String.`*

```C++
String getMessage();
```




**Determine  message is being send in html format.**

return *`Boolean`* status.

```C++
bool htmlFormat();
```




 
**Add Carbon Copy (CC) Email.**

param *`email`* - The CC Email String.

```C++
void addCC(const String &email);
```





**Remove specified Carbon Copy (CC) Email**

param *`email`* - The CC Email String to remove.

```C++
void removeCC(const String &email);
```





**Remove specified Carbon Copy (CC) Email**

param *`index`* - The CC Email index to remove.
  
```C++
void removeCC(uint8_t index);
```





**Clear all Carbon Copy (CC) Emails**

```C++
void clearCC();
```





**Get Carbon Copy (CC) Email at specified index.**

param *`index`* - The CC Email index to get.
return *`The CC Email string`* at the index.

```C++
String getCC(uint8_t index);
```





**Get the number of Carbon Copy (CC) Email.**

return *`Number`* of CC Emails.

```C++
uint8_t ccCount();
```





**Add Blind Carbon Copy (BCC) Email**

param *`email`* - The BCC Email String.

```C++
void addBCC(const String &email);
```





**Remove specified Blind Carbon Copy (BCC) Email**

param *`email`* - The BCC Email String to remove.

```C++
void removeBCC(const String &email);
```





**Remove specified Blind Carbon Copy (BCC) Email**

param *`index`* - The BCC Email index to remove.

```C++
void removeBCC(uint8_t index);
```
  





**Clear all Blind Carbon Copy (BCC) Emails**

```C++
void clearBCC();
```





**Get Blind Carbon Copy (BCC) Email at specified index.**

param *`index`* - The BCC Email index to get.

return *`The BCC Email string`* at the index.

```C++
String getBCC(uint8_t index);
```





**Get the number of Blind Carbon Copy (BCC) Email**

return *`Number`* of BCC Emails.

```C++
uint8_t bccCount();
```





**Add attchement data (binary) from internal memory (flash or ram).**

param *`fileName`* - The file name String that recipient can be saved.

param *`mimeType`* - The MIME type of file (image/jpeg, image/png, text/plain...). Can be empty String.

param *`data`* - The byte array of data (uint8_t).

param *`size`* - The data length in byte.

```C++
void addAttachData(const String &fileName, const String &mimeType, uint8_t *data, uint16_t size);
```
 
 


 
**Remove specified attachment data.**

param *`fileName`* - The file name of the attachment data to remove.

```C++
void removeAttachData(const String &fileName);
```
 
 



**Remove specified attachment data.**

param *`index`* - The index of the attachment data (count only data type attachment) to remove. 

```C++
void removeAttachData(uint8_t index);
```




**Get the number of attachment data.**

return *`Number`* of attach data.

```C++
uint8_t attachDataCount();
```



**Add attchement file from SD card**

param *`fileName`* - The file name String that recipient can be saved.

param *`mimeType`* - The MIME type of file (image/jpeg, image/png, text/plain...). Can be omitted.

```C++
void addAttachFile(const String &filePath, const String &mimeType = "");
```




**Remove specified attachment file from Email object.**

param *`fileName`* - The file name of the attachment file to remove.

```C++
void removeAttachFile(const String &filePath);
```




**Set storage type for all attach files.**

param *`storageType`* - The storage type to read attach file, MailClientStorageType::SD or MailClientStorageType::SPIFFS.

```C++
void setFileStorageType(uint8_t storageType);
```






**Remove specified attachment file.**

param *`index`* - The index of the attachment file (count only file type attachment) to remove.

```C++
void removeAttachFile(uint8_t index);
```




**Clear all attachment data.**

```C++
void clearAttachData();
```




**Clear all attachment file.**

```C++
void clearAttachFile();
```




**Clear all attachments (both data and file type attachments).**

```C++
void clearAttachment();
```
 
 
 
 
**Get number of attachments (both data and file type attachments).**

return *`Number`* of all attachemnts.

```C++
uint8_t attachFileCount();
```





**Add one or more custom message header field.**
    
param *`field`* - custom header String inform of FIELD: VALUE

This header field will add to message header.

```C++
void addCustomMessageHeader(const String &field);
```






**Remove one custom message header field that previously added..**
    
param *`field`* - custom custom message header field String to remove.

```C++
void removeCustomMessageHeader(const String &field);
```





**Remove one custom message header field that previously added by its index.**
    
param *`index`* - custom message header field index (number) to remove.

  
```C++
void removeCustomMessageHeader(uint8_t index);
```





**Clear all ccustom message header field that previously added.**
  
```C++
void clearCustomMessageHeader();
```






**Get the number of custom message header field that previously added.**
    
return *`Number`* of custom message header field.

```C++
uint8_t CustomMessageHeaderCount();
```






**Get custom message header field that previously added by index.**
    
param *`index`* - The custom message header field index to get.

return *`The custom message header field string at the index`*.

```C++
String getCustomMessageHeader(uint8_t index);
```





**Clear all data from Email object to free memory.**

```C++
void empty();
```




  
**Set the Email sending status callback function to Email object.**

param *`sendCallback`* - The callback function that accept the sendStatusCallback param.

```C++
void setSendCallback(sendStatusCallback sendCallback);
```




__MailClient.Time functions__


**Get the time from NTP server and set to device.**

param *`gmtOffset`* - The GMT time offset in hour.

param *`daylightOffset`* - The Daylight time offset in hour.

return - *`Boolean`* type status indicates the success of operation.

This requires internet connectivity.

```C++
bool setClock(float gmtOffset, float daylightOffset);
```






**Get the Unix time.**

return - *`uint32_t`* value of current Unix time.

```C++
uint32_t getUnixTime();
```






**Get timestamp from defined year, month, date, hour, minute, and second.**

param *`year`* - Year.
param *`mon`* - Month (1 to 12).
param *`date`* - Date.
param *`hour`* - Hour.
param *`mins`* - Minute.
param *`sec`* - Second.

return - *`time_t`* value of timestamp.

```C++
time_t getTimestamp(int year, int mon, int date, int hour, int mins, int sec);
```






**Get current year.**

return - *`int`* value of current year.

```C++
int getYear();
```






**Get current month.**

return - *`int`* value of current month.

```C++
int getMonth();
```





**Get current date.**

return - *`int`* value of current date.

```C++
int getDay();
```






**Get current day of week.**

return - *`int`* value of day of week.

1 for sunday and 7 for saturday

```C++
int getDayOfWeek();
```





**Get current day of week in String.**

return - *`String`* value of day of week.

Returns sunday, monday, tuesday, wednesday, thurseday, friday and saturday.

```C++
String getDayOfWeekString();
```






**Get current hour.**

return - *`int`* value of current hour (0 to 23).

```C++
int getHour();
```






**Get current minute.**

return - *`int`* value of current minute (0 to 59).

```C++
int getMin();
```






**Get current second.**

return - *`int`* value of current second (0 to 59).

```C++
int getSecond();
```







**Get the total days of current year.**

return - *`int`* value of total days of current year.

```C++
int getNumberOfDayThisYear();
```






**Get the total days of from January 1, 1970 to specific date.**

param *`year`* - Year from 1970.
param *`mon`* - Month (1 to 12).
param *`date`* - Date.

return - *`int`* value of total days.

```C++
int getTotalDays(int year, int month, int day);
```





**Get the day of week from specific date.**

param *`year`* - Year.
param *`month`* - Month (1 to 12).
param *`day`* - Date.

return - *`int`* value of day of week.

1 for sunday and 7 for saturday

```C++
int dayofWeek(int year, int month, int day);
```






**Get the second of current hour.**

return - *`int`* value of current second.

```C++
int getCurrentSecond();
```





**Get the current timestamp.**

return - *`uint64_t`* value of current timestamp.

```C++
uint64_t getCurrentTimestamp();
```






**Get time (year, month, day, hour, minute, and second) from second counted from January 1, 1970.**

param *`secCount`* - The seconds from January 1, 1970 00.00.
param *`yrs`* - The return year.
param *`months`* - The return month.
param *`days`* - The return day.
param *`hr`* - The return hour.
param *`min`* - The return minute.
param *`sec`* - The return second.

```C++
void getTimeFromSec(int secCount, int &yrs, int &months, int &days, int &hr, int &min, int &sec);
```






## License

The MIT License (MIT)

Copyright (c) 2019 K. Suwatchai (Mobizt)


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




