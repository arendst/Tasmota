/*
  xsns_121_multical21.ino - Kamstrup Multical21 / FlowIQ wireless M-Bus (C1) water
                            meter reader for Tasmota, using a TI CC1101 sub-GHz
                            transceiver on hardware SPI.

  Copyright (C) 2026  Tasmota integration based on:
    https://github.com/pthalin/esp32-multical21  (GPL-3.0)
    Original code (C) 2020 chester4444@wolke7.net

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  -----------------------------------------------------------------------------

  Wiring (assigned at runtime via Tasmota "Configure Module"):

    CC1101 pin        Tasmota GPIO function
    --------------    ---------------------------
    VCC               3V3
    GND               GND
    CSN               SPI CS
    MOSI              SPI MOSI
    MISO              SPI MISO
    SCK               SPI CLK
    GDO0              CC1101 GDO0    (interrupt, any free GPIO)
    GDO2              not connected

  Example for the ESP32-C3 Super Mini board the original project targets:
    CSN=GPIO7, MOSI=GPIO6, MISO=GPIO5, SCK=GPIO4, GDO0=GPIO10.

  Runtime configuration (console / MQTT, persisted to filesystem -> no re-flash):

    M21Key <32-hex-chars>     - AES-128 decryption key (ask your water provider)
    M21Id  <8-hex-chars>      - Meter serial number (as printed on the meter)
    M21Type <0|1>             - 0 = Multical21 (default), 1 = FlowIQ 2200
    M21Period <seconds>       - Min. seconds between two telemetry pushes
                                (0 = use Tasmota TelePeriod only, default = 0)
    M21Info                   - Show driver status

    Issued without arguments any setter prints the current (masked) value.

  Build:
    Add  #define USE_MULTICAL21  to user_config_override.h, or
        -DUSE_MULTICAL21        to platformio build_flags.
    Works on ESP32 and ESP8266 (D1 mini etc.).

\*********************************************************************************************/

#ifdef USE_MULTICAL21

#define XSNS_121                       121

#include <SPI.h>
// Self-contained AES-128 ECB (public-domain, derived from tiny-AES-c by Kokke).
// Avoids any dependency on external crypto libraries so the driver builds in
// every Tasmota environment (tasmota, tasmota-sensors, ESP8266, ESP32, ...).

/*-------------------------------------------------------------------------------------------*\
 * CC1101 register / strobe definitions (subset needed for wM-Bus mode C1).
 * Default values taken from the upstream Multical21 project (chester4444 / pthalin).
\*-------------------------------------------------------------------------------------------*/

#define M21_WRITE_BURST                0x40
#define M21_READ_SINGLE                0x80
#define M21_READ_BURST                 0xC0

#define M21_CC_CONFIG_REGISTER         M21_READ_SINGLE
#define M21_CC_STATUS_REGISTER         M21_READ_BURST

#define M21_CC_PATABLE                 0x3E
#define M21_CC_TXFIFO                  0x3F
#define M21_CC_RXFIFO                  0x3F

#define M21_CC_SRES                    0x30
#define M21_CC_SCAL                    0x33
#define M21_CC_SRX                     0x34
#define M21_CC_SIDLE                   0x36
#define M21_CC_SFRX                    0x3A

#define M21_CC_IOCFG2                  0x00
#define M21_CC_IOCFG0                  0x02
#define M21_CC_FIFOTHR                 0x03
#define M21_CC_SYNC1                   0x04
#define M21_CC_SYNC0                   0x05
#define M21_CC_PKTLEN                  0x06
#define M21_CC_PKTCTRL1                0x07
#define M21_CC_PKTCTRL0                0x08
#define M21_CC_ADDR                    0x09
#define M21_CC_CHANNR                  0x0A
#define M21_CC_FSCTRL1                 0x0B
#define M21_CC_FSCTRL0                 0x0C
#define M21_CC_FREQ2                   0x0D
#define M21_CC_FREQ1                   0x0E
#define M21_CC_FREQ0                   0x0F
#define M21_CC_MDMCFG4                 0x10
#define M21_CC_MDMCFG3                 0x11
#define M21_CC_MDMCFG2                 0x12
#define M21_CC_MDMCFG1                 0x13
#define M21_CC_MDMCFG0                 0x14
#define M21_CC_DEVIATN                 0x15
#define M21_CC_MCSM1                   0x17
#define M21_CC_MCSM0                   0x18
#define M21_CC_FOCCFG                  0x19
#define M21_CC_BSCFG                   0x1A
#define M21_CC_AGCCTRL2                0x1B
#define M21_CC_AGCCTRL1                0x1C
#define M21_CC_AGCCTRL0                0x1D
#define M21_CC_FREND1                  0x21
#define M21_CC_FREND0                  0x22
#define M21_CC_FSCAL3                  0x23
#define M21_CC_FSCAL2                  0x24
#define M21_CC_FSCAL1                  0x25
#define M21_CC_FSCAL0                  0x26
#define M21_CC_FSTEST                  0x29
#define M21_CC_TEST2                   0x2C
#define M21_CC_TEST1                   0x2D
#define M21_CC_TEST0                   0x2E

#define M21_CC_MARCSTATE               0x35
#define M21_CC_RSSI                    0x34
#define M21_MARCSTATE_IDLE             0x01
#define M21_MARCSTATE_RX               0x0D

// Mode C1 default register values (from upstream WaterMeter.h)
static const uint8_t M21_CC_DEFAULTS[] PROGMEM = {
  M21_CC_IOCFG2,   0x2E,
  M21_CC_IOCFG0,   0x06,
  M21_CC_FIFOTHR,  0x00,
  M21_CC_PKTLEN,   0x30,
  M21_CC_PKTCTRL1, 0x00,
  M21_CC_PKTCTRL0, 0x02,  // infinite length
  M21_CC_SYNC1,    0x54,
  M21_CC_SYNC0,    0x3D,
  M21_CC_ADDR,     0x00,
  M21_CC_CHANNR,   0x00,
  M21_CC_FSCTRL1,  0x08,
  M21_CC_FSCTRL0,  0x00,
  M21_CC_FREQ2,    0x21,  // 868.95 MHz
  M21_CC_FREQ1,    0x6B,
  M21_CC_FREQ0,    0xD0,
  M21_CC_MDMCFG4,  0x5C,  // 103 kBaud
  M21_CC_MDMCFG3,  0x04,
  M21_CC_MDMCFG2,  0x06,
  M21_CC_MDMCFG1,  0x22,
  M21_CC_MDMCFG0,  0xF8,
  M21_CC_DEVIATN,  0x44,
  M21_CC_MCSM1,    0x00,
  M21_CC_MCSM0,    0x18,
  M21_CC_FOCCFG,   0x2E,
  M21_CC_BSCFG,    0xBF,
  M21_CC_AGCCTRL2, 0x43,
  M21_CC_AGCCTRL1, 0x09,
  M21_CC_AGCCTRL0, 0xB5,
  M21_CC_FREND1,   0xB6,
  M21_CC_FREND0,   0x10,
  M21_CC_FSCAL3,   0xEA,
  M21_CC_FSCAL2,   0x2A,
  M21_CC_FSCAL1,   0x00,
  M21_CC_FSCAL0,   0x1F,
  M21_CC_FSTEST,   0x59,
  M21_CC_TEST2,    0x81,
  M21_CC_TEST1,    0x35,
  M21_CC_TEST0,    0x09,
};

/*-------------------------------------------------------------------------------------------*\
 * Driver state
\*-------------------------------------------------------------------------------------------*/

#define M21_FRAME_MAX                  64
#define M21_CFG_FILE                   "/.drvset121"
#define M21_CFG_MAGIC                  0x4D32314B  // 'M21K'
#define M21_CFG_VERSION                1

#define M21_METER_MULTICAL21           0
#define M21_METER_FLOWIQ2200           1

typedef struct {
  uint32_t magic;
  uint16_t version;
  uint16_t telemetry_period;     // seconds, 0 = use TelePeriod only
  uint8_t  meter_type;           // 0 = Multical21, 1 = FlowIQ 2200
  uint8_t  reserved[3];
  uint8_t  meter_id[4];          // little-endian as printed (digit-pair order)
  uint8_t  aes_key[16];          // AES-128 key
} M21Config;

typedef struct {
  M21Config cfg;
  int8_t   pin_cs;
  int8_t   pin_gdo0;

  bool     hw_ok;                // SPI/CC1101 initialised
  bool     configured;           // key + meter id set
  bool     have_data;            // at least one valid frame decoded

  float    total_m3;
  float    target_m3;
  int8_t   flow_temp_c;
  int8_t   ambient_temp_c;
  int8_t   last_rssi_dbm;

  uint32_t frames_total;
  uint32_t frames_valid;
  uint32_t last_valid_ms;
  uint32_t last_publish_ms;
} M21State;

static M21State *M21 = nullptr;
static volatile bool M21_packet_flag = false;

static void IRAM_ATTR M21Gdo0Isr(void) {
  M21_packet_flag = true;
}

/*-------------------------------------------------------------------------------------------*\
 * CC1101 low-level SPI helpers
 *
 * The CC1101 is the only device on the bus that needs MODE 0 @ 5 MHz, MSB first.
 * Every byte exchange is wrapped in SPI.beginTransaction / endTransaction so other
 * Tasmota SPI drivers (display, SD card, MFRC522, ...) can share the same bus
 * with their own settings.
\*-------------------------------------------------------------------------------------------*/

static const SPISettings M21_SPI_SETTINGS(5000000, MSBFIRST, SPI_MODE0);

static inline void M21CsLow(void)  { digitalWrite(M21->pin_cs, LOW); }
static inline void M21CsHigh(void) { digitalWrite(M21->pin_cs, HIGH); }

static bool M21WaitMiso(void) {
  int8_t miso = Pin(GPIO_SPI_MISO);
  if (miso < 0) { return false; }
  for (uint16_t i = 0; i < 10000; i++) {
    if (digitalRead(miso) == LOW) { return true; }
  }
  return false;
}

static void M21WriteReg(uint8_t addr, uint8_t value) {
  SPI.beginTransaction(M21_SPI_SETTINGS);
  M21CsLow();
  M21WaitMiso();
  SPI.transfer(addr);
  SPI.transfer(value);
  M21CsHigh();
  SPI.endTransaction();
}

static void M21Strobe(uint8_t cmd) {
  SPI.beginTransaction(M21_SPI_SETTINGS);
  M21CsLow();
  delayMicroseconds(5);
  M21WaitMiso();
  SPI.transfer(cmd);
  delayMicroseconds(5);
  M21CsHigh();
  SPI.endTransaction();
}

static uint8_t M21ReadReg(uint8_t addr, uint8_t type) {
  uint8_t val;
  SPI.beginTransaction(M21_SPI_SETTINGS);
  M21CsLow();
  M21WaitMiso();
  SPI.transfer(addr | type);
  val = SPI.transfer(0x00);
  M21CsHigh();
  SPI.endTransaction();
  return val;
}

static uint8_t M21ReadFifoByte(void) {
  return M21ReadReg(M21_CC_RXFIFO, M21_CC_CONFIG_REGISTER);
}

static bool M21Reset(void) {
  SPI.beginTransaction(M21_SPI_SETTINGS);
  M21CsHigh();
  delayMicroseconds(5);
  int8_t mosi = Pin(GPIO_SPI_MOSI);
  int8_t sck  = Pin(GPIO_SPI_CLK);
  if (mosi >= 0) { digitalWrite(mosi, LOW); }
  if (sck  >= 0) { digitalWrite(sck,  HIGH); }
  M21CsLow();
  delayMicroseconds(10);
  M21CsHigh();
  delayMicroseconds(45);
  M21CsLow();
  if (!M21WaitMiso()) { M21CsHigh(); SPI.endTransaction(); return false; }
  SPI.transfer(M21_CC_SRES);
  if (!M21WaitMiso()) { M21CsHigh(); SPI.endTransaction(); return false; }
  M21CsHigh();
  SPI.endTransaction();
  return true;
}

static void M21StartReceiver(void) {
  M21Strobe(M21_CC_SIDLE);
  uint32_t guard = millis();
  while (M21ReadReg(M21_CC_MARCSTATE, M21_CC_STATUS_REGISTER) != M21_MARCSTATE_IDLE) {
    if (millis() - guard > 50) { break; }
    delay(1);
  }
  M21Strobe(M21_CC_SFRX);
  M21Strobe(M21_CC_SRX);
  guard = millis();
  while (M21ReadReg(M21_CC_MARCSTATE, M21_CC_STATUS_REGISTER) != M21_MARCSTATE_RX) {
    if (millis() - guard > 50) { break; }
    delay(1);
  }
}

static void M21LoadDefaults(void) {
  for (size_t i = 0; i < sizeof(M21_CC_DEFAULTS); i += 2) {
    uint8_t addr  = pgm_read_byte(&M21_CC_DEFAULTS[i]);
    uint8_t value = pgm_read_byte(&M21_CC_DEFAULTS[i + 1]);
    M21WriteReg(addr, value);
  }
}

/*-------------------------------------------------------------------------------------------*\
 * AES-128 ECB (public-domain, tiny-AES-c)
\*-------------------------------------------------------------------------------------------*/

static const uint8_t M21_AES_SBOX[256] PROGMEM = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const uint8_t M21_AES_RCON[11] PROGMEM = {
  0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36
};

static inline uint8_t M21Sbox(uint8_t v) { return pgm_read_byte(&M21_AES_SBOX[v]); }

// Key expansion: 16-byte key -> 11 * 16-byte round keys (176 bytes total).
static void M21AesKeyExpand(const uint8_t *key, uint8_t *rk) {
  memcpy(rk, key, 16);
  for (uint8_t i = 1; i <= 10; i++) {
    uint8_t *prev = rk + (i - 1) * 16;
    uint8_t *cur  = rk + i * 16;
    uint8_t t[4] = { prev[13], prev[14], prev[15], prev[12] };  // RotWord
    for (uint8_t j = 0; j < 4; j++) { t[j] = M21Sbox(t[j]); }   // SubWord
    t[0] ^= pgm_read_byte(&M21_AES_RCON[i]);
    for (uint8_t j = 0; j < 4; j++) { cur[j] = prev[j] ^ t[j]; }
    for (uint8_t j = 4; j < 16; j++) { cur[j] = prev[j] ^ cur[j - 4]; }
  }
}

static inline uint8_t M21Xtime(uint8_t x) {
  return (uint8_t)((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

static void M21AesEncryptBlock(uint8_t *st, const uint8_t *rk) {
  // AddRoundKey 0
  for (uint8_t i = 0; i < 16; i++) { st[i] ^= rk[i]; }
  for (uint8_t round = 1; round <= 10; round++) {
    // SubBytes
    for (uint8_t i = 0; i < 16; i++) { st[i] = M21Sbox(st[i]); }
    // ShiftRows
    uint8_t t;
    t = st[1];  st[1]  = st[5];  st[5]  = st[9];  st[9]  = st[13]; st[13] = t;
    t = st[2];  st[2]  = st[10]; st[10] = t;
    t = st[6];  st[6]  = st[14]; st[14] = t;
    t = st[15]; st[15] = st[11]; st[11] = st[7];  st[7]  = st[3];  st[3]  = t;
    // MixColumns (skipped in last round)
    if (round != 10) {
      for (uint8_t c = 0; c < 4; c++) {
        uint8_t *col = st + 4 * c;
        uint8_t a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
        uint8_t s = a0 ^ a1 ^ a2 ^ a3;
        col[0] ^= s ^ M21Xtime(a0 ^ a1);
        col[1] ^= s ^ M21Xtime(a1 ^ a2);
        col[2] ^= s ^ M21Xtime(a2 ^ a3);
        col[3] ^= s ^ M21Xtime(a3 ^ a0);
      }
    }
    // AddRoundKey
    const uint8_t *rkr = rk + round * 16;
    for (uint8_t i = 0; i < 16; i++) { st[i] ^= rkr[i]; }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * EN13757 CRC and AES-128-CTR via BearSSL
\*-------------------------------------------------------------------------------------------*/

#define M21_CRC_POLY                   0x3D65

static uint16_t M21Crc16(const uint8_t *data, size_t len) {
  uint16_t crc = 0x0000;
  for (size_t i = 0; i < len; i++) {
    uint8_t b = data[i];
    for (uint8_t j = 0; j < 8; j++) {
      bool xorflag = ((crc & 0x8000) >> 8) ^ (b & 0x80);
      crc = xorflag ? ((crc << 1) ^ M21_CRC_POLY) : (crc << 1);
      b <<= 1;
    }
  }
  return ~crc;
}

static void M21AesCtrDecrypt(const uint8_t *payload, uint8_t *buf, size_t len) {
  // 16-byte CTR counter = { payload[1..8], payload[10], payload[12..15], 0, 0, 0 }
  uint8_t iv[16];
  memcpy(iv, &payload[1], 8);
  iv[8]  = payload[10];
  iv[9]  = payload[12];
  iv[10] = payload[13];
  iv[11] = payload[14];
  iv[12] = payload[15];
  iv[13] = 0;
  iv[14] = 0;
  iv[15] = 0;

  uint8_t rk[176];                  // 11 * 16-byte round keys
  M21AesKeyExpand(M21->cfg.aes_key, rk);

  uint8_t ks[16];                   // keystream block
  for (size_t off = 0; off < len; off += 16) {
    memcpy(ks, iv, 16);
    M21AesEncryptBlock(ks, rk);
    size_t chunk = (len - off) < 16 ? (len - off) : 16;
    for (size_t i = 0; i < chunk; i++) {
      buf[off + i] ^= ks[i];
    }
    // increment 32-bit big-endian counter at iv[12..15]
    for (int i = 15; i >= 12; i--) {
      if (++iv[i]) { break; }
    }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * wM-Bus frame parsing (Multical21 compact 0x79 / long 0x78, FlowIQ 2200 compact)
\*-------------------------------------------------------------------------------------------*/

static void M21ParsePlain(const uint8_t *data, size_t len) {
  if (len < 7) { return; }                     // need at least CRC(2) + CI(1) + a few payload bytes

  uint16_t calc = M21Crc16(data + 2, len - 2);
  uint16_t recv = (uint16_t)data[1] << 8 | data[0];
  if (calc != recv) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("M21: CRC mismatch calc=%04X recv=%04X len=%u CI=%02X"),
           calc, recv, (unsigned)len, data[2]);
    return;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("M21: CRC ok len=%u CI=%02X"), (unsigned)len, data[2]);

  int pos_tt, pos_tg, pos_ft, pos_at;
  bool have_extra = true;

  if (M21->cfg.meter_type == M21_METER_FLOWIQ2200) {
    if (data[2] != 0x79) { return; }
    pos_tt = 29;
    pos_tg = pos_ft = pos_at = -1;
    have_extra = false;
  } else if (data[2] == 0x79) {                // Multical21 compact frame
    pos_tt = 9;  pos_tg = 13; pos_ft = 17; pos_at = 18;
  } else if (data[2] == 0x78) {                // Multical21 long frame
    pos_tt = 10; pos_tg = 16; pos_ft = 23; pos_at = 29;
  } else {
    return;
  }

  if ((size_t)(pos_tt + 4) > len) { return; }

  uint32_t tt = (uint32_t)data[pos_tt]
              | ((uint32_t)data[pos_tt + 1] << 8)
              | ((uint32_t)data[pos_tt + 2] << 16)
              | ((uint32_t)data[pos_tt + 3] << 24);
  M21->total_m3 = (float)tt / 1000.0f;

  // Target volume is present in the Multical21 compact frame even though the
  // temperatures are only carried by the long frame; parse them independently.
  if (have_extra && pos_tg >= 0 && (size_t)(pos_tg + 4) <= len) {
    uint32_t tg = (uint32_t)data[pos_tg]
                | ((uint32_t)data[pos_tg + 1] << 8)
                | ((uint32_t)data[pos_tg + 2] << 16)
                | ((uint32_t)data[pos_tg + 3] << 24);
    M21->target_m3 = (float)tg / 1000.0f;
  }
  if (have_extra && pos_ft >= 0 && (size_t)pos_ft < len) {
    M21->flow_temp_c = (int8_t)data[pos_ft];
  }
  if (have_extra && pos_at >= 0 && (size_t)pos_at < len) {
    M21->ambient_temp_c = (int8_t)data[pos_at];
  }

  M21->have_data    = true;
  M21->frames_valid++;
  M21->last_valid_ms = millis();
}

static void M21HandleFrame(uint8_t length, const uint8_t *payload) {
  // Log every frame's meter id so the user can identify their meter
  AddLog(LOG_LEVEL_DEBUG, PSTR("M21: RX id=%02X%02X%02X%02X len=%u rssi=%d dBm"),
         payload[6], payload[5], payload[4], payload[3],
         (unsigned)length, (int)M21->last_rssi_dbm);
  // Verify meter id (payload[3..6] vs cfg meter_id, MSB-first vs LSB-first match)
  for (uint8_t i = 0; i < 4; i++) {
    if (M21->cfg.meter_id[i] != payload[6 - i]) {
      return;                                  // not our meter
    }
  }
  if (length < 18) { return; }

  uint8_t cipher_len = length - 2 - 16;
  if (cipher_len > M21_FRAME_MAX) { return; }

  // Dump the unencrypted wM-Bus header (first 16 bytes) so the actual frame
  // layout of the user's meter variant can be inspected with Weblog 4.
  char hex[16 * 2 + 1];
  for (uint8_t i = 0; i < 16 && i < length; i++) {
    static const char nib[] = "0123456789ABCDEF";
    hex[2 * i]     = nib[payload[i] >> 4];
    hex[2 * i + 1] = nib[payload[i] & 0x0F];
  }
  hex[2 * (length < 16 ? length : 16)] = 0;
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("M21: HDR %s cipherlen=%u"), hex, (unsigned)cipher_len);

  uint8_t buf[M21_FRAME_MAX];
  memcpy(buf, &payload[16], cipher_len);
  M21AesCtrDecrypt(payload, buf, cipher_len);

  // Dump the decrypted payload as hex
  char dhex[M21_FRAME_MAX * 2 + 1];
  uint8_t dn = cipher_len < M21_FRAME_MAX ? cipher_len : M21_FRAME_MAX;
  for (uint8_t i = 0; i < dn; i++) {
    static const char nib[] = "0123456789ABCDEF";
    dhex[2 * i]     = nib[buf[i] >> 4];
    dhex[2 * i + 1] = nib[buf[i] & 0x0F];
  }
  dhex[2 * dn] = 0;
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("M21: DEC %s"), dhex);

  M21ParsePlain(buf, cipher_len);
}

static void M21Receive(void) {
  uint8_t p1 = M21ReadFifoByte();
  uint8_t p2 = M21ReadFifoByte();
  uint8_t length = M21ReadFifoByte();

  if (length >= M21_FRAME_MAX || p1 != 0x54 || p2 != 0x3D) {
    M21StartReceiver();
    return;
  }

  uint8_t payload[M21_FRAME_MAX];
  for (uint8_t i = 0; i < length; i++) {
    payload[i] = M21ReadFifoByte();
  }

  // CC1101 RSSI register -> dBm (offset 74, see TI DN505)
  uint8_t raw_rssi = M21ReadReg(M21_CC_RSSI, M21_CC_STATUS_REGISTER);
  int16_t rssi_dbm = (raw_rssi >= 128) ? ((int16_t)raw_rssi - 256) / 2 - 74
                                       : (int16_t)raw_rssi / 2 - 74;
  M21->last_rssi_dbm = (int8_t)rssi_dbm;
  M21->frames_total++;

  if (M21->configured) {
    M21HandleFrame(length, payload);
  }

  M21StartReceiver();
}

/*-------------------------------------------------------------------------------------------*\
 * Persistent configuration
 *
 * The driver no longer relies on the LittleFS filesystem (which is unavailable
 * on the 1MB ESP8266 Tasmota build). Instead we use Tasmota's built-in
 * persistent Mem slots, which survive every reboot without requiring USE_UFILESYS:
 *   Mem1 = AES key (32 hex characters)
 *   Mem2 = meter ID (8 hex characters)
 *   Mem3 = meter type (decimal: 0 = Multical21, 1 = FlowIQ2200)
 *   Mem4 = telemetry period in seconds (decimal)
 * The user can therefore inspect or change the configuration either with the
 * M21* commands or directly with `Mem1 <hex>`, `Mem` (list) etc.
\*-------------------------------------------------------------------------------------------*/

#define M21_SLOT_KEY     (SET_MEM1)
#define M21_SLOT_ID      (SET_MEM2)
#define M21_SLOT_TYPE    (SET_MEM3)
#define M21_SLOT_PERIOD  (SET_MEM4)

static void M21UpdateConfigured(void) {
  bool key_set = false;
  for (uint8_t i = 0; i < 16; i++) {
    if (M21->cfg.aes_key[i]) { key_set = true; break; }
  }
  bool id_set = false;
  for (uint8_t i = 0; i < 4; i++) {
    if (M21->cfg.meter_id[i]) { id_set = true; break; }
  }
  M21->configured = key_set && id_set;
}

// Forward declarations (helpers defined later in the file)
static int  M21HexVal(char c);
static bool M21ParseHex(const char *s, uint8_t *out, size_t nbytes);
static void M21BytesToHex(const uint8_t *in, size_t n, char *out);

static void M21SaveCfg(void) {
  M21->cfg.magic   = M21_CFG_MAGIC;
  M21->cfg.version = M21_CFG_VERSION;

  char buf[33];
  M21BytesToHex(M21->cfg.aes_key, 16, buf);  buf[32] = 0;
  SettingsUpdateText(M21_SLOT_KEY, buf);
  M21BytesToHex(M21->cfg.meter_id, 4, buf);  buf[8]  = 0;
  SettingsUpdateText(M21_SLOT_ID, buf);
  snprintf_P(buf, sizeof(buf), PSTR("%u"), (unsigned)M21->cfg.meter_type);
  SettingsUpdateText(M21_SLOT_TYPE, buf);
  snprintf_P(buf, sizeof(buf), PSTR("%u"), (unsigned)M21->cfg.telemetry_period);
  SettingsUpdateText(M21_SLOT_PERIOD, buf);

  AddLog(LOG_LEVEL_INFO, PSTR("M21: SaveCfg -> Mem1..Mem4"));
  M21UpdateConfigured();
}

static void M21LoadCfg(void) {
  memset(&M21->cfg, 0, sizeof(M21Config));
  M21->cfg.magic   = M21_CFG_MAGIC;
  M21->cfg.version = M21_CFG_VERSION;

  const char *s;

  s = SettingsText(M21_SLOT_KEY);
  if (s && strlen(s) == 32) { M21ParseHex(s, M21->cfg.aes_key, 16); }

  s = SettingsText(M21_SLOT_ID);
  if (s && strlen(s) == 8)  { M21ParseHex(s, M21->cfg.meter_id, 4); }

  s = SettingsText(M21_SLOT_TYPE);
  if (s && *s) { M21->cfg.meter_type = (uint8_t)strtoul(s, nullptr, 10); }

  s = SettingsText(M21_SLOT_PERIOD);
  if (s && *s) { M21->cfg.telemetry_period = (uint16_t)strtoul(s, nullptr, 10); }

  AddLog(LOG_LEVEL_INFO, PSTR("M21: LoadCfg from Mem1..Mem4 (id=%s period=%u)"),
         SettingsText(M21_SLOT_ID), (unsigned)M21->cfg.telemetry_period);
  M21UpdateConfigured();
}

/*-------------------------------------------------------------------------------------------*\
 * Hex helpers
\*-------------------------------------------------------------------------------------------*/

static int M21HexVal(char c) {
  if (c >= '0' && c <= '9') { return c - '0'; }
  if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
  if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
  return -1;
}

static bool M21ParseHex(const char *s, uint8_t *out, size_t nbytes) {
  if (!s) { return false; }
  if (strlen(s) != nbytes * 2) { return false; }
  for (size_t i = 0; i < nbytes; i++) {
    int hi = M21HexVal(s[2 * i]);
    int lo = M21HexVal(s[2 * i + 1]);
    if (hi < 0 || lo < 0) { return false; }
    out[i] = (uint8_t)((hi << 4) | lo);
  }
  return true;
}

static void M21BytesToHex(const uint8_t *in, size_t n, char *out) {
  static const char hex[] = "0123456789ABCDEF";
  for (size_t i = 0; i < n; i++) {
    out[2 * i]     = hex[in[i] >> 4];
    out[2 * i + 1] = hex[in[i] & 0x0F];
  }
  out[2 * n] = 0;
}

/*-------------------------------------------------------------------------------------------*\
 * Init
\*-------------------------------------------------------------------------------------------*/

// Allocate the state struct and load persisted settings. Always called - even
// when no GPIOs are configured yet - so that M21* commands are usable from the
// console right after flashing.
static void M21AllocState(void) {
  if (M21) { return; }
  M21 = (M21State*)calloc(1, sizeof(M21State));
  if (!M21) { return; }
  M21->pin_cs   = -1;
  M21->pin_gdo0 = -1;
  M21LoadCfg();
}

static void M21Init(void) {
  M21AllocState();
  if (!M21) { return; }
  if (M21->hw_ok) { return; }                        // already up
  if (!PinUsed(GPIO_CC1101_GDO0)) { return; }
  if (TasmotaGlobal.spi_enabled != SPI_MOSI_MISO) { return; }
  if (!PinUsed(GPIO_SPI_CS)) { return; }

  M21->pin_cs   = Pin(GPIO_SPI_CS);
  M21->pin_gdo0 = Pin(GPIO_CC1101_GDO0);

  pinMode(M21->pin_cs, OUTPUT);
  digitalWrite(M21->pin_cs, HIGH);
  pinMode(M21->pin_gdo0, INPUT);

  // SPI bus may have been initialised already by another driver (display, SD,
  // MFRC522, ...). SPI.begin() is idempotent and does not reset the bus,
  // so we do not disturb existing peripherals. Per-transfer settings
  // (frequency / mode) are applied via SPI.beginTransaction() in every helper.
#ifdef ESP8266
  SPI.begin();                            // ESP8266: fixed HW pins SCK=GPIO14 (D5), MISO=GPIO12 (D6), MOSI=GPIO13 (D7)
#else
  SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif

  bool ok = false;
  for (uint8_t attempt = 0; attempt < 3 && !ok; attempt++) {
    ok = M21Reset();
    if (!ok) { delay(20); }
  }
  if (!ok) {
    AddLog(LOG_LEVEL_INFO, PSTR("M21: CC1101 reset failed"));
    return;
  }

  M21LoadDefaults();
  M21Strobe(M21_CC_SCAL);
  delay(1);

  attachInterrupt(digitalPinToInterrupt(M21->pin_gdo0), M21Gdo0Isr, FALLING);
  M21StartReceiver();

  M21->hw_ok = true;
  AddLog(LOG_LEVEL_INFO, PSTR("M21: CC1101 ready on CS=%d GDO0=%d, cfg=%s"),
         M21->pin_cs, M21->pin_gdo0, M21->configured ? PSTR("ok") : PSTR("missing"));
}

static void M21Poll(void) {
  if (!M21 || !M21->hw_ok) { return; }
  if (!M21_packet_flag) { return; }

  detachInterrupt(digitalPinToInterrupt(M21->pin_gdo0));
  M21_packet_flag = false;
  M21Receive();
  attachInterrupt(digitalPinToInterrupt(M21->pin_gdo0), M21Gdo0Isr, FALLING);
}

/*-------------------------------------------------------------------------------------------*\
 * Show / publish
\*-------------------------------------------------------------------------------------------*/

#define M21_LABEL                      "Multical21"

static void M21Show(bool json) {
  if (!M21) { return; }
  // JSON: only when we already have at least one valid frame (otherwise we
  // would publish all-zero values via telemetry).
  if (json && !M21->have_data) { return; }

  if (json) {
    ResponseAppend_P(PSTR(",\"" M21_LABEL "\":{\"Total\":%3_f,\"Target\":%3_f,"
                         "\"Flow\":%d,\"Ambient\":%d,\"Rssi\":%d}"),
                    &M21->total_m3, &M21->target_m3,
                    M21->flow_temp_c, M21->ambient_temp_c, M21->last_rssi_dbm);
#ifdef USE_WEBSERVER
  } else {
    // Web UI: always show, even before first frame, so the user sees the
    // sensor on the main page (waiting state).
    if (M21->have_data) {
      WSContentSend_PD(PSTR("{s}" M21_LABEL " " D_VOLUME "{m}%3_f " D_UNIT_CUBIC_METER "{e}"),
                      &M21->total_m3);
      WSContentSend_PD(PSTR("{s}" M21_LABEL " Target{m}%3_f " D_UNIT_CUBIC_METER "{e}"),
                      &M21->target_m3);
      WSContentSend_PD(PSTR("{s}" M21_LABEL " Flow{m}%d " D_UNIT_DEGREE D_UNIT_CELSIUS "{e}"),
                      M21->flow_temp_c);
      WSContentSend_PD(PSTR("{s}" M21_LABEL " Ambient{m}%d " D_UNIT_DEGREE D_UNIT_CELSIUS "{e}"),
                      M21->ambient_temp_c);
      WSContentSend_PD(PSTR("{s}" M21_LABEL " RSSI{m}%d dBm{e}"),
                      M21->last_rssi_dbm);
      WSContentSend_PD(PSTR("{s}" M21_LABEL " Frames{m}%u / %u{e}"),
                      (unsigned)M21->frames_valid, (unsigned)M21->frames_total);
    } else if (M21->hw_ok) {
      WSContentSend_PD(PSTR("{s}" M21_LABEL "{m}waiting for frame "
                            "(%u rcvd){e}"), (unsigned)M21->frames_total);
    } else {
      WSContentSend_PD(PSTR("{s}" M21_LABEL "{m}hardware off{e}"));
    }
#endif  // USE_WEBSERVER
  }
}

static void M21EverySecond(void) {
  if (!M21 || !M21->have_data) { return; }
  uint16_t period = M21->cfg.telemetry_period;
  if (!period) { return; }                      // 0 -> only via TelePeriod

  uint32_t now = millis();
  if ((int32_t)(M21->last_valid_ms - M21->last_publish_ms) <= 0) { return; }
  if ((now - M21->last_publish_ms) < ((uint32_t)period * 1000)) { return; }

  M21->last_publish_ms = now;
  ResponseClear();
  ResponseAppendTime();
  M21Show(true);
  ResponseJsonEnd();
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
}

/*-------------------------------------------------------------------------------------------*\
 * Commands
\*-------------------------------------------------------------------------------------------*/

#define D_PRFX_M21                     "M21"
#define D_CMND_M21_KEY                 "Key"
#define D_CMND_M21_ID                  "Id"
#define D_CMND_M21_TYPE                "Type"
#define D_CMND_M21_PERIOD              "Period"
#define D_CMND_M21_INFO                "Info"

const char kM21Commands[] PROGMEM = D_PRFX_M21 "|"
  D_CMND_M21_KEY "|" D_CMND_M21_ID "|" D_CMND_M21_TYPE "|" D_CMND_M21_PERIOD "|" D_CMND_M21_INFO;

static void M21CmndKey(void);
static void M21CmndId(void);
static void M21CmndType(void);
static void M21CmndPeriod(void);
static void M21CmndInfo(void);

void (* const M21Command[])(void) PROGMEM = {
  &M21CmndKey, &M21CmndId, &M21CmndType, &M21CmndPeriod, &M21CmndInfo
};

static void M21CmndKey(void) {
  if (!M21) { ResponseCmndError(); return; }
  if (XdrvMailbox.data_len > 0) {
    uint8_t buf[16];
    if (!M21ParseHex(XdrvMailbox.data, buf, 16)) {
      ResponseCmndError();
      return;
    }
    memcpy(M21->cfg.aes_key, buf, 16);
    M21SaveCfg();
  }
  // Return masked status only (do not leak the key)
  bool key_set = false;
  for (uint8_t i = 0; i < 16; i++) {
    if (M21->cfg.aes_key[i]) { key_set = true; break; }
  }
  Response_P(PSTR("{\"" D_PRFX_M21 D_CMND_M21_KEY "\":\"%s\"}"),
             key_set ? PSTR("set") : PSTR("missing"));
}

static void M21CmndId(void) {
  if (!M21) { ResponseCmndError(); return; }
  if (XdrvMailbox.data_len > 0) {
    uint8_t buf[4];
    if (!M21ParseHex(XdrvMailbox.data, buf, 4)) {
      ResponseCmndError();
      return;
    }
    memcpy(M21->cfg.meter_id, buf, 4);
    M21SaveCfg();
  }
  char hex[9];
  M21BytesToHex(M21->cfg.meter_id, 4, hex);
  Response_P(PSTR("{\"" D_PRFX_M21 D_CMND_M21_ID "\":\"%s\"}"), hex);
}

static void M21CmndType(void) {
  if (!M21) { ResponseCmndError(); return; }
  if (XdrvMailbox.data_len > 0) {
    uint32_t v = strtoul(XdrvMailbox.data, nullptr, 10);
    if (v > 1) { ResponseCmndError(); return; }
    M21->cfg.meter_type = (uint8_t)v;
    M21SaveCfg();
  }
  Response_P(PSTR("{\"" D_PRFX_M21 D_CMND_M21_TYPE "\":%d}"), M21->cfg.meter_type);
}

static void M21CmndPeriod(void) {
  if (!M21) { ResponseCmndError(); return; }
  if (XdrvMailbox.data_len > 0) {
    int32_t v = strtol(XdrvMailbox.data, nullptr, 10);
    if (v < 0 || v > 3600) { ResponseCmndError(); return; }
    M21->cfg.telemetry_period = (uint16_t)v;
    M21SaveCfg();
  }
  Response_P(PSTR("{\"" D_PRFX_M21 D_CMND_M21_PERIOD "\":%d}"),
             M21->cfg.telemetry_period);
}

static void M21CmndInfo(void) {
  if (!M21) { ResponseCmndError(); return; }
  char hexid[9];
  M21BytesToHex(M21->cfg.meter_id, 4, hexid);
  Response_P(PSTR("{\"" D_PRFX_M21 D_CMND_M21_INFO "\":{"
                  "\"Hw\":\"%s\",\"Configured\":\"%s\","
                  "\"" D_CMND_M21_ID "\":\"%s\",\"" D_CMND_M21_TYPE "\":%d,"
                  "\"" D_CMND_M21_PERIOD "\":%d,"
                  "\"Frames\":%u,\"Valid\":%u,\"Rssi\":%d}}"),
             M21->hw_ok ? PSTR("ok") : PSTR("off"),
             M21->configured ? PSTR("ok") : PSTR("missing"),
             hexid, M21->cfg.meter_type, M21->cfg.telemetry_period,
             (unsigned)M21->frames_total, (unsigned)M21->frames_valid,
             M21->last_rssi_dbm);
}

/*-------------------------------------------------------------------------------------------*\
 * Interface
\*-------------------------------------------------------------------------------------------*/

bool Xsns121(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      M21Init();
      break;
    case FUNC_EVERY_50_MSECOND:
      M21Poll();
      break;
    case FUNC_EVERY_SECOND:
      M21EverySecond();
      break;
    case FUNC_JSON_APPEND:
      M21Show(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      M21Show(false);
      break;
#endif  // USE_WEBSERVER
    case FUNC_COMMAND:
      if (M21) {
        result = DecodeCommand(kM21Commands, M21Command);
      }
      break;
    case FUNC_ACTIVE:
      result = (M21 != nullptr);
      break;
  }
  return result;
}

#endif  // USE_MULTICAL21
