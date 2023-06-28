/*
 ---------------------------------------------------------------------------
 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue Date: 20/12/2007
*/

// Correct Output (for variable block size - AES_BLOCK_SIZE undefined):

// lengths:  block = 16 bytes, key = 16 bytes
// key     = 2b7e151628aed2a6abf7158809cf4f3c
// input   = 3243f6a8885a308d313198a2e0370734
// encrypt = 3925841d02dc09fbdc118597196a0b32
// decrypt = 3243f6a8885a308d313198a2e0370734

// lengths:  block = 16 bytes, key = 24 bytes
// key     = 2b7e151628aed2a6abf7158809cf4f3c762e7160f38b4da5
// input   = 3243f6a8885a308d313198a2e0370734
// encrypt = f9fb29aefc384a250340d833b87ebc00
// decrypt = 3243f6a8885a308d313198a2e0370734

// lengths:  block = 16 bytes, key = 32 bytes
// key     = 2b7e151628aed2a6abf7158809cf4f3c762e7160f38b4da56a784d9045190cfe
// input   = 3243f6a8885a308d313198a2e0370734
// encrypt = 1a6e6c2c662e7da6501ffb62bc9e93f3
// decrypt = 3243f6a8885a308d313198a2e0370734

#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "aestst.h"

void out_state(long s0, long s1, long s2, long s3)
{
    printf("\n%08lx%08lx%08lx%08lx", s0, s1, s2, s3);
}

void oblk(char m[], unsigned char v[], unsigned long n)
{   unsigned long i;
    
    printf("\n%s", m);

    for(i = 0; i < n; ++i)
        printf("%02x", v[i]);
}

void message(const char *s)   { printf("%s", s); }

unsigned char pih[32] = // hex digits of pi
{
    0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
    0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34,
    0x4a, 0x40, 0x93, 0x82, 0x22, 0x99, 0xf3, 0x1d,
    0x00, 0x82, 0xef, 0xa9, 0x8e, 0xc4, 0xe6, 0xc8
};

unsigned char exh[32] =  // hex digits of e
{
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    0x76, 0x2e, 0x71, 0x60, 0xf3, 0x8b, 0x4d, 0xa5,
    0x6a, 0x78, 0x4d, 0x90, 0x45, 0x19, 0x0c, 0xfe
};

unsigned char res[3][32] =
{
    { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32
    },
    { 0xf9, 0xfb, 0x29, 0xae, 0xfc, 0x38, 0x4a, 0x25,
        0x03, 0x40, 0xd8, 0x33, 0xb8, 0x7e, 0xbc, 0x00
    },
    { 0x1a, 0x6e, 0x6c, 0x2c, 0x66, 0x2e, 0x7d, 0xa6,
        0x50, 0x1f, 0xfb, 0x62, 0xbc, 0x9e, 0x93, 0xf3
    }
};

void cycles(volatile uint64_t *rtn)
{
#if defined( _MSCVER )
    __asm   // read the Pentium Time Stamp Counter
    {   cpuid
        rdtsc
        mov     ecx,rtn
        mov     [ecx],eax
        mov     [ecx+4],edx
        cpuid
    }
#elif defined( __GNUC__ )
    /*__asm__ __volatile__("rdtsc": "=A" (*rtn)); */
#endif
}

int main(void)
{   unsigned char   out[32], ret[32], err = 0;
    f_ectx          alge[1];
    f_dctx          algd[1];

    aes_init();

    message("\nRun tests for the AES algorithm");

    memset(&alge, 0, sizeof(trzr_aes_encrypt_ctx));
    memset(&algd, 0, sizeof(trzr_aes_decrypt_ctx));

#if defined( AES_128 )
    memset(out, 0xcc, 16); memset(ret, 0xcc, 16);
    printf("\n\n// lengths:  block = 16, bytes, key = 16 bytes");
    f_enc_key128(alge, exh);
    oblk("// key     = ", exh, 16);
    oblk("// input   = ", pih, 16);
    do_enc(alge, pih, out, 1);
    oblk("// encrypt = ", out, 16);
    if(memcmp(out, res[0], 16)) { message (" error"); err += 1; }
    f_dec_key128(algd, exh);
    do_dec(algd, out, ret, 1);
    oblk("// decrypt = ", ret, 16);
    if(memcmp(ret, pih, 16)) { message (" error"); err += 2; }
#endif

#if defined( AES_192 )
    memset(out, 0xcc, 16); memset(ret, 0xcc, 16);
    printf("\n\n// lengths:  block = 16, bytes, key = 24 bytes");
    f_enc_key192(alge, exh);
    oblk("// key     = ", exh, 24);
    oblk("// input   = ", pih, 16);
    do_enc(alge, pih, out, 1);
    oblk("// encrypt = ", out, 16);
    if(memcmp(out, res[1], 16))  { message (" error"); err += 4; }
    f_dec_key192(algd, exh);
    do_dec(algd, out, ret, 1);
    oblk("// decrypt = ", ret, 16);
    if(memcmp(ret, pih, 16))  { message (" error"); err += 8; }
#endif

#if defined( AES_256 )
    memset(out, 0xcc, 16); memset(ret, 0xcc, 16);
    printf("\n\n// lengths:  block = 16, bytes, key = 32 bytes");
    f_enc_key256(alge, exh);
    oblk("// key     = ", exh, 32);
    oblk("// input   = ", pih, 16);
    do_enc(alge, pih, out, 1);
    oblk("// encrypt = ", out, 16);
    if(memcmp(out, res[2], 16))  { message (" error"); err += 16; }
    f_dec_key256(algd, exh);
    do_dec(algd, out, ret, 1);
    oblk("// decrypt = ", ret, 16);
    if(memcmp(ret, pih, 16))  { message (" error"); err += 32; }
#endif

    if(!err)
        message("\n\nThese values are all correct\n\n");
    else
        message("\n\nSome values are in error\n\n");

    return 0;
}
