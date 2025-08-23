/* _sha_rom_idf5x.c — BearSSL drop‑in with ESP32 ROM SHA acceleration
 *
 *  ⚠ TEST‑ONLY IMPLEMENTATION — SERIALIZED VIA GLOBAL SPINLOCK ⚠
 *
 *  Overview:
 *  ---------
 *  - Provides BearSSL hash functions backed by the ESP32 ROM SHA engine.
 *  - Uses a single global ROM SHA_CTX instance.
 *  - Access is serialized with a global portMUX spinlock.
 *
 *  Concurrency Warning:
 *  --------------------
 *  - This locking only prevents simultaneous entry into the ROM SHA engine.
 *  - It does NOT make multiple concurrent hash contexts safe.
 *  - Interleaving operations from different contexts will corrupt state.
 *  - Only one hash context may be active at any given time.
 *
 *  Platform Notes:
 *  ---------------
 *  - ESP32 (ESP_PLATFORM): Uses ROM SHA acceleration as described above.
 *  - ESP8266 / other targets: This translation unit compiles to nothing;
 *    stock BearSSL symbols are provided instead.
 *
 *  SHA‑256 Limitation on ESP32:
 *  ----------------------------
 *  - The ESP32 ROM SHA‑256 implementation shares internal state with the
 *    hardware engine; without pause/resume support, state cannot be saved
 *    and restored safely.
 *  - In this “single global context” model, running SHA‑256 concurrently
 *    or interleaving with other operations will corrupt the ROM state.
 *  - Therefore, SHA‑256 acceleration is only enabled when
 *    SOC_SHA_SUPPORT_RESUME is available; otherwise it is unsafe here.
 *  - When unsupported, SHA‑256 silently falls back to BearSSL’s software
 *    implementation at build time (callers see no behavioral change).
 *
 *  Supported ROM Algorithms (if present in ROM):
 *  ----------------------------------------------
 *    • SHA‑1
 *    • SHA‑224
 *    • SHA‑256  (see limitation and fallback above)
 *    • SHA‑384
 *    • SHA‑512
 */

#include "t_inner.h"

#if defined(USE_SHA_ROM)
#if defined(ESP_PLATFORM) && !defined(ESP8266)

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"

#if __has_include("soc/sha_caps.h")
# include "soc/sha_caps.h"
#elif __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No SHA capability header found"
#endif

#if __has_include("esp_rom_sha.h")
# include "esp_rom_sha.h"
#elif __has_include("rom/sha.h")
# include "rom/sha.h"
#else
# error "No ROM SHA header found"
#endif

#define HAVE_ROM_SHA1    (SOC_SHA_SUPPORT_SHA1)
#define HAVE_ROM_SHA224  (SOC_SHA_SUPPORT_SHA224)
#define HAVE_ROM_SHA256  (SOC_SHA_SUPPORT_SHA256)
#define HAVE_ROM_SHA384  (SOC_SHA_SUPPORT_SHA384)
#define HAVE_ROM_SHA512  (SOC_SHA_SUPPORT_SHA512)

#if defined(SOC_SHA_SUPPORT_RESUME)
/* Newer chips: mode passed to init, update has no mode */
#define ROM_SHA_ENABLE()                     ets_sha_enable()
#define ROM_SHA_DISABLE()                    ets_sha_disable()
#define ROM_SHA_INIT_MODE(ctx, mode)         ets_sha_init((ctx), (mode))
#define ROM_SHA_UPDATE_MODE(ctx, mode, d, l, last) \
                                             ets_sha_update((ctx), (const unsigned char*)(d), (uint32_t)(l), (last))
#define ROM_SHA_FINISH_MODE(ctx, mode, out)  ets_sha_finish((ctx), (unsigned char*)(out))
#define ROM_SHA_FINALIZE_MODE(ctx, mode, out) \
    do { ROM_SHA_UPDATE_MODE((ctx), mode, NULL, 0, true); ROM_SHA_FINISH_MODE((ctx), mode, (out)); } while (0)

#else
/* Old ESP32: mode passed to update/finish, init takes only ctx */
#define ROM_SHA_ENABLE()                     ets_sha_enable()
#define ROM_SHA_DISABLE()                    ets_sha_disable()
#define ROM_SHA_INIT_MODE(ctx, mode)         ets_sha_init((ctx))
#define ROM_SHA_UPDATE_MODE(ctx, mode, d, l, last) \
                                             ets_sha_update((ctx), (mode), (const uint8_t*)(d), (size_t)((l) * 8))
#define ROM_SHA_FINISH_MODE(ctx, mode, out)  ets_sha_finish((ctx), (mode), (uint8_t*)(out))
#define ROM_SHA_FINALIZE_MODE(ctx, mode, out) \
    do { ROM_SHA_UPDATE_MODE((ctx), mode, NULL, 0, true); ROM_SHA_FINISH_MODE((ctx), mode, (out)); } while (0)
#endif


/* One global ROM context; accesses serialized across cores/tasks. */
static SHA_CTX rom_ctx;
static portMUX_TYPE s_sha_mux = portMUX_INITIALIZER_UNLOCKED;
#define SHA_ENTER()  portENTER_CRITICAL(&s_sha_mux)
#define SHA_EXIT()   portEXIT_CRITICAL(&s_sha_mux)

/* ================================================================
 * SHA-1 (ROM path, no save/restore)
 * ================================================================ */
#if HAVE_ROM_SHA1
void br_sha1_init(br_sha1_context *cc) {
    SHA_ENTER();
    cc->vtable = &br_sha1_vtable;
    cc->count  = 0;
    ROM_SHA_ENABLE();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA1);
    SHA_EXIT();
}

void br_sha1_update(br_sha1_context *cc, const void *data, size_t len) {
    if (!len) return;
    SHA_ENTER();
    ROM_SHA_UPDATE_MODE(&rom_ctx, SHA1, data, len, false);
    cc->count += len;
    SHA_EXIT();
}

void br_sha1_out(const br_sha1_context *cc, void *out) {
    (void)cc;
    SHA_ENTER();
    ROM_SHA_FINALIZE_MODE(&rom_ctx, SHA1, out);
    ROM_SHA_INIT_MODE(&rom_ctx, SHA1);
    SHA_EXIT();
}

const br_hash_class br_sha1_vtable PROGMEM = {
    sizeof(br_sha1_context),
    BR_HASHDESC_ID(br_sha1_ID)
      | BR_HASHDESC_OUT(20)
      | BR_HASHDESC_LBLEN(6)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **)) &br_sha1_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha1_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha1_out,
    NULL,
    NULL
};
#endif

/* ================================================================
 * SHA-224 (ROM path, no save/restore)
 * ================================================================ */
/* ================================================================
 * SHA-224 (ROM path, no save/restore)
 * ================================================================ */
#if HAVE_ROM_SHA224
void br_sha224_init(br_sha224_context *cc) {
    SHA_ENTER();
    cc->vtable = &br_sha224_vtable;
    cc->count  = 0;
    ROM_SHA_ENABLE();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_224);
    SHA_EXIT();
}

void br_sha224_update(br_sha224_context *cc, const void *data, size_t len) {
    if (!len) return;
    SHA_ENTER();
    ROM_SHA_UPDATE_MODE(&rom_ctx, SHA2_224, data, len, false);
    cc->count += len;
    SHA_EXIT();
}

void br_sha224_out(const br_sha224_context *cc, void *out) {
    (void)cc;
    uint8_t tmp[32];
    SHA_ENTER();
    ROM_SHA_FINALIZE_MODE(&rom_ctx, SHA2_224, tmp);
    memcpy(out, tmp, 28);
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_224);
    SHA_EXIT();
}

const br_hash_class br_sha224_vtable PROGMEM = {
    sizeof(br_sha224_context),
    BR_HASHDESC_ID(br_sha224_ID)
      | BR_HASHDESC_OUT(28)
      | BR_HASHDESC_LBLEN(6)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **)) &br_sha224_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha224_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha224_out,
    NULL,
    NULL
};
#endif /* HAVE_ROM_SHA224 */


/* ================================================================
 * SHA-256 with ROM acceleration + per-blob replay save/restore
 * ================================================================ */
#if HAVE_ROM_SHA256 && defined(SOC_SHA_SUPPORT_RESUME)
#undef br_sha256_init
#undef br_sha256_update
#undef br_sha256_out
#undef br_sha256_state
#undef br_sha256_set_state

#define S256_CAP 64

/* Capture buffer for current context */
static struct {
    uint8_t  buf[S256_CAP];
    uint8_t  len;
} s256_capture;

void br_sha256_init(br_sha256_context *cc)
{
    SHA_ENTER();
    cc->vtable = &br_sha256_vtable;
    cc->count  = 0;
    s256_capture.len = 0;
    ROM_SHA_ENABLE();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_256);
    SHA_EXIT();
}

void br_sha256_update(br_sha256_context *cc, const void *data, size_t len)
{
    if (!len) return;
    const uint8_t *dp = (const uint8_t *)data;

    SHA_ENTER();
    ROM_SHA_UPDATE_MODE(&rom_ctx, SHA2_256, dp, len, false);
    cc->count += len;

    if (s256_capture.len < S256_CAP) {
        size_t want = S256_CAP - s256_capture.len;
        size_t take = (len < want) ? len : want;
        memcpy(s256_capture.buf + s256_capture.len, dp, take);
        s256_capture.len += (uint8_t)take;
    }
    SHA_EXIT();
}

void br_sha256_out(const br_sha256_context *cc, void *out)
{
    (void)cc;
    SHA_ENTER();
    ROM_SHA_FINALIZE_MODE(&rom_ctx, SHA2_256, out);
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_256);
    SHA_EXIT();
}

uint64_t br_sha256_state(const br_sha256_context *cc, void *dst)
{
    /* Store exactly the captured bytes in the blob */
    memset(dst, 0, S256_CAP);
    memcpy(dst, s256_capture.buf, s256_capture.len);
    return cc->count;
}

void br_sha256_set_state(br_sha256_context *cc, const void *src, uint64_t count)
{
    const uint8_t *sp = (const uint8_t *)src;

    SHA_ENTER();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_256);

    if (count <= S256_CAP) {
        ROM_SHA_UPDATE_MODE(&rom_ctx, SHA2_256, sp, (size_t)count, false);
        cc->count = count;
    } else {
        cc->count = 0;
        s256_capture.len = 0;
    }
    SHA_EXIT();
}

const br_hash_class br_sha256_vtable PROGMEM = {
    sizeof(br_sha256_context),
    BR_HASHDESC_ID(br_sha256_ID)
      | BR_HASHDESC_OUT(32)
      | BR_HASHDESC_STATE(S256_CAP)    /* store full ipad/opad block */
      | BR_HASHDESC_LBLEN(6)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **)) &br_sha256_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha256_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha256_out,
    (uint64_t (*)(const br_hash_class *const *, void *)) &br_sha256_state,
    (void (*)(const br_hash_class **, const void *, uint64_t)) &br_sha256_set_state
};
#endif /* HAVE_ROM_SHA256 */

/* ================================================================
 * SHA-384 (ROM path, no save/restore)
 * ================================================================ */
#if HAVE_ROM_SHA384
void br_sha384_init(br_sha384_context *cc) {
    SHA_ENTER();
    cc->vtable = &br_sha384_vtable;
    cc->count  = 0;
    ROM_SHA_ENABLE();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_384);
    SHA_EXIT();
}

void br_sha384_update(br_sha384_context *cc, const void *data, size_t len) {
    if (!len) return;
    SHA_ENTER();
    ROM_SHA_UPDATE_MODE(&rom_ctx, SHA2_384, data, len, false);
    cc->count += len;
    SHA_EXIT();
}

void br_sha384_out(const br_sha384_context *cc, void *out) {
    (void)cc;
    uint8_t tmp[64];
    SHA_ENTER();
    ROM_SHA_FINALIZE_MODE(&rom_ctx, SHA2_384, tmp);
    memcpy(out, tmp, 48);
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_384);
    SHA_EXIT();
}

const br_hash_class br_sha384_vtable PROGMEM = {
    sizeof(br_sha384_context),
    BR_HASHDESC_ID(br_sha384_ID)
      | BR_HASHDESC_OUT(48)
      | BR_HASHDESC_LBLEN(7)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **)) &br_sha384_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha384_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha384_out,
    NULL,
    NULL
};
#endif

/* ================================================================
 * SHA-512 (ROM path, no save/restore)
 * ================================================================ */
#if HAVE_ROM_SHA512
#undef br_sha512_update
void br_sha512_init(br_sha512_context *cc) {
    SHA_ENTER();
    cc->vtable = &br_sha512_vtable;
    cc->count  = 0;
    ROM_SHA_ENABLE();
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_512);
    SHA_EXIT();
}

void br_sha512_update(br_sha512_context *cc, const void *data, size_t len) {
    if (!len) return;
    SHA_ENTER();
    ROM_SHA_UPDATE_MODE(&rom_ctx, SHA2_512, data, len, false);
    cc->count += len;
    SHA_EXIT();
}

void br_sha512_out(const br_sha512_context *cc, void *out) {
    (void)cc;
    SHA_ENTER();
    ROM_SHA_FINALIZE_MODE(&rom_ctx, SHA2_512, out);
    ROM_SHA_INIT_MODE(&rom_ctx, SHA2_512);
    SHA_EXIT();
}

const br_hash_class br_sha512_vtable PROGMEM = {
    sizeof(br_sha512_context),
    BR_HASHDESC_ID(br_sha512_ID)
      | BR_HASHDESC_OUT(64)
      | BR_HASHDESC_LBLEN(7)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **)) &br_sha512_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha512_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha512_out,
    NULL,
    NULL
};
#endif

#else
/* ===== ESP8266 - leave it unchanged ===== */
#endif
#endif //USE_SHA_ROM