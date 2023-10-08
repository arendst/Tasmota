#ifndef _MU_BIGNUM_H_
#define _MU_BIGNUM_H_

#define BIGNUM_MBEDTLS

#ifdef BIGNUM_OPENSSL
#include <openssl/bn.h>


typedef BIGNUM mu_bn_t;
typedef BN_CTX mu_bn_ctx_t;


static inline mu_bn_t *mu_bn_new_from_hex(const char *hex)
{
     mu_bn_t *a = BN_new();
     if (a)
	  BN_hex2bn(&a, hex);
     return a;
}

static inline mu_bn_t *mu_bn_new_from_bin(const char *str, int str_len)
{
  return BN_bin2bn((unsigned char *)str, str_len, NULL);
}

static inline mu_bn_t *mu_bn_new()
{
     return BN_new();
}

static inline void mu_bn_free(mu_bn_t *bn)
{
     return BN_free(bn);
}

static inline mu_bn_ctx_t *mu_bn_ctx_new()
{
     return BN_CTX_new();
}

static inline void mu_bn_ctx_free(mu_bn_ctx_t *ctx)
{
     return BN_CTX_free(ctx);
}

static inline unsigned int mu_bn_sizeof(mu_bn_t *bn)
{
     return BN_num_bytes(bn);
}


static inline char *mu_bn_to_bin(mu_bn_t *bn, int *len)
{
	*len = mu_bn_sizeof(bn);
	char *p = malloc(*len);
	if (p) {
		BN_bn2bin(bn, (unsigned char *)p);
	}
	return p;
}

static inline int mu_bn_get_rand(mu_bn_t *bn, int bits, int top, int bottom)
{
	return BN_rand(bn, bits, top, bottom);
}

static inline int mu_bn_a_exp_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
	return BN_mod_exp(result, a, b, c, ctx);
}

static inline int mu_bn_a_mul_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
	return BN_mod_mul(result, a, b, c, ctx);
}

static inline int mu_bn_a_add_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
	if (BN_add(result, a, b) != 1)
		return 1;
	return BN_mod(result, result, c, ctx);
}
#endif /* BIGNUM_OPENSSL */


#ifdef BIGNUM_MBEDTLS
#include <mbedtls/bignum.h>
#include <esp_system.h>
#ifdef CONFIG_IDF_TARGET_ESP8266
#include <driver/rtc.h>
#endif
typedef mbedtls_mpi mu_bn_t;
typedef mu_bn_t mu_bn_ctx_t;

static inline mu_bn_t *mu_bn_new()
{
    mu_bn_t *a = malloc(sizeof (mu_bn_t));
    if (!a)
        return NULL;
    mbedtls_mpi_init(a);
    return a;
}
static inline mu_bn_t *mu_bn_new_from_hex(const char *hex)
{
    mu_bn_t *a = mu_bn_new();
    if (!a)
        return NULL;

    mbedtls_mpi_read_string(a, 16, hex);
    return a;
}

static inline mu_bn_t *mu_bn_new_from_bin(const char *str, int str_len)
{

    mu_bn_t *a = mu_bn_new();
    if (!a) {
        return NULL;
    }
    mbedtls_mpi_read_binary(a, (unsigned char *)str, str_len);
    return a;
}


static inline void mu_bn_free(mu_bn_t *bn)
{
    if (bn) {
        mbedtls_mpi_free(bn);
        free(bn);
    }
}

static inline mu_bn_ctx_t *mu_bn_ctx_new()
{
    mu_bn_t *bn = mu_bn_new();
    return ( mu_bn_ctx_t *)bn;
}

static inline void mu_bn_ctx_free(mu_bn_ctx_t *ctx)
{
    mu_bn_free((mu_bn_t *)ctx);
}

static inline unsigned int mu_bn_sizeof(mu_bn_t *bn)
{
    return mbedtls_mpi_size(bn);
}


static inline char *mu_bn_to_bin(mu_bn_t *bn, int *len)
{
	*len = mu_bn_sizeof(bn);
	char *p = malloc(*len);
	if (p) {
        mbedtls_mpi_write_binary(bn, (unsigned char *)p, *len);
	}
	return p;
}

static inline int mu_get_random(void *ctx, unsigned char *data, size_t len)
{
    esp_fill_random(data, len);
    return 0;
}
static inline int mu_bn_get_rand(mu_bn_t *bn, int bits, int top, int bottom)
{

    return mbedtls_mpi_fill_random(bn, bits / 8,  mu_get_random, NULL);
}

#ifdef CONFIG_IDF_TARGET_ESP8266
static inline int mu_bn_a_exp_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
    int ret;
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
    ret =  mbedtls_mpi_exp_mod(result, a, b, c, (mu_bn_t *) ctx);
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    return ret;
}
static inline int mu_bn_a_mul_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
    mbedtls_mpi tmp_result;
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
    mbedtls_mpi_init(&tmp_result);
    mbedtls_mpi_mul_mpi(&tmp_result, a, b);
    mbedtls_mpi_mod_mpi(result, &tmp_result, c);
    mbedtls_mpi_free(&tmp_result);
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    return 0;
}
#else

#include "port/bignum.h"
#include "port/bignum_impl.h"

static inline int mu_bn_a_exp_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
    //return mbedtls_mpi_exp_mod(result, a, b, c, (mu_bn_t *) ctx);
	// wangbin changed
	printf("esp_mpi_exp_mod\n");
	return esp_mpi_exp_mod(result, a, b, c, (mu_bn_t *) ctx);
}


static inline int mu_bn_a_mul_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
	printf("esp_mpi_mul_mpi_mod\n");
    return esp_mpi_mul_mpi_mod(result, a, b, c);
}
#endif /* !CONFIG_IDF_TARGET_ESP8266 */

static inline int mu_bn_a_add_b_mod_c(mu_bn_t *result, mu_bn_t *a, mu_bn_t *b, mu_bn_t *c, mu_bn_ctx_t *ctx)
{
    int     res;
    mbedtls_mpi  t;
#ifdef CONFIG_IDF_TARGET_ESP8266
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
#endif
    mbedtls_mpi_init(&t);
    res = mbedtls_mpi_add_mpi(&t, a, b);
    if (res == 0) {
        res = mbedtls_mpi_mod_mpi(result, &t, c);
    }
    mbedtls_mpi_free(&t);
#ifdef CONFIG_IDF_TARGET_ESP8266
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
#endif
    return res;
}
#endif /* BIGNUM_MBEDTLS */
#endif /* ! _MU_BIGNUM_H_ */
