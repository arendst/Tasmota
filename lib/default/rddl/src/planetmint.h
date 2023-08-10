#ifndef _PLANETMINT_TX_H_
#define _PLANETMINT_TX_H_
#define PLANETMINT_VERSION "3.0"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "base58.h"
#include "hmac.h"
#include "json-maker.h"
#include "tiny-json.h"
#include "sha3.h"
#include "bip32.h"
#include "curves.h"

#ifdef TASMOTA
#include "base64_plntmnt.h"
#include "ed25519.h"
#else
#include "base64.h"
#include "ed25519-donna/ed25519.h"
#endif




#define ASSET_MAX_SIZE 1024
#define METADATA_MAX_SIZE 1024

typedef struct CC {
  char type[64];
  union {
    // public key types
    // struct { char public_key[45], *signature; };
    struct {
      char public_key[45];
    };
    // threshold
    struct {
      long threshold;
      uint8_t size;
      struct CC **subconditions;
    };
  };
} CC;

/** Structure representing Planetmint output's condition (only fit Ed25519). */
typedef struct {
  CC details;
  char uri[256];
} PLANETMINT_CONDITION;

/** Structure representing Planetmint a transaction's output properties. */
typedef struct {
  char amount[8];
  PLANETMINT_CONDITION condition;
  // FIXED CONDITION FOR EDCURVE
  char public_keys[8][45];
  uint8_t num_public_keys;
} PLANETMINT_OUTPUT;


/** Structure representing Planetmint a fulfilled transaction's input properties. */
typedef struct {
  char transaction_id[65];
  uint8_t output_index;
} PLANETMINT_INPUT_FULFILLS;

/** Structure representing Planetmint a transaction's input properties. */
typedef struct {
  char fulfillment[256];
  PLANETMINT_INPUT_FULFILLS fulfills;
  char owners_before[8][256];
  uint8_t num_owners;
} PLANETMINT_INPUT;

/** Structure representing Planetmint transaction properties. */
typedef struct {
  char asset[ASSET_MAX_SIZE];
  char metadata[METADATA_MAX_SIZE];
  char operation[32];
  PLANETMINT_OUTPUT outputs[8];
  uint8_t num_outputs;
  PLANETMINT_INPUT inputs[8];
  uint8_t num_inputs;
  char version[8];
  char id[65];
} PLANETMINT_TX;

/**
  * void planetmint_fulfill
  * Create the DER encoded fulfillement for a given transaction's input.
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param priv_key Public key useable to verify signature as uint8_t array .
  * @param sig Transaction signature as uint8_t array
  * @param input_index Transaction's input index to fulfill
  */
void planetmint_fulfill(PLANETMINT_TX *tx, uint8_t *sig, uint8_t *pub_key, uint8_t input_index);

/**
  * void planetmint_serialize
  * Generate transaction's id by hashing the serialized transaction, populate tx.id and update JSON string
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param json_tx Transaction as JSON string.
  * @param maxlen Transaction's maximum length
  */
void planetmint_serialize(PLANETMINT_TX *tx, uint8_t *json_tx, uint16_t maxlen);

/**
  * void planetmint_fulfill_and_serialize
  * Execute planetmint_fulfill and planetmint_serialize for a given transaction input.
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param json_tx Transaction as JSON string.
  * @param maxlen Transaction's maximum length
  * @param priv_key Public key useable to verify signature as uint8_t array .
  * @param sig Transaction signature as uint8_t array
  */
void planetmint_fulfill_and_serialize(PLANETMINT_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pub_key);

/**
  * void planetmint_sign_transaction
  * Takes a json string, hashes it sha3_256 and signs it with ed25519.
  * @param json_tx Transaction as JSON string.
  * @param len Transaction's length
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param sig Signature output as uint8_t array
  */
void planetmint_sign_transaction(uint8_t *json_tx, uint16_t len, uint8_t *priv_key, uint8_t *pub_key, uint8_t *sig);

/** char *planetmint_build_json_inputs
  * Build JSON transaction's inputs from PLANETMINT_INPUT struct array
  * @param inputs A pointer to PLANETMINT_INPUT array.
  * @param num_inputs Count of inputs.
  * @param json_obj Transaction output as JSON string
  */
char *planetmint_build_json_inputs(PLANETMINT_INPUT *inputs, uint8_t num_inputs, char *json_obj);

/** char *planetmint_build_json_outputs
  * Build JSON transaction's outputs from PLANETMINT_OUTPUT struct array
  * @param outputs A pointer to PLANETMINT_OUTPUT array.
  * @param num_outputs Count of outputs.
  * @param json_obj Transaction output as JSON string
  */
char *planetmint_build_json_outputs(PLANETMINT_OUTPUT *outputs, uint8_t num_outputs, char *json_obj);

/** void planetmint_build_json_tx
  * Build JSON transaction from PLANETMINT_TX struct
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param json_tx Transaction output as JSON string
  */
void planetmint_build_json_tx(PLANETMINT_TX *tx, char *json_tx);

/** int planetmint_parse_inputs
  * Handles JSON transaction inputs prepared by bigchain driver
  * @param json_obj A valid handler of a json object.
  * @param inputs A pointer to PLANETMINT_INPUT array.
  * @return Inputs count.
  */
int planetmint_parse_inputs(const json_t *json_obj, PLANETMINT_INPUT *inputs);

/** int planetmint_parse_outputs
  * Handles JSON transaction outputs prepared by bigchain driver
  * @param json_obj A valid handler of a json object.
  * @param outputs A pointer to PLANETMINT_OUTPUT array.
  * @return Outputs count.
  */
int planetmint_parse_outputs(const json_t *json_obj, PLANETMINT_OUTPUT *outputs);

/** int planetmint_parse_field
  * Handles JSON transaction asset or metadata prepared by user
  * @param json_obj A valid handler of a json object.
  * @param field_name Should be asset or metadata
  * @param output A pointer to char array.
  * @return json_obj length
  */
int planetmint_parse_field(const json_t *json_obj, const char* field_name, char* output);

/** bool planetmint_parse_json
  * Handles JSON transaction prepared by bigchain driver.
  *
  * WARNING: The json_tx argument should be writable as it will be modified during deserialization.
  *
  * @param json_tx The transaction as stringified JSON
  * @param tx A pointer to PLANETMINT_TX struct.
  * @return Success or failure.
  */
bool planetmint_parse_json(char *json_tx, PLANETMINT_TX *tx);

/** bool prepare_tx
  * when 'operation' is CREATE then 'asset' can be arbitrary,
  * but when 'operation' is TRANSFER then 'asset' must be the transaction id of the asset which is to be tranfered.
  *
  * No spaces in the json structure: {"key 1": "value a"} becomes '{"key 1":"value a"}'
  *
  * Keys should be alphabetically ordered: {"key 1":"value a","A key":"A value"} becomes {"A key":"A value","key 1":"value a"}
  * No numbers fields, only strings {"key 1":2.3123} becomes {"key 1":"2.3123"}
  *
  * eg: asset input should be formatted like {"asset":{"data":{...}}} or {"asset":{"id":""}}
  *
  * WARNING: The asset and metadata arguments should be writable as they will be modified during deserialization.
  *
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param operation can be either 'C' or 'T' for CREATE and TRANSFER respectively.
  * @param asset JSON stringified transaction's asset.
  * @param metadata JSON stringified transaction's asset.
  * @param base_pubkey Private key base58 encoded as char array.
  * @return Success or failure.
  */
bool prepare_tx(PLANETMINT_TX *tx, const char operation, char *asset, char *metadata, char *base_pubkey);

/** void fulfill_tx
  * Fulfill a prepared transaction's input (default to index 0) with a given key pair and serialize the transaction.
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param json A uint8_t array that will contain serialized JSON transaction.
  * @param maxlen A uint16_t indicating max length of the output JSON.
  */
void fulfill_tx(PLANETMINT_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen);

/** void partial_fulfill_tx
  *  Used to create fulfillement for each input with different key pair.
  *  Fulfill a prepared transaction with a given key pair, without serializing the transaction.
  * @param tx A pointer to PLANETMINT_TX struct.
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param json A uint8_t array that will contain serialized JSON transaction.
  * @param maxlen A uint16_t indicating max length of the output JSON.
  * @param input_index A uint8_t indicating which input to fulfill.
  */
void partial_fulfill_tx(PLANETMINT_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen, uint8_t input_index);

#endif // _PLANETMINT_TX_H_
