/**
 * This example showed how to delete messages from the opened mailbox folder.
 * 
 * Email: suwatchai@outlook.com
 * 
 * Github: https://github.com/mobizt/ESP-Mail-Client
 * 
 * Copyright (c) 2020 mobizt
 *
*/

/** To receive Email using Gmail, IMAP option should be enabled. https://support.google.com/mail/answer/7126229?hl=en
 * and also https://accounts.google.com/b/0/DisplayUnlockCaptcha
 * 
*/

/** For ESP8266, with BearSSL WiFi Client 
 * The memory reserved for completed valid SSL response from IMAP is 16 kbytes which
 * may cause your device out of memory reset in case the memory 
 * allocation error.
*/

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "################"
#define WIFI_PASSWORD "################"

/* The imap host name e.g. imap.gmail.com for GMail or outlook.office365.com for Outlook */
#define IMAP_HOST "################"

/** The imap port e.g. 
 * 143  or esp_mail_imap_port_143
 * 993 or esp_mail_imap_port_993
*/
#define IMAP_PORT 993

/* The log in credentials */
#define AUTHOR_EMAIL "################"
#define AUTHOR_PASSWORD "################"

/* Print the list of mailbox folders */
void printAllMailboxesInfo(IMAPSession &imap);

/* Print the selected folder info */
void printSelectedMailboxInfo(IMAPSession &imap);

/* The IMAP Session object used for Email reading */
IMAPSession imap;


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

    /** Enable the debug via Serial port 
     * none debug or 0
     * basic debug or 1
    */
    imap.debug(1);

    /* Declare the session config data */
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = IMAP_HOST;
    session.server.port = IMAP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;


    /* Setup the configuration for searching or fetching operation and its result */
    IMAP_Config config;

    /* Connect to server with the session and config */
    if (!imap.connect(&session, &config))
        return;

    /*  {Optional] */
    printAllMailboxesInfo(imap);

    /* Open or select the mailbox folder to read or search the message */
    if (!imap.selectFolder("INBOX"))
        return;

    /* Define the MessageList class to add the message to delete */
    MessageList toDelete;
    /* Add message uid to delete to the list */
    toDelete.add(10);
    toDelete.add(12);

    /* Delete all messages in the list (move to trash) */
    if(imap.deleteMessages(&toDelete))
        Serial.println("Messages deeted");

    /* Delete all messages permanently by assign the second param to true*/
    //imap.deleteMessages(&toDelete, true);
}

void loop()
{

}
