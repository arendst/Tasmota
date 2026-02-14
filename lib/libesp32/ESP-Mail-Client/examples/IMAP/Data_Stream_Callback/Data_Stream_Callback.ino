/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example shows how to read Email and collect the stream data to print or store via the callback function.

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

/** Assign SD card type and FS used in src/ESP_Mail_FS.h and
 * change the config for that card interfaces in src/extras/SDHelper.h
 */

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <ESP_Mail_Client.h>

// Provide the SD card interfaces setting and mounting
#include <extras/SDHelper.h>

#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<password>"

/** For Gmail, IMAP option should be enabled. https://support.google.com/mail/answer/7126229?hl=en
 * and also https://accounts.google.com/b/0/DisplayUnlockCaptcha
 *
 * Some Gmail user still not able to sign in using account password even above options were set up,
 * for this case, use "App Password" to sign in instead.
 * About Gmail "App Password", go to https://support.google.com/accounts/answer/185833?hl=en
 *
 * For Yahoo mail, log in to your yahoo mail in web browser and generate app password by go to
 * https://login.yahoo.com/account/security/app-passwords/add/confirm?src=noSrc
 *
 * To use Gmai and Yahoo's App Password to sign in, define the AUTHOR_PASSWORD with your App Password
 * and AUTHOR_EMAIL with your account email.
 */

/* The imap host name e.g. imap.gmail.com for GMail or outlook.office365.com for Outlook */
#define IMAP_HOST "<host>"

/** The imap port e.g.
 * 143  or esp_mail_imap_port_143
 * 993 or esp_mail_imap_port_993
 */
#define IMAP_PORT 993

/* The log in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

void mimeDataStreamCallback(MIME_Data_Stream_Info streaminfo);

/* Declare the global used IMAPSession object for IMAP transport */
IMAPSession imap;

int progress = 0;
int lastProgress = -1;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void setup()
{

    Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
    while (!Serial)
        ;
#endif

    Serial.println();

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
        
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    unsigned long ms = millis();
#endif

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    /*  Set the network reconnection option */
    MailClient.networkReconnect(true);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    MailClient.clearAP();
    MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

#if defined(ESP_MAIL_DEFAULT_SD_FS) // defined in src/ESP_Mail_FS.h
    // Mount SD card.
    SD_Card_Mounting(); // See src/extras/SDHelper.h
#endif

    /* Set the callback function to get MIME Data stream */
    imap.mimeDataStreamCallback(mimeDataStreamCallback);

    /** In case the SD card/adapter was used for the file storagge, the SPI pins can be configure from
     * MailClient.sdBegin function which may be different for ESP32 and ESP8266
     * For ESP32, assign all of SPI pins
     * MailClient.sdBegin(14,2,15,13)
     * Which SCK = 14, MISO = 2, MOSI = 15 and SS = 13
     * And for ESP8266, assign the CS pins of SPI port
     * MailClient.sdBegin(15)
     * Which pin 15 is the CS pin of SD card adapter
     */

    /* Declare the Session_Config for user defined session credentials */
    Session_Config config;

    /* Set the session config */
    config.server.host_name = IMAP_HOST;
    config.server.port = IMAP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    /* Define the IMAP_Data object used for user defined IMAP operating options. */
    IMAP_Data imap_data;

    /* Set the storage to save the downloaded files and attachments */
    imap_data.storage.saved_path = F("/email_data");

    /** The file storage type e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd
     */
    imap_data.storage.type = esp_mail_file_storage_type_sd;

    /** Set to download headers, text and html messaeges,
     * attachments and inline images respectively.
     */
    imap_data.download.header = true;
    imap_data.download.text = true;
    imap_data.download.html = true;
    imap_data.download.attachment = true;
    imap_data.download.inlineImg = true;

    /** Set to enable the results i.e. html and text messaeges
     * which the content stored in the IMAPSession object is limited
     * by the option imap_data.limit.msg_size.
     * The whole message can be download through imap_data.download.text
     * or imap_data.download.html which not depends on these enable options.
     */
    imap_data.enable.html = true;
    imap_data.enable.text = true;

    /* Set to enable the sort the result by message UID in the decending order */
    imap_data.enable.recent_sort = true;

    /* Set to report the download progress via the default serial port */
    imap_data.enable.download_status = true;

    /* Header fields parsing is case insensitive by default to avoid uppercase header in some server e.g. iCloud
    , to allow case sensitive parse, uncomment below line*/
    // imap_data.enable.header_case_sensitive = true;

    /* Set the limit of number of messages in the search results */
    imap_data.limit.search = 5;

    /** Set the maximum size of message stored in
     * IMAPSession object in byte
     */
    imap_data.limit.msg_size = 512;

    /** Set the maximum attachments and inline images files size
     * that can be downloaded in byte.
     * The file which its size is largger than this limit may be saved
     * as truncated file.
     */
    imap_data.limit.attachment_size = 1024 * 1024 * 5;

    /* Connect to the server */
    if (!imap.connect(&config, &imap_data))
        return;

    if (imap.isAuthenticated())
        Serial.println("\nSuccessfully logged in.");
    else
        Serial.println("\nConnected with no Auth.");

    /* Open or select the mailbox folder to read or search the message */
    if (!imap.selectFolder(F("INBOX")))
        return;

    /** Message UID to fetch or read e.g. 100.
     * In this case we will get the UID from the max message number (lastest message)
     */
    imap_data.fetch.uid = imap.getUID(imap.selectedFolder().msgCount());

    /* Set seen flag */

    // The message with "Seen" flagged means the message was already read or seen by user.
    // The default value of this option is set to false.
    // If you want to set the message flag as "Seen", set this option to true.
    // If this option is false, the message flag was unchanged.
    // To set or remove flag from message, see Set_Flags.ino example.

    // imap_data.fetch.set_seen = true;

    /* Read or search the Email and close the session */

    // When message was fetched or read, the /Seen flag will not set or message remained in unseen or unread status,
    // as this is the purpose of library (not UI application), user can set the message status as read by set \Seen flag
    // to message, see the Set_Flags.ino example.
    MailClient.readMail(&imap);

    /* Clear all stored data in IMAPSession object */
    imap.empty();
}

void loop()
{
}

void mimeDataStreamCallback(MIME_Data_Stream_Info streaminfo)
{
    if (streaminfo.isFirstData)
    {
        progress = 0;
        lastProgress = -1;

        Serial.print("Message UID: ");
        Serial.println(streaminfo.uid);

        Serial.print("Content Type: ");
        Serial.println(streaminfo.type);

        Serial.print("Content Disposition: ");
        Serial.println(streaminfo.disposition);

        Serial.print("Text Character Set: ");
        Serial.println(streaminfo.charSet);

        Serial.print("Content Transfer Encoding: ");
        Serial.println(streaminfo.transfer_encoding);

        // The total octets of encoded or non-encoded MIME content.
        // The size of decoded content may be different.
        Serial.print("Total Octets: ");
        Serial.println(streaminfo.octet_size);

        if (strcmp(streaminfo.disposition, "attachment") == 0 || strcmp(streaminfo.disposition, "inline") == 0)
        {

            if (strcmp(streaminfo.disposition, "inline") == 0)
            {
                Serial.print("Content ID: ");
                Serial.println(streaminfo.cid);
            }

            Serial.print("Name: ");
            Serial.println(streaminfo.name);

            Serial.print("File Name: ");
            Serial.println(streaminfo.filename);

            Serial.print("Size: ");
            Serial.println(streaminfo.size);

            Serial.print("Content Description: ");
            Serial.println(streaminfo.description);

            Serial.print("Creation Date: ");
            Serial.println(streaminfo.date);
        }

        Serial.println("Content:");
    }

    progress = 100 * streaminfo.octet_count / streaminfo.octet_size;

    if (progress != lastProgress && (progress == 0 || progress == 100 || lastProgress + 5 <= progress))
    {

        lastProgress = progress;

        // The size of current decoded chunk data
        Serial.print("Data Length: ");
        Serial.print(streaminfo.data_size);

        Serial.print(", Reading %: ");
        Serial.println(progress);
    }

    // Decoded chunk data is available here
    if (streaminfo.data)
    {

        // If streaminfo.transfer_encoding is 'base64',
        // to print or send null terminated string from stream data

        /**
         char str[streaminfo.data_size + 1];
         memcpy(str, streaminfo.data, streaminfo.data_size);
         str[streaminfo.data_size] = 0;
         Serial.print(str);
        */

        // If streaminfo.transfer_encoding is not 'base64', the string can be
        // taken directly from casting streaminfo.data as (const char*)streaminfo.data

        // To write data to file (if fs is File class object that open in appended mode)
        // fs.write((uint8_t *)streaminfo.data, streaminfo.data_size);

        // streaminfo.data_size is not more than 512
    }

    if (streaminfo.isLastData)
    {
        Serial.println();
        Serial.println();
    }
}