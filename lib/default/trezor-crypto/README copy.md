[ed25519](http://ed25519.cr.yp.to/) is an 
[Elliptic Curve Digital Signature Algortithm](http://en.wikipedia.org/wiki/Elliptic_Curve_DSA), 
developed by [Dan Bernstein](http://cr.yp.to/djb.html), 
[Niels Duif](http://www.nielsduif.nl/), 
[Tanja Lange](http://hyperelliptic.org/tanja), 
[Peter Schwabe](http://www.cryptojedi.org/users/peter/), 
and [Bo-Yin Yang](http://www.iis.sinica.edu.tw/pages/byyang/).

This project provides performant, portable 32-bit & 64-bit implementations. All implementations are 
of course constant time in regard to secret data.

#### Performance

SSE2 code and benches have not been updated yet. I will do those next.

Compilers versions are gcc 4.6.3, icc 13.1.1, clang 3.4-1~exp1.

Batch verification time (in parentheses) is the average time per 1 verification in a batch of 64 signatures. Counts are in thousands of cycles.

Note that SSE2 performance may be less impressive on AMD & older CPUs with slower SSE ops!

Visual Studio performance for `ge25519_scalarmult_base_niels` will lag behind a bit until optimized assembler versions of `ge25519_scalarmult_base_choose_niels`
are made.

##### E5200 @ 2.5ghz, march=core2

<table>
<thead><tr><th>Implementation</th><th>Sign</th><th>gcc</th><th>icc</th><th>clang</th><th>Verify</th><th>gcc</th><th>icc</th><th>clang</th></tr></thead>
<tbody>
<tr><td>ed25519-donna 64bit     </td><td></td><td>100k</td><td>110k</td><td>137k</td><td></td><td>327k (144k) </td><td>342k (163k) </td><td>422k (194k) </td></tr>
<tr><td>amd64-64-24k            </td><td></td><td>102k</td><td>    </td><td>    </td><td></td><td>355k (158k) </td><td>            </td><td>            </td></tr>
<tr><td>ed25519-donna-sse2 64bit</td><td></td><td>108k</td><td>111k</td><td>116k</td><td></td><td>353k (155k) </td><td>345k (154k) </td><td>360k (161k) </td></tr>
<tr><td>amd64-51-32k            </td><td></td><td>116k</td><td>    </td><td>    </td><td></td><td>380k (175k) </td><td>            </td><td>            </td></tr>
<tr><td>ed25519-donna-sse2 32bit</td><td></td><td>147k</td><td>147k</td><td>156k</td><td></td><td>380k (178k) </td><td>381k (173k) </td><td>430k (192k) </td></tr>
<tr><td>ed25519-donna 32bit     </td><td></td><td>597k</td><td>335k</td><td>380k</td><td></td><td>1693k (720k)</td><td>1052k (453k)</td><td>1141k (493k)</td></tr>
</tbody>
</table>

##### E3-1270 @ 3.4ghz, march=corei7-avx

<table>
<thead><tr><th>Implementation</th><th>Sign</th><th>gcc</th><th>icc</th><th>clang</th><th>Verify</th><th>gcc</th><th>icc</th><th>clang</th></tr></thead>
<tbody>
<tr><td>amd64-64-24k            </td><td></td><td> 68k</td><td>    </td><td>    </td><td></td><td>225k (104k) </td><td>            </td><td>            </td></tr>
<tr><td>ed25519-donna 64bit     </td><td></td><td> 71k</td><td> 75k</td><td> 90k</td><td></td><td>226k (105k) </td><td>226k (112k) </td><td>277k (125k) </td></tr>
<tr><td>amd64-51-32k            </td><td></td><td> 72k</td><td>    </td><td>    </td><td></td><td>218k (107k) </td><td>            </td><td>            </td></tr>
<tr><td>ed25519-donna-sse2 64bit</td><td></td><td> 79k</td><td> 82k</td><td> 92k</td><td></td><td>252k (122k) </td><td>259k (124k) </td><td>282k (131k) </td></tr>
<tr><td>ed25519-donna-sse2 32bit</td><td></td><td> 94k</td><td> 95k</td><td>103k</td><td></td><td>296k (146k) </td><td>294k (137k) </td><td>306k (147k) </td></tr>
<tr><td>ed25519-donna 32bit     </td><td></td><td>525k</td><td>299k</td><td>316k</td><td></td><td>1502k (645k)</td><td>959k (418k) </td><td>954k (416k) </td></tr>
</tbody>
</table>

#### Compilation

No configuration is needed **if you are compiling against OpenSSL**. 

##### Hash Options

If you are not compiling aginst OpenSSL, you will need a hash function.

To use a simple/**slow** implementation of SHA-512, use `-DED25519_REFHASH` when compiling `ed25519.c`. 
This should never be used except to verify the code works when OpenSSL is not available.

To use a custom hash function, use `-DED25519_CUSTOMHASH` when compiling `ed25519.c` and put your 
custom hash implementation in ed25519-hash-custom.h. The hash must have a 512bit digest and implement

	struct ed25519_hash_context;

	void ed25519_hash_init(ed25519_hash_context *ctx);
	void ed25519_hash_update(ed25519_hash_context *ctx, const uint8_t *in, size_t inlen);
	void ed25519_hash_final(ed25519_hash_context *ctx, uint8_t *hash);
	void ed25519_hash(uint8_t *hash, const uint8_t *in, size_t inlen);

##### Random Options

If you are not compiling aginst OpenSSL, you will need a random function for batch verification.

To use a custom random function, use `-DED25519_CUSTOMRANDOM` when compiling `ed25519.c` and put your 
custom hash implementation in ed25519-randombytes-custom.h. The random function must implement:

	void ED25519_FN(ed25519_randombytes_unsafe) (void *p, size_t len);

Use `-DED25519_TEST` when compiling `ed25519.c` to use a deterministically seeded, non-thread safe CSPRNG 
variant of Bob Jenkins [ISAAC](http://en.wikipedia.org/wiki/ISAAC_%28cipher%29)

##### Minor options

Use `-DED25519_INLINE_ASM` to disable the use of custom assembler routines and instead rely on portable C.

Use `-DED25519_FORCE_32BIT` to force the use of 32 bit routines even when compiling for 64 bit.

##### 32-bit

	gcc ed25519.c -m32 -O3 -c

##### 64-bit

	gcc ed25519.c -m64 -O3 -c

##### SSE2

	gcc ed25519.c -m32 -O3 -c -DED25519_SSE2 -msse2
	gcc ed25519.c -m64 -O3 -c -DED25519_SSE2

clang and icc are also supported


#### Usage

To use the code, link against `ed25519.o -mbits` and:

	#include "ed25519.h"

Add `-lssl -lcrypto` when using OpenSSL (Some systems don't need -lcrypto? It might be trial and error).

To generate a private key, simply generate 32 bytes from a secure
cryptographic source:

	ed25519_secret_key sk;
	randombytes(sk, sizeof(ed25519_secret_key));

To generate a public key:

	ed25519_public_key pk;
	ed25519_publickey(sk, pk);

To sign a message:

	ed25519_signature sig;
	ed25519_sign(message, message_len, sk, pk, signature);

To verify a signature:

	int valid = ed25519_sign_open(message, message_len, pk, signature) == 0;

To batch verify signatures:

	const unsigned char *mp[num] = {message1, message2..}
	size_t ml[num] = {message_len1, message_len2..}
	const unsigned char *pkp[num] = {pk1, pk2..}
	const unsigned char *sigp[num] = {signature1, signature2..}
	int valid[num]

	/* valid[i] will be set to 1 if the individual signature was valid, 0 otherwise */
	int all_valid = ed25519_sign_open_batch(mp, ml, pkp, sigp, num, valid) == 0;

**Note**: Batch verification uses `ed25519_randombytes_unsafe`, implemented in 
`ed25519-randombytes.h`, to generate random scalars for the verification code. 
The default implementation now uses OpenSSLs `RAND_bytes`.

Unlike the [SUPERCOP](http://bench.cr.yp.to/supercop.html) version, signatures are
not appended to messages, and there is no need for padding in front of messages. 
Additionally, the secret key does not contain a copy of the public key, so it is 
32 bytes instead of 64 bytes, and the public key must be provided to the signing
function.

##### Curve25519

Curve25519 public keys can be generated thanks to 
[Adam Langley](http://www.imperialviolet.org/2013/05/10/fastercurve25519.html) 
leveraging Ed25519's precomputed basepoint scalar multiplication.

	curved25519_key sk, pk;
	randombytes(sk, sizeof(curved25519_key));
	curved25519_scalarmult_basepoint(pk, sk);

Note the name is curved25519, a combination of curve and ed25519, to prevent 
name clashes. Performance is slightly faster than short message ed25519
signing due to both using the same code for the scalar multiply.

#### Testing

Fuzzing against reference implemenations is now available. See [fuzz/README](fuzz/README.md).

Building `ed25519.c` with `-DED25519_TEST` and linking with `test.c` will run basic sanity tests
and benchmark each function. `test-batch.c` has been incorporated in to `test.c`.

`test-internals.c` is standalone and built the same way as `ed25519.c`. It tests the math primitives
with extreme values to ensure they function correctly. SSE2 is now supported.

#### Papers

[Available on the Ed25519 website](http://ed25519.cr.yp.to/papers.html)