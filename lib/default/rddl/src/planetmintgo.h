#ifndef _PLANETMINT_GO_H_
#define _PLANETMINT_GO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PUBKEY_SIZE 35
#define ADDRESS_TAIL 20
#include "planetmintgo/machine/machine.pb-c.h"
#include "planetmintgo/machine/tx.pb-c.h"
#include "cosmos/tx/v1beta1/tx.pb-c.h"
#include "google/protobuf/any.pb-c.h"
#include "cosmos/base/v1beta1/coin.pb-c.h"


#define PLANETMINT_PMPB 0x03E14247
#define PLANETMINT_PMPR 0x03E142B0

bool get_account_info( const char* json_obj, int* account_id, int* sequence );
bool get_address_info_from_accounts( const char* json_obj, const char* address, int* account_id, int* sequence );

void pubkey2address( const uint8_t* pubkey, size_t key_length, uint8_t* address);
int getAddressString( const uint8_t* address, char* stringbuffer);

void gnerateAnyCIDAttestMsg( Google__Protobuf__Any* anyMsg, char *public_address );
void gnerateAnyCIDAttestMsgGeneric( Google__Protobuf__Any* anyMsg, const char* cid, 
        uint8_t* priv_key, uint8_t* pub_key,
        char *public_address );

void generateAnyAttestMachineMsg(Google__Protobuf__Any* anyMsg, Planetmintgo__Machine__MsgAttestMachine* machineMsg);

void prepareTx( Google__Protobuf__Any* anyMsg, 
        Cosmos__Base__V1beta1__Coin* coin,
        uint8_t *priv_key, uint8_t *pub_key,
        uint64_t sequence, const char *chain_id, uint64_t account_id,
        uint8_t** tx_bytes, size_t* tx_size);
bool removeIPAddr( char* gps_data );

#ifdef __cplusplus
}
#endif

#endif
