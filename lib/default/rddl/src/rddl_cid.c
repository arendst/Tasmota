#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "sha2.h"
#include "base32.h"
#include "rddl_cid.h"

void string_to_lowercase(char* str) {
    for(int i = 0; str[i]; i++){
      str[i] = tolower(str[i]);
    }
}

char* create_cid_v1_from_string(const char* data) {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    sha256(data, strlen(data), hash);

    uint8_t pre_encoded_cid[SHA256_DIGEST_LENGTH + 4];  // 4 extra bytes for version and codec and digest length
    pre_encoded_cid[0] = 0x01;  // cidv1
    pre_encoded_cid[1] = 0x55;  // raw multicodec
    pre_encoded_cid[2] = 0x12;  // sha256
    pre_encoded_cid[3] = 0x20;  // sha256 digest length
    memcpy(pre_encoded_cid + 4, hash, SHA256_DIGEST_LENGTH);  // copy the hash after the version and codec

    size_t input_length = SHA256_DIGEST_LENGTH + 4;  // calculate the length of the actual content to be encoded
    size_t buffer_size = (input_length + 4) / 5 * 8 + 1;  // calculate the buffer size for the base32-encoded string

    char* base32_cid = (char*)getStack(buffer_size + 2);  // Allocate 2 extra bytes: one for the 'b' prefix and one for the null terminator
    base32_cid[0] = 'b';  // Add 'b' prefix
    base32_encode(pre_encoded_cid, input_length, base32_cid + 1, buffer_size, BASE32_ALPHABET_RFC4648);
    string_to_lowercase(base32_cid);
    return base32_cid;
}

void sha256(const char* data, size_t data_size, uint8_t* hash) {
    sha256_Raw((const uint8_t*)data, data_size, hash);
}


uint8_t* decode_cid_v1(const char* base32_cid) {
    size_t input_length = strlen(base32_cid) - 1;  // Calculate the length of the base32-encoded string without the 'b' prefix
    size_t buffer_size = (input_length * 5 + 7) / 8;  // Calculate the buffer size for the decoded string

    uint8_t* decoded_cid = (uint8_t*)malloc(buffer_size);
    if (!decoded_cid) {
        return NULL;  // Memory allocation failed
    }

    uint8_t* end = base32_decode(base32_cid + 1, input_length, decoded_cid, buffer_size, BASE32_ALPHABET_RFC4648);
    if (!end) {
        free(decoded_cid);
        return NULL;  // base32 decoding failed
    }

    uint8_t cid_version = decoded_cid[0];
    uint8_t multicodec = decoded_cid[1];
    uint8_t sha265_version = decoded_cid[2];
    uint8_t digest_length = decoded_cid[3];

    // Check that the CID version, multicodec, and multihash are as expected
    if (cid_version != 0x01 || multicodec != 0x55 || sha265_version != 0x12 || digest_length != 0x20) {
        free(decoded_cid);
        return NULL;  // Incorrect CID version, multicodec, or multihash
    }

    // Allocate a separate buffer for the multihash
    uint8_t* multihash = (uint8_t*)malloc(SHA256_DIGEST_LENGTH);
    if (!multihash) {
        free(decoded_cid);
        return NULL;  // Memory allocation failed
    }

    // Copy the multihash from the decoded CID
    memcpy(multihash, decoded_cid + 4, SHA256_DIGEST_LENGTH);

    // Free the decoded CID buffer
    free(decoded_cid);

    // Return the multihash
    return multihash;
}




