#ifndef RDDL_CID_H
#define RDDL_CID_H

#include <stddef.h>
#include <stdint.h>

/* SHA-256 constants */
#define SHA2_256_CODEC 0x12
#define CID_VERSION_1 0x01
#define SHA256_DIGEST_LENGTH 32

/* Function prototypes */
void string_to_lowercase(char* str);
char* create_cid_v1_from_string(const char* data);
void sha256(const char* data, size_t data_size, uint8_t* hash);
uint8_t* decode_cid_v1(const char* base32_cid);

#endif /* RDDL_CID_H */
