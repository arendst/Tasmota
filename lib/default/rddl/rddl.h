#pragma once

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

const uint8_t *fromhex2(const char *str);
void tohex2(char *hexbuf, uint8_t *str, int strlen);

const char* getMnemonic();
const char* setMnemonic( char* pMnemonic, size_t len );

int validateSignature();

bool SignDataHash(int json_data_start, int current_length, const char* data_str, char* pubkey_out, char* sig_out, char* hash_out);

extern char* g_mnemonic;