/*
 * _sha_hal_idf5x.c — BearSSL drop‑in with ESP32 HAL SHA acceleration
 *
 * This file provides hardware-accelerated implementations of BearSSL-compatible
 * hash functions (SHA-1, SHA-224, SHA-256, SHA-384, SHA-512) using the ESP-IDF 5.x
 * SHA HAL. It replaces the software digest core with direct access to the ESP32's
 * SHA engine, preserving BearSSL's context structure and API semantics.
 *
 * Each hash context uses a minimal layout:
 *   - val[] holds the midstate as a raw little-endian byte image.
 *   - buf[] holds the partial block (64 or 128 bytes).
 *   - count tracks the total input length in bytes.
 *
 * Endianness is preserved across state() and set_state() calls, allowing seamless
 * serialization and restoration of midstate snapshots. All conversions for hardware
 * interaction are localized to update() and out().
 *
 * This module is designed for drop-in replacement with no changes to BearSSL clients.
 * It supports multihash, midstate injection, and digest resumption.
 * 
 * Author: Christian Baars
 */

#include "t_inner.h"

#if defined(USE_SHA_ROM)
#if defined(ESP_PLATFORM) && !defined(ESP8266)

#include <stdint.h>

#include "freertos/FreeRTOS.h"

#if __has_include("soc/sha_caps.h")
# include "soc/sha_caps.h"
#elif __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No SHA capability header found"
#endif
#if SOC_SHA_SUPPORT_RESUME

#if __has_include("hal/sha_ll.h")
# include "hal/sha_ll.h"
# define HAVE_SHA_LL 1
#else
# define HAVE_SHA_LL 0
#endif


#define HAVE_HAL_SHA1    (SOC_SHA_SUPPORT_SHA1)
#define HAVE_HAL_SHA224  (SOC_SHA_SUPPORT_SHA224)
#define HAVE_HAL_SHA256  (SOC_SHA_SUPPORT_SHA256)
#define HAVE_HAL_SHA384  (SOC_SHA_SUPPORT_SHA384)
#define HAVE_HAL_SHA512  (SOC_SHA_SUPPORT_SHA512)

static portMUX_TYPE s_sha_mux = portMUX_INITIALIZER_UNLOCKED;
#define SHA_ENTER()  {portENTER_CRITICAL(&s_sha_mux); int __DECLARE_RCC_ATOMIC_ENV; sha_ll_enable_bus_clock(true); sha_ll_reset_register();}
#define SHA_EXIT()   {portEXIT_CRITICAL(&s_sha_mux); int __DECLARE_RCC_ATOMIC_ENV; sha_ll_enable_bus_clock(false);}
#define SHA_WAIT()   {while (sha_ll_busy()) { } }

/* ================================================================
 * SHA-1 (HAL path, no save/restore) - needs char buf[112];
 * ================================================================ */
#if HAVE_HAL_SHA1

static void __attribute__((noinline))
sha_hal_process_block(void *state_buf, const void *blk,
                      esp_sha_type type, size_t digest_words,
                      size_t block_words, bool first)
{
    SHA_ENTER();
    if (!first) {
        sha_ll_write_digest(type, state_buf, digest_words);
    }
    sha_ll_fill_text_block(blk, block_words);
    if (first) {
        sha_ll_start_block(type);
    } else {
        sha_ll_continue_block(type);
    }
    SHA_WAIT();
    sha_ll_read_digest(type, state_buf, digest_words);
    SHA_EXIT();
}

static const uint8_t S1_IV_BYTES[20] PROGMEM = {
    0x67,0x45,0x23,0x01, 0xEF,0xCD,0xAB,0x89,
    0x98,0xBA,0xDC,0xFE, 0x10,0x32,0x54,0x76,
    0xC3,0xD2,0xE1,0xF0
};

void br_sha1_init(br_sha1_context *cc)
{
    cc->vtable = &br_sha1_vtable;
    cc->count = 0;
    memset(cc->buf, 0, sizeof cc->buf);
    /* Store IV bytes exactly as given (LE words) into cc->val */
    memcpy(cc->val, S1_IV_BYTES, sizeof S1_IV_BYTES);
}

void br_sha1_update(br_sha1_context *cc, const void *data, size_t len)
{
    const uint8_t *src = data;
    size_t used = (size_t)(cc->count & 63U);

    if (!len) return;
    cc->count += len;

    if (used) {
        size_t take = 64 - used;
        if (take > len) take = len;
        memcpy(cc->buf + used, src, take);
        src += take;
        len -= take;
        used += take;
        if (used == 64) {
            sha_hal_process_block(cc->val, cc->buf, SHA1, 5, 16, cc->count == 64);
            used = 0;
        }
    }
    while (len >= 64) {
        sha_hal_process_block(cc->val, src, SHA1, 5, 16,
                              (cc->count - len == 0) && !used);
        src += 64;
        len -= 64;
    }
    if (len) {
        memcpy(cc->buf, src, len);
    }
}

void br_sha1_out(const br_sha1_context *cc, void *out)
{
    br_sha1_context ctx = *cc;
    size_t used = (size_t)(ctx.count & 63U);
    uint64_t bit_len = (uint64_t)ctx.count << 3;

    ctx.buf[used++] = 0x80;
    if (used > 56) {
        memset(ctx.buf + used, 0, 64 - used);
        sha_hal_process_block(ctx.val, ctx.buf, SHA1, 5, 16, ctx.count <= 64);
        used = 0;
    }
    memset(ctx.buf + used, 0, 56 - used);
    br_enc64be(ctx.buf + 56, bit_len);
    sha_hal_process_block(ctx.val, ctx.buf, SHA1, 5, 16, ctx.count <= 64);
    memcpy(out, ctx.val, 20);
}

uint64_t br_sha1_state(const br_sha1_context *cc, void *dst)
{
    /* Export raw 20-byte LE midstate image exactly as stored */
    memcpy(dst, cc->val, 20);
    return cc->count;
}

void br_sha1_set_state(br_sha1_context *cc, const void *src, uint64_t count)
{
    if (!(cc->vtable == &br_sha1_vtable)) {
        cc->vtable = &br_sha1_vtable;
    }
    /* Import raw 20-byte LE midstate image exactly as provided */
    memcpy(cc->val, src, 20);
    cc->count = count;
    memset(cc->buf, 0, sizeof cc->buf);
}

const br_hash_class br_sha1_vtable PROGMEM = {
    sizeof(br_sha1_context),
    BR_HASHDESC_ID(br_sha1_ID)
      | BR_HASHDESC_OUT(20)
      | BR_HASHDESC_LBLEN(6)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE
      | BR_HASHDESC_STATE(32),
    (void (*)(const br_hash_class **)) &br_sha1_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha1_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha1_out,
    (uint64_t (*)(const br_hash_class *const *, void *)) &br_sha1_state,
    (void (*)(const br_hash_class **, const void *, uint64_t)) &br_sha1_set_state
};

#endif /* HAVE_HAL_SHA1 */

/* ================================================================
 * SHA-224 (HAL path, no save/restore)
 * ================================================================ */
#if HAVE_HAL_SHA224

/* SHA-224 IV as 32-bit words (FIPS 180-4) */
static const uint32_t S224_IV_WORDS[8] = {
    0xc1059ed8U, 0x367cd507U, 0x3070dd17U, 0xf70e5939U,
    0xffc00b31U, 0x68581511U, 0x64f98fa7U, 0xbefa4fa4U
};

void br_sha224_init(br_sha224_context *cc) {
    cc->vtable = &br_sha224_vtable;
    cc->count = 0;
    memset(cc->buf, 0, sizeof cc->buf);
    /* Internal state is LE; encode IV directly in LE form into cc->val */
    br_range_enc32le(cc->val, S224_IV_WORDS, 8);
}

/* Reuse SHA-256 out, then truncate to 28 bytes */
void br_sha224_out(const br_sha224_context *cc, void *out) {
    uint8_t full[32];
    br_sha256_out((const br_sha256_context *)cc, full);
    memcpy(out, full, 28);
}

const br_hash_class br_sha224_vtable PROGMEM = {
    sizeof(br_sha224_context),
    BR_HASHDESC_ID(br_sha224_ID)
      | BR_HASHDESC_OUT(28)
      | BR_HASHDESC_LBLEN(6)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE
      | BR_HASHDESC_STATE(32),
    (void (*)(const br_hash_class **)) &br_sha224_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha224_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha224_out,
    (uint64_t (*)(const br_hash_class *const *, void *)) &br_sha224_state,
    (void (*)(const br_hash_class **, const void *, uint64_t)) &br_sha224_set_state
};

#endif /* HAVE_HAL_SHA224 */

/* ================================================================
 * SHA-256 (HAL path, save/restore)
 * ================================================================ */
#if HAVE_HAL_SHA256 && defined(SOC_SHA_SUPPORT_RESUME)

/* Fixed SHA-256 IV in big-endian bytes (spec-defined) */
static const uint8_t S256_IV_BYTES[32] PROGMEM = {
    0x6a, 0x09, 0xe6, 0x67, 0xbb, 0x67, 0xae, 0x85,
    0x3c, 0x6e, 0xf3, 0x72, 0xa5, 0x4f, 0xf5, 0x3a,
    0x51, 0x0e, 0x52, 0x7f, 0x9b, 0x05, 0x68, 0x8c,
    0x1f, 0x83, 0xd9, 0xab, 0x5b, 0xe0, 0xcd, 0x19
};

/* Maintain the partial block in cc->buf[] (64 bytes) */
static inline void s256_update_partial(br_sha256_context *cc,
                                       const uint8_t *data_ptr,
                                       size_t data_len,
                                       size_t prev_partial_len)
{
    size_t new_partial_len = (prev_partial_len + data_len) & 63U;

    if (new_partial_len == 0) {
        /* Exact block boundary: clear partial buffer */
        memset(cc->buf, 0, sizeof cc->buf);
        return;
    }

    if (data_len >= new_partial_len) {
        /* Tail entirely from current input */
        memcpy(cc->buf, data_ptr + (data_len - new_partial_len), new_partial_len);
    } else {
        /* Need some bytes from previous partial tail */
        size_t need_prev = new_partial_len - data_len;
        if (need_prev && need_prev <= prev_partial_len) {
            memmove(cc->buf, cc->buf + (prev_partial_len - need_prev), need_prev);
        } else if (need_prev) {
            memset(cc->buf, 0, need_prev);
        }
        memcpy(cc->buf + need_prev, data_ptr, data_len);
    }

    if (new_partial_len < sizeof cc->buf) {
        memset(cc->buf + new_partial_len, 0, sizeof cc->buf - new_partial_len);
    }
}

void br_sha256_init(br_sha256_context *cc)
{
    cc->vtable = &br_sha256_vtable;
    cc->count = 0;
    memset(cc->buf, 0, sizeof cc->buf);
    /* Keep midstate bytes exactly as IV (BE byte image), as in the original */
    memcpy(cc->val, S256_IV_BYTES, 32);
}

void br_sha224_update(br_sha224_context *cc, const void *data, size_t len)
{
    if (!len) return;

    /* Embedded mode selection by vtable pointer (no desc, no tags) */
    const int mode = (cc->vtable == &br_sha256_vtable) ? SHA2_256 : SHA2_224;

    const uint8_t *data_ptr = (const uint8_t *)data;
    const uint8_t *orig_ptr = data_ptr;
    size_t orig_len = len;

    size_t prev_partial_len = (size_t)(cc->count & 63U);
    bool starting_from_iv = (cc->count == 0) && (prev_partial_len == 0);
    size_t stitch_take = 0;
    if (prev_partial_len && len >= (64 - prev_partial_len))
        stitch_take = 64 - prev_partial_len;

    if (stitch_take || len >= 64) {
        size_t block_count_in_run = 0;

        SHA_ENTER();

        /* Complete the partial block (stitch) if needed */
        if (stitch_take) {
            uint32_t midstate_words[8];
            /* Original semantics: decode state from LE words before seeding HW */
            br_range_dec32le(midstate_words, 8, cc->val);
            sha_ll_load(mode);
            sha_ll_write_digest(mode, midstate_words, 8);

            uint8_t first_block[64];
            memcpy(first_block, cc->buf, prev_partial_len);
            memcpy(first_block + prev_partial_len, data_ptr, stitch_take);

            sha_ll_fill_text_block(first_block, 16);
            sha_ll_continue_block(mode);
            SHA_WAIT();

            data_ptr += stitch_take;
            len -= stitch_take;
            block_count_in_run = 1;
        }

        /* Process full 64-byte blocks */
        if (len >= 64) {
            if (starting_from_iv && stitch_take == 0) {
                /* Start from IV directly */
                sha_ll_fill_text_block(data_ptr, 16);
                sha_ll_start_block(mode);
                SHA_WAIT();
                data_ptr += 64;
                len -= 64;
                block_count_in_run = 1;
            } else if (block_count_in_run == 0) {
                /* Seed from current midstate once before the run */
                uint32_t midstate_words2[8];
                br_range_dec32le(midstate_words2, 8, cc->val);
                sha_ll_load(mode);
                sha_ll_write_digest(mode, midstate_words2, 8);
            }

#if defined(CONFIG_IDF_TARGET_ARCH_RISCV)  /* RISC-V: enforce 8-block (512B) window with checkpointing */
            while (len >= 64) {
                sha_ll_fill_text_block(data_ptr, 16);
                sha_ll_continue_block(mode);
                SHA_WAIT();

                data_ptr += 64;
                len -= 64;
                block_count_in_run++;

                if (block_count_in_run == 8 && len >= 64) {
                    uint32_t midstate_words3[8];
                    sha_ll_load(mode);
                    sha_ll_read_digest(mode, midstate_words3, 8);
                    sha_ll_load(mode);
                    sha_ll_write_digest(mode, midstate_words3, 8);
                    block_count_in_run = 0;
                }
            }
#else       /* Xtensa: simple per-block processing, no extra checkpointing */
            while (len >= 64) {
                sha_ll_fill_text_block(data_ptr, 16);
                sha_ll_continue_block(mode);
                SHA_WAIT();
                data_ptr += 64;
                len -= 64;
            }
#endif
        }

        /* Commit midstate back; keep the same on-wire format as before: LE words in cc->val */
        {
            uint32_t tmp_words[8];
            sha_ll_load(mode);
            SHA_WAIT();
            sha_ll_read_digest(mode, tmp_words, 8);
            br_range_enc32le(cc->val, tmp_words, 8);
        }
        SHA_EXIT();
    }

    /* Bookkeeping and partial buffer maintenance */
    cc->count += orig_len;
    s256_update_partial((br_sha256_context *)cc, orig_ptr, orig_len, prev_partial_len);
}

void br_sha256_out(const br_sha256_context *cc, void *out)
{
    uint8_t  saved_partial[sizeof cc->buf];
    uint64_t saved_count = cc->count;
    uint8_t  saved_state[32];

    memcpy(saved_partial, cc->buf, sizeof cc->buf);
    memcpy(saved_state, cc->val, 32);

    uint8_t partial_len = (uint8_t)(saved_count & 63U);
    const int mode = (cc->vtable == &br_sha256_vtable) ? SHA2_256 : SHA2_224;

    bool midstate_is_iv = (memcmp(saved_state, S256_IV_BYTES, 32) == 0);
    bool no_full_blocks_done = (saved_count < 64);
    bool must_start_from_iv = midstate_is_iv && no_full_blocks_done;

    SHA_ENTER();

    if (!must_start_from_iv) {
        uint32_t midstate_words[8];
        /* Original semantics: state stored as LE words in cc->val */
        br_range_dec32le(midstate_words, 8, saved_state);
        sha_ll_load(mode);
        sha_ll_write_digest(mode, midstate_words, 8);
    }

    uint8_t final_blocks[128];
    memcpy(final_blocks, saved_partial, partial_len);
    final_blocks[partial_len] = 0x80;

    size_t zero_len = (partial_len < 56) ? (56 - partial_len - 1)
                                         : (120 - partial_len - 1);
    if (zero_len) {
        memset(final_blocks + partial_len + 1, 0, zero_len);
    }

    uint64_t bit_length = saved_count * 8;
    size_t len_pos = partial_len + 1 + zero_len;
    for (int i = 0; i < 8; i++) {
        final_blocks[len_pos + i] = (uint8_t)(bit_length >> (56 - 8 * i));
    }
    size_t total_final_len = len_pos + 8;

    if (total_final_len == 64) {
        sha_ll_fill_text_block(final_blocks, 16);
        if (must_start_from_iv)
            sha_ll_start_block(mode);
        else
            sha_ll_continue_block(mode);
        SHA_WAIT();
    } else {
        sha_ll_fill_text_block(final_blocks, 16);
        if (must_start_from_iv)
            sha_ll_start_block(mode);
        else
            sha_ll_continue_block(mode);
        SHA_WAIT();
        sha_ll_fill_text_block(final_blocks + 64, 16);
        sha_ll_continue_block(mode);
        SHA_WAIT();
    }

    {
        uint32_t tmp_words[8];
        sha_ll_load(mode);
        sha_ll_read_digest(mode, tmp_words, 8);
        SHA_EXIT();
        /* Final digest serialization remains LE as in your original */
        br_range_enc32le(out, tmp_words, 8);
    }

    /* Non-destructive out(): restore snapshot */
    memcpy(((br_sha256_context *)cc)->buf, saved_partial, sizeof cc->buf);
    ((br_sha256_context *)cc)->count = saved_count;
    /* cc->val was never modified in out() */
}

uint64_t br_sha224_state(const br_sha256_context *cc, void *dst)
{
    /* Export midstate bytes exactly as stored (LE words image), unchanged */
    memcpy(dst, cc->val, 32);
    return cc->count;
}

void br_sha224_set_state(br_sha256_context *cc, const void *src, uint64_t count)
{
    if( !(cc->vtable == &br_sha256_vtable) && !(cc->vtable == &br_sha224_vtable)){
        cc->vtable = &br_sha256_vtable; /* safety for multihash fresh gen context */
    }
    /* Reset partial, set count, and import state bytes exactly as provided */
    memset(cc->buf, 0, sizeof cc->buf);
    memcpy(cc->val, src, 32);
    cc->count = count;
}

const br_hash_class br_sha256_vtable PROGMEM = {
    sizeof(br_sha256_context),
    BR_HASHDESC_ID(br_sha256_ID)
        | BR_HASHDESC_OUT(32)
        | BR_HASHDESC_STATE(32)
        | BR_HASHDESC_LBLEN(6)
        | BR_HASHDESC_MD_PADDING
        | BR_HASHDESC_MD_PADDING_BE,
    (void (*)(const br_hash_class **))&br_sha256_init,
    (void (*)(const br_hash_class **,
        const void *, size_t))&br_sha256_update,
    (void (*)(const br_hash_class *const *, void *))&br_sha256_out,
    (uint64_t (*)(const br_hash_class *const *, void *))&br_sha256_state,
    (void (*)(const br_hash_class **, const void *, uint64_t))
        &br_sha256_set_state
};

#endif /* HAVE_HAL_SHA256 && SOC_SHA_SUPPORT_RESUME */

/* ================================================================
 * SHA-384 (HAL path, no save/restore)
 * ================================================================ */
#if HAVE_HAL_SHA384

static const uint64_t S384_IV_WORDS[8] = {
    0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
    0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
    0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
    0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
};

void br_sha384_init(br_sha384_context *cc) {
    cc->vtable = &br_sha384_vtable;
    cc->count = 0;
    memset(cc->buf, 0, sizeof cc->buf);
    /* Internal state is LE; encode IV directly in LE form into cc->val */
    br_range_enc64le(cc->val, S384_IV_WORDS, 8);
}

void br_sha384_update(br_sha384_context *cc, const void *data, size_t len) {
    if (!len) return;
    const uint8_t *src = (const uint8_t *)data;
    size_t used = (size_t)(cc->count & 127U);
    uint64_t prior_count = cc->count;
    cc->count += len;

    if (used) {
        size_t take = 128 - used;
        if (take > len) take = len;
        memcpy(cc->buf + used, src, take);
        src += take;
        len -= take;
        used += take;
        if (used == 128) {
            uint64_t st_be[8];
            br_range_dec64le(st_be, 8, cc->val);
            br_range_enc64be(st_be, st_be, 8);
            SHA_ENTER();
            sha_ll_load(SHA2_512);
            sha_ll_write_digest(SHA2_512, st_be, 16);
            sha_ll_fill_text_block(cc->buf, 32);
            sha_ll_continue_block(SHA2_512);
            SHA_WAIT();
            sha_ll_read_digest(SHA2_512, st_be, 16);
            SHA_EXIT();
            br_range_dec64be(st_be, 8, st_be);
            br_range_enc64le(cc->val, st_be, 8);
            prior_count += 128;
            used = 0;
        }
    }

    if (len >= 128) {
        uint64_t st_be[8];
        br_range_dec64le(st_be, 8, cc->val);
        br_range_enc64be(st_be, st_be, 8);
        bool first = (prior_count == 0);
        SHA_ENTER();
        sha_ll_load(SHA2_512);
        sha_ll_write_digest(SHA2_512, st_be, 16);
        while (len >= 128) {
            sha_ll_fill_text_block(src, 32);
            if (first) {
                sha_ll_start_block(SHA2_512);
                first = false;
            } else {
                sha_ll_continue_block(SHA2_512);
            }
            SHA_WAIT();
            src += 128;
            len -= 128;
        }
        sha_ll_read_digest(SHA2_512, st_be, 16);
        SHA_EXIT();
        br_range_dec64be(st_be, 8, st_be);
        br_range_enc64le(cc->val, st_be, 8);
    }

    if (len) memcpy(cc->buf, src, len);
}

void br_sha384_out(const br_sha384_context *cc, void *out) {
    br_sha384_context ctx = *cc;
    size_t used = (size_t)(ctx.count & 127U);
    uint64_t bit_hi = (uint64_t)(ctx.count >> 61);
    uint64_t bit_lo = (uint64_t)(ctx.count << 3);

    ctx.buf[used++] = 0x80;
    if (used > 112) {
        memset(ctx.buf + used, 0, 128 - used);
        uint64_t st_be[8];
        br_range_dec64le(st_be, 8, ctx.val);
        br_range_enc64be(st_be, st_be, 8);
        SHA_ENTER();
        sha_ll_load(SHA2_512);
        sha_ll_write_digest(SHA2_512, st_be, 16);
        sha_ll_fill_text_block(ctx.buf, 32);
        sha_ll_continue_block(SHA2_512);
        SHA_WAIT();
        sha_ll_read_digest(SHA2_512, st_be, 16);
        SHA_EXIT();
        br_range_dec64be(st_be, 8, st_be);
        br_range_enc64le(ctx.val, st_be, 8);
        used = 0;
    }

    memset(ctx.buf + used, 0, 112 - used);
    br_enc64be(ctx.buf + 112, bit_hi);
    br_enc64be(ctx.buf + 120, bit_lo);

    uint64_t st_be2[8];
    br_range_dec64le(st_be2, 8, ctx.val);
    br_range_enc64be(st_be2, st_be2, 8);
    SHA_ENTER();
    sha_ll_load(SHA2_512);
    sha_ll_write_digest(SHA2_512, st_be2, 16);
    sha_ll_fill_text_block(ctx.buf, 32);
    sha_ll_continue_block(SHA2_512);
    SHA_WAIT();
    sha_ll_read_digest(SHA2_512, st_be2, 16);
    SHA_EXIT();

    br_range_dec64be(st_be2, 8, st_be2);
    br_range_enc64be(out, st_be2, 6);
}

uint64_t br_sha384_state(const br_sha384_context *cc, void *dst) {
    /* Export raw 64-byte LE midstate image exactly as stored */
    memcpy(dst, cc->val, 64);
    return cc->count;
}

void br_sha384_set_state(br_sha384_context *cc, const void *src, uint64_t count) {
    if (!(cc->vtable == &br_sha384_vtable)) {
        cc->vtable = &br_sha384_vtable;
    }
    /* Import raw 64-byte LE midstate image exactly as provided */
    memcpy(cc->val, src, 64);
    cc->count = count;
    memset(cc->buf, 0, sizeof cc->buf);
}

const br_hash_class br_sha384_vtable PROGMEM = {
    sizeof(br_sha384_context),
    BR_HASHDESC_ID(br_sha384_ID)
        | BR_HASHDESC_OUT(48)
        | BR_HASHDESC_STATE(64)
        | BR_HASHDESC_LBLEN(7)
        | BR_HASHDESC_MD_PADDING
        | BR_HASHDESC_MD_PADDING_BE
        | BR_HASHDESC_MD_PADDING_128,
    (void (*)(const br_hash_class **)) &br_sha384_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha384_update,
    (void (*)(const br_hash_class *const *, void *)) &br_sha384_out,
    (uint64_t (*)(const br_hash_class *const *, void *)) &br_sha384_state,
    (void (*)(const br_hash_class **, const void *, uint64_t)) &br_sha384_set_state
};

#endif /* HAVE_HAL_SHA384 */

/* ================================================================
 * SHA-512 (HAL path, no save/restore)
 * Uses SHA-384 update/state/set_state
 * ================================================================ */
#if HAVE_HAL_SHA512

static const uint64_t S512_IV_WORDS[8] = {
    0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
    0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

void br_sha512_init(br_sha512_context *cc) {
    cc->vtable = &br_sha512_vtable;
    cc->count = 0;
    memset(cc->buf, 0, sizeof cc->buf);
    /* Internal state is LE; encode IV directly in LE form into cc->val */
    br_range_enc64le(cc->val, S512_IV_WORDS, 8);
}

void br_sha512_out(const br_sha512_context *cc, void *out) {
    br_sha512_context ctx = *cc;
    size_t used = (size_t)(ctx.count & 127U);
    uint64_t bit_hi = (uint64_t)(ctx.count >> 61);
    uint64_t bit_lo = (uint64_t)(ctx.count << 3);

    ctx.buf[used++] = 0x80;
    if (used > 112) {
        memset(ctx.buf + used, 0, 128 - used);
        uint64_t st_be[8];
        br_range_dec64le(st_be, 8, ctx.val);
        br_range_enc64be(st_be, st_be, 8);
        SHA_ENTER();
        sha_ll_load(SHA2_512);
        sha_ll_write_digest(SHA2_512, st_be, 16);
        sha_ll_fill_text_block(ctx.buf, 32);
        sha_ll_continue_block(SHA2_512);
        SHA_WAIT();
        sha_ll_read_digest(SHA2_512, st_be, 16);
        SHA_EXIT();
        br_range_dec64be(st_be, 8, st_be);
        br_range_enc64le(ctx.val, st_be, 8);
        used = 0;
    }

    memset(ctx.buf + used, 0, 112 - used);
    br_enc64be(ctx.buf + 112, bit_hi);
    br_enc64be(ctx.buf + 120, bit_lo);

    uint64_t st_be2[8];
    br_range_dec64le(st_be2, 8, ctx.val);
    br_range_enc64be(st_be2, st_be2, 8);
    SHA_ENTER();
    sha_ll_load(SHA2_512);
    sha_ll_write_digest(SHA2_512, st_be2, 16);
    sha_ll_fill_text_block(ctx.buf, 32);
    sha_ll_continue_block(SHA2_512);
    SHA_WAIT();
    sha_ll_read_digest(SHA2_512, st_be2, 16);
    SHA_EXIT();

    /* Output digest in big‑endian bytes as BearSSL expects */
    br_range_dec64be(st_be2, 8, st_be2);
    br_range_enc64be(out, st_be2, 8);
}

const br_hash_class br_sha512_vtable PROGMEM = {
    sizeof(br_sha512_context),
    BR_HASHDESC_ID(br_sha512_ID)
      | BR_HASHDESC_OUT(64)
      | BR_HASHDESC_STATE(64)
      | BR_HASHDESC_LBLEN(7)
      | BR_HASHDESC_MD_PADDING
      | BR_HASHDESC_MD_PADDING_BE
      | BR_HASHDESC_MD_PADDING_128,
    (void (*)(const br_hash_class **)) &br_sha512_init,
    (void (*)(const br_hash_class **, const void *, size_t)) &br_sha384_update,  /* reuse SHA-384 update */
    (void (*)(const br_hash_class *const *, void *)) &br_sha512_out,
    (uint64_t (*)(const br_hash_class *const *, void *)) &br_sha512_state,
    (void (*)(const br_hash_class **, const void *, uint64_t)) &br_sha512_set_state
};

#endif /* HAVE_HAL_SHA512 */

#else
/* ===== ESP8266 - leave it unchanged ===== */
#endif // SOC_SHA_SUPPORT_RESUME
#endif // defined(ESP_PLATFORM) && !defined(ESP8266)
#endif //USE_SHA_ROM
