#include "re1.5.h"

int _re1_5_classmatch(const char *pc, const char *sp)
{
    // pc points to "cnt" byte after opcode
    int is_positive = (pc[-1] == Class);
    int cnt = *pc++;
    while (cnt--) {
        if (*sp >= *pc && *sp <= pc[1]) return is_positive;
        pc += 2;
    }
    return !is_positive;
}

int _re1_5_namedclassmatch(const char *pc, const char *sp)
{
    // pc points to name of class
    int off = (*pc >> 5) & 1;
    if ((*pc | 0x20) == 'd') {
        if (!(*sp >= '0' && *sp <= '9')) {
            off ^= 1;
        }
    } else if ((*pc | 0x20) == 's') {
        if (!(*sp == ' ' || (*sp >= '\t' && *sp <= '\r'))) {
            off ^= 1;
        }
    } else { // w
        if (!((*sp >= 'A' && *sp <= 'Z') || (*sp >= 'a' && *sp <= 'z') || (*sp >= '0' && *sp <= '9') || *sp == '_')) {
            off ^= 1;
        }
    }
    return off;
}
