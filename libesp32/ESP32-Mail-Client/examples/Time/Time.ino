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
    
    //Set Clock
    //GMT offset (3 Hrs), Daylight offset (0 Hrs)
    MailClient.Time.setClock(3, 0);

    Serial.println("Number of Days This Year (since January): " + String(MailClient.Time.getNumberOfDayThisYear()));
    Serial.println("Day of Week Number: " + String(MailClient.Time.getDayOfWeek()));
    Serial.println("Day of Week String: : " + String(MailClient.Time.getDayOfWeekString()));
    Serial.println("Total seconds today: : " + String(MailClient.Time.getCurrentSecond()));
    Serial.println();
}

void loop()
{

    if (!MailClient.Time.clockReady)
        return;

    //Print out current date and time
    int d = MailClient.Time.getDay();
    int m = MailClient.Time.getMonth();
    int y = MailClient.Time.getYear();
    int hr = MailClient.Time.getHour();
    int min = MailClient.Time.getMin();
    int sec = MailClient.Time.getSec();
    Serial.print("Current Time (GMT+3): ");
    Serial.print(d);
    Serial.print("/");
    Serial.print(m);
    Serial.print("/");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(hr);
    Serial.print(":");
    Serial.print(min);
    Serial.print(":");
    Serial.println(sec);

    uint32_t todayFromMidnightTimestamp = MailClient.Time.getTimestamp(y, m, d, 0, 0, 0);
    uint32_t currentTimestamp = MailClient.Time.getUnixTime();
    uint32_t totalSecondsFromMidnight = currentTimestamp - todayFromMidnightTimestamp;

    //Assumed we countdown until 15:00:00 everyday
    uint8_t targetSec = 0;
    uint8_t targetMin = 0;
    uint8_t targetHr = 15;
    uint32_t targetSecondsFromMidnight = targetHr * 60 * 60 + targetMin * 60 + targetSec;

    if (targetSecondsFromMidnight >= totalSecondsFromMidnight)
    {
        uint32_t diffSeconds = targetSecondsFromMidnight - totalSecondsFromMidnight;
        int remainYrs, remainMonths, remainDays, remainHr, remainMin, remainSec;
        MailClient.Time.getTimeFromSec(diffSeconds, remainYrs, remainMonths, remainDays, remainHr, remainMin, remainSec);
        Serial.print("Everyday countdown until 15:00:00 is ");
        Serial.print(remainHr);
        Serial.print(" Hr, ");
        Serial.print(remainMin);
        Serial.print(" Min and ");
        Serial.print(remainSec);
        Serial.println(" Sec to go.");
    }
    else
    {
        Serial.println("Everyday countdown until 15:00:00 was passed.");
    }

    //Assumed we countdown until 18/12/2019 8:30:45
    uint32_t targetTimestamp = MailClient.Time.getTimestamp(2019, 12, 18, 8, 30, 45);
    if (targetTimestamp >= currentTimestamp)
    {
        uint32_t diffSeconds = targetTimestamp - currentTimestamp;
        int remainYrs, remainMonths, remainDays, remainHr, remainMin, remainSec;
        MailClient.Time.getTimeFromSec(diffSeconds, remainYrs, remainMonths, remainDays, remainHr, remainMin, remainSec);
        Serial.print("One time countdown until 18/12/2019 8:30:45 is ");
        Serial.print(remainYrs);
        Serial.print(" Years, ");
        Serial.print(remainMonths);
        Serial.print(" Months, ");
        Serial.print(remainDays);
        Serial.print(" Days, ");
        Serial.print(remainHr);
        Serial.print(" Hr, ");
        Serial.print(remainMin);
        Serial.print(" Min and ");
        Serial.print(remainSec);
        Serial.println(" Sec to go.");
    }
    else
    {
        Serial.println("One time countdown until 18/12/2019 8:30:45 was finished.");
    }
    Serial.println();

    delay(1000);
}

