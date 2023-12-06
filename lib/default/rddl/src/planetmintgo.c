#include <stdio.h>

#include "sha2.h"
#include "ripemd160.h"
#include "base32.h"
#include "rddl.h"
#include "rddl_cid.h"
#include "ecdsa.h"
#include "secp256k1.h"
#include "segwit_addr.h"


#include "cosmos/crypto/secp256k1/keys.pb-c.h"
#include "planetmintgo/machine/tx.pb-c.h"
#include "planetmintgo/asset/tx.pb-c.h"

#include "planetmintgo.h"

#define COMPACT_SIG_MAGIC_OFFSET (27)

void pubkey2address(const uint8_t *pubkey, size_t key_length, uint8_t *address)
{
    SHA256_CTX ctx;
    uint8_t ripemd160_hash[RIPEMD160_DIGEST_LENGTH] = {0};

    uint8_t hash[SHA256_DIGEST_LENGTH] = {0};
    sha256_Init(&ctx);
    // Hash the string
    sha256_Update(&ctx, pubkey, key_length);
    sha256_Final(&ctx, hash);

    ripemd160((const uint8_t *)hash, SHA256_DIGEST_LENGTH, ripemd160_hash);
    memcpy(address, ripemd160_hash, 20);
}

int getAddressString(const uint8_t *address, char *stringbuffer)
{
    const char *hrp = "plmnt";
    size_t data_len = 32;
    uint8_t paddingbuffer[32] = {0};
    uint8_t base32_enc[100] = {0};
    base32_encode_unsafe(address, 20, base32_enc);

    size_t len = strlen((const char*)base32_enc);
    return bech32_encode(stringbuffer, hrp, base32_enc, data_len);
}

size_t myStackSize = 0;
volatile uint8_t myStack[MY_STACK_LIMIT] = {0};

uint8_t* getStack( size_t size )
{
    if (myStackSize + size > MY_STACK_LIMIT)
        return NULL;

    uint8_t* current_ptr = &myStack[myStackSize];
    myStackSize += size;
    return current_ptr;
}

void clearStack()
{
    myStackSize = 0;
}


int tx_to_tw_raw(Cosmos__Tx__V1beta1__Tx *tx, Cosmos__Tx__V1beta1__TxRaw *txRaw)
{
    txRaw->body_bytes.len = cosmos__tx__v1beta1__tx_body__get_packed_size(tx->body);
    txRaw->body_bytes.data = (uint8_t*)getStack(txRaw->body_bytes.len);
    if( !txRaw->body_bytes.data)
        return -1;
    cosmos__tx__v1beta1__tx_body__pack(tx->body, txRaw->body_bytes.data);

    txRaw->auth_info_bytes.len = cosmos__tx__v1beta1__auth_info__get_packed_size(tx->auth_info);
    txRaw->auth_info_bytes.data = (uint8_t*)getStack(txRaw->auth_info_bytes.len);
    if( !txRaw->auth_info_bytes.data )
        return -1;
    cosmos__tx__v1beta1__auth_info__pack(tx->auth_info, txRaw->auth_info_bytes.data);
    return 0;
}

int prepareTx( Google__Protobuf__Any* anyMsg, Cosmos__Base__V1beta1__Coin* coin, 
        uint8_t *priv_key, uint8_t *pub_key,
        uint64_t sequence, const char *chain_id, uint64_t account_id,
        uint8_t** tx_bytes, size_t* tx_size)
{
    Cosmos__Tx__V1beta1__TxBody body;
    cosmos__tx__v1beta1__tx_body__init(&body);
    Google__Protobuf__Any* messages[1] = {(Google__Protobuf__Any*)anyMsg};
    body.n_messages = 1;
    body.messages = (Google__Protobuf__Any**)messages;
    body.timeout_height = 0;

    Cosmos__Tx__V1beta1__ModeInfo__Single mode_single= COSMOS__TX__V1BETA1__MODE_INFO__SINGLE__INIT;
    mode_single.mode = COSMOS__TX__SIGNING__V1BETA1__SIGN_MODE__SIGN_MODE_DIRECT;
    
    Cosmos__Tx__V1beta1__ModeInfo mode_info = COSMOS__TX__V1BETA1__MODE_INFO__INIT;
    mode_info.sum_case = COSMOS__TX__V1BETA1__MODE_INFO__SUM_SINGLE;
    mode_info.single = &mode_single;



    Cosmos__Crypto__Secp256k1__PubKey pubkey = COSMOS__CRYPTO__SECP256K1__PUB_KEY__INIT;
    pubkey.key.len = 33;
    pubkey.key.data = pub_key;

    Google__Protobuf__Any any_pub_key = GOOGLE__PROTOBUF__ANY__INIT;
    any_pub_key.type_url = "/cosmos.crypto.secp256k1.PubKey";
    any_pub_key.value.len = cosmos__crypto__secp256k1__pub_key__get_packed_size(&pubkey);
    any_pub_key.value.data = (uint8_t*)getStack(any_pub_key.value.len);
    if( !any_pub_key.value.data )
        return -1;
    cosmos__crypto__secp256k1__pub_key__pack(&pubkey, any_pub_key.value.data);

    Cosmos__Tx__V1beta1__SignerInfo signInfo = COSMOS__TX__V1BETA1__SIGNER_INFO__INIT;
    cosmos__tx__v1beta1__signer_info__init(&signInfo);
    signInfo.mode_info = &mode_info;
    signInfo.sequence = sequence;
    signInfo.public_key = &any_pub_key;
    


    Cosmos__Base__V1beta1__Coin* coins[1] = {(Cosmos__Base__V1beta1__Coin*)coin};

    Cosmos__Tx__V1beta1__Fee fee = COSMOS__TX__V1BETA1__FEE__INIT;
    cosmos__tx__v1beta1__fee__init(&fee);
    fee.amount = (Cosmos__Base__V1beta1__Coin**)coins;
    fee.gas_limit = 200000;
    fee.n_amount = 1;


    Cosmos__Tx__V1beta1__SignerInfo* signer_infos[1] = {(Cosmos__Tx__V1beta1__SignerInfo*)&signInfo};
    Cosmos__Tx__V1beta1__AuthInfo auth;
    cosmos__tx__v1beta1__auth_info__init(&auth);
    auth.n_signer_infos = 1;
    auth.signer_infos = (Cosmos__Tx__V1beta1__SignerInfo**)signer_infos;
    auth.fee = &fee;

    //
    //transaction composition
    //
    Cosmos__Tx__V1beta1__Tx tx;
    cosmos__tx__v1beta1__tx__init(&tx);
    tx.body = &body;
    tx.auth_info = &auth;
    // tx.n_signatures=0;
    // tx.signatures = NULL;

    Cosmos__Tx__V1beta1__TxRaw txRaw;
    cosmos__tx__v1beta1__tx_raw__init(&txRaw);
    if( tx_to_tw_raw(&tx, &txRaw) < 0 )
        return -1;
    txRaw.n_signatures = tx.n_signatures;


    // create Cosmos__Tx__V1beta1__SignDoc
    // to create a signature
    Cosmos__Tx__V1beta1__SignDoc signDoc;
    cosmos__tx__v1beta1__sign_doc__init(&signDoc);
    signDoc.body_bytes = txRaw.body_bytes;
    signDoc.auth_info_bytes = txRaw.auth_info_bytes;
    signDoc.chain_id = (char*)chain_id;
    signDoc.account_number = account_id;

    ProtobufCBinaryData binMessage;
    binMessage.len = cosmos__tx__v1beta1__sign_doc__get_packed_size(&signDoc);
    binMessage.data = (uint8_t*)getStack(binMessage.len);
    if( !binMessage.data )
        return -1;
    cosmos__tx__v1beta1__sign_doc__pack(&signDoc, binMessage.data);

    uint8_t digest[SHA256_DIGEST_LENGTH] = {0};
    sha256_Raw(binMessage.data, binMessage.len, digest);

    unsigned char signature[64] = {0};
    const ecdsa_curve *curve = &secp256k1;
    int res = ecdsa_sign_digest(curve, (const unsigned char *)priv_key, (const unsigned char *)digest, signature, NULL, NULL);

    ProtobufCBinaryData sig;
    sig.len=64;
    sig.data= signature; 
    ProtobufCBinaryData sigs[1]={sig};
    txRaw.n_signatures=1;
    txRaw.signatures=sigs;

    (*tx_size) = cosmos__tx__v1beta1__tx_raw__get_packed_size( &txRaw );
    (*tx_bytes) = (uint8_t*)getStack(*tx_size);
    if( !(*tx_bytes) )
        return -1;
    cosmos__tx__v1beta1__tx_raw__pack(&txRaw, (*tx_bytes));
    return 0;
}

int generateAnyAttestMachineMsg(Google__Protobuf__Any* anyMsg, Planetmintgo__Machine__MsgAttestMachine* machineMsg)
{
    anyMsg->type_url = "/planetmintgo.machine.MsgAttestMachine";
    anyMsg->value.len = planetmintgo__machine__msg_attest_machine__get_packed_size(machineMsg);
    anyMsg->value.data = getStack(anyMsg->value.len);
    if( !anyMsg->value.data )
        return -1;
    planetmintgo__machine__msg_attest_machine__pack(machineMsg, anyMsg->value.data);
    return 0;
}

int generateAnyCIDAttestMsg( Google__Protobuf__Any* anyMsg, const char* cid, uint8_t* priv_key, uint8_t* pub_key, char* public_address, const char* ext_pub_key)
{

    Planetmintgo__Asset__MsgNotarizeAsset msg = PLANETMINTGO__ASSET__MSG_NOTARIZE_ASSET__INIT;

    msg.creator = public_address;
    msg.cid = (char*)cid;


    anyMsg->type_url = "/planetmintgo.asset.MsgNotarizeAsset";
    anyMsg->value.len = planetmintgo__asset__msg_notarize_asset__get_packed_size(&msg);
    anyMsg->value.data = (uint8_t*)getStack(anyMsg->value.len);
    if( !anyMsg->value.data )
        return -1;
    planetmintgo__asset__msg_notarize_asset__pack(&msg, anyMsg->value.data);
    return 0;
}

bool get_account_info( const char* json_obj, int* account_id, int* sequence)
{

    char* end_str = "\"";
    char* search_string = "\"account_number\":\"";

    char account_memory[10]= {0};
    char sequence_memory[10]= {0};

    char* ptr = strstr( json_obj, search_string);
    if( !ptr )
        return false;
    size_t len = strlen(search_string);
    char* endptr = strstr( ptr+ len, end_str );
    memcpy( (void*)account_memory, ptr+ len,endptr-(ptr+len) );


    search_string = "\"sequence\":\"";
    ptr = strstr( json_obj, search_string);
    if( !ptr )
        return false;
    len = strlen(search_string);
    endptr = strstr( ptr+ len, end_str );
    memcpy( (void*)sequence_memory, ptr+ len,endptr-(ptr+len) );

    *account_id = atoi( account_memory );
    *sequence = atoi( sequence_memory );
    return true;
}

bool removeIPAddr( char* gps_data )
{
    char* search_string = ",\"User-IP\":";
    char* substitution_str = "}";
    char* ptr = strstr( gps_data, search_string);
    if( !ptr )
        return false;
    ptr[0]= substitution_str[0];
    ptr[1]=0;
    return true;
}
