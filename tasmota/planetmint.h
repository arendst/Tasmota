#ifndef _BIGCHAIN_TX_H_
#define _BIGCHAIN_TX_H_
#define BDB_VERSION "3.0"
#include "tiny-json.h"
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

/** Structure representing BigChainDB output's condition (only fit Ed25519). */
typedef struct {
  CC details;
  char uri[256];
} BIGCHAIN_CONDITION;

/** Structure representing BigChainDB a transaction's output properties. */
typedef struct {
  char amount[8];
  BIGCHAIN_CONDITION condition;
  // FIXED CONDITION FOR EDCURVE
  char public_keys[8][45];
  uint8_t num_public_keys;
} BIGCHAIN_OUTPUT;


/** Structure representing BigChainDB a fulfilled transaction's input properties. */
typedef struct {
  char transaction_id[65];
  uint8_t output_index;
} BIGCHAIN_INPUT_FULFILLS;

/** Structure representing BigChainDB a transaction's input properties. */
typedef struct {
  char fulfillment[256];
  BIGCHAIN_INPUT_FULFILLS fulfills;
  char owners_before[8][256];
  uint8_t num_owners;
} BIGCHAIN_INPUT;

/** Structure representing BigChainDB transaction properties. */
typedef struct {
  char asset[ASSET_MAX_SIZE];
  char metadata[METADATA_MAX_SIZE];
  char operation[32];
  BIGCHAIN_OUTPUT outputs[8];
  uint8_t num_outputs;
  BIGCHAIN_INPUT inputs[8];
  uint8_t num_inputs;
  char version[8];
  char id[65];
} BIGCHAIN_TX;

/**
  * void bigchain_fulfill
  * Create the DER encoded fulfillement for a given transaction's input.
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param priv_key Public key useable to verify signature as uint8_t array .
  * @param sig Transaction signature as uint8_t array
  * @param input_index Transaction's input index to fulfill
  */
void bigchain_fulfill(BIGCHAIN_TX *tx, uint8_t *sig, uint8_t *pub_key, uint8_t input_index);

/**
  * void bigchain_serialize
  * Generate transaction's id by hashing the serialized transaction, populate tx.id and update JSON string
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param json_tx Transaction as JSON string.
  * @param maxlen Transaction's maximum length
  */
void bigchain_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen);

/**
  * void bigchain_fulfill_and_serialize
  * Execute bigchain_fulfill and bigchain_serialize for a given transaction input.
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param json_tx Transaction as JSON string.
  * @param maxlen Transaction's maximum length
  * @param priv_key Public key useable to verify signature as uint8_t array .
  * @param sig Transaction signature as uint8_t array
  */
void bigchain_fulfill_and_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pub_key);

/**
  * void bigchain_sign_transaction
  * Takes a json string, hashes it sha3_256 and signs it with ed25519.
  * @param json_tx Transaction as JSON string.
  * @param len Transaction's length
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param sig Signature output as uint8_t array
  */
void bigchain_sign_transaction(uint8_t *json_tx, uint16_t len, uint8_t *priv_key, uint8_t *pub_key, uint8_t *sig);

/** char *bigchain_build_json_inputs
  * Build JSON transaction's inputs from BIGCHAIN_INPUT struct array
  * @param inputs A pointer to BIGCHAIN_INPUT array.
  * @param num_inputs Count of inputs.
  * @param json_obj Transaction output as JSON string
  */
char *bigchain_build_json_inputs(BIGCHAIN_INPUT *inputs, uint8_t num_inputs, char *json_obj);

/** char *bigchain_build_json_outputs
  * Build JSON transaction's outputs from BIGCHAIN_OUTPUT struct array
  * @param outputs A pointer to BIGCHAIN_OUTPUT array.
  * @param num_outputs Count of outputs.
  * @param json_obj Transaction output as JSON string
  */
char *bigchain_build_json_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t num_outputs, char *json_obj);

/** void bigchain_build_json_tx
  * Build JSON transaction from BIGCHAIN_TX struct
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param json_tx Transaction output as JSON string
  */
void bigchain_build_json_tx(BIGCHAIN_TX *tx, char *json_tx);

/** int bigchain_parse_inputs
  * Handles JSON transaction inputs prepared by bigchain driver
  * @param json_obj A valid handler of a json object.
  * @param inputs A pointer to BIGCHAIN_INPUT array.
  * @return Inputs count.
  */
int bigchain_parse_inputs(const json_t *json_obj, BIGCHAIN_INPUT *inputs);

/** int bigchain_parse_outputs
  * Handles JSON transaction outputs prepared by bigchain driver
  * @param json_obj A valid handler of a json object.
  * @param outputs A pointer to BIGCHAIN_OUTPUT array.
  * @return Outputs count.
  */
int bigchain_parse_outputs(const json_t *json_obj, BIGCHAIN_OUTPUT *outputs);

/** int bigchain_parse_field
  * Handles JSON transaction asset or metadata prepared by user
  * @param json_obj A valid handler of a json object.
  * @param field_name Should be asset or metadata
  * @param output A pointer to char array.
  * @return json_obj length
  */
int bigchain_parse_field(const json_t *json_obj, const char* field_name, char* output);

/** bool bigchain_parse_json
  * Handles JSON transaction prepared by bigchain driver.
  *
  * WARNING: The json_tx argument should be writable as it will be modified during deserialization.
  *
  * @param json_tx The transaction as stringified JSON
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @return Success or failure.
  */
bool bigchain_parse_json(char *json_tx, BIGCHAIN_TX *tx);

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
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param operation can be either 'C' or 'T' for CREATE and TRANSFER respectively.
  * @param asset JSON stringified transaction's asset.
  * @param metadata JSON stringified transaction's asset.
  * @param base_pubkey Private key base58 encoded as char array.
  * @return Success or failure.
  */
bool prepare_tx(BIGCHAIN_TX *tx, const char operation, char *asset, char *metadata, char *base_pubkey);

/** void fulfill_tx
  * Fulfill a prepared transaction's input (default to index 0) with a given key pair and serialize the transaction.
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param json A uint8_t array that will contain serialized JSON transaction.
  * @param maxlen A uint16_t indicating max length of the output JSON.
  */
void fulfill_tx(BIGCHAIN_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen);

/** void partial_fulfill_tx
  *  Used to create fulfillement for each input with different key pair.
  *  Fulfill a prepared transaction with a given key pair, without serializing the transaction.
  * @param tx A pointer to BIGCHAIN_TX struct.
  * @param priv_key Public key as uint8_t array .
  * @param pub_key Private key as uint8_t array.
  * @param json A uint8_t array that will contain serialized JSON transaction.
  * @param maxlen A uint16_t indicating max length of the output JSON.
  * @param input_index A uint8_t indicating which input to fulfill.
  */
void partial_fulfill_tx(BIGCHAIN_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen, uint8_t input_index);

bool getKeyFromSeed( const uint8_t* seed, uint8_t* priv_key, uint8_t* pub_key);


#endif // _BIGCHAIN_TX_H_
