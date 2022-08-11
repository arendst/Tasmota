

/**
 * This example will send the Email in plain text version using ESP32 and LAN8720 Ethernet module.
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

/**
 * There are many sources for LAN8720 and ESP32 interconnection on the internet which may
 * work for your LAN8720 board.
 * 
 * Some methods worked unless no IP is available.
 * 
 * This modification and interconnection provided in this example are mostly worked as
 * the 50 MHz clock was created internally in ESP32 which GPIO 17 is set to be output of this clock
 * and feeds to the LAN8720 chip XTAL input.
 * 
 * The on-board LAN8720 50 MHz XTAL chip will be disabled by connect its enable pin or pin 1 to GND.
 * 
 * Please see the images in the folder "modified_LAN8720_board_images" for how to modify the LAN8720 board.
 * 
 * The LAN8720 Ethernet modified board and ESP32 board wiring connection.
 * 
 * ESP32                        LAN8720                       
 * 
 * GPIO17 - EMAC_CLK_OUT_180    nINT/REFCLK - LAN8720 XTAL1/CLKIN     4k7 Pulldown
 * GPIO22 - EMAC_TXD1           TX1
 * GPIO19 - EMAC_TXD0           TX0
 * GPIO21 - EMAC_TX_EN          TX_EN
 * GPIO26 - EMAC_RXD1           RX1
 * GPIO25 - EMAC_RXD0           RX0
 * GPIO27 - EMAC_RX_DV          CRS
 * GPIO23 - MDC                 MDC
 * GPIO18 - MDIO                MDIO
 * GND                          GND
 * 3V3                          VCC
 * 
*/

//In case of Gmail, to send the Email via port 465 (SSL), less secure app option should be enabled in the account settings. https://myaccount.google.com/lesssecureapps?pli=1

#include <WiFi.h>
#include <ESP_Mail_Client.h>

#ifdef ETH_CLK_MODE
#undef ETH_CLK_MODE
#endif
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT //RMII clock output from GPIO17

// Pin# of the enable signal for the external crystal oscillator (-1 to disable)
#define ETH_POWER_PIN -1

// Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_TYPE ETH_PHY_LAN8720

// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_ADDR 1

// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDC_PIN 23

// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_MDIO_PIN 18

static bool eth_connected = false;


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
#define SMTP_PORT 25

/* The sign in credentials */
#define AUTHOR_EMAIL "################"
#define AUTHOR_PASSWORD "################"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

unsigned long sendMillis = 0;

void WiFiEvent(WiFiEvent_t event)
{
  //Do not run any function here to prevent stack overflow or nested interrupt
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    //set eth hostname here
    ETH.setHostname("esp32-ethernet");

    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("ETH Connected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.print("ETH MAC: ");
    Serial.print(ETH.macAddress());
    Serial.print(", IPv4: ");
    Serial.print(ETH.localIP());
    if (ETH.fullDuplex())
    {
      Serial.print(", FULL_DUPLEX");
    }
    Serial.print(", ");
    Serial.print(ETH.linkSpeed());
    Serial.println("Mbps");
    eth_connected = true;

    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH Disconnected");
    eth_connected = false;
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("ETH Stopped");
    eth_connected = false;
    break;
  default:
    break;
  }
}

void sendMail()
{

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /** ########################################################
   * Some properties of SMTPSession data and parameters pass to 
   * SMTP_Message class accept the pointer to constant char
   * i.e. const char*. 
   * 
   * You may assign a string literal to that properties or function 
   * like below example.
   *   
   * session.login.user_domain = "mydomain.net";
   * session.login.user_domain = String("mydomain.net").c_str();
   * 
   * or
   * 
   * String doman = "mydomain.net";
   * session.login.user_domain = domain.c_str();
   * 
   * And
   * 
   * String name = "Jack " + String("dawson");
   * String email = "jack_dawson" + String(123) + "@mail.com";
   * 
   * message.addRecipient(name.c_str(), email.c_str());
   * 
   * message.addHeader(String("Message-ID: <abcde.fghij@gmail.com>").c_str());
   * 
   * or
   * 
   * String header = "Message-ID: <abcde.fghij@gmail.com>";
   * message.addHeader(header.c_str());
   * 
   * ###########################################################
  */

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "mydomain.net";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "ESP Mail";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Test sending plain text Email";
  message.addRecipient("Someone", "####@#####_dot_com");

  String textMsg = "This is simple plain text message";
  message.text.content = textMsg.c_str();

  /** The Plain text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
  */
  message.text.charSet = "us-ascii";

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
  */
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
  */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
  */
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.onEvent(WiFiEvent);

  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
}

void loop()
{
  if (eth_connected && (millis() - sendMillis > 300000 || sendMillis == 0))
  {
    sendMillis = millis();
    sendMail();
  }
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
