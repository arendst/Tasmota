#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Python Class for compressing short strings.

This class contains a highly modified and optimized version of Unishox
for Tasmota converted in C ported to Pyhton3.

It was basically developed to individually compress and decompress small strings
(see https://github.com/siara-cc/Unishox)
In general compression utilities such as zip, gzip do not compress short strings
well and often expand them. They also use lots of memory which makes them unusable
in constrained environments like Arduino.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

class Unishox:
    """
    This is a highly modified and optimized version of Unishox
    for Tasmota, aimed at compressing `Rules` which are typically
    short strings from 50 to 500 bytes.

    @author Stephan Hadinger
    @revised Norbert Richter
    """

    # pylint: disable=bad-continuation,bad-whitespace,line-too-long
    cl_95 = [0x4000 +  3, 0x3F80 + 11, 0x3D80 + 11, 0x3C80 + 10, 0x3BE0 + 12, 0x3E80 + 10, 0x3F40 + 11, 0x3EC0 + 10, 0x3BA0 + 11, 0x3BC0 + 11, 0x3D60 + 11, 0x3B60 + 11, 0x3A80 + 10, 0x3AC0 + 10, 0x3A00 +  9, 0x3B00 + 10, 0x38C0 + 10, 0x3900 + 10, 0x3940 + 11, 0x3960 + 11, 0x3980 + 11, 0x39A0 + 11, 0x39C0 + 11, 0x39E0 + 12, 0x39F0 + 12, 0x3880 + 10, 0x3CC0 + 10, 0x3C00 +  9, 0x3D00 + 10, 0x3E00 +  9, 0x3F00 + 10, 0x3B40 + 11, 0x3BF0 + 12, 0x2B00 +  8, 0x21C0 + 11, 0x20C0 + 10, 0x2100 + 10, 0x2600 +  7, 0x2300 + 11, 0x21E0 + 12, 0x2140 + 11, 0x2D00 +  8, 0x2358 + 13, 0x2340 + 12, 0x2080 + 10, 0x21A0 + 11, 0x2E00 +  8, 0x2C00 +  8, 0x2180 + 11, 0x2350 + 13, 0x2F80 +  9, 0x2F00 +  9, 0x2A00 +  8, 0x2160 + 11, 0x2330 + 12, 0x21F0 + 12, 0x2360 + 13, 0x2320 + 12, 0x2368 + 13, 0x3DE0 + 12, 0x3FA0 + 11, 0x3DF0 + 12, 0x3D40 + 11, 0x3F60 + 11, 0x3FF0 + 12, 0xB000 +  4, 0x1C00 +  7, 0x0C00 +  6, 0x1000 +  6, 0x6000 +  3, 0x3000 +  7, 0x1E00 +  8, 0x1400 +  7, 0xD000 +  4, 0x3580 +  9, 0x3400 +  8, 0x0800 +  6, 0x1A00 +  7, 0xE000 +  4, 0xC000 +  4, 0x1800 +  7, 0x3500 +  9, 0xF800 +  5, 0xF000 +  5, 0xA000 +  4, 0x1600 +  7, 0x3300 +  8, 0x1F00 +  8, 0x3600 +  9, 0x3200 +  8, 0x3680 +  9, 0x3DA0 + 11, 0x3FC0 + 11, 0x3DC0 + 11, 0x3FE0 + 12]

    # enum {SHX_STATE_1 = 1, SHX_STATE_2};    // removed Unicode state
    SHX_STATE_1 = 1
    SHX_STATE_2 = 2

    SHX_SET1 = 0
    SHX_SET1A = 1
    SHX_SET1B = 2
    SHX_SET2 = 3

    sets = [['\0', ' ', 'e', '\0', 't', 'a', 'o', 'i', 'n', 's', 'r'],
            ['\0', 'l', 'c', 'd', 'h', 'u', 'p', 'm', 'b', 'g', 'w'],
            ['f', 'y', 'v', 'k', 'q', 'j', 'x', 'z', '\0', '\0', '\0'],
            ['\0', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'],
            ['.', ',', '-', '/', '?', '+', ' ', '(', ')', '$', '@'],
            [';', '#', ':', '<', '^', '*', '"', '{', '}', '[', ']'],
            ['=', '%', '\'', '>', '&', '_', '!', '\\', '|', '~', '`']]

    us_vcode = [2 + (0 << 3), 3 + (3 << 3), 3 + (1 << 3), 4 + (6 << 3), 0,
    #           5,            6,            7,            8, 9, 10
                4 + (4 << 3), 3 + (2 << 3), 4 + (8 << 3), 0, 0,  0,
    #           11,          12, 13,            14, 15
                4 + (7 << 3), 0,  4 + (5 << 3),  0,  5 + (9 << 3),
    #           16, 17, 18, 19, 20, 21, 22, 23
                0, 0, 0, 0, 0, 0, 0, 0,
    #           24, 25, 26, 27, 28, 29, 30, 31
                0, 0, 0, 0, 0, 0, 0, 5 + (10 << 3) ]
    #           0,            1,            2, 3,            4, 5, 6, 7,
    us_hcode  = [1 + (1 << 3), 2 + (0 << 3), 0, 3 + (2 << 3), 0, 0, 0, 5 + (3 << 3),
    #            8, 9, 10, 11, 12, 13, 14, 15,
                0, 0, 0, 0, 0, 0, 0, 5 + (5 << 3),
    #            16, 17, 18, 19, 20, 21, 22, 23
                0, 0, 0, 0, 0, 0, 0, 5 + (4 << 3),
    #            24, 25, 26, 27, 28, 29, 30, 31
                0, 0, 0, 0, 0, 0, 0, 5 + (6 << 3) ]
    # pylint: enable=bad-continuation,bad-whitespace

    ESCAPE_MARKER = 0x2A

    TERM_CODE = 0x37C0
    # TERM_CODE_LEN = 10
    DICT_CODE = 0x0000
    DICT_CODE_LEN = 5
    #DICT_OTHER_CODE = 0x0000
    #DICT_OTHER_CODE_LEN = 6
    RPT_CODE_TASMOTA = 0x3780
    RPT_CODE_TASMOTA_LEN = 10
    BACK2_STATE1_CODE = 0x2000
    BACK2_STATE1_CODE_LEN = 4
    #BACK_FROM_UNI_CODE = 0xFE00
    #BACK_FROM_UNI_CODE_LEN = 8
    LF_CODE = 0x3700
    LF_CODE_LEN = 9
    TAB_CODE = 0x2400
    TAB_CODE_LEN = 7
    ALL_UPPER_CODE = 0x2200
    ALL_UPPER_CODE_LEN = 8
    SW2_STATE2_CODE = 0x3800
    SW2_STATE2_CODE_LEN = 7
    ST2_SPC_CODE = 0x3B80
    ST2_SPC_CODE_LEN = 11
    BIN_CODE_TASMOTA = 0x8000
    BIN_CODE_TASMOTA_LEN = 3

    NICE_LEN = 5

    mask = [0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF]

    # pylint: disable=missing-function-docstring,invalid-name

    # Input
    # out = bytearray
    def append_bits(self, out, ol, code, clen, state):
        #print("Append bits {ol} {code} {clen} {state}".format(ol=ol, code=code, clen=clen, state=state))
        if state == self.SHX_STATE_2:
            # remove change state prefix
            if (code >> 9) == 0x1C:
                code <<= 7
                clen -= 7
        while clen > 0:
            cur_bit = ol % 8
            blen = 8 if (clen > 8) else clen
            a_byte = (code >> 8) & self.mask[blen - 1]
            #print("append_bits a_byte {ab} blen {blen}".format(ab=a_byte,blen=blen))
            a_byte >>= cur_bit
            if blen + cur_bit > 8:
                blen = (8 - cur_bit)
            if cur_bit == 0:
                out[ol // 8] = a_byte
            else:
                out[ol // 8] |= a_byte
            code <<= blen
            ol += blen
            if 0 == ol % 8:     # pylint: disable=misplaced-comparison-constant
                # we completed a full byte
                last_c = out[(ol // 8) - 1]
                if last_c in (0, self.ESCAPE_MARKER):
                    out[ol // 8] = 1 + last_c           # increment to 0x01 or 0x2B
                    out[(ol // 8) -1] = self.ESCAPE_MARKER   # replace old value with marker
                    ol += 8   # add one full byte
            clen -= blen
        return ol

    codes   = [0x82, 0xC3, 0xE5, 0xED, 0xF5]    # pylint: disable=bad-whitespace
    bit_len = [   5,    7,    9,   12,   16]    # pylint: disable=bad-whitespace

    def encodeCount(self, out, ol, count):
        #print("encodeCount ol = {ol}, count = {count}".format(ol=ol, count=count))
        till = 0
        base = 0
        for i in range(len(self.bit_len)):
            bit_len_i = self.bit_len[i]
            till += (1 << bit_len_i)
            if count < till:
                codes_i = self.codes[i]
                ol = self.append_bits(out, ol, (codes_i & 0xF8) << 8, codes_i & 0x07, 1)
                #print("encodeCount append_bits ol = {ol}, code = {code}, len = {len}".format(ol=ol,code=(codes_i & 0xF8) << 8,len=codes_i & 0x07))
                ol = self.append_bits(out, ol, (count - base) << (16 - bit_len_i), bit_len_i, 1)
                #print("encodeCount append_bits ol = {ol}, code = {code}, len = {len}".format(ol=ol,code=(count - base) << (16 - bit_len_i),len=bit_len_i))
                return ol
            base = till
        return ol

    # Returns (int, ol, state, is_all_upper)
    def matchOccurance(self, inn, len_, l_, out, ol, state, is_all_upper):
        # int j, k;
        longest_dist = 0
        longest_len = 0
        #for (j = l_ - self.NICE_LEN; j >= 0; j--) {
        j = l_ - self.NICE_LEN
        while j >= 0:
            k = l_
            #for (k = l_; k < len && j + k - l_ < l_; k++) {
            while k < len_ and j + k - l_ < l_:
                if inn[k] != inn[j + k - l_]:
                    break
                k += 1
            if k - l_ > self.NICE_LEN - 1:
                match_len = k - l_ - self.NICE_LEN
                match_dist = l_ - j - self.NICE_LEN + 1
                if match_len > longest_len:
                    longest_len = match_len
                    longest_dist = match_dist
            j -= 1

        if longest_len:
            #print("longest_len {ll}".format(ll=longest_len))
            #ol_save = ol
            if state == self.SHX_STATE_2 or is_all_upper:
                is_all_upper = 0
                state = self.SHX_STATE_1
                ol = self.append_bits(out, ol, self.BACK2_STATE1_CODE, self.BACK2_STATE1_CODE_LEN, state)

            ol = self.append_bits(out, ol, self.DICT_CODE, self.DICT_CODE_LEN, 1)
            ol = self.encodeCount(out, ol, longest_len)
            ol = self.encodeCount(out, ol, longest_dist)
            #print("longest_len {ll} longest_dist {ld} ol {ols}-{ol}".format(ll=longest_len, ld=longest_dist, ol=ol, ols=ol_save))
            l_ += longest_len + self.NICE_LEN
            l_ -= 1

            return l_, ol, state, is_all_upper
        return -l_, ol, state, is_all_upper


    def compress(self, inn, len_, out, len_out):
        ol = 0
        state = self.SHX_STATE_1
        is_all_upper = 0
        l = 0
        while l < len_:
        # for (l=0; l<len_; l++) {

            c_in = inn[l]

            if l and l < len_ - 4:
                if c_in == inn[l - 1] and c_in == inn[l + 1] and c_in == inn[l + 2] and c_in == inn[l + 3]:
                    rpt_count = l + 4
                    while rpt_count < len_ and inn[rpt_count] == c_in:
                        rpt_count += 1
                    rpt_count -= l

                    if state == self.SHX_STATE_2 or is_all_upper:
                        is_all_upper = 0
                        state = self.SHX_STATE_1
                        ol = self.append_bits(out, ol, self.BACK2_STATE1_CODE, self.BACK2_STATE1_CODE_LEN, state) # back to lower case and Set1

                    ol = self.append_bits(out, ol, self.RPT_CODE_TASMOTA, self.RPT_CODE_TASMOTA_LEN, 1)     # reusing CRLF for RPT
                    ol = self.encodeCount(out, ol, rpt_count - 4)
                    l += rpt_count
                    #l -= 1
                    continue

            if l < (len_ - self.NICE_LEN + 1):
                #l_old = l
                (l, ol, state, is_all_upper) = self.matchOccurance(inn, len_, l, out, ol, state, is_all_upper)
                if l > 0:
                    #print("matchOccurance l = {l} l_old = {lo}".format(l=l,lo=l_old))
                    l += 1    # for loop
                    continue

                l = -l

            if state == self.SHX_STATE_2:      # if Set2
                if ord(' ') <= c_in <= ord('@') or ord('[') <= c_in <= ord('`') or ord('{') <= c_in <= ord('~'):
                    pass
                else:
                    state = self.SHX_STATE_1        # back to Set1 and lower case
                    ol = self.append_bits(out, ol, self.BACK2_STATE1_CODE, self.BACK2_STATE1_CODE_LEN, state)

            is_upper = 0
            if ord('A') <= c_in <= ord('Z'):
                is_upper = 1
            else:
                if is_all_upper:
                    is_all_upper = 0
                    ol = self.append_bits(out, ol, self.BACK2_STATE1_CODE, self.BACK2_STATE1_CODE_LEN, state)

            if 32 <= c_in <= 126:
                if is_upper and not is_all_upper:
                    ll = l+5
                    # for (ll=l+5; ll>=l && ll<len_; ll--) {
                    while l <= ll < len_:
                        if inn[ll] < ord('A') or inn[ll] > ord('Z'):
                            break

                        ll -= 1

                    if ll == l-1:
                        ol = self.append_bits(out, ol, self.ALL_UPPER_CODE, self.ALL_UPPER_CODE_LEN, state)   # CapsLock
                        is_all_upper = 1

                if state == self.SHX_STATE_1 and ord('0') <= c_in <= ord('9'):
                    ol = self.append_bits(out, ol, self.SW2_STATE2_CODE, self.SW2_STATE2_CODE_LEN, state)   # Switch to sticky Set2
                    state = self.SHX_STATE_2

                c_in -= 32
                if is_all_upper and is_upper:
                    c_in += 32
                if c_in == 0 and state == self.SHX_STATE_2:
                    ol = self.append_bits(out, ol, self.ST2_SPC_CODE, self.ST2_SPC_CODE_LEN, state)       # space from Set2 ionstead of Set1
                else:
                    # ol = self.append_bits(out, ol, pgm_read_word(&c_95[c_in]), pgm_read_byte(&l_95[c_in]), state);  // original version with c/l in split arrays
                    cl = self.cl_95[c_in]
                    ol = self.append_bits(out, ol, cl & 0xFFF0, cl & 0x000F, state)

            elif c_in == 10:
                ol = self.append_bits(out, ol, self.LF_CODE, self.LF_CODE_LEN, state)         # LF
            elif c_in == '\t':
                ol = self.append_bits(out, ol, self.TAB_CODE, self.TAB_CODE_LEN, state)       # TAB
            else:
                ol = self.append_bits(out, ol, self.BIN_CODE_TASMOTA, self.BIN_CODE_TASMOTA_LEN, state)       # Binary, we reuse the Unicode marker which 3 bits instead of 9
                ol = self.encodeCount(out, ol, (255 - c_in) & 0xFF)


            # check that we have some headroom in the output buffer
            if ol // 8 >= len_out - 4:
                return -1      # we risk overflow and crash

            l += 1

        bits = ol % 8
        if bits:
            ol = self.append_bits(out, ol, self.TERM_CODE, 8 - bits, 1)   # 0011 0111 1100 0000 TERM = 0011 0111 11
        return (ol + 7) // 8
        # return ol // 8 + 1 if (ol%8) else 0


    def getBitVal(self, inn, bit_no, count):
        c_in = inn[bit_no >> 3]
        if bit_no >> 3 and self.ESCAPE_MARKER == inn[(bit_no >> 3) - 1]:
            c_in -= 1
        r = 1 << count if (c_in & (0x80 >> (bit_no % 8))) else 0
        #print("getBitVal r={r}".format(r=r))
        return r

    # Returns:
    # 0..11
    # or -1 if end of stream
    def getCodeIdx(self, code_type, inn, len_, bit_no_p):
        code = 0
        count = 0
        while count < 5:
            if bit_no_p >= len_:
                return -1, bit_no_p
            # detect marker
            if self.ESCAPE_MARKER == inn[bit_no_p >> 3]:
                bit_no_p += 8      # skip marker

            if bit_no_p >= len_:
                return -1, bit_no_p

            code += self.getBitVal(inn, bit_no_p, count)
            bit_no_p += 1
            count += 1
            code_type_code = code_type[code]
            if code_type_code and (code_type_code & 0x07) == count:
                #print("getCodeIdx = {r}".format(r=code_type_code >> 3))
                return code_type_code >> 3, bit_no_p

        #print("getCodeIdx  not found = {r}".format(r=1))
        return 1, bit_no_p

    def getNumFromBits(self, inn, bit_no, count):
        ret = 0
        while count:
            count -= 1
            if self.ESCAPE_MARKER == inn[bit_no >> 3]:
                bit_no += 8      # skip marker
            ret += self.getBitVal(inn, bit_no, count)
            bit_no += 1
        return ret

    def readCount(self, inn, bit_no_p, len_):
        (idx, bit_no_p) = self.getCodeIdx(self.us_hcode, inn, len_, bit_no_p)
        if idx >= 1:
            idx -= 1    # we skip v = 1 (code '0') since we no more accept 2 bits encoding
        if idx >= 5 or idx < 0:
            return 0, bit_no_p  # unsupported or end of stream
        till = 0
        bit_len_idx = 0
        base = 0
        #for (uint32_t i = 0; i <= idx; i++) {
        i = 0
        while i <= idx:
        # for i in range(idx):
            base = till
            bit_len_idx = self.bit_len[i]
            till += (1 << bit_len_idx)
            i += 1

        count = self.getNumFromBits(inn, bit_no_p, bit_len_idx) + base
        #print("readCount getNumFromBits = {count} ({bl})".format(count=count,bl=bit_len_idx))

        bit_no_p += bit_len_idx
        return count, bit_no_p

    def decodeRepeat(self, inn, len_, out, ol, bit_no):
        #print("decodeRepeat Enter")
        (dict_len, bit_no) = self.readCount(inn, bit_no, len_)
        dict_len += self.NICE_LEN
        (dist, bit_no) = self.readCount(inn, bit_no, len_)
        dist += self.NICE_LEN - 1
        #memcpy(out + ol, out + ol - dist, dict_len);
        i = 0
        while i < dict_len:
        #for i in range(dict_len):
            out[ol + i] = out[ol - dist + i]
            i += 1
        ol += dict_len

        return ol, bit_no

    def decompress(self, inn, len_, out, len_out):
        ol = 0
        bit_no = 0
        dstate = self.SHX_SET1
        is_all_upper = 0

        len_ <<= 3    # *8, len_ in bits
        out[ol] = 0
        while bit_no < len_:
            c = 0
            is_upper = is_all_upper
            (v, bit_no) = self.getCodeIdx(self.us_vcode, inn, len_, bit_no)    # read vCode
            #print("bit_no {b}. v = {v}".format(b=bit_no,v=v))
            if v < 0:
                break     # end of stream
            h = dstate     # Set1 or Set2
            if v == 0:    # Switch which is common to Set1 and Set2, first entry
                (h, bit_no) = self.getCodeIdx(self.us_hcode, inn, len_, bit_no)    # read hCode
                #print("bit_no {b}. h = {h}".format(b=bit_no,h=h))
                if h < 0:
                    break     # end of stream
                if h == self.SHX_SET1:          # target is Set1
                    if dstate == self.SHX_SET1:   # Switch from Set1 to Set1 us UpperCase
                        if is_all_upper:      # if CapsLock, then back to LowerCase
                            is_upper = 0
                            is_all_upper = 0
                            continue

                        (v, bit_no) = self.getCodeIdx(self.us_vcode, inn, len_, bit_no)   # read again vCode
                        if v < 0:
                            break     # end of stream
                        if v == 0:
                            (h, bit_no) = self.getCodeIdx(self.us_hcode, inn, len_, bit_no)  # read second hCode
                            if h < 0:
                                break      # end of stream
                            if h == self.SHX_SET1:  # If double Switch Set1, the CapsLock
                                is_all_upper = 1
                                continue

                        is_upper = 1      # anyways, still uppercase
                    else:
                        dstate = self.SHX_SET1  # if Set was not Set1, switch to Set1
                        continue

                elif h == self.SHX_SET2:    # If Set2, switch dstate to Set2
                    if dstate == self.SHX_SET1:
                        dstate = self.SHX_SET2
                    continue

                if h != self.SHX_SET1:    # all other Sets (why not else)
                    (v, bit_no) = self.getCodeIdx(self.us_vcode, inn, len_, bit_no)    # we changed set, now read vCode for char
                    if v < 0:
                        break      # end of stream

            if v == 0 and h == self.SHX_SET1A:
                #print("v = 0, h = self.SHX_SET1A")
                if is_upper:
                    (temp, bit_no) = self.readCount(inn, bit_no, len_)
                    out[ol] = 255 - temp    # binary
                    ol += 1
                else:
                    (ol, bit_no) = self.decodeRepeat(inn, len_, out, ol, bit_no)   # dist
                continue

            if h == self.SHX_SET1 and v == 3:
                # was Unicode, will do Binary instead
                (temp, bit_no) = self.readCount(inn, bit_no, len_)
                out[ol] = 255 - temp    # binary
                ol += 1
                continue

            if h < 7 and v < 11:
                #print("h {h} v {v}".format(h=h,v=v))
                c = ord(self.sets[h][v])
            if ord('a') <= c <= ord('z'):
                if is_upper:
                    c -= 32       # go to UpperCase for letters
            else:          # handle all other cases
                if is_upper and dstate == self.SHX_SET1 and v == 1:
                    c = ord('\t')     # If UpperCase Space, change to TAB
                if h == self.SHX_SET1B:
                    if 8 == v:   # was LF or RPT, now only LF   # pylint: disable=misplaced-comparison-constant
                        out[ol] = ord('\n')
                        ol += 1
                        continue

                    if 9 == v:           # was CRLF, now RPT    # pylint: disable=misplaced-comparison-constant
                        (count, bit_no) = self.readCount(inn, bit_no, len_)
                        count += 4
                        if ol + count >= len_out:
                            return -1        # overflow

                        rpt_c = out[ol - 1]
                        while count:
                            count -= 1
                            out[ol] = rpt_c
                            ol += 1
                        continue

                    if 10 == v:         # pylint: disable=misplaced-comparison-constant
                        break           # TERM, stop decoding

            out[ol] = c
            ol += 1

            if ol >= len_out:
                return -1         # overflow

        return ol

    # pylint: enable=missing-function-docstring


if __name__ == "__main__":
    # pylint: disable=line-too-long
    UNISHOX = Unishox()
    BYTES_ = bytearray(2048)
    INN = bytearray(b'ON Switch1#State==1 DO Add1 1 ENDON ON Var1#State==0 DO ShutterStop1 ENDON ON Var1#State==1 DO ShutterClose1 ENDON ON Var1#State>=2 DO Var1 0 ENDON ON Shutter1#Close DO Var1 0 ENDON ON Switch2#State==1 DO Add2 1 ENDON ON Var2#State==0 DO ShutterStop1 ENDON ON Var2#State==1 DO ShutterOpen1 ENDON ON Var2#State>=2 DO Var2 0 ENDON ON Shutter1#Open DO Var2 0 ENDON')
    LEN_ = UNISHOX.compress(INN, len(INN), BYTES_, len(BYTES_))
    print("Compressed from {fromm} to {to} ({p}%)".format(fromm=len(INN), to=LEN_, p=(100-LEN_/len(INN)*100)))

    OUT = bytearray(2048)
    LEN_ = UNISHOX.decompress(BYTES_, LEN_, OUT, len(OUT))
    print(str(OUT, 'utf-8').split('\x00')[0])
