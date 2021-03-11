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
#include <stdint.h>
#include <ctype.h>

/* Functions to convert Little Endian byte stream to
 * uint16, uint32 and uint64
 */
uint16_t get_u16_le(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint16_t val;

	val  = (uint16_t)p[0];
	val |= (uint16_t)p[1] << 8;

	return val;
}

uint32_t get_u32_le(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint32_t val;

	val  = (uint32_t)p[0];
	val |= (uint32_t)p[1] << 8;
	val |= (uint32_t)p[2] << 16;
	val |= (uint32_t)p[3] << 24;

	return val;
}

uint64_t get_u64_le(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint64_t val;

	val  = (uint64_t)p[0];
	val |= (uint64_t)p[1] << 8;
	val |= (uint64_t)p[2] << 16;
	val |= (uint64_t)p[3] << 24;
	val |= (uint64_t)p[4] << 32;
	val |= (uint64_t)p[5] << 40;
	val |= (uint64_t)p[6] << 48;
	val |= (uint64_t)p[7] << 56;

	return val;
}

/* Functions to convert Big Endian byte stream to
 * uint16, uint32 and uint64
 */
uint16_t get_u16_be(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint16_t val;

	val  = (uint16_t)p[0] << 8;
	val |= (uint16_t)p[1];

	return val;
}

uint32_t get_u32_be(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint32_t val;

	val  = (uint32_t)p[0] << 24;
	val |= (uint32_t)p[1] << 16;
	val |= (uint32_t)p[2] << 8;
	val |= (uint32_t)p[3];

	return val;
}

uint64_t get_u64_be(const void *val_ptr)
{
	const uint8_t *p = (const uint8_t *)val_ptr;
	uint64_t val;

	val  = (uint64_t)p[0] << 56;
	val |= (uint64_t)p[1] << 48;
	val |= (uint64_t)p[2] << 40;
	val |= (uint64_t)p[3] << 32;
	val |= (uint64_t)p[4] << 24;
	val |= (uint64_t)p[5] << 16;
	val |= (uint64_t)p[6] << 8;
	val |= (uint64_t)p[7];

	return val;
}

/* Functions to convert uint16, uint32 and uint64
 * to Little Endian
 */
void put_u16_le(void *val_ptr, const uint16_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)val & 0xff;
	p[1] = (uint8_t)(val >> 8) & 0xff;
}

void put_u32_le(void *val_ptr, const uint32_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)val & 0xff;
	p[1] = (uint8_t)(val >> 8) & 0xff;
	p[2] = (uint8_t)(val >> 16) & 0xff;
	p[3] = (uint8_t)(val >> 24) & 0xff;
}

void put_u64_le(void *val_ptr, const uint64_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)val & 0xff;
	p[1] = (uint8_t)(val >> 8) & 0xff;
	p[2] = (uint8_t)(val >> 16) & 0xff;
	p[3] = (uint8_t)(val >> 24) & 0xff;
	p[4] = (uint8_t)(val >> 32) & 0xff;
	p[5] = (uint8_t)(val >> 40) & 0xff;
	p[6] = (uint8_t)(val >> 48) & 0xff;
	p[7] = (uint8_t)(val >> 56) & 0xff;
}

/* Functions to convert uint16, uint32 and uint64
 * to Big Endian
 */
void put_u16_be(void *val_ptr, const uint16_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)(val >> 8) & 0xff;
	p[1] = (uint8_t)val & 0xff;
}

void put_u32_be(void *val_ptr, const uint32_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)((val >> 24) & 0xff);
	p[1] = (uint8_t)((val >> 16) & 0xff);
	p[2] = (uint8_t)((val >> 8) & 0xff);
	p[3] = (uint8_t)(val & 0xff);
}

void put_u64_be(void *val_ptr, const uint64_t val)
{
	uint8_t *p = (uint8_t *)val_ptr;

	p[0] = (uint8_t)(val >> 56) & 0xff;
	p[1] = (uint8_t)(val >> 48) & 0xff;
	p[2] = (uint8_t)(val >> 40) & 0xff;
	p[3] = (uint8_t)(val >> 32) & 0xff;
	p[4] = (uint8_t)(val >> 24) & 0xff;
	p[5] = (uint8_t)(val >> 16) & 0xff;
	p[6] = (uint8_t)(val >> 8) & 0xff;
	p[7] = (uint8_t)val & 0xff;
}
