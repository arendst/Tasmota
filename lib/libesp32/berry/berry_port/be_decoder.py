#/********************************************************************
#** Copyright (c) 2018-2020 Guan Wenliang
#** This file is part of the Berry default interpreter.
#** skiars@qq.com, https://github.com/Skiars/berry
#** See Copyright Notice in the LICENSE file or at
#** https://github.com/Skiars/berry/blob/master/LICENSE
#********************************************************************/
#
# Python port of src/be_decoder.h and src/be_opcodes.h
# All C macro code is included as comments.
# C function/macro names are preserved as Python function names.

###############################################################################
# Constants
###############################################################################

# #define NO_JUMP  -1
NO_JUMP = -1

###############################################################################
# Field bit-widths
###############################################################################

# #define IOP_BITS   6u
# #define IRA_BITS   8u
# #define IRKB_BITS  9u
# #define IRKC_BITS  9u
IOP_BITS  = 6
IRA_BITS  = 8
IRKB_BITS = 9
IRKC_BITS = 9

###############################################################################
# Field positions
###############################################################################

# #define IRKC_POS  0u
# #define IRKB_POS  (IRKC_POS + IRKC_BITS)
# #define IRA_POS   (IRKB_POS + IRKB_BITS)
# #define IOP_POS   (IRA_POS + IRA_BITS)
# #define IAx_BITS  (IRA_BITS + IRKB_BITS + IRKC_BITS)
# #define IBx_BITS  (IRKC_BITS + IRKB_BITS)
IRKC_POS = 0
IRKB_POS = IRKC_POS + IRKC_BITS   # 9
IRA_POS  = IRKB_POS + IRKB_BITS   # 18
IOP_POS  = IRA_POS + IRA_BITS     # 26
IAx_BITS = IRA_BITS + IRKB_BITS + IRKC_BITS  # 26
IBx_BITS = IRKC_BITS + IRKB_BITS              # 18

###############################################################################
# Masks and limits
###############################################################################

# #define INS_MASK(pos, bits)  ((binstruction)((1 << (bits)) - 1) << (pos))
def INS_MASK(pos, bits):
    return ((1 << bits) - 1) << pos

# #define IOP_MASK   INS_MASK(IOP_POS, IOP_BITS)
# #define IRA_MASK   INS_MASK(IRA_POS, IRA_BITS)
# #define IRKB_MASK  INS_MASK(IRKB_POS, IRKB_BITS)
# #define IRKC_MASK  INS_MASK(IRKC_POS, IRKC_BITS)
# #define IAx_MASK   INS_MASK(0, IAx_BITS)
# #define IBx_MASK   INS_MASK(0, IBx_BITS)
# #define IsBx_MAX   cast_int(IBx_MASK >> 1)
# #define IsBx_MIN   cast_int(-IsBx_MAX - 1)
IOP_MASK  = INS_MASK(IOP_POS, IOP_BITS)
IRA_MASK  = INS_MASK(IRA_POS, IRA_BITS)
IRKB_MASK = INS_MASK(IRKB_POS, IRKB_BITS)
IRKC_MASK = INS_MASK(IRKC_POS, IRKC_BITS)
IAx_MASK  = INS_MASK(0, IAx_BITS)
IBx_MASK  = INS_MASK(0, IBx_BITS)
IsBx_MAX  = IBx_MASK >> 1   # 131071
IsBx_MIN  = -IsBx_MAX - 1   # -131072

# #define KR_MASK  ((1 << (IRKB_BITS-1)) - 1)
KR_MASK = (1 << (IRKB_BITS - 1)) - 1  # 0xFF

###############################################################################
# Basic field operations
###############################################################################

# #define INS_GETx(i, mask, pos)  cast_int(((binstruction)(i) & (mask)) >> (pos))
def INS_GETx(i, mask, pos):
    return ((i & 0xFFFFFFFF) & mask) >> pos

# #define INS_SETx(v, mask, pos)  (((binstruction)(v) << (pos)) & (mask))
def INS_SETx(v, mask, pos):
    # Cast to uint32 first to match C's (binstruction)(v) cast on negative values
    return (((v & 0xFFFFFFFF) << pos) & mask) & 0xFFFFFFFF

###############################################################################
# Instruction field getters
###############################################################################

# #define IGET_OP(i)   cast(bopcode, INS_GETx(i, IOP_MASK, IOP_POS))
def IGET_OP(i):
    return INS_GETx(i, IOP_MASK, IOP_POS)

# #define IGET_RA(i)   INS_GETx(i, IRA_MASK, IRA_POS)
def IGET_RA(i):
    return INS_GETx(i, IRA_MASK, IRA_POS)

# #define IGET_RKB(i)  INS_GETx(i, IRKB_MASK, IRKB_POS)
def IGET_RKB(i):
    return INS_GETx(i, IRKB_MASK, IRKB_POS)

# #define IGET_RKC(i)  INS_GETx(i, IRKC_MASK, IRKC_POS)
def IGET_RKC(i):
    return INS_GETx(i, IRKC_MASK, IRKC_POS)

# #define IGET_Bx(i)   INS_GETx(i, IBx_MASK, 0)
def IGET_Bx(i):
    return INS_GETx(i, IBx_MASK, 0)

# #define IGET_sBx(i)  (IGET_Bx(i) - IsBx_MAX)
def IGET_sBx(i):
    return IGET_Bx(i) - IsBx_MAX

###############################################################################
# Instruction field setters
###############################################################################

# #define ISET_OP(i)   INS_SETx(i, IOP_MASK, IOP_POS)
def ISET_OP(i):
    return INS_SETx(i, IOP_MASK, IOP_POS)

# #define ISET_RA(i)   INS_SETx(i, IRA_MASK, IRA_POS)
def ISET_RA(i):
    return INS_SETx(i, IRA_MASK, IRA_POS)

# #define ISET_RKB(i)  INS_SETx(i, IRKB_MASK, IRKB_POS)
def ISET_RKB(i):
    return INS_SETx(i, IRKB_MASK, IRKB_POS)

# #define ISET_RKC(i)  INS_SETx(i, IRKC_MASK, IRKC_POS)
def ISET_RKC(i):
    return INS_SETx(i, IRKC_MASK, IRKC_POS)

# #define ISET_Bx(i)   INS_SETx(i, IBx_MASK, 0)
def ISET_Bx(i):
    return INS_SETx(i, IBx_MASK, 0)

# #define ISET_sBx(i)  (ISET_Bx(cast_int(i) + IsBx_MAX))
def ISET_sBx(i):
    return ISET_Bx(int(i) + IsBx_MAX)

###############################################################################
# K-flag helpers (constant vs register discrimination)
###############################################################################

# #define isK(v)     (((v) & (1 << (IRKB_BITS - 1))) != 0)
def isK(v):
    return (v & (1 << (IRKB_BITS - 1))) != 0

# #define setK(v)    ((v) | (1 << (IRKB_BITS - 1)))
def setK(v):
    return v | (1 << (IRKB_BITS - 1))

# #define KR2idx(v)  ((v) & 0xFF)
def KR2idx(v):
    return v & 0xFF

# #define isKB(v)    (((v) & (1 << (IRA_POS - 1))) != 0)
def isKB(v):
    return (v & (1 << (IRA_POS - 1))) != 0

# #define isKC(v)    (((v) & (1 << (IRKB_POS - 1))) != 0)
def isKC(v):
    return (v & (1 << (IRKB_POS - 1))) != 0

###############################################################################
# Opcode enum (from src/be_opcodes.h)
#
# typedef enum {
#     #define OPCODE(opc) OP_##opc
#     #include "be_opcodes.h"
#     #undef OPCODE
# } bopcode;
#
# /* define opcode, don't change order */
# /*  opcode            parameters         description */
# OPCODE(ADD),        /*  A, B, C  |   R(A) <- RK(B) + RK(C) */
# OPCODE(SUB),        /*  A, B, C  |   R(A) <- RK(B) - RK(C) */
# OPCODE(MUL),        /*  A, B, C  |   R(A) <- RK(B) * RK(C) */
# OPCODE(DIV),        /*  A, B, C  |   R(A) <- RK(B) / RK(C) */
# OPCODE(MOD),        /*  A, B, C  |   R(A) <- RK(B) % RK(C) */
# OPCODE(LT),         /*  A, B, C  |   R(A) <- RK(B) < RK(C) */
# OPCODE(LE),         /*  A, B, C  |   R(A) <- RK(B) <= RK(C) */
# OPCODE(EQ),         /*  A, B, C  |   R(A) <- RK(B) == RK(C) */
# OPCODE(NE),         /*  A, B, C  |   R(A) <- RK(B) != RK(C) */
# OPCODE(GT),         /*  A, B, C  |   R(A) <- RK(B) > RK(C) */
# OPCODE(GE),         /*  A, B, C  |   R(A) <- RK(B) >= RK(C) */
# OPCODE(AND),        /*  A, B, C  |   R(A) <- RK(B) & RK(C) */
# OPCODE(OR),         /*  A, B, C  |   R(A) <- RK(B) | RK(C) */
# OPCODE(XOR),        /*  A, B, C  |   R(A) <- RK(B) ^ RK(C) */
# OPCODE(SHL),        /*  A, B, C  |   R(A) <- RK(B) << RK(C) */
# OPCODE(SHR),        /*  A, B, C  |   R(A) <- RK(B) >> RK(C) */
# OPCODE(CONNECT),    /*  A, B, C  |   R(A) <- connect(RK(B), RK(C)) */
# OPCODE(NEG),        /*  A, B     |   R(A) <- -RK(B) */
# OPCODE(FLIP),       /*  A, B     |   R(A) <- ~RK(B) */
# OPCODE(LDNIL),      /*  A        |   R(A) <- nil */
# OPCODE(LDBOOL),     /*  A, B, C  |   R(A) <- cast_bool(B), if(C): pc++ */
# OPCODE(LDINT),      /*  A, sBx   |   R(A) <- sBx */
# OPCODE(LDCONST),    /*  A, Bx    |   R(A) <- K(Bx) */
# OPCODE(MOVE),       /*  A, B, C  |   R(A) <- RK(B) */
# OPCODE(GETGBL),     /*  A, Bx    |   R(A) <- GLOBAL(Bx) */
# OPCODE(SETGBL),     /*  A, Bx    |   R(A) -> GLOBAL(Bx) */
# OPCODE(GETUPV),     /*  A, Bx    |   R(A) <- UPVALUE(Bx) */
# OPCODE(SETUPV),     /*  A, Bx    |   R(A) -> UPVALUE(Bx) */
# OPCODE(JMP),        /*  sBx      |   pc <- pc + sBx */
# OPCODE(JMPT),       /*  A, sBx   |   if(R(A)): pc <- pc + sBx */
# OPCODE(JMPF),       /*  A, sBx   |   if(not R(A)): pc <- pc + sBx */
# OPCODE(CALL),       /*  A, B     |   CALL(R(A), B) */
# OPCODE(RET),        /*  A, B     |   if (R(A)) R(-1) <- RK(B) else R(-1) <- nil */
# OPCODE(CLOSURE),    /*  A, Bx    |   R(A) <- CLOSURE(proto_table[Bx]) */
# OPCODE(GETMBR),     /*  A, B, C  |   R(A) <- RK(B).RK(C) */
# OPCODE(GETMET),     /*  A, B, C  |   R(A) <- RK(B).RK(C), R(A+1) <- RK(B) */
# OPCODE(SETMBR),     /*  A, B, C  |   R(A).RK(B) <- RK(C) */
# OPCODE(GETIDX),     /*  A, B, C  |   R(A) <- RK(B)[RK(C)] */
# OPCODE(SETIDX),     /*  A, B, C  |   R(A)[RK(B)] <- RK(C) */
# OPCODE(SETSUPER),   /*  A, B     |   class:R(A) set super with class:RK(B) */
# OPCODE(CLOSE),      /*  A        |   close upvalues */
# OPCODE(IMPORT),     /*  A, B, C  |   IF (A == C) import module name as RK(B) to RK(A), ELSE from module RK(C) import name as RK(B) to RK(A) */
# OPCODE(EXBLK),      /*  A, Bx    |   ... */
# OPCODE(CATCH),      /*  A, B, C  |   ... */
# OPCODE(RAISE),      /*  A, B, C  |   RAISE(B,C) B is code, C is description. A==0 only B provided, A==1 B and C are provided, A==2 rethrow with both parameters already on stack */
# OPCODE(CLASS),      /*  Bx       |   init class in K[Bx] */
# OPCODE(GETNGBL),    /*  A, B     |   R(A) <- GLOBAL[RK(B)] by name */
# OPCODE(SETNGBL)     /*  A, B     |   R(A) -> GLOBAL[RK(B)] by name */
###############################################################################

OP_ADD      = 0
OP_SUB      = 1
OP_MUL      = 2
OP_DIV      = 3
OP_MOD      = 4
OP_LT       = 5
OP_LE       = 6
OP_EQ       = 7
OP_NE       = 8
OP_GT       = 9
OP_GE       = 10
OP_AND      = 11
OP_OR       = 12
OP_XOR      = 13
OP_SHL      = 14
OP_SHR      = 15
OP_CONNECT  = 16
OP_NEG      = 17
OP_FLIP     = 18
OP_LDNIL    = 19
OP_LDBOOL   = 20
OP_LDINT    = 21
OP_LDCONST  = 22
OP_MOVE     = 23
OP_GETGBL   = 24
OP_SETGBL   = 25
OP_GETUPV   = 26
OP_SETUPV   = 27
OP_JMP      = 28
OP_JMPT     = 29
OP_JMPF     = 30
OP_CALL     = 31
OP_RET      = 32
OP_CLOSURE  = 33
OP_GETMBR   = 34
OP_GETMET   = 35
OP_SETMBR   = 36
OP_GETIDX   = 37
OP_SETIDX   = 38
OP_SETSUPER = 39
OP_CLOSE    = 40
OP_IMPORT   = 41
OP_EXBLK    = 42
OP_CATCH    = 43
OP_RAISE    = 44
OP_CLASS    = 45
OP_GETNGBL  = 46
OP_SETNGBL  = 47

# Total number of opcodes
OP_COUNT = 48

# Opcode name table (for debug/disassembly)
opcode_names = [
    "ADD", "SUB", "MUL", "DIV", "MOD",
    "LT", "LE", "EQ", "NE", "GT", "GE",
    "AND", "OR", "XOR", "SHL", "SHR",
    "CONNECT", "NEG", "FLIP",
    "LDNIL", "LDBOOL", "LDINT", "LDCONST",
    "MOVE",
    "GETGBL", "SETGBL", "GETUPV", "SETUPV",
    "JMP", "JMPT", "JMPF",
    "CALL", "RET", "CLOSURE",
    "GETMBR", "GETMET", "SETMBR",
    "GETIDX", "SETIDX",
    "SETSUPER", "CLOSE", "IMPORT",
    "EXBLK", "CATCH", "RAISE",
    "CLASS",
    "GETNGBL", "SETNGBL",
]
