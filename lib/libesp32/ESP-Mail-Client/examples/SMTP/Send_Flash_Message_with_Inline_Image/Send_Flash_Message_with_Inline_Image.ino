
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example shows how to send Email with inline images stored in flash memory.

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

// The file systems for flash and sd memory can be changed in ESP_Mail_FS.h.

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

#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<password>"

/** For Gmail, the app password will be used for log in
 *  Check out https://github.com/mobizt/ESP-Mail-Client#gmail-smtp-and-imap-required-app-passwords-to-sign-in
 *
 * For Yahoo mail, log in to your yahoo mail in web browser and generate app password by go to
 * https://login.yahoo.com/account/security/app-passwords/add/confirm?src=noSrc
 *
 * To use Gmai and Yahoo's App Password to sign in, define the AUTHOR_PASSWORD with your App Password
 * and AUTHOR_EMAIL with your account email.
 */

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "<host>"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT esp_mail_smtp_port_587

/* The log in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

/* Recipient email address */
#define RECIPIENT_EMAIL "<recipient email here>"

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

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

    Serial.println("Mounting flash...");

    const char *html = "<span style=\"color:#023ac7;\">This message contains inline image.</span><br/><br/><img src=\"cid:image-001\" alt=\"orange image\"  width=\"640\" height=\"360\">";
    const char *base64Img = "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";

#if defined(ESP32) || defined(ESP8266)
#if defined(ESP32)
    if (ESP_MAIL_DEFAULT_FLASH_FS.begin(true))
#elif defined(ESP8266)
    if (ESP_MAIL_DEFAULT_FLASH_FS.begin())
#endif
    {

        // ESP_MAIL_DEFAULT_FLASH_FS.format();

        if (ESP_MAIL_DEFAULT_FLASH_FS.exists("/base64Img.jpg"))
            ESP_MAIL_DEFAULT_FLASH_FS.remove("/base64Img.jpg");

        Serial.println("Preparing flash file attachments...");

#if defined(ESP32)
        File file = ESP_MAIL_DEFAULT_FLASH_FS.open("/base64Img.jpg", FILE_WRITE);
#elif defined(ESP8266)
        File file = ESP_MAIL_DEFAULT_FLASH_FS.open("/base64Img.jpg", "w");
#endif
        file.print(base64Img);
        file.close();

#if defined(ESP32)
        file = ESP_MAIL_DEFAULT_FLASH_FS.open("/msg.html", FILE_WRITE);
#elif defined(ESP8266)
        file = ESP_MAIL_DEFAULT_FLASH_FS.open("/msg.html", "w");
#endif
        file.print(html);
        file.close();
    }
    else
    {
        Serial.println("Flash filesystem monting Failed");
    }
#endif

    /*  Set the network reconnection option */
    MailClient.networkReconnect(true);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    MailClient.clearAP();
    MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    /** Enable the debug via Serial port
     * 0 for no debugging
     * 1 for basic level debugging
     *
     * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
     */
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    /* Declare the Session_Config for user defined session credentials */
    Session_Config config;

    /* Set the session config */
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    /** Assign your host name or you public IPv4 or IPv6 only
     * as this is the part of EHLO/HELO command to identify the client system
     * to prevent connection rejection.
     * If host name or public IP is not available, ignore this or
     * use loopback address "127.0.0.1".
     *
     * Assign any text to this option may cause the connection rejection.
     */
    config.login.user_domain = F("127.0.0.1");

    /*
    Set the NTP config time
    For times east of the Prime Meridian use 0-12
    For times west of the Prime Meridian add 12 to the offset.
    Ex. American/Denver GMT would be -6. 6 + 12 = 18
    See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
    */
    config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    config.time.gmt_offset = 3;
    config.time.day_light_offset = 0;

    /* Declare the message class */
    SMTP_Message message;

    /* Enable the chunked data transfer with pipelining for large message if server supported */
    message.enable.chunking = true;

    /* Set the message headers */
    message.sender.name = F("ESP Mail");
    message.sender.email = AUTHOR_EMAIL;

    message.subject = F("Test sending base64 inline image stored in flash memory");
    message.addRecipient(F("user1"), RECIPIENT_EMAIL);

    /* Two alternative content versions are sending in this example e.g. plain text and html */

    /* Assign blob data (in flash or ram) as HTML message */
    message.html.blob.data = (const uint8_t *)html;
    message.html.blob.size = strlen(html);

    // Or get the content from file
    // message.html.file.name = "/msg.html";
    // message.html.file.type = esp_mail_file_storage_type_flash;

    /** The content transfer encoding e.g.
     * enc_7bit or "7bit" (not encoded)
     * enc_qp or "quoted-printable" (encoded) <- not supported for message from blob and file
     * enc_base64 or "base64" (encoded)
     * enc_binary or "binary" (not encoded)
     * enc_8bit or "8bit" (not encoded)
     * The default value is "7bit"
     */
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /** The HTML text message character set e.g.
     * us-ascii
     * utf-8
     * utf-7
     * The default value is utf-8
     */
    message.html.charSet = F("utf-8");

    message.text.content = F("This message contains inline image.\r\nThe inline image was not shown in the plain text message.");
    message.text.charSet = F("utf-8");
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    /* Set the custom message header */
    message.addHeader(F("Message-ID: <user1@gmail.com>"));

    /* The attachment data item */
    SMTP_Attachment att;
    /** Set the inline image info e.g.
     * file name, MIME type, file path, file storage type,
     * transfer encoding and content encoding
     */
    att.descr.filename = F("base64Img.jpg");
    att.descr.mime = F("image/jpg");

#if defined(ESP32) || defined(ESP8266)
    att.file.path = F("/base64Img.jpg");
    /** The file storage type e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd
     */
    att.file.storage_type = esp_mail_file_storage_type_flash;
#elif defined(ARDUINO_ARCH_SAMD)
    att.blob.data = (const uint8_t *)base64Img;
    att.blob.size = sizeof(base64Img);
#endif

    att.descr.content_id = F("image-001"); // The content id (cid) of orange image in the src tag

    /* Need to be base64 transfer encoding for inline image */
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    /** The attach image file is already base64 encoded file.
     * Then set the content encoding to match the transfer encoding
     * which no encoding was taken place prior to sending.
     */
    att.descr.content_encoding = Content_Transfer_Encoding::enc_base64;

    /* Add inline image to the message */
    message.addInlineImage(att);

    /* Connect to the server */
    if (!smtp.connect(&config))
    {
        MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
        return;
    }

    if (smtp.isAuthenticated())
        Serial.println("\nSuccessfully logged in.");
    else
        Serial.println("\nConnected with no Auth.");

    /* Start sending the Email and close the session */
    if (!MailClient.sendMail(&smtp, &message, true))
        MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

    // to clear sending result log
    // smtp.sendingResult.clear();

    MailClient.printf("Free Heap: %d\n", MailClient.getFreeHeap());
}

void loop()
{
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Print the sending result */
    if (status.success())
    {
        // MailClient.printf used in the examples is for format printing via debug Serial port
        // that works for all supported Arduino platform SDKs e.g. SAMD, ESP32 and ESP8266.
        // In ESP8266 and ESP32, you can use Serial.printf directly.

        Serial.println("----------------");
        MailClient.printf("Message sent success: %d\n", status.completedCount());
        MailClient.printf("Message sent failed: %d\n", status.failedCount());
        Serial.println("----------------\n");

        for (size_t i = 0; i < smtp.sendingResult.size(); i++)
        {
            /* Get the result item */
            SMTP_Result result = smtp.sendingResult.getItem(i);

            // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
            // your device time was synched with NTP server.
            // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
            // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

            MailClient.printf("Message No: %d\n", i + 1);
            MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
            MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
            MailClient.printf("Recipient: %s\n", result.recipients.c_str());
            MailClient.printf("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");

        // You need to clear sending result as the memory usage will grow up.
        smtp.sendingResult.clear();
    }
}