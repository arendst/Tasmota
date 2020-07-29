#ifndef __SENDEMAIL_H
#define __SENDEMAIL_H

//#define DEBUG_EMAIL_PORT

//#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
#include <base64.h>
//#include <core_version.h>

#ifdef ESP8266
#include "WiFiClientSecureLightBearSSL.h"
#else
#include <WiFiClientSecure.h>
#endif

class SendEmail
{
  private:
    const String host;
    const int port;
    const String user;
    const String passwd;
    const int timeout;
    const bool ssl;
    const int auth_used;
#ifdef ESP8266
    // use bear ssl
    BearSSL::WiFiClientSecure_light *client;
#else
    WiFiClient *client;
#endif

    String readClient();
    void a3_to_a4(unsigned char * a4, unsigned char * a3);
    int base64_encode(char *output, const char *input, int inputLen);
  public:
   SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used);
   bool send(const String& from, const String& to, const String& subject, const char *msg);
   void send_message_txt(char *msg);
   ~SendEmail() {client->stop(); delete client;}
};

#endif
