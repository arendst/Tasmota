

/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example showes how to send Email using custom commands.

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

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "<host>"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465 // port 465 is not available for Outlook.com
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT esp_mail_smtp_port_587 // for STARTTLS

/* The log in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "-----END CERTIFICATE-----\n";

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void customCommandCallback(SMTP_Response res)
{

    // The res.id is the command identifier number that use to identify the source of command.

    // The command identifier number can be set via the last parameter of customConnect and sendCustomCommand functions.

    // If command identifier number was not set in those functions, the res.id received will be auto increased and begins with 0

    MailClient.printf("> C: Command ID %d\n", res.id);
    MailClient.printf("< S: %s\n", res.text.c_str());

    if (res.statusCode > 0)
    {
        MailClient.printf("> C: Response finished with status code %d\n\n", res.statusCode);
    }
}

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

    /* Declare the Session_Config for user defined session credentials */
    Session_Config config;

    /* Set the session config */
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    bool tls = false;

    /* Connect to the server */
    if (smtp.customConnect(&config /* session credentials */, customCommandCallback) != 220)
    {
        Serial.println("! E: Unable to connect to server");
        return;
    }

init:

    if (smtp.sendCustomCommand(F("EHLO 127.0.0.1"), customCommandCallback) != 250)
    {
        smtp.closeSession();
        return;
    }

    // Only for SMTP port 587 in supported server that accepts STARTTLS

    if (!tls)
    {
        if (smtp.sendCustomCommand(F("STARTTLS"), customCommandCallback) != 220)
        {
            smtp.closeSession();
            return;
        }

        tls = true;

        // Send greeting again
        goto init;
    }

    if (smtp.sendCustomCommand(F("AUTH LOGIN"), customCommandCallback) != 334)
    {
        smtp.closeSession();
        return;
    }

    if (smtp.sendCustomCommand(MailClient.toBase64(config.login.email), customCommandCallback) != 334)
    {
        smtp.closeSession();
        return;
    }

    if (smtp.sendCustomCommand(MailClient.toBase64(config.login.password), customCommandCallback) != 235)
    {
        smtp.closeSession();
        return;
    }

    if (smtp.isAuthenticated())
        Serial.println("Successfully logged in.\n");
    else
        Serial.println("Connected with no Auth.\n");

    // Please don't forget to change sender@xxxxxx.com to your email
    if (smtp.sendCustomCommand(F("MAIL FROM:<sender@xxxxxx.com>"), customCommandCallback) != 250)
    {
        smtp.closeSession();
        return;
    }

    // Please don't forget to change recipient@xxxxx.com with your recipient email
    if (smtp.sendCustomCommand(F("RCPT TO:<recipient@xxxxx.com>"), customCommandCallback) != 250)
    {
        smtp.closeSession();
        return;
    }

    if (smtp.sendCustomCommand(F("DATA"), customCommandCallback) != 354)
    {
        smtp.closeSession();
        return;
    }

    if (!smtp.sendCustomData(F("Subject: Test sending Email\r\n")))
    {
        smtp.closeSession();
        return;
    }

    if (!smtp.sendCustomData(F("Hello World!\r\n")))
    {
        smtp.closeSession();
        return;
    }

    if (smtp.sendCustomCommand(F("."), customCommandCallback) != 250)
    {
        smtp.closeSession();
        return;
    }

    // Do not use this command in ESP8266 due to memory leaks in ESP8266 core BearSSL.
    // smtp.sendCustomCommand(F("QUIT"), customCommandCallback);

    smtp.closeSession();

    // to clear sending result log
    // smtp.sendingResult.clear();

    MailClient.printf("Free Heap: %d\n", MailClient.getFreeHeap());
}

void loop()
{
}
