/*
 * HTTP Client for ESP8266 wrapper v1.0.3
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
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

#ifndef ESP_Mail_HTTPClient_CPP
#define ESP_Mail_HTTPClient_CPP

#ifdef ESP8266

#include "ESP_Mail_HTTPClient.h"

ESP_Mail_HTTPClient::ESP_Mail_HTTPClient()
{
}

ESP_Mail_HTTPClient::~ESP_Mail_HTTPClient()
{
  if (_wcs)
  {
    _wcs->stop();
    _wcs.reset(nullptr);
    _wcs.release();
  }

  std::string().swap(_host);
  std::string().swap(_caCertFile);
  _cacert.reset(new char);
  _cacert = nullptr;
}

bool ESP_Mail_HTTPClient::begin(const char *host, uint16_t port)
{
  if (strcmp(_host.c_str(), host) != 0)
    mflnChecked = false;

  _host = host;
  _port = port;

  //probe for fragmentation support at the specified size
  if (!mflnChecked)
  {
    fragmentable = _wcs->probeMaxFragmentLength(_host.c_str(), _port, chunkSize);
    if (fragmentable)
    {
      _bsslRxSize = chunkSize;
      _bsslTxSize = chunkSize;
      _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);
    }
    mflnChecked = true;
  }

  if (!fragmentable)
    _wcs->setBufferSizes(maxRXBufSize / rxBufDivider, maxTXBufSize / txBufDivider);

  return true;
}

bool ESP_Mail_HTTPClient::connected()
{
  if (_wcs)
  {
    if (_secured)
      return _wcs->connected();
    else
      return _wcs->_ns_connected();
  }

  return false;
}

bool ESP_Mail_HTTPClient::send(const char *header)
{
  if (!connected())
    return false;
  if (_secured)
    return (_wcs->write((uint8_t *)header, strlen(header)) == strlen(header));
  else
    return (_wcs->_ns_write((uint8_t *)header, strlen(header)) == strlen(header));
}

int ESP_Mail_HTTPClient::send(const char *header, const char *payload)
{
  size_t size = strlen(payload);
  if (strlen(header) > 0)
  {
    if (!connect(_secured))
    {
      return ESP_MAIL_ERROR_HTTPC_ERROR_CONNECTION_REFUSED;
    }

    if (!send(header))
    {
      return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_HEADER_FAILED;
    }
  }

  if (size > 0)
  {
    if (_secured)
    {
      if (_wcs->write((uint8_t *)&payload[0], size) != size)
        return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
    }
    else
    {
      if (_wcs->_ns_write((uint8_t *)&payload[0], size) != size)
        return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
    }
  }

  return 0;
}

WiFiClient *ESP_Mail_HTTPClient::stream(void)
{
  if (connected())
    return _wcs.get();
  return nullptr;
}

ESP_Mail::ESP_Mail_WCS *ESP_Mail_HTTPClient::_stream(void)
{
  if (connected())
    return _wcs.get();
  return nullptr;
}

size_t ESP_Mail_HTTPClient::_ns_print(const char *buf)
{
  size_t size = strlen(buf);
  return _wcs->_ns_write((uint8_t *)&buf[0], size);
}

size_t ESP_Mail_HTTPClient::_ns_println(const char *buf)
{
  size_t size = strlen(buf);
  size_t wr = _wcs->_ns_write((uint8_t *)&buf[0], size);
  std::string s = "\r\n";
  wr += _wcs->_ns_write((uint8_t *)s.c_str(), s.length());
  return wr;
}

bool ESP_Mail_HTTPClient::connect(bool secured)
{
  _secured = secured;

  if (connected())
  {
    if (_secured)
    {
      while (_wcs->available() > 0)
        _wcs->read();
    }
    else
    {
      while (_wcs->_ns_available() > 0)
        _wcs->_ns_read();
    }
    return true;
  }

  _wcs->setStartTLS(!_secured);
  if (!_wcs->connect(_host.c_str(), _port))
    return false;

  return connected();
}


void ESP_Mail_HTTPClient::setCACert(const char *caCert)
{

#ifndef USING_AXTLS
  _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);
#endif

  if (caCert)
  {
#ifndef USING_AXTLS
    _wcs->setTrustAnchors(new ESP_Mail::ESP_Mail_X509List(caCert));
#else
    _wcs->setCACert_P(caCert, strlen_P(caCert));
#endif
    _certType = 1;
  }
  else
  {
#ifndef USING_AXTLS
    _wcs->setInsecure();
#endif
    _certType = 0;
  }

  _wcs->setNoDelay(true);
}

void ESP_Mail_HTTPClient::setCertFile(const char *caCertFile, esp_mail_file_storage_type storageType, uint8_t sdPin)
{

#ifndef USING_AXTLS
  _sdPin = sdPin;
  _wcs->setBufferSizes(_bsslRxSize, _bsslTxSize);

  if (_clockReady && strlen(caCertFile) > 0)
  {

    fs::File f;
    if (storageType == esp_mail_file_storage_type_flash)
    {
      ESP_MAIL_FLASH_FS.begin();
      if (ESP_MAIL_FLASH_FS.exists(caCertFile))
        f = ESP_MAIL_FLASH_FS.open(caCertFile, "r");
    }
    else if (storageType == esp_mail_file_storage_type_sd)
    {
      ESP_MAIL_SD_FS.begin(_sdPin);
      if (ESP_MAIL_SD_FS.exists(caCertFile))
        f = ESP_MAIL_SD_FS.open(caCertFile, FILE_READ);
    }

    if (f)
    {
      size_t len = f.size();
      uint8_t *der = new uint8_t[len];
      if (f.available())
        f.read(der, len);
      f.close();
      _wcs->setTrustAnchors(new ESP_Mail::ESP_Mail_X509List(der, len));
      delete[] der;
    }
    _certType = 2;
  }
#endif

  _wcs->setNoDelay(true);
}

#endif /* ESP8266 */

#endif /* ESP_Mail_HTTPClient_CPP */