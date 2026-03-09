
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example showes how to send text Email via the custom port.

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
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT 2525 // non-standard port (TLS)

/* The log in credentials */
#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"

/* Recipient email address */
#define RECIPIENT_EMAIL "<recipient email here>"

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "-----END CERTIFICATE-----\n";

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

    /** Set the ports and protocols
     *  This allows non-standard port to work with this library
     *  The port that assigned with config.server.port will map with the
     *  protocol assigned here
     */

    config.ports_functions.list = new port_function[4];
    config.ports_functions.size = 4;

    config.ports_functions.list[0].port = 25;
    config.ports_functions.list[0].protocol = esp_mail_protocol_plain_text;

    config.ports_functions.list[1].port = 465;
    config.ports_functions.list[1].protocol = esp_mail_protocol_ssl;

    config.ports_functions.list[2].port = 587;
    config.ports_functions.list[2].protocol = esp_mail_protocol_tls;

    config.ports_functions.list[3].port = 2525;
    config.ports_functions.list[3].protocol = esp_mail_protocol_tls;

    /** In ESP32, timezone environment will not keep after wake up boot from sleep.
     * The local time will equal to GMT time.
     *
     * To sync or set time with NTP server with the valid local time after wake up boot,
     * set both gmt and day light offsets to 0 and assign the timezone environment string e.g.

       config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
       config.time.gmt_offset = 0;
       config.time.day_light_offset = 0;
       config.time.timezone_env_string = "JST-9"; // for Tokyo

     * The library will get (sync) the time from NTP server without GMT time offset adjustment
     * and set the timezone environment variable later.
     *
     * This timezone environment string will be stored to flash or SD file named "/tz_env.txt"
     * which set via config.time.timezone_file.
     *
     * See the timezone environment string list from
     * https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     *
     */

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = F("ESP Mail"); // This witll be used with 'MAIL FROM' command and 'From' header field.
    message.sender.email = AUTHOR_EMAIL; // This witll be used with 'From' header field.
    message.subject = F("Test sending plain text Email");
    message.addRecipient(F("Someone"), RECIPIENT_EMAIL); // This will be used with RCPT TO command and 'To' header field.

    String textMsg = "This is simple plain text message";
    message.text.content = textMsg;

    /** If the message to send is a large string, to reduce the memory used from internal copying  while sending,
     * you can assign string to message.text.blob by cast your string to uint8_t array like this
     *
     * String myBigString = "..... ......";
     * message.text.blob.data = (uint8_t *)myBigString.c_str();
     * message.text.blob.size = myBigString.length();
     *
     * or assign string to message.text.nonCopyContent, like this
     *
     * message.text.nonCopyContent = myBigString.c_str();
     *
     * Only base64 encoding is supported for content transfer encoding in this case.
     */

    /** The Plain text message character set e.g.
     * us-ascii
     * utf-8
     * utf-7
     * The default value is utf-8
     */
    message.text.charSet = F("us-ascii");

    /** The content transfer encoding e.g.
     * enc_7bit or "7bit" (not encoded)
     * enc_qp or "quoted-printable" (encoded)
     * enc_base64 or "base64" (encoded)
     * enc_binary or "binary" (not encoded)
     * enc_8bit or "8bit" (not encoded)
     * The default value is "7bit"
     */
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    // If this is a reply message
    // message.in_reply_to = "<parent message id>";
    // message.references = "<parent references> <parent message id>";

    /** The message priority
     * esp_mail_smtp_priority_high or 1
     * esp_mail_smtp_priority_normal or 3
     * esp_mail_smtp_priority_low or 5
     * The default value is esp_mail_smtp_priority_low
     */
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

    // message.response.reply_to = "someone@somemail.com";
    // message.response.return_path = "someone@somemail.com";

    /** The Delivery Status Notifications e.g.
     * esp_mail_smtp_notify_never
     * esp_mail_smtp_notify_success
     * esp_mail_smtp_notify_failure
     * esp_mail_smtp_notify_delay
     * The default value is esp_mail_smtp_notify_never
     */
    // message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

    /* Set the custom message header */
    message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

    // For Root CA certificate verification (ESP8266 and ESP32 only)
    // config.certificate.cert_data = rootCACert;
    // or
    // config.certificate.cert_file = "/path/to/der/file";
    // config.certificate.cert_file_storage_type = esp_mail_file_storage_type_flash; // esp_mail_file_storage_type_sd
    // config.certificate.verify = true;

    // The WiFiNINA firmware the Root CA certification can be added via the option in Firmware update tool in Arduino IDE

    /* Connect to server with the session config */

    // Library will be trying to sync the time with NTP server if time is never sync or set.
    // This is 10 seconds blocking process.
    // If time reading was timed out, the error "NTP server time reading timed out" will show via debug and callback function.
    // You can manually sync time by yourself with NTP library or calling configTime in ESP32 and ESP8266.
    // Time can be set manually with provided timestamp to function smtp.setSystemTime.

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

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
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
