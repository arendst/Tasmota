/*
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2019 mobizt
 *
*/


//To use send Email for Gmail to port 465 (SSL), less secure app option should be enabled. https://myaccount.google.com/lesssecureapps?pli=1

//To receive Email for Gmail, IMAP option should be enabled. https://support.google.com/mail/answer/7126229?hl=en

/*
  ===========================================================================================================================
  To prevent stack overrun in case of you want to download email attachments in IMAP readMail,
  increase the stack size in app_main() in esp32 main.cpp will help by change the stack size from 8192 to any more value
  as following

  xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle, ARDUINO_RUNNING_CORE);
  to
  xTaskCreatePinnedToCore(loopTask, "loopTask", 16384, NULL, 1, &loopTaskHandle, ARDUINO_RUNNING_CORE);

  For Arduino, file esp32's main.cpp is at C:\Users\USER_NAME\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.1\cores\esp32\main.cpp
  And for platformIO, that file is at C:\Users\USER_NAME\.platformio\packages\framework-arduinoespressif32\cores\esp32\main.cpp
  ===========================================================================================================================

*/

#include <Arduino.h>
#include "ESP32_MailClient.h"
#include "SD.h"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"



//The Email Reading data object contains config and data that received
IMAPData imapData;

//Callback function to get the Email reading status
void readCallback(ReadStatus info);

//List all files in SD card
void printDirectory(File &dir, int depth);

void readEmail();

unsigned long lastTime = 0;

void setup()
{

  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to AP");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println();

  MailClient.sdBegin();
  //MailClient.sdBegin(14,2,15,13); //SCK, MISO, MOSI,SS for TTGO T8 v1.7 or 1.8

  File dir = SD.open("/");

  printDirectory(dir, 0);

  Serial.println();

  imapData.setLogin("imap.gmail.com", 993, "YOUR_EMAIL_ACCOUNT@gmail.com", "YOUR_EMAIL_PASSWORD");
  imapData.setFolder("INBOX");

  //Clear fetch UID
  //If fetch UID was set, no search is perform.
  imapData.setFetchUID("");

  //imapData.setSearchCriteria("UID SINCE 10-Feb-2019");
  //imapData.setSearchCriteria("UID 700:*");
  //imapData.setSearchCriteria("UID SEARCH NOT SEEN");
  //imapData.setSearchCriteria("UID SEARCH UNSEEN");
  imapData.setSearchCriteria("UID SEARCH ALL");

  //To fetch or read one message UID = 320
  //imapData.setFechUID("320");

  //Set SD folder to save download messages and attachments
  imapData.setSaveFilePath("/email_data");

  //Save attachament
  imapData.setDownloadAttachment(true);

  //Set fetch/search result to return html message
  imapData.setHTMLMessage(true);

  //Set fetch/search result to return text message
  imapData.setTextMessage(true);

  //Set to save html message in SD card with decoded content.
  imapData.saveHTMLMessage(true, true);

  //Set to save text message in SD card with decoded content.
  imapData.saveTextMessage(true, true);

  //Set the maximum result when search criteria was set.
  imapData.setSearchLimit(10);

  //Set the sort order of returning message upon most recent received email.
  imapData.setRecentSort(true);

  //Set the return tex/html message size in byte.
  imapData.setMessageBufferSize(200);

  //Set the maximum attachment size 5 MB (each file)
  imapData.setAttachmentSizeLimit(1024 * 1024 * 5);

  //Set the Email receive callback function.
  imapData.setReadCallback(readCallback);

  //Set to get attachment downloading progress status.
  imapData.setDownloadReport(true);

  //Set the storage types to save download attachments or messages (SD is default)
  //imapData.setFileStorageType(MailClientStorageType::SPIFFS)
  imapData.setFileStorageType(MailClientStorageType::SD);

  MailClient.readMail(imapData);
}

void readEmail()
{

  Serial.println();
  Serial.println("Read Email...");

  imapData.setFetchUID("10");
  imapData.setSearchCriteria("");
  MailClient.readMail(imapData);

  imapData.setFetchUID("11");
  imapData.setSearchCriteria("");
  MailClient.readMail(imapData);

  imapData.setFetchUID("12");
  imapData.setSearchCriteria("");
  MailClient.readMail(imapData);
}

void loop()
{

  if (millis() - lastTime > 1000 * 60 * 3)
  {

    lastTime = millis();
    Serial.println(ESP.getFreeHeap());

    readEmail();
  }
}

//Callback function to get the Email reading status
void readCallback(ReadStatus msg)
{
  //Print the current status
  Serial.println("INFO: " + msg.info());

  if (msg.status() != "")
    Serial.println("STATUS: " + msg.status());

  //Show the result when reading finished
  if (msg.success())
  {

    for (int i = 0; i < imapData.availableMessages(); i++)
    {
      Serial.println("=================");

      //Search result number which varied upon search crieria
      Serial.println("Messsage Number: " + imapData.getNumber(i));

      //UID only available when assigned UID keyword in setSearchCriteria
      //e.g. imapData.setSearchCriteria("UID SEARCH ALL");
      Serial.println("Messsage UID: " + imapData.getUID(i));
      Serial.println("Messsage ID: " + imapData.getMessageID(i));
      Serial.println("Accept Language: " + imapData.getAcceptLanguage(i));
      Serial.println("Content Language: " + imapData.getContentLanguage(i));
      Serial.println("From: " + imapData.getFrom(i));
      Serial.println("From Charset: " + imapData.getFromCharset(i));
      Serial.println("To: " + imapData.getTo(i));
      Serial.println("To Charset: " + imapData.getToCharset(i));
      Serial.println("CC: " + imapData.getCC(i));
      Serial.println("CC Charset: " + imapData.getCCCharset(i));
      Serial.println("Date: " + imapData.getDate(i));
      Serial.println("Subject: " + imapData.getSubject(i));
      Serial.println("Subject Charset: " + imapData.getSubjectCharset(i));

      //If setHeaderOnly to false;
      if (!imapData.isHeaderOnly())
      {
        Serial.println("Text Message: " + imapData.getTextMessage(i));
        Serial.println("Text Message Charset: " + imapData.getTextMessgaeCharset(i));
        Serial.println("HTML Message: " + imapData.getHTMLMessage(i));
        Serial.println("HTML Message Charset: " + imapData.getHTMLMessgaeCharset(i));
        if (imapData.isFetchMessageFailed(i))
          Serial.println("Fetch Error: " + imapData.getFetchMessageFailedReason(i));

        if (imapData.isDownloadMessageFailed(i))
          Serial.println("Save Content Error: " + imapData.getDownloadMessageFailedReason(i));

        if (imapData.getAttachmentCount(i) > 0)
        {

          Serial.println("**************");
          Serial.println("Attachment: " + String(imapData.getAttachmentCount(i)) + " file(s)");

          for (int j = 0; j < imapData.getAttachmentCount(i); j++)
          {
            Serial.println("File Index: " + String(j + 1));
            Serial.println("Filename: " + imapData.getAttachmentFileName(i, j));
            Serial.println("Name: " + imapData.getAttachmentName(i, j));
            Serial.println("Size: " + String(imapData.getAttachmentFileSize(i, j)));
            Serial.println("Type: " + imapData.getAttachmentType(i, j));
            Serial.println("Creation Date: " + imapData.getAttachmentCreationDate(i, j));
            if (imapData.isDownloadAttachmentFailed(i, j))
              Serial.println("Download Attachment Error: " + imapData.getDownloadAttachmentFailedReason(i, j));
          }
        }
      }

      Serial.println();
    }
  }
}

//List all files in SD card
void printDirectory(File &dir, int depth)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    for (uint8_t i = 0; i < depth; i++)
      Serial.print("|    ");

    std::string name = entry.name();
    if (entry.isDirectory())
    {
      Serial.print("+----" + String(name.substr(name.find_last_of("/\\") + 1).c_str()) + "\r\n");
      printDirectory(entry, depth + 1);
    }
    else
    {
      Serial.print("+--" + String(name.substr(name.find_last_of("/\\") + 1).c_str()));
      Serial.print("\t\t\t(");
      Serial.print(entry.size(), DEC);
      Serial.println(")");
    }
    entry.close();
  }
}
