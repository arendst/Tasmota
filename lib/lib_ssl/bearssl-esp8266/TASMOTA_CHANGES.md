# Tasmota changes to BearSSL

This library is BearSSL by Thomas Pornin, as adapted for ESP8266 by Earle F. Philhower
([bearssl-esp8266](https://github.com/earlephilhower/bearssl-esp8266)), with Tasmota changes on top.
Every Tasmota change in `src/` is marked with a `// TASMOTA` comment (`grep -rn TASMOTA src`).
Files that are new are marked `// TASMOTA: new file` on their first line.

Not marked, because they are mechanical: headers are renamed with a `t_` prefix
(`bearssl.h` → `t_bearssl.h`, `inner.h` → `t_inner.h`, `config.h` → `t_config.h`) so that they don't
collide with the BearSSL copy inside the ESP8266 Arduino core (see `bearssl_esp8266-customized.txt`).

Sizes below are flash bytes of a full firmware (`tasmota-4M-custom` for ESP8266, `tasmota32` for ESP32),
unless stated otherwise. Speed figures are instruction counts per hashed block taken from the
disassembly, or estimates from an instruction-level emulator: they are relative, not measured on a device.

## Configuration

| Setting | Value | File |
|---|---|---|
| `BR_SLOW_MUL15` | 1: Karatsuba multiplication for P-256 and Curve25519, smaller code | `t_bearssl_tasmota_config.h` |
| `BR_MAX_RSA_SIZE` | 2048 (4096 with `-DUSE_4K_RSA`) | `t_bearssl_tasmota_config.h` |
| `BR_MAX_EC_SIZE` | 384: P-256 and P-384, no P-521 | `t_bearssl_tasmota_config.h` |

## ESP32 support

- `pgmspace_bearssl.h` (new): `PROGMEM` / `pgm_read_*` wrappers that also work on ESP32.
- `t_inner.h`: `BR_LOMUL` and the hardware random generator (`BR_USE_ESP8266_RAND`) also on ESP32;
  `stack_thunk_yield()` is a no-op and `memcpy_P` is `memcpy` on ESP32.
- `t_inner.h`: `BR_USE_UNIX_TIME` is off on ESP8266, since Tasmota passes the time explicitly
  (`br_x509_minimal_set_time()`) from its own NTP client.
- `chacha20_ct.c`: no `PROGMEM` for the constants on ESP32.

## Hardware acceleration on ESP32 (new files)

- `ec/_ec_p256_m15.c`, `ec/_ec_c25519_m15.c`: P-256 and Curve25519 using the ESP32 ROM big-integer
  (MPI) accelerator, when built with `USE_SHA_ROM` and `SOC_MPI_SUPPORTED`.
- `hash/_sha_hal_idf5x.c`: SHA-1/224/256/384/512 using the ESP32 SHA peripheral, when built with
  `USE_SHA_ROM` on chips whose SHA engine can resume from a saved state (`SOC_SHA_SUPPORT_RESUME`:
  all ESP32 variants except the original ESP32).

## Security

- `ec_all_m15.c`, `ec_prime_i15.c`: only advertise the curves that fit in `BR_MAX_EC_SIZE`. The i15
  big-integer buffers are sized from it; advertising P-521 let a peer pick it and overflow those stack
  buffers. With `BR_MAX_EC_SIZE` 384, a P-521 server key or certificate is rejected cleanly
  (`BR_ERR_X509_BAD_SIGNATURE`, 52).

## Size reductions

### SHA-384 / SHA-512 (`hash/sha2big.c`)

One SHA-512 step per loop iteration instead of 8 unrolled steps, and a 16-word circular message
schedule instead of `w[80]`.

| | ESP8266 | ESP32 |
|---|---|---|
| `sha2big.c` code + tables (object file) | 4,500 → 2,436 | 4,488 → 2,471 |
| Firmware | see "ESP8266 symbol renames" | −2,044 |
| Stack of the compression function | 784 → 288 | 800 → 288 |
| Time per 128-byte block (emulator estimate) | 181 → 216 µs at 80 MHz (+19%) | 95 → 130 µs at 160 MHz (+37%) |

Only the TLS handshake uses SHA-384 (certificates signed with SHA-384), about 20 blocks per handshake.
Further reductions were tried and rejected because they were slower: 32-bit (hi, lo) arithmetic
(−250 bytes, +21–28% time) and a plain 64-round loop for SHA-256 (+55–72% time).

SHA-384 itself is optional on the Tasmota side: `#undef USE_MQTT_TLS_SHA384` (`my_user_config.h`)
removes it from the TLS client (ESP8266 −5.5 KB, ESP32 about −2.3 KB), at the cost of rejecting
certificates signed with SHA-384, such as LetsEncrypt ISRG Root X2 and its E5..E9 intermediates.

### SHA-224 / SHA-256 (`hash/sha2small.c`)

- 4 unrolled steps per loop iteration instead of 8: object 1,938 → 1,592 (ESP8266),
  1,794 → 1,446 (ESP32). Same speed (3,399 instructions per block on ESP8266, +1.5% on ESP32).
  This is the code used on ESP8266, and on ESP32 chips when neither the SHA hardware nor the
  mbedtls path below applies.
- ESP32: the compression function is taken from mbedtls (`mbedtls_internal_sha256_process`), which
  ESP-IDF always links for OTA image checks (`bootloader_support`). This removes BearSSL's own
  compression function and `K[]` table: −852 bytes on the original ESP32. Only when mbedtls uses its
  generic implementation (`MBEDTLS_SHA256_ALT` not defined, i.e. `CONFIG_MBEDTLS_HARDWARE_SHA` off);
  otherwise the BearSSL code is used. Cost: about 10–20% more time per block (3,690 instructions plus
  16 ROM calls, against about 3,500; 572 memory accesses against 441).
  See "ESP32 variants" for the other chips.

### GHASH for AES-GCM (`t_bearssl_hash.h`)

New macro `br_ghash_tasmota`: `br_ghash_ctmul` on ESP32, `br_ghash_ctmul32` on ESP8266. ESP32 cores have
a 32×32→64-bit multiply instruction, so `ctmul` is smaller (1,401 → 1,071 bytes) and faster (main loop
386 → 312 instructions, 20 → 9 calls). Used by the TLS client, Berry `AES_GCM` and WebRTC.

### P-521 tables (`ec_prime_i15.c`, `ec_secp521r1.c`, `ecdsa_i15_*_raw.c`)

Compiled only if `BR_MAX_EC_SIZE >= 521`: about −450 bytes on ESP32 (with the GHASH change: −856 in total).
`br_ec_prime_i15` no longer advertises P-521 either.

### ESP8266 symbol renames (`t_bearssl_hash.h`, `t_bearssl_prf.h`, `t_bearssl_ec.h`)

The ESP8266 Arduino core ships its own BearSSL (`libbearssl.a`) with the same symbol names, and the
linker took SHA-224/256/384/512, the TLS 1.2 SHA-256 PRF and the EC implementations from the core,
not from this library. On ESP8266 only, these symbols are renamed (`t_br_...`) so that this library's
smaller code is used:

| Renamed | ESP8266 flash | Notes |
|---|---|---|
| SHA-384/512 | −3,012 (IRAM +92) | the core copy was 8 steps unrolled |
| `br_ec_all_m15`, `br_ec_p256_m15`, `br_ec_c25519_m15` | −3,052 | `BR_SLOW_MUL15`; P-521 no longer accepted, like ESP32 |
| SHA-224/256 and `br_tls12_sha256_prf` | −700 | the core SHA-256 needed 3,942 instructions per block, ours 3,399 |
| `br_tls12_sha384_prf` | −5,368 when the `*_SHA384` cipher suites are enabled | otherwise the core PRF pulls in the core SHA-384 (unrolled) next to ours |

Not done on ESP32, where there is no second copy (and ESP-Mail-Client uses the plain names).
The speed of the `BR_SLOW_MUL15` curve code on ESP8266 has not been measured.

### Shared inline helpers on ESP8266 (`t_inner.h`, `codec/shared_inline.c`)

`br_enc32be`, `br_dec32be`, `br_enc64be`, `br_i15_zero`, `NOT`, `EQ`, `GT` are C99 `inline` instead of
`static inline` on ESP8266, with one out-of-line copy in `codec/shared_inline.c` instead of up to 7.
Same inlining decisions, so same speed: −156 bytes flash, −92 bytes IRAM. On ESP32 (LTO) the same change
made the firmware 60 bytes larger, so it is not applied there.

## Totals (this round of changes)

Full firmware, default build options, `USE_MQTT_TLS_SHA384` on (SHA-384 kept):

| Change | ESP8266 `tasmota-4M-custom` | ESP32 `tasmota32` |
|---|---|---|
| SHA-384/512 loop + 16-word schedule | −3,012 (with rename) | −2,044 |
| EC renames (`BR_SLOW_MUL15` code) | −3,052 | 0 |
| SHA-224/256 ×4 unrolling (+ rename on ESP8266) | −700 | −384 |
| GHASH `ctmul` + P-521 tables | 0 | −856 |
| Shared inline helpers | −424 | 0 |
| SHA-256 compression from mbedtls | 0 | −852 |
| **Total** | **−7,188** (694,754 → 687,566) | **−4,136** (≈2,258,450 → 2,254,314) |
| RAM | −32 (41,760 → 41,728) | −8 |
| IRAM | 0 (30,149) | 0 |

## ESP32 variants

Each variant built twice, with the mbedtls SHA-256 path on (B) and forced off (A):

| Env | Chip | SHA-256 used | Flash (B) | Flash (A) | Difference |
|---|---|---|---|---|---|
| `tasmota32` | ESP32 (Xtensa LX6) | mbedtls software compression | 2,254,314 | 2,255,166 | −852 |
| `tasmota32s3` | ESP32-S3 (Xtensa LX7) | SHA hardware (`_sha_hal_idf5x.c`) | 2,150,063 | 2,150,063 | 0 |
| `tasmota32c3` | ESP32-C3 (RISC-V) | SHA hardware | 2,268,174 | 2,268,174 | 0 |
| `tasmota32c6` | ESP32-C6 (RISC-V) | SHA hardware | 2,345,408 | 2,345,408 | 0 |
| `tasmota32p4` | ESP32-P4 (RISC-V) | SHA hardware | 2,250,484 | 2,250,484 | 0 |

All builds succeed. On S3, C3, C6 and P4, `USE_SHA_ROM` makes `_sha_hal_idf5x.c` provide SHA-1/224/256
(and SHA-384/512 where the hardware supports them), so `sha2small.c` is not linked and the mbedtls path
has no effect. It would only apply on those chips if `USE_SHA_ROM` is removed (ESP-IDF older than
5.5.4, see `pio-tools/add_c_flags.py`); that case compiles but was not built here.

`CONFIG_MBEDTLS_HARDWARE_SHA` is off in the ESP-IDF libraries of every variant, so mbedtls uses its
generic code and `MBEDTLS_SHA256_ALT` is not defined. If a future ESP-IDF build turns it on,
`sha2small.c` falls back to the BearSSL code by itself.

## Tests

- Host (macOS, clang): whole library built with `-Wall`; BearSSL `testcrypto`: SHA-1/224/256/384/512,
  multihash, HMAC, HKDF, HMAC_DRBG, TLS PRF, GHASH (`ctmul`, `ctmul32`), GCM, CCM, AES small,
  P-256, Curve25519; NIST vectors for SHA-384/512; certificate chains P-256 / P-384 accepted,
  P-521 rejected. The mbedtls SHA-256 path is tested against mbedtls 3.6.6 (the ESP-IDF version).
- `testcrypto` RSA i15 fails at 4096 bits: expected, `BR_MAX_RSA_SIZE` is 2048.
- Not yet tested on a device.
