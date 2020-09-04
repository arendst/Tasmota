/*
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2019 mobizt
 *
*/


#include <Arduino.h>
#include "ESP32_MailClient.h"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

//The Email Reading data object contains config and data that received
IMAPData imapData;

void readEmail();

unsigned long lastTime = 0;

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

    Serial.println();

    imapData.setLogin("imap.gmail.com", 993, "YOUR_EMAIL_ACCOUNT@gmail.com", "YOUR_EMAIL_PASSWORD");
    imapData.setFolder("INBOX");
    imapData.setDebug(true);

    //Set \Seen and \Answered to flags for message with UID 100
    MailClient.setFlag(imapData, 100, "\\Seen \\Answered");

    //Add \Seen and \Answered to flags for message with UID 100
    //MailClient.addFlag(imapData, 100, "\\Seen \\Answered");

    //Remove \Seen and \Answered from flags for message with UID 100
    //MailClient.removeFlag(imapData, 100, "\\Seen \\Answered");
}



void loop()
{

}

