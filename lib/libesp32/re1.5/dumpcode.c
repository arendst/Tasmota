// Copyright 2014 Paul Sokolovsky.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

void re1_5_dumpcode(ByteProg *prog)
{
    int pc = 0;
    char *code = prog->insts;
    while (pc < prog->bytelen) {
                printf("%2d: ", pc);
                switch(code[pc++]) {
                default:
                        assert(0);
//                        re1_5_fatal("printprog");
                case Split:
                        printf("split %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case RSplit:
                        printf("rsplit %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case Jmp:
                        printf("jmp %d (%d)\n", pc + (signed char)code[pc] + 1, (signed char)code[pc]);
                        pc++;
                        break;
                case Char:
                        printf("char %c\n", code[pc++]);
                        break;
                case Any:
                        printf("any\n");
                        break;
                case Class:
                case ClassNot: {
                        int num = code[pc];
                        printf("class%s %d", (code[pc - 1] == ClassNot ? "not" : ""), num);
                        pc++;
                        while (num--) {
                            printf(" 0x%02x-0x%02x", code[pc], code[pc + 1]);
                            pc += 2;
                        }
                        printf("\n");
                        break;
                }
                case NamedClass:
                        printf("namedclass %c\n", code[pc++]);
                        break;
                case Match:
                        printf("match\n");
                        break;
                case Save:
                        printf("save %d\n", (unsigned char)code[pc++]);
                        break;
                case Bol:
                        printf("assert bol\n");
                        break;
                case Eol:
                        printf("assert eol\n");
                        break;
                }
    }
    printf("Bytes: %d, insts: %d\n", prog->bytelen, prog->len);
}
