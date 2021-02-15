/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_lexer.h"
#include "be_string.h"
#include "be_mem.h"
#include "be_gc.h"
#include "be_exec.h"
#include "be_map.h"
#include "be_vm.h"

#define SHORT_STR_LEN       32
#define EOS                 '\0' /* end of source */

#define type_count()        (int)array_count(kwords_tab)
#define lexbuf(lex)         ((lex)->buf.s)
#define isvalid(lex)        ((lex)->cursor < (lex)->endbuf)
#define lgetc(lex)          ((lex)->cursor)
#define setstr(lex, v)      ((lex)->token.u.s = (v))
#define setint(lex, v)      ((lex)->token.u.i = (v))
#define setreal(lex, v)     ((lex)->token.u.r = (v))
#define match(lex, pattern) while (pattern(lgetc(lex))) { save(lex); }

#if BE_USE_SCRIPT_COMPILER

/* IMPORTANT: This must follow the enum found in be_lexer.h !!! */
static const char* const kwords_tab[] = {
    "NONE", "EOS", "ID", "INT", "REAL", "STR",
    "=", "+=","-=", "*=", "/=", "%=", "&=", "|=",
    "^=", "<<=", ">>=", "+", "-", "*", "/", "%",
    "<", "<=", "==", "!=", ">", ">=", "&", "|",
    "^", "<<", ">>", "..", "&&", "||", "!", "~",
    "(", ")", "[", "]", "{", "}", ".", ",", ";",
    ":", "?", "->", "if", "elif", "else", "while",
    "for", "def", "end", "class", "break", "continue",
    "return", "true", "false", "nil", "var", "do",
    "import", "as", "try", "except", "raise"
};

void be_lexerror(blexer *lexer, const char *msg)
{
    bvm *vm = lexer->vm;
    const char *error = be_pushfstring(vm,
        "%s:%d: %s", lexer->fname, lexer->linenumber, msg);
    be_lexer_deinit(lexer);
    be_raise(vm, "syntax_error", error);
}

static void keyword_registe(bvm *vm)
{
    int i;
    for (i = KeyIf; i < type_count(); ++i) {
        bstring *s = be_newstr(vm, kwords_tab[i]);
        be_gc_fix(vm, gc_object(s));
        be_str_setextra(s, i);
    }
}

static void keyword_unregiste(bvm *vm)
{
    int i;
    for (i = KeyIf; i < type_count(); ++i) {
        bstring *s = be_newstr(vm, kwords_tab[i]);
        be_gc_unfix(vm, gc_object(s));
    }
}

static bstring* cache_string(blexer *lexer, bstring *s)
{
    bvalue *res;
    bvm *vm = lexer->vm;
    var_setstr(vm->top, s);
    be_stackpush(vm); /* cache string to stack */
    res = be_map_findstr(lexer->vm, lexer->strtab, s);
    if (res) {
        s = var_tostr(&be_map_val2node(res)->key);
    } else {
        res = be_map_insertstr(vm, lexer->strtab, s, NULL);
        var_setnil(res);
    }
    be_stackpop(vm, 1); /* pop string frome stack */
    return s;
}

static bstring* lexer_newstrn(blexer *lexer, const char *str, size_t len)
{
    return cache_string(lexer, be_newstrn(lexer->vm, str, len));
}

bstring* be_lexer_newstr(blexer *lexer, const char *str)
{
    return cache_string(lexer, be_newstr(lexer->vm, str));
}

static int next(blexer *lexer)
{
    struct blexerreader *lr = &lexer->reader;
    if (!(lr->len--)) {
        static const char eos = EOS;
        const char *s = lr->readf(lr->data, &lr->len);
        lr->s = s ? s : &eos;
        --lr->len;
    }
    // lexer->cursor = *lr->s++;        // SH
    lexer->cursor = pgm_read_byte(lr->s++);
    return lexer->cursor;
}

static void clear_buf(blexer *lexer)
{
    lexer->buf.len = 0;
}

/* save and next */
static int save(blexer *lexer)
{
    int ch = lgetc(lexer);
    struct blexerbuf *buf = &lexer->buf;
    if (buf->len >= buf->size) {
        size_t size = buf->size << 1;
        buf->s = be_realloc(lexer->vm, buf->s, buf->size, size);
        buf->size = size;
    }
    buf->s[buf->len++] = (char)ch;
    return next(lexer);
}

static bstring* buf_tostr(blexer *lexer)
{
    struct blexerbuf *buf = &lexer->buf;
    return lexer_newstrn(lexer, buf->s, buf->len);
}

static int is_newline(int c)
{
    return c == '\n' || c == '\r';
}

static int is_digit(int c)
{
    return c >= '0' && c <= '9';
}

static int is_octal(int c)
{
    return c >= '0' && c <= '7';
}

static int is_letter(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static int is_word(int c)
{
    return is_letter(c) || is_digit(c);
}

static int check_next(blexer *lexer, int c)
{
    if (lgetc(lexer) == c) {
        next(lexer);
        return 1;
    }
    return 0;
}

static int char2hex(int c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 0x0A;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 0x0A;
    }
    return -1;
}

static int check2hex(blexer *lexer, int c)
{
    c = char2hex(c);
    if (c < 0) {
        be_lexerror(lexer, "invalid hexadecimal number");
    }
    return c;
}

static int read_hex(blexer *lexer, const char *src)
{
    int c = check2hex(lexer, *src++);
    return ((unsigned)c << 4) + check2hex(lexer, *src);
}

static int read_oct(blexer *lexer, const char *src)
{
    int c = 0;
    const char *end = src + 3;
    while (src < end && is_octal(*src)) {
        c = 8 * c + *src++ - '0';
    }
    if (src < end) {
        be_lexerror(lexer, "invalid octal number");
    }
    return c;
}

static void tr_string(blexer *lexer)
{
    char *dst, *src, *end;
    dst = src = lexbuf(lexer);
    end = lexbuf(lexer) + lexer->buf.len;
    while (src < end) {
        int c = *src++;
        switch (c) {
        case '\n': case '\r':
            be_lexerror(lexer, "unfinished string");
            break;
        case '\\':
            switch (*src) {
            case 'a': c = '\a'; break;
            case 'b': c = '\b'; break;
            case 'f': c = '\f'; break;
            case 'n': c = '\n'; break;
            case 'r': c = '\r'; break;
            case 't': c = '\t'; break;
            case 'v': c = '\v'; break;
            case '\\': c = '\\'; break;
            case '\'': c = '\''; break;
            case '"': c = '"'; break;
            case '?': c = '?'; break;
            case 'x': c = read_hex(lexer, ++src); ++src; break;
            default:
                c = read_oct(lexer, src);
                if (c != EOS) {
                    src += 2;
                }
                break;
            }
            ++src;
            break;
        default:
            break;
        }
        *dst++ = (char)c;
    }
    lexer->buf.len = dst - lexbuf(lexer);
}

static int skip_newline(blexer *lexer)
{
    int lc = lgetc(lexer);
    next(lexer);
    if (is_newline(lgetc(lexer)) && lgetc(lexer) != lc) {
        next(lexer); /* skip "\n\r" or "\r\n" */
    }
    lexer->linenumber++;
    return lexer->cursor;
}

static void skip_comment(blexer *lexer)
{
    next(lexer); /* skip '#' */
    if (lgetc(lexer) == '-') { /* mult-line comment */
        int mark, c = 'x'; /* skip first '-' (#- ... -#) */
        do {
            mark = c == '-';
            if (is_newline(c)) {
                c = skip_newline(lexer);
                continue;
            }
            c = next(lexer);
        } while (!(mark && c == '#') && c != EOS);
        next(lexer); /* skip '#' */
    } else { /* line comment */
        while (!is_newline(lgetc(lexer)) && lgetc(lexer)) {
            next(lexer);
        }
    }
}

static bbool scan_realexp(blexer *lexer)
{
    int c = lgetc(lexer);
    if (c == 'e' || c == 'E') {
        c = save(lexer);
        if (c == '+' || c == '-') {
            c = save(lexer);
        }
        if (!is_digit(c)) {
            be_lexerror(lexer, "malformed number");
        }
        match(lexer, is_digit);
        return btrue;
    }
    return bfalse;
}

static btokentype scan_dot_real(blexer *lexer)
{
    if (save(lexer) == '.') { /* is '..' */
        next(lexer);
        return OptConnect;
    }
    if (is_digit(lgetc(lexer))) {
        match(lexer, is_digit);
        scan_realexp(lexer);
        setreal(lexer, be_str2real(lexbuf(lexer), NULL));
        return TokenReal;
    }
    return OptDot;
}

/* check the dots is a decimal dot or '..' operator */
static bbool decimal_dots(blexer *lexer)
{
    if (lgetc(lexer) == '.') { /* '..' or real */
        if (save(lexer) != '.') { /* read numberic => \.\b* */
            match(lexer, is_digit); /* match and skip numberic */
            return btrue;
        }
        /* token '..' */
        next(lexer); /*  skip the second '.' */
        lexer->cacheType = OptConnect;
    }
    return bfalse; /* operator '..' */
}

static bint scan_hexadecimal(blexer *lexer)
{
    bint res = 0;
    int dig, num = 0;
    while ((dig = char2hex(lgetc(lexer))) >= 0) {
        res = ((bint)res << 4) + dig;
        next(lexer);
        ++num;
    }
    if (num == 0) {
        be_lexerror(lexer, "invalid hexadecimal number");
    }
    return res;
}

static btokentype scan_decimal(blexer *lexer)
{
    btokentype type = TokenInteger;
    match(lexer, is_digit);
    if (decimal_dots(lexer) | scan_realexp(lexer)) {
        type = TokenReal;
    }
    lexer->buf.s[lexer->buf.len] = '\0';
    if (type == TokenReal) {
        setreal(lexer, be_str2real(lexbuf(lexer), NULL));
    } else {
        setint(lexer, be_str2int(lexbuf(lexer), NULL));
    }
    return type;
}

static btokentype scan_numeral(blexer *lexer)
{
    btokentype type = TokenInteger;
    int c0 = lgetc(lexer), c1 = save(lexer);
    /* hex: 0[xX][0-9a-fA-F]+ */
    if (c0 == '0' && (c1 == 'x' || c1 == 'X')) {
        next(lexer);
        setint(lexer, scan_hexadecimal(lexer));
    } else {
        type = scan_decimal(lexer);
    }
    /* can't follow decimal or letter after numeral */
    if (lexer->cacheType == TokenNone) {
        if (is_letter(lgetc(lexer)) || decimal_dots(lexer)) {
            be_lexerror(lexer, "malformed number");
        }
    }
    return type;
}

static btokentype scan_identifier(blexer *lexer)
{
    int type;
    bstring *s;
    save(lexer);
    match(lexer, is_word);
    s = buf_tostr(lexer);
    type = str_extra(s);
    if (type >= KeyIf && type < type_count()) {
        lexer->token.type = (btokentype)type;
        return lexer->token.type;
    }
    setstr(lexer, s); /* set identifier name */
    return TokenId;
}

static btokentype scan_string(blexer *lexer)
{
    int c, end = lgetc(lexer);
    next(lexer); /* skip '"' or '\'' */
    while ((c = lgetc(lexer)) != EOS && (c != end)) {
        save(lexer);
        if (c == '\\') {
            save(lexer); /* skip '\\.' */
        }
    }
    tr_string(lexer);
    setstr(lexer, buf_tostr(lexer));
    next(lexer); /* skip '"' or '\'' */
    return TokenString;
}

static btokentype scan_assign(blexer *lexer, btokentype is, btokentype not)
{
    next(lexer);
    return check_next(lexer, '=') ? is : not;
}

static btokentype scan_sub(blexer *lexer)
{
    btokentype op;
    switch (next(lexer)) {
        case '>': op = OptArrow; break;
        case '=': op = OptSubAssign; break;
        default: return OptSub;
    }
    next(lexer);
    return op;
}

static btokentype scan_and(blexer *lexer)
{
    btokentype op;
    switch (next(lexer)) {
        case '&': op = OptAnd; break;
        case '=': op = OptAndAssign; break;
        default: return OptBitAnd;
    }
    next(lexer);
    return op;
}

static btokentype scan_or(blexer *lexer)
{
    btokentype op;
    switch (next(lexer)) {
        case '|': op = OptOr; break;
        case '=': op = OptOrAssign; break;
        default: return OptBitOr;
    }
    next(lexer);
    return op;
}

static btokentype scan_le(blexer *lexer)
{
    switch (next(lexer)) {
    case '=':
        next(lexer);
        return OptLE;
    case '<':
        next(lexer);
        return check_next(lexer, '=') ? OptLsfAssign : OptShiftL;
    default:
        return OptLT;
    }
}

static btokentype scan_ge(blexer *lexer)
{
    switch (next(lexer)) {
    case '=':
        next(lexer);
        return OptGE;
    case '>':
        next(lexer);
        return check_next(lexer, '=') ? OptRsfAssign : OptShiftR;
    default:
        return OptGT;
    }
}

static btokentype lexer_next(blexer *lexer)
{
    for (;;) {
        switch (lgetc(lexer)) {
        case '\r': case '\n': /* newline */
            skip_newline(lexer);
            break;
        case ' ': case '\t': case '\f': case '\v': /* spaces */
            next(lexer);
            break;
        case '#': /* comment */
            skip_comment(lexer);
            break;
        case EOS: return TokenEOS; /* end of source stream */
        /* operator */
        case '+': return scan_assign(lexer, OptAddAssign, OptAdd);
        case '-': return scan_sub(lexer);
        case '*': return scan_assign(lexer, OptMulAssign, OptMul);
        case '/': return scan_assign(lexer, OptDivAssign, OptDiv);
        case '%': return scan_assign(lexer, OptModAssign, OptMod);
        case '(': next(lexer); return OptLBK;
        case ')': next(lexer); return OptRBK;
        case '[': next(lexer); return OptLSB;
        case ']': next(lexer); return OptRSB;
        case '{': next(lexer); return OptLBR;
        case '}': next(lexer); return OptRBR;
        case ',': next(lexer); return OptComma;
        case ';': next(lexer); return OptSemic;
        case ':': next(lexer); return OptColon;
        case '?': next(lexer); return OptQuestion;
        case '^': return scan_assign(lexer, OptXorAssign, OptBitXor);
        case '~': next(lexer); return OptFlip;
        case '&': return scan_and(lexer);
        case '|': return scan_or(lexer);
        case '<': return scan_le(lexer);
        case '>': return scan_ge(lexer);
        case '=':
            next(lexer);
            return check_next(lexer, '=') ? OptEQ : OptAssign;
        case '!':
            next(lexer);
            return check_next(lexer, '=') ? OptNE : OptNot;
        case '\'': case '"':
            return scan_string(lexer);
        case '.':
            return scan_dot_real(lexer);
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return scan_numeral(lexer);
        default:
            if (is_letter(lgetc(lexer))) {
                return scan_identifier(lexer);
            }
            be_lexerror(lexer, be_pushfstring(lexer->vm,
                "stray '\\%d' in program", (unsigned char)lgetc(lexer)));
            return TokenNone; /* error */
        }
    }
}

static void lexerbuf_init(blexer *lexer)
{
    lexer->buf.size = SHORT_STR_LEN;
    lexer->buf.s = be_malloc(lexer->vm, SHORT_STR_LEN);
    lexer->buf.len = 0;
}

void be_lexer_init(blexer *lexer, bvm *vm,
    const char *fname, breader reader, void *data)
{
    lexer->vm = vm;
    lexer->cacheType = TokenNone;
    lexer->fname = fname;
    lexer->linenumber = 1;
    lexer->lastline = 1;
    lexer->reader.readf = reader;
    lexer->reader.data = data;
    lexer->reader.len = 0;
    lexerbuf_init(lexer);
    keyword_registe(vm);
    lexer->strtab = be_map_new(vm);
    var_setmap(vm->top, lexer->strtab);
    be_stackpush(vm); /* save string to cache */
    next(lexer); /* read the first character */
}

void be_lexer_deinit(blexer *lexer)
{
    be_free(lexer->vm, lexer->buf.s, lexer->buf.size);
    keyword_unregiste(lexer->vm);
    be_stackpop(lexer->vm, 1); /* pop strtab */
}

int be_lexer_scan_next(blexer *lexer)
{
    btokentype type;
    if (lexer->cacheType != TokenNone) {
        lexer->token.type = lexer->cacheType;
        lexer->cacheType = TokenNone;
        return 1;
    }
    if (lgetc(lexer) == EOS) { /* clear lexer */
        lexer->token.type = TokenEOS;
        return 0;
    }
    lexer->lastline = lexer->linenumber;
    type = lexer_next(lexer);
    clear_buf(lexer);
    if (type != TokenNone) {
        lexer->token.type = type;
    } else {
        lexer->token.type = TokenEOS;
        return 0;
    }
    return 1;
}

const char* be_token2str(bvm *vm, btoken *token)
{
    switch (token->type) {
    case TokenString:
    case TokenId:
        return str(token->u.s);
    case TokenInteger:
        return be_pushfstring(vm, "%d", token->u.i);
    case TokenReal:
        return be_pushfstring(vm, "%g", token->u.r);
    default:
        return kwords_tab[token->type];
    }
}

const char* be_tokentype2str(btokentype type)
{
    return kwords_tab[type];
}

#endif
