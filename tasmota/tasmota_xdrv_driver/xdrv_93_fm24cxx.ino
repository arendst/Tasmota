/*
  xdrv_93_fm24cxx.ino - External FRAM over I2C with console / berry R/W operations

  Copyright (C) 2026 by Martin Macák - HexaMaster <hexamaster@icloud.com>

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

#ifdef USE_I2C
#ifdef USE_FM24CXX

#define XDRV_93 93
#define XI2C_98 98

#ifndef FM24CXX_I2C_ADD
#define FM24CXX_I2C_ADD 0x57
#endif

#ifndef FM24CXX_BLOCK_SIZE
#define FM24CXX_BLOCK_SIZE 0
#endif

#ifndef FM24CXX_CAPACITY
#define FM24CXX_CAPACITY 8192 
#endif

#ifndef FM24CXX_I2C_CHUNK
#define FM24CXX_I2C_CHUNK 32
#endif

#ifndef FM24CXX_MAX_WRITE_BYTES
#define FM24CXX_MAX_WRITE_BYTES 4096
#endif

#ifndef FM24CXX_JSON_MAX_BYTES
#define FM24CXX_JSON_MAX_BYTES 4096
#endif

#ifndef FM24CXX_JSON_MAX_STRING
#define FM24CXX_JSON_MAX_STRING 4096
#endif

// TX payload must also include 2 address bytes in the same Wire transmission.
// Typical Wire TX buffer is 32 bytes -> 30 bytes payload + 2 bytes addr.
#define FM24CXX_TX_PAYLOAD_MAX  (FM24CXX_I2C_CHUNK > 2 ? (FM24CXX_I2C_CHUNK - 2) : 1)

struct {
  bool    detected = false;
  uint8_t address  = (uint8_t)FM24CXX_I2C_ADD;
  uint8_t bus      = 0;

  uint32_t total_bytes = (uint32_t)FM24CXX_CAPACITY;

  uint32_t block_size  = (uint32_t)FM24CXX_BLOCK_SIZE;  // 0 => whole chip
  uint16_t block_count = 1;

  uint32_t last_block_size = 0;  // if total_bytes not divisible
} fm24;

// ---------- Helpers ----------

static char* Fm24_JsonEscapeAlloc(const char *in, uint32_t len) {
  // worst-case: every char -> \u00XX (6 chars)
  uint32_t need = 0;
  for (uint32_t i = 0; i < len; i++) {
    uint8_t c = (uint8_t)in[i];
    if (c == '\"' || c == '\\' || c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t') need += 2;
    else if (c < 0x20) need += 6;   // control char -> \u00XX
    else need += 1;
  }

  char *out = (char*)malloc(need + 1);
  if (!out) return nullptr;

  static const char kHex[] = "0123456789ABCDEF";
  char *d = out;

  for (uint32_t i = 0; i < len; i++) {
    uint8_t c = (uint8_t)in[i];
    switch (c) {
      case '\"': *d++='\\'; *d++='\"'; break;
      case '\\': *d++='\\'; *d++='\\'; break;
      case '\b': *d++='\\'; *d++='b';  break;
      case '\f': *d++='\\'; *d++='f';  break;
      case '\n': *d++='\\'; *d++='n';  break;
      case '\r': *d++='\\'; *d++='r';  break;
      case '\t': *d++='\\'; *d++='t';  break;
      default:
        if (c < 0x20) {
          *d++='\\'; *d++='u'; *d++='0'; *d++='0';
          *d++=kHex[(c >> 4) & 0x0F];
          *d++=kHex[c & 0x0F];
        } else {
          *d++ = (char)c;
        }
        break;
    }
  }

  *d = 0;
  return out;
}

static void Fm24_RecalcBlocks(void) {
  uint32_t bs = fm24.block_size;

  if (bs == 0 || bs >= fm24.total_bytes) {
    fm24.block_size = fm24.total_bytes;
    fm24.block_count = 1;
    fm24.last_block_size = fm24.total_bytes;
    return;
  }

  uint32_t full = fm24.total_bytes / bs;
  uint32_t rem  = fm24.total_bytes % bs;

  fm24.block_count = (uint16_t)full;
  if (rem) fm24.block_count++;

  fm24.last_block_size = rem ? rem : bs;
}

static bool Fm24_GetBlockParams(uint32_t block, uint32_t *start, uint32_t *len) {
  if (!fm24.detected) return false;
  if (block >= fm24.block_count) return false;

  uint32_t s = block * fm24.block_size;
  uint32_t l = fm24.block_size;

  if ((block == (uint32_t)(fm24.block_count - 1)) && (fm24.last_block_size != fm24.block_size)) {
    l = fm24.last_block_size;
  }

  if (s + l > fm24.total_bytes) return false;
  *start = s;
  *len   = l;
  return true;
}

// Robust hex parser: accepts "AABBCC", "AA BB CC", "0xAA,0xBB,0xCC", etc.
static uint32_t Fm24_ParseHexBytes(const char *s, uint8_t *out, uint32_t out_max) {
  uint32_t n = 0;
  int hi = -1;

  while (*s && n < out_max) {

    // Skip 0x / 0X prefix as a whole token (do NOT consume '0' as a nibble)
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
      s += 2;
      continue;
    }

    char c = *s++;
    int v = -1;

    if (c >= '0' && c <= '9') v = c - '0';
    else if (c >= 'a' && c <= 'f') v = 10 + (c - 'a');
    else if (c >= 'A' && c <= 'F') v = 10 + (c - 'A');
    else continue; // separators

    if (hi < 0) hi = v;
    else {
      out[n++] = (uint8_t)((hi << 4) | v);
      hi = -1;
    }
  }

  return n;
}

static void Fm24_LogHexLine(uint32_t addr, const uint8_t *buf, uint32_t len) {
  AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: 0x%04X: %*_H"), (uint16_t)addr, (int)len, buf);
}

// ---------- Low-level I2C FRAM access (16-bit address, MSB first) ----------

static bool Fm24_I2cRead(uint16_t mem_addr, uint8_t *buf, uint32_t len) {

  TwoWire& myWire = I2cGetWire(fm24.bus);
  if (&myWire == nullptr) { return false; }

  uint32_t done = 0;
  while (done < len) {
    uint32_t chunk = len - done;
    if (chunk > FM24CXX_I2C_CHUNK) chunk = FM24CXX_I2C_CHUNK;

    // Set internal address pointer
    myWire.beginTransmission(fm24.address);
    myWire.write((uint8_t)(mem_addr >> 8));       // MSB
    myWire.write((uint8_t)(mem_addr & 0xFF));     // LSB
    if (myWire.endTransmission(true) != 0) {      // STOP
      return false;
    }

    // Read data
    if (chunk != (uint32_t)myWire.requestFrom((uint8_t)fm24.address, (uint8_t)chunk)) {
      // Drain
      while (myWire.available()) (void)myWire.read();
      return false;
    }

    for (uint32_t i = 0; i < chunk; i++) {
      if (!myWire.available()) return false;
      buf[done + i] = (uint8_t)myWire.read();
    }

    done += chunk;
    mem_addr = (uint16_t)(mem_addr + chunk);
    yield();
  }

  return true;
}

static bool Fm24_I2cWrite(uint16_t mem_addr, const uint8_t *buf, uint32_t len) {

  TwoWire& myWire = I2cGetWire(fm24.bus);
  if (&myWire == nullptr) { return false; }

  uint32_t done = 0;
  while (done < len) {
    uint32_t chunk = len - done;
    if (chunk > FM24CXX_TX_PAYLOAD_MAX) chunk = FM24CXX_TX_PAYLOAD_MAX;

    myWire.beginTransmission(fm24.address);
    myWire.write((uint8_t)(mem_addr >> 8));       // MSB
    myWire.write((uint8_t)(mem_addr & 0xFF));     // LSB
    for (uint32_t i = 0; i < chunk; i++) {
      myWire.write(buf[done + i]);
    }

    if (myWire.endTransmission(true) != 0) {
      return false;
    }

    done += chunk;
    mem_addr = (uint16_t)(mem_addr + chunk);
    yield();
  }

  return true;
}

static bool Fm24_WriteFill(uint16_t mem_addr, uint8_t fill, uint32_t len) {
  uint8_t tmp[FM24CXX_TX_PAYLOAD_MAX];
  memset(tmp, fill, sizeof(tmp));

  uint32_t done = 0;
  while (done < len) {
    uint32_t chunk = len - done;
    if (chunk > sizeof(tmp)) chunk = sizeof(tmp);

    if (!Fm24_I2cWrite((uint16_t)(mem_addr + done), tmp, chunk)) {
      return false;
    }
    done += chunk;
    yield();
  }
  return true;
}

// ---------- Detect ----------

static void Fm24_Detect(void) {
  if (fm24.detected) return;
  if (!I2cEnabled(XI2C_98)) return;

  fm24.address     = (uint8_t)FM24CXX_I2C_ADD;
  fm24.total_bytes = (uint32_t)FM24CXX_CAPACITY;
  fm24.block_size  = (uint32_t)FM24CXX_BLOCK_SIZE;
  Fm24_RecalcBlocks();

  for (fm24.bus = 0; fm24.bus < 2; fm24.bus++) {
    if (!I2cSetDevice(fm24.address, fm24.bus)) continue;

    uint8_t b = 0;
    fm24.detected = Fm24_I2cRead(0x0000, &b, 1);

    if (fm24.detected) {
      I2cSetActiveFound(fm24.address, "FM24CXX", fm24.bus);
      AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: detected bus=%d addr=0x%02X size=%u block=%u blocks=%u"),
            fm24.bus, fm24.address,
            (unsigned)fm24.total_bytes,
            (unsigned)fm24.block_size,
            (unsigned)fm24.block_count);
      return;
    }
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("FM24CXX: not detected (addr=0x%02X)"), (uint8_t)FM24CXX_I2C_ADD);
}

// ---------- Tasmota console command handling ----------

#define D_PRFX_FRAM             "Fram"
#define D_CMND_FRAM_INFO        "Info"              // General info (etc... {"FramInfo":{"bus":1,"addr":"0x50","size":65535,"block":1024,"blocks":64}})
#define D_CMND_FRAM_READ_FORMAT "ReadFormat"        // Read HEX values from block and format them in tasmota console (STREAM)
#define D_CMND_FRAM_READ        "Read"              // Read HEX values from block to result map (easy access from Berry)
#define D_CMND_FRAM_WRITE       "Write"             // Write HEX values to block. Unused block bytes are filled with zeros
#define D_CMND_FRAM_FORMAT      "Erase"             // Erase selected block or whole fram (if "all") - fill with zeros
#define D_CMND_FRAM_READ_STRING  "ReadString"       // Read block data as STRING - json output
#define D_CMND_FRAM_WRITE_STRING "WriteString"      // Write data to block as STRING. 

static void CmndFramInfo(void);
static void CmndFramReadFormat(void);
static void CmndFramRead(void);
static void CmndFramWrite(void);
static void CmndFramErase(void);
static void CmndFramReadString(void);
static void CmndFramWriteString(void);

const char kFramCommands[] PROGMEM =
  D_PRFX_FRAM "|" D_CMND_FRAM_INFO "|" D_CMND_FRAM_READ_FORMAT "|" D_CMND_FRAM_READ "|" D_CMND_FRAM_WRITE "|" D_CMND_FRAM_FORMAT "|" D_CMND_FRAM_READ_STRING "|" D_CMND_FRAM_WRITE_STRING;

void (*const FramCommand[])(void) PROGMEM = {
  &CmndFramInfo, &CmndFramReadFormat, &CmndFramRead, &CmndFramWrite, &CmndFramErase, &CmndFramReadString, &CmndFramWriteString
};

static void CmndFramInfo(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramInfo\":\"not detected\"}"));
    return;
  }

  Response_P(PSTR("{\"FramInfo\":{\"bus\":%d,\"addr\":\"0x%02X\",\"size\":%u,\"block\":%u,\"blocks\":%u}}"),
             fm24.bus, fm24.address,
             (unsigned)fm24.total_bytes,
             (unsigned)fm24.block_size,
             (unsigned)fm24.block_count);
}

static void CmndFramReadFormat(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramReadFormat\":\"not detected\"}"));
    return;
  }

  if (!XdrvMailbox.data_len) { ResponseCmndFailed(); return; }

  uint32_t block = (uint32_t)strtoul(XdrvMailbox.data, nullptr, 0);
  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) { ResponseCmndFailed(); return; }

  AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: READ block=%u start=0x%04X len=%u"),
         (unsigned)block, (uint16_t)start, (unsigned)len);

  uint8_t tmp[16];
  for (uint32_t off = 0; off < len; off += sizeof(tmp)) {
    uint32_t chunk = len - off;
    if (chunk > sizeof(tmp)) chunk = sizeof(tmp);

    if (!Fm24_I2cRead((uint16_t)(start + off), tmp, chunk)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: READ failed at 0x%04X"), (uint16_t)(start + off));
      ResponseCmndFailed();
      return;
    }
    Fm24_LogHexLine(start + off, tmp, chunk);
    yield();
  }

  Response_P(PSTR("{\"FramReadFormat\":{\"block\":%u,\"bytes\":%u}}"), (unsigned)block, (unsigned)len);
}

static void CmndFramRead(void) {
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramRead\":\"not detected\"}"));
    return;
  }

  char *p = XdrvMailbox.data;
  while (p && (*p == ' ' || *p == '\t' || *p == ',')) p++;

  if (!p || *p == 0) {
    Response_P(PSTR("{\"FramRead\":\"usage: FramRead <block>\"}"));
    return;
  }

  uint32_t block = (uint32_t)strtoul(p, nullptr, 0);

  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) {
    ResponseCmndFailed();
    return;
  }

  // buffer protection
  if (len > FM24CXX_JSON_MAX_BYTES) {
    Response_P(PSTR("{\"FramRead\":{\"error\":\"too big\",\"block\":%u,\"bytes\":%u,\"max\":%u}}"),
               (unsigned)block, (unsigned)len, (unsigned)FM24CXX_JSON_MAX_BYTES);
    return;
  }

  uint8_t *buf = (uint8_t*)malloc(len);
  if (!buf) { ResponseCmndFailed(); return; }

  if (!Fm24_I2cRead((uint16_t)start, buf, len)) {
    free(buf);
    ResponseCmndFailed();
    return;
  }

  char *hex = (char*)malloc((len * 2) + 1);
  if (!hex) { free(buf); ResponseCmndFailed(); return; }
  hex[len * 2] = 0;

  static const char kHex[] = "0123456789ABCDEF";
  for (uint32_t i = 0; i < len; i++) {
    uint8_t v = buf[i];
    hex[i * 2 + 0] = kHex[v >> 4];
    hex[i * 2 + 1] = kHex[v & 0x0F];
  }

  Response_P(PSTR("{\"FramRead\":{\"block\":%u,\"bytes\":%u,\"data\":\"%s\"}}"),
             (unsigned)block, (unsigned)len, hex);

  free(hex);
  free(buf);
}

static void CmndFramWrite(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramWrite\":\"not detected\"}"));
    return;
  }

  if (!XdrvMailbox.data_len) { ResponseCmndFailed(); return; }

  char *p = XdrvMailbox.data;
  while (p && (*p == ' ' || *p == '\t' || *p == ',')) p++;

  char *save = nullptr;
  char *a1 = strtok_r(p, " \t,", &save);
  if (!a1) { ResponseCmndFailed(); return; }

  uint32_t block = (uint32_t)strtoul(a1, nullptr, 0);
  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) { ResponseCmndFailed(); return; }

  char *hex = save;
  while (hex && (*hex == ' ' || *hex == '\t' || *hex == ',')) hex++;

  if (!hex || *hex == 0) {
    Response_P(PSTR("{\"FramWrite\":\"write needs hex payload\"}"));
    return;
  }

  uint32_t max_write = len;
  if (max_write > FM24CXX_MAX_WRITE_BYTES) max_write = FM24CXX_MAX_WRITE_BYTES;

  uint8_t *data = (uint8_t*)malloc(max_write);
  if (!data) { ResponseCmndFailed(); return; }

  uint32_t n = Fm24_ParseHexBytes(hex, data, max_write);
  if (n == 0) {
    free(data);
    Response_P(PSTR("{\"FramWrite\":\"no hex bytes parsed\"}"));
    return;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: WRITE block=%u start=0x%04X len=%u parsed=%u (fill rest with 0x00)"),
         (unsigned)block, (uint16_t)start, (unsigned)len, (unsigned)n);

  if (!Fm24_I2cWrite((uint16_t)start, data, n)) {
    free(data);
    AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: WRITE failed at 0x%04X"), (uint16_t)start);
    ResponseCmndFailed();
    return;
  }
  free(data);

  if (n < len) {
    if (!Fm24_WriteFill((uint16_t)(start + n), 0x00, (len - n))) {
      AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: FILL failed at 0x%04X"), (uint16_t)(start + n));
      ResponseCmndFailed();
      return;
    }
  }

  Response_P(PSTR("{\"FramWrite\":{\"block\":%u,\"bytes\":%u,\"filled\":%u}}"),
             (unsigned)block, (unsigned)n, (unsigned)(len - (n < len ? n : len)));
}

static void CmndFramErase(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramErase\":\"not detected\"}"));
    return;
  }

  if (!XdrvMailbox.data_len) { ResponseCmndFailed(); return; }

  char *p = XdrvMailbox.data;
  while (p && (*p == ' ' || *p == '\t' || *p == ',')) p++;

  if (!strcasecmp(p, "all")) {
    AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: FORMAT all blocks (%u), fill=0x00"), (unsigned)fm24.block_count);

    for (uint32_t b = 0; b < fm24.block_count; b++) {
      uint32_t start = 0, len = 0;
      if (!Fm24_GetBlockParams(b, &start, &len)) { ResponseCmndFailed(); return; }

      if (!Fm24_WriteFill((uint16_t)start, 0x00, len)) {
        AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: FORMAT failed at block=%u addr=0x%04X"), (unsigned)b, (uint16_t)start);
        ResponseCmndFailed();
        return;
      }
      yield();
    }

    Response_P(PSTR("{\"FramErase\":\"all\"}"));
    return;
  }

  uint32_t block = (uint32_t)strtoul(p, nullptr, 0);
  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) { ResponseCmndFailed(); return; }

  AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: FORMAT block=%u start=0x%04X len=%u fill=0x00"),
         (unsigned)block, (uint16_t)start, (unsigned)len);

  if (!Fm24_WriteFill((uint16_t)start, 0x00, len)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: FORMAT failed at 0x%04X"), (uint16_t)start);
    ResponseCmndFailed();
    return;
  }

  Response_P(PSTR("{\"FramErase\":%u}"), (unsigned)block);
}

static void CmndFramReadString(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramReadString\":\"not detected\"}"));
    return;
  }

  char *p = XdrvMailbox.data;
  while (p && (*p == ' ' || *p == '\t' || *p == ',')) p++;

  if (!p || *p == 0) {
    Response_P(PSTR("{\"FramReadString\":\"usage: FramReadString <block>\"}"));
    return;
  }

  uint32_t block = (uint32_t)strtoul(p, nullptr, 0);

  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) { ResponseCmndFailed(); return; }

  // chunked read
  uint32_t max_raw = len;
  if (max_raw > FM24CXX_JSON_MAX_STRING) max_raw = FM24CXX_JSON_MAX_STRING;

  char *raw = (char*)malloc(max_raw + 1);
  if (!raw) { ResponseCmndFailed(); return; }

  uint32_t raw_len = 0;
  bool zero_found = false;

  uint8_t tmp[FM24CXX_I2C_CHUNK];

  for (uint32_t off = 0; off < len && raw_len < max_raw && !zero_found; ) {
    uint32_t chunk = len - off;
    if (chunk > sizeof(tmp)) chunk = sizeof(tmp);

    if (!Fm24_I2cRead((uint16_t)(start + off), tmp, chunk)) {
      free(raw);
      ResponseCmndFailed();
      return;
    }

    for (uint32_t i = 0; i < chunk && raw_len < max_raw; i++) {
      uint8_t c = tmp[i];
      if (c == 0x00) { zero_found = true; break; }
      raw[raw_len++] = (char)c;
    }

    off += chunk;
    yield();
  }

  raw[raw_len] = 0;

  bool truncated = (!zero_found) && (raw_len == max_raw) && (max_raw < len);

  char *esc = Fm24_JsonEscapeAlloc(raw, raw_len);
  if (!esc) {
    free(raw);
    ResponseCmndFailed();
    return;
  }

  Response_P(PSTR("{\"FramReadString\":{\"block\":%u,\"bytes\":%u,\"truncated\":%u,\"data\":\"%s\"}}"),
             (unsigned)block, (unsigned)raw_len, (unsigned)(truncated ? 1 : 0), esc);

  free(esc);
  free(raw);
}

static void CmndFramWriteString(void) {
  Fm24_Detect();
  if (!fm24.detected) {
    Response_P(PSTR("{\"FramWriteString\":\"not detected\"}"));
    return;
  }

  if (!XdrvMailbox.data_len) { ResponseCmndFailed(); return; }

  char *p = XdrvMailbox.data;
  while (p && (*p == ' ' || *p == '\t' || *p == ',')) p++;

  char *save = nullptr;
  char *a1 = strtok_r(p, " \t,", &save);
  if (!a1) { ResponseCmndFailed(); return; }

  uint32_t block = (uint32_t)strtoul(a1, nullptr, 0);

  uint32_t start = 0, len = 0;
  if (!Fm24_GetBlockParams(block, &start, &len)) { ResponseCmndFailed(); return; }

  char *str = save;
  while (str && (*str == ' ' || *str == '\t' || *str == ',')) str++;

  if (!str || *str == 0) {
    Response_P(PSTR("{\"FramWriteString\":\"write needs string payload\"}"));
    return;
  }

  // quote string support ("")
  if (str[0] == '\"') {
    str++;
    char *endq = strrchr(str, '\"');
    if (endq) *endq = 0;
  }

  uint32_t max_write = len;
  if (max_write > FM24CXX_MAX_WRITE_BYTES) max_write = FM24CXX_MAX_WRITE_BYTES;

  uint32_t n = (uint32_t)strnlen(str, max_write);
  bool truncated = (str[n] != 0);   // strnlen hitol limit

  AddLog(LOG_LEVEL_INFO, PSTR("FM24CXX: WRITE_STRING block=%u start=0x%04X str_len=%u (fill rest with 0x00)"),
         (unsigned)block, (uint16_t)start, (unsigned)n);

  if (n > 0) {
    if (!Fm24_I2cWrite((uint16_t)start, (const uint8_t*)str, n)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: WRITE_STRING failed at 0x%04X"), (uint16_t)start);
      ResponseCmndFailed();
      return;
    }
  }

  if (n < len) {
    if (!Fm24_WriteFill((uint16_t)(start + n), 0x00, (len - n))) {
      AddLog(LOG_LEVEL_ERROR, PSTR("FM24CXX: WRITE_STRING fill failed at 0x%04X"), (uint16_t)(start + n));
      ResponseCmndFailed();
      return;
    }
  }

  Response_P(PSTR("{\"FramWriteString\":{\"block\":%u,\"bytes\":%u,\"filled\":%u,\"truncated\":%u}}"),
             (unsigned)block, (unsigned)n, (unsigned)(len - (n < len ? n : len)), (unsigned)(truncated ? 1 : 0));
}

// ---------- Interface ----------

bool Xdrv93(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      Fm24_Detect();
      break;

    case FUNC_COMMAND:
      result = DecodeCommand(kFramCommands, FramCommand);
      break;

    case FUNC_ACTIVE:
      result = fm24.detected;
      break;
  }

  return result;
}

#endif  // USE_FM24CXX
#endif  // USE_I2C
