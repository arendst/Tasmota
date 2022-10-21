/*
  Copy of BearSSLHelpers.h
  
  WiFiClientBearSSL- SSL client/server for esp8266 using BearSSL libraries
  - Mostly compatible with Arduino WiFi shield library and standard
    WiFiClient/ServerSecure (except for certificate handling).

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

#ifndef ESP_Mail_BearSSLHelpers_H
#define ESP_Mail_BearSSLHelpers_H

#ifdef ESP8266

#include <bearssl/bearssl.h>
#include <Updater.h>

// Internal opaque structures, not needed by user applications
namespace ESP_Mail_brssl
{
  class public_key;
  class private_key;
}; // namespace ESP_Mail_brssl

namespace ESP_Mail
{

  // Holds either a single public RSA or EC key for use when BearSSL wants a pubkey.
  // Copies all associated data so no need to keep input PEM/DER keys.
  // All inputs can be either in RAM or PROGMEM.
  class PublicKey
  {
  public:
    PublicKey();
    PublicKey(const char *pemKey);
    PublicKey(const uint8_t *derKey, size_t derLen);
    ~PublicKey();

    bool parse(const char *pemKey);
    bool parse(const uint8_t *derKey, size_t derLen);

    // Accessors for internal use, not needed by apps
    bool isRSA() const;
    bool isEC() const;
    const br_rsa_public_key *getRSA() const;
    const br_ec_public_key *getEC() const;

    // Disable the copy constructor, we're pointer based
    PublicKey(const PublicKey &that) = delete;

  private:
    ESP_Mail_brssl::public_key *_key;
  };

  // Holds either a single private RSA or EC key for use when BearSSL wants a secretkey.
  // Copies all associated data so no need to keep input PEM/DER keys.
  // All inputs can be either in RAM or PROGMEM.
  class PrivateKey
  {
  public:
    PrivateKey();
    PrivateKey(const char *pemKey);
    PrivateKey(const uint8_t *derKey, size_t derLen);
    ~PrivateKey();

    bool parse(const char *pemKey);
    bool parse(const uint8_t *derKey, size_t derLen);

    // Accessors for internal use, not needed by apps
    bool isRSA() const;
    bool isEC() const;
    const br_rsa_private_key *getRSA() const;
    const br_ec_private_key *getEC() const;

    // Disable the copy constructor, we're pointer based
    PrivateKey(const PrivateKey &that) = delete;

  private:
    ESP_Mail_brssl::private_key *_key;
  };

  // Holds one or more X.509 certificates and associated trust anchors for
  // use whenever BearSSL needs a cert or TA.  May want to have multiple
  // certs for things like a series of trusted CAs (but check the CertStore class
  // for a more memory efficient way).
  // Copies all associated data so no need to keep input PEM/DER certs.
  // All inputs can be either in RAM or PROGMEM.
  class ESP_Mail_X509List
  {
  public:
    ESP_Mail_X509List();
    ESP_Mail_X509List(const char *pemCert);
    ESP_Mail_X509List(const uint8_t *derCert, size_t derLen);
    ~ESP_Mail_X509List();

    bool append(const char *pemCert);
    bool append(const uint8_t *derCert, size_t derLen);

    // Accessors
    size_t getCount() const
    {
      return _count;
    }
    const br_x509_certificate *getX509Certs() const
    {
      return _cert;
    }
    const br_x509_trust_anchor *getTrustAnchors() const
    {
      return _ta;
    }

    // Disable the copy constructor, we're pointer based
    ESP_Mail_X509List(const ESP_Mail_X509List &that) = delete;

  private:
    size_t _count;
    br_x509_certificate *_cert;
    br_x509_trust_anchor *_ta;
  };

  // Opaque object which wraps the BearSSL SSL session to make repeated connections
  // significantly faster.  Completely optional.
  class ESP_Mail_WCS;

  class Session
  {
    friend class ESP_Mail_WCS;

  public:
    Session() { memset(&_session, 0, sizeof(_session)); }

  private:
    br_ssl_session_parameters *getSession() { return &_session; }
    // The actual BearSSL ession information
    br_ssl_session_parameters _session;
  };

  // Updater SHA256 hash and signature verification
  class HashSHA256 : public UpdaterHashClass
  {
  public:
    virtual void begin() override;
    virtual void add(const void *data, uint32_t len) override;
    virtual void end() override;
    virtual int len() override;
    virtual const void *hash() override;
    virtual const unsigned char *oid() override;

  private:
    br_sha256_context _cc;
    unsigned char _sha256[32];
  };

  class SigningVerifier : public UpdaterVerifyClass
  {
  public:
    virtual uint32_t length() override;
    virtual bool verify(UpdaterHashClass *hash, const void *signature, uint32_t signatureLen) override;

  public:
    SigningVerifier(PublicKey *pubKey) { _pubKey = pubKey; }

  private:
    PublicKey *_pubKey;
  };
  /*
// Stack thunked versions of calls
extern "C" {
extern unsigned char *thunk_br_ssl_engine_recvapp_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_br_ssl_engine_recvapp_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_br_ssl_engine_recvrec_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_br_ssl_engine_recvrec_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_br_ssl_engine_sendapp_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_br_ssl_engine_sendapp_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_br_ssl_engine_sendrec_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_br_ssl_engine_sendrec_ack(br_ssl_engine_context *cc, size_t len);
};
*/

}; // namespace ESP_Mail

#endif /* ESP8266 */

#endif /* ESP_Mail_BearSSLHelpers_H */
