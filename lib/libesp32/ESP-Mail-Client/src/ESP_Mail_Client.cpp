#ifndef ESP_MAIL_CLIENT_CPP
#define ESP_MAIL_CLIENT_CPP

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

/**
 * Mail Client Arduino Library for Arduino devices.
 *
 * Created August 28, 2023
 *
 * This library allows Espressif's ESP32, ESP8266, SAMD and RP2040 Pico devices to send and read Email through the SMTP and IMAP servers.
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ESP_Mail_Client.h"
#include "ESP_Mail_Client_Version.h"

#include "ESP_Mail_IMAP.h"
#include "ESP_Mail_SMTP.h"

void ESP_Mail_Client::networkReconnect(bool reconnect)
{
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE) && (defined(ESP32) || defined(ESP8266))
  WiFi.setAutoReconnect(reconnect);
#endif
  networkAutoReconnect = reconnect;
}

void ESP_Mail_Client::printf(const char *format, ...)
{
  int size = 2048;
  char s[size];
  va_list va;
  va_start(va, format);
  vsnprintf(s, size, format, va);
  va_end(va);
  ESP_MAIL_DEFAULT_DEBUG_PORT.print(s);
}

void ESP_Mail_Client::addAP(const String &ssid, const String &password)
{
  wifi.addAP(ssid, password);
}

void ESP_Mail_Client::clearAP()
{
  wifi.clearAP();
}

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

bool ESP_Mail_Client::sdBegin(int8_t ss, int8_t sck, int8_t miso, int8_t mosi, uint32_t frequency)
{
  return mbfs->sdBegin(ss, sck, miso, mosi, frequency);
}

#if defined(ESP8266) || defined(MB_ARDUINO_PICO)
bool ESP_Mail_Client::sdBegin(SDFSConfig *sdFSConfig)
{
  return mbfs->sdFatBegin(sdFSConfig);
}
#endif

#if defined(ESP32)
bool ESP_Mail_Client::sdBegin(int8_t ss, SPIClass *spiConfig, uint32_t frequency)
{
  return mbfs->sdSPIBegin(ss, spiConfig, frequency);
}
#endif

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
bool ESP_Mail_Client::sdBegin(SdSpiConfig *sdFatSPIConfig, int8_t ss, int8_t sck, int8_t miso, int8_t mosi)
{
  return mbfs->sdFatBegin(sdFatSPIConfig, ss, sck, miso, mosi);
}

bool ESP_Mail_Client::sdBegin(SdioConfig *sdFatSDIOConfig)
{
  return mbfs->sdFatBegin(sdFatSDIOConfig);
}
#endif

#endif

#if defined(ESP32) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD_MMC)

bool ESP_Mail_Client::sdMMCBegin(const char *mountpoint, bool mode1bit, bool format_if_mount_failed)
{
  return mbfs->sdMMCBegin(mountpoint, mode1bit, format_if_mount_failed);
}

#endif

int ESP_Mail_Client::getFreeHeap()
{
#if defined(MB_ARDUINO_ESP)
  return ESP.getFreeHeap();
#elif defined(MB_ARDUINO_PICO)
  return rp2040.getFreeHeap();
#else
  return 0;
#endif
}

// All following functions are for IMAP or SMTP only
#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

void ESP_Mail_Client::resumeNetwork(ESP_Mail_TCPClient *client)
{
  client->networkReconnect();
}

template <class T>
bool ESP_Mail_Client::sessionExisted(T sessionPtr)
{

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)

  Session_Config *config = sessionPtr->_session_cfg;
  _vectorImpl<int> *configPtrList = &(sessionPtr->_configPtrList);

  if (config)
  {
    int ptr = toAddr(*config);
    for (size_t i = 0; i < configPtrList->size(); i++)
    {
      if ((*configPtrList)[i] == ptr)
        return true;
    }

    sessionPtr->closeSession();
    sessionPtr->_responseStatus.errorCode = MAIL_CLIENT_ERROR_SESSION_CONFIG_WAS_NOT_ASSIGNED;
    sessionPtr->_responseStatus.text.clear();
  }

#endif
  return false;
}

void ESP_Mail_Client::debugPrintNewLine()
{
#if !defined(SILENT_MODE)
  esp_mail_debug_print("", true);
#endif
}

template <class T>
void ESP_Mail_Client::callBackSendNewLine(T sessionPtr, bool success)
{
#if defined(SESSION_DEBUG_ENABLED)
  sendCallback<T>(sessionPtr, "", false, success);
#endif
}

void ESP_Mail_Client::appendTagSpace(MB_String &buf, PGM_P tag)
{
  buf += (tag == NULL) ? esp_mail_imap_tag_str : tag;
  appendSpace(buf);
}

template <class T>
void ESP_Mail_Client::appendList(MB_String &buf, _vectorImpl<T> &list)
{
  for (size_t i = 0; i < list.size(); i++)
  {
    if (i > 0)
      buf += esp_mail_str_8; /* "," */
    buf += list[i];
  }
}

void ESP_Mail_Client::appendSpace(MB_String &buf)
{
  buf += esp_mail_str_2 /* " " */;
}

void ESP_Mail_Client::appendSpace(MB_String &buf, bool withTag, PGM_P value)
{
  if (withTag)
    appendTagSpace(buf);
  buf += value;
  appendSpace(buf);
}

void ESP_Mail_Client::appendSpace(MB_String &buf, bool withTag, int nunArgs, ...)
{
  if (withTag)
    appendTagSpace(buf);

  va_list ap;
  va_start(ap, nunArgs);
  PGM_P p = va_arg(ap, PGM_P);
  if (p)
    buf += p;
  for (int i = 2; i <= nunArgs; i++)
  {
    appendSpace(buf);
    p = va_arg(ap, PGM_P);
    if (p)
      buf += p;
  }
  va_end(ap);
  appendSpace(buf);
}

void ESP_Mail_Client::prependSpace(MB_String &buf, PGM_P value)
{
  appendSpace(buf);
  buf += value;
}

void ESP_Mail_Client::appendDot(MB_String &buf)
{
  buf += esp_mail_str_27; /* "." */
}

void ESP_Mail_Client::prependDot(MB_String &buf, PGM_P value)
{
  buf += esp_mail_str_27; /* "." */
  buf += value;
}

void ESP_Mail_Client::joinStringSpace(MB_String &buf, bool withTag, int nunArgs, ...)
{
  if (withTag)
    appendTagSpace(buf);

  va_list ap;
  va_start(ap, nunArgs);
  PGM_P p = va_arg(ap, PGM_P);
  if (p)
    buf += p;
  for (int i = 2; i <= nunArgs; i++)
  {
    appendSpace(buf);
    p = va_arg(ap, PGM_P);
    if (p)
      buf += p;
  }
  va_end(ap);
}

void ESP_Mail_Client::appendImap4KeyValue(MB_String &buf, PGM_P key, PGM_P value)
{
  buf += esp_mail_str_11; /* "\"" */
  buf += key;
  buf += esp_mail_str_11; /* "\"" */
  appendSpace(buf);
  buf += esp_mail_str_11; /* "\"" */
  buf += value;
  buf += esp_mail_str_11; /* "\"" */
}

void ESP_Mail_Client::joinStringDot(MB_String &buf, int nunArgs, ...)
{
  va_list ap;
  va_start(ap, nunArgs);
  PGM_P p = va_arg(ap, PGM_P);
  if (p)
    buf += p;
  for (int i = 2; i <= nunArgs; i++)
  {
    buf += esp_mail_str_27; /* "." */
    p = va_arg(ap, PGM_P);
    if (p)
      buf += p;
  }
  va_end(ap);
}

template <class T>
void ESP_Mail_Client::sendCallback(T sessionPtr, PGM_P info, bool prependCRLF, bool success)
{

#if defined(SESSION_DEBUG_ENABLED)

  sessionPtr->_cbData._info.clear();

  if (prependCRLF)
    appendNewline(sessionPtr->_cbData._info);
  if (strlen_P(info) > 0)
  {
    sessionPtr->_cbData._info += esp_mail_str_33; /* "#### " */
    sessionPtr->_cbData._info += info;
  }
  sessionPtr->_cbData._success = success;
  if (sessionPtr->_statusCallback && !sessionPtr->_customCmdResCallback)
    sessionPtr->_statusCallback(sessionPtr->_cbData);

#endif
}

template <class T>
void ESP_Mail_Client::printDebug(T sessionPtr, PGM_P cbMsg, PGM_P dbMsg, esp_mail_debug_tag_type type, bool prependCRLF, bool success)
{
#if defined(SESSION_DEBUG_ENABLED)

  if (sessionPtr->_statusCallback != NULL && !isResponseCB<T>(sessionPtr))
    sendCallback<T>(sessionPtr, cbMsg, prependCRLF, success);
  else if (sessionPtr->_debug)
    debugPrintNewLine();

  if (sessionPtr->_debug)
    esp_mail_debug_print_tag(dbMsg, type, true);

#endif
}

void ESP_Mail_Client::printProgress(int progress, int &lastProgress)
{
#if !defined(SILENT_MODE)
  if (progress > 100)
    progress = 100;

  if (lastProgress != progress && (progress == 0 || progress == 100 || lastProgress + ESP_MAIL_PROGRESS_REPORT_STEP <= progress))
  {
    int len = 16;
    int curTick = progress * len / 100;
    int lastTick = lastProgress * len / 100;

    if (curTick > lastTick || progress == 0 || progress == 100)
    {
      MB_String s;
      for (int i = 0; i < len; i++)
      {
        if (i == 0)
          s = '[';
        s += i < progress * len / 100 ? '#' : ' ';
        if (i == len - 1)
          s += ']';
      }
      appendSpace(s);
      s += progress;
      appendSpace(s);
      s += esp_mail_str_24; /* "%" */
      esp_mail_debug_print_tag(s.c_str(), esp_mail_debug_tag_type_client, true);
    }

    lastProgress = progress;
  }
#endif
}

void ESP_Mail_Client::setTimezone(const char *TZ_Var, const char *TZ_file)
{

  if (!TZ_Var)
    return;

#if defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO)
  if (strlen(TZ_Var) > 0)
  {

#if defined(ESP32)

    mb_fs_mem_storage_type type = mb_fs_mem_storage_type_undefined;

#if defined(MBFS_FLASH_FS)
    type = mb_fs_mem_storage_type_flash;
#elif defined(MBFS_SD_FS)
    type = mb_fs_mem_storage_type_sd;
#endif

    if (type != mb_fs_mem_storage_type_undefined)
    {
      MB_String filename = TZ_file;
      if (mbfs->open(filename, type, mb_fs_open_mode_write) > -1)
      {
        mbfs->print(type, TZ_Var);
        mbfs->close(type);
      }
    }

#endif

    setenv("TZ", TZ_Var, 1);
    tzset();
    timezoneEnvSet = true;
  }
#endif
}

void ESP_Mail_Client::preparePortFunction(Session_Config *session_config, bool isSMTP, bool &secure, bool &secureMode, bool &ssl)
{

  if (session_config->ports_functions.list)
  {
    if (session_config->ports_functions.use_internal_list)
    {
      session_config->ports_functions.use_internal_list = false;
      delete[] session_config->ports_functions.list;
      session_config->ports_functions.list = nullptr;
    }
  }

  if (!session_config->ports_functions.list)
  {
    if (isSMTP)
    {
#if defined(ENABLE_SMTP)
      session_config->ports_functions.use_internal_list = true;
      session_config->ports_functions.list = new port_function[3];
      session_config->ports_functions.size = 3;

      for (int i = 0; i < 3; i++)
        session_config->ports_functions.list[i] = smtp_ports[i];
#endif
    }
    else
    {
#if defined(ENABLE_IMAP)
      session_config->ports_functions.use_internal_list = true;
      session_config->ports_functions.list = new port_function[2];
      session_config->ports_functions.size = 2;

      for (int i = 0; i < 2; i++)
        session_config->ports_functions.list[i] = imap_ports[i];
#endif
    }
  }

  getPortFunction(session_config->server.port, session_config->ports_functions, secure, secureMode, ssl, session_config->secure.startTLS);
}

void ESP_Mail_Client::getPortFunction(uint16_t port, struct esp_mail_ports_functions &ports_functions, bool &secure, bool &secureMode, bool &ssl, bool &starttls)
{
  for (size_t i = 0; i < ports_functions.size; i++)
  {
    if (ports_functions.list[i].port == port)
    {
      if (ports_functions.list[i].protocol == esp_mail_protocol_plain_text)
      {
        secure = false;
        secureMode = false;
      }
      else
      {
        if (ports_functions.list[i].protocol == esp_mail_protocol_tls)
          starttls = true;

        secureMode = !starttls;

        if (ports_functions.list[i].protocol == esp_mail_protocol_ssl)
          ssl = true;
      }
      return;
    }
  }
}

void ESP_Mail_Client::getTimezone(const char *TZ_file, MB_String &out)
{

#if defined(ESP32)

  mb_fs_mem_storage_type type = mb_fs_mem_storage_type_undefined;

#if defined(MBFS_FLASH_FS)
  type = mb_fs_mem_storage_type_flash;
#elif defined(MBFS_SD_FS)
  type = mb_fs_mem_storage_type_sd;
#endif

  if (type != mb_fs_mem_storage_type_undefined)
  {
    MB_String filename = TZ_file;

    if (mbfs->open(filename, type, mb_fs_open_mode_read) > 0)
    {
      out.clear();
      while (mbfs->available(type))
      {
        out += (char)mbfs->read(type);
      }
      mbfs->close(type);
    }
  }
#endif
}

void ESP_Mail_Client::setTime(const char *TZ_Var, const char *TZ_file, bool wait, bool debugProgress)
{

  timeStatus = Time.timeReady();

  if (!timeStatus)
  {

#if defined(ENABLE_IMAP) || defined(ENABLE_SMTP)

#if defined(ENABLE_NTP_TIME)

    if (WiFI_CONNECTED)
    {
      Time.readNTPTime(wait ? 10000 : 0, debugProgress);
    }
    else
    {
#if !defined(SILENT_MODE)
      esp_mail_debug_print_tag(esp_mail_error_client_str_4 /* "NTP server time reading cannot begin when valid time is required because of no WiFi capability/activity detected." */, esp_mail_debug_tag_type_warning, true);
      esp_mail_debug_print_tag(esp_mail_error_client_str_5 /* "Please set the library reference time manually using smtp.setSystemTime or imap.setSystemTime." */, esp_mail_debug_tag_type_warning, true);
#endif
    }

    getSetTimezoneEnv(TZ_file, TZ_Var);

#elif !defined(SILENT_MODE)
    esp_mail_debug_print_tag(esp_mail_error_client_str_5 /* "Please set the library reference time manually using smtp.setSystemTime or imap.setSystemTime." */, esp_mail_debug_tag_type_warning, true);
#endif

#endif
  }

  timeStatus = Time.timeReady();
}

void ESP_Mail_Client::getSetTimezoneEnv(const char *TZ_file, const char *TZ_Var)
{
  // set and get TZ environment variable

  MB_String timezone;

  // only ESP32 only
  getTimezone(TZ_file, timezone);

  if (timezone.length() == 0)
    timezone = TZ_Var;

  // if timezone string assign
  setTimezone(timezone.c_str(), TZ_file);
}

bool ESP_Mail_Client::validEmail(const char *s)
{
  MB_String str(s);
  size_t at = str.find('@');
  size_t dot = str.find('.', at);
  return (at != MB_String::npos) && (dot != MB_String::npos);
}

#if defined(ENABLE_SMTP) && defined(ENABLE_IMAP)

bool ESP_Mail_Client::mAppendMessage(IMAPSession *imap, SMTP_Message *msg, bool lastAppend, MB_StringPtr flags, MB_StringPtr dateTime)
{
  this->imap = imap;
  calDataLen = true;
  dataLen = 0;
  imap_ts = 0;

  if (!sessionExisted<IMAPSession *>(imap))
    return false;

  MB_String _flags = flags;
  _flags.trim();

  MB_String _dt = dateTime;
  _dt.trim();

  bool rfc822MSG = false;

  sendContent(nullptr, msg, false, rfc822MSG);

  MB_String cmd;

  if (!imap->_feature_capability[esp_mail_imap_read_capability_multiappend])
  {
    lastAppend = true;
    imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
  }

  if (imap->_prev_imap_cmd != esp_mail_imap_cmd_append)
    joinStringSpace(cmd, true, 2, imap_commands[esp_mail_imap_command_append].text, imap->_currentFolder.c_str());

  appendSpace(cmd);

  if (_flags.length() > 0)
  {
    appendString(cmd, _flags.c_str(), false, false, esp_mail_string_mark_type_round_bracket);
    appendSpace(cmd);
  }

  if (_dt.length() > 0)
  {
    appendString(cmd, _dt.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    appendSpace(cmd);
  }

  appendString(cmd, MB_String((int)dataLen).c_str(), false, false, esp_mail_string_mark_type_curly_bracket);

  if (imapSend(imap, cmd.c_str(), true) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
  {
    imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
    return false;
  }

  imap->_imap_cmd = esp_mail_imap_command::esp_mail_imap_cmd_append;

  if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, false))
  {
    imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
    return false;
  }

  calDataLen = false;

  rfc822MSG = false;

  if (!sendContent(nullptr, msg, false, rfc822MSG))
  {
    imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
    return false;
  }

  if (lastAppend)
  {
    if (imapSend(imap, esp_mail_str_18 /* "\r\n" */, false) == ESP_MAIL_CLIENT_TRANSFER_DATA_FAILED)
    {
      imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
      return false;
    }

    imap->_imap_cmd = esp_mail_imap_command::esp_mail_imap_cmd_append_last;

    if (!handleIMAPResponse(imap, IMAP_STATUS_BAD_COMMAND, false))
    {
      imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
      return false;
    }

    imap->_prev_imap_cmd = esp_mail_imap_cmd_sasl_login;
  }

  if (!lastAppend)
    imap->_prev_imap_cmd = esp_mail_imap_cmd_append;
  else
  {
#if !defined(SILENT_MODE)
    altSendCallback(nullptr, esp_mail_cb_str_45 /* "Message append successfully" */, esp_mail_dbg_str_73 /* "message append successfully" */, esp_mail_debug_tag_type_client, true, false);
#endif
  }
  return true;
}

#endif

char *ESP_Mail_Client::getRandomUID()
{
  char *tmp = allocMem<char *>(36);
  sprintf(tmp, "%d", (int)random(2000, 4000));
  return tmp;
}

void ESP_Mail_Client::splitToken(const char *str, _vectorImpl<MB_String> &tk, const char *delim)
{
  char *p = allocMem<char *>(strlen(str));
  strcpy(p, str);
  char *pp = p;
  char *end = p;
  MB_String tmp;
  while (pp != NULL)
  {
    // See RFC2047.h
    ESP_MAIL_STRSEP(&end, delim);
    if (strlen(pp) > 0)
    {
      tmp = pp;
      tk.push_back(tmp);
    }
    pp = end;
  }
  // release memory
  freeMem(&p);
}

int ESP_Mail_Client::strpos(const char *haystack, const char *needle, int offset, bool caseSensitive)
{
  if (!haystack || !needle)
    return -1;

  int hlen = strlen(haystack);
  int nlen = strlen(needle);

  if (hlen == 0 || nlen == 0)
    return -1;

  int hidx = offset, nidx = 0;
  while ((*(haystack + hidx) != '\0') && (*(needle + nidx) != '\0') && hidx < hlen)
  {

    bool nm = caseSensitive ? *(needle + nidx) != *(haystack + hidx) : tolower(*(needle + nidx)) != tolower(*(haystack + hidx));

    if (nm)
    {
      hidx++;
      nidx = 0;
    }
    else
    {
      nidx++;
      hidx++;
      if (nidx == nlen)
        return hidx - nidx;
    }
  }

  return -1;
}

char *ESP_Mail_Client::subStr(const char *buf, PGM_P beginToken, PGM_P endToken, int beginPos, int endPos, bool caseSensitive)
{
  char *tmp = nullptr;
  if (beginToken)
  {
    int p1 = strposP(buf, beginToken, beginPos, caseSensitive);
    if (p1 != -1)
    {
      while (buf[p1 + strlen_P(beginToken)] == ' ' || buf[p1 + strlen_P(beginToken)] == '\r' || buf[p1 + strlen_P(beginToken)] == '\n')
      {
        p1++;
        if (strlen(buf) <= p1 + strlen_P(beginToken))
        {
          p1--;
          break;
        }
      }

      int p2 = -1;
      if (endPos == 0)
        p2 = strposP(buf, endToken, p1 + strlen_P(beginToken), caseSensitive);

      if (p2 == -1)
        p2 = strlen(buf);

      int len = p2 - p1 - strlen_P(beginToken);
      int ofs = endToken ? strlen_P(endToken) : 1;
      tmp = allocMem<char *>(len + ofs);
      memcpy(tmp, &buf[p1 + strlen_P(beginToken)], len);
    }
  }
  else
  {
    int p1 = strposP(buf, endToken, beginPos);
    if (p1 != -1)
    {
      tmp = allocMem<char *>(p1);
      memcpy(tmp, &buf[2], p1 - 1);
    }
  }

  return tmp;
}

bool ESP_Mail_Client::getHeader(const char *buf, PGM_P beginToken, MB_String &out, bool caseSensitive)
{
  if (strcmpP(buf, 0, beginToken, caseSensitive))
  {
    char *tmp = subStr(buf, beginToken, NULL, 0, -1, caseSensitive);
    if (tmp)
    {
      out = tmp;
      // release memory
      freeMem(&tmp);
      return true;
    }
  }

  return false;
}

void ESP_Mail_Client::appendHeaderField(MB_String &buf, const char *name, PGM_P value, bool comma, bool newLine, esp_mail_string_mark_type type)
{
  appendHeaderName(buf, name);
  appendString(buf, value, comma, newLine, type);
}

void ESP_Mail_Client::appendAddressHeaderField(MB_String &buf, esp_mail_address_info_t &source, esp_mail_rfc822_header_field_types type, bool header, bool comma, bool newLine)
{
  // Construct header field.
  if (header)
    appendHeaderName(buf, rfc822_headers[type].text);

  if (type != esp_mail_rfc822_header_field_cc && type != esp_mail_rfc822_header_field_bcc &&
      source.name.length() > 0)
  {
    appendString(buf, source.name.c_str(), false, false, esp_mail_string_mark_type_double_quote);
    // Add white space after name for SMTP to fix iCloud Mail Service IMAP search compatibility issue #278
    // This is not restricted by rfc2822.
    appendSpace(buf);
  }

  appendString(buf, source.email.c_str(), comma, newLine, esp_mail_string_mark_type_angle_bracket);
}

void ESP_Mail_Client::appendHeaderName(MB_String &buf, const char *name, bool clear, bool lowercase, bool space)
{
  if (clear)
    buf.clear();

  if (lowercase)
    appendLowerCaseString(buf, name);
  else
    buf += name;
  buf += esp_mail_str_34; /* ":" */
  if (space)
    appendSpace(buf);
}

void ESP_Mail_Client::appendLowerCaseString(MB_String &buf, PGM_P value, bool clear)
{
  if (clear)
    buf.clear();
  char *tmp = strP2Lower(value);
  buf += tmp;
  freeMem(&tmp);
}

void ESP_Mail_Client::appendHeaderProp(MB_String &buf, PGM_P prop, const char *value, bool &firstProp, bool lowerCase, bool isString, bool newLine)
{
  if (firstProp)
    buf += esp_mail_str_35; /* ";" */
  appendSpace(buf);
  if (lowerCase)
    appendLowerCaseString(buf, prop);
  else
    buf += prop;
  buf += esp_mail_str_7; /* "=" */
  if (isString)
    buf += esp_mail_str_11; /* "\"" */
  buf += value;
  if (isString)
    buf += esp_mail_str_11; /* "\"" */
  buf += esp_mail_str_35;   /* ";" */
  if (newLine)
    appendNewline(buf);

  firstProp = false;
}

void ESP_Mail_Client::appendString(MB_String &buf, PGM_P value, bool comma, bool newLine, esp_mail_string_mark_type type)
{
  if (comma)
    buf += esp_mail_str_8; /* "," */

  switch (type)
  {
  case esp_mail_string_mark_type_double_quote:
    buf += esp_mail_str_11; /* "\"" */
    break;
  case esp_mail_string_mark_type_angle_bracket:
    buf += esp_mail_str_19; /* "<" */
    break;
  case esp_mail_string_mark_type_round_bracket:
    buf += esp_mail_str_38; /* "(" */
    break;
  case esp_mail_string_mark_type_curly_bracket:
    buf += esp_mail_str_36; /* "{" */
    break;
  case esp_mail_string_mark_type_square_bracket:
    buf += esp_mail_str_40; /* "[" */
    break;
  default:
    break;
  }

  if (value)
    buf += value;

  switch (type)
  {
  case esp_mail_string_mark_type_double_quote:
    buf += esp_mail_str_11; /* "\"" */
    break;
  case esp_mail_string_mark_type_angle_bracket:
    buf += esp_mail_str_20; /* ">" */
    break;
  case esp_mail_string_mark_type_round_bracket:
    buf += esp_mail_str_39; /* ")" */
    break;
  case esp_mail_string_mark_type_curly_bracket:
    buf += esp_mail_str_37; /* "}" */
    break;
  case esp_mail_string_mark_type_square_bracket:
    buf += esp_mail_str_41; /* "]" */
    break;
  default:
    break;
  }

  if (newLine)
    appendNewline(buf);
}

void ESP_Mail_Client::maskString(MB_String &buf, int len)
{
  for (int i = 0; i < len; i++)
    buf += esp_mail_str_3; /* "*" */
}

void ESP_Mail_Client::appendDomain(MB_String &buf, const char *domain)
{
  buf += strlen(domain) > 0 ? domain : pgm2Str(esp_mail_str_1 /* "127.0.0.1" */);
}

void ESP_Mail_Client::appendEmbedMessage(MB_String &buf, esp_mail_message_body_t &body, bool isHtml)
{
  appendHeaderName(buf, message_headers[esp_mail_message_header_field_content_disposition].text);
  appendString(buf, body.embed.type == esp_mail_smtp_embed_message_type_inline ? esp_mail_content_disposition_type_t::inline_ : esp_mail_content_disposition_type_t::attachment, false, false);

  PGM_P pgm = isHtml ? esp_mail_str_14 /* "msg.html" */ : esp_mail_str_13; /* "msg.txt" */
  MB_String filename;
  if (body.embed.filename.length() > 0)
    filename = body.embed.filename;
  else
    filename = pgm;

  bool firstProp = true;
  appendHeaderProp(buf, message_headers[esp_mail_message_header_field_filename].text, filename.c_str(), firstProp, true, true, true);

  if (body.embed.type == esp_mail_smtp_embed_message_type_inline)
  {
    appendHeaderName(buf, message_headers[esp_mail_message_header_field_content_location].text);
    body.embed.filename.length() > 0 ? appendString(buf, body.embed.filename.c_str(), false, true) : appendString(buf, pgm, false, true);
    appendHeaderField(buf, message_headers[esp_mail_message_header_field_content_id].text, body._int.cid.c_str(), false, true, esp_mail_string_mark_type_angle_bracket);
  }
}

void ESP_Mail_Client::appendNewline(MB_String &buf)
{
  buf += esp_mail_str_18; /* "\r\n" */
}

void ESP_Mail_Client::getExtfromMIME(const char *mime, MB_String &ext)
{
  ext.clear();
  for (int i = 0; i < esp_mail_file_extension_maxType; i++)
  {
    if (strcmp_P(mime, mimeinfo[i].mimeType) == 0)
    {
      ext = mimeinfo[i].endsWith;
      break;
    }
  }

  if (ext.length() == 0)
    ext = esp_mail_str_42; /* ".dat" */
}

MB_String ESP_Mail_Client::mGetBase64(MB_StringPtr str)
{
  MB_String data = str;
  return encodeBase64Str((uint8_t *)(data.c_str()), data.length());
}

int ESP_Mail_Client::readLine(ESP_Mail_TCPClient *client, char *buf, int bufLen, bool withLineBreak, int &count, bool &ovf, unsigned long timeoutSec, bool &isTimeout)
{
  int ret = -1;
  char c = 0;
  char _c = 0;
  int idx = 0;
  ovf = idx >= bufLen;
  bool lineBreak = false;
  isTimeout = false;

  unsigned long ms = millis();

  // Instead of relying on data available, we looks for line break until timed out or disconnected or overflown occurred.
  while (idx < bufLen && client->connected() && (!lineBreak || client->available() /* data may not available sometimes */))
  {
    if (millis() - ms >= timeoutSec * 1000)
    {
      isTimeout = true;
      break;
    }

    yield_impl();

    ret = client->read();
    if (ret > -1)
    {
      c = (char)ret;
      buf[idx++] = c;
      count++;
      if (_c == '\r' && c == '\n')
      {
        lineBreak = true;
        if (!withLineBreak)
        {
          buf[idx - 2] = 0;
          idx -= 2;
        }
        return idx;
      }
      _c = c;

      if (idx >= bufLen - 1)
      {
        ovf = true;
        return idx;
      }
    }
  }
  return idx;
}

template <class T>
bool ESP_Mail_Client::readResponse(T sessionPtr, char *buf, int bufLen, int &readLen, bool withLineBreak, int &count, MB_String &ovfBuf)
{
  bool ovf = false, isTimeout = false;
  unsigned long timeoutSec = TCP_CLIENT_DEFAULT_TCP_TIMEOUT_SEC;

  do
  {
    timeoutSec = sessionPtr->client.tcpTimeout();
    int len = readLine(&(sessionPtr->client), buf, bufLen, withLineBreak, count, ovf, timeoutSec, isTimeout);
    readLen += len;
    if (len > 0 && (ovf || ovfBuf.length() > 0))
      ovfBuf += buf;

  } while (ovf);

  if (isTimeout)
    return false;

  if (ovfBuf.length() > 0)
  {

#if defined(SESSION_DEBUG_ENABLED)
    sessionPtr->_responseStatus.errorCode = MAIL_CLIENT_ERROR_BUFFER_OVERFLOW;
    sessionPtr->_responseStatus.text.clear();
    if (sessionPtr->_debug)
      esp_mail_debug_print_tag(sessionPtr->errorReason().c_str(), esp_mail_debug_tag_type_warning, true);
#endif
  }

  return true;
}

template <class T>
bool ESP_Mail_Client::reconnect(T sessionPtr, unsigned long dataTime, bool downloadRequest)
{
  if (!sessionPtr)
    return false;

  sessionPtr->client.setSession(sessionPtr->_session_cfg);
  networkStatus = sessionPtr->client.networkReady();

  if (dataTime > 0)
  {
    if (millis() - dataTime > (unsigned long)sessionPtr->client.tcpTimeout())
    {
      closeTCPSession<T>(sessionPtr);

      if (sessionPtr->_sessionType == esp_mail_session_type_imap)
      {
#if defined(ENABLE_IMAP)
        IMAPSession *ss = (IMAPSession *)sessionPtr;
        if (ss->_headers.size() > 0)
        {
          if (downloadRequest)
          {
            errorStatusCB<IMAPSession *, IMAPSession *>(ss, nullptr, IMAP_STATUS_ERROR_DOWNLAD_TIMEOUT, true);
            if (cPart(ss) && cHeader(ss)->part_headers.size() > 0)
              cPart(ss)->download_error = ss->errorReason().c_str();
          }
          else
          {
            errorStatusCB<IMAPSession *, IMAPSession *>(ss, nullptr, MAIL_CLIENT_ERROR_READ_TIMEOUT, true);
            if (cHeader(ss))
              cHeader(ss)->error_msg = ss->errorReason().c_str();
          }
        }
        else
        {
#if !defined(SILENT_MODE)
          if (sessionPtr->_debug)
            esp_mail_debug_print_tag(esp_mail_error_network_str_9 /* "response read timed out" */, esp_mail_debug_tag_type_error, true);
#endif
        }
#endif
      }
      else
      {
#if defined(ENABLE_SMTP)
        SMTPSession *ss = (SMTPSession *)sessionPtr;
        errorStatusCB<SMTPSession *, IMAPSession *>(ss, this->imap, MAIL_CLIENT_ERROR_READ_TIMEOUT, false);
#endif
      }
      return false;
    }
  }

  if (!networkStatus)
  {
    closeTCPSession<T>(sessionPtr);

    if (sessionPtr->_sessionType == esp_mail_session_type_imap)
    {
#if defined(ENABLE_IMAP)
      IMAPSession *ss = (IMAPSession *)sessionPtr;
      if (ss->_mbif._idleTimeMs > 0 || ss->_imap_cmd == esp_mail_imap_cmd_idle || ss->_imap_cmd == esp_mail_imap_cmd_done)
      {
        // defer the polling error report
        if (millis() - ss->_last_polling_error_ms > 10000 && !sessionPtr->connected())
        {
          ss->_last_polling_error_ms = millis();
          errorStatusCB<IMAPSession *, IMAPSession *>(ss, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, true);
        }
      }
      else if (millis() - ss->_last_network_error_ms > 1000)
      {
        ss->_last_network_error_ms = millis();
        errorStatusCB<IMAPSession *, IMAPSession *>(ss, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, true);
      }

      if (ss->_headers.size() > 0)
      {
        if (cPart(ss) && downloadRequest)
          cPart(ss)->download_error = ss->errorReason().c_str();
        else if (cHeader(ss))
          cHeader(ss)->error_msg = ss->errorReason().c_str();
      }

#endif
    }
    else
    {
#if defined(ENABLE_SMTP)
      SMTPSession *ss = (SMTPSession *)sessionPtr;
      errorStatusCB<SMTPSession *, IMAPSession *>(ss, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, false);
#endif
    }

    if (millis() - _lastReconnectMillis > _reconnectTimeout && !sessionPtr->connected())
    {
      if (sessionPtr->_session_cfg->network_connection_handler)
      {
        // dummy
        sessionPtr->client.disconnect();
        sessionPtr->_session_cfg->network_connection_handler();
      }
      else
      {
        if (MailClient.networkAutoReconnect)
          MailClient.resumeNetwork(&(sessionPtr->client));
      }

      _lastReconnectMillis = millis();
    }

    networkStatus = sessionPtr->client.networkReady();
  }

  return networkStatus;
}

template <class T>
void ESP_Mail_Client::sendCB(T sessionPtr, PGM_P info, bool prependCRLF, bool success)
{
  if (sessionPtr)
  {
    sessionPtr->_cbData._info.clear();

    if (prependCRLF)
      appendNewline(sessionPtr->_cbData._info);
    if (strlen_P(info) > 0)
    {
      sessionPtr->_cbData._info += esp_mail_str_33; /* "#### " */
      sessionPtr->_cbData._info += info;
    }
    sessionPtr->_cbData._success = success;
    if (sessionPtr->_statusCallback)
      sessionPtr->_statusCallback(sessionPtr->_cbData);
  }
}

template <class T>
void ESP_Mail_Client::sendErrorCB(T sessionPtr, PGM_P info, bool prependCRLF, bool success)
{
#if !defined(SILENT_MODE)
  MB_String e = esp_mail_str_12; /* "Error, " */
  e += info;
  sendCB<T>(sessionPtr, e.c_str(), prependCRLF, success);
#endif
}

template <class T1, class T2>
void ESP_Mail_Client::errorStatusCB(T1 sessionPtr, T2 sessionPtr2, int error, bool clearLastStatus)
{

  if (sessionPtr)
  {
    sessionPtr->_responseStatus.errorCode = error;

    if (clearLastStatus)
      sessionPtr->_responseStatus.text.clear();

#if !defined(SILENT_MODE)
    if (sessionPtr->_statusCallback && !sessionPtr->_customCmdResCallback)
      sendErrorCB<T1>(sessionPtr, sessionPtr->errorReason().c_str(), false, false);

    if (sessionPtr->_debug && !sessionPtr->_customCmdResCallback)
      esp_mail_debug_print_tag(sessionPtr->errorReason().c_str(), esp_mail_debug_tag_type_error, true);
#endif
  }
  else if (sessionPtr2 && !calDataLen)
    errorStatusCB<T2, T2>(sessionPtr2, nullptr, error, clearLastStatus);
}

template <class T>
bool ESP_Mail_Client::isResponseCB(T sessionPtr)
{
#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
  return sessionPtr->_customCmdResCallback != NULL;
#endif
  return false;
}

template <class T>
void ESP_Mail_Client::printLibInfo(T sessionPtr)
{
#if defined(SESSION_DEBUG_ENABLED)

  if (sessionPtr->_statusCallback != NULL && !isResponseCB<T>(sessionPtr))
    sendCallback(sessionPtr,
                 sessionPtr->_sessionType == esp_mail_session_type_smtp ? esp_mail_cb_str_1 /* "Connecting to SMTP server..." */
                                                                        : esp_mail_cb_str_15 /* "Connecting to IMAP server..." */,
                 false, false);

  if (sessionPtr->_debug && !isResponseCB<T>(sessionPtr))
  {
    MB_String dbMsg = esp_mail_version_str; /* "ESP Mail Client v" */
    dbMsg += ESP_MAIL_VERSION;
    esp_mail_debug_print_tag(dbMsg.c_str(), esp_mail_debug_tag_type_client, true);

#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)
    if (ESP.getPsramSize() == 0 && !isResponseCB<T>(sessionPtr))
      esp_mail_debug_print_tag(esp_mail_error_mem_str_4 /* "PSRAM was enabled but not detected." */, esp_mail_debug_tag_type_warning, true);
#endif
  }

#endif
}

template <class T>
bool ESP_Mail_Client::beginConnection(Session_Config *session_config, T sessionPtr, bool secureMode)
{

  sessionPtr->client.setWiFi(&wifi);
  sessionPtr->client.setSession(session_config);

  if (!reconnect<T>(sessionPtr))
    return false;

#if defined(SESSION_DEBUG_ENABLED)
  if (sessionPtr->_debug && !isResponseCB<T>(sessionPtr))
  {
    esp_mail_debug_print_tag(sessionPtr->_sessionType == esp_mail_session_type_smtp
                                 ? esp_mail_dbg_str_2 /* "connecting to SMTP server" */
                                 : esp_mail_dbg_str_18 /* "connecting to IMAP server" */,
                             esp_mail_debug_tag_type_client, true);

    MB_String dbMsg = esp_mail_dbg_str_19; /* "Host > " */
    dbMsg += session_config->server.host_name;
    esp_mail_debug_print_tag(dbMsg.c_str(), esp_mail_debug_tag_type_client, true);

    dbMsg = esp_mail_dbg_str_20; /* "Port > " */
    dbMsg += session_config->server.port;
    esp_mail_debug_print_tag(dbMsg.c_str(), esp_mail_debug_tag_type_client, true);
  }
#endif

  sessionPtr->client.begin(session_config->server.host_name.c_str(), session_config->server.port);

  sessionPtr->client.ethDNSWorkAround();

  if (!sessionPtr->client.connect(secureMode, session_config->certificate.verify))
  {
    if (sessionPtr->_sessionType == esp_mail_session_type_smtp)
    {
#if defined(ENABLE_SMTP)
      return handleSMTPError((SMTPSession *)sessionPtr, SMTP_STATUS_SERVER_CONNECT_FAILED, false);
#endif
    }
    else
    {
#if defined(ENABLE_IMAP)
      return handleIMAPError((IMAPSession *)sessionPtr, IMAP_STATUS_SERVER_CONNECT_FAILED, false);
#endif
    }
  }

  return true;
}

template <class T>
bool ESP_Mail_Client::prepareTime(Session_Config *session_config, T sessionPtr)
{
  bool timeShouldBeValid = false;

  if (sessionPtr->_sessionType == esp_mail_session_type_smtp)
    timeShouldBeValid = true;
#if !defined(ESP_MAIL_DISABLE_SSL)
  else
    timeShouldBeValid = session_config->certificate.cert_file.length() > 0 || session_config->cert_ptr != 0;
#endif

#if defined(ENABLE_NTP_TIME) && defined(ESP_MAIL_WIFI_IS_AVAILABLE)
  bool ntpEnabled = true;
#else
  bool ntpEnabled = false;
#endif

#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)

  if (session_config->time.ntp_server.length() > 0 || timeShouldBeValid)
  {

    Time.begin(session_config->time.gmt_offset, session_config->time.day_light_offset, session_config->time.ntp_server.c_str());

    if (!Time.timeReady())
    {
      if (sessionPtr->client.type() == esp_mail_client_type_external_gsm_client)
      {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int min = 0;
        int sec = 0;
        float timezone = 0;
        if (sessionPtr->client.gprsGetTime(year, month, day, hour, min, sec, timezone))
          Time.setTimestamp(Time.getTimestamp(year, month, day, hour, min, sec), timezone);
      }
      else if (session_config->time.ntp_server.length())
      {
#if defined(ENABLE_NTP_TIME)
#if !defined(SILENT_MODE)
        if (sessionPtr->_debug && !isResponseCB<T>(sessionPtr))
          esp_mail_debug_print_tag(esp_mail_dbg_str_21 /* "Reading time from NTP server" */, esp_mail_debug_tag_type_client, false);
#endif

        setTime(session_config->time.timezone_env_string.c_str(), session_config->time.timezone_file.c_str(), true, sessionPtr->_debug && !isResponseCB<T>(sessionPtr));
#endif
      }
    }
  }

#endif

#if defined(ESP32)
  if (Time.timeReady() && !timezoneEnvSet)
    getSetTimezoneEnv(session_config->time.timezone_file.c_str(), session_config->time.timezone_env_string.c_str());
#endif

  if (Time.timeReady())
    return true;
  else if (WiFI_CONNECTED && timeShouldBeValid)
  {
    errorStatusCB<T, IMAPSession *>(sessionPtr, nullptr, ntpEnabled && session_config->time.ntp_server.length() ? MAIL_CLIENT_ERROR_NTP_TIME_SYNC_TIMED_OUT : MAIL_CLIENT_ERROR_TIME_WAS_NOT_SET, false);
    return false;
  }

  return true;
}

template <class T>
bool ESP_Mail_Client::sessionReady(T sessionPtr)
{
  // If network connection failure or tcp session closed, close session to clear resources.
  if (!reconnect<T>(sessionPtr) || !connected<T>(sessionPtr))
  {
    closeTCPSession<T>(sessionPtr);

#if defined(ENABLE_IMAP)
    if (sessionPtr->_sessionType == esp_mail_session_type_imap && !connected<T>(sessionPtr))
      errorStatusCB<T, IMAPSession *>(sessionPtr, nullptr, MAIL_CLIENT_ERROR_CONNECTION_CLOSED, false);
#endif

    return false;
  }

  return true;
}

void ESP_Mail_Client::setCert(Session_Config *session_config, const char *ca)
{
  int ptr = reinterpret_cast<int>(ca);
  if (ptr != session_config->cert_ptr)
  {
    session_config->cert_updated = true;
    session_config->cert_ptr = ptr;
  }
}

void ESP_Mail_Client::setSecure(ESP_Mail_TCPClient &client, Session_Config *session_config)
{

  client.setMBFS(mbfs);

  client.setSession(session_config);

#if !defined(ESP_MAIL_DISABLE_SSL)

  if (client.getCertType() == esp_mail_cert_type_undefined || session_config->cert_updated)
  {
    if (session_config->certificate.cert_file.length() > 0 || session_config->certificate.cert_data != NULL || session_config->cert_ptr > 0)
    {
      client.setClockReady(timeStatus);
    }

    if (session_config->certificate.cert_file.length() == 0)
    {
      if (session_config->cert_ptr > 0)
        client.setCACert(reinterpret_cast<const char *>(session_config->cert_ptr));
      else if (session_config->certificate.cert_data != NULL)
        client.setCACert(session_config->certificate.cert_data);
      else
        client.setCACert(NULL);
    }
    else
    {
      if (!client.setCertFile(session_config->certificate.cert_file.c_str(), mbfs_type session_config->certificate.cert_file_storage_type))
        client.setCACert(NULL);
    }
    session_config->cert_updated = false;
  }
#endif
}

void ESP_Mail_Client::appendMultipartContentType(MB_String &buf, esp_mail_multipart_types type, const char *boundary)
{
  bool firstProp = true;
  appendHeaderField(buf, message_headers[esp_mail_message_header_field_content_type].text, multipart_types[type].text, false, false);
  appendHeaderProp(buf, esp_mail_str_90 /* "boundary" */, boundary, firstProp, false, true, true);
  appendNewline(buf);
}

String ESP_Mail_Client::errorReason(bool isSMTP, int errorCode, const char *msg)
{
  MB_String ret;

#if defined(ENABLE_ERROR_STRING) && !defined(SILENT_MODE)

  // If there is server meanningful response (msg) is available, return it instead
  if (strlen(msg) > 0)
    return msg;

  // The error code enums were defined in ESP_Mail_Error.h and MB_FS.h.
  switch (errorCode)
  {

  case TCP_CLIENT_ERROR_CONNECTION_REFUSED:
    ret = esp_mail_error_network_str_7; /* "connection refused" */
    break;
  case TCP_CLIENT_ERROR_SEND_DATA_FAILED:
    ret = esp_mail_error_network_str_8; /* "data sending failed" */
    break;
  case TCP_CLIENT_ERROR_NOT_INITIALIZED:
    ret = esp_mail_error_client_str_1; /* "client and/or necessary callback functions are not yet assigned" */
    break;
  case TCP_CLIENT_ERROR_NOT_CONNECTED:
    ret = esp_mail_error_network_str_4; /* "not connected" */
    break;

  case MAIL_CLIENT_ERROR_CONNECTION_CLOSED:
    ret = esp_mail_error_network_str_6; /* "connection closed" */
    break;
  case MAIL_CLIENT_ERROR_READ_TIMEOUT:
    ret = esp_mail_error_network_str_3; /* "response read timed out" */
    break;
  case MAIL_CLIENT_ERROR_SSL_TLS_STRUCTURE_SETUP:
    ret = esp_mail_error_ssl_str_1; /* "fail to set up the SSL/TLS structure" */
    break;
  case MAIL_CLIENT_ERROR_OUT_OF_MEMORY:
    ret = esp_mail_error_mem_str_8; /* "out of memory" */
    break;
  case MAIL_CLIENT_ERROR_NTP_TIME_SYNC_TIMED_OUT:
    ret = esp_mail_error_network_str_2; /* "NTP server time reading timed out" */
    break;
  case MAIL_CLIENT_ERROR_SESSION_CONFIG_WAS_NOT_ASSIGNED:
    ret = esp_mail_error_session_str_1; /* "the Session_Config object was not assigned" */
    break;
  case MAIL_CLIENT_ERROR_TIME_WAS_NOT_SET:
    ret = esp_mail_error_time_str_1; /* "library or device time was not set" */
    break;
  case MAIL_CLIENT_ERROR_NOT_YET_LOGIN:
    ret = esp_mail_error_auth_str_3; /* "not yet log in" */
    break;
  case MAIL_CLIENT_ERROR_BUFFER_OVERFLOW:
    ret = esp_mail_error_mem_str_9; /* "buffer overflow" */
    break;

#if defined(ENABLE_SMTP)
  case SMTP_STATUS_SERVER_CONNECT_FAILED:
    ret = esp_mail_error_network_str_1; /* "unable to connect to server" */
    break;
  case SMTP_STATUS_SMTP_GREETING_GET_RESPONSE_FAILED:
    ret = esp_mail_error_smtp_str_1; /* "SMTP server greeting failed" */
    break;
  case SMTP_STATUS_SMTP_GREETING_SEND_ACK_FAILED:
    ret = esp_mail_error_smtp_str_1; /* "SMTP server greeting failed" */
    break;
  case SMTP_STATUS_AUTHEN_NOT_SUPPORT:
    ret = esp_mail_error_auth_str_1; /* "the provided SASL authentication mechanism is not support" */
    break;
  case SMTP_STATUS_AUTHEN_FAILED:
    ret = esp_mail_error_smtp_str_2; /* "authentication failed" */
    break;
  case SMTP_STATUS_USER_LOGIN_FAILED:
    ret = esp_mail_error_smtp_str_2; /* "authentication failed" */
    break;
  case SMTP_STATUS_PASSWORD_LOGIN_FAILED:
    ret = esp_mail_error_smtp_str_3; /* "login password is not valid" */
    break;
  case SMTP_STATUS_SEND_HEADER_SENDER_FAILED:
    ret = esp_mail_error_smtp_str_4; /* "send header failed" */
    break;
  case SMTP_STATUS_SEND_HEADER_RECIPIENT_FAILED:
    ret = esp_mail_error_smtp_str_9; /* "set recipient failed" */
    break;
  case SMTP_STATUS_SEND_BODY_FAILED:
    ret = esp_mail_error_smtp_str_5; /* "send body failed" */
    break;
  case SMTP_STATUS_SERVER_OAUTH2_LOGIN_DISABLED:
    ret = esp_mail_error_auth_str_2; /* "OAuth2.0 log in was disabled for this server" */
    break;
  case SMTP_STATUS_NO_VALID_RECIPIENTS_EXISTED:
    ret = esp_mail_error_smtp_str_8; /* "some of the recipient Email address is not valid" */
    break;
  case SMTP_STATUS_NO_VALID_SENDER_EXISTED:
    ret = esp_mail_error_smtp_str_7; /* "sender Email address is not valid" */
    break;
  case SMTP_STATUS_NO_SUPPORTED_AUTH:
    ret = esp_mail_error_auth_str_1; /* "the provided SASL authentication mechanism is not support" */
    break;
  case SMTP_STATUS_SEND_CUSTOM_COMMAND_FAILED:
    ret = esp_mail_error_smtp_str_10; /* "send custom command failed" */
    break;
  case SMTP_STATUS_XOAUTH2_AUTH_FAILED:
    ret = esp_mail_error_smtp_str_11; /* "XOAuth2 authenticate failed" */
    break;
  case SMTP_STATUS_UNDEFINED:
    ret = esp_mail_error_smtp_str_12; /* "undefined error" */
    break;
#endif

#if defined(ENABLE_IMAP)
  case IMAP_STATUS_SERVER_CONNECT_FAILED:
    ret = esp_mail_error_network_str_1; /* "unable to connect to server" */
    break;
  case IMAP_STATUS_IMAP_RESPONSE_FAILED:
    ret = esp_mail_error_imap_str_18; /* "server replied NO or BAD response" */
    break;
  case IMAP_STATUS_AUTHENTICATE_FAILED:
    ret = esp_mail_error_imap_str_19; /* "authenticate failed" */
    break;
  case IMAP_STATUS_BAD_COMMAND:
    ret = esp_mail_error_imap_str_17; /* "could not parse command" */
    break;
  case IMAP_STATUS_STORE_FAILED:
    ret = esp_mail_error_imap_str_20; /* "flags or keywords store failed" */
    break;
  case IMAP_STATUS_SERVER_OAUTH2_LOGIN_DISABLED:
    ret = esp_mail_error_imap_str_21; /* "server is not support OAuth2 login" */
    break;
  case IMAP_STATUS_NO_MESSAGE:
    ret = esp_mail_error_imap_str_5; /* "some of the requested messages no longer exist" */
    break;
  case IMAP_STATUS_ERROR_DOWNLAD_TIMEOUT:
    ret = esp_mail_error_network_str_5; /* "connection timeout" */
    break;
  case IMAP_STATUS_CLOSE_MAILBOX_FAILED:
    ret = esp_mail_error_imap_str_3; /* "fail to close the mailbox" */
    break;
  case IMAP_STATUS_OPEN_MAILBOX_FAILED:
    ret = esp_mail_error_imap_str_4; /* "fail to open the mailbox" */
    break;
  case IMAP_STATUS_LIST_MAILBOXS_FAILED:
    ret = esp_mail_error_imap_str_1; /* "fail to list the mailboxes" */
    break;
  case IMAP_STATUS_CHECK_CAPABILITIES_FAILED:
    ret = esp_mail_error_imap_str_2; /* "fail to check the capabilities" */
    break;
  case IMAP_STATUS_NO_SUPPORTED_AUTH:
    ret = esp_mail_error_auth_str_1; /* "the provided SASL authentication mechanism is not support" */
    break;
  case IMAP_STATUS_NO_MAILBOX_FOLDER_OPENED:
    ret = esp_mail_error_imap_str_5; /* "no mailbox opened" */
    break;
  case IMAP_STATUS_FIRMWARE_UPDATE_INIT_FAILED:
    ret = esp_mail_error_imap_str_6; /* "firmware update initialization failed" */
    break;
  case IMAP_STATUS_FIRMWARE_UPDATE_WRITE_FAILED:
    ret = esp_mail_error_imap_str_7; /* "firmware update write failed" */
    break;
  case IMAP_STATUS_FIRMWARE_UPDATE_END_FAILED:
    ret = esp_mail_error_imap_str_8; /* "firmware update finalize failed" */
    break;
  case IMAP_STATUS_CHANGEDSINC_MODSEQ_TEST_FAILED:
    ret = esp_mail_error_imap_str_14; /* "no message changed since (assigned) modsec" */
    break;
  case IMAP_STATUS_MODSEQ_WAS_NOT_SUPPORTED:
    ret = esp_mail_error_imap_str_15; /* "CONDSTORE was not supported or modsec was not supported for selected mailbox" */
    break;

#endif

#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)

  case MB_FS_ERROR_FILE_IO_ERROR:
    ret = esp_mail_error_mem_str_7; /* "file I/O error" */
    break;
  case MB_FS_ERROR_FILE_NOT_FOUND:
    ret = esp_mail_error_mem_str_6; /* "file not found." */
    break;
  case MB_FS_ERROR_FLASH_STORAGE_IS_NOT_READY:
    ret = esp_mail_error_mem_str_1; /* "flash Storage is not ready." */
    break;
  case MB_FS_ERROR_SD_STORAGE_IS_NOT_READY:
    ret = esp_mail_error_mem_str_2; /* "SD Storage is not ready." */
    break;
  case MB_FS_ERROR_FILE_STILL_OPENED:
    ret = esp_mail_error_mem_str_5; /* "file is still opened." */
    break;

#endif
  default:
    break;
  }

#endif

  return ret.c_str();
}

template <class T>
void ESP_Mail_Client::closeTCPSession(T sessionPtr)
{
  if (!sessionPtr)
    return;

  sessionPtr->client.stop();

  _lastReconnectMillis = millis();

  memset(sessionPtr->_auth_capability, 0, esp_mail_auth_capability_maxType);
  memset(sessionPtr->_feature_capability, 0,
         sessionPtr->_sessionType == esp_mail_session_type_smtp
#if defined(ENABLE_SMTP)
             ? (int)esp_mail_smtp_send_capability_maxType
#else
             ? 0
#endif
             : (int)esp_mail_imap_read_capability_maxType);

  sessionPtr->_authenticated = false;
  sessionPtr->_loginStatus = false;
}

template <class T>
bool ESP_Mail_Client::connected(T sessionPtr)
{
  return sessionPtr->client.connected();
}

size_t ESP_Mail_Client::getReservedLen(size_t len)
{
  return mbfs->getReservedLen(len);
}

template <typename T>
T ESP_Mail_Client::allocMem(size_t size, bool clear)
{
  return reinterpret_cast<T>(mbfs->newP(size, clear));
}

void ESP_Mail_Client::freeMem(void *ptr)
{
  mbfs->delP(ptr);
}

bool ESP_Mail_Client::strcmpP(const char *buf, int ofs, PGM_P beginToken, bool caseSensitive)
{
  if (ofs < 0)
  {
    int p = strposP(buf, beginToken, 0, caseSensitive);
    if (p == -1)
      return false;
    ofs = p;
  }

  char *tmp2 = allocMem<char *>(strlen_P(beginToken) + 1);
  memcpy(tmp2, &buf[ofs], strlen_P(beginToken));
  tmp2[strlen_P(beginToken)] = 0;
  MB_String s = beginToken;
  bool ret = (strcasecmp(s.c_str(), tmp2) == 0);
  // release memory
  freeMem(&tmp2);
  return ret;
}

int ESP_Mail_Client::strposP(const char *buf, PGM_P beginToken, int ofs, bool caseSensitive)
{
  MB_String s = beginToken;
  return strpos(buf, s.c_str(), ofs, caseSensitive);
}

char *ESP_Mail_Client::strP(PGM_P pgm)
{
  size_t len = strlen_P(pgm) + 1;
  char *buf = allocMem<char *>(len);
  strcpy_P(buf, pgm);
  buf[len - 1] = 0;
  return buf;
}

char *ESP_Mail_Client::strP2Lower(PGM_P pgm)
{
  size_t len = strlen_P(pgm) + 1;
  char *buf = allocMem<char *>(len);
  strcpy_P(buf, pgm);

  for (char *p = buf; *p; p++)
  {
    *p = tolower(*p);
  }

  buf[len - 1] = 0;
  return buf;
}

void ESP_Mail_Client::strReplaceP(MB_String &buf, PGM_P name, PGM_P value)
{
  char *n = strP(name);
  char *v = strP(value);

  buf.replaceAll(n, v);

  freeMem(&n);
  freeMem(&v);
}

bool ESP_Mail_Client::isOAuthError(char *buf, int bufLen, int &chunkIdx, int ofs)
{
  bool ret = false;
  if (chunkIdx == 0)
  {
    size_t olen;
    unsigned char *decoded = decodeBase64((const unsigned char *)(buf + ofs), bufLen - ofs, &olen);
    if (decoded)
    {
      ret = strposP((char *)decoded, esp_mail_str_44 /* "{\"status\":" */, 0) > -1;
      freeMem(&decoded);
    }
    chunkIdx++;
  }
  return ret;
}

MB_String ESP_Mail_Client::getXOAUTH2String(const MB_String &email, const MB_String &accessToken)
{
  MB_String raw = esp_mail_str_45; /* "user=" */
  raw += email;
  raw += esp_mail_str_46; /* "\1auth=Bearer " */
  raw += accessToken;
  raw += esp_mail_str_43; /* "\1\1" */
  return encodeBase64Str((const unsigned char *)raw.c_str(), raw.length());
}

unsigned char *ESP_Mail_Client::decodeBase64(const unsigned char *src, size_t len, size_t *out_len)
{
  unsigned char *out, *pos, block[4], tmp;
  size_t i, count, olen;
  int pad = 0;
  size_t extra_pad;

  unsigned char *dtable = allocMem<unsigned char *>(256);

  memset(dtable, 0x80, 256);

  for (i = 0; i < sizeof(b64_index_table) - 1; i++)
    dtable[b64_index_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++)
  {
    if (dtable[src[i]] != 0x80)
      count++;
  }

  if (count == 0)
    goto exit;

  extra_pad = (4 - count % 4) % 4;

  olen = (count + extra_pad) / 4 * 3;

  pos = out = allocMem<unsigned char *>(olen);

  if (out == NULL)
    goto exit;

  count = 0;

  for (i = 0; i < len + extra_pad; i++)
  {
    unsigned char val;

    if (i >= len)
      val = '=';
    else
      val = src[i];
    tmp = dtable[val];
    if (tmp == 0x80)
      continue;

    if (val == '=')
      pad++;
    block[count] = tmp;
    count++;
    if (count == 4)
    {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
      if (pad)
      {
        if (pad == 1)
          pos--;
        else if (pad == 2)
          pos -= 2;
        else
        {
          // release memory
          free(out);
          goto exit;
        }
        break;
      }
    }
  }

  *out_len = pos - out;
  // release memory
  freeMem(&dtable);
  return out;
exit:
  // release memory
  freeMem(&dtable);
  return nullptr;
}

MB_String ESP_Mail_Client::encodeBase64Str(const unsigned char *src, size_t len)
{
  return encodeBase64Str((uint8_t *)src, len);
}

MB_String ESP_Mail_Client::encodeBase64Str(uint8_t *src, size_t len)
{
  MB_String outStr;
  unsigned char *out, *pos;
  const unsigned char *end, *in;
  size_t olen = 4 * ((len + 2) / 3);
  if (olen < len)
    return outStr;

  outStr.resize(olen);
  out = (unsigned char *)&outStr[0];

  end = src + len;
  in = src;
  pos = out;

  while (end - in >= 3)
  {
    *pos++ = b64_index_table[in[0] >> 2];
    *pos++ = b64_index_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = b64_index_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = b64_index_table[in[2] & 0x3f];
    in += 3;
  }

  if (end - in)
  {
    *pos++ = b64_index_table[in[0] >> 2];
    if (end - in == 1)
    {
      *pos++ = b64_index_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    }
    else
    {
      *pos++ = b64_index_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = b64_index_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
  }

  return outStr;
}

#endif

ESP_Mail_Client MailClient = ESP_Mail_Client();

#endif /* ESP_MAIL_CLIENT_CPP */
