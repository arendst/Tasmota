/*
 * _ec_p256_m15.c — BearSSL P-256 implementation using ESP32 ROM-backed Montgomery arithmetic
 *
 * This file provides a fast elliptic curve implementation for secp256r1 (P-256),
 * leveraging the ESP32's ROM bigint accelerator for modular multiplication.
 *
 * Key features:
 *   - Field arithmetic in normal domain using Montgomery-backed multiply/square.
 *   - Jacobian point representation with full group law (point add/double).
 *   - Scalar multiplication via double-and-add, supporting arbitrary base points.
 *   - Conversion between affine and Jacobian coordinates.
 *   - Compact encoding/decoding of uncompressed points (04 || X || Y).
 *   - Fully compatible with BearSSL's ec_impl API.
 *
 * All field elements are stored as 8×32-bit little-endian limbs.
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

#include <stdint.h>
#include "rom/bigint.h"
#include "t_inner.h"

#define WORDS 8

/* ESP32 ROM Montgomery parameters (little-endian).*/
static const uint32_t P_LE[8] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF
};

static const uint32_t RR_LE[8] = {
    0x03000000, 0x00000000, 0xFFFFFFFF, 0xFBFFFFFF,
    0xFEFFFFFF, 0xFFFFFFFF, 0xFDFFFFFF, 0x04000000
};

/* -p^{-1} mod 2^32 */
static const uint32_t MPRIME = 0x00000001;

/* Factor to convert ROM Montgomery output back to normal domain (8 limbs) */
static const uint32_t CINV2_LE[8] = {
    0xB15F7DC9, 0x21BC7192, 0xF82DEBEB, 0xF2086906,
    0x8AD3BB54, 0xE34453E4, 0xB2B4EF16, 0x5FF55809
};

/* Generator point G in little-endian 32-bit limbs (LSW first) */
static const uint32_t Gx[WORDS] = {
    0xD898C296, 0xF4A13945, 0x2DEB33A0, 0x77037D81,
    0x63A440F2, 0xF8BCE6E5, 0xE12C4247, 0x6B17D1F2
};

static const uint32_t Gy[WORDS] = {
    0x37BF51F5, 0xCBB64068, 0x6B315ECE, 0x2BCE3357,
    0x7C0F9E16, 0x8EE7EB4A, 0xFE1A7F9B, 0x4FE342E2
};

typedef struct {
    uint32_t X[WORDS];
    uint32_t Y[WORDS];
    uint32_t Z[WORDS];
} p256_pt;

/* ---------- small utilities ---------- */

static inline void zclear(uint32_t *dst) {
    memset(dst, 0, WORDS * sizeof(uint32_t));
}

static inline void zcopy(uint32_t *dst, const uint32_t *src) {
    memcpy(dst, src, WORDS * sizeof(uint32_t));
}

static inline int is_zero(const uint32_t *a) {
    uint32_t acc = 0;
    for (int i = 0; i < WORDS; i++) acc |= a[i];
    return acc == 0;
}

/* big-endian bytes -> internal little-endian limbs (reverse word order) */
static void be32_to_le32(const uint8_t *src, uint32_t *dst) {
    for (int i = 0; i < WORDS; i++) {
        int j = WORDS - 1 - i;
        dst[i] = ((uint32_t)src[4*j] << 24)
               | ((uint32_t)src[4*j + 1] << 16)
               | ((uint32_t)src[4*j + 2] << 8)
               | ((uint32_t)src[4*j + 3]);
    }
}

/* internal little-endian limbs -> big-endian bytes (reverse word order) */
static void le32_to_be32(const uint32_t *src, uint8_t *dst) {
    for (int i = 0; i < WORDS; i++) {
        int j = WORDS - 1 - i;
        dst[4*j]     = (uint8_t)(src[i] >> 24);
        dst[4*j + 1] = (uint8_t)(src[i] >> 16);
        dst[4*j + 2] = (uint8_t)(src[i] >> 8);
        dst[4*j + 3] = (uint8_t)(src[i]);
    }
}

/* ---------- field arithmetic modulo p (normal domain) ---------- */

static inline const uint32_t *Pmod(void) { return P_LE; }

static int ge_mod_p(const uint32_t *a) {
    const uint32_t *P = Pmod();
    for (int i = WORDS - 1; i >= 0; i--) {
        if (a[i] > P[i]) return 1;
        if (a[i] < P[i]) return 0;
    }
    return 1; /* equal */
}

static void field_add_mod(uint32_t *dst, const uint32_t *a, const uint32_t *b) {
    const uint32_t *P = Pmod();
    uint64_t carry = 0;
    for (int i = 0; i < WORDS; i++) {
        uint64_t sum = (uint64_t)a[i] + b[i] + carry;
        dst[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    if (carry || ge_mod_p(dst)) {
        uint64_t borrow = 0;
        for (int i = 0; i < WORDS; i++) {
            uint64_t diff = (uint64_t)dst[i] - P[i] - borrow;
            dst[i] = (uint32_t)diff;
            borrow = (diff >> 63) & 1;
        }
    }
}

static void field_sub_mod(uint32_t *dst, const uint32_t *a, const uint32_t *b) {
    const uint32_t *P = Pmod();
    uint64_t borrow = 0;
    for (int i = 0; i < WORDS; i++) {
        uint64_t diff = (uint64_t)a[i] - b[i] - borrow;
        dst[i] = (uint32_t)diff;
        borrow = (diff >> 63) & 1;
    }
    if (borrow) {
        uint64_t carry = 0;
        for (int i = 0; i < WORDS; i++) {
            uint64_t sum = (uint64_t)dst[i] + P[i] + carry;
            dst[i] = (uint32_t)sum;
            carry = sum >> 32;
        }
    }
}

/* ROM-backed modular multiply returning normal-domain result (8 limbs) */
static void rom_field_mul(uint32_t *dst, const uint32_t *a, const uint32_t *b) {
    uint32_t tmp[WORDS];

    ets_bigint_enable();

    /* Montgomery multiply in ROM (returns Montgomery residue) */
    ets_bigint_modmult(a, b, P_LE, MPRIME, RR_LE, WORDS);
    ets_bigint_wait_finish();
    ets_bigint_getz(tmp, WORDS);

    /* Convert out of Montgomery domain using the proven CINV2_LE */
    ets_bigint_modmult(tmp, CINV2_LE, P_LE, MPRIME, RR_LE, WORDS);
    ets_bigint_wait_finish();
    ets_bigint_getz(dst, WORDS);

    ets_bigint_disable();
}

static inline void field_mul(uint32_t *dst, const uint32_t *a, const uint32_t *b) {
    rom_field_mul(dst, a, b);
}

static inline void field_sqr(uint32_t *dst, const uint32_t *a) {
    rom_field_mul(dst, a, a);
}

/* Square-and-multiply exponentiation for p-2 (normal domain throughout) */
static void field_inv(uint32_t *out, const uint32_t *a) {
    static const uint32_t EXP_P_MINUS_2[WORDS] = {
        0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
        0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF
    };
    uint32_t res[WORDS], base[WORDS];

    zclear(res); res[0] = 1;     /* res = 1 */
    zcopy(base, a);              /* base = a */

    for (int wi = WORDS - 1; wi >= 0; wi--) {
        uint32_t w = EXP_P_MINUS_2[wi];
        for (int b = 31; b >= 0; b--) {
            field_sqr(res, res);
            if ((w >> b) & 1U) {
                field_mul(res, res, base);
            }
        }
    }
    zcopy(out, res);
}

/* ---------- point utilities ---------- */

static inline void load_generator(p256_pt *Pp) {
    zcopy(Pp->X, Gx);
    zcopy(Pp->Y, Gy);
    zclear(Pp->Z);
    Pp->Z[0] = 1;
}

static void to_affine(p256_pt *Pp) {
    uint32_t zi[WORDS], zi2[WORDS], xi[WORDS], yi[WORDS];
    uint32_t z0 = 0;
    for (int i = 0; i < WORDS; i++) z0 |= Pp->Z[i];
    if (z0 == 0) {
        zclear(Pp->X); zclear(Pp->Y); zclear(Pp->Z);
        return;
    }
    field_inv(zi, Pp->Z);
    field_sqr(zi2, zi);
    field_mul(xi, Pp->X, zi2);
    field_mul(yi, Pp->Y, zi2);
    field_mul(yi, yi, zi);
    zcopy(Pp->X, xi);
    zcopy(Pp->Y, yi);
    zclear(Pp->Z); Pp->Z[0] = 1;
}

/* ---------- group law (Jacobian, a = -3) ---------- */

__attribute__((noinline))
static void p256_point_double(p256_pt *Q) {
    if (is_zero(Q->Y)) { zclear(Q->X); zclear(Q->Y); zclear(Q->Z); return; }

    uint32_t Z2[WORDS], M[WORDS], Mtmp[WORDS], S[WORDS], T1[WORDS], T2[WORDS], X3[WORDS], Y3[WORDS];

    /* Z2 = Z^2 */
    field_sqr(Z2, Q->Z);

    /* T1 = X - Z^2 ; T2 = X + Z^2 */
    field_sub_mod(T1, Q->X, Z2);
    field_add_mod(T2, Q->X, Z2);

    /* M = (X - Z^2) * (X + Z^2) */
    field_mul(M, T1, T2);

    /* M = 3 * M */
    zcopy(Mtmp, M);
    field_add_mod(M, M, M);    /* 2*M */
    field_add_mod(M, M, Mtmp); /* 3*M */

    /* S = 4 * X * Y^2 */
    field_sqr(S, Q->Y);        /* Y^2 */
    field_mul(S, S, Q->X);     /* X*Y^2 */
    field_add_mod(S, S, S);    /* 2*X*Y^2 */
    field_add_mod(S, S, S);    /* 4*X*Y^2 */

    /* X3 = M^2 - 2*S */
    field_sqr(X3, M);
    field_sub_mod(X3, X3, S);
    field_sub_mod(X3, X3, S);

    /* Y3 = M*(S - X3) - 8*Y^4 */
    field_sub_mod(Y3, S, X3);
    field_mul(Y3, Y3, M);

    field_sqr(T1, Q->Y);       /* Y^2 */
    field_sqr(T1, T1);         /* Y^4 */
    field_add_mod(T1, T1, T1); /* 2*Y^4 */
    field_add_mod(T1, T1, T1); /* 4*Y^4 */
    field_add_mod(T1, T1, T1); /* 8*Y^4 */

    field_sub_mod(Y3, Y3, T1);

    /* Z3 = 2*Y*Z */
    field_mul(Q->Z, Q->Y, Q->Z);
    field_add_mod(Q->Z, Q->Z, Q->Z);

    zcopy(Q->X, X3);
    zcopy(Q->Y, Y3);
}

__attribute__((noinline))
static void p256_point_add(p256_pt *R, const p256_pt *Pp, const p256_pt *Qp) {
    if (is_zero(Pp->Z)) { zcopy(R->X, Qp->X); zcopy(R->Y, Qp->Y); zcopy(R->Z, Qp->Z); return; }
    if (is_zero(Qp->Z)) { zcopy(R->X, Pp->X); zcopy(R->Y, Pp->Y); zcopy(R->Z, Pp->Z); return; }

    uint32_t Z1Z1[WORDS], Z2Z2[WORDS], U1[WORDS], U2[WORDS];
    uint32_t S1[WORDS], S2[WORDS], H[WORDS], RR[WORDS];
    uint32_t H2[WORDS], H3[WORDS], U1H2[WORDS], X3[WORDS], Y3[WORDS], Z3[WORDS], t[WORDS];

    field_sqr(Z1Z1, Pp->Z);
    field_sqr(Z2Z2, Qp->Z);

    field_mul(U1, Pp->X, Z2Z2);
    field_mul(U2, Qp->X, Z1Z1);

    field_mul(t, Qp->Z, Z2Z2);  /* Z2^3 */
    field_mul(S1, Pp->Y, t);

    field_mul(t, Pp->Z, Z1Z1);  /* Z1^3 */
    field_mul(S2, Qp->Y, t);

    field_sub_mod(H, U2, U1);
    field_sub_mod(RR, S2, S1);

    if (is_zero(H)) {
        if (is_zero(RR)) {
            p256_pt D = *Pp;
            p256_point_double(&D);
            zcopy(R->X, D.X);
            zcopy(R->Y, D.Y);
            zcopy(R->Z, D.Z);
        } else {
            zclear(R->X); zclear(R->Y); zclear(R->Z); /* infinity */
        }
        return;
    }

    field_sqr(H2, H);
    field_mul(H3, H, H2);
    field_mul(U1H2, U1, H2);

    field_sqr(X3, RR);
    field_sub_mod(X3, X3, H3);
    field_sub_mod(X3, X3, U1H2);
    field_sub_mod(X3, X3, U1H2); /* -2*U1H2 */

    field_sub_mod(Y3, U1H2, X3);
    field_mul(Y3, Y3, RR);

    field_mul(t, S1, H3);
    field_sub_mod(Y3, Y3, t);

    field_mul(t, Pp->Z, Qp->Z);
    field_mul(Z3, t, H);

    zcopy(R->X, X3);
    zcopy(R->Y, Y3);
    zcopy(R->Z, Z3);
}

/* ---------- shared scalar multiply helpers (reduce duplication) ---------- */

static void scalar_mul_point(p256_pt *R, const p256_pt *Base, const uint8_t *k, size_t klen) {
    zclear(R->X); zclear(R->Y); zclear(R->Z); /* R = O */
    for (size_t bi = 0; bi < klen * 8; bi++) {
        p256_point_double(R);
        if ((k[bi >> 3] >> (7 - (bi & 7))) & 1) {
            p256_point_add(R, R, Base);
        }
    }
}

/* Load uncompressed point (04 || X || Y) into Jacobian with Z=1 */
static int load_point_uncompressed(p256_pt *Pp, const unsigned char *buf, size_t len) {
    if (len != 65 || buf[0] != 0x04) return 0;
    be32_to_le32(buf + 1,  Pp->X);
    be32_to_le32(buf + 33, Pp->Y);
    zclear(Pp->Z); Pp->Z[0] = 1;
    return 1;
}

static void store_point_uncompressed(unsigned char *buf, const p256_pt *Pp) {
    buf[0] = 0x04;
    le32_to_be32(Pp->X, buf + 1);
    le32_to_be32(Pp->Y, buf + 33);
}

/* ---------- BearSSL ec_impl API ---------- */

static const unsigned char *api_generator(int curve, size_t *len) {
    (void)curve;
    *len = br_secp256r1.generator_len;
    return br_secp256r1.generator;
}

static const unsigned char *api_order(int curve, size_t *len) {
    (void)curve;
    *len = br_secp256r1.order_len;
    return br_secp256r1.order;
}

static size_t api_xoff(int curve, size_t *len) {
    (void)curve;
    *len = 32;
    return 1;
}

static uint32_t api_mul(unsigned char *G, size_t Glen,
                        const unsigned char *x, size_t xlen,
                        int curve) {
    (void)curve;
    p256_pt Pp, R;
    if (!load_point_uncompressed(&Pp, G, Glen)) return 0;

    scalar_mul_point(&R, &Pp, x, xlen);
    to_affine(&R);
    store_point_uncompressed(G, &R);
    return 1;
}

static size_t api_mulgen(unsigned char *Rbuf,
                         const unsigned char *x, size_t xlen,
                         int curve) {
    (void)curve;
    p256_pt Gp, R;
    load_generator(&Gp);

    scalar_mul_point(&R, &Gp, x, xlen);
    to_affine(&R);
    store_point_uncompressed(Rbuf, &R);
    return 65;
}

static uint32_t api_muladd(unsigned char *A, const unsigned char *B,
                           size_t Glen,
                           const unsigned char *x, size_t xlen,
                           const unsigned char *y, size_t ylen,
                           int curve) {
    (void)curve;
    p256_pt Pp, Qp, R, T;

    if (!load_point_uncompressed(&Pp, A, Glen)) return 0;

    scalar_mul_point(&R, &Pp, x, xlen);

    if (B) {
        if (!load_point_uncompressed(&Qp, B, Glen)) return 0;
    } else {
        load_generator(&Qp);
    }

    scalar_mul_point(&T, &Qp, y, ylen);
    p256_point_add(&R, &R, &T);

    to_affine(&R);
    store_point_uncompressed(A, &R);
    return 1;
}

const br_ec_impl br_ec_p256_m15 PROGMEM = {
    (uint32_t)0x00800000,
    &api_generator,
    &api_order,
    &api_xoff,
    &api_mul,
    &api_mulgen,
    &api_muladd
};

#endif // SOC_MPI_SUPPORTED
#endif // defined(ESP_PLATFORM) && !defined(ESP8266)
#endif // USE_SHA_ROM