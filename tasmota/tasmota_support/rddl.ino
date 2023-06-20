#include <stdio.h>
#include <sys/random.h>

#include "trezor-crypto/options.h"
#include "trezor-crypto/address.h"
#include "trezor-crypto/aes/aes.h"
#include "trezor-crypto/base32.h"
#include "trezor-crypto/base58.h"
#include "trezor-crypto/bignum.h"
#include "trezor-crypto/bip32.h"
#include "trezor-crypto/bip39.h"
#include "trezor-crypto/blake256.h"
#include "trezor-crypto/blake2b.h"
#include "trezor-crypto/blake2s.h"
#include "trezor-crypto/curves.h"
#include "trezor-crypto/ecdsa.h"
#include "trezor-crypto/ed25519-donna/ed25519-donna.h"
#include "trezor-crypto/ed25519-donna/curve25519-donna-scalarmult-base.h"
#include "trezor-crypto/ed25519-donna/ed25519-keccak.h"
#include "trezor-crypto/ed25519-donna/ed25519.h"
#include "trezor-crypto/hmac.h"
#include "trezor-crypto/memzero.h"
#include "trezor-crypto/nist256p1.h"
#include "trezor-crypto/pbkdf2.h"
#include "trezor-crypto/rand.h"
#include "trezor-crypto/rc4.h"
#include "trezor-crypto/rfc6979.h"
#include "trezor-crypto/script.h"
#include "trezor-crypto/secp256k1.h"
#include "trezor-crypto/sha2.h"
#include "trezor-crypto/sha3.h"

#include "tasmota_support/rddl.h"

char* g_mnemonic = NULL;

static bool bIsDynamicallyAllocated = false;

const uint8_t *fromhex2(const char *str) {
  static uint8_t buf[FROMHEX_MAXLEN];
  size_t len = strlen(str) / 2;
  if (len > FROMHEX_MAXLEN) len = FROMHEX_MAXLEN;
  for (size_t i = 0; i < len; i++) {
    uint8_t c = 0;
    if (str[i * 2] >= '0' && str[i * 2] <= '9') c += (str[i * 2] - '0') << 4;
    if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
      c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
    if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
      c += (str[i * 2 + 1] - '0');
    if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
      c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
    buf[i] = c;
  }
  return buf;
}


void tohex2(char *hexbuf, uint8_t *str, int strlen){
   // char hexbuf[strlen];
    for (int i = 0 ; i < strlen/2 ; i++) {
        sprintf(&hexbuf[2*i], "%02X", str[i]);
    }
  hexbuf[strlen-2] = '\0';
}

const char* getMnemonic()
{
  // Generate a random master seed
  uint8_t master_seed[32];
  for( int i = 0; i< 32; ++i )
    master_seed[i]= random();

  // Generate a 12-word mnemonic phrase from the master seed
  const char * mnemonic_phrase = mnemonic_from_data(master_seed, 32);

  printf("%s\n", mnemonic_phrase);
  return mnemonic_phrase;
}

const char* setMnemonic( char* pMnemonic, size_t len )
{
  uint8_t seed[64] = {0};

  if( mnemonic_check( pMnemonic ) )
  {
    mnemonic_to_seed(pMnemonic, "TREZOR", seed, 0);
    if( g_mnemonic && bIsDynamicallyAllocated )
    {
      delete g_mnemonic;
    }
    g_mnemonic= new char[len+1];
    memset( g_mnemonic,0, len+1 );
    memcpy_P(g_mnemonic,pMnemonic, len);
    bIsDynamicallyAllocated = true;

    return (const char*)g_mnemonic;
  }
  else
    return "";
}

int validateSignature() {
  const ecdsa_curve *curve = &secp256k1;


  uint8_t pub_key[33] = {0};
  uint8_t hash[32] = {0};
  uint8_t computed_sig[64] = {0};

  const char pub_key_str[] = "02F8BC8B413BF803EA1DA9BE0FBFF4ED23FEED17A859187242007544F8535D3457";
  const char hash_str[] = "83EC230810630863EEB5C873206F45E60D5FB9EA3F5241EEECFB514F261A57DF";
  const char sig_str[] = "F551CDF6156FD2A8CC29428B61FDB9F5224928D5A5937E38F36D2D566C11B1DF13CD12E3BA2DAE6A33F091C549A5ADE537A5F07121AA1F4D4286B51260B228DE";


  memcpy(pub_key, fromhex2(pub_key_str), 33);
  memcpy(hash, fromhex2(hash_str), 32);
  memcpy(computed_sig, fromhex2(sig_str), 64);

  int verified = ecdsa_verify_digest(curve, pub_key, computed_sig, hash);
  return verified;
}

bool SignDataHash(int json_data_start, int current_length, const char* data_str, char* pubkey_out, char* sig_out, char* hash_out)
{
    uint8_t seed[64];

    mnemonic_to_seed(g_mnemonic, "TREZOR", seed, 0);
    HDNode node2;

  // test vector from https://en.bitcoin.it/wiki/BIP_0032_TestVectors for Elliptic Curves SECP256K1
    hdnode_from_seed( seed, 64, SECP256K1_NAME, &node2);
    hdnode_fill_public_key(&node2);

//    char* str = "\"ENERGY\":{\"TotalStartTime\":\"2022-10-13T10:12:20\",\"Total\":100.293,\"Yesterday\":0.508,\"Today\":0.222,\"Power\":21,\"ApparentPower\":39,\"ReactivePower\":32,\"Factor\":0.54,\"Voltage\":229,\"Current\":0.168}";
    uint8_t hash[32];

    size_t p2bsigned_length = current_length - json_data_start;
    const char* p2Bsigned = data_str + json_data_start;;
    // char* response = ResponseData();

    // Initialize the SHA-256 hasher
    SHA256_CTX ctx;
    sha256_Init(&ctx);

    // Hash the string
    sha256_Update(&ctx, (const uint8_t*) p2Bsigned, p2bsigned_length);
    sha256_Final(&ctx, hash);

    // Initialize the ECDSA context
    const ecdsa_curve *curve = &secp256k1;

    uint8_t priv_key[32] = {0};
    uint8_t pub_key[33] = {0};
    uint8_t digest[32] = {0};
    uint8_t expected_sig[64] = {0};
    uint8_t computed_sig[64] = {0};
    int res = 0;

    memcpy(priv_key, node2.private_key, 32);

    res = ecdsa_sign_digest(curve, priv_key, hash, computed_sig, NULL, NULL);

    // Print the signature and verification result
    memcpy(pub_key, node2.public_key, 33);

    int verified = ecdsa_verify_digest(curve, pub_key, computed_sig, hash);

    // prepare and convert outputs to hex-strings
    tohex2( pubkey_out, pub_key, 68);
    tohex2( sig_out, computed_sig, 130);
    tohex2( hash_out, hash, 66);

    return verified;
}
