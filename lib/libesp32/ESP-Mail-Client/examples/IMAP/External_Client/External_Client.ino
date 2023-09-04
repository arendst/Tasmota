/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
 */

/**
 * This example shows how to send Email using EthernetClient.
 *
 * This example used ESP32 and WIZnet W5500 Ethernet module.
 *
 * Please see examples/SMTP/Ethernet and examples/SMTP/External_Client for more usage
 */

/** Note for library update from v2.x.x to v3.x.x.
 *
 *  Struct data names changed
 *
 * "ESP_Mail_Session" changes to "Session_Config"
 * "IMAP_Config" changes to "IMAP_Data"
 *
 * Changes in the examples
 *
 * ESP_Mail_Session session;
 * to
 * Session_Config config;
 *
 * IMAP_Config config;
 * to
 * IMAP_Data imap_data;
 */

#include <Arduino.h>

#include <ESP_Mail_Client.h>

#include <Ethernet.h>

#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<password>"

#define IMAP_HOST "<host>"
#define IMAP_PORT 993

#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

void imapCallback(IMAP_Status status);

void printAllMailboxesInfo(IMAPSession &imap);

void printSelectedMailboxInfo(SelectedFolderInfo sFolder);

void printMessages(std::vector<IMAP_MSG_Item> &msgItems, bool headerOnly);

void printAttacements(std::vector<IMAP_Attach_Item> &atts);

IMAPSession imap;

EthernetClient eth_client;

void networkConnection()
{
    // Reset the network connection
    WiFi.disconnect();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
        if (millis() - ms >= 5000)
        {
            Serial.println(" failed!");
            return;
        }
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}

void networkStatusRequestCallback()
{
    imap.setNetworkStatus(WiFi.status() == WL_CONNECTED);
}

void setup()
{

    Serial.begin(115200);

    Serial.println();

    MailClient.networkReconnect(true);

    // The WiFi credentials are required for SAMD21
    // due to it does not have reconnect feature.
    MailClient.clearAP();
    MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);

    networkConnection();

    imap.debug(1);

    /*
    For internal NTP client
    For times east of the Prime Meridian use 0-12
    For times west of the Prime Meridian add 12 to the offset.
    Ex. American/Denver GMT would be -6. 6 + 12 = 18
    See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
    */
    MailClient.setUDPClient(&udp_client, 0 /* GMT offset */);

    MailClient.networkReconnect(true);

    imap.callback(imapCallback);

    Session_Config config;

    config.server.host_name = IMAP_HOST;
    config.server.port = IMAP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    IMAP_Data imap_data;

    imap_data.search.criteria.clear();

    imap_data.search.unseen_msg = true;

    imap_data.storage.saved_path = F("/email_data");

    imap_data.storage.type = esp_mail_file_storage_type_flash;

    imap_data.download.header = true;
    imap_data.download.text = true;
    imap_data.download.html = true;
    imap_data.download.attachment = true;
    imap_data.download.inlineImg = true;

    imap_data.enable.html = true;
    imap_data.enable.text = true;

    imap_data.enable.recent_sort = true;

    imap_data.enable.download_status = true;

    imap_data.limit.search = 5;

    imap_data.limit.msg_size = 512;

    imap_data.limit.attachment_size = 1024 * 1024 * 5;

    imap.setClient(&eth_client);

    imap.networkConnectionRequestCallback(networkConnection);

    imap.networkStatusRequestCallback(networkStatusRequestCallback);

    if (!imap.connect(&config, &imap_data))
        return;

    if (imap.isAuthenticated())
        Serial.println("\nSuccessfully logged in.");
    else
        Serial.println("\nConnected with no Auth.");

    printAllMailboxesInfo(imap);

    if (!imap.selectFolder(F("INBOX")))
        return;

    printSelectedMailboxInfo(imap.selectedFolder());

    imap_data.fetch.uid = imap.getUID(imap.selectedFolder().msgCount());

    MailClient.readMail(&imap);

    imap.empty();
}

void loop()
{
}

void imapCallback(IMAP_Status status)
{
    Serial.println(status.info());

    if (status.success())
    {
        IMAP_MSG_List msgList = imap.data();
        printMessages(msgList.msgItems, imap.headerOnly());
        imap.empty();
    }
}

void printAllMailboxesInfo(IMAPSession &imap)
{
    FoldersCollection folders;

    if (imap.getFolders(folders))
    {
        for (size_t i = 0; i < folders.size(); i++)
        {
            FolderInfo folderInfo = folders.info(i);
            MailClient.printf("%s%s%s", i == 0 ? "\nAvailable folders: " : ", ", folderInfo.name, i == folders.size() - 1 ? "\n" : "");
        }
    }
}

void printSelectedMailboxInfo(SelectedFolderInfo sFolder)
{
    /* Show the mailbox info */
    MailClient.printf("\nInfo of the selected folder\nTotal Messages: %d\n", sFolder.msgCount());
    MailClient.printf("UID Validity: %d\n", sFolder.uidValidity());
    MailClient.printf("Predicted next UID: %d\n", sFolder.nextUID());
    if (sFolder.unseenIndex() > 0)
        MailClient.printf("First Unseen Message Number: %d\n", sFolder.unseenIndex());
    else
        MailClient.printf("Unseen Messages: No\n");

    if (sFolder.modSeqSupported())
        MailClient.printf("Highest Modification Sequence: %llu\n", sFolder.highestModSeq());
    for (size_t i = 0; i < sFolder.flagCount(); i++)
        MailClient.printf("%s%s%s", i == 0 ? "Flags: " : ", ", sFolder.flag(i).c_str(), i == sFolder.flagCount() - 1 ? "\n" : "");

    if (sFolder.flagCount(true))
    {
        for (size_t i = 0; i < sFolder.flagCount(true); i++)
            MailClient.printf("%s%s%s", i == 0 ? "Permanent Flags: " : ", ", sFolder.flag(i, true).c_str(), i == sFolder.flagCount(true) - 1 ? "\n" : "");
    }
}

void printAttacements(std::vector<IMAP_Attach_Item> &atts)
{
    MailClient.printf("Attachment: %d file(s)\n****************************\n", atts.size());
    for (size_t j = 0; j < atts.size(); j++)
    {
        IMAP_Attach_Item att = atts[j];
        MailClient.printf("%d. Filename: %s, Name: %s, Size: %d, MIME: %s, Type: %s, Description: %s, Creation Date: %s\n", j + 1, att.filename, att.name, att.size, att.mime, att.type == esp_mail_att_type_attachment ? "attachment" : "inline", att.description, att.creationDate);
    }
    Serial.println();
}

void printMessages(std::vector<IMAP_MSG_Item> &msgItems, bool headerOnly)
{

    for (size_t i = 0; i < msgItems.size(); i++)
    {
        IMAP_MSG_Item msg = msgItems[i];

        Serial.println("****************************");
        MailClient.printf("Number: %d\n", msg.msgNo);
        MailClient.printf("UID: %d\n", msg.UID);

        // The attachment status in search may be true in case the "multipart/mixed"
        // content type header was set with no real attachtment included.
        MailClient.printf("Attachment: %s\n", msg.hasAttachment ? "yes" : "no");

        MailClient.printf("Messsage-ID: %s\n", msg.ID);

        if (strlen(msg.flags))
            MailClient.printf("Flags: %s\n", msg.flags);
        if (strlen(msg.acceptLang))
            MailClient.printf("Accept Language: %s\n", msg.acceptLang);
        if (strlen(msg.contentLang))
            MailClient.printf("Content Language: %s\n", msg.contentLang);
        if (strlen(msg.from))
            MailClient.printf("From: %s\n", msg.from);
        if (strlen(msg.sender))
            MailClient.printf("Sender: %s\n", msg.sender);
        if (strlen(msg.to))
            MailClient.printf("To: %s\n", msg.to);
        if (strlen(msg.cc))
            MailClient.printf("CC: %s\n", msg.cc);
        if (strlen(msg.bcc))
            MailClient.printf("BCC: %s\n", msg.bcc);
        if (strlen(msg.date))
        {
            MailClient.printf("Date: %s\n", msg.date);
            MailClient.printf("Timestamp: %d\n", (int)MailClient.Time.getTimestamp(msg.date));
        }
        if (strlen(msg.subject))
            MailClient.printf("Subject: %s\n", msg.subject);
        if (strlen(msg.reply_to))
            MailClient.printf("Reply-To: %s\n", msg.reply_to);
        if (strlen(msg.return_path))
            MailClient.printf("Return-Path: %s\n", msg.return_path);
        if (strlen(msg.in_reply_to))
            MailClient.printf("In-Reply-To: %s\n", msg.in_reply_to);
        if (strlen(msg.references))
            MailClient.printf("References: %s\n", msg.references);
        if (strlen(msg.comments))
            MailClient.printf("Comments: %s\n", msg.comments);
        if (strlen(msg.keywords))
            MailClient.printf("Keywords: %s\n", msg.keywords);

        if (!headerOnly)
        {
            if (strlen(msg.text.content))
                MailClient.printf("Text Message: %s\n", msg.text.content);
            if (strlen(msg.text.charSet))
                MailClient.printf("Text Message Charset: %s\n", msg.text.charSet);
            if (strlen(msg.text.transfer_encoding))
                MailClient.printf("Text Message Transfer Encoding: %s\n", msg.text.transfer_encoding);
            if (strlen(msg.html.content))
                MailClient.printf("HTML Message: %s\n", msg.html.content);
            if (strlen(msg.html.charSet))
                MailClient.printf("HTML Message Charset: %s\n", msg.html.charSet);
            if (strlen(msg.html.transfer_encoding))
                MailClient.printf("HTML Message Transfer Encoding: %s\n\n", msg.html.transfer_encoding);

            if (msg.rfc822.size() > 0)
            {
                MailClient.printf("\r\nRFC822 Messages: %d message(s)\n****************************\n", msg.rfc822.size());
                printMessages(msg.rfc822, headerOnly);
            }

            if (msg.attachments.size() > 0)
                printAttacements(msg.attachments);
        }

        Serial.println();
    }
}
