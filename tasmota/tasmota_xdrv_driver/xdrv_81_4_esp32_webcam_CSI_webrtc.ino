/*
  xdrv_81_4_esp32_webcam_CSI_webrtc.ino - WebRTC Support for Tasmota ESP32-P4

  Copyright (C) 2025  Christian Baars and Theo Arends

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

#ifdef ESP32
#ifdef USE_CSI_WEBCAM

#include <WiFiUdp.h>
#include <t_bearssl.h>
#include "rom/crc.h"

/*********************************************************************************************/
// Constants
/*********************************************************************************************/

#define WEBRTC_DTLS_PORT      5005
#define WEBRTC_SRTP_AUTH_TAG  10
#define WEBRTC_SRTP_MASTER_KEY_LEN   16
#define WEBRTC_SRTP_MASTER_SALT_LEN  14
#define WEBRTC_SRTP_MAX_PAYLOAD      1100

#define WEBRTC_SRTP_LABEL_ENC        0x00
#define WEBRTC_SRTP_LABEL_AUTH       0x01
#define WEBRTC_SRTP_LABEL_SALT       0x02

#define DTLS_PROTO_VERSION 0xFEFD  // DTLS 1.2
#define DTLS_CT_CCS           20
#define DTLS_CT_ALERT         21
#define DTLS_CT_HANDSHAKE     22
#define DTLS_CT_APPDATA       23
#define DTLS_REC_HDR          13
#define DTLS_HS_HDR           12

#define DTLS_HT_CLIENT_HELLO  1
#define DTLS_HT_SERVER_HELLO  2
#define DTLS_HT_HELLO_VERIFY  3
#define DTLS_HT_CERTIFICATE   11
#define DTLS_HT_SERVER_KE     12
#define DTLS_HT_SERVER_DONE   14
#define DTLS_HT_CLIENT_KE     16
#define DTLS_HT_FINISHED      20

#define WEBRTC_TLS_CS_ECDHE_ECDSA_AES128_GCM_SHA256 0xC02B
#define WEBRTC_TLS_NAMED_CURVE_P256  0x0017
#define WEBRTC_TLS_EXT_USE_SRTP      0x000E
#define WEBRTC_TLS_EXT_EC_POINT_FMT  0x000B
#define WEBRTC_TLS_EXT_EXTENDED_MASTER_SECRET 0x0017
#define WEBRTC_SRTP_PROFILE_AES128_CM_SHA1_80 0x0001
#define WEBRTC_GCM_TAG_LEN           16
#define WEBRTC_GCM_EXPLICIT_IV_LEN   8
#define DTLS_COOKIE_LEN       32

/*********************************************************************************************/
// Data Structures
/*********************************************************************************************/

enum webrtc_state_t {
  WEBRTC_IDLE = 0,
  WEBRTC_SIG_HAVE_OFFER,
  WEBRTC_SIG_SENT_ANSWER,
  WEBRTC_DTLS_HANDSHAKING,
  WEBRTC_DTLS_CONNECTED,
  WEBRTC_SRTP_READY,
  WEBRTC_STREAMING
};

enum dtls_hs_state_t {
  DTLS_HS_IDLE = 0,
  DTLS_HS_WAIT_CH,
  DTLS_HS_WAIT_CH_COOKIE,
  DTLS_HS_WAIT_CLIENT_FLIGHT,
  DTLS_HS_ESTABLISHED
};

struct srtp_context_t {
  uint8_t master_key[WEBRTC_SRTP_MASTER_KEY_LEN];
  uint8_t master_salt[WEBRTC_SRTP_MASTER_SALT_LEN];
  uint8_t enc_key[16];
  uint8_t salt_key[14];
  uint8_t auth_key[20];
  br_aes_big_ctr_keys aes_ctr;
  br_hmac_key_context hmac_key;
};

struct media_track_t {
  bool active;
  uint8_t payload_type;
  uint32_t ssrc;
  uint16_t seq;
  uint32_t timestamp;
  uint32_t roc;
  uint64_t packet_index;
  srtp_context_t srtp;
};

struct WebRTC_State_t {
  webrtc_state_t state;
  dtls_hs_state_t hs_state;

  char ice_ufrag[10];
  char ice_pwd[34];
  char rem_ufrag[64];
  char rem_pwd[64];
  bool ice_lite;

  uint8_t fingerprint[32];
  char fingerprint_str[96];

  uint8_t client_random[32];
  uint8_t server_random[32];
  br_sha256_context hs_hash;

  uint8_t cookie_secret[32];
  uint8_t expected_cookie[DTLS_COOKIE_LEN];

  uint16_t srv_msg_seq;
  uint16_t cli_msg_seq;

  uint16_t epoch_in;
  uint16_t epoch_out;
  uint64_t seq_out;

  br_ec_private_key cert_sk;
  uint8_t cert_key_ram[32];  // Private key copied from PROGMEM for BearSSL

  uint8_t ecdhe_priv[32];
  br_ec_private_key ecdhe_sk;
  uint8_t ecdhe_pub[65];
  size_t ecdhe_pub_len;

  uint8_t master_secret[48];
  uint8_t client_write_key[16];
  uint8_t server_write_key[16];
  uint8_t client_write_iv[4];
  uint8_t server_write_iv[4];

  br_aes_big_ctr_keys gcm_enc_aes;
  br_gcm_context gcm_enc;
  br_aes_big_ctr_keys gcm_dec_aes;
  br_gcm_context gcm_dec;

  uint8_t last_flight[1400];
  size_t last_flight_len;

  media_track_t video;
  media_track_t audio;
  uint32_t last_audio_time;

  IPAddress remote_ip;
  uint16_t remote_port;
  bool udp_ready;
  bool client_ke_done;
  bool client_ccs_done;
  bool use_srtp_agreed;

  uint32_t session_id;

  SemaphoreHandle_t mutex;
  TaskHandle_t handshake_task;
};

static WebRTC_State_t* WebRTC = NULL;
static WiFiUDP WebRTC_udp;

void WcSRTPKDF(srtp_context_t* ctx, uint8_t label);
void WcSendSrtpPacket(media_track_t* track, const uint8_t* payload, size_t len, bool marker);

/*********************************************************************************************/
// Self-signed EC certificate (P-256) and private key in DER format
/*********************************************************************************************/

static const unsigned char WEBRTC_CERT_DER[] PROGMEM = {
  0x30, 0x82, 0x01, 0x7d, 0x30, 0x82, 0x01, 0x23, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x14, 0x28, 0xf0, 0xc1, 0xb2, 0x32, 0x73, 0xeb, 0x5f, 0x42,
  0xfe, 0x73, 0xef, 0xd7, 0xbb, 0x55, 0x3c, 0xfa, 0x33, 0xf7, 0x7e, 0x30,
  0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30,
  0x14, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x09,
  0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x30, 0x1e, 0x17,
  0x0d, 0x32, 0x36, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x33, 0x31, 0x32,
  0x37, 0x5a, 0x17, 0x0d, 0x33, 0x36, 0x30, 0x32, 0x30, 0x38, 0x31, 0x34,
  0x33, 0x31, 0x32, 0x37, 0x5a, 0x30, 0x14, 0x31, 0x12, 0x30, 0x10, 0x06,
  0x03, 0x55, 0x04, 0x03, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68,
  0x6f, 0x73, 0x74, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48,
  0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03,
  0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x99, 0xf3, 0x76, 0x36, 0x7d, 0xeb,
  0x8d, 0xc2, 0xdd, 0xb8, 0xbb, 0x4a, 0x15, 0xff, 0xf9, 0xe2, 0x2c, 0x2e,
  0x13, 0xfb, 0xba, 0x5e, 0xe9, 0xfd, 0x90, 0xea, 0xae, 0x2d, 0x75, 0x50,
  0x60, 0x5e, 0xe9, 0x5c, 0xb8, 0x6f, 0x61, 0xec, 0xf3, 0x06, 0xaf, 0xb2,
  0xfc, 0x93, 0xe2, 0xe3, 0xe5, 0x61, 0x0a, 0x0a, 0x9b, 0x62, 0x0c, 0x77,
  0xee, 0x7b, 0x75, 0xac, 0xe7, 0x46, 0xe0, 0xf8, 0xc6, 0xf0, 0xa3, 0x53,
  0x30, 0x51, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04,
  0x14, 0xa1, 0x10, 0xf5, 0x8a, 0x18, 0x1a, 0xba, 0x0f, 0x8d, 0x2b, 0x19,
  0x09, 0xd3, 0xf4, 0xcb, 0x95, 0x73, 0x8f, 0xca, 0x8e, 0x30, 0x1f, 0x06,
  0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xa1, 0x10,
  0xf5, 0x8a, 0x18, 0x1a, 0xba, 0x0f, 0x8d, 0x2b, 0x19, 0x09, 0xd3, 0xf4,
  0xcb, 0x95, 0x73, 0x8f, 0xca, 0x8e, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d,
  0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30,
  0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03,
  0x48, 0x00, 0x30, 0x45, 0x02, 0x21, 0x00, 0xa0, 0x14, 0x0a, 0x5e, 0x9a,
  0x69, 0xc9, 0x86, 0x75, 0xa7, 0x1b, 0x0e, 0xef, 0xf6, 0xe4, 0x95, 0x8b,
  0x3d, 0x7d, 0xc6, 0xfe, 0xc0, 0x87, 0xf5, 0xef, 0x2b, 0x8f, 0xe1, 0xa9,
  0x39, 0xb7, 0x19, 0x02, 0x20, 0x57, 0x28, 0xfc, 0x0f, 0x89, 0xb2, 0x63,
  0x9f, 0xf9, 0xe9, 0x27, 0xcf, 0x4c, 0x01, 0xb1, 0x04, 0xf9, 0x26, 0x7b,
  0xae, 0x4c, 0xa1, 0xe2, 0xb0, 0x82, 0x07, 0xfc, 0xa8, 0xfe, 0x03, 0xe4,
  0x9f
};

static const unsigned char WEBRTC_KEY_PRIV[] PROGMEM = {
  0xab, 0x12, 0x37, 0x06, 0x41,
  0x58, 0x47, 0x5b, 0x7f, 0x9c, 0x90, 0x46, 0x6f, 0x27, 0x94, 0xc9, 0x78,
  0x8c, 0x6c, 0x63, 0xf0, 0xb8, 0xad, 0x90, 0xd6, 0xbf, 0xa5, 0x79, 0xb4,
  0xec, 0x1d, 0x72
};

/*********************************************************************************************/
// CRC32 for STUN Fingerprint (RFC 5389) - use ESP ROM function
/*********************************************************************************************/

uint32_t wc_crc32(const uint8_t *data, size_t nbytes) {
  return crc32_le(0, data, nbytes);
}

/*********************************************************************************************/
// Byte helpers (big-endian)
/*********************************************************************************************/

static inline void put_be16(uint8_t* p, uint16_t v) { p[0] = v >> 8; p[1] = v; }
static inline void put_be24(uint8_t* p, uint32_t v) { p[0] = v >> 16; p[1] = v >> 8; p[2] = v; }
static inline void put_be32(uint8_t* p, uint32_t v) { p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v; }
static inline void put_be48(uint8_t* p, uint64_t v) {
  p[0] = v >> 40; p[1] = v >> 32; p[2] = v >> 24; p[3] = v >> 16; p[4] = v >> 8; p[5] = v;
}
static inline uint16_t get_be16(const uint8_t* p) { return (p[0] << 8) | p[1]; }
static inline uint32_t get_be24(const uint8_t* p) { return ((uint32_t)p[0] << 16) | (p[1] << 8) | p[2]; }

/*********************************************************************************************/
// DTLS record layer helpers
/*********************************************************************************************/

static size_t dtls_build_record_header(uint8_t* buf, uint8_t ct, uint16_t epoch, uint64_t seq, uint16_t payload_len) {
  buf[0] = ct;
  put_be16(buf + 1, DTLS_PROTO_VERSION);
  put_be16(buf + 3, epoch);
  put_be48(buf + 5, seq);
  put_be16(buf + 11, payload_len);
  return DTLS_REC_HDR;
}

static size_t dtls_build_hs_header(uint8_t* buf, uint8_t type, uint32_t body_len, uint16_t msg_seq) {
  buf[0] = type;
  put_be24(buf + 1, body_len);
  put_be16(buf + 4, msg_seq);
  put_be24(buf + 6, 0);
  put_be24(buf + 9, body_len);
  return DTLS_HS_HDR;
}

static void dtls_hash_hs(const uint8_t* hs_hdr_and_body, size_t len) {
  if (!WebRTC) return;
  br_sha256_update(&WebRTC->hs_hash, hs_hdr_and_body, len);
}

static void dtls_send_raw(const uint8_t* data, size_t len) {
  if (!WebRTC || WebRTC->remote_port == 0) return;
  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);
  WebRTC_udp.beginPacket(WebRTC->remote_ip, WebRTC->remote_port);
  WebRTC_udp.write(data, len);
  WebRTC_udp.endPacket();
  if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex);
}

/*********************************************************************************************/
// DTLS GCM record encryption / decryption
/*********************************************************************************************/

static bool dtls_encrypt_record(uint8_t* out, size_t* out_len, uint8_t ct,
    const uint8_t* plaintext, size_t pt_len) {
  uint64_t seq = WebRTC->seq_out;
  uint16_t epoch = WebRTC->epoch_out;
  uint8_t nonce[12];
  memcpy(nonce, WebRTC->server_write_iv, 4);
  put_be16(nonce + 4, epoch);
  put_be48(nonce + 6, seq);

  uint8_t explicit_iv[8];
  put_be16(explicit_iv, epoch);
  put_be48(explicit_iv + 2, seq);

  uint8_t aad[13];
  put_be16(aad, epoch);
  put_be48(aad + 2, seq);
  aad[8] = ct;
  put_be16(aad + 9, DTLS_PROTO_VERSION);
  put_be16(aad + 11, (uint16_t)pt_len);

  uint16_t rec_payload_len = WEBRTC_GCM_EXPLICIT_IV_LEN + pt_len + WEBRTC_GCM_TAG_LEN;
  size_t pos = dtls_build_record_header(out, ct, epoch, seq, rec_payload_len);

  memcpy(out + pos, explicit_iv, WEBRTC_GCM_EXPLICIT_IV_LEN);
  pos += WEBRTC_GCM_EXPLICIT_IV_LEN;

  memcpy(out + pos, plaintext, pt_len);

  br_gcm_reset(&WebRTC->gcm_enc, nonce, 12);
  br_gcm_aad_inject(&WebRTC->gcm_enc, aad, 13);
  br_gcm_flip(&WebRTC->gcm_enc);
  br_gcm_run(&WebRTC->gcm_enc, 1, out + pos, pt_len);
  pos += pt_len;

  br_gcm_get_tag(&WebRTC->gcm_enc, out + pos);
  pos += WEBRTC_GCM_TAG_LEN;

  WebRTC->seq_out++;
  *out_len = pos;
  return true;
}

static bool dtls_decrypt_record(const uint8_t* rec, size_t rec_len, uint8_t* pt_out, size_t* pt_len) {
  if (rec_len < DTLS_REC_HDR + WEBRTC_GCM_EXPLICIT_IV_LEN + WEBRTC_GCM_TAG_LEN) return false;

  uint16_t epoch = get_be16(rec + 3);
  uint64_t seq = 0;
  for (int i = 0; i < 6; i++) seq = (seq << 8) | rec[5 + i];
  uint16_t payload_len = get_be16(rec + 11);

  if ((size_t)(DTLS_REC_HDR + payload_len) > rec_len) return false;
  if (payload_len < WEBRTC_GCM_EXPLICIT_IV_LEN + WEBRTC_GCM_TAG_LEN) return false;

  size_t ct_len = payload_len - WEBRTC_GCM_EXPLICIT_IV_LEN - WEBRTC_GCM_TAG_LEN;

  const uint8_t* explicit_iv = rec + DTLS_REC_HDR;
  const uint8_t* ciphertext = explicit_iv + WEBRTC_GCM_EXPLICIT_IV_LEN;
  const uint8_t* tag = ciphertext + ct_len;

  uint8_t nonce[12];
  memcpy(nonce, WebRTC->client_write_iv, 4);
  memcpy(nonce + 4, explicit_iv, 8);

  uint16_t rec_version = get_be16(rec + 1);
  uint8_t aad[13];
  put_be16(aad, epoch);
  put_be48(aad + 2, seq);
  aad[8] = rec[0];
  put_be16(aad + 9, rec_version);
  put_be16(aad + 11, (uint16_t)ct_len);

  memcpy(pt_out, ciphertext, ct_len);

  br_gcm_reset(&WebRTC->gcm_dec, nonce, 12);
  br_gcm_aad_inject(&WebRTC->gcm_dec, aad, 13);
  br_gcm_flip(&WebRTC->gcm_dec);
  br_gcm_run(&WebRTC->gcm_dec, 0, pt_out, ct_len);

  if (!br_gcm_check_tag(&WebRTC->gcm_dec, tag)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: GCM tag verify FAILED"));
    return false;
  }

  *pt_len = ct_len;
  return true;
}

/*********************************************************************************************/
// DTLS Initialization
/*********************************************************************************************/

void WcInitDTLS(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("WebRTC: WcInitDTLS Start"));

  if (WebRTC == NULL) {
    WebRTC = (WebRTC_State_t*)calloc(1, sizeof(WebRTC_State_t));
    if (!WebRTC) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Alloc Failed (%d bytes)"), sizeof(WebRTC_State_t));
      return;
    }
  }

  if (WebRTC->mutex == NULL) WebRTC->mutex = xSemaphoreCreateMutex();
  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);

  if (WebRTC->handshake_task) { vTaskDelete(WebRTC->handshake_task); WebRTC->handshake_task = NULL; }

  // Generate ICE credentials only once per session
  if (WebRTC->ice_ufrag[0] == 0) {
    snprintf(WebRTC->ice_ufrag, sizeof(WebRTC->ice_ufrag), "%08X", esp_random());
    snprintf(WebRTC->ice_pwd, sizeof(WebRTC->ice_pwd), "%08X%08X%08X%08X", esp_random(), esp_random(), esp_random(), esp_random());
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Generated ICE credentials ufrag=%s pwd=%s"), WebRTC->ice_ufrag, WebRTC->ice_pwd);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Reusing ICE credentials ufrag=%s pwd=%s"), WebRTC->ice_ufrag, WebRTC->ice_pwd);
  }
  WebRTC->ice_lite = true;

  // Use PROGMEM certificate - copy to temp buffer for SHA256
  br_sha256_context sha_ctx;
  br_sha256_init(&sha_ctx);
  uint8_t cert_temp[sizeof(WEBRTC_CERT_DER)];
  memcpy_P(cert_temp, WEBRTC_CERT_DER, sizeof(WEBRTC_CERT_DER));
  br_sha256_update(&sha_ctx, cert_temp, sizeof(WEBRTC_CERT_DER));
  br_sha256_out(&sha_ctx, WebRTC->fingerprint);

  char* ptr = WebRTC->fingerprint_str;
  for (int i = 0; i < 32; i++) {
    ptr += sprintf(ptr, "%02X", WebRTC->fingerprint[i]);
    if (i < 31) *ptr++ = ':';
  }
  *ptr = 0;

  // Copy private key from PROGMEM to RAM for BearSSL
  memcpy_P(WebRTC->cert_key_ram, WEBRTC_KEY_PRIV, 32);
  WebRTC->cert_sk.curve = BR_EC_secp256r1;
  WebRTC->cert_sk.x = WebRTC->cert_key_ram;
  WebRTC->cert_sk.xlen = 32;

  esp_fill_random(WebRTC->cookie_secret, sizeof(WebRTC->cookie_secret));

  WebRTC->hs_state = DTLS_HS_WAIT_CH;
  WebRTC->srv_msg_seq = 0;
  WebRTC->cli_msg_seq = 0;
  WebRTC->epoch_in = 0;
  WebRTC->epoch_out = 0;
  WebRTC->seq_out = 0;
  WebRTC->client_ke_done = false;
  WebRTC->client_ccs_done = false;
  WebRTC->use_srtp_agreed = false;
  WebRTC->last_flight_len = 0;

  WebRTC->video.active = false;
  WebRTC->video.payload_type = 96;
  WebRTC->video.ssrc = esp_random();
  WebRTC->video.seq = (uint16_t)esp_random();
  WebRTC->video.timestamp = esp_random();
  WebRTC->video.roc = 0;

  WebRTC->audio.active = false;
#ifdef USE_I2S_OPUS
  WebRTC->audio.payload_type = 111;  // dynamic PT for Opus
#else
  WebRTC->audio.payload_type = 8;    // PCMA
#endif
  WebRTC->audio.ssrc = esp_random();
  WebRTC->audio.seq = (uint16_t)esp_random();
  WebRTC->audio.timestamp = esp_random();
  WebRTC->audio.roc = 0;

  WebRTC->remote_port = 0;
  WebRTC->udp_ready = false;
  WebRTC->state = WEBRTC_IDLE;
  WebRTC->session_id++;

  xTaskCreatePinnedToCore(HandshakeTask, "WcDTLS", 12288, NULL, 5, &WebRTC->handshake_task, 0);

  AddLog(LOG_LEVEL_INFO, PSTR("WebRTC: Init Complete, FP=%s"), WebRTC->fingerprint_str);
  if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex);
}

/*********************************************************************************************/
// STUN Helper: Hex Dump
/*********************************************************************************************/

void wc_hex_dump(const char* label, const uint8_t* data, size_t len) {
  char hex_str[128];
  size_t offset = 0;
  
  for (size_t i = 0; i < len && i < 64; i += 16) {
    offset = 0;
    for (size_t j = 0; j < 16 && (i + j) < len; j++) {
      offset += snprintf(hex_str + offset, sizeof(hex_str) - offset, "%02X ", data[i + j]);
    }
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: %s [%04d]: %s"), label, i, hex_str);
  }
  if (len > 64) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: %s ... (%d more bytes)"), label, len - 64);
  }
}

/*********************************************************************************************/
// STUN Helper: Parse Attributes
/*********************************************************************************************/

bool wc_stun_find_attribute(const uint8_t* msg, int msg_len, uint16_t attr_type, 
                             const uint8_t** attr_value, uint16_t* attr_len) {
  if (msg_len < 20) return false;
  
  uint16_t stun_len = get_be16(msg + 2);
  if (20 + stun_len > msg_len) return false;
  
  size_t pos = 20;
  size_t end = 20 + stun_len;
  
  while (pos + 4 <= end) {
    uint16_t type = get_be16(msg + pos);
    uint16_t len = get_be16(msg + pos + 2);
    
    if (pos + 4 + len > end) break;
    
    if (type == attr_type) {
      *attr_value = msg + pos + 4;
      *attr_len = len;
      return true;
    }
    
    // Move to next attribute (with padding)
    pos += 4 + len;
    while (pos % 4 != 0 && pos < end) pos++;
  }
  
  return false;
}

/*********************************************************************************************/
// STUN Handler (ICE-lite)
/*********************************************************************************************/

void WcHandleSTUN(uint8_t* msg, int len, IPAddress remIP, uint16_t remPort) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: WcHandleSTUN called, len=%d"), len);
  
  // 1. Validate Header
  if (len < 20) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: STUN too short (%d < 20)"), len);
    return;
  }
  
  // Check for Binding Request (0x0001)
  if (msg[0] != 0 || msg[1] != 1) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Not Binding Request [%02X %02X], expected [00 01]"), msg[0], msg[1]);
    return;
  }
  
  // Magic Cookie Check (Fixed 0x2112A442)
  if (msg[4] != 0x21 || msg[5] != 0x12 || msg[6] != 0xA4 || msg[7] != 0x42) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Bad magic cookie [%02X %02X %02X %02X]"), msg[4], msg[5], msg[6], msg[7]);
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Valid STUN Binding Request from %s:%d"), remIP.toString().c_str(), remPort);
  
  // Hex dump the request
  wc_hex_dump("STUN-REQ", msg, len);
  
  // Parse USERNAME from request (for debugging)
  const uint8_t* req_username = NULL;
  uint16_t req_username_len = 0;
  bool has_username = wc_stun_find_attribute(msg, len, 0x0006, &req_username, &req_username_len);
  
  if (has_username && req_username_len > 0 && req_username_len < 128) {
    char username_str[129];
    memcpy(username_str, req_username, req_username_len);
    username_str[req_username_len] = 0;
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Request USERNAME='%s' (len=%d)"), username_str, req_username_len);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Request has no USERNAME attribute"));
  }
  
  // Parse PRIORITY (for debugging)
  const uint8_t* priority_val = NULL;
  uint16_t priority_len = 0;
  if (wc_stun_find_attribute(msg, len, 0x0024, &priority_val, &priority_len) && priority_len == 4) {
    uint32_t priority = get_be16(priority_val) << 16 | get_be16(priority_val + 2);
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Request PRIORITY=%u"), priority);
  }
  
  // Parse ICE-CONTROLLING / ICE-CONTROLLED
  const uint8_t* ice_controlling = NULL;
  uint16_t ice_controlling_len = 0;
  if (wc_stun_find_attribute(msg, len, 0x802A, &ice_controlling, &ice_controlling_len)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Request has ICE-CONTROLLING"));
  }
  const uint8_t* ice_controlled = NULL;
  uint16_t ice_controlled_len = 0;
  if (wc_stun_find_attribute(msg, len, 0x8029, &ice_controlled, &ice_controlled_len)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Request has ICE-CONTROLLED"));
  }
  
  // Latch Remote IP/Port (Critical for ICE-Lite)
  if (WebRTC->remote_port == 0) {
    WebRTC->remote_ip = remIP;
    WebRTC->remote_port = remPort;
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Latching remote peer %s:%d"), remIP.toString().c_str(), remPort);
  } else if (remPort != WebRTC->remote_port || remIP != WebRTC->remote_ip) {
    // ICE restart or new candidate? Update latch.
    WebRTC->remote_ip = remIP;
    WebRTC->remote_port = remPort;
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Updated remote peer %s:%d"), remIP.toString().c_str(), remPort);
  }
  
  // 2. Prepare Response Buffer (NO USERNAME - RFC 5389)
  uint8_t resp[100];
  memset(resp, 0, sizeof(resp));
  
  // Header: Binding Response (0x0101)
  resp[0] = 0x01; resp[1] = 0x01;
  // Copy Transaction ID (12 bytes) from Request
  memcpy(resp + 8, msg + 8, 12);
  
  // Copy Magic Cookie
  memcpy(resp + 4, msg + 4, 4);
  
  int pos = 20; // Start after header
  
  // 3. Add XOR-MAPPED-ADDRESS (Type 0x0020)
  resp[pos++] = 0x00; resp[pos++] = 0x20; // Type
  resp[pos++] = 0x00; resp[pos++] = 0x08; // Length (8 bytes)
  resp[pos++] = 0x00; resp[pos++] = 0x01; // Family (IPv4)
  
  // X-Port: Port ^ (MagicCookie >> 16)
  uint16_t xport = remPort ^ 0x2112;
  resp[pos++] = (xport >> 8) & 0xFF;
  resp[pos++] = xport & 0xFF;
  
  // X-Address: IP ^ MagicCookie
  resp[pos++] = remIP[0] ^ 0x21;
  resp[pos++] = remIP[1] ^ 0x12;
  resp[pos++] = remIP[2] ^ 0xA4;
  resp[pos++] = remIP[3] ^ 0x42;
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: XOR-MAPPED-ADDRESS added (no USERNAME per RFC 5389)"));
  
  // 4. Add MESSAGE-INTEGRITY (Type 0x0008)
  // RFC 5389 §15.4: HMAC-SHA1 over header+attrs BEFORE MI, with length
  // field set as if message ends right after MI attribute.

  int body_before_mi = pos - 20;
  int len_for_hmac = body_before_mi + 24; // include MI attr (4+20) in length
  resp[2] = (len_for_hmac >> 8) & 0xFF;
  resp[3] =  len_for_hmac       & 0xFF;

  int hmac_input_len = pos; // bytes BEFORE MI attribute

  AddLog(LOG_LEVEL_DEBUG,
         PSTR("WebRTC: HMAC input length=%d, using LOCAL password='%s'"),
         hmac_input_len, WebRTC->ice_pwd);

  br_hmac_key_context kc;
  br_hmac_key_init(&kc, &br_sha1_vtable,
                   WebRTC->ice_pwd, strlen(WebRTC->ice_pwd));
  br_hmac_context hc;
  br_hmac_init(&hc, &kc, 0);
  br_hmac_update(&hc, resp, hmac_input_len);

  resp[pos++] = 0x00; resp[pos++] = 0x08;  // MI type
  resp[pos++] = 0x00; resp[pos++] = 0x14;  // MI length = 20
  br_hmac_out(&hc, resp + pos);
  pos += 20;

  // 5. Add FINGERPRINT (Type 0x8028)

  // Total body length now (for final STUN header): len_for_hmac + 8 bytes FP
  int len_total = len_for_hmac + 8;
  resp[2] = (len_total >> 8) & 0xFF;
  resp[3] =  len_total       & 0xFF;

  // Append FINGERPRINT attribute header
  resp[pos++] = 0x80; resp[pos++] = 0x28;
  resp[pos++] = 0x00; resp[pos++] = 0x04;

  // CRC32 over message up to (not including) FINGERPRINT attribute (RFC 5389 §15.5)
  uint32_t crc = wc_crc32(resp, pos - 4);
  crc ^= 0x5354554E; // XOR with mask

  resp[pos++] = (crc >> 24) & 0xFF;
  resp[pos++] = (crc >> 16) & 0xFF;
  resp[pos++] = (crc >>  8) & 0xFF;
  resp[pos++] =  crc        & 0xFF;

  
  // Hex dump the response
  wc_hex_dump("STUN-RESP", resp, pos);
  
  // 6. Send (mutex-protected for thread safety with SRTP sender)
  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);
  WebRTC_udp.beginPacket(remIP, remPort);
  WebRTC_udp.write(resp, pos);
  int sent = WebRTC_udp.endPacket();
  if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex);
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Sent STUN Binding Response (%d bytes) to %s:%d, result=%d"), 
      pos, remIP.toString().c_str(), remPort, sent);
}

/*********************************************************************************************/
// DTLS Cookie
/*********************************************************************************************/

static void dtls_compute_cookie(const uint8_t* client_random, IPAddress ip, uint16_t port, uint8_t* cookie_out) {
  br_hmac_key_context kc;
  br_hmac_key_init(&kc, &br_sha256_vtable, WebRTC->cookie_secret, 32);
  br_hmac_context hc;
  br_hmac_init(&hc, &kc, 32);
  br_hmac_update(&hc, client_random, 32);
  uint32_t ip32 = (uint32_t)ip;
  br_hmac_update(&hc, &ip32, 4);
  br_hmac_update(&hc, &port, 2);
  br_hmac_out(&hc, cookie_out);
}

/*********************************************************************************************/
// DTLS HelloVerifyRequest
/*********************************************************************************************/

static void dtls_send_hello_verify(const uint8_t* client_random, IPAddress ip, uint16_t port) {
  uint8_t cookie[DTLS_COOKIE_LEN];
  dtls_compute_cookie(client_random, ip, port, cookie);
  memcpy(WebRTC->expected_cookie, cookie, DTLS_COOKIE_LEN);

  uint8_t body[3 + DTLS_COOKIE_LEN];
  put_be16(body, DTLS_PROTO_VERSION);
  body[2] = DTLS_COOKIE_LEN;
  memcpy(body + 3, cookie, DTLS_COOKIE_LEN);

  uint8_t pkt[DTLS_REC_HDR + DTLS_HS_HDR + sizeof(body)];
  size_t hs_body_len = sizeof(body);
  size_t hs_total = DTLS_HS_HDR + hs_body_len;

  dtls_build_hs_header(pkt + DTLS_REC_HDR, DTLS_HT_HELLO_VERIFY, hs_body_len, WebRTC->srv_msg_seq++);
  memcpy(pkt + DTLS_REC_HDR + DTLS_HS_HDR, body, hs_body_len);
  dtls_build_record_header(pkt, DTLS_CT_HANDSHAKE, 0, WebRTC->seq_out, hs_total);
  WebRTC->seq_out++;

  memcpy(WebRTC->last_flight, pkt, sizeof(pkt));
  WebRTC->last_flight_len = sizeof(pkt);

  dtls_send_raw(pkt, sizeof(pkt));
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Sent HelloVerifyRequest"));
}

/*********************************************************************************************/
// DTLS Server Flight: ServerHello + Certificate + ServerKeyExchange + ServerHelloDone
/*********************************************************************************************/

static bool dtls_send_server_flight(void) {
  const br_ec_impl* ec = br_ec_get_default();
  if (!ec) return false;

  esp_fill_random(WebRTC->server_random, 32);

  br_hmac_drbg_context rng;
  uint8_t rng_seed[48];
  esp_fill_random(rng_seed, sizeof(rng_seed));
  br_hmac_drbg_init(&rng, &br_sha256_vtable, rng_seed, sizeof(rng_seed));

  size_t klen = br_ec_keygen((const br_prng_class **)&rng, ec, &WebRTC->ecdhe_sk,
      WebRTC->ecdhe_priv, BR_EC_secp256r1);
  if (klen == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: ECDHE keygen failed"));
    return false;
  }
  WebRTC->ecdhe_sk.x = WebRTC->ecdhe_priv;

  WebRTC->ecdhe_pub_len = br_ec_compute_pub(ec, NULL, WebRTC->ecdhe_pub, &WebRTC->ecdhe_sk);
  if (WebRTC->ecdhe_pub_len == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: ECDHE compute pub failed"));
    return false;
  }

  uint8_t flight[1400];
  size_t fpos = 0;

  // --- ServerHello ---
  {
    uint8_t sh_body[256];
    size_t bp = 0;

    put_be16(sh_body + bp, DTLS_PROTO_VERSION); bp += 2;
    memcpy(sh_body + bp, WebRTC->server_random, 32); bp += 32;
    sh_body[bp++] = 0; // session_id_len
    put_be16(sh_body + bp, WEBRTC_TLS_CS_ECDHE_ECDSA_AES128_GCM_SHA256); bp += 2;
    sh_body[bp++] = 0; // compression

    // Extensions
    size_t ext_start = bp;
    bp += 2; // placeholder for extensions length

    // use_srtp
    put_be16(sh_body + bp, WEBRTC_TLS_EXT_USE_SRTP); bp += 2;
    put_be16(sh_body + bp, 5); bp += 2;      // ext data len
    put_be16(sh_body + bp, 2); bp += 2;      // profile list len
    put_be16(sh_body + bp, WEBRTC_SRTP_PROFILE_AES128_CM_SHA1_80); bp += 2;
    sh_body[bp++] = 0; // mki_len

    // ec_point_formats
    put_be16(sh_body + bp, WEBRTC_TLS_EXT_EC_POINT_FMT); bp += 2;
    put_be16(sh_body + bp, 2); bp += 2;
    sh_body[bp++] = 1; // list len
    sh_body[bp++] = 0; // uncompressed
    // extended_master_secret (RFC 7627)
    put_be16(sh_body + bp, WEBRTC_TLS_EXT_EXTENDED_MASTER_SECRET); bp += 2;  // 0x0017
    put_be16(sh_body + bp, 0);                              bp += 2;  // zero-length data

    put_be16(sh_body + ext_start, bp - ext_start - 2);

    size_t rec_payload = DTLS_HS_HDR + bp;
    size_t rec_total = DTLS_REC_HDR + rec_payload;
    if (fpos + rec_total > sizeof(flight)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Server flight overflow at ServerHello"));
      return false;
    }

    uint8_t hs_hdr[DTLS_HS_HDR];
    dtls_build_hs_header(hs_hdr, DTLS_HT_SERVER_HELLO, bp, WebRTC->srv_msg_seq++);

    dtls_build_record_header(flight + fpos, DTLS_CT_HANDSHAKE, 0, WebRTC->seq_out++, rec_payload);
    fpos += DTLS_REC_HDR;
    memcpy(flight + fpos, hs_hdr, DTLS_HS_HDR);
    fpos += DTLS_HS_HDR;
    memcpy(flight + fpos, sh_body, bp);

    dtls_hash_hs(flight + fpos - DTLS_HS_HDR, DTLS_HS_HDR + bp);
    fpos += bp;
  }

  // --- Certificate ---
  {
    uint32_t cert_body_len = 3 + 3 + sizeof(WEBRTC_CERT_DER);
    size_t rec_payload = DTLS_HS_HDR + cert_body_len;
    size_t rec_total = DTLS_REC_HDR + rec_payload;
    if (fpos + rec_total > sizeof(flight)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Server flight overflow at Certificate"));
      return false;
    }

    uint8_t hs_hdr[DTLS_HS_HDR];
    dtls_build_hs_header(hs_hdr, DTLS_HT_CERTIFICATE, cert_body_len, WebRTC->srv_msg_seq++);

    dtls_build_record_header(flight + fpos, DTLS_CT_HANDSHAKE, 0, WebRTC->seq_out++, rec_payload);
    fpos += DTLS_REC_HDR;

    size_t hs_start = fpos;
    memcpy(flight + fpos, hs_hdr, DTLS_HS_HDR); fpos += DTLS_HS_HDR;
    put_be24(flight + fpos, 3 + sizeof(WEBRTC_CERT_DER)); fpos += 3;
    put_be24(flight + fpos, sizeof(WEBRTC_CERT_DER)); fpos += 3;
    memcpy_P(flight + fpos, WEBRTC_CERT_DER, sizeof(WEBRTC_CERT_DER)); fpos += sizeof(WEBRTC_CERT_DER);

    dtls_hash_hs(flight + hs_start, DTLS_HS_HDR + cert_body_len);
  }

  // --- ServerKeyExchange ---
  {
    uint8_t ec_params[4 + 65];
    size_t ep = 0;
    ec_params[ep++] = 3; // named_curve
    put_be16(ec_params + ep, WEBRTC_TLS_NAMED_CURVE_P256); ep += 2;
    ec_params[ep++] = (uint8_t)WebRTC->ecdhe_pub_len;
    memcpy(ec_params + ep, WebRTC->ecdhe_pub, WebRTC->ecdhe_pub_len);
    ep += WebRTC->ecdhe_pub_len;

    br_sha256_context sig_hash;
    br_sha256_init(&sig_hash);
    br_sha256_update(&sig_hash, WebRTC->client_random, 32);
    br_sha256_update(&sig_hash, WebRTC->server_random, 32);
    br_sha256_update(&sig_hash, ec_params, ep);
    uint8_t hash_val[32];
    br_sha256_out(&sig_hash, hash_val);

    uint8_t sig_buf[80];
    br_ecdsa_sign signer = br_ecdsa_sign_asn1_get_default();
    size_t sig_len = signer(ec, &br_sha256_vtable, hash_val, &WebRTC->cert_sk, sig_buf);
    if (sig_len == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: ECDSA sign failed"));
      return false;
    }

    uint32_t ske_body_len = ep + 2 + 2 + sig_len;
    size_t rec_payload = DTLS_HS_HDR + ske_body_len;
    size_t rec_total = DTLS_REC_HDR + rec_payload;
    if (fpos + rec_total > sizeof(flight)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Server flight overflow at ServerKeyExchange"));
      return false;
    }

    uint8_t hs_hdr[DTLS_HS_HDR];
    dtls_build_hs_header(hs_hdr, DTLS_HT_SERVER_KE, ske_body_len, WebRTC->srv_msg_seq++);

    dtls_build_record_header(flight + fpos, DTLS_CT_HANDSHAKE, 0, WebRTC->seq_out++, rec_payload);
    fpos += DTLS_REC_HDR;

    size_t hs_start = fpos;
    memcpy(flight + fpos, hs_hdr, DTLS_HS_HDR); fpos += DTLS_HS_HDR;
    memcpy(flight + fpos, ec_params, ep); fpos += ep;
    flight[fpos++] = 4; // SHA-256
    flight[fpos++] = 3; // ECDSA
    put_be16(flight + fpos, sig_len); fpos += 2;
    memcpy(flight + fpos, sig_buf, sig_len); fpos += sig_len;

    dtls_hash_hs(flight + hs_start, DTLS_HS_HDR + ske_body_len);
  }

  // --- ServerHelloDone ---
  {
    size_t rec_total = DTLS_REC_HDR + DTLS_HS_HDR;
    if (fpos + rec_total > sizeof(flight)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Server flight overflow at ServerHelloDone"));
      return false;
    }

    uint8_t hs_hdr[DTLS_HS_HDR];
    dtls_build_hs_header(hs_hdr, DTLS_HT_SERVER_DONE, 0, WebRTC->srv_msg_seq++);

    dtls_build_record_header(flight + fpos, DTLS_CT_HANDSHAKE, 0, WebRTC->seq_out++, DTLS_HS_HDR);
    fpos += DTLS_REC_HDR;
    memcpy(flight + fpos, hs_hdr, DTLS_HS_HDR);

    dtls_hash_hs(flight + fpos, DTLS_HS_HDR);
    fpos += DTLS_HS_HDR;
  }

  memcpy(WebRTC->last_flight, flight, fpos);
  WebRTC->last_flight_len = fpos;

  dtls_send_raw(flight, fpos);
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Sent server flight (%d bytes, SH+Cert+SKE+SHD)"), fpos);
  return true;
}

/*********************************************************************************************/
// DTLS Key Derivation
/*********************************************************************************************/

static bool dtls_derive_keys(const uint8_t *premaster, size_t pm_len) {
  // 1) Compute session_hash = SHA-256(handshake_messages)
  uint8_t session_hash[32];
  br_sha256_context tmp;
  memcpy(&tmp, &WebRTC->hs_hash, sizeof(tmp));
  br_sha256_out(&tmp, session_hash);

  // 2) EMS master_secret = PRF(premaster, "extended master secret", session_hash)
  br_tls_prf_seed_chunk chunks[1];
  chunks[0].data = session_hash;
  chunks[0].len  = sizeof(session_hash);  // 32

  br_tls12_sha256_prf(WebRTC->master_secret, 48,
                      premaster, pm_len,
                      "extended master secret", 1, chunks);

  // 3) Key expansion: unchanged (server_random || client_random)
  uint8_t seed2[64];
  memcpy(seed2,       WebRTC->server_random, 32);
  memcpy(seed2 + 32, WebRTC->client_random, 32);

  chunks[0].data = seed2;
  chunks[0].len  = sizeof(seed2);  // 64

  uint8_t key_block[40];
  br_tls12_sha256_prf(key_block, sizeof(key_block),
                      WebRTC->master_secret, 48,
                      "key expansion", 1, chunks);

  memcpy(WebRTC->client_write_key, key_block,        16);
  memcpy(WebRTC->server_write_key, key_block + 16,   16);
  memcpy(WebRTC->client_write_iv,  key_block + 32,   4);
  memcpy(WebRTC->server_write_iv,  key_block + 36,   4);

  br_aes_big_ctr_init(&WebRTC->gcm_enc_aes, WebRTC->server_write_key, 16);
  br_gcm_init(&WebRTC->gcm_enc,
              (const br_block_ctr_class **)&WebRTC->gcm_enc_aes,
              br_ghash_ctmul32);

  br_aes_big_ctr_init(&WebRTC->gcm_dec_aes, WebRTC->client_write_key, 16);
  br_gcm_init(&WebRTC->gcm_dec,
              (const br_block_ctr_class **)&WebRTC->gcm_dec_aes,
              br_ghash_ctmul32);

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: TLS key block derived (EMS)"));
  return true;
}

/*********************************************************************************************/
// DTLS Finished verification & sending
/*********************************************************************************************/

static bool dtls_verify_client_finished(const uint8_t* verify_data, size_t vd_len) {
  if (vd_len != 12) return false;

  uint8_t hs_digest[32];
  br_sha256_context tmp;
  memcpy(&tmp, &WebRTC->hs_hash, sizeof(tmp));
  br_sha256_out(&tmp, hs_digest);

  uint8_t expected[12];
  br_tls_prf_seed_chunk chunks[1];
  chunks[0].data = hs_digest;
  chunks[0].len = 32;
  br_tls12_sha256_prf(expected, 12, WebRTC->master_secret, 48,
      "client finished", 1, chunks);

  if (memcmp(verify_data, expected, 12) != 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Client Finished MISMATCH"));
    return false;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Client Finished verified OK"));
  return true;
}

static bool dtls_send_server_ccs_finished(void) {
  uint8_t flight[128];
  size_t fpos = 0;

  // CCS record (last record in epoch 0)
  uint8_t ccs_body = 0x01;
  dtls_build_record_header(flight + fpos, DTLS_CT_CCS, 0, WebRTC->seq_out++, 1);
  fpos += DTLS_REC_HDR;
  flight[fpos++] = ccs_body;

  if (WebRTC->epoch_out == 0) {
    WebRTC->epoch_out = 1;
    WebRTC->seq_out = 0;
  }

  uint8_t hs_digest[32];
  br_sha256_context tmp;
  memcpy(&tmp, &WebRTC->hs_hash, sizeof(tmp));
  br_sha256_out(&tmp, hs_digest);

  uint8_t server_vd[12];
  br_tls_prf_seed_chunk chunks[1];
  chunks[0].data = hs_digest;
  chunks[0].len = 32;
  br_tls12_sha256_prf(server_vd, 12, WebRTC->master_secret, 48,
      "server finished", 1, chunks);

  uint8_t fin_msg[DTLS_HS_HDR + 12];
  dtls_build_hs_header(fin_msg, DTLS_HT_FINISHED, 12, WebRTC->srv_msg_seq++);
  memcpy(fin_msg + DTLS_HS_HDR, server_vd, 12);

  uint8_t enc_buf[128];
  size_t enc_len;
  if (!dtls_encrypt_record(enc_buf, &enc_len, DTLS_CT_HANDSHAKE, fin_msg, DTLS_HS_HDR + 12)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Encrypt server Finished failed"));
    return false;
  }

  memcpy(flight + fpos, enc_buf, enc_len);
  fpos += enc_len;

  memcpy(WebRTC->last_flight, flight, fpos);
  WebRTC->last_flight_len = fpos;

  dtls_send_raw(flight, fpos);
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Sent server CCS + Finished"));
  return true;
}

/*********************************************************************************************/
// DTLS-SRTP Key Export (RFC 5764)
/*********************************************************************************************/

static void dtls_export_srtp_keys(void) {
  uint8_t seed[64];
  memcpy(seed, WebRTC->client_random, 32);
  memcpy(seed + 32, WebRTC->server_random, 32);

  br_tls_prf_seed_chunk chunks[1];
  chunks[0].data = seed;
  chunks[0].len = 64;

  uint8_t ekm[60];
  br_tls12_sha256_prf(ekm, 60, WebRTC->master_secret, 48,
      "EXTRACTOR-dtls_srtp", 1, chunks);

  uint8_t* srv_key  = ekm + 16;
  uint8_t* srv_salt = ekm + 46;

  media_track_t* tracks[2] = {&WebRTC->video, &WebRTC->audio};
  for (int i = 0; i < 2; i++) {
    memcpy(tracks[i]->srtp.master_key, srv_key, WEBRTC_SRTP_MASTER_KEY_LEN);
    memcpy(tracks[i]->srtp.master_salt, srv_salt, WEBRTC_SRTP_MASTER_SALT_LEN);

    WcSRTPKDF(&tracks[i]->srtp, WEBRTC_SRTP_LABEL_ENC);
    WcSRTPKDF(&tracks[i]->srtp, WEBRTC_SRTP_LABEL_SALT);
    WcSRTPKDF(&tracks[i]->srtp, WEBRTC_SRTP_LABEL_AUTH);

    br_aes_big_ctr_init(&tracks[i]->srtp.aes_ctr, tracks[i]->srtp.enc_key, 16);
    br_hmac_key_init(&tracks[i]->srtp.hmac_key, &br_sha1_vtable, tracks[i]->srtp.auth_key, 20);

    tracks[i]->roc = 0;
    tracks[i]->packet_index = 0;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: SRTP keys exported"));
}

/*********************************************************************************************/
// DTLS ClientHello parser
/*********************************************************************************************/

static bool dtls_parse_client_hello(const uint8_t* hs_body, size_t body_len, bool is_retry) {
  if (body_len < 38) return false;

  size_t p = 0;
  // uint16_t cli_version = get_be16(hs_body + p);
  p += 2;
  memcpy(WebRTC->client_random, hs_body + p, 32);
  p += 32;

  uint8_t sid_len = hs_body[p++];
  if (p + sid_len > body_len) return false;
  p += sid_len;

  uint8_t cookie_len = hs_body[p++];
  if (p + cookie_len > body_len) return false;

  if (!is_retry && cookie_len == 0) {
    return true;
  }

  if (is_retry) {
    if (cookie_len != DTLS_COOKIE_LEN) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: retry CH cookie_len=%d != %d"),
            cookie_len, DTLS_COOKIE_LEN);
      return false;
    }

    if (memcmp(hs_body + p, WebRTC->expected_cookie, DTLS_COOKIE_LEN) != 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Cookie mismatch in retry CH"));
      wc_hex_dump("CH-cookie", hs_body + p, cookie_len);
      wc_hex_dump("exp-cookie", WebRTC->expected_cookie, DTLS_COOKIE_LEN);
      return false;
    }

    p += cookie_len;
  }

  WebRTC->use_srtp_agreed = false;

  if (p + 2 > body_len) return true;
  uint16_t cs_len = get_be16(hs_body + p); p += 2;
  if (p + cs_len > body_len) return true;
  p += cs_len;

  if (p + 1 > body_len) return true;
  uint8_t comp_len = hs_body[p++];
  if (p + comp_len > body_len) return true;
  p += comp_len;

  if (p + 2 > body_len) return true;
  uint16_t ext_total = get_be16(hs_body + p); p += 2;
  size_t ext_end = p + ext_total;
  if (ext_end > body_len) ext_end = body_len;

  while (p + 4 <= ext_end) {
    uint16_t ext_type = get_be16(hs_body + p); p += 2;
    uint16_t ext_len = get_be16(hs_body + p); p += 2;
    if (p + ext_len > ext_end) break;

    if (ext_type == WEBRTC_TLS_EXT_USE_SRTP && ext_len >= 4) {
      uint16_t profile_list_len = get_be16(hs_body + p);
      for (size_t i = 0; i + 1 < profile_list_len; i += 2) {
        uint16_t profile = get_be16(hs_body + p + 2 + i);
        if (profile == WEBRTC_SRTP_PROFILE_AES128_CM_SHA1_80) {
          WebRTC->use_srtp_agreed = true;
          break;
        }
      }
    }

    p += ext_len;
  }

  return true;
}

/*********************************************************************************************/
// DTLS Handshake Processing
/*********************************************************************************************/

static void dtls_process_handshake_record(const uint8_t* rec, size_t rec_len) {
  if (rec_len < DTLS_REC_HDR) return;

  uint16_t epoch = get_be16(rec + 3);
  uint16_t payload_len = get_be16(rec + 11);
  const uint8_t* payload = rec + DTLS_REC_HDR;

  if ((size_t)(DTLS_REC_HDR + payload_len) > rec_len) return;

  if (epoch == 0 && WebRTC->epoch_in == 0) {
    if (payload_len < DTLS_HS_HDR) return;

    uint8_t hs_type = payload[0];
    uint32_t hs_length = get_be24(payload + 1);
    // uint16_t hs_msg_seq = get_be16(payload + 4);
    uint32_t frag_off = get_be24(payload + 6);
    uint32_t frag_len = get_be24(payload + 9);

    if (frag_off != 0 || frag_len != hs_length) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Fragmented HS msg, dropping"));
      return;
    }

    if (DTLS_HS_HDR + hs_length > payload_len) return;
    const uint8_t* hs_body = payload + DTLS_HS_HDR;

    switch (WebRTC->hs_state) {
      case DTLS_HS_WAIT_CH:
        if (hs_type == DTLS_HT_CLIENT_HELLO) {
          if (!dtls_parse_client_hello(hs_body, hs_length, false)) break;
          dtls_send_hello_verify(WebRTC->client_random, WebRTC->remote_ip, WebRTC->remote_port);
          WebRTC->hs_state = DTLS_HS_WAIT_CH_COOKIE;
          WebRTC->state = WEBRTC_DTLS_HANDSHAKING;
          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: CH received, sent HVR"));
        }
        break;

      case DTLS_HS_WAIT_CH_COOKIE:
        if (hs_type == DTLS_HT_CLIENT_HELLO) {
          // Quick parse just to read cookie_len
          if (hs_length < 38) break;  // version(2) + random(32) + sid_len(1) + cookie_len(1)

          size_t p = 0;
          p += 2;   // client_version
          p += 32;  // client_random

          if (p >= hs_length) break;
          uint8_t sid_len = hs_body[p++];
          if (p + sid_len > hs_length) break;
          p += sid_len;
          if (p >= hs_length) break;

          uint8_t cookie_len = hs_body[p];

          if (cookie_len == 0) {
            // This is just a retransmission of the original ClientHello
            // while we are already in WAIT_CH_COOKIE. Re-send the *same*
            // HelloVerifyRequest (same msg_seq / seq_out) instead of
            // generating a fresh one.
            AddLog(LOG_LEVEL_DEBUG,
                  PSTR("WebRTC: Retransmit CH without cookie in WAIT_CH_COOKIE, re-sending previous HVR"));
            if (WebRTC->last_flight_len > 0) {
              dtls_send_raw(WebRTC->last_flight, WebRTC->last_flight_len);
            }
            break;
          }

          // Normal retry with cookie: verify cookie via parser
          if (!dtls_parse_client_hello(hs_body, hs_length, true)) {
            // Bad cookie: send a fresh HVR with a new cookie
            AddLog(LOG_LEVEL_ERROR,
                  PSTR("WebRTC: Retry CH with bad cookie, sending new HVR"));
            dtls_send_hello_verify(WebRTC->client_random,
                                  WebRTC->remote_ip, WebRTC->remote_port);
            break;
          }

          // Cookie OK: start transcript and send server flight
          br_sha256_init(&WebRTC->hs_hash);
          dtls_hash_hs(payload, DTLS_HS_HDR + hs_length);
          WebRTC->client_ke_done  = false;
          WebRTC->client_ccs_done = false;

          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Cookie OK, seq_out=%d srv_msg_seq=%d"),
                (int)WebRTC->seq_out, WebRTC->srv_msg_seq);

          if (!dtls_send_server_flight()) {
            AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Failed to send server flight"));
            break;
          }

          WebRTC->hs_state = DTLS_HS_WAIT_CLIENT_FLIGHT;
          WebRTC->state    = WEBRTC_DTLS_HANDSHAKING;
        }
        break;

      case DTLS_HS_WAIT_CLIENT_FLIGHT:
        if (hs_type == DTLS_HT_CLIENT_HELLO) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Retransmit CH in WAIT_CLIENT_FLIGHT, resending server flight (%d bytes)"),
              WebRTC->last_flight_len);
          dtls_send_raw(WebRTC->last_flight, WebRTC->last_flight_len);
          break;
        }

        if (hs_type == DTLS_HT_CLIENT_KE && !WebRTC->client_ke_done) {
          if (hs_length < 1) break;
          uint8_t pt_len = hs_body[0];
          if (pt_len < 2 || (size_t)(1 + pt_len) > hs_length) break;

          const br_ec_impl* ec = br_ec_get_default();
          if (!ec) break;

          uint8_t shared[65];
          memcpy(shared, hs_body + 1, pt_len);

          uint32_t ok = ec->mul(shared, pt_len,
              WebRTC->ecdhe_sk.x, WebRTC->ecdhe_sk.xlen, BR_EC_secp256r1);
          if (!ok) {
            AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: ECDH mul failed"));
            break;
          }

          size_t xoff_len;
          size_t xoff = ec->xoff(BR_EC_secp256r1, &xoff_len);
          uint8_t premaster[32];
          memcpy(premaster, shared + xoff, xoff_len);

          dtls_hash_hs(payload, DTLS_HS_HDR + hs_length);

          if (!dtls_derive_keys(premaster, xoff_len)) break;

          WebRTC->client_ke_done = true;
          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: ClientKeyExchange processed"));
        }
        break;

      default:
        break;
    }
  } else if (epoch == 1 && WebRTC->epoch_in == 1 && WebRTC->hs_state == DTLS_HS_WAIT_CLIENT_FLIGHT) {
    uint8_t pt_buf[256];
    size_t pt_len;
    if (!dtls_decrypt_record(rec, rec_len, pt_buf, &pt_len)) return;

    if (pt_len < DTLS_HS_HDR) return;
    uint8_t hs_type = pt_buf[0];
    uint32_t hs_length = get_be24(pt_buf + 1);

    if (hs_type == DTLS_HT_FINISHED && hs_length == 12) {
      if (!dtls_verify_client_finished(pt_buf + DTLS_HS_HDR, 12)) return;

      dtls_hash_hs(pt_buf, DTLS_HS_HDR + 12);

      if (!dtls_send_server_ccs_finished()) return;

      WebRTC->hs_state = DTLS_HS_ESTABLISHED;
      WebRTC->state = WEBRTC_DTLS_CONNECTED;

      if (WebRTC->use_srtp_agreed) {
        dtls_export_srtp_keys();
        WebRTC->state = WEBRTC_STREAMING;
        WebRTC->video.active = true;
        WebRTC->audio.active = true;
        AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: DTLS-SRTP established, streaming!"));
#ifdef USE_I2S_AUDIO
        WcAudioStart();
#endif
      } else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: DTLS done but no SRTP profile agreed"));
      }
      AddLog(LOG_LEVEL_INFO, PSTR("WebRTC: DTLS HANDSHAKE COMPLETE"));
    }
  }
}

/*********************************************************************************************/
// DTLS Record Dispatcher
/*********************************************************************************************/

void HandleWebRTCDTLS(void) {
  if (!WebRTC || WebRTC->state < WEBRTC_SIG_SENT_ANSWER) return;

  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);
  int len = WebRTC_udp.parsePacket();
  if (len <= 0) { if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex); return; }

  uint8_t tmp[1500];
  int rlen = WebRTC_udp.read(tmp, sizeof(tmp));
  IPAddress fromIP = WebRTC_udp.remoteIP();
  uint16_t fromPort = WebRTC_udp.remotePort();
  if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex);

  if (rlen <= 0) return;

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: UDP pkt %d bytes from %s:%d [%02X %02X %02X %02X]"),
         rlen, fromIP.toString().c_str(), fromPort,
         rlen > 0 ? tmp[0] : 0, rlen > 1 ? tmp[1] : 0,
         rlen > 2 ? tmp[2] : 0, rlen > 3 ? tmp[3] : 0);

  // Latch remote peer (ICE‑lite style)
  WebRTC->remote_ip   = fromIP;
  WebRTC->remote_port = fromPort;

  // STUN (first byte 0 or 1, magic cookie at offset 4)
  if (rlen >= 20 && tmp[0] < 2) {
    WcHandleSTUN(tmp, rlen, fromIP, fromPort);
    return;
  }

  // RTP/RTCP (first byte 128-191) — ignore inbound media/RTCP
  if (tmp[0] >= 128 && tmp[0] < 192) {
    return;
  }

  // DTLS content types 20-63
  if (tmp[0] < 20 || tmp[0] > 63) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Unknown pkt type=%d from %s:%d"),
           tmp[0], fromIP.toString().c_str(), fromPort);
    return;
  }

  // Process ALL DTLS records contained in this UDP datagram
  int offset = 0;
  while (offset + DTLS_REC_HDR <= rlen) {
    uint8_t  content_type = tmp[offset];
    uint16_t epoch        = get_be16(tmp + offset + 3);
    uint16_t record_len   = get_be16(tmp + offset + 11);

    if (record_len == 0 || record_len > 2048 ||
        offset + DTLS_REC_HDR + record_len > rlen) {
      AddLog(LOG_LEVEL_ERROR,
             PSTR("WebRTC: Invalid DTLS record len=%d at offset=%d (rlen=%d)"),
             record_len, offset, rlen);
      break;
    }

    AddLog(LOG_LEVEL_DEBUG,
           PSTR("WebRTC: Record type=%d epoch=%d len=%d offset=%d state=%d"),
           content_type, epoch, record_len, offset, WebRTC->hs_state);

    const uint8_t *rec     = tmp + offset;
    size_t         rec_len = DTLS_REC_HDR + record_len;

    // Handshake records
    if (content_type == DTLS_CT_HANDSHAKE) {
      if (WebRTC->hs_state == DTLS_HS_ESTABLISHED) {
        if (WebRTC->last_flight_len > 0) {
          AddLog(LOG_LEVEL_DEBUG,
                 PSTR("WebRTC: Retransmit last flight (%d bytes) for client retransmit"),
                 WebRTC->last_flight_len);
          dtls_send_raw(WebRTC->last_flight, WebRTC->last_flight_len);
        }
        offset += DTLS_REC_HDR + record_len;
        continue;
      }
      dtls_process_handshake_record(rec, rec_len);
    }
    // ChangeCipherSpec
    else if (content_type == DTLS_CT_CCS) {
      if (epoch == 0 && WebRTC->hs_state == DTLS_HS_WAIT_CLIENT_FLIGHT) {
        WebRTC->epoch_in        = 1;
        WebRTC->client_ccs_done = true;
        AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Client CCS received, epoch_in=1"));
      } else {
        AddLog(LOG_LEVEL_DEBUG,
               PSTR("WebRTC: Ignoring CCS (epoch=%d hs_state=%d)"),
               epoch, WebRTC->hs_state);
      }
    }
    // Alert
    else if (content_type == DTLS_CT_ALERT) {
      if (record_len >= 2) {
        uint8_t level = rec[DTLS_REC_HDR];
        uint8_t desc  = rec[DTLS_REC_HDR + 1];
        AddLog(LOG_LEVEL_ERROR,
               PSTR("WebRTC: DTLS Alert level=%d desc=%d (hs=%d)"),
               level, desc, WebRTC->hs_state);
      } else {
        AddLog(LOG_LEVEL_ERROR,
               PSTR("WebRTC: DTLS Alert record too short (len=%d)"),
               record_len);
      }
    }
    // Other DTLS types (e.g. appdata) are ignored here
    else {
      AddLog(LOG_LEVEL_DEBUG,
             PSTR("WebRTC: Ignoring DTLS type=%d epoch=%d len=%d"),
             content_type, epoch, record_len);
    }

    offset += DTLS_REC_HDR + record_len;
  }
}


/*********************************************************************************************/
// Background Handshake Task
/*********************************************************************************************/

void HandshakeTask(void *pvParameters) {
  uint32_t last_diag = 0;

  while (!WebRTC) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  if (!WebRTC->udp_ready) {
    if (WebRTC_udp.begin(WEBRTC_DTLS_PORT)) {
      WebRTC->udp_ready = true;
      AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: UDP bound port %d (from task)"), WEBRTC_DTLS_PORT);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: UDP bind FAILED on port %d"), WEBRTC_DTLS_PORT);
    }
  }

  while (true) {
    if (WebRTC && WebRTC->state >= WEBRTC_SIG_SENT_ANSWER && WebRTC->state < WEBRTC_STREAMING) {
      if (millis() - last_diag > 3000) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Waiting STUN/DTLS (state=%d hs=%d udp=%d)"),
            WebRTC->state, WebRTC->hs_state, WebRTC->udp_ready);
        last_diag = millis();
      }
      HandleWebRTCDTLS();
      vTaskDelay(pdMS_TO_TICKS(5));
    } else if (WebRTC && WebRTC->state >= WEBRTC_STREAMING) {
      HandleWebRTCDTLS();
      vTaskDelay(pdMS_TO_TICKS(100));
    } else {
      vTaskDelay(pdMS_TO_TICKS(200));
    }
  }
}

/*********************************************************************************************/
// SDP Parsing and Answer Generation
/*********************************************************************************************/

void WcParseSDP(const char* sdp) {
  if (!WebRTC) return;

  const char* ufrag = strstr(sdp, "a=ice-ufrag:");
  if (ufrag) {
    ufrag += 12;
    size_t len = strcspn(ufrag, "\r\n");
    if (len < sizeof(WebRTC->rem_ufrag)) {
      strncpy(WebRTC->rem_ufrag, ufrag, len);
      WebRTC->rem_ufrag[len] = 0;
    }
  }

  const char* pwd = strstr(sdp, "a=ice-pwd:");
  if (pwd) {
    pwd += 10;
    size_t len = strcspn(pwd, "\r\n");
    if (len < sizeof(WebRTC->rem_pwd)) {
      strncpy(WebRTC->rem_pwd, pwd, len);
      WebRTC->rem_pwd[len] = 0;
    }
  }

  // Find H264 payload type from offer — Chrome assigns PT dynamically (often 102, not 96)
  // Look for "a=rtpmap:NNN H264/90000" to find the correct PT
  WebRTC->video.payload_type = 96;  // default fallback
  const char* rtpmap = sdp;
  while ((rtpmap = strstr(rtpmap, "a=rtpmap:")) != NULL) {
    rtpmap += 9;  // skip "a=rtpmap:"
    int pt = atoi(rtpmap);
    if (pt > 0 && strstr(rtpmap, "H264/90000") != NULL) {
      // Verify this rtpmap line contains H264 (not a later line)
      const char* eol = strpbrk(rtpmap, "\r\n");
      const char* h264 = strstr(rtpmap, "H264/90000");
      if (h264 && (!eol || h264 < eol)) {
        // Check for packetization-mode=1 in corresponding fmtp
        char fmtp_search[32];
        snprintf(fmtp_search, sizeof(fmtp_search), "a=fmtp:%d ", pt);
        const char* fmtp = strstr(sdp, fmtp_search);
        if (fmtp && strstr(fmtp, "packetization-mode=1")) {
          WebRTC->video.payload_type = (uint8_t)pt;
          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Found H264 PT=%d (packetization-mode=1)"), pt);
          break;
        }
        // Accept without fmtp check if no packetization-mode=1 found yet
        if (WebRTC->video.payload_type == 96) {
          WebRTC->video.payload_type = (uint8_t)pt;
        }
      }
    }
  }

  WebRTC->state = WEBRTC_SIG_HAVE_OFFER;
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Parsed SDP, remote ufrag=%s H264_PT=%d"),
         WebRTC->rem_ufrag, WebRTC->video.payload_type);
}

// Base64 encode helper for SPS/PPS in SDP
static void wc_base64_encode(const uint8_t* data, size_t len, char* out, size_t out_size) {
  const char* b64 PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  
  size_t out_pos = 0;
  for (size_t i = 0; i < len; i += 3) {
    if (out_pos + 4 >= out_size) break;
    
    uint32_t n = (data[i] << 16);
    if (i + 1 < len) n |= (data[i+1] << 8);
    if (i + 2 < len) n |= data[i+2];
    
    out[out_pos++] = b64[(n >> 18) & 63];
    out[out_pos++] = b64[(n >> 12) & 63];
    
    if (i + 1 < len) {
      out[out_pos++] = b64[(n >> 6) & 63];
    } else {
      out[out_pos++] = '=';
    }
    
    if (i + 2 < len) {
      out[out_pos++] = b64[n & 63];
    } else {
      out[out_pos++] = '=';
    }
  }
  out[out_pos] = 0;
}

void WcGenerateAnswer(void) {
  if (!WebRTC) return;
  IPAddress ip = NetworkAddress();
  char buf[256];

  Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN);
  Webserver->send(200, PSTR("application/sdp"), "");

  snprintf_P(buf, sizeof(buf), PSTR(
    "v=0\r\no=- %lu 2 IN IP4 %s\r\ns=-\r\nt=0 0\r\na=ice-lite\r\n"),
    millis(), ip.toString().c_str());
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
    "a=fingerprint:sha-256 %s\r\na=group:BUNDLE 0 1\r\na=msid-semantic: WMS tasmota\r\n"),
    WebRTC->fingerprint_str);
  Webserver->sendContent(buf);

  uint8_t vpt = WebRTC->video.payload_type;
  snprintf_P(buf, sizeof(buf), PSTR(
    "m=video %d UDP/TLS/RTP/SAVPF %d\r\nc=IN IP4 %s\r\na=rtcp-mux\r\n"),
    WEBRTC_DTLS_PORT, vpt, ip.toString().c_str());
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
    "a=ice-ufrag:%s\r\na=ice-pwd:%s\r\na=setup:passive\r\n"),
    WebRTC->ice_ufrag, WebRTC->ice_pwd);
  Webserver->sendContent(buf);

  // Generate SPS/PPS Base64 strings for sprop-parameter-sets
  char sps_base64[1024] = "";
  char pps_base64[512] = "";
  char fmtp_line[1536] = "";
  
  if (Wc.h264.sps_pps_captured && Wc.h264.sps_len > 0 && Wc.h264.pps_len > 0) {
    wc_base64_encode(Wc.h264.sps_buffer, Wc.h264.sps_len, sps_base64, sizeof(sps_base64));
    wc_base64_encode(Wc.h264.pps_buffer, Wc.h264.pps_len, pps_base64, sizeof(pps_base64));
    
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Including SPS/PPS in SDP (SPS:%d->%d chars, PPS:%d->%d chars)"),
           Wc.h264.sps_len, strlen(sps_base64), Wc.h264.pps_len, strlen(pps_base64));
    
    // Derive profile-level-id from actual SPS bytes instead of hardcoding
    char plid[7] = "42001E";  // fallback
    if (Wc.h264.sps_len >= 4) {
      snprintf(plid, sizeof(plid), "%02X%02X%02X",
               Wc.h264.sps_buffer[1], Wc.h264.sps_buffer[2], Wc.h264.sps_buffer[3]);
    }
    snprintf_P(fmtp_line, sizeof(fmtp_line), PSTR(
      "a=mid:0\r\na=sendonly\r\na=rtpmap:%d H264/90000\r\n"
      "a=fmtp:%d packetization-mode=1;profile-level-id=%s;sprop-parameter-sets=%s,%s\r\n"
      "a=fingerprint:sha-256 %s\r\n"),
      vpt, vpt, plid, sps_base64, pps_base64, WebRTC->fingerprint_str);
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: No SPS/PPS captured, SDP will not include sprop-parameter-sets"));
    snprintf_P(fmtp_line, sizeof(fmtp_line), PSTR(
      "a=mid:0\r\na=sendonly\r\na=rtpmap:%d H264/90000\r\n"
      "a=fmtp:%d packetization-mode=1;profile-level-id=42001E\r\n"
      "a=fingerprint:sha-256 %s\r\n"),
      vpt, vpt, WebRTC->fingerprint_str);
  }
  
  Webserver->sendContent(fmtp_line);

  snprintf_P(buf, sizeof(buf), PSTR(
    "a=msid:tasmota video\r\n"
    "a=candidate:1 1 udp 2130706431 %s %d typ host\r\n"
    "a=end-of-candidates\r\n"
    "a=ssrc:%u cname:tasmota-video\r\n"
    "a=ssrc:%u msid:tasmota video\r\n"),
    ip.toString().c_str(), WEBRTC_DTLS_PORT, WebRTC->video.ssrc, WebRTC->video.ssrc);
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
#ifdef USE_I2S_OPUS
    "m=audio %d UDP/TLS/RTP/SAVPF 111\r\nc=IN IP4 %s\r\na=rtcp-mux\r\n"),
#else
    "m=audio %d UDP/TLS/RTP/SAVPF 8 0\r\nc=IN IP4 %s\r\na=rtcp-mux\r\n"),
#endif
    WEBRTC_DTLS_PORT, ip.toString().c_str());
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
    "a=ice-ufrag:%s\r\na=ice-pwd:%s\r\na=setup:passive\r\n"),
    WebRTC->ice_ufrag, WebRTC->ice_pwd);
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
#ifdef USE_I2S_OPUS
    "a=mid:1\r\na=sendonly\r\na=rtpmap:111 opus/48000/2\r\n"
    "a=fmtp:111 minptime=20;useinbandfec=0;stereo=0;sprop-stereo=0\r\n"
    "a=fingerprint:sha-256 %s\r\n"),
#else
    "a=mid:1\r\na=sendonly\r\na=rtpmap:8 PCMA/8000\r\na=rtpmap:0 PCMU/8000\r\n"
    "a=fingerprint:sha-256 %s\r\n"),
#endif
    WebRTC->fingerprint_str);
  Webserver->sendContent(buf);

  snprintf_P(buf, sizeof(buf), PSTR(
    "a=msid:tasmota audio\r\n"
    "a=candidate:1 1 udp 2130706431 %s %d typ host\r\n"
    "a=end-of-candidates\r\n"
    "a=ssrc:%u cname:tasmota-audio\r\n"
    "a=ssrc:%u msid:tasmota audio\r\n"),
    ip.toString().c_str(), WEBRTC_DTLS_PORT, WebRTC->audio.ssrc, WebRTC->audio.ssrc);
  Webserver->sendContent(buf);

  WebRTC->state = WEBRTC_SIG_SENT_ANSWER;

  WebRTC->hs_state = DTLS_HS_WAIT_CH;
  WebRTC->epoch_in = 0;
  WebRTC->epoch_out = 0;
  WebRTC->seq_out = 0;
  WebRTC->srv_msg_seq = 0;
  WebRTC->client_ke_done = false;
  WebRTC->client_ccs_done = false;

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: SDP Answer sent"));
}

void HandleWebRTCOffer(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Got Offer"));

  if (!WebRTC) WcInitDTLS();
  if (!WebRTC) { Webserver->send(500, PSTR("text/plain"), PSTR("Init Failed")); return; }

  if (Wc.core.session_type != SESSION_WEBRTC) {
    Webserver->send(500, PSTR("text/plain"), PSTR("Wrong Session Type"));
    return;
  }
  if (!Webserver->hasArg("plain")) {
    Webserver->send(400, PSTR("text/plain"), PSTR("Body missing"));
    return;
  }

  String offer = Webserver->arg("plain");
  WcParseSDP(offer.c_str());
  WcGenerateAnswer();
}

/*********************************************************************************************/
// SRTP Key Derivation (RFC 3711)
/*********************************************************************************************/

void WcSRTPKDF(srtp_context_t* ctx, uint8_t label) {
  uint8_t iv[16];
  memset(iv, 0, 16);
  memcpy(iv, ctx->master_salt, WEBRTC_SRTP_MASTER_SALT_LEN);
  iv[7] ^= label;  // RFC 3711 §4.3.1: IV = master_salt XOR (label || r)

  uint8_t out[32];
  memset(out, 0, 32);

  br_aes_big_ctr_keys kdf_ctx;
  br_aes_big_ctr_init(&kdf_ctx, ctx->master_key, WEBRTC_SRTP_MASTER_KEY_LEN);

  // BearSSL CTR uses 12-byte IV + 4-byte counter (big-endian at bytes 12-15).
  // SRTP needs 14-byte IV + 2-byte block counter at bytes 14-15.
  // Fold iv[12..13] into the upper 16 bits of the cc parameter.
  uint32_t cc0 = ((uint32_t)iv[12] << 24) | ((uint32_t)iv[13] << 16);

  size_t out_len = (label == WEBRTC_SRTP_LABEL_AUTH) ? 20 : 16;
  br_aes_big_ctr_run(&kdf_ctx, iv, cc0, out, out_len);

  if (label == WEBRTC_SRTP_LABEL_ENC) {
    memcpy(ctx->enc_key, out, 16);
  } else if (label == WEBRTC_SRTP_LABEL_SALT) {
    memcpy(ctx->salt_key, out, 14);
  } else if (label == WEBRTC_SRTP_LABEL_AUTH) {
    memcpy(ctx->auth_key, out, 20);
  }
}

/*********************************************************************************************/
// SRTP Packet Encryption (RFC 3711)
/*********************************************************************************************/

void WcSendSrtpPacket(media_track_t* track, const uint8_t* payload, size_t len, bool marker) {
  if (!WebRTC || !track->active || WebRTC->state != WEBRTC_STREAMING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: SRTP not ready (state=%d active=%d)"), 
           WebRTC ? WebRTC->state : -1, track ? track->active : 0);
    return;
  }
  if (WebRTC->remote_port == 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: No remote port"));
    return;
  }

  // Check payload size - must fit in WEBRTC_SRTP_MAX_PAYLOAD
  if (len > WEBRTC_SRTP_MAX_PAYLOAD) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: Payload too large %d > %d"), len, WEBRTC_SRTP_MAX_PAYLOAD);
    return;
  }

  uint8_t rtp_header[12];
  rtp_header[0] = 0x80;
  rtp_header[1] = track->payload_type | (marker ? 0x80 : 0x00);
  rtp_header[2] = (track->seq >> 8) & 0xFF;
  rtp_header[3] = track->seq & 0xFF;
  rtp_header[4] = (track->timestamp >> 24) & 0xFF;
  rtp_header[5] = (track->timestamp >> 16) & 0xFF;
  rtp_header[6] = (track->timestamp >> 8) & 0xFF;
  rtp_header[7] = track->timestamp & 0xFF;
  rtp_header[8] = (track->ssrc >> 24) & 0xFF;
  rtp_header[9] = (track->ssrc >> 16) & 0xFF;
  rtp_header[10] = (track->ssrc >> 8) & 0xFF;
  rtp_header[11] = track->ssrc & 0xFF;

  // Calculate packet index with current seq/roc
  uint64_t pkt_idx = ((uint64_t)track->roc << 16) | track->seq;

  uint8_t iv[16];
  memset(iv, 0, 16);
  memcpy(iv, track->srtp.salt_key, 14);

  iv[4] ^= (track->ssrc >> 24) & 0xFF;
  iv[5] ^= (track->ssrc >> 16) & 0xFF;
  iv[6] ^= (track->ssrc >> 8) & 0xFF;
  iv[7] ^= track->ssrc & 0xFF;

  iv[8]  ^= (pkt_idx >> 40) & 0xFF;
  iv[9]  ^= (pkt_idx >> 32) & 0xFF;
  iv[10] ^= (pkt_idx >> 24) & 0xFF;
  iv[11] ^= (pkt_idx >> 16) & 0xFF;
  iv[12] ^= (pkt_idx >> 8) & 0xFF;
  iv[13] ^= pkt_idx & 0xFF;

  // BearSSL CTR uses 12-byte IV + 4-byte counter (big-endian at bytes 12-15).
  // SRTP needs 14-byte IV + 2-byte block counter at bytes 14-15.
  // Fold iv[12..13] into the upper 16 bits of the cc parameter.
  uint32_t cc0 = ((uint32_t)iv[12] << 24) | ((uint32_t)iv[13] << 16);

  uint8_t enc_payload[len];  // VLA: use actual size, not 1100-byte max (saves stack for audio)
  memcpy(enc_payload, payload, len);

  uint32_t enc_start = micros();

  br_aes_big_ctr_run(&track->srtp.aes_ctr, iv, cc0, enc_payload, len);

  uint8_t tag[20];
  uint32_t roc_be = __builtin_bswap32(track->roc);

  br_hmac_context hmac_ctx;
  br_hmac_init(&hmac_ctx, &track->srtp.hmac_key, 0);
  br_hmac_update(&hmac_ctx, rtp_header, 12);
  br_hmac_update(&hmac_ctx, enc_payload, len);
  br_hmac_update(&hmac_ctx, &roc_be, 4);
  br_hmac_out(&hmac_ctx, tag);

  uint32_t enc_time = micros() - enc_start;

  // Rolling average encryption time (per packet, AES-CTR + HMAC)
  static uint32_t enc_avg_us = 0;
  static uint32_t enc_last_log = 0;
  enc_avg_us = (enc_avg_us * 7 + enc_time) / 8;  // 8-sample EMA
  if (millis() - enc_last_log >= 5000) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: SRTP Encrypt avg:%uus (last:%uus) PayloadSize:%u"),
           enc_avg_us, enc_time, len);
    enc_last_log = millis();
  }

  // Debug SRTP authentication tag for first few packets (reset on new session)
  static uint32_t debug_packet_count = 0;
  static uint32_t debug_session_id = 0;
  if (debug_session_id != WebRTC->session_id) { debug_packet_count = 0; debug_session_id = WebRTC->session_id; }
  if (debug_packet_count < 5) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: SRTP TAG[0:3]=%02X%02X%02X%02X seq=%u roc=%u pkt_idx=%u:%u"),
           tag[0], tag[1], tag[2], tag[3], track->seq, track->roc,
           (uint32_t)(pkt_idx >> 32), (uint32_t)(pkt_idx & 0xFFFFFFFF));
    debug_packet_count++;
  }

  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);
  WebRTC_udp.beginPacket(WebRTC->remote_ip, WebRTC->remote_port);
  WebRTC_udp.write(rtp_header, 12);
  WebRTC_udp.write(enc_payload, len);
  WebRTC_udp.write(tag, WEBRTC_SRTP_AUTH_TAG);
  int sent = WebRTC_udp.endPacket();
  if (WebRTC->mutex) xSemaphoreGive(WebRTC->mutex);

  if (sent > 0) {
    // Log first few packets and then periodically (reset on new session)
    static uint32_t packet_count = 0;
    static uint32_t pkt_session_id = 0;
    if (pkt_session_id != WebRTC->session_id) { packet_count = 0; pkt_session_id = WebRTC->session_id; }
    if (packet_count < 10 || (packet_count % 100) == 0) {
      // ESP32 doesn't support %llu properly, split 64-bit into two 32-bit parts
      uint32_t pkt_idx_high = (uint32_t)(pkt_idx >> 32);
      uint32_t pkt_idx_low = (uint32_t)(pkt_idx & 0xFFFFFFFF);
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: SRTP sent seq=%u ts=%u len=%d to %s:%d (pkt=%u:%u)"),
             track->seq, track->timestamp, len, 
             WebRTC->remote_ip.toString().c_str(), WebRTC->remote_port,
             pkt_idx_high, pkt_idx_low);
    }
    packet_count++;
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("WebRTC: UDP send failed seq=%u len=%d to %s:%d"),
           track->seq, len, WebRTC->remote_ip.toString().c_str(), WebRTC->remote_port);
  }

  // Update sequence number and handle rollover
  track->seq++;
  if (track->seq == 0) {  // wrapped from 0xFFFF to 0
    track->roc++;
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: ROC incremented to %u"), track->roc);
  }
}

/*********************************************************************************************/
// H.264 Video over SRTP
/*********************************************************************************************/

void WcSendSrtpNal(uint8_t* nal_data, size_t nal_len) {
  if (nal_len == 0 || !WebRTC) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Empty NAL or no WebRTC"));
    return;
  }

  uint8_t nal_header = nal_data[0];
  uint8_t nal_type = nal_header & 0x1F;
  
  // Log NAL type for debugging (reset on new session)
  static uint32_t nal_count = 0;
  static bool idr_seen = false;  // Drop P-frames until first IDR
  static uint32_t nal_session_id = 0;
  if (nal_session_id != WebRTC->session_id) { nal_count = 0; idr_seen = false; nal_session_id = WebRTC->session_id; }
  
  if (nal_count < 20 || (nal_count % 100) == 0) {
    const char* nal_name = "Unknown";
    switch (nal_type) {
      case 1: nal_name = "P-frame"; break;
      case 5: nal_name = "IDR (I-frame)"; break;
      case 6: nal_name = "SEI"; break;
      case 7: nal_name = "SPS"; break;
      case 8: nal_name = "PPS"; break;
      case 9: nal_name = "AUD"; break;
    }
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: NAL type=%d (%s) len=%d (total=%u idr_seen=%d)"), 
           nal_type, nal_name, nal_len, nal_count, idr_seen);
  }
  nal_count++;

  // Drop P-frames until we've seen the first IDR — Chrome can't decode without one
  if (!idr_seen && nal_type != 5 && nal_type != 7 && nal_type != 8 && nal_type != 6) {
    return;
  }

  // Send SPS/PPS before EVERY IDR frame — Chrome needs them for decoding
  if (nal_type == 5) {
    idr_seen = true;
    if (Wc.h264.sps_pps_captured && Wc.h264.sps_len > 0 && Wc.h264.pps_len > 0) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: Sending SPS/PPS before IDR"));
      if (Wc.h264.sps_len <= WEBRTC_SRTP_MAX_PAYLOAD) {
        WcSendSrtpPacket(&WebRTC->video, Wc.h264.sps_buffer, Wc.h264.sps_len, false);
      }
      if (Wc.h264.pps_len <= WEBRTC_SRTP_MAX_PAYLOAD) {
        WcSendSrtpPacket(&WebRTC->video, Wc.h264.pps_buffer, Wc.h264.pps_len, false);
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: IDR frame but no SPS/PPS captured!"));
    }
  }

  if (nal_len <= WEBRTC_SRTP_MAX_PAYLOAD) {
    bool marker = (nal_type == 1 || nal_type == 5);  // I/P-frame or IDR
    WcSendSrtpPacket(&WebRTC->video, nal_data, nal_len, marker);
    // Note: seq is incremented inside WcSendSrtpPacket now
  } else {
    // FU-A fragmentation
    uint8_t fu_indicator = (nal_header & 0xE0) | 28;
    uint8_t fu_type = nal_header & 0x1F;
    size_t offset = 1;
    size_t payload_cap = WEBRTC_SRTP_MAX_PAYLOAD - 2;
    uint8_t fragment_count = 0;

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: Fragmenting NAL %d bytes into FU-A"), nal_len);

    while (offset < nal_len) {
      size_t chunk = nal_len - offset;
      if (chunk > payload_cap) chunk = payload_cap;

      bool is_first = (offset == 1);
      bool is_last = (offset + chunk >= nal_len);

      uint8_t fu_header = fu_type;
      if (is_first) fu_header |= 0x80;
      if (is_last)  fu_header |= 0x40;

      uint8_t pkt[WEBRTC_SRTP_MAX_PAYLOAD];
      pkt[0] = fu_indicator;
      pkt[1] = fu_header;
      memcpy(pkt + 2, nal_data + offset, chunk);

      bool marker = is_last && (nal_type == 1 || nal_type == 5);
      WcSendSrtpPacket(&WebRTC->video, pkt, chunk + 2, marker);
      // Note: seq is incremented inside WcSendSrtpPacket now

      offset += chunk;
      fragment_count++;
    }
    
    if (fragment_count > 0) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WebRTC: NAL fragmented into %d packets"), fragment_count);
    }
  }
}

void WcSendSrtpVideo(uint8_t* data, size_t len) {
  if (!WebRTC || len == 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: No video data or WebRTC not ready"));
    return;
  }

  // Calculate timestamp increment: 90kHz clock / fps
  uint32_t timestamp_inc = 90000 / Wc.core.config.fps;
  if (timestamp_inc == 0) timestamp_inc = 3000; // default to 30fps if fps is 0
  WebRTC->video.timestamp += timestamp_inc;

  // Log frame statistics periodically (reset on new session)
  static uint32_t frame_count = 0;
  static uint32_t total_bytes = 0;
  static uint32_t last_log_time = 0;
  static uint32_t vid_session_id = 0;
  if (vid_session_id != WebRTC->session_id) { frame_count = 0; total_bytes = 0; last_log_time = 0; vid_session_id = WebRTC->session_id; }
  
  frame_count++;
  total_bytes += len;
  
  uint32_t now = millis();
  if (now - last_log_time >= 5000) { // Log every 5 seconds
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Video frame %u: %u bytes, ts=%u, seq=%u, fps=%u"),
           frame_count, len, WebRTC->video.timestamp, WebRTC->video.seq, Wc.core.config.fps);
    last_log_time = now;
  }

  uint8_t* p = data;
  size_t remaining = len;
  uint8_t nal_count = 0;

  // Parse H.264 NAL units (separated by start codes: 0x000001 or 0x00000001)
  while (remaining > 0) {
    size_t sc_len = 0;
    if (remaining >= 4 && p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1) {
      sc_len = 4; // 4-byte start code
    } else if (remaining >= 3 && p[0] == 0 && p[1] == 0 && p[2] == 1) {
      sc_len = 3; // 3-byte start code
    } else {
      // No start code found, skip this byte
      p++;
      remaining--;
      continue;
    }

    p += sc_len;
    remaining -= sc_len;

    // Find next start code to determine NAL unit length
    size_t nal_len = remaining;
    for (size_t i = 0; i + 2 < remaining; i++) {
      if (p[i] == 0 && p[i+1] == 0 && p[i+2] == 1) {
        nal_len = i;
        break;
      }
      if (i + 3 < remaining && p[i] == 0 && p[i+1] == 0 && p[i+2] == 0 && p[i+3] == 1) {
        nal_len = i;
        break;
      }
    }

    if (nal_len == 0 && remaining > 0) {
      nal_len = 1; // Minimum NAL unit size
    }

    if (nal_len > 0) {
      WcSendSrtpNal(p, nal_len);
      nal_count++;
    }

    p += nal_len;
    remaining -= nal_len;
  }

  if (nal_count > 0 && frame_count <= 10) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Frame %u parsed into %u NAL units"), frame_count, nal_count);
  }
}

// Audio task (I2S mic → Opus/G.711 → SRTP) is in xdrv_81_4a_esp32_webcam_CSI_audio.ino

/*********************************************************************************************/
// WebRTC Processing Task
/*********************************************************************************************/

void WebRTCProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  esp_h264_enc_in_frame_t in_frame = {};
  esp_h264_enc_out_frame_t out_frame = {};

  size_t yuv_size;
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  uint32_t last_profile_log = 0;
  bool was_streaming = false;  // Track DTLS state transitions to discard stale frames

  while (true) {
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);

    if (Wc.core.state == CAM_STOPPING) break;
    if (Wc.core.state == CAM_PAUSING) {
      Wc.core.state = CAM_PAUSED;
      xSemaphoreTake(Wc.core.resume_sem, portMAX_DELAY);
      if (Wc.core.state == CAM_STOPPING) break;
      continue;
    }

    if (Wc.core.state != CAM_STREAMING || ulNotificationValue == 0) continue;
    if (xSemaphoreTake(Wc.core.frame_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      continue;
    }

    if (Wc.core.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.core.frame_mutex);
      break;
    }

    uint32_t frame_start = millis();
    uint8_t *src = Wc.core.frame_buffer[Wc.core.read_idx];
    esp_cache_msync(src, Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    in_frame.raw_data.buffer = src;
    yuv_size = Wc.core.config.width * Wc.core.config.height * 3 / 2;
    in_frame.raw_data.len = yuv_size;
    out_frame.raw_data.buffer = Wc.h264.out_buffer;
    out_frame.raw_data.len = Wc.h264.out_buffer_size;

    uint32_t encode_start = micros();
    esp_h264_err_t ret = esp_h264_enc_process(Wc.h264.handle, &in_frame, &out_frame);
    uint32_t encode_time = micros() - encode_start;
    xSemaphoreGive(Wc.core.frame_mutex);

    if (ret == ESP_H264_ERR_OK && out_frame.length > 0) {
      bool is_keyframe = (out_frame.frame_type == ESP_H264_FRAME_TYPE_IDR ||
                          out_frame.frame_type == ESP_H264_FRAME_TYPE_I);
      if (!is_keyframe) {
        Wc.h264.motion_raw = out_frame.length;
        uint32_t raw_score = out_frame.length / 128;
        Wc.h264.motion_val = (Wc.h264.motion_val * 4 + raw_score) / 5;
      }

      if (WebRTC && WebRTC->state == WEBRTC_STREAMING) {
        // On DTLS transition: discard stale frames, wait for fresh IDR
        if (!was_streaming) {
          bool is_idr = (out_frame.frame_type == ESP_H264_FRAME_TYPE_IDR ||
                         out_frame.frame_type == ESP_H264_FRAME_TYPE_I);
          if (!is_idr) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Discarding stale P-frame after DTLS connect"));
            continue;  // skip this frame, wait for IDR
          }
          was_streaming = true;
          AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: First IDR after DTLS connect, starting stream"));
        }
        WcSendSrtpVideo(out_frame.raw_data.buffer, out_frame.length);
      } else {
        was_streaming = false;
      }

      WcStats.frames_processed++;
      frames_in_second++;
      WcStats.bytes_sent += out_frame.length;
    }

    WcStats.last_frame_time_ms = millis() - frame_start;

    if (millis() - last_fps_calc >= 1000) {
      WcStats.last_fps = frames_in_second;
      frames_in_second = 0;
      last_fps_calc = millis();
      if (WcStats.start_time > 0) {
        WcStats.uptime_seconds = (millis() - WcStats.start_time) / 1000;
      }
    }

    if (millis() - last_profile_log >= 5000) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Encode:%uus Size:%u FPS:%u State:%d VidActive:%d"),
        encode_time,
        out_frame.length,
        WcStats.last_fps,
        WebRTC ? WebRTC->state : -1,
        WebRTC ? WebRTC->video.active : 0);
      last_profile_log = millis();
    }
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Processing task exited"));
  Wc.core.cam_task_handle = NULL;
  vTaskDelete(NULL);
}

/*********************************************************************************************/
// Setup
/*********************************************************************************************/

void WcWebRTCStop(void) {
  if (!WebRTC) return;
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Stopping"));

  // 0. Stop audio task first (before invalidating WebRTC state)
#ifdef USE_I2S_AUDIO
  WcAudioStop();
#endif

  // 1. Signal inactive first so other tasks/functions see it and bail out
  WebRTC->state = WEBRTC_IDLE;
  WebRTC->video.active = false;
  WebRTC->audio.active = false;

  // 2. Delete handshake task before touching any shared state
  if (WebRTC->handshake_task) {
    vTaskDelete(WebRTC->handshake_task);
    WebRTC->handshake_task = NULL;
  }

  // 3. Take mutex to ensure no concurrent UDP/SRTP sends are in flight
  if (WebRTC->mutex) xSemaphoreTake(WebRTC->mutex, portMAX_DELAY);

  WebRTC_udp.stop();
  WebRTC->udp_ready = false;

  if (WebRTC->mutex) {
    xSemaphoreGive(WebRTC->mutex);
    vSemaphoreDelete(WebRTC->mutex);
    WebRTC->mutex = NULL;
  }

  // 4. Zero sensitive key material before freeing
  memset(WebRTC->master_secret, 0, sizeof(WebRTC->master_secret));
  memset(WebRTC->ecdhe_priv, 0, sizeof(WebRTC->ecdhe_priv));
  memset(WebRTC->cert_key_ram, 0, sizeof(WebRTC->cert_key_ram));
  memset(WebRTC->client_write_key, 0, sizeof(WebRTC->client_write_key));
  memset(WebRTC->server_write_key, 0, sizeof(WebRTC->server_write_key));

  free(WebRTC);
  WebRTC = NULL;
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Stopped"));
}

uint32_t WcSetupWebRTC(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Setup Start"));

  if (WebRTC == NULL) {
    WebRTC = (WebRTC_State_t*)calloc(1, sizeof(WebRTC_State_t));
  }

  if (WebRTC) {
    if (!WebRTC->mutex) WebRTC->mutex = xSemaphoreCreateMutex();
    WebServer_on(PSTR("/webrtc/offer"), HandleWebRTCOffer);
    WcInitDTLS();
    AddLog(LOG_LEVEL_DEBUG, PSTR("WebRTC: Setup Complete"));
  }
  return 1;
}

#endif // USE_CSI_WEBCAM
#endif // ESP32
