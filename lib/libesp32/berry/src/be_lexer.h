/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_LEXER_H
#define BE_LEXER_H

#include "be_object.h"

typedef enum {
    TokenNone = 0,
    TokenEOS,       /* end of source */
    TokenId,        /* identifier */
    TokenInteger,
    TokenReal,
    TokenString,
    /* operator, don't change order */
    /* assign operator */
    OptAssign,      /* operator, = */
    OptAddAssign,   /* operator, += */
    OptSubAssign,   /* operator, -= */
    OptMulAssign,   /* operator, *= */
    OptDivAssign,   /* operator, /= */
    OptModAssign,   /* operator, %= */
    OptAndAssign,   /* operator, &= */
    OptOrAssign,    /* operator, |= */
    OptXorAssign,   /* operator, ^= */
    OptLsfAssign,   /* operator, <<= */
    OptRsfAssign,   /* operator, >>= */
    /* binary operator */
    OptAdd,         /* operator, + */
    OptSub,         /* operator, - */
    OptMul,         /* operator, * */
    OptDiv,         /* operator, / */
    OptMod,         /* operator, % */
    OptLT,          /* operator, < */
    OptLE,          /* operator, <= */
    OptEQ,          /* operator, == */
    OptNE,          /* operator, != */
    OptGT,          /* operator, > */
    OptGE,          /* operator, >= */
    OptBitAnd,      /* operatoe, &  */
    OptBitOr,       /* operatoe, |  */
    OptBitXor,      /* operatoe, ^  */
    OptShiftL,      /* operatoe, <<  */
    OptShiftR,      /* operatoe, >>  */
    OptConnect,     /* operator, .. */
    OptAnd,         /* operator, && */
    OptOr,          /* operator, || */
    /* unary operator */
    OptNot,         /* operator, ! */
    OptFlip,        /* operator, ~ */
    /* postfix operator or bracket */
    OptLBK,         /* operator, ( bracket */
    OptRBK,         /* operator, ) bracket */
    OptLSB,         /* operator, [ square bracket */
    OptRSB,         /* operator, ] square bracket */
    OptLBR,         /* operator, { brace */
    OptRBR,         /* operator, } brace */
    OptDot,         /* operator, . dot */
    /* other symbol */
    OptComma,       /* operator, , */
    OptSemic,       /* operator, ; */
    OptColon,       /* operator, : */
    OptQuestion,    /* operator, ? */
    OptArrow,       /* operator, -> */
    /* keyword */
    KeyIf,          /* keyword if */
    KeyElif,        /* keyword elif */
    KeyElse,        /* keyword else */
    KeyWhile,       /* keyword while */
    KeyFor,         /* keyword for */
    KeyDef,         /* keyword def */
    KeyEnd,         /* keyword end */
    KeyClass,       /* keyword class */
    KeyBreak,       /* keyword break */
    KeyContinue,    /* keyword continue */
    KeyReturn,      /* keyword return */
    KeyTrue,        /* keyword true */
    KeyFalse,       /* keyword false */
    KeyNil,         /* keyword nil */
    KeyVar,         /* keyword var */
    KeyDo,          /* keyword do */
    KeyImport,      /* keyword import */
    KeyAs,          /* keyword as */
    KeyTry,         /* keyword try */
    KeyExcept,      /* keyword except */
    KeyRaise,       /* keyword raise */
    KeyStatic       /* keyword static */
} btokentype;

struct blexerreader {
    const char *s;
    size_t len;
    void *data;
    breader readf;
};

struct blexerbuf {
    char *s;
    size_t len, size;
};

typedef struct btoken {
    btokentype type;
    union {
        bstring *s;
        bint i;
        breal r;
    } u;
} btoken;

typedef struct blexer {
    const char *fname;
    btoken token;
    int linenumber;
    int lastline;
    btokentype cacheType;
    struct blexerbuf buf;
    struct blexerreader reader;
    bmap *strtab;
    bvm *vm;
    int cursor;
} blexer;

void be_lexer_init(blexer *lexer, bvm *vm,
    const char *fname, breader reader, void *data);
void be_lexer_deinit(blexer *lexer);
void be_lexerror(blexer *lexer, const char *msg);
int be_lexer_scan_next(blexer *lexer);
bstring* be_lexer_newstr(blexer *lexer, const char *str);
const char *be_token2str(bvm *vm, btoken *token);
const char* be_tokentype2str(btokentype type);

#endif
