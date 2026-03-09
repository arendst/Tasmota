
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
 */

// This example showes how to set the library and/or device time manually.

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
#include "RTC.h"
#endif

#include <ESP_Mail_Client.h>

#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<password>"

#define SMTP_HOST "<host>"
#define SMTP_PORT esp_mail_smtp_port_587

#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"
#define RECIPIENT_EMAIL "<recipient email here>"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void setup()
{

    Serial.begin(115200);

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

    Serial.print("Waiting for NTP server time reading");

#if defined(ESP8266) || defined(ESP32) && !defined(ARDUINO_NANO_RP2040_CONNECT)

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    while (time(nullptr) < ESP_MAIL_CLIENT_VALID_TS)
    {
        delay(100);
    }

#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)

    configTime(10000, 0, "pool.ntp.org", "time.nist.gov");
    while (time(nullptr) < ESP_MAIL_CLIENT_VALID_TS)
    {
        delay(100);
    }

#elif __has_include(<WiFiNINA.h>) || __has_include(<WiFi101.h>)
    time_t ts = 0;
    do
    {
        ts = WiFi.getTime();
        delay(100);
    } while (ts < ESP_MAIL_CLIENT_VALID_TS);

    float gmtOffset = 3.0; // GMT offset in hour

    smtp.setSystemTime(ts, gmtOffset);

#elif __has_include(<WiFiS3.h>)

    // see https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc

    RTC.begin();

    // RTCTime startTime(30, Month::JUNE, 2023, 13, 37, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
    // RTC.setTime(startTime);

    RTCTime currentTime;

    // Get current time from RTC
    RTC.getTime(currentTime);

    float gmtOffset = 3.0; // GMT offset in hour
    smtp.setSystemTime(currentTime.getUnixTime(), gmtOffset);

#endif

    // To disable library internal NTP time reading, please comment or remove the following macro defined in src/ESP_Mail_FS.h
    // #define ENABLE_NTP_TIME

    MailClient.networkReconnect(true);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    MailClient.clearAP();
    MailClient.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    smtp.debug(1);

    smtp.callback(smtpCallback);

    Session_Config config;

    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;

    config.login.user_domain = F("127.0.0.1");

    /**
     * Once the system time (using smtp.setSystemTime) or device time was set before calling smtp.connect, the following config will
     * not take effect when NTP time is enabled.
     *
     * config.time.ntp_server
     * config.time.gmt_offset
     * config.time.day_light_offset
     *
     * To reset the reference time and use config.time instead, call smtp.setSystemTime(0) whenever you want.
     */

    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = F("ESP Mail");
    message.sender.email = AUTHOR_EMAIL;
    message.addRecipient(F("Someone"), RECIPIENT_EMAIL);

    /* The time format of timestamp to inject into subject or content as using in strftime C++ function */
    message.timestamp.tag = "#esp_mail_current_time";

    /* The tag that will be replaced with current timestamp */
    message.timestamp.format = "%B %d, %Y %H:%M:%S";

    message.subject = F("Test sending plain text Email (#esp_mail_current_time)");

    message.text.content = "This is simple plain text message\n\nSent #esp_mail_current_time";

    if (!smtp.connect(&config))
    {
        MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
        return;
    }

    if (!smtp.isLoggedIn())
    {
        Serial.println("\nNot yet logged in.");
    }
    else
    {
        if (smtp.isAuthenticated())
            Serial.println("\nSuccessfully logged in.");
        else
            Serial.println("\nConnected with no Auth.");
    }

    if (!MailClient.sendMail(&smtp, &message))
        MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}

void loop()
{
}

void smtpCallback(SMTP_Status status)
{
    Serial.println(status.info());

    if (status.success())
    {

        Serial.println("----------------");
        MailClient.printf("Message sent success: %d\n", status.completedCount());
        MailClient.printf("Message sent failed: %d\n", status.failedCount());
        Serial.println("----------------\n");

        for (size_t i = 0; i < smtp.sendingResult.size(); i++)
        {

            SMTP_Result result = smtp.sendingResult.getItem(i);

            MailClient.printf("Message No: %d\n", i + 1);
            MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
            MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
            MailClient.printf("Recipient: %s\n", result.recipients.c_str());
            MailClient.printf("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");

        smtp.sendingResult.clear();
    }
}
