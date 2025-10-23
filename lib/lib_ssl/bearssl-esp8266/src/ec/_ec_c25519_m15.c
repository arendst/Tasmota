/*
 * _ec_c25519_m15.c — BearSSL Curve25519 (X25519) implementation using ESP32 ROM-backed Montgomery arithmetic
 *
 * This file provides a fast Montgomery ladder implementation for Curve25519 scalar
 * multiplication (X25519), leveraging the ESP32's ROM bigint accelerator for modular
 * multiplication in the prime field p = 2^255 - 19.
 *
 * Key features:
 *   - Field arithmetic in the normal domain using single-step ROM-backed multiply/square.
 *   - 8×32-bit little-endian limb representation for all field elements.
 *   - Constant-time Montgomery ladder for scalar multiplication.
 *   - RFC 7748–compliant clamping of scalar inputs.
 *   - Supports multiplication by arbitrary u‑coordinates and basepoint generation.
 *   - Fully compatible with BearSSL's ec_impl API.
 *
 * Internal operations avoid heap allocation and use fixed-size buffers.
 *
 * Requires: ESP32 platform with SOC_MPI_SUPPORTED enabled.
 *
 * Author: Christian Baars
 */

#if defined(USE_SHA_ROM)
#if defined(ESP_PLATFORM) && !defined(ESP8266) && !defined(CONFIG_IDF_TARGET_ESP32)

#if __has_include("soc/sha_caps.h")
# include "soc/sha_caps.h"
#elif __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No ESP capability header found"
#endif

#if SOC_MPI_SUPPORTED

#include "rom/bigint.h"
#include "t_inner.h"

#define WORDS 8  /* 8×32-bit limbs */

/* Prime p = 2^255 - 19 (little-endian 32-bit limbs) */
static const uint32_t P_LE[WORDS] = {
    0xFFFFFFED, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF
};

/* R^2 mod p (with R = 2^256 mod p = 38) */
static const uint32_t RR_LE[WORDS] = { 1444, 0,0,0,0,0,0,0 };

/* A24 = 121665 (normal-domain constant) */
static const uint32_t A24_LE[WORDS] = { 121665, 0,0,0,0,0,0,0 };

static const uint32_t MPRIME = 0x286BCA1B;

/* ---------- limb utilities ---------- */

static inline void zclear(uint32_t *a) { memset(a, 0, WORDS * sizeof(uint32_t)); }
static inline void zcopy(uint32_t *dst, const uint32_t *src) { memcpy(dst, src, WORDS * sizeof(uint32_t)); }

/* ct compare: return 1 if a >= b, else 0 */
static inline uint32_t ge_ct(const uint32_t *a, const uint32_t *b) {
    uint32_t gt = 0, eq = 1;
    for (int i = WORDS - 1; i >= 0; i--) {
        uint32_t ai = a[i], bi = b[i];
        uint32_t gt_i = (ai > bi);
        uint32_t lt_i = (ai < bi);
        gt |= (eq & gt_i);
        eq &= ~(gt_i | lt_i);
    }
    return gt | eq;
}

static inline void add_mod(uint32_t *d, const uint32_t *a, const uint32_t *b) {
    uint64_t c = 0;
    for (int i = 0; i < WORDS; i++) {
        c = (uint64_t)a[i] + b[i] + (c >> 32);
        d[i] = (uint32_t)c;
    }
    uint32_t need_sub = (uint32_t)(c >> 32);
    need_sub |= ge_ct(d, P_LE);
    uint32_t borrow = 0, tmp[WORDS];
    for (int i = 0; i < WORDS; i++) {
        uint64_t t = (uint64_t)d[i] - P_LE[i] - borrow;
        tmp[i] = (uint32_t)t;
        borrow = (uint32_t)(t >> 63);
    }
    for (int i = 0; i < WORDS; i++) d[i] = need_sub ? tmp[i] : d[i];
}

static inline void sub_mod(uint32_t *d, const uint32_t *a, const uint32_t *b) {
    uint32_t borrow = 0;
    for (int i = 0; i < WORDS; i++) {
        uint64_t t = (uint64_t)a[i] - b[i] - borrow;
        d[i] = (uint32_t)t;
        borrow = (uint32_t)(t >> 63);
    }
    uint64_t c = 0, tmp[WORDS];
    for (int i = 0; i < WORDS; i++) {
        c = (uint64_t)d[i] + P_LE[i] + (c >> 32);
        tmp[i] = (uint32_t)c;
    }
    for (int i = 0; i < WORDS; i++) d[i] = borrow ? tmp[i] : d[i];
}

static inline void field_mul(uint32_t *dst, const uint32_t *a, const uint32_t *b) {
    ets_bigint_enable();
    ets_bigint_modmult(a, b, P_LE, MPRIME, RR_LE, WORDS);
    ets_bigint_wait_finish();
    ets_bigint_getz(dst, WORDS);
    ets_bigint_disable();
}

static inline void field_sqr(uint32_t *dst, const uint32_t *a) {
    field_mul(dst, a, a);
}

/* Fermat inversion: a^(p-2) in normal domain */
static void field_inv(uint32_t *out, const uint32_t *a) {
    static const uint32_t EXP_P_MINUS_2[WORDS] = {
        0xFFFFFFEB, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF
    };
    uint32_t res[WORDS], base[WORDS];
    zclear(res); res[0] = 1;
    zcopy(base, a);
    for (int wi = WORDS - 1; wi >= 0; wi--) {
        uint32_t w = EXP_P_MINUS_2[wi];
        for (int b = 31; b >= 0; b--) {
            field_sqr(res, res);
            if ((w >> b) & 1U) field_mul(res, res, base);
        }
    }
    zcopy(out, res);
}

/* Conditional swap */
static inline void cswap(uint32_t *a, uint32_t *b, uint32_t ctl) {
    uint32_t mask = -ctl;
    for (int i = 0; i < WORDS; i++) {
        uint32_t t = (a[i] ^ b[i]) & mask;
        a[i] ^= t; b[i] ^= t;
    }
}

/* ---------- X25519 ladder (normal domain) ---------- */

static const unsigned char GEN[] PROGMEM = {
    0x09, 0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

static const unsigned char ORDER[] PROGMEM = {
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static const unsigned char *
api_generator(int curve, size_t *len)
{
    (void)curve;
    *len = 32;
    return GEN;
}

static const unsigned char *
api_order(int curve, size_t *len)
{
    (void)curve;
    *len = 32;
    return ORDER;
}

static size_t
api_xoff(int curve, size_t *len)
{
    (void)curve;
    *len = 32;
    return 0;
}

static uint32_t
api_mul(unsigned char *G, size_t Glen,
        const unsigned char *kb, size_t kblen, int curve)
{
    (void)curve;

    if (Glen != 32 || kblen > 32) {
        return 0;
    }

    /* Clamp scalar per RFC 7748 */
    unsigned char k[32];
    memset(k, 0, 32 - kblen);
    memcpy(k + (32 - kblen), kb, kblen);
    k[31] &= 0xF8;
    k[0] &= 0x7F;
    k[0] |= 0x40;

    /* Load u and clear high bit per RFC 7748 */
    unsigned char u_bytes[32];
    memcpy(u_bytes, G, 32);
    u_bytes[31] &= 0x7F;

    uint32_t x1[WORDS], x2[WORDS], z2[WORDS], x3[WORDS], z3[WORDS];
    br_range_dec32le(x1, WORDS, u_bytes);

    /* Initialize:
     *   (x2:z2) = (1:0)
     *   (x3:z3) = (u:1)
     */
    zclear(z2);
    zclear(x2); x2[0] = 1;
    zcopy(x3, x1);
    zclear(z3); z3[0] = 1;

    uint32_t a[WORDS], aa[WORDS], b[WORDS], bb[WORDS];
    uint32_t c[WORDS], d[WORDS], e[WORDS], da[WORDS], cb[WORDS];
    uint32_t t[WORDS];

    uint32_t swap = 0;
    for (int i = 254; i >= 0; i--) {
        uint32_t kt = (k[31 - (i >> 3)] >> (i & 7)) & 1U;
        swap ^= kt;
        cswap(x2, x3, swap);
        cswap(z2, z3, swap);
        swap = kt;

        /* Ladder step */
        add_mod(a, x2, z2);        /* a = x2 + z2 */
        sub_mod(b, x2, z2);        /* b = x2 - z2 */
        field_sqr(aa, a);          /* aa = a^2 */
        field_sqr(bb, b);          /* bb = b^2 */
        sub_mod(e, aa, bb);        /* e = aa - bb */

        add_mod(c, x3, z3);        /* c = x3 + z3 */
        sub_mod(d, x3, z3);        /* d = x3 - z3 */
        field_mul(da, d, a);       /* da = d * a */
        field_mul(cb, c, b);       /* cb = c * b */

        add_mod(x3, da, cb);       /* x3 = (da + cb)^2 */
        field_sqr(x3, x3);
        sub_mod(z3, da, cb);       /* z3 = (da - cb)^2 * x1 */
        field_sqr(z3, z3);
        field_mul(z3, z3, x1);

        field_mul(x2, aa, bb);     /* x2 = aa * bb */

        /* z2 = e * (aa + A24 * e) */
        field_mul(t, A24_LE, e);   /* t = A24 * e */
        add_mod(t, t, aa);         /* t = aa + A24*e */
        field_mul(z2, e, t);       /* z2 = e * t */
    }

    cswap(x2, x3, swap);
    cswap(z2, z3, swap);

    /* u = x2 / z2 */
    uint32_t z2i[WORDS], unorm[WORDS];
    field_inv(z2i, z2);
    field_mul(unorm, x2, z2i);

    /* Final reduction if needed and serialize */
    if (ge_ct(unorm, P_LE)) {
        sub_mod(unorm, unorm, P_LE);
    }
    br_range_enc32le(G, unorm, WORDS);
    return 1;
}

static size_t
api_mulgen(unsigned char *R,
           const unsigned char *x, size_t xlen, int curve)
{
    const unsigned char *G0;
    size_t Glen;

    G0 = api_generator(curve, &Glen);
    memcpy_P(R, G0, Glen);
    api_mul(R, Glen, x, xlen, curve);
    return Glen;
}

static uint32_t
api_muladd(unsigned char *A, const unsigned char *B, size_t len,
           const unsigned char *x, size_t xlen,
           const unsigned char *y, size_t ylen, int curve)
{
    (void)A; (void)B; (void)len; (void)x; (void)xlen; (void)y; (void)ylen; (void)curve;
    /* Not applicable for Curve25519 (no ECDSA). */
    return 0;
}

/* see bearssl_ec.h */
const br_ec_impl br_ec_c25519_m15 PROGMEM = {
    (uint32_t)0x20000000,
    &api_generator,
    &api_order,
    &api_xoff,
    &api_mul,
    &api_mulgen,
    &api_muladd
};

#endif /* SOC_MPI_SUPPORTED */
#endif /* ESP_PLATFORM && !ESP8266 */
#endif /* USE_SHA_ROM */
