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
#include <hexbin.h>
static int hex2bin_byte(uint8_t *byte)
{
    if (*byte >= '0' && *byte <= '9')
        *byte -= '0';
    else if (*byte >= 'a' && *byte <= 'f')
        *byte -= ('a' - 0x0a);
    else if (*byte >= 'A' && *byte <= 'F')
        *byte -= ('A' - 0x0a);
    else
        return -1;
    return 0;
}

int hex2bin(const char *ihex, size_t ilen, uint8_t *obin, size_t *olen)
{
    if (ilen > (2 * (*olen))) {
        return -1;
    } else if (ilen % 2) {
        return -1;
    }
    int i, j;
    uint8_t byte;
    for (i = 0, j = 0; i < ilen / 2; i++) {
       byte = ihex[j++];
       if (hex2bin_byte(&byte) < 0) {
           return -1;
       }
       obin[i] = (byte << 4);
       byte = ihex[j++];
       if (hex2bin_byte(&byte) < 0) {
           return -1;
       }
       obin[i] |= byte;
    }
    *olen = i;
    return 0;
}
