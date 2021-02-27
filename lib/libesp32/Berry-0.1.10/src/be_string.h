/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_STRING_H
#define BE_STRING_H

#include "be_object.h"

#define SHORT_STR_MAX_LEN   64

typedef struct {
    bstring_header;
#if BE_USE_STR_HASH_CACHE
    uint32_t hash;
#endif
    /* char s[]; */
} bsstring;

typedef struct {
    bstring str;
    int llen;
    /* char s[]; */
} blstring;

typedef struct {
    bstring_header;
    uint32_t hash;
    const char *s;
} bcstring;

#define str_len(_s) \
    (pgm_read_byte(&(_s)->slen) == 255 ? cast(blstring*, _s)->llen : pgm_read_byte(&(_s)->slen))

#define str(_s)                 be_str2cstr(_s)
#define str_extra(_s)           ((_s)->extra)
#define str_literal(_vm, _s)    be_newstrn((_vm), (_s), sizeof(_s) - 1)

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_const_strtab.h"
#endif

void be_string_init(bvm *vm);
void be_string_deleteall(bvm *vm);
int be_eqstr(bstring *s1, bstring *s2);
bstring* be_newstr(bvm *vm, const char *str);
bstring* be_newstrn(bvm *vm, const char *str, size_t len);
bstring* be_newlongstr(bvm *vm, const char *str, size_t len);
void be_gcstrtab(bvm *vm);
uint32_t be_strhash(const bstring *s);
const char* be_str2cstr(const bstring *s);
void be_str_setextra(bstring *s, int extra);

#endif
