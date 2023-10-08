/*
  CertStoreBearSSL.cpp - Library for Arduino ESP8266
  Copyright (c) 2018 Earle F. Philhower, III

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
#ifndef BSSL_CERTSTORE_CPP
#define BSSL_CERTSTORE_CPP

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"
#if defined(USE_LIB_SSL_ENGINE)

#include "BSSL_CertStore.h"

#if defined(ESP_SSL_FS_SUPPORTED)

#include <memory>

#if defined(DEBUG_ESP_SSL) && defined(DEBUG_ESP_PORT)
#define DEBUG_BSSL(fmt, ...) DEBUG_ESP_PORT.printf_P((PGM_P)PSTR("BSSL:" fmt), ##__VA_ARGS__)
#else
#define DEBUG_BSSL(...)
#endif

namespace bssl
{

  extern "C"
  {
    // Callback for the x509 decoder
    static void dn_append(void *ctx, const void *buf, size_t len)
    {
      br_sha256_context *sha1 = (br_sha256_context *)ctx;
      br_sha256_update(sha1, buf, len);
    }
  }

  CertStore::~CertStore()
  {
    free(_indexName);
    free(_dataName);
  }

  CertStore::CertInfo CertStore::_preprocessCert(uint32_t length, uint32_t offset, const void *raw)
  {
    CertStore::CertInfo ci;

    // Clear the CertInfo
    memset(&ci, 0, sizeof(ci));

    // Process it using SHA256, same as the hashed_dn
    br_x509_decoder_context_libmail *ctx = new (std::nothrow) br_x509_decoder_context_libmail;
    br_sha256_context *sha256 = new (std::nothrow) br_sha256_context;
    if (!ctx || !sha256)
    {
      if (ctx)
        delete ctx;
      if (sha256)
        delete sha256;
      DEBUG_BSSL("CertStore::_preprocessCert: OOM\n");
      return ci;
    }

    br_sha256_init(sha256);
    br_x509_decoder_init_libmail(ctx, dn_append, sha256);
    br_x509_decoder_push_libmail(ctx, (const void *)raw, length);

    // Copy result to structure
    br_sha256_out(sha256, &ci.sha256);
    ci.length = length;
    ci.offset = offset;

    // Clean up allocated memory
    delete sha256;
    delete ctx;

    // Return result
    return ci;
  }

  // The certs.ar file is a UNIX ar format file, concatenating all the
  // individual certificates into a single blob in a space-efficient way.
  int CertStore::initCertStore(FS &fs, const char *indexFileName, const char *dataFileName)
  {
    int count = 0;
    uint32_t offset = 0;

    _fs = &fs;

    // In case initCertStore called multiple times, don't leak old filenames
    free(_indexName);
    free(_dataName);

    // No strdup_P, so manually do it
    _indexName = (char *)malloc(strlen_P(indexFileName) + 1);
    _dataName = (char *)malloc(strlen_P(dataFileName) + 1);
    if (!_indexName || !_dataName)
    {
      free(_indexName);
      free(_dataName);
      return 0;
    }
    memcpy_P(_indexName, indexFileName, strlen_P(indexFileName) + 1);
    memcpy_P(_dataName, dataFileName, strlen_P(dataFileName) + 1);

    File index = _fs->open(_indexName, FILE_WRITE);
    if (!index)
    {
      return 0;
    }

    File data = _fs->open(_dataName, FILE_READ);
    if (!data)
    {
      index.close();
      return 0;
    }

    uint8_t magic[8];
    if (data.read(magic, sizeof(magic)) != sizeof(magic) ||
        memcmp(magic, "!<arch>\n", sizeof(magic)))
    {
      data.close();
      index.close();
      return 0;
    }
    offset += sizeof(magic);

    while (true)
    {
      uint8_t fileHeader[60];
      // 0..15 = filename in ASCII
      // 48...57 = length in decimal ASCII
      int32_t length;
      if (data.read(fileHeader, sizeof(fileHeader)) != sizeof(fileHeader))
      {
        break;
      }
      offset += sizeof(fileHeader);
      fileHeader[58] = 0;
      if (1 != sscanf((char *)(fileHeader + 48), "%d", (int *)(&length)) || !length)
      {
        break;
      }

      void *raw = malloc(length);
      if (!raw)
      {
        break;
      }
      if ((int)data.read((uint8_t *)raw, length) != length)
      {
        free(raw);
        break;
      }

      // If the filename starts with "//" then this is a rename file, skip it
      if (fileHeader[0] != '/' || fileHeader[1] != '/')
      {
        CertStore::CertInfo ci = _preprocessCert(length, offset, raw);
        if (index.write((uint8_t *)&ci, sizeof(ci)) != (ssize_t)sizeof(ci))
        {
          free(raw);
          break;
        }
        count++;
      }

      offset += length;
      free(raw);
      if (offset & 1)
      {
        uint8_t x;
        data.read(&x, 1);
        offset++;
      }
    }
    data.close();
    index.close();
    return count;
  }

  void CertStore::installCertStore(br_x509_minimal_context *ctx)
  {
    br_x509_minimal_set_dynamic(ctx, (void *)this, findHashedTA, freeHashedTA);
  }

  const br_x509_trust_anchor *CertStore::findHashedTA(void *ctx, void *hashed_dn, size_t len)
  {
    CertStore *cs = static_cast<CertStore *>(ctx);
    CertStore::CertInfo ci;

    if (!cs || len != sizeof(ci.sha256) || !cs->_indexName || !cs->_dataName || !cs->_fs)
    {
      return nullptr;
    }

    File index = cs->_fs->open(cs->_indexName, FILE_READ);
    if (!index)
    {
      return nullptr;
    }

    while (index.read((uint8_t *)&ci, sizeof(ci)) == sizeof(ci))
    {
      if (!memcmp(ci.sha256, hashed_dn, sizeof(ci.sha256)))
      {
        index.close();
        uint8_t *der = (uint8_t *)malloc(ci.length);
        if (!der)
        {
          return nullptr;
        }
        File data = cs->_fs->open(cs->_dataName, FILE_READ);
        if (!data)
        {
          free(der);
          return nullptr;
        }
        if (!data.seek(ci.offset, SeekSet))
        {
          data.close();
          free(der);
          return nullptr;
        }
        if ((int)data.read(der, ci.length) != (int)ci.length)
        {
          free(der);
          return nullptr;
        }
        data.close();
        cs->_x509 = new (std::nothrow) X509List(der, ci.length);
        free(der);
        if (!cs->_x509)
        {
          DEBUG_BSSL("CertStore::findHashedTA: OOM\n");
          return nullptr;
        }

        br_x509_trust_anchor *ta = (br_x509_trust_anchor *)cs->_x509->getTrustAnchors();
        memcpy(ta->dn.data, ci.sha256, sizeof(ci.sha256));
        ta->dn.len = sizeof(ci.sha256);

        return ta;
      }
    }
    index.close();
    return nullptr;
  }

  void CertStore::freeHashedTA(void *ctx, const br_x509_trust_anchor *ta)
  {
    CertStore *cs = static_cast<CertStore *>(ctx);
    (void)ta; // Unused
    delete cs->_x509;
    cs->_x509 = nullptr;
  }

}

#endif

#endif

#endif