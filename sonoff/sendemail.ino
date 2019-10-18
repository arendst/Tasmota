#ifdef USE_SENDMAIL

#include "sendemail.h"

// enable serial debugging
//#define DEBUG_EMAIL_PORT Serial

//SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const bool ssl);
//SendEmail::send(const String& from, const String& to, const String& subject, const String& msg)
// sendmail [server:port:user:passwd:from:to:subject] data
// sendmail [*:*:*:*:*:to:subject] data uses defines from user_config
// sendmail currently only works with core 2.4.2
//HW Watchdog 8.44 sec.
//SW Watchdog 3.2 sec.

#define SEND_MAIL_MINRAM 12*1024

uint16_t SendMail(char *buffer) {
  uint16_t count;
  char *params,*oparams;
  char *mserv;
  uint16_t port;
  char *user;
  char *pstr;
  char *passwd;
  char *from;
  char *to;
  char *subject;
  char *cmd;
  char secure=0,auth=0;
  uint16_t status=1;
  SendEmail *mail=0;
  uint16_t blen;
  char *endcmd;

  //DebugFreeMem();

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
    SetSerialBaudrate(115200);
    DEBUG_EMAIL_PORT.print("mailsize: ");
    DEBUG_EMAIL_PORT.println(blen);
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
    user=(char*)EMAIL_USER;
  }
#endif
#ifdef EMAIL_PASSWORD
  if (*passwd=='*') {
    passwd=(char*)EMAIL_PASSWORD;
  }
#endif
#ifdef EMAIL_SERVER
  if (*mserv=='*') {
    mserv=(char*)EMAIL_SERVER;
  }
#endif //USE_SENDMAIL


#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(mserv);
  DEBUG_EMAIL_PORT.println(port);
  DEBUG_EMAIL_PORT.println(user);
  DEBUG_EMAIL_PORT.println(passwd);
#endif

  // 2 seconds timeout
  #define MAIL_TIMEOUT 500
  mail = new SendEmail(mserv,port,user,passwd, MAIL_TIMEOUT, auth);

#ifdef EMAIL_FROM
  if (*from=='*') {
    from=(char*)EMAIL_FROM;
  }
#endif

#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(from);
  DEBUG_EMAIL_PORT.println(to);
  DEBUG_EMAIL_PORT.println(subject);
  DEBUG_EMAIL_PORT.println(cmd);
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




SendEmail::SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used) :
    host(host), port(port), user(user), passwd(passwd), timeout(timeout), ssl(ssl), auth_used(auth_used), client(new BearSSL::WiFiClientSecure_light(1024,1024))
{

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

//void SetSerialBaudrate(int baudrate);

bool SendEmail::send(const String& from, const String& to, const String& subject, const char *msg)
{
bool status=false;
String buffer;

  if (!host.length()) {
    return status;
  }

  client->setTimeout(timeout);
  // smtp connect
#ifdef DEBUG_EMAIL_PORT
  SetSerialBaudrate(115200);
  DEBUG_EMAIL_PORT.print("Connecting: ");
  DEBUG_EMAIL_PORT.print(host);
  DEBUG_EMAIL_PORT.print(":");
  DEBUG_EMAIL_PORT.println(port);
#endif

  if (!client->connect(host.c_str(), port)) {
#ifdef DEBUG_EMAIL_PORT
      DEBUG_EMAIL_PORT.println("Connection failed: ");
      //DEBUG_EMAIL_PORT.println (client->getLastSSLError());
#endif
    goto exit;
  }

  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("220"))) {
    goto exit;
  }

  buffer = F("EHLO ");
  buffer += client->localIP().toString();

  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }
  if (user.length()>0  && passwd.length()>0 ) {

    //buffer = F("STARTTLS");
    //client->println(buffer);

    buffer = F("AUTH LOGIN");
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
    DEBUG_EMAIL_PORT.println(buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    DEBUG_EMAIL_PORT.println(buffer);
#endif
    if (!buffer.startsWith(F("334")))
    {
      goto exit;
    }
    base64 b;
    //buffer = user;
    //buffer = b.encode(buffer);
    buffer = b.encode(user);

    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  //DEBUG_EMAIL_PORT.println(user);
  DEBUG_EMAIL_PORT.println(buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
    if (!buffer.startsWith(F("334"))) {
      goto exit;
    }
    //buffer = this->passwd;
    //buffer = b.encode(buffer);
    buffer = b.encode(passwd);
    client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  //DEBUG_EMAIL_PORT.println(passwd);
  DEBUG_EMAIL_PORT.println(buffer);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
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
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }
  buffer = F("RCPT TO:");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("250"))) {
    goto exit;
  }

  buffer = F("DATA");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("354"))) {
    goto exit;
  }
  buffer = F("From: ");
  buffer += from;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = F("To: ");
  buffer += to;
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = F("Subject: ");
  buffer += subject;
  buffer += F("\r\n");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif

  client->println(msg);
  client->println('.');
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(msg);
#endif

  buffer = F("QUIT");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif

  status=true;
exit:

  return status;
}


#endif // USE_SENDMAIL
