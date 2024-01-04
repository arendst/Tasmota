/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "WebAuthentication.h"
#include <libb64/cencode.h>
#ifdef ESP32
#include "mbedtls/md5.h"
#else
#include "md5.h"
#endif


// Basic Auth hash = base64("username:password")

bool checkBasicAuthentication(const char * hash, const char * username, const char * password){
  if(username == NULL || password == NULL || hash == NULL)
    return false;

  size_t toencodeLen = strlen(username)+strlen(password)+1;
  size_t encodedLen = base64_encode_expected_len(toencodeLen);
  if(strlen(hash) != encodedLen)
    return false;

  char *toencode = new char[toencodeLen+1];
  if(toencode == NULL){
    return false;
  }
  char *encoded = new char[base64_encode_expected_len(toencodeLen)+1];
  if(encoded == NULL){
    delete[] toencode;
    return false;
  }
  sprintf(toencode, "%s:%s", username, password);
  if(base64_encode_chars(toencode, toencodeLen, encoded) > 0 && memcmp(hash, encoded, encodedLen) == 0){
    delete[] toencode;
    delete[] encoded;
    return true;
  }
  delete[] toencode;
  delete[] encoded;
  return false;
}

static bool getMD5(uint8_t * data, uint16_t len, char * output){//33 bytes or more
#ifdef ESP32
    mbedtls_md5_context _ctx;
#else
    md5_context_t _ctx;
#endif
  uint8_t i;
  uint8_t * _buf = (uint8_t*)malloc(16);
  if(_buf == NULL)
    return false;
  memset(_buf, 0x00, 16);
#ifdef ESP32
  mbedtls_md5_init(&_ctx);
  mbedtls_md5_starts_ret(&_ctx);
  mbedtls_md5_update_ret(&_ctx, data, len);
  mbedtls_md5_finish_ret(&_ctx, _buf);
#else
  MD5Init(&_ctx);
  MD5Update(&_ctx, data, len);
  MD5Final(_buf, &_ctx);
#endif
  for(i = 0; i < 16; i++) {
    sprintf(output + (i * 2), "%02x", _buf[i]);
  }
  free(_buf);
  return true;
}

static String genRandomMD5(){
#ifdef ESP8266
  uint32_t r = RANDOM_REG32;
#else
  uint32_t r = rand();
#endif
  char * out = (char*)malloc(33);
  if(out == NULL || !getMD5((uint8_t*)(&r), 4, out))
    return "";
  String res = String(out);
  free(out);
  return res;
}

static String stringMD5(const String& in){
  char * out = (char*)malloc(33);
  if(out == NULL || !getMD5((uint8_t*)(in.c_str()), in.length(), out))
    return "";
  String res = String(out);
  free(out);
  return res;
}

String generateDigestHash(const char * username, const char * password, const char * realm){
  if(username == NULL || password == NULL || realm == NULL){
    return "";
  }
  char * out = (char*)malloc(33);
  String res = String(username);
  res.concat(":");
  res.concat(realm);
  res.concat(":");
  String in = res;
  in.concat(password);
  if(out == NULL || !getMD5((uint8_t*)(in.c_str()), in.length(), out))
    return "";
  res.concat(out);
  free(out);
  return res;
}

String requestDigestAuthentication(const char * realm){
  String header = "realm=\"";
  if(realm == NULL)
    header.concat("asyncesp");
  else
    header.concat(realm);
  header.concat( "\", qop=\"auth\", nonce=\"");
  header.concat(genRandomMD5());
  header.concat("\", opaque=\"");
  header.concat(genRandomMD5());
  header.concat("\"");
  return header;
}

bool checkDigestAuthentication(const char * header, const char * method, const char * username, const char * password, const char * realm, bool passwordIsHash, const char * nonce, const char * opaque, const char * uri){
  if(username == NULL || password == NULL || header == NULL || method == NULL){
    //os_printf("AUTH FAIL: missing requred fields\n");
    return false;
  }

  String myHeader = String(header);
  int nextBreak = myHeader.indexOf(",");
  if(nextBreak < 0){
    //os_printf("AUTH FAIL: no variables\n");
    return false;
  }

  String myUsername = String();
  String myRealm = String();
  String myNonce = String();
  String myUri = String();
  String myResponse = String();
  String myQop = String();
  String myNc = String();
  String myCnonce = String();

  myHeader += ", ";
  do {
    String avLine = myHeader.substring(0, nextBreak);
    avLine.trim();
    myHeader = myHeader.substring(nextBreak+1);
    nextBreak = myHeader.indexOf(",");

    int eqSign = avLine.indexOf("=");
    if(eqSign < 0){
      //os_printf("AUTH FAIL: no = sign\n");
      return false;
    }
    String varName = avLine.substring(0, eqSign);
    avLine = avLine.substring(eqSign + 1);
    if(avLine.startsWith("\"")){
      avLine = avLine.substring(1, avLine.length() - 1);
    }

    if(varName.equals("username")){
      if(!avLine.equals(username)){
        //os_printf("AUTH FAIL: username\n");
        return false;
      }
      myUsername = avLine;
    } else if(varName.equals("realm")){
      if(realm != NULL && !avLine.equals(realm)){
        //os_printf("AUTH FAIL: realm\n");
        return false;
      }
      myRealm = avLine;
    } else if(varName.equals("nonce")){
      if(nonce != NULL && !avLine.equals(nonce)){
        //os_printf("AUTH FAIL: nonce\n");
        return false;
      }
      myNonce = avLine;
    } else if(varName.equals("opaque")){
      if(opaque != NULL && !avLine.equals(opaque)){
        //os_printf("AUTH FAIL: opaque\n");
        return false;
      }
    } else if(varName.equals("uri")){
      if(uri != NULL && !avLine.equals(uri)){
        //os_printf("AUTH FAIL: uri\n");
        return false;
      }
      myUri = avLine;
    } else if(varName.equals("response")){
      myResponse = avLine;
    } else if(varName.equals("qop")){
      myQop = avLine;
    } else if(varName.equals("nc")){
      myNc = avLine;
    } else if(varName.equals("cnonce")){
      myCnonce = avLine;
    }
  } while(nextBreak > 0);

  String ha1 = (passwordIsHash) ? String(password) : stringMD5(myUsername + ":" + myRealm + ":" + String(password));
  String ha2 = String(method) + ":" + myUri;
  String response = ha1 + ":" + myNonce + ":" + myNc + ":" + myCnonce + ":" + myQop + ":" + stringMD5(ha2);

  if(myResponse.equals(stringMD5(response))){
    //os_printf("AUTH SUCCESS\n");
    return true;
  }

  //os_printf("AUTH FAIL: password\n");
  return false;
}
