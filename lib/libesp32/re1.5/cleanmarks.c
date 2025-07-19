// Copyright 2014 Paul Sokolovsky.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

void
cleanmarks(ByteProg *prog)
{
    char *pc = prog->insts;
    char *end = pc + prog->bytelen;
    while (pc < end) {
        *pc &= 0x7f;
        switch (*pc) {
        case Class:
        case ClassNot:
            pc += (unsigned char)pc[1] * 2;
        case NamedClass:
        case Jmp:
        case Split:
        case RSplit:
        case Save:
        case Char:
            pc++;
            break;
#ifdef DEBUG
        case Bol:
        case Eol:
        case Any:
        case Match:
            break;
        default:
            printf("Unknown instruction 0x%02x pc %ld\n", (unsigned char)*pc, pc - prog->insts);
            re1_5_fatal("cleanmarks");
#endif
        }
        pc++;
    }
}
