#include "sendemail.h"

// enable serial debugging
//#define DEBUG_EMAIL_PORT Serial

SendEmail::SendEmail(const String& host, const int port, const String& user, const String& passwd, const int timeout, const int auth_used) :
    host(host), port(port), user(user), passwd(passwd), timeout(timeout), ssl(ssl), auth_used(auth_used), client(new WiFiClientSecure())
{

}



String SendEmail::readClient()
{
  String r = client->readStringUntil('\n');
  r.trim();
  while (client->available()) {
    delay(0);
    r += client->readString();
  }
  return r;
}

void SetSerialBaudrate(int baudrate);

bool SendEmail::send(const String& from, const String& to, const String& subject, const String& msg)
{
  if (!host.length())
  {
    return false;
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

#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#else
  client->setInsecure();
  bool mfln = client->probeMaxFragmentLength(host.c_str(), port, 512);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.printf("MFLN supported: %s\n", mfln ? "yes" : "no");
#endif
  if (mfln) {
      client->setBufferSizes(512, 512);
  }
#endif


  if (!client->connect(host.c_str(), port))
  {
#ifdef DEBUG_EMAIL_PORT
      DEBUG_EMAIL_PORT.println("Connection failed");
#endif
    return false;
  }

  String buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  if (!buffer.startsWith(F("220")))
  {
    return false;
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
  if (!buffer.startsWith(F("250")))
  {
    return false;
  }
  if (user.length()>0  && passwd.length()>0 )
  {

    //buffer = F("STARTTLS");
    //client->println(buffer);

if (auth_used==1) {
    // plain
#ifdef USE_PLAIN
    buffer = F("AUTH PLAIN");
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
      return false;
    }

    char plainAuth[100];
    memset(plainAuth,sizeof(plainAuth),0);
		plainAuth[0] = '\0';
		strcpy(&plainAuth[1], user.c_str());
		strcpy(&plainAuth[2+user.length()],passwd.c_str());
		const char* pA = (const char*)&plainAuth;
    char buf[100];
		base64_encode(buf, pA, user.length()+passwd.length()+2);
    client->println(buf);

#ifdef DEBUG_EMAIL_PORT
    DEBUG_EMAIL_PORT.println(buf);
#endif
    buffer = readClient();
#ifdef DEBUG_EMAIL_PORT
    DEBUG_EMAIL_PORT.println(buffer);
#endif
    if (!buffer.startsWith(F("235")))
    {
      return false;
    }
#endif

} else {

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
      return false;
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
    if (!buffer.startsWith(F("334")))
    {
      return false;
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
    if (!buffer.startsWith(F("235")))
    {
      return false;
    }
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
  if (!buffer.startsWith(F("250")))
  {
    return false;
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
  if (!buffer.startsWith(F("250")))
  {
    return false;
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
  if (!buffer.startsWith(F("354")))
  {
    return false;
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
  buffer = msg;
  client->println(buffer);
  client->println('.');
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  buffer = F("QUIT");
  client->println(buffer);
#ifdef DEBUG_EMAIL_PORT
  DEBUG_EMAIL_PORT.println(buffer);
#endif
  return true;
}

#ifdef USE_PLAIN
void SendEmail::a3_to_a4(unsigned char * a4, unsigned char * a3) {
	a4[0] = (a3[0] & 0xfc) >> 2;
	a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
	a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
	a4[3] = (a3[2] & 0x3f);
}

int SendEmail::base64_encode(char *output, const char *input, int inputLen) {
  const char* _b64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int i = 0, j = 0;
	int encLen = 0;
	unsigned char a3[3];
	unsigned char a4[4];
	while(inputLen--) {
		a3[i++] = *(input++);
		if(i == 3) {
			a3_to_a4(a4, a3);
			for(i = 0; i < 4; i++) {
				output[encLen++] = _b64_alphabet[a4[i]];
			}
			i = 0;
		}
	}
	if(i) {
		for(j = i; j < 3; j++) {
			a3[j] = '\0';
		}
		a3_to_a4(a4, a3);
		for(j = 0; j < i + 1; j++) {
			output[encLen++] = _b64_alphabet[a4[j]];
		}
		while((i++ < 3)) {
			output[encLen++] = '=';
		}
	}
	output[encLen] = '\0';
	return encLen;
}
#endif
