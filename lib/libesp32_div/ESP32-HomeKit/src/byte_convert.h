// Copyright 2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/**
 * \file byte_convert.h
 * \brief Conversion between integers and byte streams
 *
 * This module offers APIs to convert 16bit, 32bit and 64 bit unsigned integers
 * into Little/Big Endian byte streams
 */
#ifndef _BYTE_CONVERT_H
#define _BYTE_CONVERT_H

#include <stdint.h>

/** Little Endian to uint16 Conversion
 * Get a uint16 integer from a little Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 2 byte stream
 *
 * \return The converted uint16 integer
 */
uint16_t get_u16_le(const void *val_ptr);

/** Little Endian to uint32 Conversion
 * Get a uint32 integer from a little Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 4 byte stream
 *
 * \return The converted uint32 integer
 */
uint32_t get_u32_le(const void *val_ptr);

/** Little Endian to uint64 Conversion
 * Get a uint64 integer from a little Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 8 byte stream
 *
 * \return The converted uint32 integer
 */
uint64_t get_u64_le(const void *val_ptr);

/** Big Endian to uint16 Conversion
 * Get a uint16 integer from a Big Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 2 byte stream
 *
 * \return The converted uint16 integer
 */
uint16_t get_u16_be(const void *val_ptr);

/** Big Endian to uint32 Conversion
 * Get a uint32 integer from a Big Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 4 byte stream
 *
 * \return The converted uint32 integer
 */
uint32_t get_u32_be(const void *val_ptr);

/** Big Endian to uint64 Conversion
 * Get a uint64 integer from a Big Endian byte stream
 *
 * \param[in] val_ptr Pointer to the 8 byte stream
 *
 * \return The converted uint16 integer
 */
uint64_t get_u64_be(const void *val_ptr);

/** Uint16 to Little Endian Conversion
 *
 * Put a uint16 integer into a Little Endian byte stream
 *
 * \param[out] Pointer to a 2 byte stream that will be filled with the value
 *
 * \param[in] The uint16 integer
 */
void put_u16_le(void *val_ptr, const uint16_t val);

/** Uint32 to Little Endian Conversion
 *
 * Put a uint32 integer into a Little Endian byte stream
 *
 * \param[out] Pointer to a 4 byte stream that will be filled with the value
 *
 * \param[in] The uint32 integer
 */
void put_u32_le(void *val_ptr, const uint32_t val);

/** Uint64 to Little Endian Conversion
 *
 * Put a uint64 integer into a Little Endian byte stream
 *
 * \param[out] Pointer to a 8 byte stream that will be filled with the value
 *
 * \param[in] The uint64 integer
 */
void put_u64_le(void *val_ptr, const uint64_t val);

/** Uint16 to Big Endian Conversion
 *
 * Put a uint16 integer into a Big Endian byte stream
 *
 * \param[out] Pointer to a 2 byte stream that will be filled with the value
 *
 * \param[in] The uint16 integer
 */
void put_u16_be(void *val_ptr, const uint16_t val);

/** Uint32 to Big Endian Conversion
 *
 * Put a uint32 integer into a Big Endian byte stream
 *
 * \param[out] Pointer to a 4 byte stream that will be filled with the value
 *
 * \param[in] The uint32 integer
 */
void put_u32_be(void *val_ptr, const uint32_t val);

/** Uint64 to Big Endian Conversion
 *
 * Put a uint64 integer into a Big Endian byte stream
 *
 * \param[out] Pointer to a 8 byte stream that will be filled with the value
 *
 * \param[in] The uint64 integer
 */
void put_u64_be(void *val_ptr, const uint64_t val);
#endif /* _BYTE_CONVERT_H */
