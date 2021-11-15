#ifndef _MU_SRP_H_
#define _MU_SRP_H_

#include <stdbool.h>
#include "mu_bignum.h"

typedef enum {
	MU_NG_3072 = 0,
} mu_ng_type_t;

typedef struct mu_srp_handle {
	int allocated;
	mu_ng_type_t type;
	mu_bn_ctx_t *ctx;

	/* N
	 * the bytes_n simply points to the static array
	 */
	mu_bn_t *n;
	char    *bytes_n;
	int      len_n;

	/* g
	 * the bytes_g simply points to the static array
	 */
	mu_bn_t *g;
	char    *bytes_g;
	int      len_g;

	/* Salt */
	mu_bn_t *s;
	char    *bytes_s;
	int      len_s;
	/* Verifier */
	mu_bn_t *v;
	/* B */
	mu_bn_t *B;
	char    *bytes_B;
	int      len_B;
	/* b */
	mu_bn_t *b;
	/* A */
	mu_bn_t *A;
	char    *bytes_A;
	int      len_A;
	/* K - session key*/
	char *session_key;
} mu_srp_handle_t;

int mu_srp_init(mu_srp_handle_t *hd, mu_ng_type_t ng);

void mu_srp_free(mu_srp_handle_t *hd);
/* Returns B (pub key) and salt
 *
 * *bytes_B MUST NOT BE FREED BY THE CALLER
 * *bytes_salt MUST NOT BE FREE BY THE CALLER
 *
 */
int mu_srp_srv_pubkey(mu_srp_handle_t *hd, const char *username, const char *pass, int pass_len, int salt_len,
		      char **bytes_B, int *len_B, char **bytes_salt);

/* Set the Salt and Verifier pre-generated for a given password.
 * This should be used only if the actual password is not available.
 * The public key can then be generated using mu_srp_srv_pubkey_from_salt_verifier()
 * and not mu_srp_srv_pubkey()
 */
int mu_srp_set_salt_verifier(mu_srp_handle_t *hd, const char *salt, int salt_len,
            const char *verifier, int verifier_len);

/* Returns B (pub key) when the salt and verifier are set using mu_srp_set_salt_verifier()
 *
 * *bytes_B MUST NOT BE FREED BY THE CALLER
 */
int mu_srp_srv_pubkey_from_salt_verifier(mu_srp_handle_t *hd, char **bytes_B, int *len_B);

/* Returns bytes_key
 * *bytes_key MUST NOT BE FREED BY THE CALLER
 */
int mu_srp_get_session_key(mu_srp_handle_t *hd, char *bytes_A, int len_A, char **bytes_key, int *len_key);

/* Exchange proofs
 * Returns 1 if user's proof is ok. Also 1 when is returned, bytes_host_proof contains our proof.
 *
 * bytes_user_proof is parameter in
 * bytes_host_proof is parameter out (should be SHA512_DIGEST_LENGTH) bytes in size
 */
int mu_srp_exchange_proofs(mu_srp_handle_t *hd, const char *username, char *bytes_user_proof, char *bytes_host_proof);



#endif /* ! _MU_SRP_H_ */
