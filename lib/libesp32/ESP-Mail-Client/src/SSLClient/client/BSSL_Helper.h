
/*
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
#ifndef BSSL_HELPER_H
#define BSSL_HELPER_H

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"

#if defined(USE_EMBED_SSL_ENGINE)

#if defined(ESP8266)

#ifdef __GNUC__
#if __GNUC__ > 4 || __GNUC__ == 10
#if defined(ARDUINO_ESP8266_GIT_VER)
#if ARDUINO_ESP8266_GIT_VER > 0
#define ESP8266_CORE_SDK_V3_X_X
#endif
#endif
#endif
#endif

#include <Arduino.h>

#include <bearssl/bearssl.h>
#include <vector>
#include <StackThunk.h>
#include <sys/time.h>
#include <IPAddress.h>
#include <Client.h>
#include <FS.h>
#include <time.h>
#include <ctype.h>
#include <vector>
#include <algorithm>

#else

#include <Arduino.h>
#include <bearssl/bearssl.h>
#include <Updater.h>
#include <StackThunk.h>

#endif

#elif defined(USE_LIB_SSL_ENGINE)

#include "../bssl/bearssl.h"

#endif

#if defined(USE_LIB_SSL_ENGINE) || defined(USE_EMBED_SSL_ENGINE)
// Cache for a TLS session with a server
// Use with BearSSL::WiFiClientSecure::setSession
// to accelerate the TLS handshake
class BearSSL_Session
{
    friend class BSSL_SSL_Client;

public:
    BearSSL_Session()
    {
        memset(&_session, 0, sizeof(_session));
    }

    br_ssl_session_parameters *getSession()
    {
        return &_session;
    }

private:
    // The actual BearSSL session information
    br_ssl_session_parameters _session;
};

static const uint16_t suites_P[] PROGMEM = {
#ifndef BEARSSL_SSL_BASIC
    BR_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256,
    BR_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256,
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CCM,
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CCM,
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8,
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    BR_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA,
    BR_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA,
    BR_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,
    BR_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256,
    BR_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256,
    BR_TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384,
    BR_TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384,
    BR_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384,
    BR_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384,
    BR_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA,
    BR_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA,
    BR_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA,
    BR_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA,
    BR_TLS_RSA_WITH_AES_128_GCM_SHA256,
    BR_TLS_RSA_WITH_AES_256_GCM_SHA384,
    BR_TLS_RSA_WITH_AES_128_CCM,
    BR_TLS_RSA_WITH_AES_256_CCM,
    BR_TLS_RSA_WITH_AES_128_CCM_8,
    BR_TLS_RSA_WITH_AES_256_CCM_8,
#endif
    BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
    BR_TLS_RSA_WITH_AES_128_CBC_SHA,
    BR_TLS_RSA_WITH_AES_256_CBC_SHA,
#ifndef BEARSSL_SSL_BASIC
    BR_TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA,
    BR_TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA,
    BR_TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA,
    BR_TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA,
    BR_TLS_RSA_WITH_3DES_EDE_CBC_SHA
#endif
};

// For apps which want to use less secure but faster ciphers, only
static const uint16_t faster_suites_P[] PROGMEM = {
    BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
    BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
    BR_TLS_RSA_WITH_AES_256_CBC_SHA,
    BR_TLS_RSA_WITH_AES_128_CBC_SHA};

// Internal opaque structures, not needed by user applications
namespace key_bssl
{
    class public_key;
    class private_key;
};

namespace bssl
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
        PublicKey(Stream &stream, size_t size);
        PublicKey(Stream &stream) : PublicKey(stream, stream.available()){};
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
        PublicKey &operator=(const PublicKey &that) = delete;

    private:
        key_bssl::public_key *_key;
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
        PrivateKey(Stream &stream, size_t size);
        PrivateKey(Stream &stream) : PrivateKey(stream, stream.available()){};
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
        PrivateKey &operator=(const PrivateKey &that) = delete;

    private:
        key_bssl::private_key *_key;
    };

    // Holds one or more X.509 certificates and associated trust anchors for
    // use whenever BearSSL needs a cert or TA.  May want to have multiple
    // certs for things like a series of trusted CAs (but check the CertStore class
    // for a more memory efficient way).
    // Copies all associated data so no need to keep input PEM/DER certs.
    // All inputs can be either in RAM or PROGMEM.
    class X509List
    {
    public:
        X509List();
        X509List(const char *pemCert);
        X509List(const uint8_t *derCert, size_t derLen);
        X509List(Stream &stream, size_t size);
        X509List(Stream &stream) : X509List(stream, stream.available()){};
        ~X509List();

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
        X509List(const X509List &that) = delete;
        X509List &operator=(const X509List &that) = delete;

    private:
        size_t _count;
        br_x509_certificate *_cert;
        br_x509_trust_anchor *_ta;
    };

    extern "C"
    {

        // Install hashes into the SSL engine
        static void br_ssl_client_install_hashes(br_ssl_engine_context *eng)
        {
            br_ssl_engine_set_hash(eng, br_md5_ID, &br_md5_vtable);
            br_ssl_engine_set_hash(eng, br_sha1_ID, &br_sha1_vtable);
            br_ssl_engine_set_hash(eng, br_sha224_ID, &br_sha224_vtable);
            br_ssl_engine_set_hash(eng, br_sha256_ID, &br_sha256_vtable);
            br_ssl_engine_set_hash(eng, br_sha384_ID, &br_sha384_vtable);
            br_ssl_engine_set_hash(eng, br_sha512_ID, &br_sha512_vtable);
        }

        static void br_x509_minimal_install_hashes(br_x509_minimal_context *x509)
        {
            br_x509_minimal_set_hash(x509, br_md5_ID, &br_md5_vtable);
            br_x509_minimal_set_hash(x509, br_sha1_ID, &br_sha1_vtable);
            br_x509_minimal_set_hash(x509, br_sha224_ID, &br_sha224_vtable);
            br_x509_minimal_set_hash(x509, br_sha256_ID, &br_sha256_vtable);
            br_x509_minimal_set_hash(x509, br_sha384_ID, &br_sha384_vtable);
            br_x509_minimal_set_hash(x509, br_sha512_ID, &br_sha512_vtable);
        }

        // Default initializion for our SSL clients
        static void br_ssl_client_base_init(br_ssl_client_context *cc, const uint16_t *cipher_list, int cipher_cnt)
        {
            uint16_t suites[cipher_cnt];
            memcpy_P(suites, cipher_list, cipher_cnt * sizeof(cipher_list[0]));
            br_ssl_client_zero(cc);
            br_ssl_engine_add_flags(&cc->eng, BR_OPT_NO_RENEGOTIATION); // forbid SSL renegotiation, as we free the Private Key after handshake
            br_ssl_engine_set_versions(&cc->eng, BR_TLS10, BR_TLS12);
            br_ssl_engine_set_suites(&cc->eng, suites, (sizeof suites) / (sizeof suites[0]));
            br_ssl_client_set_default_rsapub(cc);
            br_ssl_engine_set_default_rsavrfy(&cc->eng);
#ifndef BEARSSL_SSL_BASIC
            br_ssl_engine_set_default_ecdsa(&cc->eng);
#endif
            br_ssl_client_install_hashes(&cc->eng);
            br_ssl_engine_set_prf10(&cc->eng, &br_tls10_prf);
            br_ssl_engine_set_prf_sha256(&cc->eng, &br_tls12_sha256_prf);
            br_ssl_engine_set_prf_sha384(&cc->eng, &br_tls12_sha384_prf);
            br_ssl_engine_set_default_aes_cbc(&cc->eng);
#ifndef BEARSSL_SSL_BASIC
            br_ssl_engine_set_default_aes_gcm(&cc->eng);
            br_ssl_engine_set_default_aes_ccm(&cc->eng);
            br_ssl_engine_set_default_des_cbc(&cc->eng);
            br_ssl_engine_set_default_chapol(&cc->eng);
#endif
        }

        // BearSSL doesn't define a true insecure decoder, so we make one ourselves
        // from the simple parser.  It generates the issuer and subject hashes and
        // the SHA1 fingerprint, only one (or none!) of which will be used to
        // "verify" the certificate.

        // Private x509 decoder state
        struct br_x509_insecure_context
        {
            const br_x509_class *vtable;
            bool done_cert;
            const uint8_t *match_fingerprint;
            br_sha1_context sha1_cert;
            bool allow_self_signed;
            br_sha256_context sha256_subject;
            br_sha256_context sha256_issuer;
            br_x509_decoder_context_libmail ctx;
        };

        // Callback for the x509_minimal subject DN
        static void insecure_subject_dn_append(void *ctx, const void *buf, size_t len)
        {
            br_x509_insecure_context *xc = (br_x509_insecure_context *)ctx;
            br_sha256_update(&xc->sha256_subject, buf, len);
        }

        // Callback for the x509_minimal issuer DN
        static void insecure_issuer_dn_append(void *ctx, const void *buf, size_t len)
        {
            br_x509_insecure_context *xc = (br_x509_insecure_context *)ctx;
            br_sha256_update(&xc->sha256_issuer, buf, len);
        }

        // Callback for each certificate present in the chain (but only operates
        // on the first one by design).
        static void insecure_start_cert(const br_x509_class **ctx, uint32_t length)
        {
            (void)ctx;
            (void)length;
        }

        // Callback for each byte stream in the chain.  Only process first cert.
        static void insecure_append(const br_x509_class **ctx, const unsigned char *buf, size_t len)
        {
            br_x509_insecure_context *xc = (br_x509_insecure_context *)ctx;
            // Don't process anything but the first certificate in the chain
            if (!xc->done_cert)
            {
                br_sha1_update(&xc->sha1_cert, buf, len);
                br_x509_decoder_push_libmail(&xc->ctx, (const void *)buf, len);
            }
        }
        // Callback on the first byte of any certificate
        static void insecure_start_chain(const br_x509_class **ctx, const char *server_name)
        {
            br_x509_insecure_context *xc = (br_x509_insecure_context *)ctx;
#if defined(USE_EMBED_SSL_ENGINE)
            br_x509_decoder_init_libmail(&xc->ctx, insecure_subject_dn_append, xc, insecure_issuer_dn_append, xc);
#elif defined(ESP32) || defined(USE_LIB_SSL_ENGINE)
            br_x509_decoder_init_libmail(&xc->ctx, insecure_subject_dn_append, xc);
#endif
            xc->done_cert = false;
            br_sha1_init(&xc->sha1_cert);
            br_sha256_init(&xc->sha256_subject);
            br_sha256_init(&xc->sha256_issuer);
            (void)server_name;
        }

        // Callback on individual cert end.
        static void insecure_end_cert(const br_x509_class **ctx)
        {
            br_x509_insecure_context *xc = (br_x509_insecure_context *)ctx;
            xc->done_cert = true;
        }

        // Callback when complete chain has been parsed.
        // Return 0 on validation success, !0 on validation error
        static unsigned insecure_end_chain(const br_x509_class **ctx)
        {
            const br_x509_insecure_context *xc = (const br_x509_insecure_context *)ctx;
            if (!xc->done_cert)
            {
                // BSSL_BSSL_SSL_Client_DEBUG_PRINTF("insecure_end_chain: No cert seen\n");
                return 1; // error
            }

            // Handle SHA1 fingerprint matching
            char res[20];
            br_sha1_out(&xc->sha1_cert, res);
            if (xc->match_fingerprint && memcmp(res, xc->match_fingerprint, sizeof(res)))
            {

                return BR_ERR_X509_NOT_TRUSTED;
            }

            // Handle self-signer certificate acceptance
            char res_issuer[32];
            char res_subject[32];
            br_sha256_out(&xc->sha256_issuer, res_issuer);
            br_sha256_out(&xc->sha256_subject, res_subject);
            if (xc->allow_self_signed && memcmp(res_subject, res_issuer, sizeof(res_issuer)))
            {
                // BSSL_BSSL_SSL_Client_DEBUG_PRINTF("insecure_end_chain: Didn't get self-signed cert\n");
                return BR_ERR_X509_NOT_TRUSTED;
            }

            // Default (no validation at all) or no errors in prior checks = success.
            return 0;
        }

        // Return the public key from the validator (set by x509_minimal)
        static const br_x509_pkey *insecure_get_pkey(const br_x509_class *const *ctx, unsigned *usages)
        {
            const br_x509_insecure_context *xc = (const br_x509_insecure_context *)ctx;
            if (usages != nullptr)
            {
                *usages = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN; // I said we were insecure!
            }
            return &xc->ctx.pkey;
        }
    }

};
#endif

#endif