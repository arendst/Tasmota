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
#include <vector>
#include "WiFiClient.h"
#include <bearssl/bearssl.h>

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
    uint8_t _recv_fingerprint[20];   // fingerprint received

    unsigned char *_recvapp_buf;
    size_t _recvapp_len;

    bool _clientConnected(); // Is the underlying socket alive?
    bool _connectSSL(const char *hostName); // Do initial SSL handshake
    void _freeSSL();
    int _run_until(unsigned target, bool blocking = true);
    size_t _write(const uint8_t *buf, size_t size, bool pmem);
    bool _wait_for_handshake(); // Sets and return the _handshake_done after connecting

    // Optional client certificate
    br_x509_certificate _chain;     // local RAM copy
    const br_x509_certificate *_chain_P;  // PROGMEM certificate
    br_ec_private_key   _sk_ec;
    const br_ec_private_key   *_sk_ec_P;  // PROGMEM private key
    unsigned _allowed_usages;
    unsigned _cert_issuer_key_type;

};

#define ERR_OOM             -1000
#define ERR_CANT_RESOLVE_IP -1001
#define ERR_TCP_CONNECT     -1002

};

#endif
