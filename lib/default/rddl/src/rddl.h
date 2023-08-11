#ifndef RDDL_H
#define RDDL_H

#ifdef __cplusplus
extern "C" {
#endif

#define FROMHEX_MAXLEN 512

#define VERSION_PUBLIC 0x0488b21e
#define VERSION_PRIVATE 0x0488ade4

#define BDB_VERSION_PUBLIC 0x02d41400   //0x03A3FDC2
#define BDB_VERSION_PRIVATE 0x02d40fc0   //0x03A3F988

#define PLANET_VERSION_PUBLIC 0x03e25d83
#define PLANET_VERSION_PRIVATE 0x03e25944 

#define LIQUIDBTC_VERSION_PUBLIC 0X76067358
#define LIQUIDBTC_VERSION_PRIVATE 0x76066276

#define ETHEREUM_VERSION_PUBLIC 0x0488b21e
#define ETHEREUM_VERSION_PRIVATE 0x0488ade4



#define SEED_SIZE 64
#define SEED_SIZE_MNEMONIC_TO_SEED 16

extern uint8_t secret_seed[SEED_SIZE];

const uint8_t *fromHexString(const char *str);
void toHexString(char *hexbuf, uint8_t *str, int strlen);

const char* getMnemonic();
const char* setSeed( char* pMnemonic, size_t len );

const char* getMnemonicFromSeed( const uint8_t* seed, size_t length );
bool getSeedFromMnemonic( const char* pMnemonic, size_t len, uint8_t* seedbuffer );

int validateSignature();

bool getKeyFromSeed( const uint8_t* seed, uint8_t* priv_key, uint8_t* pub_key, const char* curve_name);

bool SignDataHash(const char* data_str, size_t data_length, char* pubkey_out, char* sig_out, char* hash_out);

#ifdef __cplusplus
}
#endif

#endif
