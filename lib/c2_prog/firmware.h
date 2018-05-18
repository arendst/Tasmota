#include <sys/types.h>
#include <stdint.h>

ssize_t rf_search_and_write(uint8_t *buf, size_t size);
uint8_t rf_erase_flash();