
TinyCrypt Cryptographic Library
###############################
Copyright (C) 2017 by Intel Corporation, All Rights Reserved.

Overview
********
The TinyCrypt Library provides an implementation for targeting constrained devices
with a minimal set of standard cryptography primitives, as listed below. To better
serve applications targeting constrained devices, TinyCrypt implementations differ
from the standard specifications (see the Important Remarks section for some
important differences). Certain cryptographic primitives depend on other
primitives, as mentioned in the list below.

Aside from the Important Remarks section below, valuable information on the usage,
security and technicalities of each cryptographic primitive are found in the
corresponding header file.

* SHA-256:

  * Type of primitive: Hash function.
  * Standard Specification: NIST FIPS PUB 180-4.
  * Requires: --

* HMAC-SHA256:

  * Type of primitive: Message authentication code.
  * Standard Specification: RFC 2104.
  * Requires: SHA-256

* HMAC-PRNG:

  * Type of primitive: Pseudo-random number generator (256-bit strength).
  * Standard Specification: NIST SP 800-90A.
  * Requires: SHA-256 and HMAC-SHA256.

* AES-128:

  * Type of primitive: Block cipher.
  * Standard Specification: NIST FIPS PUB 197.
  * Requires: --

* AES-CBC mode:

  * Type of primitive: Encryption mode of operation.
  * Standard Specification: NIST SP 800-38A.
  * Requires: AES-128.

* AES-CTR mode:

  * Type of primitive: Encryption mode of operation.
  * Standard Specification: NIST SP 800-38A.
  * Requires: AES-128.

* AES-CMAC mode:

  * Type of primitive: Message authentication code.
  * Standard Specification: NIST SP 800-38B.
  * Requires: AES-128.

* AES-CCM mode:

  * Type of primitive: Authenticated encryption.
  * Standard Specification: NIST SP 800-38C.
  * Requires: AES-128.

* CTR-PRNG:

  * Type of primitive: Pseudo-random number generator (128-bit strength).
  * Standard Specification: NIST SP 800-90A.
  * Requires: AES-128.
  
* ECC-DH:

  * Type of primitive: Key exchange based on curve NIST p-256.
  * Standard Specification: RFC 6090.
  * Requires: ECC auxiliary functions (ecc.h/c).

* ECC-DSA:

  * Type of primitive: Digital signature based on curve NIST p-256.
  * Standard Specification: RFC 6090.
  * Requires: ECC auxiliary functions (ecc.h/c).

Design Goals
************

* Minimize the code size of each cryptographic primitive. This means minimize
 the size of a platform-independent implementation, as presented in TinyCrypt.
 Note that various applications may require further features, optimizations with
 respect to other metrics and countermeasures for particular threats. These
 peculiarities would increase the code size and thus are not considered here.

* Minimize the dependencies among the cryptographic primitives. This means
 that it is unnecessary to build and allocate object code for more primitives
 than the ones strictly required by the intended application. In other words,
 one can select and compile only the primitives required by the application.


Important Remarks
*****************

The cryptographic implementations in TinyCrypt library have some limitations.
Some of these limitations are inherent to the cryptographic primitives
themselves, while others are specific to TinyCrypt. These limitations were accepted
in order to meet its design goals (in special, minimal code size) and to better 
serve applications targeting constrained devices in general. Some of these 
limitations are discussed in-depth below.

General Remarks
***************

* TinyCrypt does **not** intend to be fully side-channel resistant. Due to the
  variety of side-channel attacks, many of them only relevant to certain 
  platforms. In this sense, instead of penalizing all library users with
  side-channel countermeasures such as increasing the overall code size,
  TinyCrypt only implements certain generic timing-attack countermeasures.

Specific Remarks
****************

* SHA-256:

  * The number of bits_hashed in the state is not checked for overflow. Note
    however that this will only be a problem if you intend to hash more than
    2^64 bits, which is an extremely large window.

* HMAC:

  * The HMAC verification process is assumed to be performed by the application.
    This compares the computed tag with some given tag.
    Note that conventional memory-comparison methods (such as memcmp function)
    might be vulnerable to timing attacks; thus be sure to use a constant-time
    memory comparison function (such as compare_constant_time
    function provided in lib/utils.c).

  * The tc_hmac_final function, responsible for computing the message tag,
    cleans the state context before exiting. Thus, applications do not need to
    clean the TCHmacState_t ctx after calling tc_hmac_final. This should not
    be changed in future versions of the library as there are applications
    currently relying on this good-practice/feature of TinyCrypt.

* HMAC-PRNG:

  * Before using HMAC-PRNG, you *must* find an entropy source to produce a seed.
    PRNGs only stretch the seed into a seemingly random output of arbitrary
    length. The security of the output is exactly equal to the
    unpredictability of the seed.

  * NIST SP 800-90A requires three items as seed material in the initialization
    step: entropy seed, personalization and a nonce (which is not implemented).
    TinyCrypt requires the personalization byte array and automatically creates
    the entropy seed using a mandatory call to the re-seed function.

* AES-128:

  * The current implementation does not support other key-lengths (such as 256
    bits). Note that if you need AES-256, it doesn't sound as though your
    application is running in a constrained environment. AES-256 requires keys
    twice the size as for AES-128, and the key schedule is 40% larger.

* CTR mode:

  * The AES-CTR mode limits the size of a data message they encrypt to 2^32
    blocks. If you need to encrypt larger data sets, your application would
    need to replace the key after 2^32 block encryptions.

* CTR-PRNG:

  * Before using CTR-PRNG, you *must* find an entropy source to produce a seed.
    PRNGs only stretch the seed into a seemingly random output of arbitrary
    length. The security of the output is exactly equal to the
    unpredictability of the seed.

* CBC mode:

  * TinyCrypt CBC decryption assumes that the iv and the ciphertext are
    contiguous (as produced by TinyCrypt CBC encryption). This allows for a
    very efficient decryption algorithm that would not otherwise be possible.

* CMAC mode:

  * AES128-CMAC mode of operation offers 64 bits of security against collision
    attacks. Note however that an external attacker cannot generate the tags
    him/herself without knowing the MAC key. In this sense, to attack the
    collision property of AES128-CMAC, an external attacker would need the
    cooperation of the legal user to produce an exponentially high number of
    tags (e.g. 2^64) to finally be able to look for collisions and benefit
    from them. As an extra precaution, the current implementation allows to at
    most 2^48 calls to tc_cmac_update function before re-calling tc_cmac_setup
    (allowing a new key to be set), as suggested in Appendix B of SP 800-38B.

* CCM mode:

  * There are a few tradeoffs for the selection of the parameters of CCM mode.
    In special, there is a tradeoff between the maximum number of invocations
    of CCM under a given key and the maximum payload length for those
    invocations. Both things are related to the parameter 'q' of CCM mode. The
    maximum number of invocations of CCM under a given key is determined by
    the nonce size, which is: 15-q bytes. The maximum payload length for those
    invocations is defined as 2^(8q) bytes.

    To achieve minimal code size, TinyCrypt CCM implementation fixes q = 2,
    which is a quite reasonable choice for constrained applications. The
    implications of this choice are:

    The nonce size is: 13 bytes.

    The maximum payload length is: 2^16 bytes = 65 KB.

    The mac size parameter is an important parameter to estimate the security
    against collision attacks (that aim at finding different messages that
    produce the same authentication tag). TinyCrypt CCM implementation
    accepts any even integer between 4 and 16, as suggested in SP 800-38C.

  * TinyCrypt CCM implementation accepts associated data of any length between
    0 and (2^16 - 2^8) = 65280 bytes.

  * TinyCrypt CCM implementation accepts:

        * Both non-empty payload and associated data (it encrypts and
          authenticates the payload and only authenticates the associated data);

        * Non-empty payload and empty associated data (it encrypts and
          authenticates the payload);

        * Non-empty associated data and empty payload (it degenerates to an
          authentication-only mode on the associated data).

   * RFC-3610, which also specifies CCM, presents a few relevant security
     suggestions, such as: it is recommended for most applications to use a
     mac size greater than 8. Besides, it is emphasized that the usage of the
     same nonce for two different messages which are encrypted with the same
     key obviously destroys the security properties of CCM mode.

* ECC-DH and ECC-DSA:

  * TinyCrypt ECC implementation is based on micro-ecc (see
    https://github.com/kmackay/micro-ecc). In the original micro-ecc 
    documentation, there is an important remark about the way integers are
    represented:

    "Integer representation: To reduce code size, all large integers are
    represented using little-endian words - so the least significant word is
    first. You can use the 'ecc_bytes2native()' and 'ecc_native2bytes()'
    functions to convert between the native integer representation and the
    standardized octet representation."

    Note that the assumed bit layout is: {31, 30, ..., 0}, {63, 62, ..., 32},
    {95, 94, ..., 64}, {127, 126, ..., 96} for a very-long-integer (vli)
    consisting of 4 unsigned integers (as an example).

  * A cryptographically-secure PRNG function must be set (using uECC_set_rng())
    before calling uECC_make_key() or uECC_sign().

Examples of Applications
************************
It is possible to do useful cryptography with only the given small set of
primitives. With this list of primitives it becomes feasible to support a range
of cryptography usages:

 * Measurement of code, data structures, and other digital artifacts (SHA256);

 * Generate commitments (SHA256);

 * Construct keys (HMAC-SHA256);

 * Extract entropy from strings containing some randomness (HMAC-SHA256);

 * Construct random mappings (HMAC-SHA256);

 * Construct nonces and challenges (HMAC-PRNG, CTR-PRNG);

 * Authenticate using a shared secret (HMAC-SHA256);

 * Create an authenticated, replay-protected session (HMAC-SHA256 + HMAC-PRNG);

 * Authenticated encryption (AES-128 + AES-CCM);

 * Key-exchange (EC-DH);

 * Digital signature (EC-DSA);

Test Vectors
************

The library provides a test program for each cryptographic primitive (see 'test'
folder). Besides illustrating how to use the primitives, these tests evaluate
the correctness of the implementations by checking the results against
well-known publicly validated test vectors.

For the case of the HMAC-PRNG, due to the necessity of performing an extensive
battery test to produce meaningful conclusions, we suggest the user to evaluate
the unpredictability of the implementation by using the NIST Statistical Test
Suite (see References).

For the case of the EC-DH and EC-DSA implementations, most of the test vectors
were obtained from the site of the NIST Cryptographic Algorithm Validation
Program (CAVP), see References.

References
**********

* `NIST FIPS PUB 180-4 (SHA-256)`_

.. _NIST FIPS PUB 180-4 (SHA-256):
   http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf

* `NIST FIPS PUB 197 (AES-128)`_

.. _NIST FIPS PUB 197 (AES-128):
   http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf

* `NIST SP800-90A (HMAC-PRNG)`_

.. _NIST SP800-90A (HMAC-PRNG):
   http://csrc.nist.gov/publications/nistpubs/800-90A/SP800-90A.pdf

* `NIST SP 800-38A (AES-CBC and AES-CTR)`_

.. _NIST SP 800-38A (AES-CBC and AES-CTR):
   http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf

* `NIST SP 800-38B (AES-CMAC)`_

.. _NIST SP 800-38B (AES-CMAC):
   http://csrc.nist.gov/publications/nistpubs/800-38B/SP_800-38B.pdf

* `NIST SP 800-38C (AES-CCM)`_

.. _NIST SP 800-38C (AES-CCM):
    http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf

* `NIST Statistical Test Suite (useful for testing HMAC-PRNG)`_

.. _NIST Statistical Test Suite (useful for testing HMAC-PRNG):
   http://csrc.nist.gov/groups/ST/toolkit/rng/documentation_software.html

* `NIST Cryptographic Algorithm Validation Program (CAVP) site`_

.. _NIST Cryptographic Algorithm Validation Program (CAVP) site:
   http://csrc.nist.gov/groups/STM/cavp/

* `RFC 2104 (HMAC-SHA256)`_

.. _RFC 2104 (HMAC-SHA256):
   https://www.ietf.org/rfc/rfc2104.txt

* `RFC 6090 (ECC-DH and ECC-DSA)`_

.. _RFC 6090 (ECC-DH and ECC-DSA):
   https://www.ietf.org/rfc/rfc6090.txt
