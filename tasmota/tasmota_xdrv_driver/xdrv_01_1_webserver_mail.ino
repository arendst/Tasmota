/*
  xdrv_01_1_webserver_mail.ino - Mail client

  SPDX-FileCopyrightText: 2019 Gerhard Mutz and Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP8266
#ifdef USE_WEBSERVER
#ifdef USE_SENDMAIL
/*********************************************************************************************\
 * ESP8266 SendMail works only with server port 465 SSL and doesnt support STARTTLS (not supported in Arduino)
 * only a couple of mailservers support this (e.g. gmail,gmx,yahoo,freenetmail)
 *
 * sendmail [server:port:user:passwd:<from>:<to>:subject] body
 *
 * sendmail [*:*:*:*:*:to:subject] data uses defines from user_config_overwrite
 * #define EMAIL_USER "user"
 * #define EMAIL_PASSWORD "passwd"
 * #define EMAIL_FROM "<mr.x@gmail.com>"
 * #define EMAIL_SERVER "smtp.gmail.com"
 * #define EMAIL_PORT 465
 *
 * Note : starting with this update, it is not required anymore to include emails in < > as they will
 * be automatically added if needed
 * if email body consist of a single * and scripter is present
 * and a section >m is found, the lines in this section (until #) are sent as email body
 *
 * Some mail servers do not accept the IP address in the HELO (or EHLO) message but only a fully qualified
 * domain name (FQDN). To overcome this, use the following define to override this behavior and enter the desired FQDN
 * #define EMAIL_USER_DOMAIN "googlemail.com"
 *
 * sendmail works with pre2.6 using Light BearSSL
 * HW Watchdog 8.44 sec.
 * SW Watchdog 3.2 sec.
\*********************************************************************************************/

//#define DEBUG_EMAIL_PORT    // Enable debugging

#ifndef MAIL_TIMEOUT
#define MAIL_TIMEOUT 2000
#endif

#ifndef SEND_MAIL_MINRAM
#define SEND_MAIL_MINRAM 12*1024
#endif

#define xPSTR(a) a

#include <base64.h>
#include "WiFiClientSecureLightBearSSL.h"

void script_send_email_body(void(*func)(char *));

/*********************************************************************************************/

#ifdef DEBUG_EMAIL_PORT
void MailWriteAddLogBuffer(String *buffer) {
  AddLog(LOG_LEVEL_INFO, PSTR("MAI: > C: %s"), buffer->c_str());
}

void MailReadAddLogBuffer(String *buffer) {
  AddLog(LOG_LEVEL_INFO, PSTR("MAI: < S: %s"), buffer->c_str());
}
#endif

/*********************************************************************************************/

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
    // use bear ssl
    BearSSL::WiFiClientSecure_light *client;

    String readClient();
  public:
   SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used);
   bool send(const String& from, const String& to, const String& subject, const char *msg);
//   void send_message_txt(char *msg);
   ~SendEmail() {client->stop(); delete client;}
};

WiFiClient *g_client;

SendEmail::SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used) :
  host(host),
  port(port),
  user(user),
  passwd(passwd),
  timeout(timeout),
  ssl(ssl),
  auth_used(auth_used),
  client(new BearSSL::WiFiClientSecure_light(1024,1024)) {
}

String SendEmail::readClient() {
  delay(0);
  String r = client->readStringUntil('\n');

  r.trim();
  while (client->available()) {
    delay(0);
    r += client->readString();
  }
  return r;
}

bool SendEmail::send(const String& _from, const String& _to, const String& subject, const char *msg) {
  if (!host.length()) { return false; }

  client->setTimeout(timeout);
  client->setInsecure();

  // smtp connect
#ifdef DEBUG_EMAIL_PORT
  AddLog(LOG_LEVEL_INFO, PSTR("MAI: > C: connecting host %s on port %d"), host.c_str(), port);
#endif

  if (!client->connect(host.c_str(), port)) {
#ifdef DEBUG_EMAIL_PORT
    AddLog(LOG_LEVEL_INFO, PSTR("MAI: ! E: connection failed"));
#endif
    return false;
  }

  String from, to;
  from = ('<' == *_from.c_str()) ? _from : ("<" + _from + ">");
  to = ('<' == *_to.c_str()) ? _to : ("<" + _to +">");

  String buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  MailReadAddLogBuffer(&buffer);
#endif
  if (!buffer.startsWith(F("220"))) { return false; }

  buffer = F("EHLO ");
#ifdef EMAIL_USER_DOMAIN
  buffer += EMAIL_USER_DOMAIN;
#else
  buffer += client->localIP().toString();
#endif
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  MailReadAddLogBuffer(&buffer);
#endif
  if (!buffer.startsWith(F("250"))) { return false; }

  if ((user.length() > 0) && (passwd.length() > 0)) {
    buffer = F("AUTH LOGIN");
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
    MailWriteAddLogBuffer(&buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    MailReadAddLogBuffer(&buffer);
#endif
    if (!buffer.startsWith(F("334"))) { return false; }

    base64 b;
    buffer = b.encode(user);
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
    MailWriteAddLogBuffer(&buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    MailReadAddLogBuffer(&buffer);
#endif
    if (!buffer.startsWith(F("334"))) { return false; }

    buffer = b.encode(passwd);
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
    MailWriteAddLogBuffer(&buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    MailReadAddLogBuffer(&buffer);
#endif
    if (!buffer.startsWith(F("235"))) { return false; }
  }

  // smtp send mail
  buffer = F("MAIL FROM:");
  buffer += from;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  MailReadAddLogBuffer(&buffer);
#endif
  if (!buffer.startsWith(F("250"))) { return false; }

  buffer = F("RCPT TO:");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  MailReadAddLogBuffer(&buffer);
#endif
  if (!buffer.startsWith(F("250"))) { return false; }

  buffer = F("DATA");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  MailReadAddLogBuffer(&buffer);
#endif
  if (!buffer.startsWith(F("354"))) { return false; }

  buffer = F("From: ");
  buffer += from;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = F("To: ");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif
  buffer = F("Subject: ");
  buffer += subject;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif

/*-------------------------------------------------------------------------------------------*/

#ifdef USE_SCRIPT
  if (*msg == '*' && *(msg +1) == 0) {
    buffer = F("MIME-Version: 1.0\r\n");
    client->print(buffer);
    buffer = F("Content-Type: Multipart/mixed; boundary=frontier\r\n\r\n");
    client->print(buffer);
    g_client = client;
    script_send_email_body(send_message_txt);
  } else {
#endif  // USE_SCRIPT
    buffer = F("\r\n");
    client->print(buffer);
    client->println(msg);
#ifdef USE_SCRIPT
  }
#endif  // USE_SCRIPT

/*-------------------------------------------------------------------------------------------*/

  client->println('.');
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif

  buffer = F("QUIT");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  MailWriteAddLogBuffer(&buffer);
#endif

  return true;
}

/*********************************************************************************************/

uint16_t SendMail(char *buffer) {
  // return if not enough memory
  uint16_t mem = ESP_getFreeHeap();
  if (mem < SEND_MAIL_MINRAM) { return 4; }

  while (*buffer == ' ') { buffer++; }
  if (*buffer != '[') { return 1; }

  buffer++;
  char *endcmd = strchr(buffer, ']');
  if (!endcmd) { return 1; }

  // copy params
  uint16_t blen = (uint32_t)endcmd - (uint32_t)buffer;
  char *oparams = (char*)calloc(blen +2, 1);
  if (!oparams) { return 4; }

  uint16_t status = 1;
  char *params = oparams;
  strncpy(oparams, buffer, blen +2);
  oparams[blen] = 0;
  const char *cmd = endcmd +1;

#ifdef DEBUG_EMAIL_PORT
  AddLog(LOG_LEVEL_INFO, PSTR("MAI: Size: %d"), blen);
#endif

  const char *mserv = strtok(params, ":");
  if (mserv) {
    // port
    const char *pstr = strtok(NULL, ":");
    if (pstr) {
      uint16_t port;
#ifdef EMAIL_PORT
      port = (*pstr == '*') ? EMAIL_PORT : atoi(pstr);
#else
      port = atoi(pstr);
#endif
      const char *user = strtok(NULL, ":");
      if (user) {
        const char *passwd = strtok(NULL, ":");
        if (passwd) {
          const char *from = strtok(NULL, ":");
          if (from) {
            const char *to = strtok(NULL,":");
            if (to) {
              const char *subject = strtok(NULL, "]");
              if (subject) {
#ifdef EMAIL_USER
                if (*user == '*') { user = xPSTR(EMAIL_USER); }
#endif
#ifdef EMAIL_PASSWORD
                if (*passwd == '*') { passwd = xPSTR(EMAIL_PASSWORD); }
#endif
#ifdef EMAIL_SERVER
                if (*mserv == '*') { mserv = xPSTR(EMAIL_SERVER); }
#endif

#ifdef DEBUG_EMAIL_PORT
                AddLog(LOG_LEVEL_INFO, PSTR("MAI: %s, %d, %s, %s"), mserv, port, user, passwd);
#endif

#ifdef EMAIL_FROM
                if (*from == '*') { from = xPSTR(EMAIL_FROM); }
#endif

#ifdef DEBUG_EMAIL_PORT
                AddLog(LOG_LEVEL_INFO, PSTR("MAI: %s, %s, %s, %s"), from, to, subject, cmd);
#endif

                char auth = 0;
                SendEmail *mail = new SendEmail(mserv, port, user, passwd, MAIL_TIMEOUT, auth);
                if (mail) {
                  bool result = mail->send(from, to, subject, cmd);
                  delete mail;
                  if (result == true) { status = 0; }
                }
              }
            }
          }
        }
      }
    }
  }
  if (oparams) { free(oparams); }
  return status;
}

/*********************************************************************************************/

#ifdef USE_SCRIPT
#ifdef USE_UFILESYS

#include <LittleFS.h>
extern FS *ufsp;

void attach_File(char *path) {
  char *cp = path;
  while (*cp == '/') { cp++; }
  File file = ufsp->open(path, "r");
  if (file) {
    char buff[64];
    snprintf_P(buff, sizeof(buff), PSTR("Content-Disposition: attachment; filename=\"%s\"\r\n\r\n"), cp);
    g_client->write(buff);
    uint16_t flen = file.size();
    uint8_t fbuff[64];
    uint16_t blen = sizeof(fbuff);
    while (flen > 0) {
      file.read(fbuff, blen);
      flen -= blen;
      g_client->write(fbuff, blen);
      if (flen < blen) { blen = flen; }
    }
    file.close();
  } else {
    g_client->print(F("\r\n\r\nfile not found!\r\n"));
  }
}

#endif  // USE_UFILESYS

float *get_array_by_name(char *name, uint16_t *alen);
void flt2char(float num, char *nbuff);

void attach_Array(char *aname) {
  uint16_t alen;
  float *array = get_array_by_name(aname, &alen);
  if (array && alen) {
#ifdef DEBUG_EMAIL_PORT
    AddLog(LOG_LEVEL_INFO, PSTR("MAI: Array found %d"), alen);
#endif
    char buff[64];
    snprintf_P(buff, sizeof(buff), PSTR("Content-Disposition: attachment; filename=\"%s.txt\"\r\n\r\n"), aname);
    g_client->write(buff);
    // send timestamp
    strcpy(buff, GetDateAndTime(DT_LOCAL).c_str());
    strcat(buff, "\t");
    g_client->write(buff);

    float *fp=array;
    for (uint32_t cnt = 0; cnt < alen; cnt++) {
      // export array as tab gelimited text
      char nbuff[16];
      flt2char(*fp++, nbuff);
      if (cnt < (alen - 1)) {
        strcat(nbuff, "\t");
      } else {
        strcat(nbuff, "\n");
      }
      g_client->write(nbuff, strlen(nbuff));
    }
  } else {
    g_client->print(F("\r\n\r\narray not found!\r\n"));
  }
}

void send_message_txt(char *txt) {
  g_client->print(F("--frontier\r\n"));
  g_client->print(F("Content-Type: text/plain\r\n"));
  if (*txt == '&') {
    txt++;
    attach_Array(txt);
#ifdef USE_UFILESYS
  } else if (*txt == '@') {
    txt++;
    attach_File(txt);
#endif  // USE_UFILESYS
  } else {
    g_client->print(F("\r\n"));
    g_client->println(txt);
  }
  g_client->print(F("\r\n--frontier\r\n"));
}

#endif  // USE_SCRIPT

/*********************************************************************************************/

#endif  // USE_SENDMAIL
#endif  // USE_WEBSERVER
#endif  // ESP8266