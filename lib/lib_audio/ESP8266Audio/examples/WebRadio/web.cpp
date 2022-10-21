/*
  PsychoPlug
  ESP8266 based remote outlet with standalone timer and MQTT integration
  
  Copyright (C) 2017  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "web.h"

void WebPrintError(WiFiClient *client, int code)
{
  switch(code) {
    case 301: WebPrintf(client, "301 Moved Permanently"); break;
    case 400: WebPrintf(client, "400 Bad Request"); break;
    case 401: WebPrintf(client, "401 Unauthorized"); break;
    case 404: WebPrintf(client, "404 Not Found"); break;
    case 405: WebPrintf(client, "405 Method Not Allowed"); break;
    default:  WebPrintf(client, "500 Server Error"); break;
  }
}


void WebError(WiFiClient *client, int code, const char *headers, bool usePMEM)
{
  WebPrintf(client, "HTTP/1.1 %d\r\n", code);
  WebPrintf(client, "Server: PsychoPlug\r\n");
  WebPrintf(client, "Content-type: text/html\r\n");
  WebPrintf(client, "Cache-Control: no-cache, no-store, must-revalidate\r\n");
  WebPrintf(client, "Pragma: no-cache\r\n");
  WebPrintf(client, "Expires: 0\r\n");
  WebPrintf(client, "Connection: close\r\n");
  if (headers) {
    if (!usePMEM) {
      WebPrintf(client, "%s", headers);
    } else {
      WebPrintfPSTR(client, headers);
    }
  }
  WebPrintf(client, "\r\n\r\n");
  WebPrintf(client, DOCTYPE);
  WebPrintf(client, "<html><head><title>");
  WebPrintError(client, code);
  WebPrintf(client, "</title>" ENCODING "</head>\n");
  WebPrintf(client, "<body><h1>");
  WebPrintError(client, code);
  WebPrintf(client, "</h1></body></html>\r\n");
}



void WebHeaders(WiFiClient *client, PGM_P /*const char **/headers)
{
  WebPrintf(client, "HTTP/1.1 200 OK\r\n");
  WebPrintf(client, "Server: PsychoPlug\r\n");
  WebPrintf(client, "Content-type: text/html\r\n");
  WebPrintf(client, "Cache-Control: no-cache, no-store, must-revalidate\r\n");
  WebPrintf(client, "Pragma: no-cache\r\n");
  WebPrintf(client, "Connection: close\r\n");
  WebPrintf(client, "Expires: 0\r\n");
  if (headers) {
    WebPrintfPSTR(client, headers);
  }
  WebPrintf(client, "\r\n");
}




// In-place decoder, overwrites source with decoded values.  Needs 0-termination on input
// Try and keep memory needs low, speed not critical
static uint8_t b64lut(uint8_t i)
{
  if (i >= 'A' && i <= 'Z') return i - 'A';
  if (i >= 'a' && i <= 'z') return i - 'a' + 26;
  if (i >= '0' && i <= '9') return i - '0' + 52;
  if (i == '-') return 62;
  if (i == '_') return 63;
  else return 64;// sentinel
}

void Base64Decode(char *str)
{
  char *dest;
  dest = str;

  if (strlen(str)%4) return; // Not multiple of 4 == error
  
  while (*str) {
    uint8_t a = b64lut(*(str++));
    uint8_t b = b64lut(*(str++));
    uint8_t c = b64lut(*(str++));
    uint8_t d = b64lut(*(str++));
    *(dest++) = (a << 2) | ((b & 0x30) >> 4);
    if (c == 64) break;
    *(dest++) = ((b & 0x0f) << 4) | ((c & 0x3c) >> 2);
    if (d == 64) break;
    *(dest++) = ((c & 0x03) << 6) | d;
  }
  *dest = 0; // Terminate the string
}


void URLDecode(char *ptr)
{
  while (*ptr) {
    if (*ptr == '+') {
      *ptr = ' ';
    } else if (*ptr == '%') {
      if (*(ptr+1) && *(ptr+2)) {
        byte a = *(ptr + 1);
        byte b = *(ptr + 2);
        if (a>='0' && a<='9') a -= '0';
        else if (a>='a' && a<='f') a = a - 'a' + 10;
        else if (a>='A' && a<='F') a = a - 'A' + 10;
        if (b>='0' && b<='9') b -= '0';
        else if (b>='a' && b<='f') b = b - 'a' + 10;
        else if (b>='A' && b<='F') b = b - 'A' + 10;
        *ptr = ((a&0x0f)<<4) | (b&0x0f);
        // Safe strcpy the rest of the string back
        char *p1 = ptr + 1;
        char *p2 = ptr + 3;
        while (*p2) { *p1 = *p2; p1++; p2++; }
        *p1 = 0;
      }
      // OTW this is a bad encoding, just pass unchanged
    }
    ptr++;
  }
}



// Parse HTTP request
bool WebReadRequest(WiFiClient *client, char *reqBuff, int reqBuffLen, char **urlStr, char **paramStr)
{
  static char NUL = 0; // Get around writable strings...

  *urlStr = NULL;
  *paramStr = NULL;

  unsigned long timeoutMS = millis() + 5000; // Max delay before we timeout
  while (!client->available() && millis() < timeoutMS) { delay(10); }
  if (!client->available()) {
    return false;
  }
  int wlen = client->readBytesUntil('\r', reqBuff, reqBuffLen-1);
  reqBuff[wlen] = 0;

  
  // Delete HTTP version (well, anything after the 2nd space)
  char *ptr = reqBuff;
  while (*ptr && *ptr!=' ') ptr++;
  if (*ptr) ptr++;
  while (*ptr && *ptr!=' ') ptr++;
  *ptr = 0;

  URLDecode(reqBuff);

  char *url;
  char *qp;
  if (!memcmp_P(reqBuff, PSTR("GET "), 4)) {
    client->flush(); // Don't need anything here...
    
    // Break into URL and form data
    url = reqBuff+4;
    while (*url && *url=='/') url++; // Strip off leading /s
    qp = strchr(url, '?');
    if (qp) {
      *qp = 0; // End URL
      qp++;
    } else {
      qp = &NUL;
    }
  } else if (!memcmp_P(reqBuff, PSTR("POST "), 5)) {
    uint8_t newline;
    client->read(&newline, 1); // Get rid of \n

    url = reqBuff+5;
    while (*url && *url=='/') url++; // Strip off leading /s
    qp = strchr(url, '?');
    if (qp) *qp = 0; // End URL @ ?
    // In a POST the params are in the body
    int sizeleft = reqBuffLen - strlen(reqBuff) - 1;
    qp = reqBuff + strlen(reqBuff) + 1;
    int wlen = client->readBytesUntil('\r', qp, sizeleft-1);
    qp[wlen] = 0;
    client->flush();
    URLDecode(qp);
  } else {
    // Not a GET or POST, error
    WebError(client, 405, PSTR("Allow: GET, POST"));
    return false;
  }

  if (urlStr) *urlStr = url;
  if (paramStr) *paramStr = qp;

  return true;
}



// Scan out and update a pointeinto the param string, returning the name and value or false if done
bool ParseParam(char **paramStr, char **name, char **value)
{
  char *data = *paramStr;
 
  if (*data==0) return false;
  
  char *namePtr = data;
  while ((*data != 0) && (*data != '=') && (*data != '&')) data++;
  if (*data) { *data = 0; data++; }
  char *valPtr = data;
  if  (*data == '=') data++;
  while ((*data != 0) && (*data != '=') && (*data != '&')) data++;
  if (*data) { *data = 0; data++;}
  
  *paramStr = data;
  *name = namePtr;
  *value = valPtr;

  return true;
}

bool IsIndexHTML(const char *url)
{
  if (!url) return false;
  if (*url==0 || !strcmp_P(url, PSTR("/")) || !strcmp_P(url, PSTR("/index.html")) || !strcmp_P(url, PSTR("index.html"))) return true;
  else return false;
}





void WebFormText(WiFiClient *client, /*const char **/ PGM_P label, const char *name, const char *value, bool enabled)
{
  WebPrintfPSTR(client, label);
  WebPrintf(client, ": <input type=\"text\" name=\"%s\" id=\"%s\" value=\"%s\" %s><br>\n", name, name, value, !enabled?"disabled":"");
}
void WebFormText(WiFiClient *client, /*const char **/ PGM_P label, const char *name, const int value, bool enabled)
{
  WebPrintfPSTR(client, label);
  WebPrintf(client, ": <input type=\"text\" name=\"%s\" id=\"%s\" value=\"%d\" %s><br>\n", name, name, value, !enabled?"disabled":"");
}
void WebFormCheckbox(WiFiClient *client, /*const char **/ PGM_P label, const char *name, bool checked, bool enabled)
{
  WebPrintf(client, "<input type=\"checkbox\" name=\"%s\" id=\"%s\" %s %s> ", name, name, checked?"checked":"", !enabled?"disabled":"");
  WebPrintfPSTR(client, label);
  WebPrintf(client, "<br>\n");
}
void WebFormCheckboxDisabler(WiFiClient *client, PGM_P /*const char **/label, const char *name, bool invert, bool checked, bool enabled, const char *ids[])
{
  WebPrintf(client,"<input type=\"checkbox\" name=\"%s\" id=\"%s\" onclick=\"", name,name);
  if (invert) WebPrintf(client, "var x = true; if (this.checked) { x = false; }\n")
  else WebPrintf(client, "var x = false; if (this.checked) { x = true; }\n");
  for (byte i=0; ids[i][0]; i++ ) {
    WebPrintf(client, "document.getElementById('%s').disabled = x;\n", ids[i]);
  }
  WebPrintf(client, "\" %s %s> ", checked?"checked":"", !enabled?"disabled":"")
  WebPrintfPSTR(client, label);
  WebPrintf(client, "<br>\n");
}

// Scan an integer from a string, place it into dest, and then return # of bytes scanned
int ParseInt(char *src, int *dest)
{
  byte count = 0;
  bool neg = false;
  int res = 0;
  if (!src) return 0;
  if (src[0] == '-') {neg = true; src++; count++;}
  while (*src && (*src>='0') && (*src<='9')) {
    res = res * 10;
    res += *src - '0';
    src++;
    count++;
  }
  if (neg) res *= -1;
  if (dest) *dest = res;
  return count;
}

void Read4Int(char *str, byte *p)
{
  int i;
  str += ParseInt(str, &i); p[0] = i; if (*str) str++;
  str += ParseInt(str, &i); p[1] = i; if (*str) str++;
  str += ParseInt(str, &i); p[2] = i; if (*str) str++;
  str += ParseInt(str, &i); p[3] = i;
}




