

/**
 * This example will send the Email with inline images stored in flash memory.
 * The message content stores as HTML data in flash memory
 * 
 * The html and text version messages will be sent.
 * 
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: suwatchai@outlook.com
 * 
 * Github: https://github.com/mobizt/ESP-Mail-Client
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//To use send Email for Gmail to port 465 (SSL), less secure app option should be enabled. https://myaccount.google.com/lesssecureapps?pli=1

//The file systems for flash and sd memory can be changed in ESP_Mail_FS.h.

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "################"
#define WIFI_PASSWORD "################"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com
 * For yahoo mail, log in to your yahoo mail in web browser and generate app password by go to
 * https://login.yahoo.com/account/security/app-passwords/add/confirm?src=noSrc
 * and use the app password as password with your yahoo mail account to login.
 * The google app password signin is also available https://support.google.com/mail/answer/185833?hl=en
*/
#define SMTP_HOST "################"

/** The smtp port e.g. 
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
*/
#define SMTP_PORT esp_mail_smtp_port_587

/* The log in credentials */
#define AUTHOR_EMAIL "################"
#define AUTHOR_PASSWORD "################"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

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

    Serial.println("Mounting SPIFFS...");

    const char *html = "<span style=\"color:#ff0000;\">This message contains 2 inline images.</span><br/><br/><img src=\"cid:image-001\" alt=\"orange image\"  width=\"100\" height=\"100\"> <img src=\"cid:image-002\" alt=\"green image\" width=\"100\" height=\"100\">";
    const char *orangeImg = "iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAoUlEQVR42u3RMQ0AMAgAsCFgftHLiQpsENJaaFT+fqwRQoQgRAhChCBECEKECBGCECEIEYIQIQgRghCECEGIEIQIQYgQhCBECEKEIEQIQoQgBCFCECIEIUIQIgQhCBGCECEIEYIQIQhBiBCECEGIEIQIQQhChCBECEKEIEQIQhAiBCFCECIEIUIQghAhCBGCECEIEYIQIUKEIEQIQoQg5LoBGi/oCaOpTXoAAAAASUVORK5CYII=";
    const char *greenImg = "iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAoUlEQVR42u3RAQ0AMAgAoJviyWxtAtNYwzmoQGT/eqwRQoQgRAhChCBECEKECBGCECEIEYIQIQgRghCECEGIEIQIQYgQhCBECEKEIEQIQoQgBCFCECIEIUIQIgQhCBGCECEIEYIQIQhBiBCECEGIEIQIQQhChCBECEKEIEQIQhAiBCFCECIEIUIQghAhCBGCECEIEYIQIUKEIEQIQoQg5LoBBaDPbQYiMoMAAAAASUVORK5CYII=";

#if defined(ESP32)
    if (SPIFFS.begin(true))
#elif defined(ESP8266)
    if (SPIFFS.begin())
#endif
    {

        //SPIFFS.format();

        if (SPIFFS.exists("/orange.png"))
            SPIFFS.remove("/orange.png");
        if (SPIFFS.exists("/green.png"))
            SPIFFS.remove("/green.png");
        if (SPIFFS.exists("/msg.html"))
            SPIFFS.remove("/msg.html");

        Serial.println("Preparing SPIFFS attachments...");

#if defined(ESP32)
        File file = SPIFFS.open("/orange.png", FILE_WRITE);
#elif defined(ESP8266)
        File file = SPIFFS.open("/orange.png", "w");
#endif
        file.print(orangeImg);
        file.close();

#if defined(ESP32)
        file = SPIFFS.open("/green.png", FILE_WRITE);
#elif defined(ESP8266)
        file = SPIFFS.open("/green.png", "w");
#endif
        file.print(greenImg);
        file.close();

#if defined(ESP32)
        file = SPIFFS.open("/msg.html", FILE_WRITE);
#elif defined(ESP8266)
        file = SPIFFS.open("/msg.html", "w");
#endif
        file.print(html);
        file.close();
    }
    else
    {
        Serial.println("SPIFFS Monting Failed");
    }

    /** Enable the debug via Serial port
     * none debug or 0
     * basic debug or 1
    */
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    /* Declare the session config data */
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "mydomain.net";

    /* Declare the message class */
    SMTP_Message message;

    /* Enable the chunked data transfer with pipelining for large message if server supported */
    message.enable.chunking = true;

    /* Set the message headers */
    message.sender.name = "ESP Mail";
    message.sender.email = AUTHOR_EMAIL;

    message.subject = "Test sending Email with message content and inline images stored in flash memory";
    message.addRecipient("user1", "####@#####_dot_com");

    /* Two alternative content versions are sending in this example e.g. plain text and html */

    /* Assign blob data (in flash or ram) as HTML message */
    message.html.blob.data = (const uint8_t *)html;
    message.html.blob.size = strlen(html);

    //Or get the content from file
    //message.html.file.name = "/msg.html";
    //message.html.file.type = esp_mail_file_storage_type_flash;

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
    message.html.charSet = "utf-8";

    message.text.content = "This message contains 2 inline images.\r\nThe inline images were not shown in the plain text message.";
    message.text.charSet = "utf-8";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    /* Set the custom message header */
    message.addHeader("Message-ID: <user1@gmail.com>");

    /* The attachment data item */
    SMTP_Attachment att;

    /** Set the inline image info e.g.
     * file name, MIME type, file path, file storage type,
     * transfer encoding and content encoding
    */
    att.descr.filename = "orange.png";
    att.descr.mime = "image/png";
    att.file.path = "/orange.png";
    att.descr.content_id = "image-001"; //The content id (cid) of orange image in the src tag

    /** The file storage type e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and
     * esp_mail_file_storage_type_sd 
    */
    att.file.storage_type = esp_mail_file_storage_type_flash;

    /* Need to be base64 transfer encoding for inline image */
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    /** The orange.png file is already base64 encoded file.
     * Then set the content encoding to match the transfer encoding
     * which no encoding was taken place prior to sending.
    */
    att.descr.content_encoding = Content_Transfer_Encoding::enc_base64;

    /* Add inline image to the message */
    message.addInlineImage(att);

    /** Set the inline image info e.g.
     * file name, MIME type, file path, file storage type,
     * transfer encoding and content encoding
    */
    message.resetAttachItem(att); //Clear the attach item data to reuse
    att.descr.filename = "green.png";
    att.descr.mime = "image/png";
    att.file.path = "/green.png";
    att.descr.content_id = "image-002"; //The content id (cid) of green image in the src tag
    att.file.storage_type = esp_mail_file_storage_type_flash;
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
    att.descr.content_encoding = Content_Transfer_Encoding::enc_base64;
    message.addInlineImage(att);

    /* Connect to server with the session config */
    if (!smtp.connect(&session))
        return;

    /* Start sending the Email and close the session */
    if (!MailClient.sendMail(&smtp, &message, true))
        Serial.println("Error sending Email, " + smtp.errorReason());
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
        Serial.println("----------------");
        Serial.printf("Message sent success: %d\n", status.completedCount());
        Serial.printf("Message sent failled: %d\n", status.failedCount());
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < smtp.sendingResult.size(); i++)
        {
            /* Get the result item */
            SMTP_Result result = smtp.sendingResult.getItem(i);
            localtime_r(&result.timesstamp, &dt);

            Serial.printf("Message No: %d\n", i + 1);
            Serial.printf("Status: %s\n", result.completed ? "success" : "failed");
            Serial.printf("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
            Serial.printf("Recipient: %s\n", result.recipients);
            Serial.printf("Subject: %s\n", result.subject);
        }
        Serial.println("----------------\n");
    }
}
