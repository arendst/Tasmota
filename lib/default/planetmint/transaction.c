#include "memzero.h"
#include "transaction.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define DEFAULT_URI "ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072"
#define DEFAULT_CONDITION_TYPE "ed25519-sha-256"

static char* chtoa(char* dest, char ch) {
  *dest   = ch;
  *++dest = '\0';
  return dest;
}

static char* atoa(char* dest, const char * src) {
  for( ; *src != '\0'; ++dest, ++src )
    *dest = *src;
  *dest = '\0';
  return dest;
}

static char* strname(char* dest, const char * name) {
  dest = chtoa( dest, '\"' );
  if ( NULL != name ) {
    dest = atoa(dest, name);
    dest = atoa(dest, "\":\"");
  }
  return dest;
}

/** override `json_str` from json maker with no escaping */
static char* bcdb_json_str(char* dest, char const* name, char const* value) {
  dest = strname(dest, name);
  dest = atoa(dest, value);
  dest = atoa(dest, "\",");
  return dest;
}

/*
* parse_json_invalid
* Called when a string cannot be opened as JSON
* Print parsing error to stderr
*/
static void parse_json_invalid(const char *object_name) {
  fprintf(stderr, "%s%s%s\n", "Error while parsing ", object_name, " .");
}

/*
* parse_json_property_missing
* Called when a required property is missing
* Print parsing error to stderr
*/
static void parse_json_property_missing(const char *property) {
  fprintf(stderr, "%s%s%s\n", "Error the ", property, " property is not found..");
}

static void parse_json_invalid_length(int len, int max_len) {
  fprintf(stderr, "%s%d%s%d\n", "Error. Len: ", len, " Max: ", max_len);
}

void der_encode_fulfill(uint8_t *pub_key, uint8_t *sig, uint8_t *fulfill) {
  uint16_t offset = 4;

  fulfill[0] = 0xA4;
  fulfill[1] = 0x64;
  fulfill[2] = 0x80;
  fulfill[3] = 0x20;

  memcpy(fulfill + offset, pub_key, 32);
  offset += 32;

  fulfill[offset++] = 0x81;
  fulfill[offset++] = 0x40;
  memcpy(fulfill + offset, sig, 64);
}

void bigchain_fulfill(BIGCHAIN_TX *tx, uint8_t *sig, uint8_t *pub_key, uint8_t input_index) {
  char fulfillment[256] = {0};
  uint8_t der[256] = {0};

  der_encode_fulfill(pub_key, sig, der);
  bintob64(fulfillment, der, 4 + 32 + 2 + 64);

  uint16_t size = strlen(fulfillment);
  for (uint16_t i = 0; i < size; i++) {
    if (fulfillment[i] == '+')
      fulfillment[i] = '-';
    else if (fulfillment[i] == '/')
      fulfillment[i] = '_';
  }

  for (uint16_t i = size; i > 1; i--) {
    if (fulfillment[i] == '=')
      fulfillment[i] = '\0';
    else
      break;
  }

  memcpy(tx->inputs[input_index].fulfillment, fulfillment, strlen(fulfillment));
}

void bigchain_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen) {
  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, (char*)json_tx);

  uint8_t tx_id[32] = {0};
  sha3_256((const unsigned char *)json_tx, MIN(maxlen, strlen((char*)json_tx)), tx_id);

  for (uint8_t i = 0; i < 32; i++) {
    sprintf(tx->id + i * 2, "%02x", tx_id[i]);
  }

  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, (char*)json_tx);
}

void bigchain_fulfill_and_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pub_key) {
  bigchain_fulfill(tx, sig, pub_key, 0);
  bigchain_serialize(tx, json_tx, maxlen);
}

void bigchain_sign_transaction(uint8_t *json_tx, uint16_t len, uint8_t *priv_key, uint8_t *pub_key, uint8_t *sig) {
  uint8_t hash[32] = {0};
  sha3_256((const unsigned char *)json_tx, len, hash);
  ed25519_sign(hash, 32, priv_key, pub_key, sig);
}

char *bigchain_build_condition_uri(char *public_key_base58, char *uri) {
  uint8_t der[38] = {0};
  uint8_t hash[32] = {0};
  char fingerp_base64[50] = {0};
  uint8_t pubkey[32] = {0};
  size_t binsz = 32 ;
  strcpy(uri, "ni:///sha-256;");
  b58tobin(pubkey, &binsz, public_key_base58);

  der[0] = 0x30;
  der[1] = 0x22;
  der[2] = 0x80;
  der[3] = 0x20; //content-length = 32
  memcpy(der + 4, pubkey, 32);
  sha256_Raw(der, 36, hash);
  bintob64(fingerp_base64, hash, 32);
  uint8_t size = strlen(fingerp_base64);
  for (uint16_t i = 0; i < size; i++) {
    if (fingerp_base64[i] == '+')
      fingerp_base64[i] = '-';
    else if (fingerp_base64[i] == '/')
      fingerp_base64[i] = '_';
  }

  for (uint16_t i = size; i > 1; i--) {
    if (fingerp_base64[i] == '=')
      fingerp_base64[i] = '\0';
    else
      break;
  }
  memcpy(uri + 14, fingerp_base64, 43);
  strcat(uri, "?fpt=ed25519-sha-256&cost=131072");
  return uri;
}

char *bigchain_build_json_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t num_outputs, char *json_obj) {
  char *p = json_obj;
  p = json_arrOpen(p, "outputs");
  char uri_str[90] = {0};

  for (uint8_t i = 0; i < num_outputs; i++) {
    p = json_objOpen(p, NULL);
    p = json_str(p, "amount", outputs[i].amount);
    p = json_objOpen(p, "condition");
    p = json_objOpen(p, "details");
    p = json_str(p, "public_key", outputs[i].condition.details.public_key);
    p = json_str(p, "type", outputs[i].condition.details.type);
    p = json_objClose(p);

    p = bcdb_json_str(p, "uri", bigchain_build_condition_uri(outputs[i].condition.details.public_key, uri_str));
    p = json_objClose(p);

    p = json_arrOpen(p, "public_keys");
    for (uint8_t j = 0; j < outputs[i].num_public_keys; j++) {
      p = json_str(p, NULL, outputs[i].public_keys[j]);
    }
    p = json_arrClose(p);
    p = json_objClose(p);
  }
  p = json_arrClose(p);
  return p;
}

char *bigchain_build_json_inputs(BIGCHAIN_INPUT *inputs, uint8_t num_inputs, char *json_obj) {
  char *p = json_obj;

  p = json_arrOpen(p, "inputs");
  for (uint8_t i = 0; i < num_inputs; i++) {
    p = json_objOpen(p, NULL);
    if (inputs[i].fulfillment[0] != '\0') {
      p = bcdb_json_str(p, "fulfillment", inputs[i].fulfillment);
    } else {
      p = json_null(p, "fulfillment");
    }

    if (inputs[i].fulfills.transaction_id[0] != '\0') {
      p = json_objOpen(p, "fulfills");
      p = json_int(p, "output_index", inputs[i].fulfills.output_index);
      p = json_str(p, "transaction_id", inputs[i].fulfills.transaction_id);
      p = json_objClose(p);
    } else {
      p = json_null(p, "fulfills");
    }

    p = json_arrOpen(p, "owners_before");
    for (uint8_t j = 0; j < inputs[i].num_owners; j++) {
      p = json_str(p, NULL, inputs[i].owners_before[j]);
    }
    p = json_arrClose(p);
    p = json_objClose(p);
  }
  p = json_arrClose(p);
  return p;
}

void bigchain_build_json_tx(BIGCHAIN_TX *tx, char *json_tx) {
  char *p = json_tx;
  p = json_objOpen(p, NULL);

  // ASSET
  p = json_arrOpen(p, "assets");   
  p = json_objOpen(p, NULL);   
  p = json_str(p, "data", tx->asset);
  p = json_objClose(p);
  p = json_arrClose(p);

  // ID
  if (tx->id[0] != '\0') {
    p = json_str(p, "id", tx->id);
  } else {
    p = json_null(p, "id");
  }

  // INPUTS
  p = bigchain_build_json_inputs(tx->inputs, tx->num_inputs, p);

  // METADATA
  if (tx->metadata[0] != '\0') {
    p = atoa(p, tx->metadata);
    p = atoa(p, ",");
  } else {
    // p = json_null(p, "metadata");
    p = json_str(p, "metadata", "None");
  }

  // OPERATION
  p = json_str(p, "operation", tx->operation);

  // OUTPUTS
  p = bigchain_build_json_outputs(tx->outputs, tx->num_outputs, p);

  // VERSION
  p = json_str(p, "version", tx->version);
  p = json_objClose(p);
  p = json_end(p);
}

bool prepare_tx(BIGCHAIN_TX *tx, const char operation, char *asset, char *metadata, char *base_pubkey) {
  memzero(tx, sizeof(BIGCHAIN_TX));

  // ASSET
  if(strlen(asset) > sizeof(tx->asset)){
    parse_json_invalid_length(strlen(asset), sizeof(tx->asset));
    return false;
  }
  strcpy(tx->asset, asset);

  // INPUTS
  memset(tx->inputs, 0, sizeof(BIGCHAIN_INPUT));
  memcpy(tx->inputs[0].owners_before[0], base_pubkey, strlen(base_pubkey));
  tx->inputs[0].num_owners = 1;
  tx->num_inputs = 1;

  if (operation == 'C') {
    memcpy(tx->operation, "CREATE", strlen("CREATE"));
  } else if (operation == 'T') {
    memcpy(tx->operation, "TRANSFER", strlen("TRANSFER"));
  }

  // METADATA
  if(metadata)
    strcpy(tx->metadata, metadata);

  // VERSION
  memcpy(tx->version, BDB_VERSION, strlen(BDB_VERSION));

  // OUTPUTS
  memset(tx->outputs, 0, sizeof(BIGCHAIN_OUTPUT));
  tx->outputs[0].amount[0] = '1';
  memset(&tx->outputs[0].condition, 0, sizeof(BIGCHAIN_CONDITION));
  memset(&tx->outputs[0].condition.details, 0, sizeof(CC));
  memcpy(tx->outputs[0].condition.details.public_key, base_pubkey, strlen(base_pubkey));
  memcpy(tx->outputs[0].condition.details.type, DEFAULT_CONDITION_TYPE, sizeof(DEFAULT_CONDITION_TYPE));
  memcpy(tx->outputs[0].public_keys[0], base_pubkey, strlen(base_pubkey));
  tx->outputs[0].num_public_keys = 1;
  tx->num_outputs = 1;

  return true;
}

void fulfill_tx(BIGCHAIN_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen) {
  uint8_t sig[140] = {0};
  uint8_t input_index = 0;
  bigchain_build_json_tx(tx, (char*)json);
  if (strcmp(tx->operation, "TRANSFER") == 0) {
    // For TRANSFER the json string must be concatenated with the input tx_id and the output_index
    char output_index[10];
    sprintf(output_index, "%d", tx->inputs[input_index].fulfills.output_index);
    strcat((char*)json, tx->inputs[input_index].fulfills.transaction_id);
    strcat((char*)json, output_index);
  }

  bigchain_sign_transaction((uint8_t *)json, strlen((char*)json), (uint8_t *)priv_key, (uint8_t *)pub_key, (uint8_t *)sig);
  bigchain_fulfill_and_serialize(tx, (uint8_t *)json, maxlen, (uint8_t *)sig, (uint8_t *)pub_key);
}

void partial_fulfill_tx(BIGCHAIN_TX *tx, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen, uint8_t input_index) {
  uint8_t sig[140] = {0};
  bigchain_build_json_tx(tx, (char*)json);
  if (strcmp(tx->operation, "TRANSFER") == 0) {
    char output_index[10];
    sprintf(output_index, "%d", tx->inputs[input_index].fulfills.output_index);
    strcat((char*)json, tx->inputs[input_index].fulfills.transaction_id);
    strcat((char*)json, output_index);
  }
  bigchain_sign_transaction((uint8_t *)json, strlen((char*)json), (uint8_t *)priv_key, (uint8_t *)pub_key, (uint8_t *)sig);
  bigchain_fulfill(tx, sig, pub_key, input_index);
}

int bigchain_parse_inputs(const json_t* json_obj, BIGCHAIN_INPUT *inputs) {
  const json_t *inputs_field = json_getProperty(json_obj, "inputs");
  if (!inputs_field || JSON_ARRAY != json_getType(inputs_field)) {
    return 0;
  }

  memset(inputs, 0, sizeof(BIGCHAIN_INPUT));
  uint8_t i = 0;
  const json_t *input;

  for (input = json_getChild(inputs_field); input != 0; input = json_getSibling(input) ) {
    if (JSON_OBJ == json_getType(input)) {
      // FULFILLEMENT
      const char* fulfillment = json_getPropertyValue(input, "fulfillment");
      if (fulfillment) {
        memcpy(inputs[i].fulfillment, fulfillment, strlen(fulfillment));
      }

      // FULFILLS
      memset(&inputs[i].fulfills, 0, sizeof(BIGCHAIN_INPUT_FULFILLS));
      const json_t *fulfills_obj = json_getProperty(input, "fulfills");
      if (fulfills_obj && JSON_OBJ == json_getType(fulfills_obj)) {
        const json_t *output_index_field = json_getProperty(fulfills_obj, "output_index");
        int output_index =  json_getInteger(output_index_field);
        const char *transaction_id = json_getPropertyValue(fulfills_obj, "transaction_id");
        if (transaction_id) {
          inputs[i].fulfills.output_index = output_index;
          memcpy(inputs[i].fulfills.transaction_id, transaction_id, strlen(transaction_id));
        }
      }

      // OWNERS_BEFORE
      const json_t *owners_before = json_getProperty(input, "owners_before");
      if (!owners_before || JSON_ARRAY != json_getType(owners_before) ) {
        parse_json_property_missing("owners_before list");
        return 0;
      }
      uint8_t j = 0;
      const json_t *owner_before_key;
      for (owner_before_key = json_getChild(owners_before); owner_before_key != 0; owner_before_key = json_getSibling(owner_before_key) ) {
        if (JSON_TEXT == json_getType(owner_before_key)) {
          const char* owner_before = json_getValue(owner_before_key);
          memcpy(inputs[i].owners_before[j], owner_before, strlen(owner_before));
        }
        j++;
      }
      inputs[i].num_owners = j;
    }
    i++;
  }
  return i;
}


int bigchain_parse_outputs(const json_t *json_obj, BIGCHAIN_OUTPUT *outputs) {
  const json_t *outputs_field = json_getProperty(json_obj, "outputs");
  if (!outputs_field || JSON_ARRAY != json_getType(outputs_field)) {
    return 0;
  }

  memset(outputs, 0, sizeof(BIGCHAIN_OUTPUT));
  uint8_t i = 0;
  const json_t *output;
  for (output = json_getChild(outputs_field); output != 0; output = json_getSibling(output)) {
    if (JSON_OBJ == json_getType(output)) {
      // AMOUNT
      const char* amount = json_getPropertyValue(output, "amount");
      if (amount) {
        memcpy(outputs[i].amount, amount, strlen(amount));
      }

      // CONDITION
      const json_t *condition = json_getProperty(output, "condition");
      if (!condition || JSON_OBJ != json_getType(condition)) {
        parse_json_property_missing("condition");
        return 0;
      }

      // CONDITION->details
      const json_t *details = json_getProperty(condition, "details");
      if (!details || JSON_OBJ != json_getType(details)) {
        parse_json_property_missing("details");
        return 0;
      }

      const char* details_type = json_getPropertyValue(details, "type");
      if (details_type) {
        memcpy(outputs[i].condition.details.type, details_type, strlen(details_type));
      }

      const char* details_public_key = json_getPropertyValue(details, "public_key");
      if (details_public_key) {
        memcpy(outputs[i].condition.details.public_key, details_public_key, strlen(details_public_key));
      }

      // CONDITION->uri
      const char* uri = json_getPropertyValue(details, "uri");
      if (uri) {
        memcpy(outputs[i].condition.uri, uri, strlen(uri));
      }

      // PUBLIC_KEYS
      const json_t *public_keys = json_getProperty(output, "public_keys");
      if (!public_keys || JSON_ARRAY != json_getType(public_keys)) {
        parse_json_property_missing("public_keys list");
        return 0;
      }
      uint8_t j = 0;
      const json_t *public_key_key;
      for (public_key_key = json_getChild(public_keys); public_key_key != 0; public_key_key = json_getSibling(public_key_key) ) {
        if (JSON_TEXT == json_getType(public_key_key)) {
          const char* public_key = json_getValue(public_key_key);
          memcpy(outputs[i].public_keys[j], public_key, strlen(public_key));
        }
        j++;
      }
      outputs[i].num_public_keys = j;
    }
    i++;
  }
  return i;
}

static char *bigchain_parse_object(const json_t *json, char *str, const char *name) {
  const jsonType_t type = json_getType(json);
  char *p = str;
  if (type != JSON_OBJ && type != JSON_ARRAY) {
    return p;
  }

  p = type == JSON_OBJ ? json_objOpen(p, name) : json_arrOpen(p, name);

  const json_t *child;
  for (child = json_getChild(json); child != 0; child = json_getSibling(child)) {
    jsonType_t propertyType = json_getType(child);
    const char *child_name = json_getName(child);
    if (propertyType == JSON_OBJ || propertyType == JSON_ARRAY) {
      p = bigchain_parse_object(child, p, child_name);
    } else {
      const char *value = json_getValue(child);
      if (value) {
        // TODO: do types checks to allow parsing as int or boolean
        if (value[0] != '\0') {
          p = json_str(p, child_name, value);
        } else {
          p = json_null(p, child_name);
        }
      }
    }
  }

  p = type == JSON_OBJ ? json_objClose(p) : json_arrClose(p);
  return p;
}

int bigchain_parse_field(const json_t *json_obj, const char *field_name, char *output) {
  const json_t *field = json_getProperty(json_obj, field_name);
  const jsonType_t type = json_getType(field);
  if (type != JSON_OBJ && type != JSON_ARRAY) {
    parse_json_property_missing(field_name);
    return 0;
  }
  char *p = bigchain_parse_object(field, output, field_name);
  p = json_end(p);
  return p - output;
}

bool bigchain_parse_json(char* json_tx, BIGCHAIN_TX *tx) {
  //? TODO: add max fields, max asset and metadata sizes arguments
  memzero(tx, sizeof(BIGCHAIN_TX));
  json_t mem[128];
  const json_t *json_obj = json_create((char*)json_tx, mem, sizeof mem / sizeof *mem);
  if (!json_obj) {
    parse_json_invalid("Transaction");
    return false;
  }

  // ASSET
  char asset[ASSET_MAX_SIZE];
  int len = bigchain_parse_field(json_obj, "asset", asset);
  if (len >= sizeof asset) {
    parse_json_invalid_length(len, (int)sizeof asset - 1);
    return false;
  }
  memcpy(tx->asset, asset, len);

  // ID
  const char* id = json_getPropertyValue(json_obj, "id");
  if (id) {
    memcpy(tx->id, id, strlen(id));
  }

  // INPUTS
  int num_inputs = bigchain_parse_inputs(json_obj, tx->inputs);
  tx->num_inputs = num_inputs;

  // METADATA
  char metadata[METADATA_MAX_SIZE];
  len = bigchain_parse_field(json_obj, "metadata", metadata);
  if (len >= sizeof asset) {
    parse_json_invalid_length(len, (int)sizeof metadata - 1);
    return false;
  }
  memcpy(tx->metadata, metadata, len);

  // OPERATION
  const char* operation = json_getPropertyValue(json_obj, "operation");
  // TODO: validate operation
  if (!operation) {
    parse_json_property_missing("operation");
    return false;
  }
  memcpy(tx->operation, operation, strlen(operation));

  // OUTPUTS
  int num_outputs = bigchain_parse_outputs(json_obj, tx->outputs);
  tx->num_outputs = num_outputs;

  // VERSION
  const char* version = json_getPropertyValue(json_obj, "version");
  //? TODO: validate version
  if (!version) {
    parse_json_property_missing("version");
    return false;
  }
  memcpy(tx->version, version, strlen(version));

  return true;
}
