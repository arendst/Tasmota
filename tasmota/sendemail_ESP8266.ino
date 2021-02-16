#ifdef ESP8266
#ifdef USE_SENDMAIL
#include "sendemail_ESP8266.h"

// enable serial debugging
//#define DEBUG_EMAIL_PORT

// sendmail works only with server port 465 SSL and doesnt support STARTTLS (not supported in Arduino)
// only a couple of mailservers support this (e.g. gmail,gmx,yahoo,freenetmail)
// sendmail [server:port:user:passwd:from:to:subject] body
// sendmail [*:*:*:*:*:to:subject] data uses defines from user_config_overwrite
// #define EMAIL_USER "user"
// #define EMAIL_PASSWORD "passwd"
// #define EMAIL_FROM "<mr.x@gmail.com>"
// #define EMAIL_SERVER "smtp.gmail.com"
// #define EMAIL_PORT 465
// if email body consist of a single * and scripter is present
// and a section >m is found, the lines in this section (until #) are sent
// as email body

// sendmail works with pre2.6 using Light BearSSL
//HW Watchdog 8.44 sec.
//SW Watchdog 3.2 sec.

#ifndef SEND_MAIL_MINRAM
#define SEND_MAIL_MINRAM 12*1024
#endif

void script_send_email_body(void(*func)(char *));

#define xPSTR(a) a

uint16_t SendMail(char *buffer) {
  char *params,*oparams;
  const char *mserv;
  uint16_t port;
  const char *user;
  const char *pstr;
  const char *passwd;
  const char *from;
  const char *to;
  const char *subject;
  const char *cmd;
  char auth=0;
  uint16_t status=1;
  SendEmail *mail=0;
  uint16_t blen;
  char *endcmd;


// return if not enough memory
  uint16_t mem=ESP.getFreeHeap();
  if (mem<SEND_MAIL_MINRAM) {
    return 4;
  }

  while (*buffer==' ') buffer++;

  if (*buffer!='[') {
      goto exit;
  }

  buffer++;

  endcmd=strchr(buffer,']');
  if (!endcmd) {
    goto exit;
  }

  // copy params
  blen=(uint32_t)endcmd-(uint32_t)buffer;
  oparams=(char*)calloc(blen+2,1);
  if (!oparams) return 4;
  params=oparams;
  strncpy(oparams,buffer,blen+2);
  oparams[blen]=0;

  cmd=endcmd+1;

  #ifdef DEBUG_EMAIL_PORT
    AddLog(LOG_LEVEL_INFO, PSTR("mailsize: %d"),blen);
  #endif

  mserv=strtok(params,":");
  if (!mserv) {
      goto exit;
  }

  // port
  pstr=strtok(NULL,":");
  if (!pstr) {
      goto exit;
  }

#ifdef EMAIL_PORT
  if (*pstr=='*') {
    port=EMAIL_PORT;
  } else {
    port=atoi(pstr);
  }
#else
  port=atoi(pstr);
#endif

  user=strtok(NULL,":");
  if (!user) {
      goto exit;
  }

  passwd=strtok(NULL,":");
  if (!passwd) {
      goto exit;
  }

  from=strtok(NULL,":");
  if (!from) {
      goto exit;
  }

  to=strtok(NULL,":");
  if (!to) {
      goto exit;
  }

  subject=strtok(NULL,"]");
  if (!subject) {
      goto exit;
  }


#ifdef EMAIL_USER
  if (*user=='*') {
    user=xPSTR(EMAIL_USER);
  }
#endif
#ifdef EMAIL_PASSWORD
  if (*passwd=='*') {
    passwd=xPSTR(EMAIL_PASSWORD);
  }
#endif
#ifdef EMAIL_SERVER
  if (*mserv=='*') {
    mserv=xPSTR(EMAIL_SERVER);
  }
#endif //USE_SENDMAIL


#ifdef DEBUG_EMAIL_PORT
  AddLog(LOG_LEVEL_INFO, PSTR("%s - %d - %s - %s"),mserv,port,user,passwd);
#endif

  // 2 seconds timeout
#ifndef MAIL_TIMEOUT
  #define MAIL_TIMEOUT 2000
#endif
  mail = new SendEmail(mserv,port,user,passwd, MAIL_TIMEOUT, auth);

#ifdef EMAIL_FROM
  if (*from=='*') {
    from=xPSTR(EMAIL_FROM);
  }
#endif

#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s - %s - %s - %s"),from,to,subject,cmd);
#endif

  if (mail) {
    bool result=mail->send(from,to,subject,cmd);
    delete mail;
    if (result==true) status=0;
  }

exit:
  if (oparams) free(oparams);
  return status;
}


WiFiClient *g_client;
SendEmail::SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used) :
    host(host), port(port), user(user), passwd(passwd), timeout(timeout), ssl(ssl), auth_used(auth_used), client(new BearSSL::WiFiClientSecure_light(1024,1024)) {
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

bool SendEmail::send(const String& from, const String& to, const String& subject, const char *msg) {
bool status=false;
String buffer;

  if (!host.length()) {
    return status;
  }

  client->setTimeout(timeout);
  // smtp connect
#ifdef DEBUG_EMAIL_PORT
  AddLog(LOG_LEVEL_INFO, PSTR("Connecting: %s on port %d"),host.c_str(),port);
#endif

  if (!client->connect(host.c_str(), port)) {
#ifdef DEBUG_EMAIL_PORT
    AddLog(LOG_LEVEL_INFO, PSTR("Connection failed"));
#endif
    goto exit;
  }

  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  if (!buffer.startsWith(F("220"))) {
    goto exit;
  }

  buffer = F("EHLO ");
  buffer += client->localIP().toString();

  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }
  if (user.length()>0  && passwd.length()>0 ) {

    buffer = F("AUTH LOGIN");
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
    AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    if (!buffer.startsWith(F("334")))
    {
      goto exit;
    }
    base64 b;
    buffer = b.encode(user);

    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    if (!buffer.startsWith(F("334"))) {
      goto exit;
    }
    buffer = b.encode(passwd);
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
    if (!buffer.startsWith(F("235"))) {
      goto exit;
    }
  }

  // smtp send mail
  buffer = F("MAIL FROM:");
  buffer += from;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }
  buffer = F("RCPT TO:");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }

  buffer = F("DATA");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  if (!buffer.startsWith(F("354"))) {
    goto exit;
  }

  buffer = F("From: ");
  buffer += from;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = F("To: ");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif
  buffer = F("Subject: ");
  buffer += subject;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif

#ifdef USE_SCRIPT
  if (*msg=='*' && *(msg+1)==0) {
    buffer = F("MIME-Version: 1.0\r\n");
    client->print(buffer);
    buffer = F("Content-Type: Multipart/mixed; boundary=frontier\r\n\r\n");
    client->print(buffer);

    g_client=client;
    script_send_email_body(xsend_message_txt);
  } else {
#endif
    buffer = F("\r\n");
    client->print(buffer);
    client->println(msg);
#ifdef USE_SCRIPT
  }
#endif
  client->println('.');
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif

  buffer = F("QUIT");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),buffer.c_str());
#endif

  status=true;
exit:

  return status;
}

#ifdef USE_SCRIPT
void xsend_message_txt(char *msg) {

#ifdef DEBUG_EMAIL_PORT
  AddLog_P(LOG_LEVEL_INFO, PSTR("%s"),msg);
#endif

#ifdef USE_UFILESYS
  if (*msg=='@') {
    msg++;
    attach_File(msg);
  } else if (*msg=='&') {
    msg++;
    attach_Array(msg);
  } else {
    g_client->print(F("--frontier\r\n"));
    g_client->print(F("Content-Type: text/plain\r\n\r\n"));
    g_client->println(msg);
    g_client->print(F("\r\n--frontier\r\n"));
  }
#else
  if (*msg=='&') {
    msg++;
    attach_Array(msg);
  } else {
    g_client->print(F("--frontier\r\n"));
    g_client->print(F("Content-Type: text/plain\r\n\r\n"));
    g_client->println(msg);
    g_client->print(F("\r\n--frontier\r\n"));
  }
#endif
}

float *get_array_by_name(char *name, uint16_t *alen);
void flt2char(float num, char *nbuff);

void attach_Array(char *aname) {
  float *array = 0;
  uint16_t alen;
  array = get_array_by_name(aname, &alen);
  g_client->print(F("--frontier\r\n"));
  g_client->print(F("Content-Type: text/plain\r\n"));
  if (array && alen) {
#ifdef DEBUG_EMAIL_PORT
    AddLog(LOG_LEVEL_INFO, PSTR("array found %d"),alen);
#endif
    char buff[64];
    sprintf_P(buff,PSTR("Content-Disposition: attachment; filename=\"%s.txt\"\r\n\r\n"), aname);
    g_client->write(buff);
    // send timestamp
    strcpy(buff, GetDateAndTime(DT_LOCAL).c_str());
    strcat(buff,"\t");
    g_client->write(buff);

    float *fp=array;
    for (uint32_t cnt = 0; cnt<alen; cnt++) {
      // export array as tab gelimited text
      char nbuff[16];
      flt2char(*fp++, nbuff);
      if (cnt < (alen - 1)) {
        strcat(nbuff,"\t");
      } else {
        strcat(nbuff,"\n");
      }
      g_client->write(nbuff, strlen(nbuff));
    }
  } else {
    g_client->print(F("\r\n\r\narray not found!\r\n"));
  }
  g_client->print(F("\r\n--frontier\r\n"));
}
#endif


#ifdef USE_UFILESYS

#include <LittleFS.h>
extern FS *ufsp;

void attach_File(char *path) {
  g_client->print(F("--frontier\r\n"));
  g_client->print(F("Content-Type: text/plain\r\n"));
  char buff[64];
  char *cp = path;
  while (*cp=='/') cp++;
  File file = ufsp->open(path, "r");
  if (file) {
    sprintf_P(buff,PSTR("Content-Disposition: attachment; filename=\"%s\"\r\n\r\n"), cp);
    g_client->write(buff);
    uint16_t flen = file.size();
    uint8_t fbuff[64];
    uint16_t blen = sizeof(fbuff);
    while (flen>0) {
      file.read(fbuff, blen);
      flen -= blen;
      g_client->write(fbuff, blen);
      if (flen<blen) blen = flen;
    }
    file.close();
  } else {
    g_client->print(F("\r\n\r\nfile not found!\r\n"));
  }
  g_client->print(F("\r\n--frontier\r\n"));
}

#endif // USE_UFILESYS


#endif // USE_SENDMAIL

#endif // ESP32
