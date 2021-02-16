# Mail Client Arduino Library for ESP32 and ESP8266 v 1.0.13

[![Join the chat at https://gitter.im/mobizt/ESP_Mail_Client](https://badges.gitter.im/mobizt/ESP_Mail_Client.svg)](https://gitter.im/mobizt/ESP_Mail_Client?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

The complete and secure Mail Client for ESP32 and ESP8266 devices for sending and reading the Email through the SMTP and IMAP servers.

With this library, the devices can both send and read the Email with many types of attachments supported and provides more reliable and flexibilities of usages.

The library was tested and work well with ESP32s and ESP8266s based modules.

This library was developed to replace the deprecated ESP32 Mail Client library with more options and features, better reliability and also conforms to the RFC standards.

![ESP32 Mail](/media/images/esp-mail-client.svg)

Copyright (c) 2021 K. Suwatchai (Mobizt).

# Features

* Support Espressif ESP32 and ESP8266 MCUs based devices.
* Support TCP session reusage.
* Support PLAIN, LOGIN and XOAUTH2 authentication mechanisms.
* Secured connection with SSL and TLS.
* Support mailbox selection for Email reading and searching.
* Support the content encodings e.g. quoted-printable and base64.
* Support the content decodings e.g. base64, UTF-8, UTF-7, quoted-printable, ISO-8859-1 (latin1) and ISO-8859-11 (Thai).
* Support many types of embedded contents e.g. inline images, attachments, parallel media attachments and RFC822 message.
* Support full debuging.
* Support SPIFFS and SD card for file storages.
* Support Ethernet (ESP32).
* Customizable operating configurations (see the examples for the usages)

## Tested Devices

This following devices were tested.

 * Sparkfun ESP32 Thing
 * NodeMCU-32
 * WEMOS LOLIN32
 * TTGO T8 V1.8
 * M5Stack ESP32
 * NodeMCU ESP8266
 * Wemos D1 Mini (ESP8266)



## Prerequisites


The library requires Arduino's ESP32 or ESP8266 Core SDK to be installed based on the platform.

The latest Core SDK is recommended. For ESP8266, the Core SDK version 2.6.3 or later is recommended.

The ESP8266 Core SDK version 2.5.x and earlier are not supported.



## Instalation


Click on **Clone or download** dropdown at the top of repository, select **Download ZIP** and save file on your computer.

From Arduino IDE, goto menu **Sketch** -> **Include Library** -> **Add .ZIP Library...** and choose **ESP-Mail-Client-master.zip** that previously downloaded.

Go to menu **Files** -> **Examples** -> **ESP-Mail-Client-master** and choose one from examples



## Usage


See [Full Examples](/examples) for complete usages.

See [Function Description](/src/README.md) for all available functions.


The following examples showed the minimum usage which many options are not configured.

The examples in the examples folder provide the full options usages.


### Send the Email


```C++

// Include ESP Mail Client library (this library)
#include <ESP_Mail_Client.h>


// Define the SMTP Session object which used for SMTP transsport
SMTPSession smtp;

// Define the session config data which used to store the TCP session configuration
ESP_Mail_Session session;

// Set the session config
session.server.host_name = "smtp.office365.com"; //for outlook.com
session.server.port = 587;
session.login.email = "your Email address"; //set to empty for no SMTP Authentication
session.login.password = "your Email password"; //set to empty for no SMTP Authentication
session.login.user_domain = "client domain or ip e.g. mydomain.com";

// Define the SMTP_Message class variable to handle to message being transport
SMTP_Message message;

// Set the message headers
message.sender.name = "My Mail";
message.sender.email = "sender or your Email address";
message.subject = "Test sending Email";
message.addRecipient("name1", "email1");
message.addRecipient("name2", "email2");

message.addCc("email3");
message.addBcc("email4");

// Set the message content
message.text.content = "This is simple plain text message";

//Base64 data of image
const char *greenImg = "iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAoUlEQVR42u3RAQ0AMAgAoJviyWxtAtNYwzmoQGT/eqwRQoQgRAhChCBECEKECBGCECEIEYIQIQgRghCECEGIEIQIQYgQhCBECEKEIEQIQoQgBCFCECIEIUIQIgQhCBGCECEIEYIQIQhBiBCECEGIEIQIQQhChCBECEKEIEQIQhAiBCFCECIEIUIQghAhCBGCECEIEYIQIUKEIEQIQoQg5LoBBaDPbQYiMoMAAAAASUVORK5CYII=";

// Define the attachment data
SMTP_Attachment att;

// Set the attatchment info
att.descr.filename = "green.png";
att.descr.mime = "image/png";
att.blob.data = (uint8_t *)greenImg;
att.blob.size = strlen(greenImg);
// Set the transfer encoding to base64
att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
// We set the content encoding to match the above greenImage data
att.descr.content_encoding = Content_Transfer_Encoding::enc_base64;

// Add attachment to the message
message.addAttachment(att);

// Connect to server with the session config
smtp.connect(&session);

// Start sending Email and close the session
if (!MailClient.sendMail(&smtp, &message))
  Serial.println("Error sending Email, " + smtp.errorReason());


```


### Read the Email


```C++

// Include ESP Mail Client library (this library)
#include <ESP_Mail_Client.h>


// Define the IMAP Session object which used for IMAP transsport
IMAP_Config config;


// Define the session config data which used to store the TCP session configuration
ESP_Mail_Session session;

// Set the session config
session.server.host_name = "outlook.office365.com"; //for outlook.com
session.server.port = 993;
session.login.email = "your Email address";
session.login.password = "your Email password";

// Define the config class variable for searching or fetching operation and store the messsagess data
IMAP_Config config;

// Define the message UID which required to fetch or read the message
config.fetch.uid = "100";

// Define the empty search criteria to disable the messsage search
config.search.criteria = "";

// Set to enable the message content which will be stored in the IMAP_Config data
config.enable.html = true;
config.enable.text = true;


// Connect to the server with the defined session and options
imap.connect(&session, &config);

// Open or select the mailbox folder to read the message
imap.selectFolder("INBOX");


// Read the Email and close the session
MailClient.readMail(&imap);


// Get the message(s) list
IMAP_MSG_List msgList = imap.data();

for (size_t i = 0; i < msgList.msgItems.size(); i++)
{
  // Iterate to get each message data through the message item data
  IMAP_MSG_Item msg = msgList.msgItems[i];

  Serial.println("################################");
  Serial.printf("Messsage Number: %s\n", msg.msgNo);
  Serial.printf("Messsage UID: %s\n", msg.UID);
  Serial.printf("Messsage ID: %s\n", msg.ID);
  Serial.printf("Accept Language: %s\n", msg.acceptLang);
  Serial.printf("Content Language: %s\n", msg.contentLang);
  Serial.printf("From: %s\n", msg.from);
  Serial.printf("From Charset: %s\n", msg.fromCharset);
  Serial.printf("To: %s\n", msg.to);
  Serial.printf("To Charset: %s\n", msg.toCharset);
  Serial.printf("CC: %s\n", msg.cc);
  Serial.printf("CC Charset: %s\n", msg.ccCharset);
  Serial.printf("Date: %s\n", msg.date);
  Serial.printf("Subject: %s\n", msg.subject);
  Serial.printf("Subject Charset: %s\n", msg.subjectCharset);

  // If the message body is available
  if (!imap.headerOnly())
  {
    Serial.printf("Text Message: %s\n", msg.text.content);
    Serial.printf("Text Message Charset: %s\n", msg.text.charSet);
    Serial.printf("Text Message Transfer Encoding: %s\n", msg.text.transfer_encoding);
    Serial.printf("HTML Message: %s\n", msg.html.content);
    Serial.printf("HTML Message Charset: %s\n", msg.html.charSet);
    Serial.printf("HTML Message Transfer Encoding: %s\n\n", msg.html.transfer_encoding);
  }
}

```



## License

The MIT License (MIT)

Copyright (c) 2021 K. Suwatchai (Mobizt)


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
