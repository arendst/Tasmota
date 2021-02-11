/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_OPCODE_H
#define BE_OPCODE_H

#define NO_JUMP                 -1

/* define field's bit-width  */
#define IOP_BITS                6u
#define IRA_BITS                8u
#define IRKB_BITS               9u
#define IRKC_BITS               9u

/* define field's bit-width and positions */
#define IRKC_POS                0u
#define IRKB_POS                (IRKC_POS + IRKC_BITS)
#define IRA_POS                 (IRKB_POS + IRKB_BITS)
#define IOP_POS                 (IRA_POS + IRA_BITS)
#define IAx_BITS                (IRA_BITS + IRKB_BITS + IRKC_BITS)
#define IBx_BITS                (IRKC_BITS + IRKB_BITS)

/* basic field operation */
#define INS_MASK(pos, bits)     ((binstruction)((1 << (bits)) - 1) << (pos))
#define INS_GETx(i, mask, pos)  cast_int(((binstruction)(i) & (mask)) >> (pos))
#define INS_SETx(v, mask, pos)  (((binstruction)(v) << (pos)) & (mask))

/* instruction operation */
#define isK(v)                  (((v) & (1 << (IRKB_BITS - 1))) != 0)
#define setK(v)                 ((v) | (1 << (IRKB_BITS - 1)))
#define KR2idx(v)               ((v) & 0xFF)
#define isKB(v)                 (((v) & (1 << (IRA_POS - 1))) != 0)
#define isKC(v)                 (((v) & (1 << (IRKB_POS - 1))) != 0)

/* define masks and limit values */
#define IOP_MASK                INS_MASK(IOP_POS, IOP_BITS)
#define IRA_MASK                INS_MASK(IRA_POS, IRA_BITS)
#define IRKB_MASK               INS_MASK(IRKB_POS, IRKB_BITS)
#define IRKC_MASK               INS_MASK(IRKC_POS, IRKC_BITS)
#define IAx_MASK                INS_MASK(0, IAx_BITS)
#define IBx_MASK                INS_MASK(0, IBx_BITS)
#define IsBx_MAX                cast_int(IBx_MASK >> 1)
#define IsBx_MIN                cast_int(-IsBx_MAX - 1)

/* get field */
#define IGET_OP(i)              cast(bopcode, INS_GETx(i, IOP_MASK, IOP_POS))
#define IGET_RA(i)              INS_GETx(i, IRA_MASK, IRA_POS)
#define IGET_RKB(i)             INS_GETx(i, IRKB_MASK, IRKB_POS)
#define IGET_RKC(i)             INS_GETx(i, IRKC_MASK, IRKC_POS)
#define IGET_Bx(i)              INS_GETx(i, IBx_MASK, 0)
#define IGET_sBx(i)             (IGET_Bx(i) - IsBx_MAX)

/* set field */
#define ISET_OP(i)              INS_SETx(i, IOP_MASK, IOP_POS)
#define ISET_RA(i)              INS_SETx(i, IRA_MASK, IRA_POS)
#define ISET_RKB(i)             INS_SETx(i, IRKB_MASK, IRKB_POS)
#define ISET_RKC(i)             INS_SETx(i, IRKC_MASK, IRKC_POS)
#define ISET_Bx(i)              INS_SETx(i, IBx_MASK, 0)
#define ISET_sBx(i)             (ISET_Bx(cast_int(i) + IsBx_MAX))

typedef enum {
    #define OPCODE(opc) OP_##opc
    #include "be_opcodes.h"
    #undef OPCODE
} bopcode;

#endif
