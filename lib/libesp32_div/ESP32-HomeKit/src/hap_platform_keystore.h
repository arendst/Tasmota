/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _HAP_PLATFORM_KEYSTORE_H_
#define _HAP_PLATFORM_KEYSTORE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Get NVS Partition Name
 *
 * @return pointer to an allocated string indicating NVS partition Name
 */
char * hap_platform_keystore_get_nvs_partition_name();


/** Get Factory NVS Partition Name
 *
 * @return pointer to an allocated string indicating Factory NVS partition Name
 */
char * hap_platform_keystore_get_factory_nvs_partition_name();

/** Initialise Key Store Partition
 *
 * @param[in] part_name Name of Partition
 * @param[in] read_only True for Read-Only, False for Read-Write
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platform_keystore_init_partition(const char *part_name, bool read_only);

/** Get Value from Key Store
 *
 * @param[in] part_name Name of Partition
 * @param[in] name_space Name space for the key
 * @param[in] key Name of the key
 * @param[out] val Allocated buffer into which the value will be read
 * @param[in,out] val_size Size of the allocated value buffer. Will hold the size of value read on success
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platform_keystore_get(const char *part_name, const char *name_space, const char *key, uint8_t *val, size_t *val_size);

/** Set Value in Key Store
 *
 * @param[in] part_name Name of Partition
 * @param[in] name_space Name space for the key
 * @param[in] key Name of the key
 * @param[in] val Pointer to the value buffer
 * @param[in] val_len Length of the value buffer
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platform_keystore_set(const char *part_name, const char *name_space, const char *key, const uint8_t *val, const size_t val_len);

/** Delete Entry from Key Store
 *
 * @param[in] part_name Name of Partition
 * @param[in] name_space Name space for the key
 * @param[in] key Name of the key
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platform_keystore_delete(const char *part_name, const char *name_space, const char *key);

/** Delete Name space from Key Store
 *
 * @param[in] part_name Name of Partition
 * @param[in] name_space Name space for the key
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platform_keystore_delete_namespace(const char *part_name, const char *name_space);

/** Erase a Key Store partition
 *
 * @param[in] part_name Name of Partition
 *
 * @return 0 on success
 * @return -1 on error
 */
int hap_platfrom_keystore_erase_partition(const char *part_name);
#ifdef __cplusplus
}
#endif
#endif /* _HAP_PLATFORM_KEYSTORE_H_ */
