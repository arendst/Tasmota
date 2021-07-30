/*
  AudioFileSourceICYStream
  Streaming Shoutcast ICY source
  
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

#if defined(ESP32) || defined(ESP8266)

#define _GNU_SOURCE

#include "AudioFileSourceICYStream.h"
#include <string.h>

AudioFileSourceICYStream::AudioFileSourceICYStream()
{
  pos = 0;
  reconnectTries = 0;
  saveURL[0] = 0;
}

AudioFileSourceICYStream::AudioFileSourceICYStream(const char *url)
{
  saveURL[0] = 0;
  reconnectTries = 0;
  open(url);
}

bool AudioFileSourceICYStream::open(const char *url)
{
  static const char *hdr[] = { "icy-metaint", "icy-name", "icy-genre", "icy-br" };
  pos = 0;
  http.begin(client, url);
  http.addHeader("Icy-MetaData", "1");
  http.collectHeaders( hdr, 4 );
  http.setReuse(true);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    cb.st(STATUS_HTTPFAIL, PSTR("Can't open HTTP request"));
    return false;
  }
  if (http.hasHeader(hdr[0])) {
    String ret = http.header(hdr[0]);
    icyMetaInt = ret.toInt();
  } else {
    icyMetaInt = 0;
  }
  if (http.hasHeader(hdr[1])) {
    String ret = http.header(hdr[1]);
//    cb.md("SiteName", false, ret.c_str());
  }
  if (http.hasHeader(hdr[2])) {
    String ret = http.header(hdr[2]);
//    cb.md("Genre", false, ret.c_str());
  }
  if (http.hasHeader(hdr[3])) {
    String ret = http.header(hdr[3]);
//    cb.md("Bitrate", false, ret.c_str());
  }

  icyByteCount = 0;
  size = http.getSize();
  strncpy(saveURL, url, sizeof(saveURL));
  saveURL[sizeof(saveURL)-1] = 0;
  return true;
}

AudioFileSourceICYStream::~AudioFileSourceICYStream()
{
  http.end();
}

uint32_t AudioFileSourceICYStream::readInternal(void *data, uint32_t len, bool nonBlock)
{
  // Ensure we can't possibly read 2 ICY headers in a single go #355
  if (icyMetaInt > 1) {
    len = std::min((int)(icyMetaInt >> 1), (int)len);
  }
retry:
  if (!http.connected()) {
    cb.st(STATUS_DISCONNECTED, PSTR("Stream disconnected"));
    http.end();
    for (int i = 0; i < reconnectTries; i++) {
      char buff[64];
      sprintf_P(buff, PSTR("Attempting to reconnect, try %d"), i);
      cb.st(STATUS_RECONNECTING, buff);
      delay(reconnectDelayMs);
      if (open(saveURL)) {
        cb.st(STATUS_RECONNECTED, PSTR("Stream reconnected"));
        break;
      }
    }
    if (!http.connected()) {
      cb.st(STATUS_DISCONNECTED, PSTR("Unable to reconnect"));
      return 0;
    }
  }
  if ((size > 0) && (pos >= size)) return 0;

  WiFiClient *stream = http.getStreamPtr();

  // Can't read past EOF...
  if ( (size > 0) && (len > (uint32_t)(pos - size)) ) len = pos - size;

  if (!nonBlock) {
    int start = millis();
    while ((stream->available() < (int)len) && (millis() - start < 500)) yield();
  }

  size_t avail = stream->available();
  if (!nonBlock && !avail) {
    cb.st(STATUS_NODATA, PSTR("No stream data available"));
    http.end();
    goto retry;
  }
  if (avail == 0) return 0;
  if (avail < len) len = avail;

  int read = 0;
  int ret = 0;
  // If the read would hit an ICY block, split it up...
  if (((int)(icyByteCount + len) > (int)icyMetaInt) && (icyMetaInt > 0)) {
    int beforeIcy = icyMetaInt - icyByteCount;
    if (beforeIcy > 0) {
      ret = stream->read(reinterpret_cast<uint8_t*>(data), beforeIcy);
      if (ret < 0) ret = 0;
      read += ret;
      pos += ret;
      len -= ret;
      data = (void *)(reinterpret_cast<char*>(data) + ret);
      icyByteCount += ret;
      if (ret != beforeIcy) return read; // Partial read
    }

    // ICY MD handling
    int mdSize;
    uint8_t c;
    int mdret = stream->read(&c, 1);
    if (mdret==0) return read;
    mdSize = c * 16;
    if ((mdret == 1) && (mdSize > 0)) {
      // This is going to get ugly fast.
      char icyBuff[256 + 16 + 1];
      char *readInto = icyBuff + 16;
      memset(icyBuff, 0, 16); // Ensure no residual matches occur
      while (mdSize) {
        int toRead = mdSize > 256 ? 256 : mdSize;
        int ret = stream->read((uint8_t*)readInto, toRead);
        if (ret < 0) return read;
        if (ret == 0) { delay(1); continue; }
        mdSize -= ret;
        // At this point we have 0...15 = last 15 chars read from prior read plus new data
        int end = 16 + ret; // The last byte of valid data
        char *header = (char *)memmem((void*)icyBuff, end, (void*)"StreamTitle=", 12);
        if (!header) {
          // No match, so move the last 16 bytes back to the start and continue
          memmove(icyBuff, icyBuff+end-16, 16);
          delay(1);
	  continue;
        }
        // Found header, now move it to the front
        int lastValidByte = end - (header -icyBuff) + 1;
        memmove(icyBuff, header, lastValidByte);
        // Now fill the buffer to the end with read data
        while (mdSize && lastValidByte < 255) {
          int toRead = mdSize > (256 - lastValidByte) ? (256 - lastValidByte) : mdSize;
          ret = stream->read((uint8_t*)icyBuff + lastValidByte, toRead);
          if (ret==-1) return read; // error
          if (ret == 0) { delay(1); continue; }
          mdSize -= ret;
          lastValidByte += ret;
        }
        // Buffer now contains StreamTitle=....., parse it
        char *p = icyBuff+12;
        if (*p=='\'' || *p== '"' ) {
          char closing[] = { *p, ';', '\0' };
          char *psz = strstr( p+1, closing );
          if( !psz ) psz = strchr( &icyBuff[13], ';' );
          if( psz ) *psz = '\0';
          p++;
        } else {
          char *psz = strchr( p, ';' );
          if( psz ) *psz = '\0';
        }
        cb.md("StreamTitle", false, p);

        // Now skip rest of MD block
        while (mdSize) {
          int toRead = mdSize > 256 ? 256 : mdSize;
          ret = stream->read((uint8_t*)icyBuff, toRead);
          if (ret < 0) return read;
          if (ret == 0) { delay(1); continue; }
          mdSize -= ret;
        }
      }
    }
    icyByteCount = 0;
  }

  ret = stream->read(reinterpret_cast<uint8_t*>(data), len);
  if (ret < 0) ret = 0;
  read += ret;
  pos += ret;
  icyByteCount += ret;
  return read;
}

#endif
