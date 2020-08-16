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

#include <core_version.h>

#ifndef wificlientlightbearssl_h
#define wificlientlightbearssl_h
#ifdef USE_TLS
#include <vector>
#include "WiFiClient.h"
#include <t_bearssl.h>

namespace BearSSL {

class WiFiClientSecure_light : public WiFiClient {
  public:
    WiFiClientSecure_light(int recv, int xmit);
    ~WiFiClientSecure_light() override;

    void allocateBuffers(void);

    int connect(IPAddress ip, uint16_t port) override;
    int connect(const char* name, uint16_t port) override;

    uint8_t connected() override;
    size_t write(const uint8_t *buf, size_t size) override;
    size_t write_P(PGM_P buf, size_t size) override;
    size_t write(const char *buf) {
      return write((const uint8_t*)buf, strlen(buf));
    }
    size_t write_P(const char *buf) {
      return write_P((PGM_P)buf, strlen_P(buf));
    }
    size_t write(Stream& stream); // Note this is not virtual
    int read(uint8_t *buf, size_t size) override;
    int available() override;
    int read() override;
    int peek() override;
    size_t peekBytes(uint8_t *buffer, size_t length) override;
    bool flush(unsigned int maxWaitMs);
    bool stop(unsigned int maxWaitMs);
    void flush() override { (void)flush(0); }
    void stop() override { (void)stop(0); }

    // Only check SHA1 fingerprint of public key
    void setPubKeyFingerprint(const uint8_t *f1, const uint8_t *f2,
                              bool f_any = false) {
      _fingerprint1 = f1;
      _fingerprint2 = f2;
      _fingerprint_any = f_any;
    }
    const uint8_t * getRecvPubKeyFingerprint(void) {
      return _recv_fingerprint;
    }

    void setClientECCert(const br_x509_certificate *cert, const br_ec_private_key *sk,
                         unsigned allowed_usages, unsigned cert_issuer_key_type);

    void setTrustAnchor(const br_x509_trust_anchor *ta, size_t ta_size);

    // Sets the requested buffer size for transmit and receive
    void setBufferSizes(int recv, int xmit);

    // Returns whether MFLN negotiation for the above buffer sizes succeeded (after connection)
    int getMFLNStatus() {
      return connected() && br_ssl_engine_get_mfln_negotiated(_eng);
    }

    int32_t getLastError(void) {
      if (_last_error) {
        return _last_error;
      } else {
        return br_ssl_engine_last_error(_eng);
      }
    }
    inline void setLastError(int32_t err) {
      _last_error = err;
    }
    inline void clearLastError(void) {
      _last_error = 0;
    }
    inline size_t getMaxThunkStackUse(void) {
      return _max_thunkstack_use;
    }

  private:
    void _clear();
    bool _ctx_present;
    std::shared_ptr<br_ssl_client_context> _sc;
    inline bool ctx_present() {
      return _ctx_present;
    }
    br_ssl_engine_context *_eng; // &_sc->eng, to allow for client or server contexts
    std::shared_ptr<unsigned char> _iobuf_in;
    std::shared_ptr<unsigned char> _iobuf_out;
    time_t _now;
    int _iobuf_in_size;
    int _iobuf_out_size;
    bool _handshake_done;
    uint64_t _last_error;

    bool _fingerprint_any;           // accept all fingerprints
    const uint8_t *_fingerprint1;          // fingerprint1 to be checked against
    const uint8_t *_fingerprint2;          // fingerprint2 to be checked against
// **** Start patch Castellucci
/*
    uint8_t _recv_fingerprint[20];   // fingerprint received
*/
    uint8_t _recv_fingerprint[21];   // fingerprint received
// **** End patch Castellucci

    unsigned char *_recvapp_buf;
    size_t _recvapp_len;

    bool _clientConnected(); // Is the underlying socket alive?
    bool _connectSSL(const char *hostName); // Do initial SSL handshake
    void _freeSSL();
    int _run_until(unsigned target, bool blocking = true);
    size_t _write(const uint8_t *buf, size_t size, bool pmem);
    bool _wait_for_handshake(); // Sets and return the _handshake_done after connecting

    // Optional client certificate
    const br_x509_certificate *_chain_P;  // PROGMEM certificate
    const br_ec_private_key   *_sk_ec_P;  // PROGMEM private key
    const br_x509_trust_anchor *_ta_P;     // PROGMEM server CA
    size_t _ta_size;
    unsigned _allowed_usages;
    unsigned _cert_issuer_key_type;

    // record the maximum use of ThunkStack for monitoring
    size_t _max_thunkstack_use;

};

#define ERR_OOM             -1000
#define ERR_CANT_RESOLVE_IP -1001
#define ERR_TCP_CONNECT     -1002
// #define ERR_MISSING_EC_KEY  -1003   // deprecated, AWS IoT is not called if the private key is not present
#define ERR_MISSING_CA      -1004

// For reference, BearSSL error codes:
// #define BR_ERR_OK                      0
// #define BR_ERR_BAD_PARAM               1
// #define BR_ERR_BAD_STATE               2
// #define BR_ERR_UNSUPPORTED_VERSION     3
// #define BR_ERR_BAD_VERSION             4
// #define BR_ERR_BAD_LENGTH              5
// #define BR_ERR_TOO_LARGE               6
// #define BR_ERR_BAD_MAC                 7
// #define BR_ERR_NO_RANDOM               8
// #define BR_ERR_UNKNOWN_TYPE            9
// #define BR_ERR_UNEXPECTED             10
// #define BR_ERR_BAD_CCS                12
// #define BR_ERR_BAD_ALERT              13
// #define BR_ERR_BAD_HANDSHAKE          14
// #define BR_ERR_OVERSIZED_ID           15
// #define BR_ERR_BAD_CIPHER_SUITE       16
// #define BR_ERR_BAD_COMPRESSION        17
// #define BR_ERR_BAD_FRAGLEN            18
// #define BR_ERR_BAD_SECRENEG           19
// #define BR_ERR_EXTRA_EXTENSION        20
// #define BR_ERR_BAD_SNI                21
// #define BR_ERR_BAD_HELLO_DONE         22
// #define BR_ERR_LIMIT_EXCEEDED         23
// #define BR_ERR_BAD_FINISHED           24
// #define BR_ERR_RESUME_MISMATCH        25
// #define BR_ERR_INVALID_ALGORITHM      26
// #define BR_ERR_BAD_SIGNATURE          27
// #define BR_ERR_WRONG_KEY_USAGE        28
// #define BR_ERR_NO_CLIENT_AUTH         29
// #define BR_ERR_IO                     31
// #define BR_ERR_RECV_FATAL_ALERT      256
// #define BR_ERR_SEND_FATAL_ALERT      512
// #define BR_ERR_X509_OK                    32
// #define BR_ERR_X509_INVALID_VALUE         33
// #define BR_ERR_X509_TRUNCATED             34
// #define BR_ERR_X509_EMPTY_CHAIN           35
// #define BR_ERR_X509_INNER_TRUNC           36
// #define BR_ERR_X509_BAD_TAG_CLASS         37
// #define BR_ERR_X509_BAD_TAG_VALUE         38
// #define BR_ERR_X509_INDEFINITE_LENGTH     39
// #define BR_ERR_X509_EXTRA_ELEMENT         40
// #define BR_ERR_X509_UNEXPECTED            41
// #define BR_ERR_X509_NOT_CONSTRUCTED       42
// #define BR_ERR_X509_NOT_PRIMITIVE         43
// #define BR_ERR_X509_PARTIAL_BYTE          44
// #define BR_ERR_X509_BAD_BOOLEAN           45
// #define BR_ERR_X509_OVERFLOW              46
// #define BR_ERR_X509_BAD_DN                47
// #define BR_ERR_X509_BAD_TIME              48
// #define BR_ERR_X509_UNSUPPORTED           49
// #define BR_ERR_X509_LIMIT_EXCEEDED        50
// #define BR_ERR_X509_WRONG_KEY_TYPE        51
// #define BR_ERR_X509_BAD_SIGNATURE         52
// #define BR_ERR_X509_TIME_UNKNOWN          53
// #define BR_ERR_X509_EXPIRED               54
// #define BR_ERR_X509_DN_MISMATCH           55
// #define BR_ERR_X509_BAD_SERVER_NAME       56
// #define BR_ERR_X509_CRITICAL_EXTENSION    57
// #define BR_ERR_X509_NOT_CA                58
// #define BR_ERR_X509_FORBIDDEN_KEY_USAGE   59
// #define BR_ERR_X509_WEAK_PUBLIC_KEY       60
// #define BR_ERR_X509_NOT_TRUSTED           62

};

#endif  // USE_TLS
#endif  // wificlientlightbearssl_h
