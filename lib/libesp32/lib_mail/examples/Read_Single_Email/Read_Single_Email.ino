/**
 * This example will fetch or read the Email which the known message UID
 * was used for fetching.
 * 
 * Email: suwatchai@outlook.com
 * 
 * Github: https://github.com/mobizt/ESP-Mail-Client
 * 
 * Copyright (c) 2021 mobizt
 *
*/

/** To receive Email using Gmail, IMAP option should be enabled. https://support.google.com/mail/answer/7126229?hl=en
 * and also https://accounts.google.com/b/0/DisplayUnlockCaptcha
 * 
*/

/** For ESP8266, with BearSSL WiFi Client 
 * The memory reserved for completed valid SSL response from IMAP is 16 kbytes which
 * may cause your device out of memory reset in case the memory 
 * allocation error.
*/

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "################"
#define WIFI_PASSWORD "################"

/* The imap host name e.g. imap.gmail.com for GMail or outlook.office365.com for Outlook */
#define IMAP_HOST "################"

/** The imap port e.g. 
 * 143  or esp_mail_imap_port_143
 * 993 or esp_mail_imap_port_993
*/
#define IMAP_PORT 993

/* The log in credentials */
#define AUTHOR_EMAIL "################"
#define AUTHOR_PASSWORD "################"

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status);

/* Print the list of mailbox folders */
void printAllMailboxesInfo(IMAPSession &imap);

/* Print the selected folder info */
void printSelectedMailboxInfo(IMAPSession &imap);

/* Print all messages from the message list */
void printMessages(IMAPSession &imap);

/* Print all rfc822 messages included in the message */
void printRFC822Messages(IMAP_MSG_Item &msg);

/* Print all attachments info from the message */
void printAttacements(IMAP_MSG_Item &msg);

/* The IMAP Session object used for Email reading */
IMAPSession imap;


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

    /** Enable the debug via Serial port 
     * none debug or 0
     * basic debug or 1
    */
    imap.debug(1);

    /* Set the callback function to get the reading results */
    imap.callback(imapCallback);

    /** In case the SD card/adapter was used for the file storagge, the SPI pins can be configure from
     * MailClient.sdBegin function which may be different for ESP32 and ESP8266
     * For ESP32, assign all of SPI pins
     * MailClient.sdBegin(14,2,15,13)
     * Which SCK = 14, MISO = 2, MOSI = 15 and SS = 13
     * And for ESP8266, assign the CS pins of SPI port
     * MailClient.sdBegin(15)
     * Which pin 15 is the CS pin of SD card adapter
    */

    /* Declare the session config data */
    ESP_Mail_Session session;

    /** ########################################################
     * Some properties of IMAP_Config and ESP_Mail_Session data
     * accept the pointer to constant char i.e. const char*. 
     * 
     * You may assign a string literal to that properties like 
     * below example.
     *   
     * config.storage.saved_path = String("/email_data").c_str();
     * 
     * String folder = "INBOX";
     * imap.selectFolder(folder.c_str());
     * 
     * ###########################################################
    */

    /* Set the session config */
    session.server.host_name = IMAP_HOST;
    session.server.port = IMAP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;


    /* Setup the configuration for searching or fetching operation and its result */
    IMAP_Config config;

    /* Message UID to fetch or read e.g. 100 */
    config.fetch.uid = "100";

    /* Set seen flag */
    //config.fetch.set_seen = true;

    /* Search criteria */
    config.search.criteria = "";

    /* Also search the unseen message */
    config.search.unseen_msg = true;

    /* Set the storage to save the downloaded files and attachments */
    config.storage.saved_path = "/email_data";

    /** The file storage type e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and 
     * esp_mail_file_storage_type_sd 
    */
    config.storage.type = esp_mail_file_storage_type_flash;

    /** Set to download heades, text and html messaeges, 
     * attachments and inline images respectively.
    */
    config.download.header = true;
    config.download.text = true;
    config.download.html = true;
    config.download.attachment = true;
    config.download.inlineImg = true;

    /** Set to enable the results i.e. html and text messaeges 
     * which the content stored in the IMAPSession object is limited
     * by the option config.limit.msg_size.
     * The whole message can be download through config.download.text
     * or config.download.html which not depends on these enable options.
    */
    config.enable.html = true;
    config.enable.text = true;

    /* Set to enable the sort the result by message UID in the ascending order */
    config.enable.recent_sort = true;

    /* Set to report the download progress via the default serial port */
    config.enable.download_status = true;

    /* Set the limit of number of messages in the search results */
    config.limit.search = 5;

    /** Set the maximum size of message stored in 
     * IMAPSession object in byte
    */
    config.limit.msg_size = 512;

    /** Set the maximum attachments and inline images files size
     * that can be downloaded in byte. 
     * The file which its size is largger than this limit may be saved 
     * as truncated file.
    */
    config.limit.attachment_size = 1024 * 1024 * 5;


    /* Connect to server with the session and config */
    if (!imap.connect(&session, &config))
        return;

    /*  {Optional] */
    printAllMailboxesInfo(imap);

    /* Open or select the mailbox folder to read or search the message */
    if (!imap.selectFolder("INBOX"))
        return;

    /*  {Optional] */
    printSelectedMailboxInfo(imap);

    /* Read or search the Email and close the session */
    MailClient.readMail(&imap);

    /* Clear all stored data in IMAPSession object */
    imap.empty();
}

void loop()
{

}

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Show the result when reading finished */
    if (status.success())
    {
        /* Print the result */
        printMessages(imap);

        /* Clear all stored data in IMAPSession object */
        imap.empty();
        Serial.printf("Free Heap: %d", ESP.getFreeHeap());
    }
}

void printAllMailboxesInfo(IMAPSession &imap)
{
    /* Declare the folder collection class to get the list of mailbox folders */
    FoldersCollection folders;

    /* Get the mailbox folders */
    if (imap.getFolders(folders))
    {
        for (size_t i = 0; i < folders.size(); i++)
        {
            /* Iterate each folder info using the  folder info item data */
            FolderInfo folderInfo = folders.info(i);
            Serial.printf("%s%s%s", i == 0 ? "\nAvailable folders: " : ", ", folderInfo.name, i == folders.size() - 1 ? "\n" : "");
        }
    }
}

void printSelectedMailboxInfo(IMAPSession &imap)
{
    /* Declare the selected folder info class to get the info of selected mailbox folder */
    SelectedFolderInfo sFolder = imap.selectedFolder();

    /* Show the mailbox info */
    Serial.printf("\nInfo of the selected folder\nTotal Messages: %d\n", sFolder.msgCount());
    Serial.printf("Predicted next UID: %d\n", sFolder.nextUID());
    for (size_t i = 0; i < sFolder.flagCount(); i++)
        Serial.printf("%s%s%s", i == 0 ? "Flags: " : ", ", sFolder.flag(i).c_str(), i == sFolder.flagCount() - 1 ? "\n" : "");
}

void printRFC822Messages(IMAP_MSG_Item &msg)
{
    Serial.printf("RFC822 Messages: %d message(s)\n****************************\n", msg.rfc822.size());
    for (size_t j = 0; j < msg.rfc822.size(); j++)
    {
        IMAP_MSG_Item rfc822 = msg.rfc822[j];
        Serial.printf("%d. \n", j + 1);
        Serial.printf("Messsage ID: %s\n", rfc822.messageID);
        Serial.printf("From: %s\n", rfc822.from);
        Serial.printf("Sender: %s\n", rfc822.sender);
        Serial.printf("To: %s\n", rfc822.to);
        Serial.printf("CC: %s\n", rfc822.cc);
        Serial.printf("Subject: %s\n", rfc822.subject);
        Serial.printf("Date: %s\n", rfc822.date);
        Serial.printf("Reply-To: %s\n", rfc822.reply_to);
        Serial.printf("Return-Path: %s\n", rfc822.return_path);
        Serial.printf("Comment: %s\n", rfc822.comment);
        Serial.printf("Keyword: %s\n", rfc822.keyword);
        Serial.printf("Text Message: %s\n", rfc822.text.content);
        Serial.printf("Text Message Charset: %s\n", rfc822.text.charSet);
        Serial.printf("Text Message Transfer Encoding: %s\n", rfc822.text.transfer_encoding);
        Serial.printf("HTML Message: %s\n", rfc822.html.content);
        Serial.printf("HTML Message Charset: %s\n", rfc822.html.charSet);
        Serial.printf("HTML Message Transfer Encoding: %s\n\n", rfc822.html.transfer_encoding);

        if (rfc822.attachments.size() > 0)
            printAttacements(rfc822);
    }
}

void printAttacements(IMAP_MSG_Item &msg)
{
    Serial.printf("Attachment: %d file(s)\n****************************\n", msg.attachments.size());
    for (size_t j = 0; j < msg.attachments.size(); j++)
    {
        IMAP_Attach_Item att = msg.attachments[j];
        /** att.type can be
         * esp_mail_att_type_none or 0
         * esp_mail_att_type_attachment or 1
         * esp_mail_att_type_inline or 2
        */
        Serial.printf("%d. Filename: %s, Name: %s, Size: %d, MIME: %s, Type: %s, Creation Date: %s\n", j + 1, att.filename, att.name, att.size, att.mime, att.type == esp_mail_att_type_attachment ? "attachment" : "inline", att.creationDate);
    }
    Serial.println();
}

void printMessages(IMAPSession &imap)
{
    /* Get the message list from the message list data */
    IMAP_MSG_List msgList = imap.data();

    for (size_t i = 0; i < msgList.msgItems.size(); i++)
    {
        /* Iterate to get each message data through the message item data */
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

        /* If the result contains the message info (Fetch mode) */
        if (!imap.headerOnly())
        {
            Serial.printf("Text Message: %s\n", msg.text.content);
            Serial.printf("Text Message Charset: %s\n", msg.text.charSet);
            Serial.printf("Text Message Transfer Encoding: %s\n", msg.text.transfer_encoding);
            Serial.printf("HTML Message: %s\n", msg.html.content);
            Serial.printf("HTML Message Charset: %s\n", msg.html.charSet);
            Serial.printf("HTML Message Transfer Encoding: %s\n\n", msg.html.transfer_encoding);

            if (msg.attachments.size() > 0)
                printAttacements(msg);

            if (msg.rfc822.size() > 0)
                printRFC822Messages(msg);
        }

        Serial.println();
    }
}
