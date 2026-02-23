
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt/ESP-Mail-Client
 *
 * Copyright (c) 2023 mobizt
*/

// This example shows how to send Email using ESP8266 and ENC28J60 Ethernet module.

// This example requires ESP8266 Arduino Core SDK v3.x.x

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

/**
 *
 * The ENC28J60 Ethernet module and ESP8266 board, SPI port wiring connection.
 *
 * ESP8266 (Wemos D1 Mini or NodeMCU)        ENC28J60
 *
 * GPIO12 (D6) - MISO                        SO
 * GPIO13 (D7) - MOSI                        SI
 * GPIO14 (D5) - SCK                         SCK
 * GPIO16 (D0) - CS                          CS
 * GND                                       GND
 * 3V3                                       VCC
 *
 */

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <ENC28J60lwIP.h>
// #include <W5100lwIP.h>
// #include <W5500lwIP.h>

/**
 * For ENC28J60 ethernet module, uncomment this line in ESP_Mail_FS.h
  #define ENABLE_ESP8266_ENC28J60_ETH

 * For W5500 ethernet module, uncomment this line in ESP_Mail_FS.h
  #define ENABLE_ESP8266_W5500_ETH

 * For W5100 ethernet module, uncomment this line in ESP_Mail_FS.h
  #define ENABLE_ESP8266_W5100_ETH
*/

#include <ESP_Mail_Client.h>

#define SMTP_HOST "<host>"
#define SMTP_PORT 25

#define AUTHOR_EMAIL "<email>"
#define AUTHOR_PASSWORD "<password>"
#define RECIPIENT_EMAIL "<recipient email here>"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

unsigned long sendMillis = 0;

#ifdef ESP8266_CORE_SDK_V3_X_X

#define ETH_CS_PIN 16 // D0
ENC28J60lwIP eth(ETH_CS_PIN);
// Wiznet5100lwIP eth(ETH_CS_PIN);
// Wiznet5500lwIP eth(ETH_CS_PIN);

#endif

void sendMail()
{

  smtp.debug(1);

  smtp.callback(smtpCallback);

  Session_Config config;

  /* Assign the pointer to Ethernet module lwip interface */
#ifdef ESP8266_CORE_SDK_V3_X_X
#if defined(ENABLE_ESP8266_ENC28J60_ETH)
  config.spi_ethernet_module.enc28j60 = &eth;
#elif defined(ENABLE_ESP8266_W5100_ETH)
  config.spi_ethernet_module.w5100 = &eth;
#elif defined(ENABLE_ESP8266_W5500_ETH)
  config.spi_ethernet_module.w5500 = &eth;
#endif
#endif

  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

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

  SMTP_Message message;

  message.sender.name = F("ESP Mail");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Test sending plain text Email");
  message.addRecipient(F("Someone"), RECIPIENT_EMAIL);

  String textMsg = "This is simple plain text message";
  message.text.content = textMsg;

  if (!smtp.connect(&config))
  {
    MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (smtp.isAuthenticated())
    Serial.println("\nSuccessfully logged in.");
  else
    Serial.println("\nConnected with no Auth.");

  if (!MailClient.sendMail(&smtp, &message))
    MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

#ifdef ESP8266_CORE_SDK_V3_X_X

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz?
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  eth.setDefault(); // use ethernet for default route
  if (!eth.begin())
  {
    Serial.println("ethernet hardware not found ... sleeping");
    while (1)
    {
      delay(1000);
    }
  }
  else
  {
    Serial.print("connecting ethernet");
    while (!eth.connected())
    {
      Serial.print(".");
      delay(1000);
    }
  }
  Serial.println();
  Serial.print("ethernet IP address: ");
  Serial.println(eth.localIP());

#else
  Serial.println("This example requires ESP8266 Arduino Core SDK v3.x.x, please update.");
#endif
}

void loop()
{
#ifdef ESP8266_CORE_SDK_V3_X_X
  if (millis() - sendMillis > 300000 || sendMillis == 0)
  {
    sendMillis = millis();
    sendMail();
  }
#endif
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