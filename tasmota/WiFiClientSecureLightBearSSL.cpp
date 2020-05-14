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

#include "my_user_config.h"
//#ifdef USE_MQTT_TLS
#if defined ESP8266 && (defined(USE_MQTT_TLS) || defined (USE_SENDMAIL))

//#define DEBUG_TLS

#define LWIP_INTERNAL

#include <list>
#include <errno.h>
#include <algorithm>

extern "C" {
#include "osapi.h"
#include "ets_sys.h"
}
#include "debug.h"
#include "WiFiClientSecureLightBearSSL.h"	// needs to be before "ESP8266WiFi.h" to avoid conflict with Arduino headers
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "StackThunk_light.h"
#include "lwip/opt.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include <include/ClientContext.h>
#include "c_types.h"

#include <core_version.h>
#ifndef ARDUINO_ESP8266_RELEASE_2_5_2
#undef DEBUG_TLS
#endif

#ifdef DEBUG_TLS
#include "coredecls.h"
#define LOG_HEAP_SIZE(a) _Log_heap_size(a)
void _Log_heap_size(const char *msg) {
	register uint32_t *sp asm("a1");
	int freestack = 4 * (sp - g_pcont->stack);
  Serial.printf("%s %d, Fragmentation=%d, Thunkstack=%d, Free stack=%d, FreeContStack=%d\n",
                msg, ESP.getFreeHeap(), ESP.getHeapFragmentation(), stack_thunk_light_get_max_usage(),
                freestack, ESP.getFreeContStack());
}
#else
#define LOG_HEAP_SIZE(a)
#endif

// Stack thunked versions of calls
// Initially in BearSSLHelpers.h
extern "C" {
extern unsigned char *thunk_light_br_ssl_engine_recvapp_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_light_br_ssl_engine_recvapp_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_light_br_ssl_engine_recvrec_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_light_br_ssl_engine_recvrec_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_light_br_ssl_engine_sendapp_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_light_br_ssl_engine_sendapp_ack(br_ssl_engine_context *cc, size_t len);
extern unsigned char *thunk_light_br_ssl_engine_sendrec_buf( const br_ssl_engine_context *cc, size_t *len);
extern void thunk_light_br_ssl_engine_sendrec_ack(br_ssl_engine_context *cc, size_t len);
};

// Second stack thunked helpers
make_stack_thunk_light(br_ssl_engine_recvapp_ack);
make_stack_thunk_light(br_ssl_engine_recvapp_buf);
make_stack_thunk_light(br_ssl_engine_recvrec_ack);
make_stack_thunk_light(br_ssl_engine_recvrec_buf);
make_stack_thunk_light(br_ssl_engine_sendapp_ack);
make_stack_thunk_light(br_ssl_engine_sendapp_buf);
make_stack_thunk_light(br_ssl_engine_sendrec_ack);
make_stack_thunk_light(br_ssl_engine_sendrec_buf);

// create new version of Thunk function to store on SYS stack
// unless the Thunk was initialized. Thanks to AES128 GCM, we can keep
// symetric processing on the stack
void min_br_ssl_engine_recvapp_ack(br_ssl_engine_context *cc, size_t len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_recvapp_ack(cc, len);
	} else {
		return br_ssl_engine_recvapp_ack(cc, len);
	}
}
unsigned char *min_br_ssl_engine_recvapp_buf(const br_ssl_engine_context *cc, size_t *len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_recvapp_buf(cc, len);
	} else {
		return br_ssl_engine_recvapp_buf(cc, len);
	}
}
void min_br_ssl_engine_recvrec_ack(br_ssl_engine_context *cc, size_t len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_recvrec_ack(cc, len);
	} else {
		return br_ssl_engine_recvrec_ack(cc, len);
	}
}
unsigned char *min_br_ssl_engine_recvrec_buf(const br_ssl_engine_context *cc, size_t *len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_recvrec_buf(cc, len);
	} else {
		return br_ssl_engine_recvrec_buf(cc, len);
	}
}
void min_br_ssl_engine_sendapp_ack(br_ssl_engine_context *cc, size_t len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_sendapp_ack(cc, len);
	} else {
		return br_ssl_engine_sendapp_ack(cc, len);
	}
}
unsigned char *min_br_ssl_engine_sendapp_buf(const br_ssl_engine_context *cc, size_t *len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_sendapp_buf(cc, len);
	} else {
		return br_ssl_engine_sendapp_buf(cc, len);
	}
}
void min_br_ssl_engine_sendrec_ack(br_ssl_engine_context *cc, size_t len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_sendrec_ack(cc, len);
	} else {
		return br_ssl_engine_sendrec_ack(cc, len);
	}
}
unsigned char *min_br_ssl_engine_sendrec_buf(const br_ssl_engine_context *cc, size_t *len) {
	if (stack_thunk_light_get_refcnt()) {
		return thunk_light_br_ssl_engine_sendrec_buf(cc, len);
	} else {
		return br_ssl_engine_sendrec_buf(cc, len);
	}
}

// Use min_ instead of original thunk_
#define br_ssl_engine_recvapp_ack min_br_ssl_engine_recvapp_ack
#define br_ssl_engine_recvapp_buf min_br_ssl_engine_recvapp_buf
#define br_ssl_engine_recvrec_ack min_br_ssl_engine_recvrec_ack
#define br_ssl_engine_recvrec_buf min_br_ssl_engine_recvrec_buf
#define br_ssl_engine_sendapp_ack min_br_ssl_engine_sendapp_ack
#define br_ssl_engine_sendapp_buf min_br_ssl_engine_sendapp_buf
#define br_ssl_engine_sendrec_ack min_br_ssl_engine_sendrec_ack
#define br_ssl_engine_sendrec_buf min_br_ssl_engine_sendrec_buf

//#define DEBUG_ESP_SSL
#ifdef DEBUG_ESP_SSL
#define DEBUG_BSSL(fmt, ...)  DEBUG_ESP_PORT.printf_P((PGM_P)PSTR( "BSSL:" fmt), ## __VA_ARGS__)
//#define DEBUG_BSSL(fmt, ...)  Serial.printf(fmt, ## __VA_ARGS__)
#else
#define DEBUG_BSSL(...)
#endif

namespace BearSSL {

void WiFiClientSecure_light::_clear() {
  // TLS handshake may take more than the 5 second default timeout
  _timeout = 10000;   // 10 seconds max, it should never go over 6 seconds

  _sc = nullptr;
  _ctx_present = false;
  _eng = nullptr;
  _iobuf_in = nullptr;
  _iobuf_out = nullptr;
  _now = 0; // You can override or ensure time() is correct w/configTime
  setBufferSizes(1024, 1024); // reasonable minimum
  _handshake_done = false;
	_last_error = 0;
  _recvapp_buf = nullptr;
  _recvapp_len = 0;
	_fingerprint_any = true;		// by default accept all fingerprints
	_fingerprint1 = nullptr;
	_fingerprint2 = nullptr;
	_chain_P = nullptr;
	_sk_ec_P = nullptr;
	_ta_P = nullptr;
	_max_thunkstack_use = 0;
}

// Constructor
WiFiClientSecure_light::WiFiClientSecure_light(int recv, int xmit) : WiFiClient() {
  _clear();
LOG_HEAP_SIZE("StackThunk before");
  //stack_thunk_light_add_ref();
LOG_HEAP_SIZE("StackThunk after");
  // now finish the setup
  setBufferSizes(recv, xmit); // reasonable minimum
  allocateBuffers();
}

WiFiClientSecure_light::~WiFiClientSecure_light() {
  if (_client) {
    _client->unref();
    _client = nullptr;
  }
  //_cipher_list = nullptr; // std::shared will free if last reference
  _freeSSL();
}

void WiFiClientSecure_light::allocateBuffers(void) {
  // We prefer to allocate all buffers at start, rather than lazy allocation and deallocation
  // in the long run it avoids heap fragmentation and improves stability
	LOG_HEAP_SIZE("allocateBuffers before");
  _sc = std::make_shared<br_ssl_client_context>();
	LOG_HEAP_SIZE("allocateBuffers ClientContext");
  _iobuf_in = std::shared_ptr<unsigned char>(new unsigned char[_iobuf_in_size], std::default_delete<unsigned char[]>());
  _iobuf_out = std::shared_ptr<unsigned char>(new unsigned char[_iobuf_out_size], std::default_delete<unsigned char[]>());
	LOG_HEAP_SIZE("allocateBuffers after");
}

void WiFiClientSecure_light::setClientECCert(const br_x509_certificate *cert, const br_ec_private_key *sk,
																					unsigned allowed_usages, unsigned cert_issuer_key_type) {
	_chain_P = cert;
	_sk_ec_P = sk;
  _allowed_usages = allowed_usages;
  _cert_issuer_key_type = cert_issuer_key_type;
}

void WiFiClientSecure_light::setTrustAnchor(const br_x509_trust_anchor *ta) {
	_ta_P = ta;
}

void WiFiClientSecure_light::setBufferSizes(int recv, int xmit) {
  // Following constants taken from bearssl/src/ssl/ssl_engine.c (not exported unfortunately)
  const int MAX_OUT_OVERHEAD = 85;
  const int MAX_IN_OVERHEAD = 325;

  // The data buffers must be between 512B and 16KB
  recv = std::max(512, std::min(16384, recv));
  xmit = std::max(512, std::min(16384, xmit));

  // Add in overhead for SSL protocol
  recv += MAX_IN_OVERHEAD;
  xmit += MAX_OUT_OVERHEAD;
  _iobuf_in_size = recv;
  _iobuf_out_size = xmit;
}

bool WiFiClientSecure_light::stop(unsigned int maxWaitMs) {
#ifdef ARDUINO_ESP8266_RELEASE_2_4_2
  WiFiClient::stop(); // calls our virtual flush()
  _freeSSL();
	return true;
#else
  bool ret = WiFiClient::stop(maxWaitMs); // calls our virtual flush()
  _freeSSL();
  return ret;
#endif
}

bool WiFiClientSecure_light::flush(unsigned int maxWaitMs) {
  (void) _run_until(BR_SSL_SENDAPP);
#ifdef ARDUINO_ESP8266_RELEASE_2_4_2
  WiFiClient::flush();
#else
  return WiFiClient::flush(maxWaitMs);
#endif
}

int WiFiClientSecure_light::connect(IPAddress ip, uint16_t port) {
	clearLastError();
  if (!WiFiClient::connect(ip, port)) {
		setLastError(ERR_TCP_CONNECT);
    return 0;
  }
  return _connectSSL(nullptr);
}

int WiFiClientSecure_light::connect(const char* name, uint16_t port) {
  IPAddress remote_addr;
	clearLastError();
  if (!WiFi.hostByName(name, remote_addr)) {
    DEBUG_BSSL("connect: Name loopup failure\n");
		setLastError(ERR_CANT_RESOLVE_IP);
    return 0;
  }
  if (!WiFiClient::connect(remote_addr, port)) {
    DEBUG_BSSL("connect: Unable to connect TCP socket\n");
		_last_error = ERR_TCP_CONNECT;
    return 0;
  }
	LOG_HEAP_SIZE("Before calling _connectSSL");
  return _connectSSL(name);
}

void WiFiClientSecure_light::_freeSSL() {
  _ctx_present = false;
  _recvapp_buf = nullptr;
  _recvapp_len = 0;
  // This connection is toast
  _handshake_done = false;
}

bool WiFiClientSecure_light::_clientConnected() {
  return (_client && _client->state() == ESTABLISHED);
}

uint8_t WiFiClientSecure_light::connected() {
  if (available() || (_clientConnected() && _handshake_done)) {
    return true;
  }
  return false;
}

size_t WiFiClientSecure_light::_write(const uint8_t *buf, size_t size, bool pmem) {
  size_t sent_bytes = 0;

  if (!connected() || !size || !_handshake_done) {
    return 0;
  }

  do {
    // Ensure we yield if we need multiple fragments to avoid WDT
    if (sent_bytes) {
      optimistic_yield(1000);
    }

    // Get BearSSL to a state where we can send
    if (_run_until(BR_SSL_SENDAPP) < 0) {
      break;
    }

    if (br_ssl_engine_current_state(_eng) & BR_SSL_SENDAPP) {
      size_t sendapp_len;
      unsigned char *sendapp_buf = br_ssl_engine_sendapp_buf(_eng, &sendapp_len);
      int to_send = size > sendapp_len ? sendapp_len : size;
      if (pmem) {
        memcpy_P(sendapp_buf, buf, to_send);
      } else {
        memcpy(sendapp_buf, buf, to_send);
      }
      br_ssl_engine_sendapp_ack(_eng, to_send);
      br_ssl_engine_flush(_eng, 0);
      flush();
      buf += to_send;
      sent_bytes += to_send;
      size -= to_send;
    } else {
      break;
    }
  } while (size);

	LOG_HEAP_SIZE("_write");
  return sent_bytes;
}

size_t WiFiClientSecure_light::write(const uint8_t *buf, size_t size) {
  return _write(buf, size, false);
}

size_t WiFiClientSecure_light::write_P(PGM_P buf, size_t size) {
  return _write((const uint8_t *)buf, size, true);
}

// We have to manually read and send individual chunks.
size_t WiFiClientSecure_light::write(Stream& stream) {
  size_t totalSent = 0;
  size_t countRead;
  size_t countSent;

  if (!connected() || !_handshake_done) {
    DEBUG_BSSL("write: Connect/handshake not completed yet\n");
    return 0;
  }

  do {
    uint8_t temp[256]; // Temporary chunk size same as ClientContext
    countSent = 0;
    countRead = stream.readBytes(temp, sizeof(temp));
    if (countRead) {
      countSent = _write((const uint8_t*)temp, countRead, true);
      totalSent += countSent;
    }
    yield(); // Feed the WDT
  } while ((countSent == countRead) && (countSent > 0));
  return totalSent;
}

int WiFiClientSecure_light::read(uint8_t *buf, size_t size) {
  if (!ctx_present() || !_handshake_done) {
    return -1;
  }

  int avail = available();
  bool conn = connected();
  if (!avail && conn) {
    return 0;  // We're still connected, but nothing to read
  }
  if (!avail && !conn) {
    DEBUG_BSSL("read: Not connected, none left available\n");
    return -1;
  }

  if (avail) {
    // Take data from the recvapp buffer
    int to_copy = _recvapp_len < size ? _recvapp_len : size;
    memcpy(buf, _recvapp_buf, to_copy);
    br_ssl_engine_recvapp_ack(_eng, to_copy);
    _recvapp_buf = nullptr;
    _recvapp_len = 0;
    return to_copy;
  }

  if (!conn) {
    DEBUG_BSSL("read: Not connected\n");
    return -1;
  }
  return 0; // If we're connected, no error but no read.
}

int WiFiClientSecure_light::read() {
  uint8_t c;
  if (1 == read(&c, 1)) {
    return c;
  }
  DEBUG_BSSL("read: failed\n");
  return -1;
}

int WiFiClientSecure_light::available() {
  if (_recvapp_buf) {
    return _recvapp_len;  // Anything from last call?
  }
  _recvapp_buf = nullptr;
  _recvapp_len = 0;
  if (!ctx_present() || _run_until(BR_SSL_RECVAPP, false) < 0) {
    return 0;
  }
  int st = br_ssl_engine_current_state(_eng);
  if (st == BR_SSL_CLOSED) {
    return 0;  // Nothing leftover, SSL is closed
  }
  if (st & BR_SSL_RECVAPP) {
    _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
    return _recvapp_len;
  }

  return 0;
}

int WiFiClientSecure_light::peek() {
  if (!ctx_present() || !available()) {
    DEBUG_BSSL("peek: Not connected, none left available\n");
    return -1;
  }
  if (_recvapp_buf && _recvapp_len) {
    return _recvapp_buf[0];
  }
  DEBUG_BSSL("peek: No data left\n");
  return -1;
}

size_t WiFiClientSecure_light::peekBytes(uint8_t *buffer, size_t length) {
  size_t to_copy = 0;
  if (!ctx_present()) {
    DEBUG_BSSL("peekBytes: Not connected\n");
    return 0;
  }

  _startMillis = millis();
  while ((available() < (int) length) && ((millis() - _startMillis) < 5000)) {
    yield();
  }

  to_copy = _recvapp_len < length ? _recvapp_len : length;
  memcpy(buffer, _recvapp_buf, to_copy);
  return to_copy;
}

/* --- Copied almost verbatim from BEARSSL SSL_IO.C ---
   Run the engine, until the specified target state is achieved, or
   an error occurs. The target state is SENDAPP, RECVAPP, or the
   combination of both (the combination matches either). When a match is
   achieved, this function returns 0. On error, it returns -1.
*/
int WiFiClientSecure_light::_run_until(unsigned target, bool blocking) {
//LOG_HEAP_SIZE("_run_until 1");
  if (!ctx_present()) {
    DEBUG_BSSL("_run_until: Not connected\n");
    return -1;
  }
  for (int no_work = 0; blocking || no_work < 2;) {
    if (blocking) {
      // Only for blocking operations can we afford to yield()
      optimistic_yield(100);
    }

    int state;
    state = br_ssl_engine_current_state(_eng);
    if (state & BR_SSL_CLOSED) {
      return -1;
    }

    if (!(_client->state() == ESTABLISHED) && !WiFiClient::available()) {
      return (state & target) ? 0 : -1;
    }

    /*
       If there is some record data to send, do it. This takes
       precedence over everything else.
    */
    if (state & BR_SSL_SENDREC) {
      unsigned char *buf;
      size_t len;
      int wlen;

      buf = br_ssl_engine_sendrec_buf(_eng, &len);
      wlen = WiFiClient::write(buf, len);
      if (wlen <= 0) {
        /*
           If we received a close_notify and we
           still send something, then we have our
           own response close_notify to send, and
           the peer is allowed by RFC 5246 not to
           wait for it.
        */
        return -1;
      }
      if (wlen > 0) {
        br_ssl_engine_sendrec_ack(_eng, wlen);
      }
      no_work = 0;
      continue;
    }

    /*
       If we reached our target, then we are finished.
    */
    if (state & target) {
      return 0;
    }
    /*
       If some application data must be read, and we did not
       exit, then this means that we are trying to write data,
       and that's not possible until the application data is
       read. This may happen if using a shared in/out buffer,
       and the underlying protocol is not strictly half-duplex.
       This is unrecoverable here, so we report an error.
    */
    if (state & BR_SSL_RECVAPP) {
      DEBUG_BSSL("_run_until: Fatal protocol state\n");
      return -1;
    }
    /*
       If we reached that point, then either we are trying
       to read data and there is some, or the engine is stuck
       until a new record is obtained.
    */
    if (state & BR_SSL_RECVREC) {
      if (WiFiClient::available()) {
        unsigned char *buf;
        size_t len;
        int rlen;

        buf = br_ssl_engine_recvrec_buf(_eng, &len);
        rlen = WiFiClient::read(buf, len);
        if (rlen < 0) {
          return -1;
        }
        if (rlen > 0) {
          br_ssl_engine_recvrec_ack(_eng, rlen);
        }
        no_work = 0;
        continue;
      }
    }
    /*
       We can reach that point if the target RECVAPP, and
       the state contains SENDAPP only. This may happen with
       a shared in/out buffer. In that case, we must flush
       the buffered data to "make room" for a new incoming
       record.
    */
    br_ssl_engine_flush(_eng, 0);

    no_work++; // We didn't actually advance here
  }
  // We only get here if we ran through the loop without getting anything done
  return -1;
}

bool WiFiClientSecure_light::_wait_for_handshake() {
  _handshake_done = false;
  while (!_handshake_done && _clientConnected()) {
    int ret = _run_until(BR_SSL_SENDAPP);
    if (ret < 0) {
      DEBUG_BSSL("_wait_for_handshake: failed\n");
      break;
    }
    if (br_ssl_engine_current_state(_eng) & BR_SSL_SENDAPP) {
      _handshake_done = true;
    }
    optimistic_yield(1000);
  }
  return _handshake_done;
}

static uint8_t htoi (unsigned char c)
{
  if (c>='0' && c <='9') return c - '0';
  else if (c>='A' && c<='F') return 10 + c - 'A';
  else if (c>='a' && c<='f') return 10 + c - 'a';
  else return 255;
}

extern "C" {

	// see https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c
	void tohex(unsigned char * in, size_t insz, char * out, size_t outsz) {
		unsigned char * pin = in;
		static const char * hex = "0123456789ABCDEF";
		char * pout = out;
		for(; pin < in+insz; pout +=3, pin++){
			pout[0] = hex[(*pin>>4) & 0xF];
			pout[1] = hex[ *pin     & 0xF];
			pout[2] = ':';
			if (pout + 3 - out > outsz){
				/* Better to truncate output string than overflow buffer */
				/* it would be still better to either return a status */
				/* or ensure the target buffer is large enough and it never happen */
				break;
			}
		}
		pout[-1] = 0;
	}


  // BearSSL doesn't define a true insecure decoder, so we make one ourselves
  // from the simple parser.  It generates the issuer and subject hashes and
  // the SHA1 fingerprint, only one (or none!) of which will be used to
  // "verify" the certificate.

  // Private x509 decoder state
  struct br_x509_pubkeyfingerprint_context {
    const br_x509_class *vtable;
    bool done_cert;             // did we parse the first cert already?
		bool fingerprint_all;
		uint8_t *pubkey_recv_fingerprint;
    const uint8_t *fingerprint1;
    const uint8_t *fingerprint2;
    unsigned usages;            // pubkey usage
    br_x509_decoder_context ctx;  // defined in BearSSL
  };

  // Callback on the first byte of any certificate
  static void pubkeyfingerprint_start_chain(const br_x509_class **ctx, const char *server_name) {
    br_x509_pubkeyfingerprint_context *xc = (br_x509_pubkeyfingerprint_context *)ctx;
    // Don't process anything but the first certificate in the chain
    if (!xc->done_cert) {
    	br_x509_decoder_init(&xc->ctx, nullptr, nullptr, nullptr, nullptr);
		}
    (void)server_name;    // ignore server name
  }

  // Callback for each certificate present in the chain (but only operates
  // on the first one by design).
  static void pubkeyfingerprint_start_cert(const br_x509_class **ctx, uint32_t length) {
    (void) ctx;   // do nothing
    (void) length;
  }

  // Callback for each byte stream in the chain.  Only process first cert.
  static void pubkeyfingerprint_append(const br_x509_class **ctx, const unsigned char *buf, size_t len) {
    br_x509_pubkeyfingerprint_context *xc = (br_x509_pubkeyfingerprint_context *)ctx;
    // Don't process anything but the first certificate in the chain
    if (!xc->done_cert) {
      br_x509_decoder_push(&xc->ctx, (const void*)buf, len);
    }
  }

  // Callback on individual cert end.
  static void pubkeyfingerprint_end_cert(const br_x509_class **ctx) {
    br_x509_pubkeyfingerprint_context *xc = (br_x509_pubkeyfingerprint_context *)ctx;
    xc->done_cert = true;   // first cert already processed
  }

  static void pubkeyfingerprint_pubkey_fingerprint(br_sha1_context *shactx, br_rsa_public_key rsakey) {
    br_sha1_init(shactx);
    br_sha1_update(shactx, "ssh-rsa", 7);           // tag
    br_sha1_update(shactx, rsakey.e, rsakey.elen);  // exponent
    br_sha1_update(shactx, rsakey.n, rsakey.nlen);  // modulus
  }

  // Callback when complete chain has been parsed.
  // Return 0 on validation success, !0 on validation error
  static unsigned pubkeyfingerprint_end_chain(const br_x509_class **ctx) {
    br_x509_pubkeyfingerprint_context *xc = (br_x509_pubkeyfingerprint_context *)ctx;

    br_sha1_context sha1_context;
    pubkeyfingerprint_pubkey_fingerprint(&sha1_context, xc->ctx.pkey.key.rsa);
    br_sha1_out(&sha1_context, xc->pubkey_recv_fingerprint); // copy to fingerprint

		if (!xc->fingerprint_all) {
			if (0 == memcmp(xc->fingerprint1, xc->pubkey_recv_fingerprint, 20)) {
				return 0;
			}
			if (0 == memcmp(xc->fingerprint2, xc->pubkey_recv_fingerprint, 20)) {
				return 0;
			}
			return 1;		// no match, error
		} else {
	    // Default (no validation at all) or no errors in prior checks = success.
	    return 0;
		}
  }

  // Return the public key from the validator (set by x509_minimal)
  static const br_x509_pkey *pubkeyfingerprint_get_pkey(const br_x509_class *const *ctx, unsigned *usages) {
     const br_x509_pubkeyfingerprint_context *xc = (const br_x509_pubkeyfingerprint_context *)ctx;

    if (usages != NULL) {
      *usages = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN; // I said we were insecure!
    }
    return &xc->ctx.pkey;
  }

  //  Set up the x509 insecure data structures for BearSSL core to use.
  void br_x509_pubkeyfingerprint_init(br_x509_pubkeyfingerprint_context *ctx,
																			const uint8_t *fingerprint1, const uint8_t *fingerprint2,
																			uint8_t *recv_fingerprint,
																		  bool fingerprint_all) {
    static const br_x509_class br_x509_pubkeyfingerprint_vtable PROGMEM = {
      sizeof(br_x509_pubkeyfingerprint_context),
      pubkeyfingerprint_start_chain,
      pubkeyfingerprint_start_cert,
      pubkeyfingerprint_append,
      pubkeyfingerprint_end_cert,
      pubkeyfingerprint_end_chain,
      pubkeyfingerprint_get_pkey
    };

    memset(ctx, 0, sizeof * ctx);
    ctx->vtable = &br_x509_pubkeyfingerprint_vtable;
    ctx->done_cert = false;
		ctx->fingerprint1 = fingerprint1;
		ctx->fingerprint2 = fingerprint2;
		ctx->pubkey_recv_fingerprint = recv_fingerprint;
		ctx->fingerprint_all = fingerprint_all;
  }

	// We limit to a single cipher to reduce footprint
  // we reference it, don't put in PROGMEM
  static const uint16_t suites[] = {
#if defined(USE_MQTT_AWS_IOT) || defined(USE_MQTT_TLS_FORCE_EC_CIPHER)
		BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
#else
		BR_TLS_RSA_WITH_AES_128_GCM_SHA256
#endif
  };

  // Default initializion for our SSL clients
  static void br_ssl_client_base_init(br_ssl_client_context *cc) {
    br_ssl_client_zero(cc);
    // forbid SSL renegociation, as we free the Private Key after handshake
    br_ssl_engine_add_flags(&cc->eng, BR_OPT_NO_RENEGOTIATION);

    br_ssl_engine_set_versions(&cc->eng, BR_TLS12, BR_TLS12);
    br_ssl_engine_set_suites(&cc->eng, suites, (sizeof suites) / (sizeof suites[0]));
    br_ssl_client_set_default_rsapub(cc);
    br_ssl_engine_set_default_rsavrfy(&cc->eng);

    // install hashes
    br_ssl_engine_set_hash(&cc->eng, br_sha256_ID, &br_sha256_vtable);
    br_ssl_engine_set_prf_sha256(&cc->eng, &br_tls12_sha256_prf);

		// AES CTR/GCM small version, not contstant time (we don't really care here as there is no TPM anyways)
		br_ssl_engine_set_gcm(&cc->eng, &br_sslrec_in_gcm_vtable, &br_sslrec_out_gcm_vtable);
		br_ssl_engine_set_aes_ctr(&cc->eng, &br_aes_small_ctr_vtable);
		br_ssl_engine_set_ghash(&cc->eng, &br_ghash_ctmul32);

#if defined(USE_MQTT_AWS_IOT) || defined(USE_MQTT_TLS_FORCE_EC_CIPHER)
		// we support only P256 EC curve for AWS IoT, no EC curve for Letsencrypt unless forced
		br_ssl_engine_set_ec(&cc->eng, &br_ec_p256_m15);
#endif
  }
}

// Called by connect() to do the actual SSL setup and handshake.
// Returns if the SSL handshake succeeded.
bool WiFiClientSecure_light::_connectSSL(const char* hostName) {
#ifdef USE_MQTT_AWS_IOT
	if ((!_chain_P) || (!_sk_ec_P)) {
		setLastError(ERR_MISSING_EC_KEY);
		return false;
	}
#endif

	// Validation context, either full CA validation or checking only fingerprints
#ifdef USE_MQTT_TLS_CA_CERT
	br_x509_minimal_context *x509_minimal;
#else
  br_x509_pubkeyfingerprint_context *x509_insecure;
#endif

	LOG_HEAP_SIZE("_connectSSL.start");

	do {		// used to exit on Out of Memory error and keep all cleanup code at the same place
		// ============================================================
		// allocate Thunk stack, move to alternate stack and initialize
	  stack_thunk_light_add_ref();
		LOG_HEAP_SIZE("Thunk allocated");
		DEBUG_BSSL("_connectSSL: start connection\n");
	  _freeSSL();
		clearLastError();
		if (!stack_thunk_light_get_stack_bot()) break;

	  _ctx_present = true;
	  _eng = &_sc->eng; // Allocation/deallocation taken care of by the _sc shared_ptr

	  br_ssl_client_base_init(_sc.get());

		// ============================================================
		// Allocatte and initialize Decoder Context
		LOG_HEAP_SIZE("_connectSSL before DecoderContext allocation");
	  // Only failure possible in the installation is OOM
	#ifdef USE_MQTT_TLS_CA_CERT
		x509_minimal = (br_x509_minimal_context*) malloc(sizeof(br_x509_minimal_context));
		if (!x509_minimal) break;
		br_x509_minimal_init(x509_minimal, &br_sha256_vtable, _ta_P, 1);
		br_x509_minimal_set_rsa(x509_minimal, br_ssl_engine_get_rsavrfy(_eng));
		br_x509_minimal_set_hash(x509_minimal, br_sha256_ID, &br_sha256_vtable);
		br_ssl_engine_set_x509(_eng, &x509_minimal->vtable);

	#else
	  x509_insecure = (br_x509_pubkeyfingerprint_context*) malloc(sizeof(br_x509_pubkeyfingerprint_context));
		//x509_insecure = std::unique_ptr<br_x509_pubkeyfingerprint_context>(new br_x509_pubkeyfingerprint_context);
		if (!x509_insecure) break;
	  br_x509_pubkeyfingerprint_init(x509_insecure, _fingerprint1, _fingerprint2, _recv_fingerprint, _fingerprint_any);
		br_ssl_engine_set_x509(_eng, &x509_insecure->vtable);
	#endif
		LOG_HEAP_SIZE("_connectSSL after DecoderContext allocation");

		// ============================================================
		// Set send/receive buffers
	  br_ssl_engine_set_buffers_bidi(_eng, _iobuf_in.get(), _iobuf_in_size, _iobuf_out.get(), _iobuf_out_size);

		// ============================================================
		// allocate Private key if needed, only if USE_MQTT_AWS_IOT
		LOG_HEAP_SIZE("_connectSSL before PrivKey allocation");
	#ifdef USE_MQTT_AWS_IOT
		// ============================================================
		// Set the EC Private Key, only USE_MQTT_AWS_IOT
		// limited to P256 curve
		br_ssl_client_set_single_ec(_sc.get(), _chain_P, 1,
	                              _sk_ec_P, _allowed_usages,
	                              _cert_issuer_key_type, &br_ec_p256_m15, br_ecdsa_sign_asn1_get_default());
	#endif // USE_MQTT_AWS_IOT

		// ============================================================
		// Start TLS connection, ALL
	  if (!br_ssl_client_reset(_sc.get(), hostName, 0)) break;

	  auto ret = _wait_for_handshake();
	#ifdef DEBUG_ESP_SSL
	  if (!ret) {
	    DEBUG_BSSL("Couldn't connect. Error = %d\n", getLastError());
	  } else {
	    DEBUG_BSSL("Connected! MFLNStatus = %d\n", getMFLNStatus());
	  }
	#endif
		LOG_HEAP_SIZE("_connectSSL.end");
		_max_thunkstack_use = stack_thunk_light_get_max_usage();
		stack_thunk_light_del_ref();
	  //stack_thunk_light_repaint();
		LOG_HEAP_SIZE("_connectSSL.end, freeing StackThunk");

	#ifdef USE_MQTT_TLS_CA_CERT
		free(x509_minimal);
	#else
		free(x509_insecure);
	#endif
		LOG_HEAP_SIZE("_connectSSL after release of Priv Key");
	  return ret;
	} while (0);

	// ============================================================
	// if we arrived here, this means we had an OOM error, cleaning up
	setLastError(ERR_OOM);
	DEBUG_BSSL("_connectSSL: Out of memory\n");
	stack_thunk_light_del_ref();
#ifdef USE_MQTT_TLS_CA_CERT
	free(x509_minimal);
#else
	free(x509_insecure);
#endif
	LOG_HEAP_SIZE("_connectSSL clean_on_error");
	return false;
}

};

#include "t_bearssl_tasmota_config.h"

#endif  // USE_MQTT_TLS
