
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example showes how to send a reply message when specific email was received.

// The account 2 will send Hello message to account 1.

// The account 1 will poll the mailbox for incoming message, when new message received with matched subject
// and sent from account 2, the account 1 will send a reply messsage to account 2.

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

/* The imap host name e.g. imap.gmail.com for GMail or outlook.office365.com for Outlook */
#define IMAP_HOST "<imap host for account 1>"
#define IMAP_PORT 993

#define IMAP_AUTHOR_EMAIL "<email for account 1>"
#define IMAP_AUTHOR_PASSWORD "<password for account 1>"

#define REPLY_SMTP_AUTHOR_EMAIL "<email for account 1>"
#define REPLY_SMTP_AUTHOR_PASSWORD "<password for account 1>"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define REPLY_SMTP_PORT 587
#define REPLY_SMTP_HOST "<smtp host for account 1>"

#define HELLO_SMTP_AUTHOR_EMAIL "<email for account 2>"
#define HELLO_SMTP_AUTHOR_PASSWORD "<password for account 2>"

#define HELLO_SMTP_PORT 587
#define HELLO_SMTP_HOST "<smtp host for account 2>"

void setupIMAP();

bool setupHelloSMTP();

bool setupReplySMTP();

void sendHelloMessage();

void sendReplyMessage(const char *subject, const char *reply_email, const char *msgID, const char *references);

/* Print the selected folder update info */
void printPollingStatus(IMAPSession &imap);

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status);

/* Callback function to get the Email sending status */
void helloSMTPCallback(SMTP_Status status);

void replySMTPCallback(SMTP_Status status);

/* Declare the global used IMAPSession object for IMAP transport */
IMAPSession imap;

/*  Declare the global used Session_Config for user defined IMAP session credentials */
Session_Config imap_config;

/** Define the IMAP_Data object used for user defined IMAP operating options
 * and contains the IMAP operating result
 */
IMAP_Data imap_data;

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession hello_smtp;
SMTPSession reply_smtp;

/* Declare the global used Session_Config for user defined SMTP session credentials */
Session_Config hello_smtp_config;
Session_Config reply_smtp_config;

bool imapSetupOk = false;

unsigned long helloSendingMillis = 0;

String sendingSubject = "ESP Mail Hello Test!";

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

    Serial.print("Setup and connect to IMAP server... ");

    setupIMAP();

    if (!imapSetupOk)
        return;
}

void loop()
{

    /* imap.connect and imap.selectFolder or imap.openFolder nedded to be called once prior to listen */

    // Listen for mailbox changes
    if (!imap.listen())
        return;

    // Check the changes
    if (imap.folderChanged())
        printPollingStatus(imap);

    // To stop listen, use imap.stopListen(); and to listen again, call imap.listen()

    if (millis() - helloSendingMillis > 5 * 60 * 1000 || helloSendingMillis == 0)
    {
        helloSendingMillis = millis();
        Serial.print("Sending Hello message... ");
        sendHelloMessage();
    }
}

void setupIMAP()
{
    imap.debug(1);

    /* Set the callback function to get the reading results */
    imap.callback(imapCallback);

    /* Set the imap app config */
    imap_config.server.host_name = IMAP_HOST;
    imap_config.server.port = IMAP_PORT;
    imap_config.login.email = IMAP_AUTHOR_EMAIL;
    imap_config.login.password = IMAP_AUTHOR_PASSWORD;

    /* Connect to the server */
    if (!imap.connect(&imap_config, &imap_data))
        return;

    if (imap.isAuthenticated())
        Serial.println("\nIMAP client, successfully logged in.");
    else
        Serial.println("\nIMAP client, connected with no Auth.");

    /* Open or select the mailbox folder to read or search the message */
    if (!imap.selectFolder(F("INBOX")))
        return;

    imapSetupOk = true;
}

bool setupHelloSMTP()
{
    hello_smtp.debug(1);

    /* Set the callback function to get the sending results */
    hello_smtp.callback(helloSMTPCallback);

    /* Set the session config */
    hello_smtp_config.server.host_name = HELLO_SMTP_HOST;
    hello_smtp_config.server.port = HELLO_SMTP_PORT;
    hello_smtp_config.login.email = HELLO_SMTP_AUTHOR_EMAIL;
    hello_smtp_config.login.password = HELLO_SMTP_AUTHOR_PASSWORD;

    /** Assign your host name or you public IPv4 or IPv6 only
     * as this is the part of EHLO/HELO command to identify the client system
     * to prevent connection rejection.
     * If host name or public IP is not available, ignore this or
     * use loopback address "127.0.0.1".
     *
     * Assign any text to this option may cause the connection rejection.
     */
    hello_smtp_config.login.user_domain = F("127.0.0.1");

    /*
    Set the NTP config time
    For times east of the Prime Meridian use 0-12
    For times west of the Prime Meridian add 12 to the offset.
    Ex. American/Denver GMT would be -6. 6 + 12 = 18
    See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
    */
    hello_smtp_config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    hello_smtp_config.time.gmt_offset = 3;
    hello_smtp_config.time.day_light_offset = 0;

    /* Connect to the server */
    if (!hello_smtp.connect(&hello_smtp_config))
    {
        MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s", hello_smtp.statusCode(), hello_smtp.errorCode(), hello_smtp.errorReason().c_str());
        return false;
    }

    if (imap.isAuthenticated())
        Serial.println("\nHello SMTP client, successfully logged in.");
    else
        Serial.println("\nHello SMTP client, connected with no Auth.");

    return true;
}

bool setupReplySMTP()
{
    reply_smtp.debug(1);

    /* Set the callback function to get the sending results */
    reply_smtp.callback(replySMTPCallback);

    /* Set the session config */
    reply_smtp_config.server.host_name = REPLY_SMTP_HOST;
    reply_smtp_config.server.port = REPLY_SMTP_PORT;
    reply_smtp_config.login.email = REPLY_SMTP_AUTHOR_EMAIL;
    reply_smtp_config.login.password = REPLY_SMTP_AUTHOR_PASSWORD;
    reply_smtp_config.login.user_domain = F("127.0.0.1");

    /* Connect to the server */
    if (!reply_smtp.connect(&reply_smtp_config))
    {
        MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s", reply_smtp.statusCode(), reply_smtp.errorCode(), reply_smtp.errorReason().c_str());
        return false;
    }

    if (imap.isAuthenticated())
        Serial.println("\nReply SMTP client, successfully logged in.");
    else
        Serial.println("\nReply SMTP client, connected with no Auth.");

    return true;
}

void sendHelloMessage()
{

    if (!setupHelloSMTP())
        return;

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = F("ESP Mail");
    message.sender.email = HELLO_SMTP_AUTHOR_EMAIL;
    message.subject = sendingSubject.c_str();
    message.addRecipient(F("Me"), IMAP_AUTHOR_EMAIL);
    message.response.reply_to = HELLO_SMTP_AUTHOR_EMAIL; // only email address, excluded < and >
    message.text.content = F("Hello Me!");

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&hello_smtp, &message))
        MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s", hello_smtp.statusCode(), hello_smtp.errorCode(), hello_smtp.errorReason().c_str());
}

void sendReplyMessage(const char *subject, const char *reply_email, const char *msgID, const char *references)
{

    if (!setupReplySMTP())
        return;

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = F("ESP Mail");
    message.sender.email = REPLY_SMTP_AUTHOR_EMAIL;
    String reSubject = "RE: ";
    reSubject += subject;
    message.subject = reSubject;
    message.addRecipient(F("Me"), reply_email);

    message.in_reply_to = msgID;

    String ref = references;
    if (strlen(references) > 0)
        ref += " ";
    ref += msgID;

    message.references = ref;
    message.text.content = F("Yeah!, it works.");

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&reply_smtp, &message))
        MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s", reply_smtp.statusCode(), reply_smtp.errorCode(), reply_smtp.errorReason().c_str());
}

void printPollingStatus(IMAPSession &imap)
{
    /* Declare the selected folder info class to get the info of selected mailbox folder */
    SelectedFolderInfo sFolder = imap.selectedFolder();

    if (sFolder.pollingStatus().type == imap_polling_status_type_new_message)
    {
        /* Show the mailbox info */
        MailClient.printf("\nMailbox status changed\n----------------------\nTotal Messages: %d\n", sFolder.msgCount());

        MailClient.printf("New message %d, has been addedd, reading message...\n", (int)sFolder.pollingStatus().messageNum);

        // we need to stop polling before do anything
        imap.stopListen();

        // Get the UID of new message and fetch
        imap_data.fetch.uid = imap.getUID(sFolder.pollingStatus().messageNum);

        // When message was fetched or read, the /Seen flag will not set or message remained in unseen or unread status,
        // as this is the purpose of library (not UI application), user can set the message status as read by set \Seen flag
        // to message, see the Set_Flags.ino example.
        MailClient.readMail(&imap, false);
    }
}

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Show the result when reading finished */
    if (status.success())
    {

        /* Get the message list from the message list data */
        IMAP_MSG_List msgList = imap.data();

        if (strcmp(msgList.msgItems[0].subject, sendingSubject.c_str()) == 0)
        {
            Serial.print("Sending Reply message... ");
            std::string replyEmail = msgList.msgItems[0].reply_to;

            // remove < at the beginning and > at the end.
            replyEmail.erase(0, 1);
            replyEmail.pop_back();

            sendReplyMessage(msgList.msgItems[0].subject, replyEmail.c_str(), msgList.msgItems[0].ID, msgList.msgItems[0].references);
        }

        /* Clear all stored data in IMAPSession object */
        imap.empty();
    }
}

/* Callback function to get the Email sending status */
void helloSMTPCallback(SMTP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Print the sending result */
    if (status.success())
    {
        Serial.println("----------------");
        MailClient.printf("Message sent success: %d\n", status.completedCount());
        MailClient.printf("Message sent failed: %d\n", status.failedCount());
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < hello_smtp.sendingResult.size(); i++)
        {
            /* Get the result item */
            SMTP_Result result = hello_smtp.sendingResult.getItem(i);

            MailClient.printf("Message No: %d\n", i + 1);
            MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
            MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
            MailClient.printf("Recipient: %s\n", result.recipients.c_str());
            MailClient.printf("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");

        // You need to clear sending result as the memory usage will grow up
        hello_smtp.sendingResult.clear();
    }
}

void replySMTPCallback(SMTP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Print the sending result */
    if (status.success())
    {
        Serial.println("----------------");
        MailClient.printf("Message sent success: %d\n", status.completedCount());
        MailClient.printf("Message sent failed: %d\n", status.failedCount());
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < reply_smtp.sendingResult.size(); i++)
        {
            /* Get the result item */
            SMTP_Result result = reply_smtp.sendingResult.getItem(i);

            MailClient.printf("Message No: %d\n", i + 1);
            MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
            MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
            MailClient.printf("Recipient: %s\n", result.recipients.c_str());
            MailClient.printf("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");

        reply_smtp.sendingResult.clear();
    }
}
