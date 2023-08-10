#include <stdio.h>

#include "sha2.h"
#include "ripemd160.h"
#include "base32.h"
#include "rddl.h"
#include "rddl_cid.h"
#include "ecdsa.h"
#include "secp256k1.h"
#include "segwit_addr.h"
#include "tiny-json.h"


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
    const char *hrp = "cosmos";
    size_t data_len = 32;
    uint8_t paddingbuffer[32] = {0};
    uint8_t base32_enc[100] = {0};
    base32_encode_unsafe(address, 20, base32_enc);

    size_t len = strlen((const char*)base32_enc);
    return bech32_encode(stringbuffer, hrp, base32_enc, data_len);
}

void tx_to_tw_raw(Cosmos__Tx__V1beta1__Tx *tx, Cosmos__Tx__V1beta1__TxRaw *txRaw)
{
    txRaw->body_bytes.len = cosmos__tx__v1beta1__tx_body__get_packed_size(tx->body);
    txRaw->body_bytes.data = (uint8_t*)malloc(txRaw->body_bytes.len);
    cosmos__tx__v1beta1__tx_body__pack(tx->body, txRaw->body_bytes.data);

    txRaw->auth_info_bytes.len = cosmos__tx__v1beta1__auth_info__get_packed_size(tx->auth_info);
    txRaw->auth_info_bytes.data = (uint8_t*)malloc(txRaw->auth_info_bytes.len);
    cosmos__tx__v1beta1__auth_info__pack(tx->auth_info, txRaw->auth_info_bytes.data);
}

void prepareTx( Google__Protobuf__Any* anyMsg, Cosmos__Base__V1beta1__Coin* coin, 
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
    pubkey.key.data = (uint8_t*)malloc(33);
    memcpy(pubkey.key.data, pub_key, 33);
    Google__Protobuf__Any any_pub_key = GOOGLE__PROTOBUF__ANY__INIT;
    any_pub_key.type_url = "/cosmos.crypto.secp256k1.PubKey";
    any_pub_key.value.len = cosmos__crypto__secp256k1__pub_key__get_packed_size(&pubkey);
    any_pub_key.value.data = (uint8_t*)malloc(any_pub_key.value.len);
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
    tx_to_tw_raw(&tx, &txRaw);
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
    binMessage.data = (uint8_t*)malloc(binMessage.len);
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
    (*tx_bytes) = (uint8_t*)malloc( (*tx_size)  );
    cosmos__tx__v1beta1__tx_raw__pack(&txRaw, (*tx_bytes));
    
    free(binMessage.data);
    free(txRaw.body_bytes.data);
    free(txRaw.auth_info_bytes.data);
    free(pubkey.key.data);
    free(any_pub_key.value.data);
}

void generateAnyAttestMachineMsg(Google__Protobuf__Any* anyMsg, Planetmintgo__Machine__MsgAttestMachine* machineMsg)
{
    anyMsg->type_url = "/planetmintgo.machine.MsgAttestMachine";
    anyMsg->value.len = planetmintgo__machine__msg_attest_machine__get_packed_size(machineMsg);
    anyMsg->value.data = (uint8_t*)malloc(anyMsg->value.len);
    planetmintgo__machine__msg_attest_machine__pack(machineMsg, anyMsg->value.data);
}


void gnerateAnyCIDAttestMsg( Google__Protobuf__Any* anyMsg, char *public_address )
{
    Planetmintgo__Asset__MsgNotarizeAsset msg = PLANETMINTGO__ASSET__MSG_NOTARIZE_ASSET__INIT;
    msg.creator = public_address;
    msg.hash = "cid";
    msg.signature = "313d60b37ca2f168d33b7d6234f6d8725d910d0a74872350874bb0a98f8cc8584204010720b9d1dfe3800fdbf067d07bba13d2954d2e98943e18b8fe1fadf77b";
    msg.pubkey = "02328de87896b9cbb5101c335f40029e4be898988b470abbf683f1a0b318d73470";

    anyMsg->type_url = "/planetmintgo.asset.MsgNotarizeAsset";
    anyMsg->value.len = planetmintgo__asset__msg_notarize_asset__get_packed_size(&msg);
    anyMsg->value.data = (uint8_t*)malloc(anyMsg->value.len);
    planetmintgo__asset__msg_notarize_asset__pack(&msg, anyMsg->value.data);
}

void gnerateAnyCIDAttestMsgGeneric( Google__Protobuf__Any* anyMsg, const char* cid, uint8_t* priv_key, uint8_t* pub_key, char *public_address )
{

    Planetmintgo__Asset__MsgNotarizeAsset msg = PLANETMINTGO__ASSET__MSG_NOTARIZE_ASSET__INIT;

    char hex_pub_key[67] = {0};
    toHexString(hex_pub_key, pub_key, 33);

    uint8_t digest[SHA256_DIGEST_LENGTH] = {0};
    sha256(cid, strlen(cid), digest);

    const ecdsa_curve *curve = &secp256k1;
    //ed25519_sign((const unsigned char *)digest, SHA256_DIGEST_LENGTH, (const unsigned char *)priv_key, (const unsigned char *)pub_key + 1, signature);
    uint8_t signature[64]= {0};
    char signature_hex[64*2+1] = {0};
    int res = ecdsa_sign_digest(curve, (const unsigned char *)priv_key, (const unsigned char *)digest, signature, NULL, NULL);
    toHexString(signature_hex, signature, 64);

    msg.creator = public_address;
    msg.hash = (char*)cid;
    msg.signature =  signature_hex;
    msg.pubkey = hex_pub_key;

    anyMsg->type_url = "/planetmintgo.asset.MsgNotarizeAsset";
    anyMsg->value.len = planetmintgo__asset__msg_notarize_asset__get_packed_size(&msg);
    anyMsg->value.data = (uint8_t*)malloc(anyMsg->value.len);
    planetmintgo__asset__msg_notarize_asset__pack(&msg, anyMsg->value.data);

}

void attestMachine(uint8_t *priv_key, uint8_t *pub_key, char *public_address, uint8_t* signature, uint8_t** tx_bytes, size_t* tx_size)
{
    //
    // create body
    //
    Planetmintgo__Machine__Metadata metadata = PLANETMINTGO__MACHINE__METADATA__INIT;
    metadata.additionaldatacid = "CID";
    metadata.gps = "{\"Latitude\":\"-48.876667\",\"Longitude\":\"-123.393333\"}";
    metadata.assetdefinition = "{\"Version\": \"0.1\"}";
    metadata.device = "{\"Manufacturer\": \"RDDL\",\"Serial\":\"AdnT2uyt\"}";
    const char *address = "cosmos19cl05ztgt8ey6v86hjjjn3thfmpu6q2xqmsuyx";
    const char *pubKey = "AjKN6HiWucu1EBwzX0ACnkvomJiLRwq79oPxoLMY1zRw";

    
    Planetmintgo__Machine__Machine machine = PLANETMINTGO__MACHINE__MACHINE__INIT;
    machine.name = "machine";
    machine.ticker = "machine_ticker";
    machine.reissue = true;
    machine.amount = 1000;
    machine.precision = 8;
    machine.issuerplanetmint = "02328de87896b9cbb5101c335f40029e4be898988b470abbf683f1a0b318d73470";
    machine.issuerliquid = "xpub661MyMwAqRbcEigRSGNjzqsUbkoxRHTDYXDQ6o5kq6EQTSYuXxwD5zNbEXFjCG3hDmYZqCE4HFtcPAi3V3MW9tTYwqzLDUt9BmHv7fPcWaB";
    machine.machineid = "02328de87896b9cbb5101c335f40029e4be898988b470abbf683f1a0b318d73470";
    machine.metadata = &metadata;

    Planetmintgo__Machine__MsgAttestMachine machineMsg = PLANETMINTGO__MACHINE__MSG_ATTEST_MACHINE__INIT;
    machineMsg.creator = (char*)address;
    machineMsg.machine = &machine;

    Google__Protobuf__Any anyMsg = GOOGLE__PROTOBUF__ANY__INIT;
    anyMsg.type_url = "/planetmintgo.machine.MsgAttestMachine";
    anyMsg.value.len = planetmintgo__machine__msg_attest_machine__get_packed_size(&machineMsg);
    anyMsg.value.data = (uint8_t*)malloc(anyMsg.value.len);
    planetmintgo__machine__msg_attest_machine__pack(&machineMsg, anyMsg.value.data);

    Cosmos__Tx__V1beta1__TxBody body;
    cosmos__tx__v1beta1__tx_body__init(&body);
    Google__Protobuf__Any* messages[1] = {(Google__Protobuf__Any*)&anyMsg};
    body.n_messages = 1;
    body.messages = messages;
    body.timeout_height = 0;
    // body.memo = NULL;
    // body.n_extension_options=0;
    // body.extension_options=0;
    // body.n_non_critical_extension_options=0;
    // body.non_critical_extension_options=NULL;
    


    Cosmos__Tx__V1beta1__ModeInfo__Single mode_single= COSMOS__TX__V1BETA1__MODE_INFO__SINGLE__INIT;
    mode_single.mode = COSMOS__TX__SIGNING__V1BETA1__SIGN_MODE__SIGN_MODE_DIRECT;
    
    Cosmos__Tx__V1beta1__ModeInfo mode_info = COSMOS__TX__V1BETA1__MODE_INFO__INIT;
    mode_info.sum_case = COSMOS__TX__V1BETA1__MODE_INFO__SUM_SINGLE;
    mode_info.single = &mode_single;



    Cosmos__Crypto__Secp256k1__PubKey pubkey = COSMOS__CRYPTO__SECP256K1__PUB_KEY__INIT;
    pubkey.key.len = 33;
    pubkey.key.data = (uint8_t*)malloc(33);
    memcpy(pubkey.key.data, pub_key, 33);
    Google__Protobuf__Any any_pub_key = GOOGLE__PROTOBUF__ANY__INIT;
    any_pub_key.type_url = "/cosmos.crypto.secp256k1.PubKey";
    any_pub_key.value.len = cosmos__crypto__secp256k1__pub_key__get_packed_size(&pubkey);
    any_pub_key.value.data = (uint8_t*)malloc(any_pub_key.value.len);
    cosmos__crypto__secp256k1__pub_key__pack(&pubkey, any_pub_key.value.data);

    Cosmos__Tx__V1beta1__SignerInfo signInfo = COSMOS__TX__V1BETA1__SIGNER_INFO__INIT;
    cosmos__tx__v1beta1__signer_info__init(&signInfo);
    signInfo.mode_info = &mode_info;
    signInfo.sequence = 1;
    signInfo.public_key = &any_pub_key;
    


    Cosmos__Base__V1beta1__Coin coin = COSMOS__BASE__V1BETA1__COIN__INIT;
    Cosmos__Base__V1beta1__Coin* coins[1] = {&coin};
    coin.denom = "stake";
    coin.amount = "2";

    Cosmos__Tx__V1beta1__Fee fee = COSMOS__TX__V1BETA1__FEE__INIT;
    cosmos__tx__v1beta1__fee__init(&fee);
    fee.amount = coins;
    fee.gas_limit = 200000;
    fee.n_amount = 1;

    Cosmos__Base__V1beta1__Coin tip_coin = COSMOS__BASE__V1BETA1__COIN__INIT;
    Cosmos__Base__V1beta1__Coin* tip_coins[1] = {&tip_coin};
    tip_coin.denom="stake";
    tip_coin.amount="6";
    Cosmos__Tx__V1beta1__Tip tip = COSMOS__TX__V1BETA1__TIP__INIT;
    tip.n_amount=1;
    tip.amount= tip_coins;
    tip.tipper=(char*)address;

    Cosmos__Tx__V1beta1__SignerInfo* signer_infos[1] = {&signInfo};
    Cosmos__Tx__V1beta1__AuthInfo auth;
    cosmos__tx__v1beta1__auth_info__init(&auth);
    auth.n_signer_infos = 1;
    auth.signer_infos = signer_infos;
    auth.fee = &fee;
    //&auth.tip = &tip;

    size_t singer_info_len = cosmos__tx__v1beta1__signer_info__get_packed_size( &signInfo );
    size_t fee_len = cosmos__tx__v1beta1__fee__get_packed_size( &fee );
    //size_t tip_len = cosmos__tx__v1beta1__tip__get_packed_size( &tip );




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
    tx_to_tw_raw(&tx, &txRaw);
    txRaw.n_signatures = tx.n_signatures;


    // create Cosmos__Tx__V1beta1__SignDoc
    // to create a signature
    Cosmos__Tx__V1beta1__SignDoc signDoc;
    cosmos__tx__v1beta1__sign_doc__init(&signDoc);
    signDoc.body_bytes = txRaw.body_bytes;
    signDoc.auth_info_bytes = txRaw.auth_info_bytes;
    signDoc.chain_id = "chain-foobarbaz";
    signDoc.account_number = 7;

    ProtobufCBinaryData binMessage;
    binMessage.len = cosmos__tx__v1beta1__sign_doc__get_packed_size(&signDoc);
    binMessage.data = (uint8_t*)malloc(binMessage.len);
    cosmos__tx__v1beta1__sign_doc__pack(&signDoc, binMessage.data);

    uint8_t digest[SHA256_DIGEST_LENGTH] = {0};
    sha256_Raw(binMessage.data, binMessage.len, digest);

    //unsigned char signature[64] = {0};
    const ecdsa_curve *curve = &secp256k1;
    //ed25519_sign((const unsigned char *)digest, SHA256_DIGEST_LENGTH, (const unsigned char *)priv_key, (const unsigned char *)pub_key + 1, signature);
    
    int res = ecdsa_sign_digest(curve, (const unsigned char *)priv_key, (const unsigned char *)digest, signature, NULL, NULL);

    ProtobufCBinaryData sig;
    sig.len=64;
    sig.data= signature; 
    ProtobufCBinaryData sigs[1]={sig};
    txRaw.n_signatures=1;
    txRaw.signatures=sigs;

    (*tx_size) = cosmos__tx__v1beta1__tx_raw__get_packed_size( &txRaw );
    (*tx_bytes) = (uint8_t*)malloc( (*tx_size)  );
    //uint8_t* *tx_envelope = malloc( len );
    cosmos__tx__v1beta1__tx_raw__pack(&txRaw, (*tx_bytes));
    
    free(binMessage.data);
    free(txRaw.body_bytes.data);
    free(txRaw.auth_info_bytes.data);
    free(anyMsg.value.data);
    free(pubkey.key.data);
    free(any_pub_key.value.data);
    //return tx_envelope;
}

bool get_account_info( const char* json_obj, int* account_id, int* sequence)
{
    unsigned int max_fields = 20;
    json_t pool[max_fields];
    
    const json_t* response = json_create( json_obj, pool, max_fields);
    
    if ( response == NULL ) return false;
    json_t const* info_field = json_getProperty( response, "info" );
    if ( info_field == NULL ) return false;

    char const* account_value = json_getPropertyValue( info_field, "account_number" );
    char const* sequence_value = json_getPropertyValue( info_field, "sequence" );
    if( !account_value || !sequence_value )
        return false;

    *account_id = atoi( account_value );
    *sequence = atoi( sequence_value );
    return true;
}

bool get_address_info_from_accounts( const char* json_obj, const char* address, int* account_id, int* sequence)
{
    unsigned int max_fields = 1000;
    json_t pool[max_fields];
    const json_t* response = json_create( json_obj, pool, max_fields);
    
    if ( response == NULL ) return false;
    json_t const* accounts = json_getProperty( response, "accounts" );
    if ( accounts == NULL ) return false;

    json_t const* sib = json_getChild( accounts );
    while( true )
    {
        char const* address_value = json_getPropertyValue( sib, "address" );
        if(address_value && strcmp( address_value, address) == 0)
        {
            char const* account_value = json_getPropertyValue( sib, "account_number" );
            char const* sequence_value = json_getPropertyValue( sib, "sequence" );
            *account_id = atoi( account_value );
            *sequence = atoi( sequence_value );
            return true;
        }
        sib = json_getSibling( sib );
        if( !sib )
            break;

    }
    return false;   
}
