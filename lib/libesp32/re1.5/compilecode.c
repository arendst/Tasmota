// Copyright 2014-2019 Paul Sokolovsky.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

#define INSERT_CODE(at, num, pc) \
    ((code ? memmove(code + at + num, code + at, pc - at) : (void)0), pc += num)
#define REL(at, to) (to - at - 2)
#define EMIT(at, byte) (code ? (code[at] = byte) : (void)(at))
#define PC (prog->bytelen)

static int _compilecode(const char **re_loc, ByteProg *prog, int sizecode)
{
    const char *re = *re_loc;
    char *code = sizecode ? NULL : prog->insts;
    int start = PC;
    int term = PC;
    int alt_label = 0;

    for (; *re && *re != ')'; re++) {
        switch (*re) {
        case '\\': {
            re++;
            if (!*re) goto syntax_error; // Trailing backslash
            char c = *re | 0x20;
            if (c == 'd' || c == 's' || c == 'w') {
                term = PC;
                EMIT(PC++, NamedClass);
                EMIT(PC++, *re);
                prog->len++;
                break;
            }
            if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')) {
                goto unsupported_escape;
            }
        }
        default:
            term = PC;
            EMIT(PC++, Char);
            EMIT(PC++, *re);
            prog->len++;
            break;
        case '.':
            term = PC;
            EMIT(PC++, Any);
            prog->len++;
            break;
        case '[': {
            int cnt;
            term = PC;
            re++;
            if (*re == '^') {
                EMIT(PC++, ClassNot);
                re++;
            } else {
                EMIT(PC++, Class);
            }
            PC++; // Skip "# of pairs" byte
            prog->len++;
            for (cnt = 0; *re != ']'; re++, cnt++) {
                if (!*re) goto syntax_error;
                if (*re == '\\') {
                    re++;
                    if (!*re) goto syntax_error;
                    if (*re != '\\' && *re != ']') goto unsupported_escape;
                }
                EMIT(PC++, *re);
                if (re[1] == '-' && re[2] != ']') {
                    re += 2;
                }
                EMIT(PC++, *re);
            }
            EMIT(term + 1, cnt);
            break;
        }
        case '(': {
            term = PC;
            int sub;
            int capture = 1;
            re++;
            if (*re == '?') {
                re++;
                if (*re == ':') {
                    capture = 0;
                    re++;
                } else {
                    *re_loc = re;
                    return RE1_5_UNSUPPORTED_SYNTAX;
                }
            }

            if (capture) {
                sub = ++prog->sub;
                EMIT(PC++, Save);
                EMIT(PC++, 2 * sub);
                prog->len++;
            }

            int res = _compilecode(&re, prog, sizecode);
            *re_loc = re;
            if (res < 0) return res;
            if (*re != ')') return RE1_5_SYNTAX_ERROR;

            if (capture) {
                EMIT(PC++, Save);
                EMIT(PC++, 2 * sub + 1);
                prog->len++;
            }

            break;
        }
        case '{':
            *re_loc = re;
            return RE1_5_UNSUPPORTED_SYNTAX;
        case '?':
            if (PC == term) goto syntax_error; // nothing to repeat
            INSERT_CODE(term, 2, PC);
            if (re[1] == '?') {
                EMIT(term, RSplit);
                re++;
            } else {
                EMIT(term, Split);
            }
            EMIT(term + 1, REL(term, PC));
            prog->len++;
            term = PC;
            break;
        case '*':
            if (PC == term) goto syntax_error; // nothing to repeat
            INSERT_CODE(term, 2, PC);
            EMIT(PC, Jmp);
            EMIT(PC + 1, REL(PC, term));
            PC += 2;
            if (re[1] == '?') {
                EMIT(term, RSplit);
                re++;
            } else {
                EMIT(term, Split);
            }
            EMIT(term + 1, REL(term, PC));
            prog->len += 2;
            term = PC;
            break;
        case '+':
            if (PC == term) goto syntax_error; // nothing to repeat
            if (re[1] == '?') {
                EMIT(PC, Split);
                re++;
            } else {
                EMIT(PC, RSplit);
            }
            EMIT(PC + 1, REL(PC, term));
            PC += 2;
            prog->len++;
            term = PC;
            break;
        case '|':
            if (alt_label) {
                EMIT(alt_label, REL(alt_label, PC) + 1);
            }
            INSERT_CODE(start, 2, PC);
            EMIT(PC++, Jmp);
            alt_label = PC++;
            EMIT(start, Split);
            EMIT(start + 1, REL(start, PC));
            prog->len += 2;
            term = PC;
            break;
        case '^':
            EMIT(PC++, Bol);
            prog->len++;
            term = PC;
            break;
        case '$':
            EMIT(PC++, Eol);
            prog->len++;
            term = PC;
            break;
        }
    }

    if (alt_label) {
        EMIT(alt_label, REL(alt_label, PC) + 1);
    }

    *re_loc = re;
    return RE1_5_SUCCESS;

syntax_error:
    *re_loc = re;
    return RE1_5_SYNTAX_ERROR;

unsupported_escape:
    *re_loc = re;
    return RE1_5_UNSUPPORTED_ESCAPE;
}

int re1_5_sizecode(const char *re)
{
    ByteProg dummyprog = {
         // Save 0, Save 1, Match; more bytes for "search" (vs "match") prefix code
        .bytelen = 5 + NON_ANCHORED_PREFIX
    };

    int res = _compilecode(&re, &dummyprog, /*sizecode*/1);
    if (res < 0) return res;
    // If unparsed chars left
    if (*re) return RE1_5_SYNTAX_ERROR;

    return dummyprog.bytelen;
}

int re1_5_compilecode(ByteProg *prog, const char *re)
{
    prog->len = 0;
    prog->bytelen = 0;
    prog->sub = 0;

    // Add code to implement non-anchored operation ("search").
    // For anchored operation ("match"), this code will be just skipped.
    // TODO: Implement search in much more efficient manner
    prog->insts[prog->bytelen++] = RSplit;
    prog->insts[prog->bytelen++] = 3;
    prog->insts[prog->bytelen++] = Any;
    prog->insts[prog->bytelen++] = Jmp;
    prog->insts[prog->bytelen++] = -5;
    prog->len += 3;

    prog->insts[prog->bytelen++] = Save;
    prog->insts[prog->bytelen++] = 0;
    prog->len++;

    int res = _compilecode(&re, prog, /*sizecode*/0);
    if (res < 0) return res;
    // If unparsed chars left
    if (*re) return RE1_5_SYNTAX_ERROR;

    prog->insts[prog->bytelen++] = Save;
    prog->insts[prog->bytelen++] = 1;
    prog->len++;

    prog->insts[prog->bytelen++] = Match;
    prog->len++;

    return RE1_5_SUCCESS;
}

#if 0
int main(int argc, char *argv[])
{
    int pc = 0;
    ByteProg *code = re1_5_compilecode(argv[1]);
    re1_5_dumpcode(code);
}
#endif
