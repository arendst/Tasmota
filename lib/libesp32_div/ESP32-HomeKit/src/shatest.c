/************************** shatest.c **************************/
/***************** See RFC 6234 for details. *******************/
/* Copyright (c) 2011 IETF Trust and the persons identified as */
/* authors of the code.  All rights reserved.                  */
/* See sha.h for terms of use and redistribution.              */

/*
 *  Description:
 *    This file will exercise the SHA code performing
 *      the three tests documented in FIPS PUB 180-3
 *        (http://csrc.nist.gov/publications/fips/
 *         fips180-2/fips180-2withchangenotice.pdf)
 *      one that calls SHAInput with an exact multiple of 512 bits
 *      the seven tests documented for each algorithm in
 *        "The Secure Hash Algorithm Validation System (SHAVS)"
 *        (http://csrc.nist.gov/cryptval/shs/SHAVS.pdf),
 *        three of which are bit-level tests
 *
 *    These tests have subsequently been moved to pages linked from
 *      http://csrc.nist.gov/groups/ST/toolkit/examples.html
 *
 *    This file will exercise the HMAC SHA1 code performing
 *      the seven tests documented in RFCs [RFC 2202] and [RFC 4231].
 *
 *    This file will exercise the HKDF code performing
 *      the seven tests documented in RFC 4869.
 *
 *    To run the tests and just see PASSED/FAILED, use the -p option.
 *
 *    Other options exercise:
 *      hashing an arbitrary string
 *      hashing a file's contents
 *      a few error test checks
 *      printing the results in raw format
 *
 *  Portability Issues:
 *    None.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>     /* defines getopt() and optarg */
#include "sha.h"

static int scasecmp(const char *s1, const char *s2);

/*
 *  Define patterns for testing
 */
#define TEST1    "abc"
#define TEST2_1  \
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
#define TEST2_2a \
        "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
#define TEST2_2b \
        "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
#define TEST2_2  TEST2_2a TEST2_2b
#define TEST3    "a"                            /* times 1000000 */
#define TEST4a   "01234567012345670123456701234567"
#define TEST4b   "01234567012345670123456701234567"
    /* an exact multiple of 512 bits */
#define TEST4   TEST4a TEST4b                   /* times 10 */
#define TEST7_1 \
  "\x49\xb2\xae\xc2\x59\x4b\xbe\x3a\x3b\x11\x75\x42\xd9\x4a\xc8"
#define TEST8_1 \
  "\x9a\x7d\xfd\xf1\xec\xea\xd0\x6e\xd6\x46\xaa\x55\xfe\x75\x71\x46"
#define TEST9_1 \
  "\x65\xf9\x32\x99\x5b\xa4\xce\x2c\xb1\xb4\xa2\xe7\x1a\xe7\x02\x20" \
  "\xaa\xce\xc8\x96\x2d\xd4\x49\x9c\xbd\x7c\x88\x7a\x94\xea\xaa\x10" \
  "\x1e\xa5\xaa\xbc\x52\x9b\x4e\x7e\x43\x66\x5a\x5a\xf2\xcd\x03\xfe" \
  "\x67\x8e\xa6\xa5\x00\x5b\xba\x3b\x08\x22\x04\xc2\x8b\x91\x09\xf4" \
  "\x69\xda\xc9\x2a\xaa\xb3\xaa\x7c\x11\xa1\xb3\x2a"
#define TEST10_1 \
  "\xf7\x8f\x92\x14\x1b\xcd\x17\x0a\xe8\x9b\x4f\xba\x15\xa1\xd5\x9f" \
  "\x3f\xd8\x4d\x22\x3c\x92\x51\xbd\xac\xbb\xae\x61\xd0\x5e\xd1\x15" \
  "\xa0\x6a\x7c\xe1\x17\xb7\xbe\xea\xd2\x44\x21\xde\xd9\xc3\x25\x92" \
  "\xbd\x57\xed\xea\xe3\x9c\x39\xfa\x1f\xe8\x94\x6a\x84\xd0\xcf\x1f" \
  "\x7b\xee\xad\x17\x13\xe2\xe0\x95\x98\x97\x34\x7f\x67\xc8\x0b\x04" \
  "\x00\xc2\x09\x81\x5d\x6b\x10\xa6\x83\x83\x6f\xd5\x56\x2a\x56\xca" \
  "\xb1\xa2\x8e\x81\xb6\x57\x66\x54\x63\x1c\xf1\x65\x66\xb8\x6e\x3b" \
  "\x33\xa1\x08\xb0\x53\x07\xc0\x0a\xff\x14\xa7\x68\xed\x73\x50\x60" \
  "\x6a\x0f\x85\xe6\xa9\x1d\x39\x6f\x5b\x5c\xbe\x57\x7f\x9b\x38\x80" \
  "\x7c\x7d\x52\x3d\x6d\x79\x2f\x6e\xbc\x24\xa4\xec\xf2\xb3\xa4\x27" \
  "\xcd\xbb\xfb"
#define TEST7_224 \
  "\xf0\x70\x06\xf2\x5a\x0b\xea\x68\xcd\x76\xa2\x95\x87\xc2\x8d"
#define TEST8_224 \
  "\x18\x80\x40\x05\xdd\x4f\xbd\x15\x56\x29\x9d\x6f\x9d\x93\xdf\x62"
#define TEST9_224 \
  "\xa2\xbe\x6e\x46\x32\x81\x09\x02\x94\xd9\xce\x94\x82\x65\x69\x42" \
  "\x3a\x3a\x30\x5e\xd5\xe2\x11\x6c\xd4\xa4\xc9\x87\xfc\x06\x57\x00" \
  "\x64\x91\xb1\x49\xcc\xd4\xb5\x11\x30\xac\x62\xb1\x9d\xc2\x48\xc7" \
  "\x44\x54\x3d\x20\xcd\x39\x52\xdc\xed\x1f\x06\xcc\x3b\x18\xb9\x1f" \
  "\x3f\x55\x63\x3e\xcc\x30\x85\xf4\x90\x70\x60\xd2"
#define TEST10_224 \
  "\x55\xb2\x10\x07\x9c\x61\xb5\x3a\xdd\x52\x06\x22\xd1\xac\x97\xd5" \
  "\xcd\xbe\x8c\xb3\x3a\xa0\xae\x34\x45\x17\xbe\xe4\xd7\xba\x09\xab" \
  "\xc8\x53\x3c\x52\x50\x88\x7a\x43\xbe\xbb\xac\x90\x6c\x2e\x18\x37" \
  "\xf2\x6b\x36\xa5\x9a\xe3\xbe\x78\x14\xd5\x06\x89\x6b\x71\x8b\x2a" \
  "\x38\x3e\xcd\xac\x16\xb9\x61\x25\x55\x3f\x41\x6f\xf3\x2c\x66\x74" \
  "\xc7\x45\x99\xa9\x00\x53\x86\xd9\xce\x11\x12\x24\x5f\x48\xee\x47" \
  "\x0d\x39\x6c\x1e\xd6\x3b\x92\x67\x0c\xa5\x6e\xc8\x4d\xee\xa8\x14" \
  "\xb6\x13\x5e\xca\x54\x39\x2b\xde\xdb\x94\x89\xbc\x9b\x87\x5a\x8b" \
  "\xaf\x0d\xc1\xae\x78\x57\x36\x91\x4a\xb7\xda\xa2\x64\xbc\x07\x9d" \
  "\x26\x9f\x2c\x0d\x7e\xdd\xd8\x10\xa4\x26\x14\x5a\x07\x76\xf6\x7c" \
  "\x87\x82\x73"
#define TEST7_256 \
  "\xbe\x27\x46\xc6\xdb\x52\x76\x5f\xdb\x2f\x88\x70\x0f\x9a\x73"
#define TEST8_256 \
  "\xe3\xd7\x25\x70\xdc\xdd\x78\x7c\xe3\x88\x7a\xb2\xcd\x68\x46\x52"
#define TEST9_256 \
  "\x3e\x74\x03\x71\xc8\x10\xc2\xb9\x9f\xc0\x4e\x80\x49\x07\xef\x7c" \
  "\xf2\x6b\xe2\x8b\x57\xcb\x58\xa3\xe2\xf3\xc0\x07\x16\x6e\x49\xc1" \
  "\x2e\x9b\xa3\x4c\x01\x04\x06\x91\x29\xea\x76\x15\x64\x25\x45\x70" \
  "\x3a\x2b\xd9\x01\xe1\x6e\xb0\xe0\x5d\xeb\xa0\x14\xeb\xff\x64\x06" \
  "\xa0\x7d\x54\x36\x4e\xff\x74\x2d\xa7\x79\xb0\xb3"
#define TEST10_256 \
  "\x83\x26\x75\x4e\x22\x77\x37\x2f\x4f\xc1\x2b\x20\x52\x7a\xfe\xf0" \
  "\x4d\x8a\x05\x69\x71\xb1\x1a\xd5\x71\x23\xa7\xc1\x37\x76\x00\x00" \
  "\xd7\xbe\xf6\xf3\xc1\xf7\xa9\x08\x3a\xa3\x9d\x81\x0d\xb3\x10\x77" \
  "\x7d\xab\x8b\x1e\x7f\x02\xb8\x4a\x26\xc7\x73\x32\x5f\x8b\x23\x74" \
  "\xde\x7a\x4b\x5a\x58\xcb\x5c\x5c\xf3\x5b\xce\xe6\xfb\x94\x6e\x5b" \
  "\xd6\x94\xfa\x59\x3a\x8b\xeb\x3f\x9d\x65\x92\xec\xed\xaa\x66\xca" \
  "\x82\xa2\x9d\x0c\x51\xbc\xf9\x33\x62\x30\xe5\xd7\x84\xe4\xc0\xa4" \
  "\x3f\x8d\x79\xa3\x0a\x16\x5c\xba\xbe\x45\x2b\x77\x4b\x9c\x71\x09" \
  "\xa9\x7d\x13\x8f\x12\x92\x28\x96\x6f\x6c\x0a\xdc\x10\x6a\xad\x5a" \
  "\x9f\xdd\x30\x82\x57\x69\xb2\xc6\x71\xaf\x67\x59\xdf\x28\xeb\x39" \
  "\x3d\x54\xd6"
#define TEST7_384 \
  "\x8b\xc5\x00\xc7\x7c\xee\xd9\x87\x9d\xa9\x89\x10\x7c\xe0\xaa"
#define TEST8_384 \
  "\xa4\x1c\x49\x77\x79\xc0\x37\x5f\xf1\x0a\x7f\x4e\x08\x59\x17\x39"
#define TEST9_384 \
  "\x68\xf5\x01\x79\x2d\xea\x97\x96\x76\x70\x22\xd9\x3d\xa7\x16\x79" \
  "\x30\x99\x20\xfa\x10\x12\xae\xa3\x57\xb2\xb1\x33\x1d\x40\xa1\xd0" \
  "\x3c\x41\xc2\x40\xb3\xc9\xa7\x5b\x48\x92\xf4\xc0\x72\x4b\x68\xc8" \
  "\x75\x32\x1a\xb8\xcf\xe5\x02\x3b\xd3\x75\xbc\x0f\x94\xbd\x89\xfe" \
  "\x04\xf2\x97\x10\x5d\x7b\x82\xff\xc0\x02\x1a\xeb\x1c\xcb\x67\x4f" \
  "\x52\x44\xea\x34\x97\xde\x26\xa4\x19\x1c\x5f\x62\xe5\xe9\xa2\xd8" \
  "\x08\x2f\x05\x51\xf4\xa5\x30\x68\x26\xe9\x1c\xc0\x06\xce\x1b\xf6" \
  "\x0f\xf7\x19\xd4\x2f\xa5\x21\xc8\x71\xcd\x23\x94\xd9\x6e\xf4\x46" \
  "\x8f\x21\x96\x6b\x41\xf2\xba\x80\xc2\x6e\x83\xa9"
#define TEST10_384 \
  "\x39\x96\x69\xe2\x8f\x6b\x9c\x6d\xbc\xbb\x69\x12\xec\x10\xff\xcf" \
  "\x74\x79\x03\x49\xb7\xdc\x8f\xbe\x4a\x8e\x7b\x3b\x56\x21\xdb\x0f" \
  "\x3e\x7d\xc8\x7f\x82\x32\x64\xbb\xe4\x0d\x18\x11\xc9\xea\x20\x61" \
  "\xe1\xc8\x4a\xd1\x0a\x23\xfa\xc1\x72\x7e\x72\x02\xfc\x3f\x50\x42" \
  "\xe6\xbf\x58\xcb\xa8\xa2\x74\x6e\x1f\x64\xf9\xb9\xea\x35\x2c\x71" \
  "\x15\x07\x05\x3c\xf4\xe5\x33\x9d\x52\x86\x5f\x25\xcc\x22\xb5\xe8" \
  "\x77\x84\xa1\x2f\xc9\x61\xd6\x6c\xb6\xe8\x95\x73\x19\x9a\x2c\xe6" \
  "\x56\x5c\xbd\xf1\x3d\xca\x40\x38\x32\xcf\xcb\x0e\x8b\x72\x11\xe8" \
  "\x3a\xf3\x2a\x11\xac\x17\x92\x9f\xf1\xc0\x73\xa5\x1c\xc0\x27\xaa" \
  "\xed\xef\xf8\x5a\xad\x7c\x2b\x7c\x5a\x80\x3e\x24\x04\xd9\x6d\x2a" \
  "\x77\x35\x7b\xda\x1a\x6d\xae\xed\x17\x15\x1c\xb9\xbc\x51\x25\xa4" \
  "\x22\xe9\x41\xde\x0c\xa0\xfc\x50\x11\xc2\x3e\xcf\xfe\xfd\xd0\x96" \
  "\x76\x71\x1c\xf3\xdb\x0a\x34\x40\x72\x0e\x16\x15\xc1\xf2\x2f\xbc" \
  "\x3c\x72\x1d\xe5\x21\xe1\xb9\x9b\xa1\xbd\x55\x77\x40\x86\x42\x14" \
  "\x7e\xd0\x96"
#define TEST7_512 \
  "\x08\xec\xb5\x2e\xba\xe1\xf7\x42\x2d\xb6\x2b\xcd\x54\x26\x70"
#define TEST8_512 \
  "\x8d\x4e\x3c\x0e\x38\x89\x19\x14\x91\x81\x6e\x9d\x98\xbf\xf0\xa0"
#define TEST9_512 \
  "\x3a\xdd\xec\x85\x59\x32\x16\xd1\x61\x9a\xa0\x2d\x97\x56\x97\x0b" \
  "\xfc\x70\xac\xe2\x74\x4f\x7c\x6b\x27\x88\x15\x10\x28\xf7\xb6\xa2" \
  "\x55\x0f\xd7\x4a\x7e\x6e\x69\xc2\xc9\xb4\x5f\xc4\x54\x96\x6d\xc3" \
  "\x1d\x2e\x10\xda\x1f\x95\xce\x02\xbe\xb4\xbf\x87\x65\x57\x4c\xbd" \
  "\x6e\x83\x37\xef\x42\x0a\xdc\x98\xc1\x5c\xb6\xd5\xe4\xa0\x24\x1b" \
  "\xa0\x04\x6d\x25\x0e\x51\x02\x31\xca\xc2\x04\x6c\x99\x16\x06\xab" \
  "\x4e\xe4\x14\x5b\xee\x2f\xf4\xbb\x12\x3a\xab\x49\x8d\x9d\x44\x79" \
  "\x4f\x99\xcc\xad\x89\xa9\xa1\x62\x12\x59\xed\xa7\x0a\x5b\x6d\xd4" \
  "\xbd\xd8\x77\x78\xc9\x04\x3b\x93\x84\xf5\x49\x06"
#define TEST10_512 \
  "\xa5\x5f\x20\xc4\x11\xaa\xd1\x32\x80\x7a\x50\x2d\x65\x82\x4e\x31" \
  "\xa2\x30\x54\x32\xaa\x3d\x06\xd3\xe2\x82\xa8\xd8\x4e\x0d\xe1\xde" \
  "\x69\x74\xbf\x49\x54\x69\xfc\x7f\x33\x8f\x80\x54\xd5\x8c\x26\xc4" \
  "\x93\x60\xc3\xe8\x7a\xf5\x65\x23\xac\xf6\xd8\x9d\x03\xe5\x6f\xf2" \
  "\xf8\x68\x00\x2b\xc3\xe4\x31\xed\xc4\x4d\xf2\xf0\x22\x3d\x4b\xb3" \
  "\xb2\x43\x58\x6e\x1a\x7d\x92\x49\x36\x69\x4f\xcb\xba\xf8\x8d\x95" \
  "\x19\xe4\xeb\x50\xa6\x44\xf8\xe4\xf9\x5e\xb0\xea\x95\xbc\x44\x65" \
  "\xc8\x82\x1a\xac\xd2\xfe\x15\xab\x49\x81\x16\x4b\xbb\x6d\xc3\x2f" \
  "\x96\x90\x87\xa1\x45\xb0\xd9\xcc\x9c\x67\xc2\x2b\x76\x32\x99\x41" \
  "\x9c\xc4\x12\x8b\xe9\xa0\x77\xb3\xac\xe6\x34\x06\x4e\x6d\x99\x28" \
  "\x35\x13\xdc\x06\xe7\x51\x5d\x0d\x73\x13\x2e\x9a\x0d\xc6\xd3\xb1" \
  "\xf8\xb2\x46\xf1\xa9\x8a\x3f\xc7\x29\x41\xb1\xe3\xbb\x20\x98\xe8" \
  "\xbf\x16\xf2\x68\xd6\x4f\x0b\x0f\x47\x07\xfe\x1e\xa1\xa1\x79\x1b" \
  "\xa2\xf3\xc0\xc7\x58\xe5\xf5\x51\x86\x3a\x96\xc9\x49\xad\x47\xd7" \
  "\xfb\x40\xd2"
#define SHA1_SEED "\xd0\x56\x9c\xb3\x66\x5a\x8a\x43\xeb\x6e\xa2\x3d" \
  "\x75\xa3\xc4\xd2\x05\x4a\x0d\x7d"
#define SHA224_SEED "\xd0\x56\x9c\xb3\x66\x5a\x8a\x43\xeb\x6e\xa2" \
  "\x3d\x75\xa3\xc4\xd2\x05\x4a\x0d\x7d\x66\xa9\xca\x99\xc9\xce\xb0" \
  "\x27"
#define SHA256_SEED "\xf4\x1e\xce\x26\x13\xe4\x57\x39\x15\x69\x6b" \
  "\x5a\xdc\xd5\x1c\xa3\x28\xbe\x3b\xf5\x66\xa9\xca\x99\xc9\xce\xb0" \
  "\x27\x9c\x1c\xb0\xa7"
#define SHA384_SEED "\x82\x40\xbc\x51\xe4\xec\x7e\xf7\x6d\x18\xe3" \
  "\x52\x04\xa1\x9f\x51\xa5\x21\x3a\x73\xa8\x1d\x6f\x94\x46\x80\xd3" \
  "\x07\x59\x48\xb7\xe4\x63\x80\x4e\xa3\xd2\x6e\x13\xea\x82\x0d\x65" \
  "\xa4\x84\xbe\x74\x53"
#define SHA512_SEED "\x47\x3f\xf1\xb9\xb3\xff\xdf\xa1\x26\x69\x9a" \
  "\xc7\xef\x9e\x8e\x78\x77\x73\x09\x58\x24\xc6\x42\x55\x7c\x13\x99" \
  "\xd9\x8e\x42\x20\x44\x8d\xc3\x5b\x99\xbf\xdd\x44\x77\x95\x43\x92" \
  "\x4c\x1c\xe9\x3b\xc5\x94\x15\x38\x89\x5d\xb9\x88\x26\x1b\x00\x77" \
  "\x4b\x12\x27\x20\x39"

#define TESTCOUNT 10
#define HASHCOUNT 5
#define RANDOMCOUNT 4
#define HMACTESTCOUNT 7
#define HKDFTESTCOUNT 7

#define PRINTNONE 0
#define PRINTTEXT 1
#define PRINTRAW 2
#define PRINTHEX 3
#define PRINTBASE64 4

#define PRINTPASSFAIL 1
#define PRINTFAIL 2

#define length(x) (sizeof(x)-1)

/* Test arrays for hashes. */
struct hash {
    const char *name;
    SHAversion whichSha;
    int hashsize;
    struct {
        const char *testarray;
        int length;
        long repeatcount;
        int extrabits;
        int numberExtrabits;
        const char *resultarray;
    } tests[TESTCOUNT];
    const char *randomtest;
    const char *randomresults[RANDOMCOUNT];
} hashes[HASHCOUNT] = {
  { "SHA1", SHA1, SHA1HashSize,
    {
      /* 1 */ { TEST1, length(TEST1), 1, 0, 0,
        "A9993E364706816ABA3E25717850C26C9CD0D89D" },
      /* 2 */ { TEST2_1, length(TEST2_1), 1, 0, 0,
        "84983E441C3BD26EBAAE4AA1F95129E5E54670F1" },
      /* 3 */ { TEST3, length(TEST3), 1000000, 0, 0,
        "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F" },
      /* 4 */ { TEST4, length(TEST4), 10, 0, 0,
        "DEA356A2CDDD90C7A7ECEDC5EBB563934F460452" },
      /* 5 */ { "", 0, 0, 0x98, 5,
        "29826B003B906E660EFF4027CE98AF3531AC75BA" },
      /* 6 */ { "\x5e", 1, 1, 0, 0,
        "5E6F80A34A9798CAFC6A5DB96CC57BA4C4DB59C2" },
      /* 7 */ { TEST7_1, length(TEST7_1), 1, 0x80, 3,
        "6239781E03729919C01955B3FFA8ACB60B988340" },
      /* 8 */ { TEST8_1, length(TEST8_1), 1, 0, 0,
        "82ABFF6605DBE1C17DEF12A394FA22A82B544A35" },
      /* 9 */ { TEST9_1, length(TEST9_1), 1, 0xE0, 3,
        "8C5B2A5DDAE5A97FC7F9D85661C672ADBF7933D4" },
      /* 10 */ { TEST10_1, length(TEST10_1), 1, 0, 0,
        "CB0082C8F197D260991BA6A460E76E202BAD27B3" }
    }, SHA1_SEED, { "E216836819477C7F78E0D843FE4FF1B6D6C14CD4",
        "A2DBC7A5B1C6C0A8BCB7AAA41252A6A7D0690DBC",
        "DB1F9050BB863DFEF4CE37186044E2EEB17EE013",
        "127FDEDF43D372A51D5747C48FBFFE38EF6CDF7B"
     } },
  { "SHA224", SHA224, SHA224HashSize,
    {
      /* 1 */ { TEST1, length(TEST1), 1, 0, 0,
        "23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7" },
      /* 2 */ { TEST2_1, length(TEST2_1), 1, 0, 0,
        "75388B16512776CC5DBA5DA1FD890150B0C6455CB4F58B1952522525" },
      /* 3 */ { TEST3, length(TEST3), 1000000, 0, 0,
        "20794655980C91D8BBB4C1EA97618A4BF03F42581948B2EE4EE7AD67" },
      /* 4 */ { TEST4, length(TEST4), 10, 0, 0,
        "567F69F168CD7844E65259CE658FE7AADFA25216E68ECA0EB7AB8262" },
      /* 5 */ { "", 0, 0, 0x68, 5,
        "E3B048552C3C387BCAB37F6EB06BB79B96A4AEE5FF27F51531A9551C" },
      /* 6 */ { "\x07", 1, 1, 0, 0,
        "00ECD5F138422B8AD74C9799FD826C531BAD2FCABC7450BEE2AA8C2A" },
      /* 7 */ { TEST7_224, length(TEST7_224), 1, 0xA0, 3,
        "1B01DB6CB4A9E43DED1516BEB3DB0B87B6D1EA43187462C608137150" },
      /* 8 */ { TEST8_224, length(TEST8_224), 1, 0, 0,
        "DF90D78AA78821C99B40BA4C966921ACCD8FFB1E98AC388E56191DB1" },
      /* 9 */ { TEST9_224, length(TEST9_224), 1, 0xE0, 3,
        "54BEA6EAB8195A2EB0A7906A4B4A876666300EEFBD1F3B8474F9CD57" },
      /* 10 */ { TEST10_224, length(TEST10_224), 1, 0, 0,
        "0B31894EC8937AD9B91BDFBCBA294D9ADEFAA18E09305E9F20D5C3A4" }
    }, SHA224_SEED, { "100966A5B4FDE0B42E2A6C5953D4D7F41BA7CF79FD"
        "2DF431416734BE", "1DCA396B0C417715DEFAAE9641E10A2E99D55A"
        "BCB8A00061EB3BE8BD", "1864E627BDB2319973CD5ED7D68DA71D8B"
        "F0F983D8D9AB32C34ADB34", "A2406481FC1BCAF24DD08E6752E844"
        "709563FB916227FED598EB621F"
     } },
  { "SHA256", SHA256, SHA256HashSize,
  {
      /* 1 */ { TEST1, length(TEST1), 1, 0, 0, "BA7816BF8F01CFEA4141"
        "40DE5DAE2223B00361A396177A9CB410FF61F20015AD" },
      /* 2 */ { TEST2_1, length(TEST2_1), 1, 0, 0, "248D6A61D20638B8"
        "E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1" },
      /* 3 */ { TEST3, length(TEST3), 1000000, 0, 0, "CDC76E5C9914FB92"
        "81A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0" },
      /* 4 */ { TEST4, length(TEST4), 10, 0, 0, "594847328451BDFA"
        "85056225462CC1D867D877FB388DF0CE35F25AB5562BFBB5" },
      /* 5 */ { "", 0, 0, 0x68, 5, "D6D3E02A31A84A8CAA9718ED6C2057BE"
        "09DB45E7823EB5079CE7A573A3760F95" },
      /* 6 */ { "\x19", 1, 1, 0, 0, "68AA2E2EE5DFF96E3355E6C7EE373E3D"
        "6A4E17F75F9518D843709C0C9BC3E3D4" },
      /* 7 */ { TEST7_256, length(TEST7_256), 1, 0x60, 3, "77EC1DC8"
        "9C821FF2A1279089FA091B35B8CD960BCAF7DE01C6A7680756BEB972" },
      /* 8 */ { TEST8_256, length(TEST8_256), 1, 0, 0, "175EE69B02BA"
        "9B58E2B0A5FD13819CEA573F3940A94F825128CF4209BEABB4E8" },
      /* 9 */ { TEST9_256, length(TEST9_256), 1, 0xA0, 3, "3E9AD646"
        "8BBBAD2AC3C2CDC292E018BA5FD70B960CF1679777FCE708FDB066E9" },
      /* 10 */ { TEST10_256, length(TEST10_256), 1, 0, 0, "97DBCA7D"
        "F46D62C8A422C941DD7E835B8AD3361763F7E9B2D95F4F0DA6E1CCBC" },
    }, SHA256_SEED, { "83D28614D49C3ADC1D6FC05DB5F48037C056F8D2A4CE44"
        "EC6457DEA5DD797CD1", "99DBE3127EF2E93DD9322D6A07909EB33B6399"
        "5E529B3F954B8581621BB74D39", "8D4BE295BB64661CA3C7EFD129A2F7"
        "25B33072DBDDE32385B9A87B9AF88EA76F", "40AF5D3F9716B040DF9408"
        "E31536B70FF906EC51B00447CA97D7DD97C12411F4"
    } },
  { "SHA384", SHA384, SHA384HashSize,
    {
      /* 1 */ { TEST1, length(TEST1), 1, 0, 0,
        "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED163"
        "1A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7" },
      /* 2 */ { TEST2_2, length(TEST2_2), 1, 0, 0,
        "09330C33F71147E83D192FC782CD1B4753111B173B3B05D2"
        "2FA08086E3B0F712FCC7C71A557E2DB966C3E9FA91746039" },
      /* 3 */ { TEST3, length(TEST3), 1000000, 0, 0,
        "9D0E1809716474CB086E834E310A4A1CED149E9C00F24852"
        "7972CEC5704C2A5B07B8B3DC38ECC4EBAE97DDD87F3D8985" },
      /* 4 */ { TEST4, length(TEST4), 10, 0, 0,
        "2FC64A4F500DDB6828F6A3430B8DD72A368EB7F3A8322A70"
        "BC84275B9C0B3AB00D27A5CC3C2D224AA6B61A0D79FB4596" },
      /* 5 */ { "", 0, 0, 0x10, 5,
        "8D17BE79E32B6718E07D8A603EB84BA0478F7FCFD1BB9399"
        "5F7D1149E09143AC1FFCFC56820E469F3878D957A15A3FE4" },
      /* 6 */ { "\xb9", 1, 1, 0, 0,
        "BC8089A19007C0B14195F4ECC74094FEC64F01F90929282C"
        "2FB392881578208AD466828B1C6C283D2722CF0AD1AB6938" },
      /* 7 */ { TEST7_384, length(TEST7_384), 1, 0xA0, 3,
        "D8C43B38E12E7C42A7C9B810299FD6A770BEF30920F17532"
        "A898DE62C7A07E4293449C0B5FA70109F0783211CFC4BCE3" },
      /* 8 */ { TEST8_384, length(TEST8_384), 1, 0, 0,
        "C9A68443A005812256B8EC76B00516F0DBB74FAB26D66591"
        "3F194B6FFB0E91EA9967566B58109CBC675CC208E4C823F7" },
      /* 9 */ { TEST9_384, length(TEST9_384), 1, 0xE0, 3,
        "5860E8DE91C21578BB4174D227898A98E0B45C4C760F0095"
        "49495614DAEDC0775D92D11D9F8CE9B064EEAC8DAFC3A297" },
      /* 10 */ { TEST10_384, length(TEST10_384), 1, 0, 0,
        "4F440DB1E6EDD2899FA335F09515AA025EE177A79F4B4AAF"
        "38E42B5C4DE660F5DE8FB2A5B2FBD2A3CBFFD20CFF1288C0" }
    }, SHA384_SEED, { "CE44D7D63AE0C91482998CF662A51EC80BF6FC68661A3C"
        "57F87566112BD635A743EA904DEB7D7A42AC808CABE697F38F", "F9C6D2"
        "61881FEE41ACD39E67AA8D0BAD507C7363EB67E2B81F45759F9C0FD7B503"
        "DF1A0B9E80BDE7BC333D75B804197D", "D96512D8C9F4A7A4967A366C01"
        "C6FD97384225B58343A88264847C18E4EF8AB7AEE4765FFBC3E30BD485D3"
        "638A01418F", "0CA76BD0813AF1509E170907A96005938BC985628290B2"
        "5FEF73CF6FAD68DDBA0AC8920C94E0541607B0915A7B4457F7"
    } },
  { "SHA512", SHA512, SHA512HashSize,
    {
      /* 1 */ { TEST1, length(TEST1), 1, 0, 0,
        "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA2"
        "0A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD"
        "454D4423643CE80E2A9AC94FA54CA49F" },
      /* 2 */ { TEST2_2, length(TEST2_2), 1, 0, 0,
        "8E959B75DAE313DA8CF4F72814FC143F8F7779C6EB9F7FA1"
        "7299AEADB6889018501D289E4900F7E4331B99DEC4B5433A"
        "C7D329EEB6DD26545E96E55B874BE909" },
       /* 3 */ { TEST3, length(TEST3), 1000000, 0, 0,
        "E718483D0CE769644E2E42C7BC15B4638E1F98B13B204428"
        "5632A803AFA973EBDE0FF244877EA60A4CB0432CE577C31B"
        "EB009C5C2C49AA2E4EADB217AD8CC09B" },
      /* 4 */ { TEST4, length(TEST4), 10, 0, 0,
        "89D05BA632C699C31231DED4FFC127D5A894DAD412C0E024"
        "DB872D1ABD2BA8141A0F85072A9BE1E2AA04CF33C765CB51"
        "0813A39CD5A84C4ACAA64D3F3FB7BAE9" },
      /* 5 */ { "", 0, 0, 0xB0, 5,
        "D4EE29A9E90985446B913CF1D1376C836F4BE2C1CF3CADA0"
        "720A6BF4857D886A7ECB3C4E4C0FA8C7F95214E41DC1B0D2"
        "1B22A84CC03BF8CE4845F34DD5BDBAD4" },
      /* 6 */ { "\xD0", 1, 1, 0, 0,
        "9992202938E882E73E20F6B69E68A0A7149090423D93C81B"
        "AB3F21678D4ACEEEE50E4E8CAFADA4C85A54EA8306826C4A"
        "D6E74CECE9631BFA8A549B4AB3FBBA15" },
      /* 7 */ { TEST7_512, length(TEST7_512), 1, 0x80, 3,
        "ED8DC78E8B01B69750053DBB7A0A9EDA0FB9E9D292B1ED71"
        "5E80A7FE290A4E16664FD913E85854400C5AF05E6DAD316B"
        "7359B43E64F8BEC3C1F237119986BBB6" },
      /* 8 */ { TEST8_512, length(TEST8_512), 1, 0, 0,
        "CB0B67A4B8712CD73C9AABC0B199E9269B20844AFB75ACBD"
        "D1C153C9828924C3DDEDAAFE669C5FDD0BC66F630F677398"
        "8213EB1B16F517AD0DE4B2F0C95C90F8" },
      /* 9 */ { TEST9_512, length(TEST9_512), 1, 0x80, 3,
        "32BA76FC30EAA0208AEB50FFB5AF1864FDBF17902A4DC0A6"
        "82C61FCEA6D92B783267B21080301837F59DE79C6B337DB2"
        "526F8A0A510E5E53CAFED4355FE7C2F1" },
      /* 10 */ { TEST10_512, length(TEST10_512), 1, 0, 0,
        "C665BEFB36DA189D78822D10528CBF3B12B3EEF726039909"
        "C1A16A270D48719377966B957A878E720584779A62825C18"
        "DA26415E49A7176A894E7510FD1451F5" }
    }, SHA512_SEED, { "2FBB1E7E00F746BA514FBC8C421F36792EC0E11FF5EFC3"
        "78E1AB0C079AA5F0F66A1E3EDBAEB4F9984BE14437123038A452004A5576"
        "8C1FD8EED49E4A21BEDCD0", "25CBE5A4F2C7B1D7EF07011705D50C62C5"
        "000594243EAFD1241FC9F3D22B58184AE2FEE38E171CF8129E29459C9BC2"
        "EF461AF5708887315F15419D8D17FE7949", "5B8B1F2687555CE2D7182B"
        "92E5C3F6C36547DA1C13DBB9EA4F73EA4CBBAF89411527906D35B1B06C1B"
        "6A8007D05EC66DF0A406066829EAB618BDE3976515AAFC", "46E36B007D"
        "19876CDB0B29AD074FE3C08CDD174D42169D6ABE5A1414B6E79707DF5877"
        "6A98091CF431854147BB6D3C66D43BFBC108FD715BDE6AA127C2B0E79F"
    }
  }
};

/* Test arrays for HMAC. */
struct hmachash {
    const char *keyarray[5];
    int keylength[5];
    const char *dataarray[5];
    int datalength[5];
    const char *resultarray[5];
    int resultlength[5];
} hmachashes[HMACTESTCOUNT] = {
  { /* 1 */ {
      "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
      "\x0b\x0b\x0b\x0b\x0b"
    }, { 20 }, {
      "\x48\x69\x20\x54\x68\x65\x72\x65" /* "Hi There" */
    }, { 8 }, {
      /* HMAC-SHA-1 */
      "B617318655057264E28BC0B6FB378C8EF146BE00",
      /* HMAC-SHA-224 */
      "896FB1128ABBDF196832107CD49DF33F47B4B1169912BA4F53684B22",
      /* HMAC-SHA-256 */
      "B0344C61D8DB38535CA8AFCEAF0BF12B881DC200C9833DA726E9376C2E32"
      "CFF7",
      /* HMAC-SHA-384 */
      "AFD03944D84895626B0825F4AB46907F15F9DADBE4101EC682AA034C7CEB"
      "C59CFAEA9EA9076EDE7F4AF152E8B2FA9CB6",
      /* HMAC-SHA-512 */
      "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E1"
      "7CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C20"
      "3A126854"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  { /* 2 */ {
      "\x4a\x65\x66\x65" /* "Jefe" */
    }, { 4 }, {
      "\x77\x68\x61\x74\x20\x64\x6f\x20\x79\x61\x20\x77\x61\x6e\x74"
      "\x20\x66\x6f\x72\x20\x6e\x6f\x74\x68\x69\x6e\x67\x3f"
      /* "what do ya want for nothing?" */
    }, { 28 }, {
      /* HMAC-SHA-1 */
      "EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79",
      /* HMAC-SHA-224 */
      "A30E01098BC6DBBF45690F3A7E9E6D0F8BBEA2A39E6148008FD05E44",
      /* HMAC-SHA-256 */
      "5BDCC146BF60754E6A042426089575C75A003F089D2739839DEC58B964EC"
      "3843",
      /* HMAC-SHA-384 */
      "AF45D2E376484031617F78D2B58A6B1B9C7EF464F5A01B47E42EC3736322"
      "445E8E2240CA5E69E2C78B3239ECFAB21649",
      /* HMAC-SHA-512 */
      "164B7A7BFCF819E2E395FBE73B56E0A387BD64222E831FD610270CD7EA25"
      "05549758BF75C05A994A6D034F65F8F0E6FDCAEAB1A34D4A6B4B636E070A"
      "38BCE737"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  { /* 3 */
    {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa"
    }, { 20 }, {
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd"
    }, { 50 }, {
      /* HMAC-SHA-1 */
      "125D7342B9AC11CD91A39AF48AA17B4F63F175D3",
      /* HMAC-SHA-224 */
      "7FB3CB3588C6C1F6FFA9694D7D6AD2649365B0C1F65D69D1EC8333EA",
      /* HMAC-SHA-256 */
      "773EA91E36800E46854DB8EBD09181A72959098B3EF8C122D9635514CED5"
      "65FE",
      /* HMAC-SHA-384 */
      "88062608D3E6AD8A0AA2ACE014C8A86F0AA635D947AC9FEBE83EF4E55966"
      "144B2A5AB39DC13814B94E3AB6E101A34F27",
      /* HMAC-SHA-512 */
      "FA73B0089D56A284EFB0F0756C890BE9B1B5DBDD8EE81A3655F83E33B227"
      "9D39BF3E848279A722C806B485A47E67C807B946A337BEE8942674278859"
      "E13292FB"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  { /* 4 */ {
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19"
    }, { 25 }, {
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd"
    }, { 50 }, {
      /* HMAC-SHA-1 */
      "4C9007F4026250C6BC8414F9BF50C86C2D7235DA",
      /* HMAC-SHA-224 */
      "6C11506874013CAC6A2ABC1BB382627CEC6A90D86EFC012DE7AFEC5A",
      /* HMAC-SHA-256 */
      "82558A389A443C0EA4CC819899F2083A85F0FAA3E578F8077A2E3FF46729"
      "665B",
      /* HMAC-SHA-384 */
      "3E8A69B7783C25851933AB6290AF6CA77A9981480850009CC5577C6E1F57"
      "3B4E6801DD23C4A7D679CCF8A386C674CFFB",
      /* HMAC-SHA-512 */
      "B0BA465637458C6990E5A8C5F61D4AF7E576D97FF94B872DE76F8050361E"
      "E3DBA91CA5C11AA25EB4D679275CC5788063A5F19741120C4F2DE2ADEBEB"
      "10A298DD"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  { /* 5 */ {
      "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c"
      "\x0c\x0c\x0c\x0c\x0c"
    }, { 20 }, {
      "Test With Truncation"
    }, { 20 }, {
      /* HMAC-SHA-1 */
      "4C1A03424B55E07FE7F27BE1",
      /* HMAC-SHA-224 */
      "0E2AEA68A90C8D37C988BCDB9FCA6FA8",
      /* HMAC-SHA-256 */
      "A3B6167473100EE06E0C796C2955552B",
      /* HMAC-SHA-384 */
      "3ABF34C3503B2A23A46EFC619BAEF897",
      /* HMAC-SHA-512 */
      "415FAD6271580A531D4179BC891D87A6"
    }, { 12, 16, 16, 16, 16 }
  },
  { /* 6 */ {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    }, { 80, 131 }, {
      "Test Using Larger Than Block-Size Key - Hash Key First"
    }, { 54 }, {
      /* HMAC-SHA-1 */
      "AA4AE5E15272D00E95705637CE8A3B55ED402112",
      /* HMAC-SHA-224 */
      "95E9A0DB962095ADAEBE9B2D6F0DBCE2D499F112F2D2B7273FA6870E",
      /* HMAC-SHA-256 */
      "60E431591EE0B67F0D8A26AACBF5B77F8E0BC6213728C5140546040F0EE3"
      "7F54",
      /* HMAC-SHA-384 */
      "4ECE084485813E9088D2C63A041BC5B44F9EF1012A2B588F3CD11F05033A"
      "C4C60C2EF6AB4030FE8296248DF163F44952",
      /* HMAC-SHA-512 */
      "80B24263C7C1A3EBB71493C1DD7BE8B49B46D1F41B4AEEC1121B013783F8"
      "F3526B56D037E05F2598BD0FD2215D6A1E5295E64F73F63F0AEC8B915A98"
      "5D786598"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  { /* 7 */ {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    }, { 80, 131 }, {
      "Test Using Larger Than Block-Size Key and "
      "Larger Than One Block-Size Data",
      "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
      "\x75\x73\x69\x6e\x67\x20\x61\x20\x6c\x61\x72\x67\x65\x72\x20"
      "\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73\x69\x7a\x65"
      "\x20\x6b\x65\x79\x20\x61\x6e\x64\x20\x61\x20\x6c\x61\x72\x67"
      "\x65\x72\x20\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73"
      "\x69\x7a\x65\x20\x64\x61\x74\x61\x2e\x20\x54\x68\x65\x20\x6b"
      "\x65\x79\x20\x6e\x65\x65\x64\x73\x20\x74\x6f\x20\x62\x65\x20"
      "\x68\x61\x73\x68\x65\x64\x20\x62\x65\x66\x6f\x72\x65\x20\x62"
      "\x65\x69\x6e\x67\x20\x75\x73\x65\x64\x20\x62\x79\x20\x74\x68"
      "\x65\x20\x48\x4d\x41\x43\x20\x61\x6c\x67\x6f\x72\x69\x74\x68"
      "\x6d\x2e"
      /* "This is a test using a larger than block-size key and a "
          "larger than block-size data.  The key needs to be hashed "
          "before being used by the HMAC algorithm." */
    }, { 73, 152 }, {
      /* HMAC-SHA-1 */
      "E8E99D0F45237D786D6BBAA7965C7808BBFF1A91",
      /* HMAC-SHA-224 */
      "3A854166AC5D9F023F54D517D0B39DBD946770DB9C2B95C9F6F565D1",
      /* HMAC-SHA-256 */
      "9B09FFA71B942FCB27635FBCD5B0E944BFDC63644F0713938A7F51535C3A"
      "35E2",
      /* HMAC-SHA-384 */
      "6617178E941F020D351E2F254E8FD32C602420FEB0B8FB9ADCCEBB82461E"
      "99C5A678CC31E799176D3860E6110C46523E",
      /* HMAC-SHA-512 */
      "E37B6A775DC87DBAA4DFA9F96E5E3FFDDEBD71F8867289865DF5A32D20CD"
      "C944B6022CAC3C4982B10D5EEB55C3E4DE15134676FB6DE0446065C97440"
      "FA8C6A58"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  }
};

/* Test arrays for HKDF. */
struct hkdfhash {
    SHAversion whichSha;
    int ikmlength;
    const char *ikmarray;
    int saltlength;
    const char *saltarray;
    int infolength;
    const char *infoarray;
    int prklength;
    const char *prkarray;
    int okmlength;
    const char *okmarray;
} hkdfhashes[HKDFTESTCOUNT] = {
    {   /* RFC 5869 A.1. Test Case 1 */
        SHA256,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        13, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c",
        10, "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9",
        32, "077709362C2E32DF0DDC3F0DC47BBA6390B6C73BB50F9C3122EC844A"
            "D7C2B3E5",
        42, "3CB25F25FAACD57A90434F64D0362F2A2D2D0A90CF1A5A4C5DB02D56"
            "ECC4C5BF34007208D5B887185865"
    },
    {   /* RFC 5869 A.2. Test Case 2 */
        SHA256,
        80, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d"
            "\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b"
            "\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
            "\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37"
            "\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45"
            "\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f",
        80, "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d"
            "\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b"
            "\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89"
            "\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97"
            "\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5"
            "\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf",
        80, "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd"
            "\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb"
            "\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9"
            "\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7"
            "\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5"
            "\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
        32, "06A6B88C5853361A06104C9CEB35B45C"
            "EF760014904671014A193F40C15FC244",
        82, "B11E398DC80327A1C8E7F78C596A4934"
            "4F012EDA2D4EFAD8A050CC4C19AFA97C"
            "59045A99CAC7827271CB41C65E590E09"
            "DA3275600C2F09B8367793A9ACA3DB71"
            "CC30C58179EC3E87C14C01D5C1F3434F"
            "1D87"
    },
    {   /* RFC 5869 A.3. Test Case 3 */
        SHA256,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        0, "",
        0, "",
        32, "19EF24A32C717B167F33A91D6F648BDF"
            "96596776AFDB6377AC434C1C293CCB04",
        42, "8DA4E775A563C18F715F802A063C5A31"
            "B8A11F5C5EE1879EC3454E5F3C738D2D"
            "9D201395FAA4B61A96C8"
    },
    {   /* RFC 5869 A.4. Test Case 4 */
        SHA1,
        11, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        13, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c",
        10, "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9",
        20, "9B6C18C432A7BF8F0E71C8EB88F4B30BAA2BA243",
        42, "085A01EA1B10F36933068B56EFA5AD81"
            "A4F14B822F5B091568A9CDD4F155FDA2"
            "C22E422478D305F3F896"
    },
    {   /* RFC 5869 A.5. Test Case 5 */
        SHA1,
        80, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d"
            "\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b"
            "\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
            "\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37"
            "\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45"
            "\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f",
        80, "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D"
            "\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B"
            "\x7C\x7D\x7E\x7F\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89"
            "\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97"
            "\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\xA0\xA1\xA2\xA3\xA4\xA5"
            "\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF",
        80, "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD"
            "\xBE\xBF\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB"
            "\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9"
            "\xDA\xDB\xDC\xDD\xDE\xDF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7"
            "\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5"
            "\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF",
        20, "8ADAE09A2A307059478D309B26C4115A224CFAF6",
        82, "0BD770A74D1160F7C9F12CD5912A06EB"
            "FF6ADCAE899D92191FE4305673BA2FFE"
            "8FA3F1A4E5AD79F3F334B3B202B2173C"
            "486EA37CE3D397ED034C7F9DFEB15C5E"
            "927336D0441F4C4300E2CFF0D0900B52"
            "D3B4"
    },
    {   /* RFC 5869 A.6. Test Case 6 */
        SHA1,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        0, "",
        0, "",
        20, "DA8C8A73C7FA77288EC6F5E7C297786AA0D32D01",
        42, "0AC1AF7002B3D761D1E55298DA9D0506"
            "B9AE52057220A306E07B6B87E8DF21D0"
            "EA00033DE03984D34918"
    },
    {   /* RFC 5869 A.7. Test Case 7. */
        SHA1,
        22, "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c"
            "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c",
        0, 0,
        0, "",
        20, "2ADCCADA18779E7C2077AD2EB19D3F3E731385DD",
        42, "2C91117204D745F3500D636A62F64F0A"
            "B3BAE548AA53D423B0D1F27EBBA6F5E5"
            "673A081D70CCE7ACFC48"
    }
};

/*
 * Check the hash value against the expected string, expressed in hex
 */
static const char hexdigits[ ] = "0123456789ABCDEF";
int checkmatch(const unsigned char *hashvalue,
  const char *hexstr, int hashsize)
{
  int i;
  for (i = 0; i < hashsize; ++i) {
    if (*hexstr++ != hexdigits[(hashvalue[i] >> 4) & 0xF])
      return 0;
    if (*hexstr++ != hexdigits[hashvalue[i] & 0xF]) return 0;
  }
  return 1;
}

/*
 * Print the string, converting non-printable characters to "."
 */
void printstr(const char *str, int len)
{
  for ( ; len-- > 0; str++)
    putchar(isprint((unsigned char)*str) ? *str : '.');
}

/*
 * Print the string, converting all characters to hex "## ".
 */
void printxstr(const char *str, int len)
{
  char sep[2] = {0};
  for ( ; len-- > 0; str++) {
    printf("%s%c%c", sep, hexdigits[(*str >> 4) & 0xF],
      hexdigits[*str & 0xF]);
    sep[0] = ' ';
  }
}

/*
 * Print a usage message.
 */
void usage(const char *argv0)
{
  fprintf(stderr,
    "Usage:\n"
    "Common options: [-h hash] [-w|-x|-6] [-H]\n"
    "Hash a string:\n"
      "\t%s [-S expectedresult] -s hashstr [-k key] "
           "[-i info -L okm-len]\n"
    "Hash a file:\n"
      "\t%s [-S expectedresult] -f file [-k key] "
           "[-i info -L okm-len]\n"
    "Hash a file, ignoring whitespace:\n"
      "\t%s [-S expectedresult] -F file [-k key] "
           "[-i info -L okm-len]\n"
    "Additional bits to add in: [-B bitcount -b bits]\n"
    "(If -k,-i&-L are used, run HKDF-SHA###.\n"
    " If -k is used, but not -i&-L, run HMAC-SHA###.\n"
    " Otherwise, run SHA###.)\n"
    "Standard tests:\n"
      "\t%s [-m | -d] [-l loopcount] [-t test#] [-e]\n"
      "\t\t[-r randomseed] [-R randomloop-count] "
        "[-p] [-P|-X]\n"
    "-h\thash to test: "
      "0|SHA1, 1|SHA224, 2|SHA256, 3|SHA384, 4|SHA512\n"
    "-m\tperform hmac standard tests\n"
    "-k\tkey for hmac test\n"
    "-d\tperform hkdf standard tests\n"
    "-t\ttest case to run, 1-10\n"
    "-l\thow many times to run the test\n"
    "-e\ttest error returns\n"
    "-p\tdo not print results\n"
    "-P\tdo not print PASSED/FAILED\n"
    "-X\tprint FAILED, but not PASSED\n"
    "-r\tseed for random test\n"
    "-R\thow many times to run random test\n"
    "-s\tstring to hash\n"
    "-S\texpected result of hashed string, in hex\n"
    "-w\toutput hash in raw format\n"
    "-x\toutput hash in hex format\n"
    "-6\toutput hash in base64 format\n"
    "-B\t# extra bits to add in after string or file input\n"
    "-b\textra bits to add (high order bits of #, 0# or 0x#)\n"
    "-H\tinput hashstr or randomseed is in hex\n"
    , argv0, argv0, argv0, argv0);
  exit(1);
}

/*
 * Print the results and PASS/FAIL.
 */
void printResult(uint8_t *Message_Digest, int hashsize,
    const char *hashname, const char *testtype, const char *testname,
    const char *resultarray, int printResults, int printPassFail)
{
  int i, k;
  if (printResults == PRINTTEXT) {
    printf("\nhashsize=%d\n", hashsize);
    putchar('\t');
    for (i = 0; i < hashsize; ++i) {
      putchar(hexdigits[(Message_Digest[i] >> 4) & 0xF]);
      putchar(hexdigits[Message_Digest[i] & 0xF]);
      putchar(' ');
    }
    putchar('\n');
  } else if (printResults == PRINTRAW) {
    fwrite(Message_Digest, 1, hashsize, stdout);
  } else if (printResults == PRINTHEX) {
    for (i = 0; i < hashsize; ++i) {
      putchar(hexdigits[(Message_Digest[i] >> 4) & 0xF]);
      putchar(hexdigits[Message_Digest[i] & 0xF]);
    }
    putchar('\n');
  } else if (printResults == PRINTBASE64) {
    unsigned char b;
    const char *sm = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
               "0123456789+/";
    for (i = 0; i < hashsize; i += 3) {
      putchar(sm[Message_Digest[i] >> 2]);
      b = (Message_Digest[i] & 0x03) << 4;
      if (i+1 < hashsize) b |= Message_Digest[i+1] >> 4;
      putchar(sm[b]);
      if (i+1 < hashsize) {
        b = (Message_Digest[i+1] & 0x0f) << 2;
        if (i+2 < hashsize) b |= Message_Digest[i+2] >> 6;
        putchar(sm[b]);
      } else putchar('=');
      if (i+2 < hashsize) putchar(sm[Message_Digest[i+2] & 0x3f]);
      else putchar('=');
    }
    putchar('\n');
  }

  if (printResults && resultarray) {
    printf("    Should match:\n\t");
    for (i = 0, k = 0; i < hashsize; i++, k += 2) {
      putchar(resultarray[k]);
      putchar(resultarray[k+1]);
      putchar(' ');
    }
    putchar('\n');
  }

  if (printPassFail && resultarray) {
    int ret = checkmatch(Message_Digest, resultarray, hashsize);
    if ((printPassFail == PRINTPASSFAIL) || !ret)
      printf("%s %s %s: %s\n", hashname, testtype, testname,
        ret ? "PASSED" : "FAILED");
  }
}

/*
 * Exercise a hash series of functions.  The input is the testarray,
 * repeated repeatcount times, followed by the extrabits.  If the
 * result is known, it is in resultarray in uppercase hex.
 */
int hash(int testno, int loopno, int hashno,
  const char *testarray, int length, long repeatcount,
  int numberExtrabits, int extrabits, const unsigned char *keyarray,
  int keylen, const unsigned char *info, int infolen, int okmlen,
  const char *resultarray, int hashsize, int printResults,
  int printPassFail)
{
  USHAContext sha;
  HMACContext hmac;
  HKDFContext hkdf;
  int err, i;
  uint8_t Message_Digest_Buf[USHAMaxHashSize];
  uint8_t *Message_Digest = Message_Digest_Buf;
  char buf[20];

  if (printResults == PRINTTEXT) {
    printf("\nTest %d: Iteration %d, Repeat %ld\n\t'", testno+1,
      loopno, repeatcount);
    printstr(testarray, length);
    printf("'\n\t'");
    printxstr(testarray, length);
    printf("'\n");
    printf("    Length=%d bytes (%d bits), ", length, length * 8);
    printf("ExtraBits %d: %2.2x\n", numberExtrabits, extrabits);
  }

  if (info) Message_Digest = malloc(okmlen);
  memset(&sha, '\343', sizeof(sha)); /* force bad data into struct */
  memset(&hmac, '\343', sizeof(hmac));
  memset(&hkdf, '\343', sizeof(hkdf));

  err = info ? hkdfReset(&hkdf, hashes[hashno].whichSha,
                             keyarray, keylen) :
        keyarray ? hmacReset(&hmac, hashes[hashno].whichSha,
                             keyarray, keylen) :
                   USHAReset(&sha, hashes[hashno].whichSha);
  if (err != shaSuccess) {
    fprintf(stderr, "hash(): %sReset Error %d.\n",
            info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  for (i = 0; i < repeatcount; ++i) {
    err = info ? hkdfInput(&hkdf, (const uint8_t *)testarray, length) :
          keyarray ? hmacInput(&hmac, (const uint8_t *) testarray,
                               length) :
                     USHAInput(&sha, (const uint8_t *) testarray,
                               length);
    if (err != shaSuccess) {
      fprintf(stderr, "hash(): %sInput Error %d.\n",
              info ? "hkdf" : keyarray ? "hmac" : "sha", err);
      return err;
    }
  }

  if (numberExtrabits > 0) {
    err = info ? hkdfFinalBits(&hkdf, extrabits, numberExtrabits) :
          keyarray ? hmacFinalBits(&hmac, (uint8_t) extrabits,
                                   numberExtrabits) :
                     USHAFinalBits(&sha, (uint8_t) extrabits,
                                   numberExtrabits);
    if (err != shaSuccess) {
      fprintf(stderr, "hash(): %sFinalBits Error %d.\n",
              info ? "hkdf" : keyarray ? "hmac" : "sha", err);
      return err;
    }
  }

  err = info ? hkdfResult(&hkdf, 0, info, infolen,
                          Message_Digest, okmlen) :
        keyarray ? hmacResult(&hmac, Message_Digest) :
                   USHAResult(&sha, Message_Digest);
  if (err != shaSuccess) {
    fprintf(stderr, "hash(): %s Result Error %d, could not compute "
      "message digest.\n",
      info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  sprintf(buf, "%d", testno+1);
  printResult(Message_Digest, info ? okmlen : hashsize,
    hashes[hashno].name, info ? "hkdf standard test" :
    keyarray ? "hmac standard test" : "sha standard test", buf,
    resultarray, printResults, printPassFail);

  return err;
}

/*
 * Exercise an HKDF series.  The input is the testarray,
 * repeated repeatcount times, followed by the extrabits.  If the
 * result is known, it is in resultarray in uppercase hex.
 */
int hashHkdf(int testno, int loopno, int hashno,
  int printResults, int printPassFail)
{
  int err;
  unsigned char prk[USHAMaxHashSize+1];
  uint8_t okm[255 * USHAMaxHashSize+1];
  char buf[24];

  if (printResults == PRINTTEXT) {
    printf("\nTest %d: Iteration %d\n\tSALT\t'", testno+1, loopno);
    printxstr(hkdfhashes[testno].saltarray,
      hkdfhashes[testno].saltlength);
    printf("'\n\tIKM\t'");
    printxstr(hkdfhashes[testno].ikmarray,
      hkdfhashes[testno].ikmlength);
    printf("'\n\tINFO\t'");
    printxstr(hkdfhashes[testno].infoarray,
      hkdfhashes[testno].infolength);
    printf("'\n");
    printf("    L=%d bytes\n", hkdfhashes[testno].okmlength);
  }

  /* Run hkdf() against the test vectors */
  err = hkdf(hkdfhashes[testno].whichSha,
             (const uint8_t *) hkdfhashes[testno].saltarray,
             hkdfhashes[testno].saltlength,
             (const uint8_t *) hkdfhashes[testno].ikmarray,
             hkdfhashes[testno].ikmlength,
             (const uint8_t *) hkdfhashes[testno].infoarray,
             hkdfhashes[testno].infolength, okm,
             hkdfhashes[testno].okmlength);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdf Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdf %d", testno+1);
  printResult(okm, hkdfhashes[testno].okmlength,
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].okmarray, printResults, printPassFail);

  /* Now run hkdfExtract() by itself against the test vectors */
  /* to verify the intermediate results. */
  err = hkdfExtract(hkdfhashes[testno].whichSha,
                    (const uint8_t *) hkdfhashes[testno].saltarray,
                    hkdfhashes[testno].saltlength,
                    (const uint8_t *) hkdfhashes[testno].ikmarray,
                    hkdfhashes[testno].ikmlength, prk);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdfExtract Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdfExtract %d", testno+1);
  printResult(prk, USHAHashSize(hkdfhashes[testno].whichSha),
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].prkarray, printResults, printPassFail);

  /* Now run hkdfExpand() by itself against the test vectors */
  /* using the intermediate results from hkdfExtract. */
  err = hkdfExpand(hkdfhashes[testno].whichSha, prk,
    USHAHashSize(hkdfhashes[testno].whichSha),
    (const uint8_t *)hkdfhashes[testno].infoarray,
    hkdfhashes[testno].infolength, okm, hkdfhashes[testno].okmlength);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdfExpand Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdfExpand %d", testno+1);
  printResult(okm, hkdfhashes[testno].okmlength,
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].okmarray, printResults, printPassFail);

  return err;
}

/*
 * Exercise a hash series of functions.  The input is a filename.
 * If the result is known, it is in resultarray in uppercase hex.
 */
int hashfile(int hashno, const char *hashfilename, int bits,
  int bitcount, int skipSpaces, const unsigned char *keyarray,
  int keylen, const unsigned char *info, int infolen, int okmlen,
  const char *resultarray, int hashsize,
  int printResults, int printPassFail)
{
  USHAContext sha;
  HMACContext hmac;
  HKDFContext hkdf;
  int err, nread, c;
  unsigned char buf[4096];
  uint8_t Message_Digest_Buf[USHAMaxHashSize];
  uint8_t *Message_Digest = Message_Digest_Buf;
  unsigned char cc;
  FILE *hashfp = (strcmp(hashfilename, "-") == 0) ? stdin :
    fopen(hashfilename, "r");

  if (!hashfp) {
    fprintf(stderr, "cannot open file '%s'\n", hashfilename);
    return shaStateError;
  }

  if (info) Message_Digest = malloc(okmlen);
  memset(&sha, '\343', sizeof(sha)); /* force bad data into struct */
  memset(&hmac, '\343', sizeof(hmac));
  memset(&hkdf, '\343', sizeof(hkdf));
  err = info ? hkdfReset(&hkdf, hashes[hashno].whichSha,
                             keyarray, keylen) :
        keyarray ? hmacReset(&hmac, hashes[hashno].whichSha,
                             keyarray, keylen) :
                   USHAReset(&sha, hashes[hashno].whichSha);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %sReset Error %d.\n",
            info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  if (skipSpaces)
    while ((c = getc(hashfp)) != EOF) {
      if (!isspace(c)) {
        cc = (unsigned char)c;
        err = info ? hkdfInput(&hkdf, &cc, 1) :
              keyarray ? hmacInput(&hmac, &cc, 1) :
                         USHAInput(&sha, &cc, 1);
        if (err != shaSuccess) {
          fprintf(stderr, "hashfile(): %sInput Error %d.\n",
                  info ? "hkdf" : keyarray ? "hmac" : "sha", err);
          if (hashfp != stdin) fclose(hashfp);
          return err;
        }
      }
    }
  else
    while ((nread = fread(buf, 1, sizeof(buf), hashfp)) > 0) {
      err = info ? hkdfInput(&hkdf, buf, nread) :
            keyarray ? hmacInput(&hmac, buf, nread) :
                       USHAInput(&sha, buf, nread);
      if (err != shaSuccess) {
        fprintf(stderr, "hashfile(): %s Error %d.\n",
                info ? "hkdf" : keyarray ? "hmacInput" :
                "shaInput", err);
        if (hashfp != stdin) fclose(hashfp);
        return err;
      }
    }

  if (bitcount > 0)
    err = info ? hkdfFinalBits(&hkdf, bits, bitcount) :
          keyarray ? hmacFinalBits(&hmac, bits, bitcount) :
                   USHAFinalBits(&sha, bits, bitcount);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %s Error %d.\n",
            info ? "hkdf" : keyarray ? "hmacFinalBits" :
            "shaFinalBits", err);
    if (hashfp != stdin) fclose(hashfp);
    return err;
  }

  err = info ? hkdfResult(&hkdf, 0, info, infolen,
                          Message_Digest, okmlen) :
        keyarray ? hmacResult(&hmac, Message_Digest) :
                   USHAResult(&sha, Message_Digest);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %s Error %d.\n",
            info ? "hkdf" : keyarray ? "hmacResult" :
            "shaResult", err);
    if (hashfp != stdin) fclose(hashfp);
    return err;
  }

  printResult(Message_Digest, info ? okmlen : hashsize,
    hashes[hashno].name, "file", hashfilename, resultarray,
    printResults, printPassFail);

  if (hashfp != stdin) fclose(hashfp);
  if (info) free(Message_Digest);
  return err;
}

/*
 * Exercise a hash series of functions through multiple permutations.
 * The input is an initial seed.  That seed is replicated 3 times.
 * For 1000 rounds, the previous three results are used as the input.
 * This result is then checked, and used to seed the next cycle.
 * If the result is known, it is in resultarrays in uppercase hex.
 */
void randomtest(int hashno, const char *seed, int hashsize,
    const char **resultarrays, int randomcount,
    int printResults, int printPassFail)
{
  int i, j; char buf[20];
  unsigned char SEED[USHAMaxHashSize], MD[1003][USHAMaxHashSize];

  /* INPUT: Seed - A random seed n bits long */
  memcpy(SEED, seed, hashsize);
  if (printResults == PRINTTEXT) {
    printf("%s random test seed= '", hashes[hashno].name);
    printxstr(seed, hashsize);
    printf("'\n");
  }

  for (j = 0; j < randomcount; j++) {
    /* MD0 = MD1 = MD2 = Seed; */
    memcpy(MD[0], SEED, hashsize);
    memcpy(MD[1], SEED, hashsize);
    memcpy(MD[2], SEED, hashsize);
    for (i=3; i<1003; i++) {
      /* Mi = MDi-3 || MDi-2 || MDi-1; */
      USHAContext Mi;
      memset(&Mi, '\343', sizeof(Mi)); /* force bad data into struct */
      USHAReset(&Mi, hashes[hashno].whichSha);
      USHAInput(&Mi, MD[i-3], hashsize);
      USHAInput(&Mi, MD[i-2], hashsize);
      USHAInput(&Mi, MD[i-1], hashsize);
      /* MDi = SHA(Mi); */
      USHAResult(&Mi, MD[i]);
    }

    /* MDj = Seed = MDi; */
    memcpy(SEED, MD[i-1], hashsize);

    /* OUTPUT: MDj */
    sprintf(buf, "%d", j);
    printResult(SEED, hashsize, hashes[hashno].name, "random test",
      buf, resultarrays ? resultarrays[j] : 0, printResults,
      (j < RANDOMCOUNT) ? printPassFail : 0);
  }
}

/*
 * Look up a hash name.
 */
int findhash(const char *argv0, const char *opt)
{
  int i;
  const char *names[HASHCOUNT][2] = {
    { "0", "sha1" }, { "1", "sha224" }, { "2", "sha256" },
    { "3", "sha384" }, { "4", "sha512" }
  };
  for (i = 0; i < HASHCOUNT; i++)
    if ((strcmp(opt, names[i][0]) == 0) ||
        (scasecmp(opt, names[i][1]) == 0))
      return i;

  fprintf(stderr, "%s: Unknown hash name: '%s'\n", argv0, opt);
  usage(argv0);
  return 0;
}

/*
 * Run some tests that should invoke errors.
 */
void testErrors(int hashnolow, int hashnohigh, int printResults,
    int printPassFail)
{
  USHAContext usha;
  uint8_t Message_Digest[USHAMaxHashSize];
  int hashno, err;

  for (hashno = hashnolow; hashno <= hashnohigh; hashno++) {
    memset(&usha, '\343', sizeof(usha)); /* force bad data */
    USHAReset(&usha, hashno);
    USHAResult(&usha, Message_Digest);
    err = USHAInput(&usha, (const unsigned char *)"foo", 3);
    if (printResults == PRINTTEXT)
      printf ("\nError %d. Should be %d.\n", err, shaStateError);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) && (err != shaStateError)))
      printf("%s se: %s\n", hashes[hashno].name,
        (err == shaStateError) ? "PASSED" : "FAILED");

    err = USHAFinalBits(&usha, 0x80, 3);
    if (printResults == PRINTTEXT)
      printf ("\nError %d. Should be %d.\n", err, shaStateError);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) && (err != shaStateError)))
      printf("%s se: %s\n", hashes[hashno].name,
        (err == shaStateError) ? "PASSED" : "FAILED");

    err = USHAReset(0, hashes[hashno].whichSha);
    if (printResults == PRINTTEXT)
       printf("\nError %d. Should be %d.\n", err, shaNull);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) && (err != shaNull)))
       printf("%s usha null: %s\n", hashes[hashno].name,
        (err == shaNull) ? "PASSED" : "FAILED");

    switch (hashno) {
      case SHA1: err = SHA1Reset(0); break;
      case SHA224: err = SHA224Reset(0); break;
      case SHA256: err = SHA256Reset(0); break;
      case SHA384: err = SHA384Reset(0); break;
      case SHA512: err = SHA512Reset(0); break;
    }
    if (printResults == PRINTTEXT)
       printf("\nError %d. Should be %d.\n", err, shaNull);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) && (err != shaNull)))
       printf("%s sha null: %s\n", hashes[hashno].name,
        (err == shaNull) ? "PASSED" : "FAILED");
  }
}

/* replace a hex string in place with its value */
int unhexStr(char *hexstr)
{
  char *o = hexstr;
  int len = 0, nibble1 = 0, nibble2 = 0;
  if (!hexstr) return 0;
  for ( ; *hexstr; hexstr++) {
    if (isalpha((int)(unsigned char)(*hexstr))) {
      nibble1 = tolower((int)(unsigned char)(*hexstr)) - 'a' + 10;
    } else if (isdigit((int)(unsigned char)(*hexstr))) {
      nibble1 = *hexstr - '0';
    } else {
      printf("\nError: bad hex character '%c'\n", *hexstr);
    }
    if (!*++hexstr) break;
    if (isalpha((int)(unsigned char)(*hexstr))) {
      nibble2 = tolower((int)(unsigned char)(*hexstr)) - 'a' + 10;
    } else if (isdigit((int)(unsigned char)(*hexstr))) {
      nibble2 = *hexstr - '0';
    } else {
      printf("\nError: bad hex character '%c'\n", *hexstr);
    }
    *o++ = (char)((nibble1 << 4) | nibble2);
    len++;
  }
  return len;
}

int main(int argc, char **argv)
{
  int i, err;
  int loopno, loopnohigh = 1;
  int hashno, hashnolow = 0, hashnohigh = HASHCOUNT - 1;
  int testno, testnolow = 0, testnohigh;
  int ntestnohigh = 0;
  int printResults = PRINTTEXT;
  int printPassFail = 1;
  int checkErrors = 0;
  char *hashstr = 0;
  int hashlen = 0;
  const char *resultstr = 0;
  char *randomseedstr = 0;
  int runHmacTests = 0;
  int runHkdfTests = 0;
  char *hmacKey = 0;
  int hmaclen = 0;
  char *info = 0;
  int infolen = 0, okmlen = 0;
  int randomcount = RANDOMCOUNT;
  const char *hashfilename = 0;
  const char *hashFilename = 0;
  int extrabits = 0, numberExtrabits = 0;
  int strIsHex = 0;

  if ('A' != 0x41) {
    fprintf(stderr, "%s: these tests require ASCII\n", argv[0]);
  }

  while ((i = getopt(argc, argv,
                      "6b:B:def:F:h:i:Hk:l:L:mpPr:R:s:S:t:wxX")) != -1)
    switch (i) {
      case 'b': extrabits = strtol(optarg, 0, 0); break;
      case 'B': numberExtrabits = atoi(optarg); break;
      case 'd': runHkdfTests = 1; break;
      case 'e': checkErrors = 1; break;
      case 'f': hashfilename = optarg; break;
      case 'F': hashFilename = optarg; break;
      case 'h': hashnolow = hashnohigh = findhash(argv[0], optarg);
        break;
      case 'H': strIsHex = 1; break;
      case 'i': info = optarg; infolen = strlen(optarg); break;
      case 'k': hmacKey = optarg; hmaclen = strlen(optarg); break;
      case 'l': loopnohigh = atoi(optarg); break;
      case 'L': okmlen = strtol(optarg, 0, 0); break;
      case 'm': runHmacTests = 1; break;
      case 'P': printPassFail = 0; break;
      case 'p': printResults = PRINTNONE; break;
      case 'R': randomcount = atoi(optarg); break;
      case 'r': randomseedstr = optarg; break;
      case 's': hashstr = optarg; hashlen = strlen(hashstr); break;
      case 'S': resultstr = optarg; break;
      case 't': testnolow = ntestnohigh = atoi(optarg) - 1; break;
      case 'w': printResults = PRINTRAW; break;
      case 'x': printResults = PRINTHEX; break;
      case 'X': printPassFail = 2; break;
      case '6': printResults = PRINTBASE64; break;
      default: usage(argv[0]);
      }

  if (strIsHex) {
    hashlen = unhexStr(hashstr);
    unhexStr(randomseedstr);
    hmaclen = unhexStr(hmacKey);
    infolen = unhexStr(info);
  }
  testnohigh = (ntestnohigh != 0) ? ntestnohigh:
               runHmacTests ? (HMACTESTCOUNT-1) :
               runHkdfTests ? (HKDFTESTCOUNT-1) :
               (TESTCOUNT-1);
  if ((testnolow < 0) ||
      (testnohigh >= (runHmacTests ? HMACTESTCOUNT : TESTCOUNT)) ||
      (hashnolow < 0) || (hashnohigh >= HASHCOUNT) ||
      (hashstr && (testnolow == testnohigh)) ||
      (randomcount < 0) ||
      (resultstr && (!hashstr && !hashfilename && !hashFilename)) ||
      ((runHmacTests || hmacKey) && randomseedstr) ||
      (hashfilename && hashFilename) ||
      (info && ((infolen <= 0) || (okmlen <= 0))) ||
      (info && !hmacKey))
    usage(argv[0]);

  /*
   *  Perform SHA/HMAC tests
   */
  for (hashno = hashnolow; hashno <= hashnohigh; ++hashno) {
    if (printResults == PRINTTEXT)
      printf("Hash %s\n", hashes[hashno].name);
    err = shaSuccess;

    for (loopno = 1; (loopno <= loopnohigh) && (err == shaSuccess);
         ++loopno) {
      if (hashstr)
        err = hash(0, loopno, hashno, hashstr, hashlen, 1,
          numberExtrabits, extrabits, (const unsigned char *)hmacKey,
          hmaclen, (const uint8_t *) info, infolen, okmlen, resultstr,
          hashes[hashno].hashsize, printResults, printPassFail);

      else if (randomseedstr)
        randomtest(hashno, randomseedstr, hashes[hashno].hashsize, 0,
          randomcount, printResults, printPassFail);

      else if (hashfilename)
        err = hashfile(hashno, hashfilename, extrabits,
                       numberExtrabits, 0,
                       (const unsigned char *)hmacKey, hmaclen,
                       (const uint8_t *) info, infolen, okmlen,
                       resultstr, hashes[hashno].hashsize,
                       printResults, printPassFail);

      else if (hashFilename)
        err = hashfile(hashno, hashFilename, extrabits,
                       numberExtrabits, 1,
                       (const unsigned char *)hmacKey, hmaclen,
                       (const uint8_t *) info, infolen, okmlen,
                       resultstr, hashes[hashno].hashsize,
                       printResults, printPassFail);

      else /* standard tests */ {
        for (testno = testnolow;
             (testno <= testnohigh) && (err == shaSuccess); ++testno) {
          if (runHmacTests) {
            err = hash(testno, loopno, hashno,
                       hmachashes[testno].dataarray[hashno] ?
                       hmachashes[testno].dataarray[hashno] :
                       hmachashes[testno].dataarray[1] ?
                       hmachashes[testno].dataarray[1] :
                       hmachashes[testno].dataarray[0],
                       hmachashes[testno].datalength[hashno] ?
                       hmachashes[testno].datalength[hashno] :
                       hmachashes[testno].datalength[1] ?
                       hmachashes[testno].datalength[1] :
                       hmachashes[testno].datalength[0],
                       1, 0, 0,
                       (const unsigned char *)(
                        hmachashes[testno].keyarray[hashno] ?
                        hmachashes[testno].keyarray[hashno] :
                        hmachashes[testno].keyarray[1] ?
                        hmachashes[testno].keyarray[1] :
                        hmachashes[testno].keyarray[0]),
                       hmachashes[testno].keylength[hashno] ?
                       hmachashes[testno].keylength[hashno] :
                       hmachashes[testno].keylength[1] ?
                       hmachashes[testno].keylength[1] :
                       hmachashes[testno].keylength[0],
                       0, 0, 0,
                       hmachashes[testno].resultarray[hashno],
                       hmachashes[testno].resultlength[hashno],
                       printResults, printPassFail);
          } else if (runHkdfTests) {
            err = hashHkdf(testno, loopno, hashno,
                       printResults, printPassFail);
          } else { /* sha tests */
            err = hash(testno, loopno, hashno,
                       hashes[hashno].tests[testno].testarray,
                       hashes[hashno].tests[testno].length,
                       hashes[hashno].tests[testno].repeatcount,
                       hashes[hashno].tests[testno].numberExtrabits,
                       hashes[hashno].tests[testno].extrabits,
                       0, 0, 0, 0, 0,
                       hashes[hashno].tests[testno].resultarray,
                       hashes[hashno].hashsize,
                       printResults, printPassFail);
          }
        }
        if (!runHmacTests && !runHkdfTests) {
          randomtest(hashno, hashes[hashno].randomtest,
            hashes[hashno].hashsize, hashes[hashno].randomresults,
            RANDOMCOUNT, printResults, printPassFail);
        }
      }
    }
  }

  /* Test some error returns */
  if (checkErrors) {
    testErrors(hashnolow, hashnohigh, printResults, printPassFail);
  }

  return 0;
}

/*
 * Compare two strings, case independently.
 * Equivalent to strcasecmp() found on some systems.
 */
int scasecmp(const char *s1, const char *s2)
{
  for (;;) {
    char u1 = tolower((int)(unsigned char)(*s1++));
    char u2 = tolower((int)(unsigned char)(*s2++));
    if (u1 != u2)
      return u1 - u2;
    if (u1 == '\0')
      return 0;
   }
}

