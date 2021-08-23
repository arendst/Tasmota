/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
/* define opcode, don't change order */
/*  opcode            parameters         description */
OPCODE(ADD),        /*  A, B, C  |   R(A) <- RK(B) + RK(C) */
OPCODE(SUB),        /*  A, B, C  |   R(A) <- RK(B) - RK(C) */
OPCODE(MUL),        /*  A, B, C  |   R(A) <- RK(B) * RK(C) */
OPCODE(DIV),        /*  A, B, C  |   R(A) <- RK(B) / RK(C) */
OPCODE(MOD),        /*  A, B, C  |   R(A) <- RK(B) % RK(C) */
OPCODE(LT),         /*  A, B, C  |   R(A) <- RK(B) < RK(C) */
OPCODE(LE),         /*  A, B, C  |   R(A) <- RK(B) <= RK(C) */
OPCODE(EQ),         /*  A, B, C  |   R(A) <- RK(B) == RK(C) */
OPCODE(NE),         /*  A, B, C  |   R(A) <- RK(B) != RK(C) */
OPCODE(GT),         /*  A, B, C  |   R(A) <- RK(B) > RK(C) */
OPCODE(GE),         /*  A, B, C  |   R(A) <- RK(B) >= RK(C) */
OPCODE(AND),        /*  A, B, C  |   R(A) <- RK(B) & RK(C) */
OPCODE(OR),         /*  A, B, C  |   R(A) <- RK(B) | RK(C) */
OPCODE(XOR),        /*  A, B, C  |   R(A) <- RK(B) ^ RK(C) */
OPCODE(SHL),        /*  A, B, C  |   R(A) <- RK(B) << RK(C) */
OPCODE(SHR),        /*  A, B, C  |   R(A) <- RK(B) >> RK(C) */
OPCODE(CONNECT),    /*  A, B, C  |   R(A) <- connect(RK(B), RK(C)) */
OPCODE(NEG),        /*  A, B     |   R(A) <- -RK(B) */
OPCODE(FLIP),       /*  A, B     |   R(A) <- ~RK(B) */
OPCODE(LDNIL),      /*  A        |   R(A) <- nil */
OPCODE(LDBOOL),     /*  A, B, C  |   R(A) <- cast_bool(B), if(C): pc++ */
OPCODE(LDINT),      /*  A, sBx   |   R(A) <- sBx */
OPCODE(LDCONST),    /*  A, Bx    |   R(A) <- K(Bx) */
OPCODE(MOVE),       /*  A, B, C  |   R(A) <- RK(B) */
OPCODE(GETGBL),     /*  A, Bx    |   R(A) <- GLOBAL(Bx) */
OPCODE(SETGBL),     /*  A, Bx    |   R(A) -> GLOBAL(Bx) */
OPCODE(GETUPV),     /*  A, Bx    |   R(A) <- UPVALUE(Bx)*/
OPCODE(SETUPV),     /*  A, Bx    |   R(A) -> UPVALUE(Bx)*/
OPCODE(JMP),        /*  sBx      |   pc <- pc + sBx */
OPCODE(JMPT),       /*  A, sBx   |   if(R(A)): pc <- pc + sBx  */
OPCODE(JMPF),       /*  A, sBx   |   if(not R(A)): pc <- pc + sBx  */
OPCODE(CALL),       /*  A, B     |   CALL(R(A), B) */
OPCODE(RET),        /*  A, B     |   if (R(A)) R(-1) <- RK(B) else R(-1) <- nil */
OPCODE(CLOSURE),    /*  A, Bx    |   R(A) <- CLOSURE(proto_table[Bx])*/
OPCODE(GETMBR),     /*  A, B, C  |   R(A) <- RK(B).RK(C) */
OPCODE(GETMET),     /*  A, B, C  |   R(A) <- RK(B).RK(C), R(A+1) <- RK(B) */
OPCODE(SETMBR),     /*  A, B, C  |   R(A).RK(B) <- RK(C) */
OPCODE(GETIDX),     /*  A, B, C  |   R(A) <- RK(B)[RK(C)] */
OPCODE(SETIDX),     /*  A, B, C  |   R(A)[RK(B)] <- RK(C) */
OPCODE(SETSUPER),   /*  A, B     |   class:R(A) set super with class:RK(B) */
OPCODE(CLOSE),      /*  A        |   close upvalues */
OPCODE(IMPORT),     /*  A, B, C  |   IF (A == C) import module name as RK(B) to RK(A), ELSE from module RK(C) import name as RK(B) to RK(A) */
OPCODE(EXBLK),      /*  A, Bx    |   ... */
OPCODE(CATCH),      /*  A, B, C  |   ... */
OPCODE(RAISE),      /*  A, B, C  |   RAISE(B,C) B is code, C is description. A==0 only B provided, A==1 B and C are provided, A==2 rethrow with both parameters already on stack */
OPCODE(CLASS),      /*  Bx       |   init class in K[Bx] */
OPCODE(GETNGBL),    /*  A, B     |   R(A) <- GLOBAL[RK(B)] by name */
OPCODE(SETNGBL)     /*  A, B     |   R(A) -> GLOBAL[RK(B)] by name */
