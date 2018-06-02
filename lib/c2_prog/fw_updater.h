#include <sys/types.h>
#include <stdint.h>

#define RF_RECORD_NO_START_FOUND -1
#define RF_RECORD_NO_END_FOUND -2
#define RF_RECORD_START_AND_END_FOUND 0

ssize_t rf_find_hex_record_start(uint8_t *buf, size_t size);
ssize_t rf_find_hex_record_end(uint8_t *buf, size_t size);
ssize_t rf_decode_and_write(uint8_t *record, size_t size);
ssize_t rf_glue_remnant_with_new_data_and_write(const uint8_t *remnant_data, uint8_t *new_data, size_t new_data_len);
ssize_t rf_search_and_write(uint8_t *buf, size_t size);
uint8_t rf_erase_flash();