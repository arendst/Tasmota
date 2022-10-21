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

#ifndef _web_h
#define _web_h

// Global way of writing out dynamic HTML to socket
// snprintf guarantees a null termination
#define WebPrintf(c, fmt, ...) { char webBuff[192]; snprintf_P(webBuff, sizeof(webBuff), PSTR(fmt), ## __VA_ARGS__); (c)->print(webBuff); delay(10);}
#define WebPrintfPSTR(c, fmt, ...) { char webBuff[192]; snprintf_P(webBuff, sizeof(webBuff), (fmt), ## __VA_ARGS__); (c)->print(webBuff); delay(10);}

// Common HTTP header bits
#define DOCTYPE "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"
#define ENCODING "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"


// Web header creation
void WebPrintError(WiFiClient *client, int code); // Sends only the error code string and a description
void WebError(WiFiClient *client, int code, const char *headers, bool usePMEM = true); // Sends whole HTTP error headers
void WebHeaders(WiFiClient *client, PGM_P /*const char **/headers); // Send success headers

// Web decoding utilities
void Base64Decode(char *str); // In-place B64 decode
void URLDecode(char *ptr); // In-place URL decode

// GET/POST parsing
bool WebReadRequest(WiFiClient *client, char *reqBuff, int reqBuffLen, char **urlStr, char **paramStr);
bool ParseParam(char **paramStr, char **name, char **value); // Get next name/parameter from a param string
bool IsIndexHTML(const char *url); // Is this meant to be index.html (/, index.htm, etc.)

// HTML FORM generation
void WebFormText(WiFiClient *client, /*const char **/ PGM_P label, const char *name, const char *value, bool enabled);
void WebFormText(WiFiClient *client, /*const char **/ PGM_P label, const char *name, const int value, bool enabled);
void WebFormCheckbox(WiFiClient *client, /*const char **/ PGM_P label, const char *name, bool checked, bool enabled);
void WebFormCheckboxDisabler(WiFiClient *client, PGM_P /*const char **/label, const char *name, bool invert, bool checked, bool enabled, const char *ids[]);

// HTML FORM parsing
int ParseInt(char *src, int *dest);
void Read4Int(char *str, byte *p);
#define ParamText(name, dest)     { if (!strcmp(namePtr, (name))) strlcpy((dest), valPtr, sizeof(dest)); }
#define ParamCheckbox(name, dest) { if (!strcmp(namePtr, (name))) (dest) = !strcmp("on", valPtr); }
#define ParamInt(name, dest)      { if (!strcmp(namePtr, (name))) ParseInt(valPtr, &dest); }
#define Param4Int(name, dest)     { if (!strcmp(namePtr, (name))) Read4Int(valPtr, (dest)); }


#endif

