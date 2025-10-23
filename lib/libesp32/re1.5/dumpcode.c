// Copyright 2014 Paul Sokolovsky.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

// TASMOTA SPECIFIC
#ifndef INST_BUF_SIZE
#define INST_BUF_SIZE   96
#endif

void be_writebuffer(const char *buffer, size_t length);

#define be_writestring(s)       be_writebuffer((s), strlen(s))

#define logbuf(...)     snprintf(__lbuf, sizeof(__lbuf), __VA_ARGS__)

#define logfmt(...)                     \
    do {                                \
        char __lbuf[INST_BUF_SIZE];     \
        logbuf(__VA_ARGS__);            \
        be_writestring(__lbuf);         \
    } while (0)

// all `printf` replaced with `logfmt`

void re1_5_dumpcode(ByteProg *prog)
{
    int pc = 0;
    char *code = prog->insts;
    while (pc < prog->bytelen) {
                logfmt("%2d: ", pc);
                switch(code[pc++]) {
                default:
                        assert(0);
//                        re1_5_fatal("printprog");
                case Split:
                        logfmt("split %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case RSplit:
                        logfmt("rsplit %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case Jmp:
                        logfmt("jmp %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case Char:
                        logfmt("char %c\n", code[pc++]);
                        break;
                case Any:
                        logfmt("any\n");
                        break;
                case Class:
                case ClassNot: {
                        int num = code[pc];
                        logfmt("class%s %d", (code[pc - 1] == ClassNot ? "not" : ""), num);
                        pc++;
                        while (num--) {
                            logfmt(" 0x%02x-0x%02x", code[pc], code[pc + 1]);
                            pc += 2;
                        }
                        logfmt("\n");
                        break;
                }
                case NamedClass:
                        logfmt("namedclass %c\n", code[pc++]);
                        break;
                case Match:
                        logfmt("match\n");
                        break;
                case Save:
                        logfmt("save %d\n", (unsigned char)code[pc++]);
                        break;
                case Bol:
                        logfmt("assert bol\n");
                        break;
                case Eol:
                        logfmt("assert eol\n");
                        break;
                }
    }
    logfmt("Bytes: %d, insts: %d\n", prog->bytelen, prog->len);
}
