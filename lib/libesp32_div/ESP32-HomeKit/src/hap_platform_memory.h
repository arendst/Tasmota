/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#ifndef _HAP_PLATFORM_MEMORY_H_
#define _HAP_PLATFORM_MEMORY_H_
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif


/** Allocate memory
 *
 * This API allocates "size" bytes of memory and returns a pointer to the allocated memory.
 *
 * @param[in] size Number of bytes to be allocated
 *
 * @return pointer to the allocated memory
 * @return NULL on failure
 */
void * hap_platform_memory_malloc(size_t size);

/** Allocate contiguous memory for items
 *
 * This API contiguously allocates enough space for "count" objects that are "size" bytes of memory each
 * and returns a pointer to the allocated memory.  The allocated memory is filled with bytes of value zero.
 *
 * @param[in] count Number of items
 * @param[in] size Size of each item
 *
 * @return pointer to the allocated memory
 * @return NULL on failure
 */
void * hap_platform_memory_calloc(size_t count, size_t size);

/** Free allocate memory
 *
 * This API frees the memory allocated by hap_platform_memory_malloc() or hap_platform_memory_calloc()
 *
 * @param[in] ptr Pointer to the allocated memory
 */
void hap_platform_memory_free(void *ptr);

#ifdef __cplusplus
}
#endif
#endif /* _HAP_PLATFORM_MEMORY_H_ */
