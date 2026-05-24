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
#include "be_strlib.h"
#include <string.h>

#define SHORT_STR_LEN       32
#define EOS                 '\0' /* end of source */

#define type_count()        (int)array_count(token_strings)
#define lexbuf(lex)         ((lex)->buf.s)
#define isvalid(lex)        ((lex)->reader.cursor < (lex)->endbuf)
#define lgetc(lex)          ((lex)->reader.cursor)
#define setstr(lex, v)      ((lex)->token.u.s = (v))
#define setint(lex, v)      ((lex)->token.u.i = (v))
#define setreal(lex, v)     ((lex)->token.u.r = (v))
#define match(lex, pattern) while (pattern(lgetc(lex))) { save(lex); }

#if BE_USE_SCRIPT_COMPILER

/* IMPORTANT: This must follow the enum found in be_lexer.h !!! */
static const char* const token_strings[] = {
    "NONE", "EOS", "ID", "INT", "REAL", "STR",
    "=", "+=","-=", "*=", "/=", "%=", "&=", "|=",
    "^=", "<<=", ">>=", "+", "-", "*", "/", "%",
    "<", "<=", "==", "!=", ">", ">=", "&", "|",
    "^", "<<", ">>", "..", "&&", "||", "!", "~",
    "(", "(", ")", "[", "]", "{", "}", ".", ",", ";",
    ":", "?", "->", "if", "elif", "else", "while",
    "for", "def", "end", "class", "break", "continue",
    "return", "true", "false", "nil", "var", "do",
    "import", "as", "try", "except", "raise", "static",
    ":=",
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
        bstring *s = be_newstr(vm, token_strings[i]);
        be_gc_fix(vm, gc_object(s));
        be_str_setextra(s, i);
    }
}

static void keyword_unregiste(bvm *vm)
{
    int i;
    for (i = KeyIf; i < type_count(); ++i) {
        bstring *s = be_newstr(vm, token_strings[i]);
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
        const char *s = lr->readf(lexer, lr->data, &lr->len);
        lr->s = s ? s : &eos;
        --lr->len;
    }
    lexer->reader.cursor = *lr->s++;
    return lexer->reader.cursor;
}

static void clear_buf(blexer *lexer)
{
    lexer->buf.len = 0;
}

static void save_char(blexer *lexer, int ch) {
    struct blexerbuf *buf = &lexer->buf;
    if (buf->len >= buf->size) {
        size_t size = buf->size << 1;
        buf->s = be_realloc(lexer->vm, buf->s, buf->size, size);
        buf->size = size;
    }
    buf->s[buf->len++] = (char)ch;
}

/* save and next */
static int save(blexer *lexer)
{
    int ch = lgetc(lexer);
    save_char(lexer, ch);
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

static int is_ident_start(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static int is_word(int c)
{
    return is_ident_start(c) || is_digit(c);
}

static int check_next(blexer *lexer, int c)
{
    if (lgetc(lexer) == c) {
        next(lexer);
        return 1;
    }
    return 0;
}

static int check2hex(blexer *lexer, int c)
{
    c = be_char2hex(c);
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

char* be_load_unicode(char *dst, const char *src)
{
    int ucode = 0, i = 4;
    while (i--) {
        int ch = *src++;
        if (ch >= '0' && ch <= '9') {
            ucode = (ucode << 4) | (ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            ucode = (ucode << 4) | (ch - 'A' + 0x0A);
        } else if (ch >= 'a' && ch <= 'f') {
            ucode = (ucode << 4) | (ch - 'a' + 0x0A);
        } else {
            return NULL;
        }
    }
    /* convert unicode to utf8 */
    if (ucode < 0x007F) {
        /* unicode: 0000 - 007F -> utf8: 0xxxxxxx */
        *dst++ = (char)(ucode & 0x7F);
    } else if (ucode < 0x7FF) {
        /* unicode: 0080 - 07FF -> utf8: 110xxxxx 10xxxxxx */
        *dst++ = (char)(((ucode >> 6) & 0x1F) | 0xC0);
        *dst++ = (char)((ucode & 0x3F) | 0x80);
    } else {
        /* unicode: 0800 - FFFF -> utf8: 1110xxxx 10xxxxxx 10xxxxxx */
        *dst++ = (char)(((ucode >> 12) & 0x0F) | 0xE0);
        *dst++ = (char)(((ucode >> 6) & 0x03F) | 0x80);
        *dst++ = (char)((ucode & 0x3F) | 0x80);
    }
    return dst;
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
            if (src >= end) {
                be_lexerror(lexer, "invalid escape sequence");
            }
            if (*src != 'u') {
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
                case 'x':
                    if (end - src < 3) {  /* need 'x' + 2 hex digits */
                        be_lexerror(lexer, "invalid hexadecimal number");
                    }
                    c = read_hex(lexer, ++src);
                    ++src;
                    break;
                default:
                    if (end - src < 3) {  /* need 3 octal digits */
                        be_lexerror(lexer, "invalid octal number");
                    }
                    c = read_oct(lexer, src);
                    if (c != EOS) {
                        src += 2;
                    }
                    break;
                }
                ++src;
                *dst++ = (char)c;
            } else {
                /* unicode encoding, ex "\uF054" is equivalent to "\xEF\x81\x94"*/
                if (end - src < 5) {  /* need 'u' + 4 hex digits */
                    be_lexerror(lexer, "incorrect '\\u' encoding");
                }
                dst = be_load_unicode(dst, src + 1);
                src += 5;
                if (dst == NULL) {
                    be_lexerror(lexer, "incorrect '\\u' encoding");
                }
            }
            break;
        default:
            *dst++ = (char)c;
            break;
        }
    }
    size_t len = dst - lexbuf(lexer);
    /* equivalent to strnlen() */
    /* lexer->buf.len = strnlen(lexbuf(lexer), len); */
    const char* str = (const char*) lexbuf(lexer);
    const char* found = memchr(str, '\0', len);
    lexer->buf.len = found ? (size_t)(found - str) : len;
}

static int skip_newline(blexer *lexer)
{
    int lc = lgetc(lexer);
    next(lexer);
    if (is_newline(lgetc(lexer)) && lgetc(lexer) != lc) {
        next(lexer); /* skip "\n\r" or "\r\n" */
    }
    lexer->linenumber++;
#if BE_USE_PREPROCESSOR
    lexer->pp_at_line_start = btrue;
#endif
    return lexer->reader.cursor;
}

static void skip_comment(blexer *lexer)
{
    next(lexer); /* skip '#' */
    if (lgetc(lexer) == '-') { /* mult-line comment */
        int lno = lexer->linenumber;
        int mark, c = 'x'; /* skip first '-' (#- ... -#) */
        do {
            mark = c == '-';
            if (is_newline(c)) {
                c = skip_newline(lexer);
                continue;
            }
            c = next(lexer);
        } while (!(mark && c == '#') && c != EOS);
        if (c == EOS) {
            be_lexerror(lexer, be_pushfstring(lexer->vm, "unterminated comment block started in line %d", lno));
        }
        next(lexer); /* skip '#' */
    } else { /* line comment */
        while (!is_newline(lgetc(lexer)) && lgetc(lexer)) {
            next(lexer);
        }
    }
}

#if BE_USE_PREPROCESSOR
/* Like skip_comment but '#' has already been consumed.
 * The cursor is on the character after '#'. */
static void skip_comment_body(blexer *lexer)
{
    if (lgetc(lexer) == '-') { /* block comment #- ... -# */
        int lno = lexer->linenumber;
        int mark, c = 'x';
        do {
            mark = c == '-';
            if (is_newline(c)) {
                c = skip_newline(lexer);
                continue;
            }
            c = next(lexer);
        } while (!(mark && c == '#') && c != EOS);
        if (c == EOS) {
            be_lexerror(lexer, be_pushfstring(lexer->vm,
                "unterminated comment block started in line %d", lno));
        }
        next(lexer); /* skip trailing '#' */
    } else { /* line comment */
        while (!is_newline(lgetc(lexer)) && lgetc(lexer)) {
            next(lexer);
        }
    }
}
#endif

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
        save_char(lexer, '\0');
        setreal(lexer, be_str2real(lexbuf(lexer), NULL));
        --lexer->buf.len;  /* drop the trailing NUL we just added */
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
    while ((dig = be_char2hex(lgetc(lexer))) >= 0) {
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
    /* decimal_dots() and scan_realexp() have side effect, so we call each explicitly */
    /* to prevent binary shortcut if the first is true */
    bbool has_decimal_dots = decimal_dots(lexer);
    bbool is_realexp = scan_realexp(lexer);
    if (has_decimal_dots || is_realexp) {
        type = TokenReal;
    }
    /* use save_char to add the null terminator, */
    /* since it handles expanding the buffer if needed. */
    save_char(lexer, '\0');
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
    /* can't follow identifier start or decimal after numeral */
    if (lexer->cacheType == TokenNone) {
        if (is_ident_start(lgetc(lexer)) || decimal_dots(lexer)) {
            be_lexerror(lexer, "malformed number");
        }
    }
    return type;
}

/* structure for a temporary reader used by transpiler, with attributes for an allocated buffer and size */
struct blexerreader_save {
    struct blexerreader reader;    
    char* s;    
    size_t size;
    char cursor;
};

/* buf reader for transpiled code from f-strings */
/* it restores the original reader when the transpiler buffer is empty */
/* the first pass returns a single byte buffer with the saved cursor */
/* the second pass restores the original reader */
static const char* _bufgets(struct blexer* lexer, void *data, size_t *size)
{
    /* this is called once the temporaty transpiler buffer is empty */
    struct blexerreader *reader = &lexer->reader;       /* current reader which is temporary only for the transpiler */
    struct blexerreader_save *reader_save = data;          /* original reader that needs to be restored once the buffer is empty */

    /* first case, we saved the cursor (fist char), we server it now */
    if (reader_save->reader.cursor >= 0) {                        /* serve the previously saved cursor */
        /* copy cursor to a 'char' type */
        reader_save->cursor = reader_save->reader.cursor;
        reader_save->reader.cursor = -1;                          /* no more cursor saved */
        *size = 1;
        return &reader_save->cursor;
    }

    /* second case, the saved cursor was returned, now restore the normal flow of the original reader */
    /* restore the original reader */
    *reader = reader_save->reader;

    /* free the memory from the structure */
    be_free(lexer->vm, reader_save->s, reader_save->size);                  /* free the buffer */
    be_free(lexer->vm, reader_save, sizeof(struct blexerreader_save));      /* free the structure */

    if (!reader->len) {     /* just in case the original buffer was also */
        return reader->readf(lexer, reader->data, size);
    }
    /* the following is not necessary, but safer */
    *size = reader->len;
    return reader->s;
}

static btokentype scan_string(blexer *lexer);   /* forward declaration */

/* scan f-string and transpile it to `format(...)` syntax then feeding the normal lexer and parser */
static void scan_f_string(blexer *lexer)
{
    char ch = '\0';
    clear_buf(lexer);
    scan_string(lexer);         /* first scan the entire string in lexer->buf */

    /* save original reader until the transpiled is processed */
    /* reader will be restored by the reader function once the transpiled buffer is empty */
    struct blexerreader_save *reader_save = (struct blexerreader_save *) be_malloc(lexer->vm, sizeof(struct blexerreader_save));           /* temporary reader */
    reader_save->reader = lexer->reader;

    /* save blexerbuf which contains the unparsed_fstring */
    struct blexerbuf buf_unparsed_fstr = lexer->buf;

    /* prepare and allocated a temporary buffer to save parsed f_string */
    lexer->buf.size = lexer->buf.size + 20;
    lexer->buf.s = be_malloc(lexer->vm, lexer->buf.size);
    lexer->buf.len = 0;

    /* parse f_string */
    /* First pass, check syntax and extract string literals, and format */
    save_char(lexer, '(');
    save_char(lexer, '"');
    for (size_t i = 0; i < buf_unparsed_fstr.len; i++) {
        ch = buf_unparsed_fstr.s[i];
        switch (ch) {
            case '%':       /* % needs to be encoded as %% */
                save_char(lexer, '%');
                save_char(lexer, '%');
                break;
            case '\\':       /* \ needs to be encoded as \\ */
                save_char(lexer, '\\');
                save_char(lexer, '\\');
                break;
            case '"':       /* " needs to be encoded as \" */
                save_char(lexer, '\\');
                save_char(lexer, '"');
                break;
            case '}':       /* }} is converted as } yet we tolerate a single } */
                if ((i+1 < buf_unparsed_fstr.len) && (buf_unparsed_fstr.s[i+1] == '}')) { i++; }      /* if '}}' replace with '}' */
                save_char(lexer, '}');
                break;
            case '\n':
                save_char(lexer, '\\');
                save_char(lexer, 'n');
                break;
            case '\r':
                save_char(lexer, '\\');
                save_char(lexer, 'r');
                break;
            default:        /* copy any other character */
                save_char(lexer, ch);
                break;
            case '{':       /* special case for { */
                i++;        /* in all cases skip to next char */
                if ((i < buf_unparsed_fstr.len) && (buf_unparsed_fstr.s[i] == '{')) {
                    save_char(lexer, '{');      /* {{ is simply encoded as { and continue parsing */
                } else {
                    /* we still don't know if '=' is present, so we copy the expression each time, and rollback if we find out the '=' is not present */
                    size_t rollback = lexer->buf.len;       /* mark the end of string for later rollback if '=' is not present */
                    /* parse inner part */
                    /* skip everything until either ':' or '}' or '=' */
                    /* if end of string is reached before '}' raise en error */
                    for (; i < buf_unparsed_fstr.len; i++) {
                        ch = buf_unparsed_fstr.s[i];
                        /* stop parsing if single ':' or '}' */
                        if (ch == '}' || (ch == ':' && buf_unparsed_fstr.s[i+1] != ':')) { break; }
                        save_char(lexer, ch);       /* copy any character unless it's '}' or single ':' */
                        if (ch == '=') { break; }   /* '=' is copied but breaks parsing as well */
                        if (ch == ':') { save_char(lexer, ch); i++; }     /* if '::' then encode the second ':' but don't parse it again in next iteration */
                    }
                    /* safe check if we reached the end of the string */
                    if (i >= buf_unparsed_fstr.len) { be_raise(lexer->vm, "syntax_error", "'}' expected"); }
                    /* if '=' detected then do some additional checks */
                    if (ch == '=') {
                        i++;        /* skip '=' and check we haven't reached the end */
                        if (i >= buf_unparsed_fstr.len) { be_raise(lexer->vm, "syntax_error", "'}' expected"); }
                        ch = buf_unparsed_fstr.s[i];
                        if ((ch != ':') && (ch != '}')) {   /* '=' must be immediately followed by ':' or '}' */
                            be_raise(lexer->vm, "syntax_error", "':' or '}' expected after '='");
                        }
                    } else {
                        /* no '=' present, rollback the text of the expression */
                        lexer->buf.len = rollback;
                    }
                    save_char(lexer, '%');       /* start format encoding */
                    if (ch == ':') {
                        /* copy format */
                        i++;
                        if ((i < buf_unparsed_fstr.len) && (buf_unparsed_fstr.s[i] == '%')) { i++; }      /* skip '%' following ':' */
                        for (; i < buf_unparsed_fstr.len; i++) {
                            ch = buf_unparsed_fstr.s[i];
                            if (ch == '}') { break; }
                            save_char(lexer, ch);
                        }
                        if (i >= buf_unparsed_fstr.len) { be_raise(lexer->vm, "syntax_error", "'}' expected"); }
                    } else {
                        /* if no formatting, output '%s' */
                        save_char(lexer, 's');
                    }
                }
                break;
        }
    }
    save_char(lexer, '"');      /* finish format string */

    /* Second pass - add arguments if any */
    for (size_t i = 0; i < buf_unparsed_fstr.len; i++) {
        /* skip any character that is not '{' followed by '{' */
        if (buf_unparsed_fstr.s[i] == '{') {
            i++;        /* in all cases skip to next char */
            if ((i < buf_unparsed_fstr.len) && (buf_unparsed_fstr.s[i] == '{')) { continue; }
            /* extract argument */
            save_char(lexer, ',');       /* add ',' to start next argument to `format()` */
            for (; i < buf_unparsed_fstr.len; i++) {
                ch = buf_unparsed_fstr.s[i];
                if (ch == ':' && (buf_unparsed_fstr.s[i+1] == ':')) {
                    save_char(lexer, ch);
                    i++;                    /* skip second ':' */
                } else if (ch == '=' || ch == ':' || ch == '}') {
                    break;
                } else {
                    save_char(lexer, ch);   /* copy expression until we reach ':', '=' or '}' */
                }
            }
            /* no need to check for end of string here, it was done already in first pass */
            if (ch == '=' || ch == ':') {       /* if '=' or ':', skip everyting until '}' */
                i++;
                for (; i < buf_unparsed_fstr.len; i++) {
                    ch = buf_unparsed_fstr.s[i];
                    if (ch == '}') { break; }
                }
            }
        }
    }
    save_char(lexer, ')');      /* add final ')' */

    /* Situation now: */
    /* `buf_unparsed_fstr` contains the buffer of the input unparsed f-string, ex: "age: {age:2i}" */
    /* `lexer->buf` contains the buffer of the transpiled f-string without call to format(), ex: '("age: %2i", age)' */
    /* `reader_save` contains the original reader to continue parsing after f-string */
    /* `lexer->reader` will contain a temporary reader from the parsed f-string */

    /* extract the parsed f-string from the temporary buffer (needs later deallocation) */
    char * parsed_fstr_s = lexer->buf.s;        /* needs later deallocation with parsed_fstr_size */
    size_t parsed_fstr_len = lexer->buf.len;
    size_t parsed_fstr_size = lexer->buf.size;

    /* restore buf to lexer */
    lexer->buf = buf_unparsed_fstr;

    /* change the temporary reader to the parsed f-string */
    lexer->reader.len = parsed_fstr_len;
    lexer->reader.data = (void*) reader_save;      /* link to the saved context */
    lexer->reader.s = parsed_fstr_s;     /* reader is responisble to deallocate later this buffer */
    lexer->reader.readf = _bufgets;

    /* add information needed for `_bufgets` to later deallocate the buffer */
    reader_save->size = parsed_fstr_size;
    reader_save->s = parsed_fstr_s;

    /* start parsing the parsed f-string, which is btw always '(' */
    next(lexer);

    /* remember that we are still in `scan_identifier()`, we replace the 'f' identifier to 'format' which is the global function to call */
    static const char FORMAT[] = "format";
    lexer->buf.len = sizeof(FORMAT) - 1;        /* we now that buf size is at least SHORT_STR_LEN (32) */
    memmove(lexer->buf.s, FORMAT, lexer->buf.len);
}

static btokentype scan_identifier(blexer *lexer)
{
    int type;
    bstring *s;
    save(lexer);
    match(lexer, is_word);
    /* check if the form is f"aaaa" or f'aaa' */
    char ch = lgetc(lexer);
    if ((lexer->buf.len == 1) && (lexer->buf.s[0] == 'f') && (ch == '"' || ch == '\'')) {
        scan_f_string(lexer);
    }
    s = buf_tostr(lexer);
    type = str_extra(s);
    if (type >= KeyIf && type < type_count()) {
        lexer->token.type = (btokentype)type;
        return lexer->token.type;
    }
    setstr(lexer, s); /* set identifier name */
    return TokenId;
}

/* munch any delimeter and return 1 if any found */
#if BE_USE_PREPROCESSOR
/* forward declarations for preprocessor functions used in skip_delimiter */
static bbool pp_skipping(blexer *lexer);
static void pp_skip_to_eol(blexer *lexer);
static void pp_define(blexer *lexer);
static void pp_undef(blexer *lexer);
static bbool pp_process_directive(blexer *lexer);
#endif
static int skip_delimiter(blexer *lexer) {
    int c = lgetc(lexer);
    int delimeter_present = 0;
    while (1) {
        if (c == '#') {
#if BE_USE_PREPROCESSOR
            /* When at line start, '#' followed by a lowercase letter
             * could be a preprocessor directive.  Stop skipping so
             * the main lexer loop can handle it properly (it has the
             * skipping logic for false conditional blocks).
             * For '#' followed by anything else (space, '-', '"', etc.)
             * treat as a normal comment. */
            if (lexer->pp_at_line_start) {
                int nc = next(lexer); /* consume '#', peek next */
                if (nc >= 'a' && nc <= 'z') {
                    /* Potential directive — stop.  We already consumed
                     * '#' but lexer_next also consumes '#' before
                     * checking directives.  So we set a flag to tell
                     * lexer_next that '#' is already consumed. */
                    lexer->pp_hash_consumed = btrue;
                    break;
                }
                /* Not a directive — handle as comment.
                 * '#' is already consumed, use skip_comment_body. */
                skip_comment_body(lexer);
                c = lgetc(lexer);
                delimeter_present = 1;
                continue;
            }
#endif
            skip_comment(lexer);
        } else if (c == '\r' || c == '\n') {
            skip_newline(lexer);
        } else if (c == ' ' || c == '\t' || c == '\f' || c ==  '\v') {
            next(lexer);
        } else {
            break;
        }
        c = lgetc(lexer);
        delimeter_present = 1;
    }
    lexer->had_whitespace = delimeter_present;
    return delimeter_present;
}

/* Scan a single string literal from the opening quote to the closing
 * quote, appending raw (un-escaped) bytes to the lexer buffer.
 * Cursor must be positioned on the opening quote; on exit the cursor
 * is past the closing quote.  Does NOT call tr_string — the caller
 * is responsible for escape processing. */
static void scan_one_literal(blexer *lexer)
{
    int c;
    int end = lgetc(lexer);     /* string delimiter, either '"' or '\'' */
    next(lexer); /* skip opening quote */
    while ((c = lgetc(lexer)) != EOS && (c != end)) {
        save(lexer);
        if (c == '\\') {
            save(lexer); /* skip '\\.' */
        }
    }
    if (c == EOS) {
        be_lexerror(lexer, "unfinished string");
    }
    next(lexer); /* skip closing quote */
}

/* After one or more string literals have been scanned into the lexer
 * buffer (still un-escaped), pick up any adjacent string literals and
 * append their raw bytes to the buffer.  On return, the cursor is
 * past the last closing quote and the buffer holds the concatenated
 * raw bytes of all literals.  The caller is responsible for calling
 * tr_string() to process escapes on the final buffer. */
static void scan_string_continue(blexer *lexer)
{
    int c;
    skip_delimiter(lexer);
    c = lgetc(lexer);
    while (c == '"' || c == '\'') {
        scan_one_literal(lexer);
        skip_delimiter(lexer);
        c = lgetc(lexer);
    }
}

static btokentype scan_string(blexer *lexer)
{
    /* scan first literal */
    scan_one_literal(lexer);
    /* then pick up any adjacent string literals */
    scan_string_continue(lexer);
    tr_string(lexer);
    setstr(lexer, buf_tostr(lexer));
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

/********************************************************************
** Preprocessor helper functions
********************************************************************/
#if BE_USE_PREPROCESSOR

/* Check if currently skipping (inside a false conditional block) */
static bbool pp_skipping(blexer *lexer)
{
    return lexer->ppdepth >= 0
        && !lexer->ppstack[lexer->ppdepth].active;
}

/* Consume characters until newline or EOF */
static void pp_skip_to_eol(blexer *lexer)
{
    while (!is_newline(lgetc(lexer)) && lgetc(lexer) != EOS) {
        next(lexer);
    }
}

/* Read an identifier from current position into lexer buffer.
 * Returns the length of the identifier read (0 if none). */
static int pp_read_ident(blexer *lexer)
{
    int len = 0;
    clear_buf(lexer);
    if (is_ident_start(lgetc(lexer))) {
        do {
            save(lexer);
            len++;
        } while (is_word(lgetc(lexer)));
    }
    return len;
}

/* Look up a macro name in the VM's preprocessor map.
 * Returns pointer to the bvalue if found, NULL otherwise. */
static bvalue* pp_find_macro(blexer *lexer, const char *name, int len)
{
    bvm *vm = lexer->vm;
    if (vm->preprocessor) {
        bstring *key = be_newstrn(vm, name, len);
        return be_map_findstr(vm, vm->preprocessor, key);
    }
    return NULL;
}

/* Evaluate truthiness of a macro for #if evaluation.
 * Returns btrue if the macro is defined and its value is not BE_INT 0. */
static bbool pp_macro_truthy(blexer *lexer, const char *name, int len)
{
    bvalue *val = pp_find_macro(lexer, name, len);
    if (!val) {
        return bfalse; /* undefined → falsy */
    }
    if (var_isint(val) && var_toint(val) == 0) {
        return bfalse; /* defined as 0 → falsy */
    }
    return btrue; /* everything else → truthy */
}

/* Ensure the VM's preprocessor map exists, lazily allocating on first use. */
static bmap* pp_ensure_map(blexer *lexer)
{
    bvm *vm = lexer->vm;
    if (!vm->preprocessor) {
        vm->preprocessor = be_map_new(vm);
    }
    return vm->preprocessor;
}

/* Try to parse a string as an integer. Returns btrue and sets *out on success. */
static bbool pp_parse_int(const char *s, int len, bint *out)
{
    bint val = 0;
    int i = 0;
    int neg = 0;
    if (len == 0) {
        return bfalse;
    }
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    } else if (s[0] == '+') {
        i = 1;
    }
    if (i >= len) {
        return bfalse; /* sign only, no digits */
    }
    for (; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return bfalse;
        }
        val = val * 10 + (s[i] - '0');
    }
    *out = neg ? -val : val;
    return btrue;
}

/* Handle #define MACRO_NAME [value]
 * Parse macro name and optional value from rest of line.
 * - No value: store BE_INT 1
 * - Value "0" or numeric: store BE_INT
 * - Otherwise: store BE_STRING
 */
static void pp_define(blexer *lexer)
{
    bvm *vm = lexer->vm;
    bmap *map;
    bstring *key;
    bvalue val;
    int namelen;
    const char *name;
    const char *vstart;
    int vlen;

    /* skip whitespace after #define */
    while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
        next(lexer);
    }

    /* read macro name */
    namelen = pp_read_ident(lexer);
    if (namelen == 0) {
        be_lexerror(lexer, "macro name expected after '#define'");
        return;
    }
    name = lexbuf(lexer);

    /* intern the macro name string before we reuse the buffer */
    map = pp_ensure_map(lexer);
    key = be_newstrn(vm, name, namelen);
    /* protect key from GC during subsequent allocations */
    var_setstr(vm->top, key);
    be_stackpush(vm);

    /* skip whitespace between name and value */
    while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
        next(lexer);
    }

    /* check if there's a value (rest of line) */
    if (is_newline(lgetc(lexer)) || lgetc(lexer) == EOS) {
        /* empty define: store integer 1 (truthy) */
        var_setint(&val, 1);
    } else {
        /* read value: collect characters until end of line */
        clear_buf(lexer);
        while (!is_newline(lgetc(lexer)) && lgetc(lexer) != EOS) {
            save(lexer);
        }
        vstart = lexbuf(lexer);
        vlen = (int)lexer->buf.len;

        /* trim trailing whitespace */
        while (vlen > 0 && (vstart[vlen - 1] == ' ' || vstart[vlen - 1] == '\t')) {
            vlen--;
        }

        if (vlen == 0) {
            /* whitespace-only value: treat as empty define */
            var_setint(&val, 1);
        } else {
            /* try to parse as integer */
            bint ival;
            if (pp_parse_int(vstart, vlen, &ival)) {
                var_setint(&val, ival);
            } else {
                /* store as string */
                bstring *s = be_newstrn(vm, vstart, vlen);
                var_setstr(&val, s);
            }
        }
    }

    /* insert or update the macro in the map */
    be_map_insertstr(vm, map, key, &val);
    be_stackpop(vm, 1); /* release GC protection for key */
}

/* Handle #undef MACRO_NAME
 * Parse macro name and remove it from the map.
 */
static void pp_undef(blexer *lexer)
{
    bvm *vm = lexer->vm;
    int namelen;

    /* skip whitespace after #undef */
    while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
        next(lexer);
    }

    /* read macro name */
    namelen = pp_read_ident(lexer);
    if (namelen == 0) {
        be_lexerror(lexer, "macro name expected after '#undef'");
        return;
    }

    /* remove from map if it exists */
    if (vm->preprocessor) {
        bstring *key = be_newstrn(vm, lexbuf(lexer), namelen);
        be_map_removestr(vm, vm->preprocessor, key);
    }

    /* consume rest of line */
    pp_skip_to_eol(lexer);
}

/* Handle #if MACRO_NAME or #if !MACRO_NAME
 * Push a new entry onto ppstack, set active/matched based on truthiness.
 */
static void pp_if(blexer *lexer)
{
    bbool negate = bfalse;
    bbool truthy;
    int namelen;

    /* check nesting depth */
    if (lexer->ppdepth + 1 >= BE_PREPROC_MAX_DEPTH) {
        be_lexerror(lexer, "preprocessor conditional nesting too deep");
        return;
    }

    /* If we are already skipping, just push a dummy inactive entry
     * to track nesting for correct #endif matching. */
    if (pp_skipping(lexer)) {
        lexer->ppdepth++;
        lexer->ppstack[lexer->ppdepth].active = 0;
        lexer->ppstack[lexer->ppdepth].matched = 1; /* prevent elif/else from activating */
        lexer->ppstack[lexer->ppdepth].line = lexer->linenumber;
        pp_skip_to_eol(lexer);
        return;
    }

    /* skip whitespace after #if */
    while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
        next(lexer);
    }

    /* check for negation */
    if (lgetc(lexer) == '!') {
        negate = btrue;
        next(lexer);
        /* skip optional whitespace after ! */
        while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
            next(lexer);
        }
    }

    /* read macro name */
    namelen = pp_read_ident(lexer);
    if (namelen == 0) {
        be_lexerror(lexer, "macro name expected after '#if'");
        return;
    }

    /* evaluate truthiness */
    truthy = pp_macro_truthy(lexer, lexbuf(lexer), namelen);
    if (negate) {
        truthy = !truthy;
    }

    /* push new conditional entry */
    lexer->ppdepth++;
    lexer->ppstack[lexer->ppdepth].active = truthy ? 1 : 0;
    lexer->ppstack[lexer->ppdepth].matched = truthy ? 1 : 0;
    lexer->ppstack[lexer->ppdepth].line = lexer->linenumber;

    /* consume rest of line */
    pp_skip_to_eol(lexer);
}

/* Handle #elif MACRO_NAME or #elif !MACRO_NAME
 * Evaluate condition only if no prior branch in this group matched.
 */
static void pp_elif(blexer *lexer)
{
    bbool negate = bfalse;
    bbool truthy;
    int namelen;

    if (lexer->ppdepth < 0) {
        be_lexerror(lexer, "'#elif' without matching '#if'");
        return;
    }

    /* if a prior branch already matched, this branch is inactive */
    if (lexer->ppstack[lexer->ppdepth].matched) {
        lexer->ppstack[lexer->ppdepth].active = 0;
        pp_skip_to_eol(lexer);
        return;
    }

    /* skip whitespace after #elif */
    while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
        next(lexer);
    }

    /* check for negation */
    if (lgetc(lexer) == '!') {
        negate = btrue;
        next(lexer);
        while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
            next(lexer);
        }
    }

    /* read macro name */
    namelen = pp_read_ident(lexer);
    if (namelen == 0) {
        be_lexerror(lexer, "macro name expected after '#elif'");
        return;
    }

    /* evaluate truthiness */
    truthy = pp_macro_truthy(lexer, lexbuf(lexer), namelen);
    if (negate) {
        truthy = !truthy;
    }

    lexer->ppstack[lexer->ppdepth].active = truthy ? 1 : 0;
    if (truthy) {
        lexer->ppstack[lexer->ppdepth].matched = 1;
    }

    /* consume rest of line */
    pp_skip_to_eol(lexer);
}

/* Handle #else
 * Toggle active state if no prior branch matched.
 * Report error on duplicate #else or missing #if.
 */
static void pp_else(blexer *lexer)
{
    if (lexer->ppdepth < 0) {
        be_lexerror(lexer, "'#else' without matching '#if'");
        return;
    }

    if (lexer->ppstack[lexer->ppdepth].matched) {
        /* a prior branch already matched — skip this #else block */
        lexer->ppstack[lexer->ppdepth].active = 0;
    } else {
        /* no prior branch matched — activate this block */
        lexer->ppstack[lexer->ppdepth].active = 1;
        lexer->ppstack[lexer->ppdepth].matched = 1;
    }

    /* consume rest of line */
    pp_skip_to_eol(lexer);
}

/* Handle #endif
 * Pop the conditional stack entry.
 */
static void pp_endif(blexer *lexer)
{
    if (lexer->ppdepth < 0) {
        be_lexerror(lexer, "'#endif' without matching '#if'");
        return;
    }

    lexer->ppdepth--;

    /* consume rest of line */
    pp_skip_to_eol(lexer);
}

/* Process a directive line after '#' has been identified at line start.
 * The lexer cursor is positioned at the first character after '#'.
 * Returns btrue if a valid directive was processed, bfalse if the '#'
 * should be treated as a comment (unrecognized keyword).
 */
static bbool pp_process_directive(blexer *lexer)
{
    int len;
    const char *kw;

    /* The character immediately after '#' must be a letter (start of
     * a directive keyword).  If it is a space, tab, '-', or anything
     * else, the '#' is a regular Berry comment — return bfalse so the
     * caller falls through to skip_comment(). */
    if (lgetc(lexer) < 'a' || lgetc(lexer) > 'z') {
        return bfalse;
    }

    /* read the directive keyword */
    len = pp_read_ident(lexer);
    if (len == 0) {
        return bfalse; /* bare '#' or '#' followed by non-ident → comment */
    }
    kw = lexbuf(lexer);

    /* match against recognized directive keywords */
    if (len == 2 && kw[0] == 'i' && kw[1] == 'f') {
        pp_if(lexer);
    } else if (len == 4 && !strncmp(kw, "elif", 4)) {
        pp_elif(lexer);
    } else if (len == 4 && !strncmp(kw, "else", 4)) {
        pp_else(lexer);
    } else if (len == 5 && !strncmp(kw, "endif", 5)) {
        pp_endif(lexer);
    } else if (len == 6 && !strncmp(kw, "define", 6)) {
        if (!pp_skipping(lexer)) {
            pp_define(lexer);
        } else {
            pp_skip_to_eol(lexer);
        }
    } else if (len == 5 && !strncmp(kw, "undef", 5)) {
        if (!pp_skipping(lexer)) {
            pp_undef(lexer);
        } else {
            pp_skip_to_eol(lexer);
        }
    } else {
        /* unrecognized keyword → treat '#' as comment */
        return bfalse;
    }

    return btrue;
}

/* Handle a '$IDENT"..."', '$IDENT\'...\'', or '$IDENT' translatable
 * string expression.  Called from the top-level dispatch in
 * lexer_next() after '$' has been consumed.  The cursor is positioned
 * on the character immediately following '$'.
 *
 * Behaviour:
 *   - '$IDENT"default"' (or '$IDENT\'default\''): if IDENT is defined
 *     as a string macro, the macro's value replaces the default text;
 *     otherwise the default text is emitted.  Adjacent string
 *     literals that follow (e.g. '$IDENT"hi" " folks"') are
 *     concatenated to the resolved value, matching Berry's normal
 *     string-literal concatenation.
 *   - '$IDENT' with no following quote: if IDENT is defined as a
 *     string macro, its value is emitted as a string token; otherwise
 *     a "stray '$' in program" syntax error is raised.
 *
 * Reports "stray '$' in program" if '$' is not followed by an
 * identifier.  Returns TokenString on success.
 */
static btokentype pp_scan_translatable(blexer *lexer)
{
    int namelen;
    int quote;
    bvalue *val;
    bstring *name, *head;
    bvm *vm = lexer->vm;

    /* The character after '$' must be an identifier-start character. */
    if (!is_ident_start(lgetc(lexer))) {
        be_lexerror(lexer, "stray '$' in program");
        return TokenNone;
    }

    /* Read the macro name into the lexer buffer. */
    namelen = pp_read_ident(lexer);

    /* Intern the macro name and keep it GC-protected on the stack. */
    name = be_newstrn(vm, lexbuf(lexer), namelen);
    var_setstr(vm->top, name);
    be_stackpush(vm);

    val = pp_find_macro(lexer, str(name), str_len(name));

    quote = lgetc(lexer);
    if (quote != '\'' && quote != '"') {
        /* '$IDENT' with no quote: emit the macro's string value if
         * defined as a string, otherwise raise stray '$'. */
        if (val && var_isstr(val)) {
            setstr(lexer, var_tostr(val));
            be_stackpop(vm, 1);
            return TokenString;
        }
        be_stackpop(vm, 1);
        be_lexerror(lexer, "stray '$' in program");
        return TokenNone;
    }

    /* Scan the default-text literal, process escapes, and choose
     * either its processed text or the macro's replacement as the
     * head of the resulting string. */
    clear_buf(lexer);
    scan_one_literal(lexer);
    tr_string(lexer);
    head = (val && var_isstr(val)) ? var_tostr(val) : buf_tostr(lexer);

    /* Keep the head string alive across further allocations. */
    var_setstr(vm->top, head);
    be_stackpush(vm);

    /* Pick up any adjacent string literals, processing each one's
     * escapes independently, and concatenate them via be_strcat. */
    skip_delimiter(lexer);
    while (lgetc(lexer) == '"' || lgetc(lexer) == '\'') {
        bstring *seg;
        clear_buf(lexer);
        scan_one_literal(lexer);
        tr_string(lexer);
        seg = buf_tostr(lexer);
        head = be_strcat(vm, var_tostr(vm->top - 1), seg);
        var_setstr(vm->top - 1, head); /* update in-place on stack */
        skip_delimiter(lexer);
    }

    setstr(lexer, var_tostr(vm->top - 1));
    be_stackpop(vm, 2); /* release head and macro name */
    return TokenString;
}

#endif /* BE_USE_PREPROCESSOR */

static btokentype lexer_next(blexer *lexer)
{
    for (;;) {
#if BE_USE_PREPROCESSOR
        /* Skipping mode: when inside a false conditional block,
         * consume all characters except #directives at line start.
         * We must still count newlines for accurate line numbers
         * and track nested #if/#endif for correct matching. */
        while (pp_skipping(lexer)) {
            int c = lgetc(lexer);
            if (c == EOS) {
                break; /* let the switch/EOS case handle end-of-source */
            }
            if (is_newline(c)) {
                skip_newline(lexer);
                /* after newline, check if next non-whitespace is '#' */
                while (lgetc(lexer) == ' ' || lgetc(lexer) == '\t') {
                    next(lexer);
                }
                if (lgetc(lexer) == '#') {
                    next(lexer); /* consume '#' */
                    if (!pp_process_directive(lexer)) {
                        /* unrecognized directive keyword while skipping:
                         * treat as comment, skip rest of line */
                        pp_skip_to_eol(lexer);
                    }
                    clear_buf(lexer); /* reset buffer after directive in skip mode */
                }
                continue;
            }
            next(lexer); /* consume non-newline character */
        }
#endif
        /* Check if skip_delimiter already consumed '#' for a directive */
#if BE_USE_PREPROCESSOR
        if (lexer->pp_hash_consumed) {
            lexer->pp_hash_consumed = bfalse;
            /* '#' was already consumed by skip_delimiter; cursor is on
             * the first letter of the directive keyword.  Jump directly
             * to directive processing (same as case '#' with
             * pp_at_line_start, but without consuming '#' again). */
            if (lgetc(lexer) == '-') {
                /* block comment #- ... -# */
                int lno = lexer->linenumber;
                int mark, c = 'x';
                do {
                    mark = c == '-';
                    if (is_newline(c)) {
                        c = skip_newline(lexer);
                        continue;
                    }
                    c = next(lexer);
                } while (!(mark && c == '#') && c != EOS);
                if (c == EOS) {
                    be_lexerror(lexer, be_pushfstring(lexer->vm,
                        "unterminated comment block started in line %d", lno));
                }
                next(lexer); /* skip trailing '#' */
            } else if (pp_process_directive(lexer)) {
                lexer->pp_at_line_start = bfalse;
                clear_buf(lexer);
            } else {
                pp_skip_to_eol(lexer);
                clear_buf(lexer);
            }
            lexer->had_whitespace = 1;
            continue; /* re-enter the for(;;) loop */
        }
#endif
        switch (lgetc(lexer)) {
        case '\r': case '\n': /* newline */
            skip_newline(lexer);
            lexer->had_whitespace = 1;
            break;
        case ' ': case '\t': case '\f': case '\v': /* spaces */
            next(lexer);
            lexer->had_whitespace = 1;
            break;
        case '#': /* comment or preprocessor directive */
#if BE_USE_PREPROCESSOR
            if (lexer->pp_at_line_start) {
                next(lexer); /* consume '#' */
                /* Block comment #- must be checked before directive parsing,
                 * since pp_process_directive would consume the whitespace. */
                if (lgetc(lexer) == '-') {
                    /* block comment #- ... -# : replicate skip_comment logic
                     * (the '#' is already consumed) */
                    int lno = lexer->linenumber;
                    int mark, c = 'x';
                    do {
                        mark = c == '-';
                        if (is_newline(c)) {
                            c = skip_newline(lexer);
                            continue;
                        }
                        c = next(lexer);
                    } while (!(mark && c == '#') && c != EOS);
                    if (c == EOS) {
                        be_lexerror(lexer, be_pushfstring(lexer->vm,
                            "unterminated comment block started in line %d", lno));
                    }
                    next(lexer); /* skip trailing '#' */
                } else if (pp_process_directive(lexer)) {
                    /* directive recognized and handled, no token emitted */
                    lexer->pp_at_line_start = bfalse;
                    clear_buf(lexer); /* reset buffer after directive processing */
                } else {
                    /* unrecognized keyword after '#' — treat as line comment */
                    pp_skip_to_eol(lexer);
                    clear_buf(lexer); /* reset buffer after skipping comment */
                }
                lexer->had_whitespace = 1;
                break;
            }
#endif
            skip_comment(lexer);
            lexer->had_whitespace = 1;
            break;
        case EOS: /* end of source stream */
#if BE_USE_PREPROCESSOR
            if (lexer->ppdepth >= 0) {
                be_lexerror(lexer, be_pushfstring(lexer->vm,
                    "unfinished conditional block started in line %d",
                    lexer->ppstack[lexer->ppdepth].line));
            }
#endif
            return TokenEOS;
        /* operator */
        case '+': return scan_assign(lexer, OptAddAssign, OptAdd);
        case '-': return scan_sub(lexer);
        case '*': return scan_assign(lexer, OptMulAssign, OptMul);
        case '/': return scan_assign(lexer, OptDivAssign, OptDiv);
        case '%': return scan_assign(lexer, OptModAssign, OptMod);
        case '(': next(lexer); return lexer->had_whitespace ? OptSpaceLBK : OptCallLBK;
        case ')': next(lexer); return OptRBK;
        case '[': next(lexer); return OptLSB;
        case ']': next(lexer); return OptRSB;
        case '{': next(lexer); return OptLBR;
        case '}': next(lexer); return OptRBR;
        case ',': next(lexer); return OptComma;
        case ';': next(lexer); return OptSemic;
        case ':':
            next(lexer);
            return check_next(lexer, '=') ? OptWalrus : OptColon;
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
        {
            btokentype type = scan_string(lexer);
            return type;
        }
#if BE_USE_PREPROCESSOR
        case '$':
            /* Translatable string expression: $IDENT"..." or $IDENT'...'.
             * Consume the '$' and dispatch to pp_scan_translatable(),
             * which reads the identifier, scans the string literal, and
             * substitutes the macro's replacement value if defined.
             * A stray '$' (not followed by IDENT and a quote) raises
             * a syntax error. */
            next(lexer); /* consume '$' */
            lexer->pp_at_line_start = bfalse;
            return pp_scan_translatable(lexer);
#endif
        case '.':
            return scan_dot_real(lexer);
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return scan_numeral(lexer);
        default:
            if (is_ident_start(lgetc(lexer))) {
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
    lexer->had_whitespace = 1; /* start with whitespace state */
#if BE_USE_PREPROCESSOR
    lexer->ppdepth = -1;           /* no conditional active */
    lexer->pp_at_line_start = btrue;
    lexer->pp_translatable_ready = bfalse;
    lexer->pp_hash_consumed = bfalse;
#endif
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
    lexer->had_whitespace = 0; /* reset whitespace flag before scanning */
    type = lexer_next(lexer);
    clear_buf(lexer);
    if (type != TokenNone) {
        lexer->token.type = type;
#if BE_USE_PREPROCESSOR
        /* Clear pp_at_line_start after emitting a token, UNLESS
         * the cursor is sitting on '#' with the flag still true.
         * This happens when skip_delimiter (inside scan_string)
         * broke out at a '#' that could be a preprocessor directive.
         * We must preserve the flag so the next lexer_next call
         * recognizes the '#' as a directive, not a comment. */
        if (type != TokenEOS && !(lexer->pp_at_line_start && (lgetc(lexer) == '#' || lexer->pp_hash_consumed))) {
            lexer->pp_at_line_start = bfalse;
        }
#endif
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
        return token_strings[token->type];
    }
}

const char* be_tokentype2str(btokentype type)
{
    return token_strings[type];
}

#endif
