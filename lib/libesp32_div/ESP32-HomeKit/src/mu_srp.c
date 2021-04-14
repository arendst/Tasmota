#include <string.h>
#include "hkdf-sha.h"
#include "mu_bignum.h"
#include "mu_srp.h"

#ifdef SRP_DEBUG
#include <stdio.h>
#define srp_print   printf
static void hex_dbg(char *name, void *buf, int buf_len)
{
	char *p = (char *)buf;
	int i;
	srp_print("%s (%d): ", name, buf_len);
	for (i = 0; i < buf_len; i++) {
		if (i % 16 == 0)
			srp_print("\r\n");
		srp_print("%02x ", (unsigned)(unsigned char)p[i]);
	}
	srp_print("\r\n");
}

static void hex_dbg_bn(char *name, mu_bn_t *bn)
{
	int len;
	char *str = mu_bn_to_bin(bn, &len);
	if (str) {
		hex_dbg(name, str, len);
		free(str);
	}
}
#else
#define srp_print(...)
#define hex_dbg(...)
#define hex_dbg_bn(...)
#endif

static inline void SHA512_hash(const uint8_t *bytes, unsigned int byte_count,
		     uint8_t digest[SHA512HashSize])
{
	SHA512Context ctx;
	SHA512Reset(&ctx);
	SHA512Input(&ctx, bytes, byte_count);
	SHA512Result(&ctx, digest);
}

/************************* SRP Stuff */
char N_3072[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2,
	0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67,
	0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E,
	0x34, 0x04, 0xDD, 0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
	0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5,
	0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF,
	0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED, 0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE,
	0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
	0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3,
	0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F, 0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3,
	0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70,
	0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
	0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B, 0xE3, 0x9E, 0x77,
	0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5,
	0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39,
	0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
	0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A,
	0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64, 0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB,
	0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6,
	0xE1, 0xE4, 0xC7, 0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
	0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B, 0xF1, 0x2F, 0xFA,
	0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F,
	0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C, 0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77,
	0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
	0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2,
	0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
char g_3072[] = { 5 };


int mu_srp_init(mu_srp_handle_t *hd, mu_ng_type_t ng)
{
	if (hd->allocated)
		mu_srp_free(hd);

	memset(hd, 0, sizeof(*hd));
	hd->allocated = 1;

	hd->ctx = mu_bn_ctx_new();
	if (! hd->ctx)
		goto error;
	if (ng != MU_NG_3072)
		goto error;

	hd->n = mu_bn_new_from_bin(N_3072, sizeof(N_3072));
	hd->bytes_n = N_3072;
	hd->len_n = sizeof(N_3072);
	if (! hd->n)
		goto error;

	hd->g = mu_bn_new_from_bin(g_3072, sizeof(g_3072));
	hd->bytes_g = g_3072;
	hd->len_g = sizeof(g_3072);
	if (! hd->g)
		goto error;
	hd->type = ng;
	return 0;
 error:
	mu_srp_free(hd);
	return -1;
}

void mu_srp_free(mu_srp_handle_t *hd)
{
	if (hd->allocated != 1)
		return;

	if (hd->ctx)
		mu_bn_ctx_free(hd->ctx);
	if (hd->n)
		mu_bn_free(hd->n);
	if (hd->g)
		mu_bn_free(hd->g);
	if (hd->s)
		mu_bn_free(hd->s);
	if (hd->bytes_s)
		free(hd->bytes_s);
	if (hd->v)
		mu_bn_free(hd->v);
	if (hd->B)
		mu_bn_free(hd->B);
	if (hd->bytes_B)
		free(hd->bytes_B);
	if (hd->b)
		mu_bn_free(hd->b);
	if (hd->A)
		mu_bn_free(hd->A);
	if (hd->bytes_A)
		free(hd->bytes_A);
	if (hd->session_key)
		free(hd->session_key);
	memset(hd, 0, sizeof(*hd));
}

static mu_bn_t *calculate_x(char *bytes_salt, int salt_len, const char *username, const char *pass, int pass_len)
{
	unsigned char digest[SHA512HashSize];
	SHA512Context ctx;
	srp_print("username:%s:\npass:%s:\npass_len:%d:\n", username, pass, pass_len);
	hex_dbg("salt", bytes_salt, salt_len);
	SHA512Reset(&ctx);
	SHA512Input(&ctx, (unsigned char *)username, strlen(username));
	SHA512Input(&ctx, (unsigned char *)":", 1);
	SHA512Input(&ctx, (unsigned char *)pass, pass_len);
	SHA512Result(&ctx, digest);
	
	SHA512Reset(&ctx);
	SHA512Input(&ctx, (unsigned char *)bytes_salt, salt_len);
	SHA512Input(&ctx, digest, sizeof(digest));
	SHA512Result(&ctx, digest);

	hex_dbg("Digest", digest, sizeof(digest));
	return mu_bn_new_from_bin((char *)digest, sizeof(digest));
}

static mu_bn_t *calculate_padded_hash(mu_srp_handle_t *hd, char *a, int len_a, char *b, int len_b)
{
	unsigned char digest[SHA512HashSize];
	SHA512Context ctx;
	int pad_len;
	char *s = NULL;

	if (len_a > len_b) {
		pad_len = hd->len_n - len_b;
	} else {
		pad_len = hd->len_n - len_a;
	}

    if (pad_len) {
        s = malloc(pad_len);
        if (s) {
            memset(s, 0, pad_len);
        }
    }

	SHA512Reset(&ctx);
	/* PAD (a) */
	if (s && (len_a != hd->len_n)) {
		SHA512Input(&ctx, (unsigned char *)s, hd->len_n - len_a);
	}

	SHA512Input(&ctx, (unsigned char *)a, len_a);

	/* PAD (b) */
	if (s && (len_b != hd->len_n)) {
		SHA512Input(&ctx, (unsigned char *)s, hd->len_n - len_b);
	}

	SHA512Input(&ctx, (unsigned char *)b, len_b);

	SHA512Result(&ctx, digest);

    if (s) {
        free(s);
    }

	hex_dbg("value", digest, sizeof(digest));
	return mu_bn_new_from_bin((char *)digest, sizeof(digest));
}

/* k = SHA (N, PAD(g))
 *
 * https://tools.ietf.org/html/draft-ietf-tls-srp-08
 */
static mu_bn_t *calculate_k(mu_srp_handle_t *hd)
{
	srp_print("k-->");
	return calculate_padded_hash(hd, hd->bytes_n, hd->len_n, hd->bytes_g, hd->len_g);
}

static mu_bn_t *calculate_u(mu_srp_handle_t *hd, char *A, int len_A)
{
	srp_print("u-->");
	return calculate_padded_hash(hd, A, len_A, hd->bytes_B, hd->len_B);
}

int __mu_srp_srv_pubkey(mu_srp_handle_t *hd, char **bytes_B, int *len_B)
{
	mu_bn_t *k = calculate_k(hd);
	mu_bn_t *kv = NULL;
	mu_bn_t *gb = NULL;
	if (!k)
		goto error;

	hd->b = mu_bn_new();
	if (!hd->b)
		goto error;
	mu_bn_get_rand(hd->b, 256, -1, 0);
	hex_dbg_bn("b", hd->b);

	/* B = kv + g^b */
	kv = mu_bn_new();
	gb = mu_bn_new();
	hd->B = mu_bn_new();
	if (!kv || !gb || ! hd->B)
		goto error;
	mu_bn_a_mul_b_mod_c(kv, k, hd->v, hd->n, hd->ctx);
	mu_bn_a_exp_b_mod_c(gb, hd->g, hd->b, hd->n, hd->ctx);
	mu_bn_a_add_b_mod_c(hd->B, kv, gb, hd->n, hd->ctx);
	hd->bytes_B = mu_bn_to_bin(hd->B, len_B);
	hd->len_B = *len_B;
	*bytes_B = hd->bytes_B;

	mu_bn_free(k);
	mu_bn_free(kv);
	mu_bn_free(gb);
	return 0;
 error:
	if (k)
		mu_bn_free(k);
	if (kv)
		mu_bn_free(kv);
	if (gb)
		mu_bn_free(gb);
	if (hd->B) {
		mu_bn_free(hd->B);
		hd->B = NULL;
	}
	if (hd->b) {
		mu_bn_free(hd->b);
		hd->b = NULL;
	}
	return -1;
	
}

int mu_srp_srv_pubkey(mu_srp_handle_t *hd, const char *username, const char *pass, int pass_len, int salt_len,
		      char **bytes_B, int *len_B, char **bytes_salt)
{
	/* Get Salt */
	int str_salt_len;
	mu_bn_t *x = NULL;
	hd->s = mu_bn_new();
	if (! hd->s)
		goto error;
	mu_bn_get_rand(hd->s, 8 * salt_len, -1, 0);
	*bytes_salt = mu_bn_to_bin(hd->s, &str_salt_len);
	if (! *bytes_salt)
		goto error;
	hd->bytes_s = *bytes_salt;
	hd->len_s = salt_len;
	hex_dbg("Salt", *bytes_salt, str_salt_len);

	/* Calculate X which is simply a hash for all these things */
	x = calculate_x(*bytes_salt, str_salt_len, username, pass, pass_len);
	if (! x)
		goto error;
	hex_dbg_bn("x", x);
	
	/* v = g^x % N */
	hd->v = mu_bn_new();
	if (! hd->v)
		goto error;
	mu_bn_a_exp_b_mod_c(hd->v, hd->g, x, hd->n, hd->ctx);
	hex_dbg_bn("Verifier", hd->v);

	if (__mu_srp_srv_pubkey(hd, bytes_B, len_B) < 0 )
		goto error;

	mu_bn_free(x);
	return 0;

error:
	if (hd->s) {
		mu_bn_free(hd->s);
		hd->s = NULL;
	}
	if (*bytes_salt) {
		free(*bytes_salt);
		*bytes_salt = NULL;
		hd->bytes_s = NULL;
		hd->len_s = 0;
	}
	if (x) {
		mu_bn_free(x);
		x = NULL;
	}
	if (hd->v) {
		mu_bn_free(hd->v);
		hd->v = NULL;
	}
	return -1;
}

int mu_srp_srv_pubkey_from_salt_verifier(mu_srp_handle_t *hd, char **bytes_B, int *len_B)
{
    return __mu_srp_srv_pubkey(hd, bytes_B, len_B);
}

int mu_srp_set_salt_verifier(mu_srp_handle_t *hd, const char *salt, int salt_len,
        const char *verifier, int verifier_len)
{
    hd->bytes_s = malloc(salt_len);
    if (!hd->bytes_s) {
        goto error;
    }
    memcpy(hd->bytes_s, salt, salt_len);
    hd->len_s = salt_len;

    hd->s = mu_bn_new_from_bin(salt, salt_len);
    if (!hd->s) {
        goto error;
    }

	hd->v = mu_bn_new_from_bin(verifier, verifier_len);
    if (!hd->v) {
        goto error;
    }
    return 0;

error:
    if (hd->bytes_s) {
        free(hd->bytes_s);
        hd->bytes_s = NULL;
        hd->len_s = 0;
    }
    if (hd->s) {
		mu_bn_free(hd->s);
		hd->s = NULL;
    }
	if (hd->v) {
		mu_bn_free(hd->v);
		hd->v = NULL;
	}
	return -1;
}

int mu_srp_get_session_key(mu_srp_handle_t *hd, char *bytes_A, int len_A, char **bytes_key, int *len_key)
{
	mu_bn_t *u, *vu, *avu, *S;
	char *bytes_S;
	int len_S;

	u = vu = avu = S = NULL;
	bytes_S = NULL;

	hd->bytes_A = malloc(len_A);
	if (! hd->bytes_A)
		goto error;
	memcpy(hd->bytes_A, bytes_A, len_A);
	hd->len_A = len_A;

	hd->A = mu_bn_new_from_bin(bytes_A, len_A);
	if (! hd->A)
		goto error;
	u = calculate_u(hd, bytes_A, len_A);
	if (! u)
		goto error;

	/* S = (A v^u)^b */
	vu = mu_bn_new();
	avu = mu_bn_new();
	S = mu_bn_new();
	if (!vu || !avu || !S )
		goto error;

	mu_bn_a_exp_b_mod_c(vu, hd->v, u, hd->n, hd->ctx);
	mu_bn_a_mul_b_mod_c(avu, hd->A, vu, hd->n, hd->ctx);
	mu_bn_a_exp_b_mod_c(S, avu, hd->b, hd->n, hd->ctx);
	hex_dbg_bn("S", S);

	bytes_S = mu_bn_to_bin(S, &len_S);
	hd->session_key = malloc(SHA512HashSize);
	if (!hd->session_key || ! bytes_S)
		goto error;

	SHA512_hash((unsigned char *)bytes_S, len_S, (unsigned char *)hd->session_key);
	*bytes_key = hd->session_key;
	*len_key = SHA512HashSize;
	
	free(bytes_S);
	mu_bn_free(vu);
	mu_bn_free(avu);
	mu_bn_free(S);
	mu_bn_free(u);
	return 0;
 error:
	if (bytes_S)
		free(bytes_S);
	if (vu)
		mu_bn_free(vu);
	if (avu)
		mu_bn_free(avu);
	if (S)
		mu_bn_free(S);
	if (u)
		mu_bn_free(u);
	if (hd->session_key) {
		free(hd->session_key);
		hd->session_key = NULL;
	}
	if (hd->A) {
		mu_bn_free(hd->A);
		hd->A = NULL;
	}
	if (hd->bytes_A) {
		free(hd->bytes_A);
		hd->bytes_A = NULL;
	}
	return -1;
}

int mu_srp_exchange_proofs(mu_srp_handle_t *hd, char *username, char *bytes_user_proof, char *bytes_host_proof)
{
	/* First calculate M */
	unsigned char hash_n[SHA512HashSize];
	unsigned char hash_g[SHA512HashSize];
	unsigned char hash_n_xor_g[SHA512HashSize];
	int i;
	SHA512_hash((unsigned char *)hd->bytes_n, hd->len_n, (unsigned char *)hash_n);
	SHA512_hash((unsigned char *)hd->bytes_g, hd->len_g, (unsigned char *)hash_g);
	for (i = 0; i < SHA512HashSize; i++)
		hash_n_xor_g[i] = hash_n[i] ^ hash_g[i];

	unsigned char hash_I[SHA512HashSize];
	SHA512_hash((unsigned char *)username, strlen(username), (unsigned char *)hash_I);
	
	SHA512Context ctx;
	unsigned char digest[SHA512HashSize];
	SHA512Reset(&ctx);
	SHA512Input(&ctx, hash_n_xor_g, SHA512HashSize);
	SHA512Input(&ctx, hash_I, SHA512HashSize);
	SHA512Input(&ctx, (unsigned char *)hd->bytes_s, hd->len_s);
	SHA512Input(&ctx, (unsigned char *)hd->bytes_A, hd->len_A);
	SHA512Input(&ctx, (unsigned char *)hd->bytes_B, hd->len_B);
	SHA512Input(&ctx, (unsigned char *)hd->session_key, SHA512HashSize);
	SHA512Result(&ctx, digest);

	hex_dbg("M", digest, sizeof(digest));
	if (memcmp(bytes_user_proof, digest, SHA512HashSize) != 0)
		return false;
	/* M is now validated, let's proceed to H(AMK) */
	SHA512Reset(&ctx);
	SHA512Input(&ctx, (unsigned char *)hd->bytes_A, hd->len_A);
	SHA512Input(&ctx, digest, SHA512HashSize);
	SHA512Input(&ctx, (unsigned char *)hd->session_key, SHA512HashSize);
	SHA512Result(&ctx, (unsigned char *)bytes_host_proof);
	hex_dbg("AMK", bytes_host_proof, SHA512HashSize);

	return true;
}
/************************* SRP Stuff Ends */
