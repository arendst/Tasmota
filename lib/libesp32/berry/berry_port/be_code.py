"""
Berry bytecode code generator.
Mirrors: src/be_code.c / src/be_code.h

Emits binstruction values into the current function's code vector.
Manages register allocation, constant table, jump patching, and
expression descriptors.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_decoder import (
    NO_JUMP,
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx, IGET_sBx,
    ISET_OP, ISET_RA, ISET_RKB, ISET_RKC, ISET_Bx, ISET_sBx,
    isK, setK,
    IRA_MASK, IBx_MASK, IsBx_MAX, IsBx_MIN,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
    OP_CONNECT, OP_NEG, OP_FLIP, OP_LDNIL,
    OP_LDBOOL, OP_LDINT, OP_LDCONST, OP_MOVE,
    OP_GETGBL, OP_SETGBL, OP_GETUPV, OP_SETUPV,
    OP_JMP, OP_JMPT, OP_JMPF,
    OP_CALL, OP_RET, OP_CLOSURE,
    OP_GETMBR, OP_GETMET, OP_SETMBR,
    OP_GETIDX, OP_SETIDX,
    OP_SETSUPER, OP_CLOSE, OP_IMPORT,
    OP_EXBLK, OP_CATCH, OP_RAISE, OP_CLASS,
    OP_GETNGBL, OP_SETNGBL,
)
from berry_port.be_object import (
    bvalue, bproto, bclosure, bclass,
    BE_NIL, BE_INT, BE_REAL, BE_STRING, BE_CLASS,
    var_setnil, var_isint, var_isreal, var_isstr,
    var_setclass,
)
from berry_port.be_vector import (
    be_vector_push_c, be_vector_data, be_vector_end,
    be_vector_count, be_vector_capacity, be_vector_at,
    be_vector_isempty,
)
from berry_port.be_string import be_eqstr
from berry_port.be_object import be_list_count
from berry_port.be_lexer import be_lexerror
from berry_port.berry_conf import (
    BE_USE_SCRIPT_COMPILER,
    BE_DEBUG_RUNTIME_INFO,
    BE_CONST_SEARCH_SIZE,
)

# Import expression/operator types from parser
from berry_port.be_parser import (
    ETVOID, ETNIL, ETBOOL, ETREAL, ETINT, ETSTRING, ETPROTO,
    ETCONST, ETLOCAL, ETGLOBAL, ETUPVAL, ETMEMBER, ETINDEX,
    ETREG, ETNGLOBAL,
    BLOCK_LOOP, BLOCK_EXCEPT,
    OptAdd, OptSub, OptMul, OptDiv, OptMod,
    OptLT, OptLE, OptEQ, OptNE, OptGT, OptGE,
    OptBitAnd, OptBitOr, OptBitXor, OptShiftL, OptShiftR,
    OptConnect, OptAnd, OptOr,
    OptNot, OptFlip,
    bexpdesc, bblockinfo, bfuncinfo,
)

# ============================================================================
# Constants (from be_code.c)
# ============================================================================

# #define NOT_MASK                (1 << 0)
# #define NOT_EXPR                (1 << 1)
# #define FUNC_RET_FLAG           (1 << 0)
NOT_MASK      = (1 << 0)
NOT_EXPR      = (1 << 1)
FUNC_RET_FLAG = (1 << 0)

# C booleans
btrue  = 1
bfalse = 0


# ============================================================================
# Helper macros (from be_code.c)
# ============================================================================

# #define isset(v, mask)          (((v) & (mask)) != 0)
def isset(v, mask):
    return (v & mask) != 0

# #define notexpr(e)              isset((e)->not, NOT_EXPR)
def notexpr(e):
    return isset(e.not_, NOT_EXPR)

# #define notmask(e)              isset((e)->not, NOT_MASK)
def notmask(e):
    return isset(e.not_, NOT_MASK)

# #define hasjump(e)              ((e)->t != (e)->f || notexpr(e))
def hasjump(e):
    return (e.t != e.f) or notexpr(e)

# #define jumpboolop(e, b)        ((b) != notmask(e) ? OP_JMPT : OP_JMPF)
def jumpboolop(e, b):
    return OP_JMPT if (b != notmask(e)) else OP_JMPF

# #define var_toidx(e)            ((e)->v.i)
def var_toidx(e):
    return e.v.i


# ============================================================================
# be_code_freeregs — macro in C header
# ============================================================================

# #define be_code_freeregs(f, n)  ((f)->freereg -= (bbyte)(n))
def be_code_freeregs(finfo, n):
    finfo.freereg -= n



# ============================================================================
# Guard: only compile if script compiler is enabled
# ============================================================================

if BE_USE_SCRIPT_COMPILER:

    # ========================================================================
    # codelineinfo — debug line info tracking
    # ========================================================================

    if BE_DEBUG_RUNTIME_INFO:
        # static void codelineinfo(bfuncinfo *finfo)
        # {
        #     bvector *vec = &finfo->linevec;
        #     int line = finfo->lexer->lastline;
        #     blineinfo *li = be_vector_end(vec);
        #     if (be_vector_isempty(vec) || li->linenumber != line) {
        #         be_vector_push(finfo->lexer->vm, vec, NULL);
        #         li = be_vector_end(vec);
        #         li->endpc = finfo->pc;
        #         li->linenumber = line;
        #         finfo->proto->lineinfo = be_vector_data(vec);
        #         finfo->proto->nlineinfo = be_vector_capacity(vec);
        #     } else {
        #         li->endpc = finfo->pc;
        #     }
        # }
        def codelineinfo(finfo):
            from berry_port.be_object import blineinfo as _blineinfo
            vec = finfo.linevec
            line = finfo.lexer.lastline
            li = be_vector_end(vec)
            if li is None or li.linenumber != line:
                be_vector_push_c(finfo.lexer.vm, vec, None)
                li = be_vector_end(vec)
                if li is None:
                    li = _blineinfo()
                    vec.data[vec.end] = li
                li.endpc = finfo.pc
                li.linenumber = line
                finfo.proto.lineinfo = be_vector_data(vec)
                finfo.proto.nlineinfo = be_vector_capacity(vec)
            else:
                li.endpc = finfo.pc
    else:
        def codelineinfo(finfo):
            pass

    # ========================================================================
    # codeinst — Add new instruction in the code vector
    # ========================================================================

    # static int codeinst(bfuncinfo *finfo, binstruction ins)
    # {
    #     /* put new instruction in code array */
    #     be_vector_push_c(finfo->lexer->vm, &finfo->code, &ins);
    #     finfo->proto->code = be_vector_data(&finfo->code);
    #     finfo->proto->codesize = be_vector_capacity(&finfo->code);
    #     codelineinfo(finfo);
    #     return finfo->pc++;
    # }
    def codeinst(finfo, ins):
        """Add new instruction in the code vector."""
        ins = ins & 0xFFFFFFFF  # ensure uint32
        be_vector_push_c(finfo.lexer.vm, finfo.code, ins)
        finfo.proto.code = be_vector_data(finfo.code)
        finfo.proto.codesize = be_vector_capacity(finfo.code)
        codelineinfo(finfo)
        pc = finfo.pc
        finfo.pc += 1
        return pc

    # ========================================================================
    # codeABC / codeABx — instruction encoding helpers
    # ========================================================================

    # static int codeABC(bfuncinfo *finfo, bopcode op, int a, int b, int c)
    # {
    #     return codeinst(finfo, ISET_OP(op)
    #         | ISET_RA(a) | ISET_RKB(b) | ISET_RKC(c));
    # }
    def codeABC(finfo, op, a, b, c):
        return codeinst(finfo, ISET_OP(op) | ISET_RA(a) | ISET_RKB(b) | ISET_RKC(c))

    # static int codeABx(bfuncinfo *finfo, bopcode op, int a, int bx)
    # {
    #     return codeinst(finfo, ISET_OP(op) | ISET_RA(a) | ISET_Bx(bx));
    # }
    def codeABx(finfo, op, a, bx):
        return codeinst(finfo, ISET_OP(op) | ISET_RA(a) | ISET_Bx(bx))

    # ========================================================================
    # code_move — Move value from register b to register a
    # ========================================================================

    # /* Move value from register b to register a */
    # /* Check the previous instruction to compact both instruction as one if possible */
    # /* If b is a constant, add LDCONST or add MOVE otherwise */
    # /* returns false if the move operation happened, or true if there was a register
    #    optimization and `b` should be replaced by `a` */
    # static bbool code_move(bfuncinfo *finfo, int a, int b)
    # {
    #     if (finfo->pc > finfo->binfo->lastjmp) {
    #         binstruction *i = be_vector_end(&finfo->code);
    #         bopcode op = IGET_OP(*i);
    #         if (op <= OP_LDNIL) { /* binop or unop */
    #             int x = IGET_RA(*i), y = IGET_RKB(*i), z = IGET_RKC(*i);
    #             if (b == x && (a == y || (op < OP_NEG && a == z))) {
    #                 *i = (*i & ~IRA_MASK) | ISET_RA(a);
    #                 return btrue;
    #             }
    #         }
    #         if (!isK(b)):
    #             binstruction mov = ISET_OP(OP_MOVE) | ISET_RA(a) | ISET_RKB(b) | ISET_RKC(0);
    #             if (mov == *i) {
    #                 return btrue;
    #             }
    #     }
    #     if (isK(b)) {
    #         codeABx(finfo, OP_LDCONST, a, b & 0xFF);
    #     } else {
    #         codeABC(finfo, OP_MOVE, a, b, 0);
    #     }
    #     return bfalse;
    # }
    def code_move(finfo, a, b):
        """Move value from register b to register a with peephole optimization."""
        if finfo.pc > finfo.binfo.lastjmp:
            i_val = be_vector_end(finfo.code)
            if i_val is not None:
                op = IGET_OP(i_val)
                if op <= OP_LDNIL:  # binop or unop
                    x = IGET_RA(i_val)
                    y = IGET_RKB(i_val)
                    z = IGET_RKC(i_val)
                    if b == x and (a == y or (op < OP_NEG and a == z)):
                        new_val = (i_val & ~IRA_MASK) | ISET_RA(a)
                        new_val = new_val & 0xFFFFFFFF
                        finfo.code.data[finfo.code.end] = new_val
                        return btrue
                if not isK(b):
                    mov = (ISET_OP(OP_MOVE) | ISET_RA(a) | ISET_RKB(b) | ISET_RKC(0)) & 0xFFFFFFFF
                    if mov == i_val:
                        return btrue
        if isK(b):
            codeABx(finfo, OP_LDCONST, a, b & 0xFF)
        else:
            codeABC(finfo, OP_MOVE, a, b, 0)
        return bfalse

    # ========================================================================
    # free_expreg — Free register at top
    # ========================================================================

    # static void free_expreg(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     /* release temporary register */
    #     if (e && e->type == ETREG && e->v.idx == finfo->freereg - 1) {
    #         be_code_freeregs(finfo, 1);
    #     }
    # }
    def free_expreg(finfo, e):
        """Free register at top (checks that it's a register)."""
        if e is not None and e.type == ETREG and e.v.idx == finfo.freereg - 1:
            be_code_freeregs(finfo, 1)

    # ========================================================================
    # allocstack — Allocate registers on the stack
    # ========================================================================

    # static void allocstack(bfuncinfo *finfo, int count)
    # {
    #     int nstack = finfo->freereg + count;
    #     if (nstack > finfo->proto->nstack) {
    #         if (nstack >= 255) {
    #             be_lexerror(finfo->lexer, "register overflow (more than 255)");
    #         }
    #         finfo->proto->nstack = (bbyte)nstack;
    #     }
    # }
    def allocstack(finfo, count):
        """Update proto's max nstack if needed."""
        nstack = finfo.freereg + count
        if nstack > finfo.proto.nstack:
            if nstack >= 255:
                be_lexerror(finfo.lexer, "register overflow (more than 255)")
            finfo.proto.nstack = nstack

    # ========================================================================
    # be_code_allocregs — Allocate count registers at top of stack
    # ========================================================================

    # int be_code_allocregs(bfuncinfo *finfo, int count)
    # {
    #     int base = finfo->freereg;
    #     allocstack(finfo, count);
    #     finfo->freereg += (char)count;
    #     return base;
    # }
    def be_code_allocregs(finfo, count):
        """Allocate count registers at top of stack, return base register."""
        base = finfo.freereg
        allocstack(finfo, count)
        finfo.freereg += count
        return base


    # ========================================================================
    # Jump helpers
    # ========================================================================

    # static void setjump(bfuncinfo *finfo, int pc, int dst)
    # {
    #     binstruction *p = be_vector_at(&finfo->code, pc);
    #     int offset = dst - (pc + 1);
    #     *p = (*p & ~IBx_MASK) | ISET_sBx(offset);
    # }
    def setjump(finfo, pc, dst):
        """Set jump destination for instruction at pc."""
        p = be_vector_at(finfo.code, pc)
        offset = dst - (pc + 1)
        new_val = (p & ~IBx_MASK) | ISET_sBx(offset)
        finfo.code.data[pc] = new_val & 0xFFFFFFFF

    # static int isjumpbool(bfuncinfo *finfo, int pc)
    # {
    #     binstruction *p = be_vector_at(&finfo->code, pc);
    #     bopcode op = IGET_OP(*p);
    #     if (op == OP_JMPT || op == OP_JMPF) {
    #         return 1;
    #     }
    #     return 0;
    # }
    def isjumpbool(finfo, pc):
        """Check if instruction at pc is a conditional jump."""
        p = be_vector_at(finfo.code, pc)
        op = IGET_OP(p)
        return 1 if (op == OP_JMPT or op == OP_JMPF) else 0

    # static int get_jump(bfuncinfo *finfo, int pc)
    # {
    #     binstruction *i = be_vector_at(&finfo->code, pc);
    #     int offset = IGET_sBx(*i);
    #     return offset == NO_JUMP ? NO_JUMP : pc + 1 + offset;
    # }
    def get_jump(finfo, pc):
        """Get the jump target from instruction at pc."""
        i = be_vector_at(finfo.code, pc)
        offset = IGET_sBx(i)
        return NO_JUMP if offset == NO_JUMP else pc + 1 + offset

    # static void patchlistaux(bfuncinfo *finfo, int list, int vtarget, int dtarget)
    # {
    #     if (vtarget > finfo->binfo->lastjmp) {
    #         finfo->binfo->lastjmp = vtarget;
    #     }
    #     if (dtarget > finfo->binfo->lastjmp) {
    #         finfo->binfo->lastjmp = dtarget;
    #     }
    #     while (list != NO_JUMP) {
    #         int next = get_jump(finfo, list);
    #         if (isjumpbool(finfo, list)) {
    #             setjump(finfo, list, dtarget);
    #         } else {
    #             setjump(finfo, list, vtarget);
    #         }
    #         list = next;
    #     }
    # }
    def patchlistaux(finfo, list_, vtarget, dtarget):
        """Patch a jump list with value target and default target."""
        if vtarget > finfo.binfo.lastjmp:
            finfo.binfo.lastjmp = vtarget
        if dtarget > finfo.binfo.lastjmp:
            finfo.binfo.lastjmp = dtarget
        while list_ != NO_JUMP:
            next_ = get_jump(finfo, list_)
            if isjumpbool(finfo, list_):
                setjump(finfo, list_, dtarget)
            else:
                setjump(finfo, list_, vtarget)
            list_ = next_

    # ========================================================================
    # appendjump — emit a jump instruction
    # ========================================================================

    # static int appendjump(bfuncinfo *finfo, bopcode op, bexpdesc *e)
    # {
    #     int reg = e ? var2anyreg(finfo, e) : 0;
    #     if (isK(reg)) {
    #         reg = be_code_allocregs(finfo, 1);
    #         code_move(finfo, reg, e->v.idx);
    #         e->v.idx = reg;
    #         e->type = ETREG;
    #     }
    #     return codeABx(finfo, op, reg, NO_JUMP + IsBx_MAX);
    # }
    def appendjump(finfo, op, e):
        """Emit a jump instruction, materializing e to a register if needed."""
        reg = var2anyreg(finfo, e) if e is not None else 0
        if isK(reg):
            reg = be_code_allocregs(finfo, 1)
            code_move(finfo, reg, e.v.idx)
            e.v.idx = reg
            e.type = ETREG
        return codeABx(finfo, op, reg, NO_JUMP + IsBx_MAX)

    # ========================================================================
    # Public jump functions
    # ========================================================================

    # int be_code_jump(bfuncinfo *finfo)
    # {
    #     return appendjump(finfo, OP_JMP, NULL);
    # }
    def be_code_jump(finfo):
        """Emit an unconditional jump."""
        return appendjump(finfo, OP_JMP, None)

    # void be_code_jumpto(bfuncinfo *finfo, int dst)
    # {
    #     be_code_patchlist(finfo, be_code_jump(finfo), dst);
    # }
    def be_code_jumpto(finfo, dst):
        """Emit a jump to a specific destination."""
        be_code_patchlist(finfo, be_code_jump(finfo), dst)

    # void be_code_jumpbool(bfuncinfo *finfo, bexpdesc *e, int jture)
    # {
    #     int pc = appendjump(finfo, jumpboolop(e, jture), e);
    #     be_code_conjump(finfo, jture ? &e->t : &e->f, pc);
    #     be_code_patchjump(finfo, jture ? e->f : e->t);
    #     free_expreg(finfo, e);
    #     jture ? (e->f = NO_JUMP) : (e->t = NO_JUMP);
    #     e->not = 0;
    # }
    def be_code_jumpbool(finfo, e, jture):
        """Emit a conditional jump based on boolean value."""
        pc = appendjump(finfo, jumpboolop(e, jture), e)
        if jture:
            e.t = _conjump_helper(finfo, e.t, pc)
        else:
            e.f = _conjump_helper(finfo, e.f, pc)
        be_code_patchjump(finfo, e.f if jture else e.t)
        free_expreg(finfo, e)
        if jture:
            e.f = NO_JUMP
        else:
            e.t = NO_JUMP
        e.not_ = 0

    # ========================================================================
    # be_code_conjump — connect jump lists
    # ========================================================================

    # void be_code_conjump(bfuncinfo *finfo, int *list, int jmp)
    # {
    #     if (jmp == NO_JUMP) {
    #         return;
    #     }
    #     if (*list == NO_JUMP) {
    #         *list = jmp;
    #     } else {
    #         int next, l = *list;
    #         while ((next = (get_jump(finfo, l))) != NO_JUMP) {
    #             l = next;
    #         }
    #         setjump(finfo, l, jmp);
    #     }
    # }
    #
    # Note: In C, `list` is an int* (pointer to int). In Python we cannot
    # pass int by reference, so be_code_conjump returns the updated list value.
    # Callers must assign: e.t = be_code_conjump(finfo, e.t, jmp)
    def be_code_conjump(finfo, list_val, jmp):
        """Connect jump to a jump list. Returns updated list value."""
        if jmp == NO_JUMP:
            return list_val
        if list_val == NO_JUMP:
            return jmp
        else:
            l = list_val
            while True:
                next_ = get_jump(finfo, l)
                if next_ == NO_JUMP:
                    break
                l = next_
            setjump(finfo, l, jmp)
            return list_val

    # Internal helper used by be_code_jumpbool
    def _conjump_helper(finfo, list_val, jmp):
        """Wrapper for be_code_conjump that returns updated list value."""
        return be_code_conjump(finfo, list_val, jmp)

    # ========================================================================
    # be_code_patchlist / be_code_patchjump
    # ========================================================================

    # void be_code_patchlist(bfuncinfo *finfo, int list, int dst)
    # {
    #     if (dst == finfo->pc) {
    #         be_code_patchjump(finfo, list);
    #     } else {
    #         patchlistaux(finfo, list, dst, dst);
    #     }
    # }
    def be_code_patchlist(finfo, list_, dst):
        """Patch a jump list to target dst."""
        if dst == finfo.pc:
            be_code_patchjump(finfo, list_)
        else:
            patchlistaux(finfo, list_, dst, dst)

    # void be_code_patchjump(bfuncinfo *finfo, int jmp)
    # {
    #     patchlistaux(finfo, jmp, finfo->pc, finfo->pc);
    # }
    def be_code_patchjump(finfo, jmp):
        """Patch a jump to the current pc."""
        patchlistaux(finfo, jmp, finfo.pc, finfo.pc)


    # ========================================================================
    # Constant table management
    # ========================================================================

    # static int newconst(bfuncinfo *finfo, bvalue *k)
    # {
    #     int idx = be_vector_count(&finfo->kvec);
    #     be_vector_push_c(finfo->lexer->vm, &finfo->kvec, k);
    #     finfo->proto->ktab = be_vector_data(&finfo->kvec);
    #     finfo->proto->nconst = be_vector_capacity(&finfo->kvec);
    #     if (k == NULL) {
    #         var_setnil(&finfo->proto->ktab[idx]);
    #     }
    #     return idx;
    # }
    def newconst(finfo, k):
        """Allocate new constant for value k. If k is None, push nil."""
        idx = be_vector_count(finfo.kvec)
        be_vector_push_c(finfo.lexer.vm, finfo.kvec, k)
        finfo.proto.ktab = be_vector_data(finfo.kvec)
        finfo.proto.nconst = be_vector_capacity(finfo.kvec)
        if k is None:
            var_setnil(finfo.proto.ktab[idx])
        return idx

    # static int findconst(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     int i, count = be_vector_count(&finfo->kvec);
    #     count = count < BE_CONST_SEARCH_SIZE ? count : BE_CONST_SEARCH_SIZE;
    #     for (i = 0; i < count; ++i) {
    #         bvalue *k = be_vector_at(&finfo->kvec, i);
    #         switch (e->type) {
    #         case ETINT:
    #             if (var_isint(k) && k->v.i == e->v.i) {
    #                 return i;
    #             }
    #             break;
    #         case ETREAL:
    #             if (var_isreal(k) && k->v.r == e->v.r) {
    #                 return i;
    #             }
    #             break;
    #         case ETSTRING:
    #             if (var_isstr(k) && be_eqstr(k->v.p, e->v.s)) {
    #                 return i;
    #             }
    #             break;
    #         default:
    #             break;
    #         }
    #     }
    #     return -1;
    # }
    def findconst(finfo, e):
        """Find constant by value and return index, or -1 if not found."""
        count = be_vector_count(finfo.kvec)
        count = min(count, BE_CONST_SEARCH_SIZE)
        for i in range(count):
            k = be_vector_at(finfo.kvec, i)
            if k is None:
                continue
            if e.type == ETINT:
                if var_isint(k) and k.v == e.v.i:
                    return i
            elif e.type == ETREAL:
                if var_isreal(k) and k.v == e.v.r:
                    return i
            elif e.type == ETSTRING:
                if var_isstr(k) and be_eqstr(k.v, e.v.s):
                    return i
        return -1

    # ========================================================================
    # exp2const — convert expdesc to constant
    # ========================================================================

    # static int exp2const(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     int idx = findconst(finfo, e);
    #     if (idx == -1) {
    #         bvalue k = {0};
    #         switch (e->type) {
    #         case ETINT:
    #             k.type = BE_INT;
    #             k.v.i = e->v.i;
    #             break;
    #         case ETREAL:
    #             k.type = BE_REAL;
    #             k.v.r = e->v.r;
    #             break;
    #         case ETSTRING:
    #             k.type = BE_STRING;
    #             k.v.s = e->v.s;
    #             break;
    #         default:
    #             break;
    #         }
    #         idx = newconst(finfo, &k);
    #     }
    #     if (idx < 256) {
    #         e->type = ETCONST;
    #         e->v.idx = setK(idx);
    #     } else {
    #         e->type = ETREG;
    #         e->v.idx = be_code_allocregs(finfo, 1);
    #         codeABx(finfo, OP_LDCONST, e->v.idx, idx);
    #     }
    #     return e->v.idx;
    # }
    def exp2const(finfo, e):
        """Convert expdesc to constant and return kreg index."""
        idx = findconst(finfo, e)
        if idx == -1:
            k = bvalue()
            if e.type == ETINT:
                k.type = BE_INT
                k.v = e.v.i
            elif e.type == ETREAL:
                k.type = BE_REAL
                k.v = e.v.r
            elif e.type == ETSTRING:
                k.type = BE_STRING
                k.v = e.v.s
            idx = newconst(finfo, k)
        if idx < 256:
            e.type = ETCONST
            e.v.idx = setK(idx)
        else:
            e.type = ETREG
            e.v.idx = be_code_allocregs(finfo, 1)
            codeABx(finfo, OP_LDCONST, e.v.idx, idx)
        return e.v.idx

    # ========================================================================
    # Suffix helpers (for ETMEMBER / ETINDEX)
    # ========================================================================

    # static void free_suffix(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     int idx = e->v.ss.idx;
    #     int nlocal = be_list_count(finfo->local);
    #     if (!isK(idx) && idx >= nlocal) {
    #         be_code_freeregs(finfo, 1);
    #     }
    #     if (e->v.ss.tt == ETREG && (int)e->v.ss.obj >= nlocal &&
    #         (e->v.ss.obj + 1 >= finfo->freereg)) {
    #         be_code_freeregs(finfo, 1);
    #     }
    # }
    def free_suffix(finfo, e):
        """Release suffix register(s)."""
        idx = e.v.ss.idx
        nlocal = be_list_count(finfo.local)
        if not isK(idx) and idx >= nlocal:
            be_code_freeregs(finfo, 1)
        if e.v.ss.tt == ETREG and e.v.ss.obj >= nlocal and (e.v.ss.obj + 1 >= finfo.freereg):
            be_code_freeregs(finfo, 1)

    # static int suffix_destreg(bfuncinfo *finfo, bexpdesc *e1, int dst, bbool no_reg_reuse)
    # {
    #     int cand_dst = dst;
    #     int nlocal = be_list_count(finfo->local);
    #     int reg1 = (e1->v.ss.tt == ETREG) ? e1->v.ss.obj : -1;
    #     int reg2 = (!isK(e1->v.ss.idx) && e1->v.ss.idx >= nlocal) ? e1->v.ss.idx : -1;
    #     if (no_reg_reuse) {
    #         reg1 = reg2 = -1;
    #     }
    #     if (reg1 >= 0 && reg2 >= 0) {
    #         if (reg1 != reg2) {
    #             cand_dst = min(reg1, reg2);
    #             be_code_freeregs(finfo, finfo->freereg - cand_dst);
    #         } else {
    #             cand_dst = reg1;
    #         }
    #     } else if (reg1 >= 0) {
    #         cand_dst = reg1;
    #     } else if (reg2 >= 0) {
    #         cand_dst = reg2;
    #     }
    #     if (dst >= finfo->freereg) {
    #         dst = cand_dst;
    #     }
    #     return dst;
    # }
    def suffix_destreg(finfo, e1, dst, no_reg_reuse):
        """Select destination register for suffix operations."""
        cand_dst = dst
        nlocal = be_list_count(finfo.local)
        reg1 = e1.v.ss.obj if e1.v.ss.tt == ETREG else -1
        reg2 = e1.v.ss.idx if (not isK(e1.v.ss.idx) and e1.v.ss.idx >= nlocal) else -1
        if no_reg_reuse:
            reg1 = -1
            reg2 = -1
        if reg1 >= 0 and reg2 >= 0:
            if reg1 != reg2:
                cand_dst = min(reg1, reg2)
                be_code_freeregs(finfo, finfo.freereg - cand_dst)
            else:
                cand_dst = reg1
        elif reg1 >= 0:
            cand_dst = reg1
        elif reg2 >= 0:
            cand_dst = reg2
        if dst >= finfo.freereg:
            dst = cand_dst
        return dst

    # static int code_suffix(bfuncinfo *finfo, bopcode op, bexpdesc *e, int dst,
    #                        bbool no_reg_reuse)
    # {
    #     dst = suffix_destreg(finfo, e, dst, no_reg_reuse);
    #     if (dst > finfo->freereg) {
    #         dst = finfo->freereg;
    #     }
    #     codeABC(finfo, op, dst, e->v.ss.obj, e->v.ss.idx);
    #     return dst;
    # }
    def code_suffix(finfo, op, e, dst, no_reg_reuse):
        """Emit a suffix operation (GETMBR, GETIDX, GETMET)."""
        dst = suffix_destreg(finfo, e, dst, no_reg_reuse)
        if dst > finfo.freereg:
            dst = finfo.freereg
        codeABC(finfo, op, dst, e.v.ss.obj, e.v.ss.idx)
        return dst

    # ========================================================================
    # code_closure — emit CLOSURE instruction
    # ========================================================================

    # static void code_closure(bfuncinfo *finfo, int idx, int dst)
    # {
    #     codeABx(finfo, OP_CLOSURE, dst, idx);
    # }
    def _code_closure(finfo, idx, dst):
        """Emit CLOSURE instruction to load closure to register."""
        codeABx(finfo, OP_CLOSURE, dst, idx)

    # ========================================================================
    # constint — check if integer should be a constant
    # ========================================================================

    # static bbool constint(bfuncinfo *finfo, bint i)
    # {
    #     if ((i < IsBx_MIN || i > IsBx_MAX) ||
    #         (i >= 0 && i <= 3 && be_vector_count(&finfo->kvec) < 256)) {
    #         return btrue;
    #     }
    #     return bfalse;
    # }
    def constint(finfo, i):
        """Check if integer should be stored as a constant."""
        if (i < IsBx_MIN or i > IsBx_MAX) or \
           (i >= 0 and i <= 3 and be_vector_count(finfo.kvec) < 256):
            return btrue
        return bfalse


    # ========================================================================
    # var2reg — Compute variable from an expdesc
    # ========================================================================

    # static int var2reg(bfuncinfo *finfo, bexpdesc *e, int dst)
    # {
    #     bbool no_reg_reuse = (dst >= 0);
    #     if (dst < 0) { dst = finfo->freereg; }
    #     be_assert(e != NULL);
    #     switch (e->type) {
    #     case ETINT:
    #         if (constint(finfo, e->v.i)) {
    #             return exp2const(finfo, e);
    #         } else {
    #             codeABx(finfo, OP_LDINT, dst, var_toidx(e) + IsBx_MAX);
    #         }
    #         break;
    #     case ETBOOL:
    #         code_bool(finfo, dst, e->v.i != 0, 0);
    #         break;
    #     case ETNIL:
    #         codeABx(finfo, OP_LDNIL, dst, 0);
    #         break;
    #     case ETREAL: case ETSTRING:
    #         return exp2const(finfo, e);
    #     case ETPROTO:
    #         code_closure(finfo, e->v.idx, dst);
    #         break;
    #     case ETGLOBAL:
    #         codeABx(finfo, OP_GETGBL, dst, e->v.idx);
    #         break;
    #     case ETNGLOBAL:
    #         codeABC(finfo, OP_GETNGBL, dst, e->v.ss.idx, 0);
    #         break;
    #     case ETUPVAL:
    #         codeABx(finfo, OP_GETUPV, dst, e->v.idx);
    #         break;
    #     case ETMEMBER:
    #         dst = code_suffix(finfo, OP_GETMBR, e, dst, no_reg_reuse);
    #         break;
    #     case ETINDEX:
    #         dst = code_suffix(finfo, OP_GETIDX, e, dst, no_reg_reuse);
    #         break;
    #     case ETLOCAL: case ETREG: case ETCONST:
    #         return e->v.idx;
    #     default:
    #         return dst;
    #     }
    #     if (dst == finfo->freereg) {
    #         be_code_allocregs(finfo, 1);
    #     }
    #     e->type = ETREG;
    #     e->v.idx = dst;
    #     return dst;
    # }
    def var2reg(finfo, e, dst):
        """Compute variable from an expdesc. Return register or constant index."""
        no_reg_reuse = (dst >= 0)
        if dst < 0:
            dst = finfo.freereg
        assert e is not None
        t = e.type
        if t == ETINT:
            if constint(finfo, e.v.i):
                return exp2const(finfo, e)
            else:
                codeABx(finfo, OP_LDINT, dst, var_toidx(e) + IsBx_MAX)
        elif t == ETBOOL:
            codeABC(finfo, OP_LDBOOL, dst, 1 if e.v.i != 0 else 0, 0)
        elif t == ETNIL:
            codeABx(finfo, OP_LDNIL, dst, 0)
        elif t == ETREAL or t == ETSTRING:
            return exp2const(finfo, e)
        elif t == ETPROTO:
            _code_closure(finfo, e.v.idx, dst)
        elif t == ETGLOBAL:
            codeABx(finfo, OP_GETGBL, dst, e.v.idx)
        elif t == ETNGLOBAL:
            codeABC(finfo, OP_GETNGBL, dst, e.v.ss.idx, 0)
        elif t == ETUPVAL:
            codeABx(finfo, OP_GETUPV, dst, e.v.idx)
        elif t == ETMEMBER:
            dst = code_suffix(finfo, OP_GETMBR, e, dst, no_reg_reuse)
        elif t == ETINDEX:
            dst = code_suffix(finfo, OP_GETIDX, e, dst, no_reg_reuse)
        elif t in (ETLOCAL, ETREG, ETCONST):
            return e.v.idx
        else:
            return dst  # error
        if dst == finfo.freereg:
            be_code_allocregs(finfo, 1)
        e.type = ETREG
        e.v.idx = dst
        return dst

    # Convenience aliases matching C macros
    # #define exp2anyreg(f, e)  exp2reg(f, e, -1)
    # #define var2anyreg(f, e)  var2reg(f, e, -1)
    def var2anyreg(finfo, e):
        return var2reg(finfo, e, -1)

    # ========================================================================
    # exp2reg — convert expression to register, handling jumps
    # ========================================================================

    # static int exp2reg(bfuncinfo *finfo, bexpdesc *e, int dst)
    # {
    #     int reg = var2reg(finfo, e, dst);
    #     if (hasjump(e)) {
    #         int pcf = NO_JUMP;
    #         int pct = NO_JUMP;
    #         int jpt = appendjump(finfo, jumpboolop(e, 1), e);
    #         if (e->type == ETREG) {
    #             reg = e->v.idx;
    #         } else {
    #             reg = (dst < 0) ? be_code_allocregs(finfo, 1) : dst;
    #         }
    #         be_code_conjump(finfo, &e->t, jpt);
    #         pcf = code_bool(finfo, reg, 0, 1);
    #         pct = code_bool(finfo, reg, 1, 0);
    #         patchlistaux(finfo, e->f, finfo->pc, pcf);
    #         patchlistaux(finfo, e->t, finfo->pc, pct);
    #         e->t = NO_JUMP;
    #         e->f = NO_JUMP;
    #         e->not = 0;
    #     }
    #     return reg;
    # }
    def exp2reg(finfo, e, dst):
        """Convert expression to register, handling conditional jumps."""
        reg = var2reg(finfo, e, dst)
        if hasjump(e):
            pcf = NO_JUMP
            pct = NO_JUMP
            jpt = appendjump(finfo, jumpboolop(e, 1), e)
            if e.type == ETREG:
                reg = e.v.idx
            else:
                reg = be_code_allocregs(finfo, 1) if dst < 0 else dst
            e.t = be_code_conjump(finfo, e.t, jpt)
            pcf = codeABC(finfo, OP_LDBOOL, reg, 0, 1)
            pct = codeABC(finfo, OP_LDBOOL, reg, 1, 0)
            patchlistaux(finfo, e.f, finfo.pc, pcf)
            patchlistaux(finfo, e.t, finfo.pc, pct)
            e.t = NO_JUMP
            e.f = NO_JUMP
            e.not_ = 0
        return reg

    def exp2anyreg(finfo, e):
        return exp2reg(finfo, e, -1)

    # ========================================================================
    # codedestreg — Select dest register from both expressions
    # ========================================================================

    # static int codedestreg(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2, int dst)
    # {
    #     if (dst < 0) { dst = finfo->freereg; }
    #     int cand_dst = dst;
    #     int con1 = e1->type == ETREG, con2 = e2->type == ETREG;
    #     if (con1 && con2) {
    #         cand_dst = min(e1->v.idx, e2->v.idx);
    #         be_code_freeregs(finfo, 1);
    #     } else if (con1) {
    #         cand_dst = e1->v.idx;
    #     } else if (con2) {
    #         cand_dst = e2->v.idx;
    #     } else {
    #         if (dst >= finfo->freereg) {
    #             cand_dst = be_code_allocregs(finfo, 1);
    #             return cand_dst;
    #         }
    #     }
    #     if (dst >= finfo->freereg) {
    #         return cand_dst;
    #     } else {
    #         return dst;
    #     }
    # }
    def codedestreg(finfo, e1, e2, dst):
        """Select destination register from both expressions."""
        if dst < 0:
            dst = finfo.freereg
        cand_dst = dst
        con1 = (e1.type == ETREG)
        con2 = (e2.type == ETREG)
        if con1 and con2:
            cand_dst = min(e1.v.idx, e2.v.idx)
            be_code_freeregs(finfo, 1)
        elif con1:
            cand_dst = e1.v.idx
        elif con2:
            cand_dst = e2.v.idx
        else:
            if dst >= finfo.freereg:
                cand_dst = be_code_allocregs(finfo, 1)
                return cand_dst
        if dst >= finfo.freereg:
            return cand_dst
        else:
            return dst

    # ========================================================================
    # binaryexp — compute binary expression
    # ========================================================================

    # static void binaryexp(bfuncinfo *finfo, bopcode op, bexpdesc *e1,
    #                       bexpdesc *e2, int dst)
    # {
    #     int src1 = exp2reg(finfo, e1, dst);
    #     int src2 = exp2anyreg(finfo, e2);
    #     dst = codedestreg(finfo, e1, e2, dst);
    #     codeABC(finfo, op, dst, src1, src2);
    #     e1->type = ETREG;
    #     e1->v.idx = dst;
    # }
    def binaryexp(finfo, op, e1, e2, dst):
        """Compute binary expression and update e1 as result."""
        src1 = exp2reg(finfo, e1, dst)
        src2 = exp2anyreg(finfo, e2)
        dst = codedestreg(finfo, e1, e2, dst)
        codeABC(finfo, op, dst, src1, src2)
        e1.type = ETREG
        e1.v.idx = dst


    # ========================================================================
    # be_code_prebinop — pre-process binary operator
    # ========================================================================

    # void be_code_prebinop(bfuncinfo *finfo, int op, bexpdesc *e)
    # {
    #     switch (op) {
    #     case OptAnd:
    #         be_code_jumpbool(finfo, e, bfalse);
    #         break;
    #     case OptOr:
    #         be_code_jumpbool(finfo, e, btrue);
    #         break;
    #     default:
    #         exp2anyreg(finfo, e);
    #         break;
    #     }
    # }
    def be_code_prebinop(finfo, op, e):
        """Pre-process binary operator (short-circuit for && and ||)."""
        if op == OptAnd:
            be_code_jumpbool(finfo, e, bfalse)
        elif op == OptOr:
            be_code_jumpbool(finfo, e, btrue)
        else:
            exp2anyreg(finfo, e)

    # ========================================================================
    # be_code_binop — Apply binary operator
    # ========================================================================

    # void be_code_binop(bfuncinfo *finfo, int op, bexpdesc *e1,
    #                    bexpdesc *e2, int dst)
    # {
    #     switch (op) {
    #     case OptAnd:
    #         var2anyreg(finfo, e2);
    #         be_code_conjump(finfo, &e2->f, e1->f);
    #         *e1 = *e2;
    #         break;
    #     case OptOr:
    #         var2anyreg(finfo, e2);
    #         be_code_conjump(finfo, &e2->t, e1->t);
    #         *e1 = *e2;
    #         break;
    #     case OptAdd: case OptSub: case OptMul: case OptDiv:
    #     case OptMod: case OptLT: case OptLE: case OptEQ:
    #     case OptNE: case OptGT: case OptGE: case OptConnect:
    #     case OptBitAnd: case OptBitOr: case OptBitXor:
    #     case OptShiftL: case OptShiftR:
    #         binaryexp(finfo, (bopcode)(op - OptAdd), e1, e2, dst);
    #         break;
    #     default: break;
    #     }
    # }
    def be_code_binop(finfo, op, e1, e2, dst):
        """Apply binary operator op to e1 and e2, result in e1."""
        if op == OptAnd:
            var2anyreg(finfo, e2)
            e2.f = be_code_conjump(finfo, e2.f, e1.f)
            # *e1 = *e2
            from berry_port.be_parser import _copy_expdesc
            _copy_expdesc(e1, e2)
        elif op == OptOr:
            var2anyreg(finfo, e2)
            e2.t = be_code_conjump(finfo, e2.t, e1.t)
            from berry_port.be_parser import _copy_expdesc
            _copy_expdesc(e1, e2)
        elif op in (OptAdd, OptSub, OptMul, OptDiv, OptMod,
                    OptLT, OptLE, OptEQ, OptNE, OptGT, OptGE,
                    OptConnect,
                    OptBitAnd, OptBitOr, OptBitXor,
                    OptShiftL, OptShiftR):
            binaryexp(finfo, op - OptAdd, e1, e2, dst)

    # ========================================================================
    # Unary operators
    # ========================================================================

    # static void unaryexp(bfuncinfo *finfo, bopcode op, bexpdesc *e)
    # {
    #     int src = exp2anyreg(finfo, e);
    #     int dst = e->type == ETREG ? src : be_code_allocregs(finfo, 1);
    #     if (!(op == OP_MOVE && src == dst)) {
    #         codeABC(finfo, op, dst, src, 0);
    #     }
    #     e->type = ETREG;
    #     e->v.idx = dst;
    # }
    def unaryexp(finfo, op, e):
        """Apply unary operator and return register number."""
        src = exp2anyreg(finfo, e)
        dst = src if e.type == ETREG else be_code_allocregs(finfo, 1)
        if not (op == OP_MOVE and src == dst):
            codeABC(finfo, op, dst, src, 0)
        e.type = ETREG
        e.v.idx = dst

    # static void code_not(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     switch (e->type) {
    #     case ETINT: e->v.i = e->v.i == 0; break;
    #     case ETREAL: e->v.i = e->v.r == cast(breal, 0); break;
    #     case ETNIL: e->v.i = 1; break;
    #     case ETBOOL: e->v.i = !e->v.i; break;
    #     case ETSTRING: e->v.i = 0; break;
    #     default: {
    #         unaryexp(finfo, OP_MOVE, e);
    #         int temp = e->t;
    #         e->t = e->f;
    #         e->f = temp;
    #         e->not = NOT_EXPR | (e->not ^ NOT_MASK);
    #         return;
    #     }
    #     }
    #     e->type = ETBOOL;
    # }
    def code_not(finfo, e):
        """Apply not to conditional expression."""
        t = e.type
        if t == ETINT:
            e.v.i = 1 if e.v.i == 0 else 0
        elif t == ETREAL:
            e.v.i = 1 if e.v.r == 0.0 else 0
        elif t == ETNIL:
            e.v.i = 1
        elif t == ETBOOL:
            e.v.i = 0 if e.v.i else 1
        elif t == ETSTRING:
            e.v.i = 0
        else:
            unaryexp(finfo, OP_MOVE, e)
            temp = e.t
            e.t = e.f
            e.f = temp
            e.not_ = NOT_EXPR | (e.not_ ^ NOT_MASK)
            return
        e.type = ETBOOL

    # static int code_neg(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     switch (e->type) {
    #     case ETINT: e->v.i = -e->v.i; break;
    #     case ETREAL: e->v.r = -e->v.r; break;
    #     case ETNIL: case ETBOOL: case ETSTRING:
    #         return 1;
    #     default:
    #         unaryexp(finfo, OP_NEG, e);
    #     }
    #     return 0;
    # }
    def code_neg(finfo, e):
        """Negative value of literal or emit NEG opcode."""
        t = e.type
        if t == ETINT:
            e.v.i = -e.v.i
        elif t == ETREAL:
            e.v.r = -e.v.r
        elif t in (ETNIL, ETBOOL, ETSTRING):
            return 1  # error
        else:
            unaryexp(finfo, OP_NEG, e)
        return 0

    # static int code_flip(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     switch (e->type) {
    #     case ETINT: e->v.i = ~e->v.i; break;
    #     case ETREAL: case ETNIL: case ETBOOL: case ETSTRING:
    #         return 2;
    #     default:
    #         unaryexp(finfo, OP_FLIP, e);
    #     }
    #     return 0;
    # }
    def code_flip(finfo, e):
        """Bit flip of literal or emit FLIP opcode."""
        t = e.type
        if t == ETINT:
            e.v.i = ~e.v.i
        elif t in (ETREAL, ETNIL, ETBOOL, ETSTRING):
            return 2  # error
        else:
            unaryexp(finfo, OP_FLIP, e)
        return 0

    # int be_code_unop(bfuncinfo *finfo, int op, bexpdesc *e)
    # {
    #     switch (op) {
    #     case OptNot:
    #         code_not(finfo, e); break;
    #     case OptFlip:
    #         return code_flip(finfo, e);
    #     case OptSub:
    #         return code_neg(finfo, e);
    #     default:
    #         break;
    #     }
    #     return 0;
    # }
    def be_code_unop(finfo, op, e):
        """Apply unary operator: not, neg or bitflip."""
        if op == OptNot:
            code_not(finfo, e)
        elif op == OptFlip:
            return code_flip(finfo, e)
        elif op == OptSub:
            return code_neg(finfo, e)
        return 0


    # ========================================================================
    # Variable assignment helpers
    # ========================================================================

    # static void setbgblvar(bfuncinfo *finfo, bopcode op, bexpdesc *e1, int src)
    # {
    #     if (isK(src)) {
    #         code_move(finfo, finfo->freereg, src);
    #         src = finfo->freereg;
    #     }
    #     codeABC(finfo, op, src, e1->v.idx, 0);
    # }
    def setbgblvar(finfo, op, e1, src):
        """Set named global variable."""
        if isK(src):
            code_move(finfo, finfo.freereg, src)
            src = finfo.freereg
        codeABC(finfo, op, src, e1.v.idx, 0)

    # static void setsupvar(bfuncinfo *finfo, bopcode op, bexpdesc *e1, int src)
    # {
    #     if (isK(src)) {
    #         code_move(finfo, finfo->freereg, src);
    #         src = finfo->freereg;
    #     }
    #     codeABx(finfo, op, src, e1->v.idx);
    # }
    def setsupvar(finfo, op, e1, src):
        """Set global or upvalue variable."""
        if isK(src):
            code_move(finfo, finfo.freereg, src)
            src = finfo.freereg
        codeABx(finfo, op, src, e1.v.idx)

    # static void setsfxvar(bfuncinfo *finfo, bopcode op, bexpdesc *e1, int src)
    # {
    #     int obj = e1->v.ss.obj;
    #     free_suffix(finfo, e1);
    #     if (isK(obj)) {
    #         code_move(finfo, finfo->freereg, obj);
    #         obj = finfo->freereg;
    #     }
    #     codeABC(finfo, op, obj, e1->v.ss.idx, src);
    # }
    def setsfxvar(finfo, op, e1, src):
        """Set member or index variable."""
        obj = e1.v.ss.obj
        free_suffix(finfo, e1)
        if isK(obj):
            code_move(finfo, finfo.freereg, obj)
            obj = finfo.freereg
        codeABC(finfo, op, obj, e1.v.ss.idx, src)

    # ========================================================================
    # be_code_setvar — Assign expr e2 to e1
    # ========================================================================

    # int be_code_setvar(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2, bbool keep_reg)
    # {
    #     bbool free_e2 = (e2->type == ETINDEX || e2->type == ETMEMBER) &&
    #                     (((e2->v.ss.idx != e1->v.idx) && (e2->v.ss.idx == finfo->freereg - 1)) ||
    #                      ((e2->v.ss.obj != e1->v.idx) && (e2->v.ss.obj == finfo->freereg - 1)));
    #     int src = exp2reg(finfo, e2,
    #         e1->type == ETLOCAL ? e1->v.idx : -1);
    #     if (!keep_reg && (e1->type != ETLOCAL || e1->v.idx != src)) {
    #         free_expreg(finfo, e2);
    #     } else if (!keep_reg && free_e2) {
    #         if (finfo->freereg > (bbyte)be_list_count(finfo->local)) {
    #             be_code_freeregs(finfo, 1);
    #         }
    #     }
    #     switch (e1->type) {
    #     case ETLOCAL:
    #         if (e1->v.idx != src) {
    #             bbool reg_optimized = code_move(finfo, e1->v.idx, src);
    #             if (reg_optimized) {
    #                 free_expreg(finfo, e2);
    #                 *e2 = *e1;
    #             }
    #         } else {
    #             *e2 = *e1;
    #         }
    #         break;
    #     case ETGLOBAL:
    #         setsupvar(finfo, OP_SETGBL, e1, src);
    #         break;
    #     case ETNGLOBAL:
    #         setbgblvar(finfo, OP_SETNGBL, e1, src);
    #         break;
    #     case ETUPVAL:
    #         setsupvar(finfo, OP_SETUPV, e1, src);
    #         break;
    #     case ETMEMBER: case ETINDEX:
    #         setsfxvar(finfo, OP_SETMBR if e1->type == ETMEMBER else OP_SETIDX, e1, src);
    #         if (keep_reg && e2->type == ETREG && e1->v.ss.obj >= be_list_count(finfo->local)) {
    #             code_move(finfo, e1->v.ss.obj, src);
    #             free_expreg(finfo, e2);
    #             e2->v.idx = e1->v.ss.obj;
    #         }
    #         break;
    #     default:
    #         return 1;
    #     }
    #     return 0;
    # }
    def be_code_setvar(finfo, e1, e2, keep_reg):
        """Assign expr e2 to e1. Returns 0 on success, 1 on incompatible type."""
        from berry_port.be_parser import _copy_expdesc

        free_e2_flag = (e2.type == ETINDEX or e2.type == ETMEMBER) and \
                       (((e2.v.ss.idx != e1.v.idx) and (e2.v.ss.idx == finfo.freereg - 1)) or
                        ((e2.v.ss.obj != e1.v.idx) and (e2.v.ss.obj == finfo.freereg - 1)))

        src = exp2reg(finfo, e2, e1.v.idx if e1.type == ETLOCAL else -1)

        if not keep_reg and (e1.type != ETLOCAL or e1.v.idx != src):
            free_expreg(finfo, e2)
        elif not keep_reg and free_e2_flag:
            if finfo.freereg > be_list_count(finfo.local):
                be_code_freeregs(finfo, 1)

        t = e1.type
        if t == ETLOCAL:
            if e1.v.idx != src:
                reg_optimized = code_move(finfo, e1.v.idx, src)
                if reg_optimized:
                    free_expreg(finfo, e2)
                    _copy_expdesc(e2, e1)
            else:
                _copy_expdesc(e2, e1)
        elif t == ETGLOBAL:
            setsupvar(finfo, OP_SETGBL, e1, src)
        elif t == ETNGLOBAL:
            setbgblvar(finfo, OP_SETNGBL, e1, src)
        elif t == ETUPVAL:
            setsupvar(finfo, OP_SETUPV, e1, src)
        elif t == ETMEMBER or t == ETINDEX:
            setsfxvar(finfo, OP_SETMBR if t == ETMEMBER else OP_SETIDX, e1, src)
            if keep_reg and e2.type == ETREG and e1.v.ss.obj >= be_list_count(finfo.local):
                code_move(finfo, e1.v.ss.obj, src)
                free_expreg(finfo, e2)
                e2.v.idx = e1.v.ss.obj
        else:
            return 1
        return 0

    # ========================================================================
    # be_code_nextreg — Get expdesc as a register at top
    # ========================================================================

    # int be_code_nextreg(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     int src = exp2anyreg(finfo, e);
    #     int dst = finfo->freereg;
    #     if ((e->type != ETREG) || (src < dst - 1)) {
    #         code_move(finfo, dst, src);
    #         be_code_allocregs(finfo, 1);
    #     } else {
    #         dst = src;
    #     }
    #     return dst;
    # }
    def be_code_nextreg(finfo, e):
        """Get the expdesc as a register, copying to top if needed."""
        src = exp2anyreg(finfo, e)
        dst = finfo.freereg
        if e.type != ETREG or src < dst - 1:
            code_move(finfo, dst, src)
            be_code_allocregs(finfo, 1)
        else:
            dst = src
        return dst

    # ========================================================================
    # be_code_getmethod — emit GETMET instruction
    # ========================================================================

    # int be_code_getmethod(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     int dst = finfo->freereg;
    #     be_assert(e->type == ETMEMBER);
    #     dst = code_suffix(finfo, OP_GETMET, e, dst, bfalse);
    #     be_code_allocregs(finfo, dst == finfo->freereg ? 2 : 1);
    #     return dst;
    # }
    def be_code_getmethod(finfo, e):
        """Emit GETMET instruction for method call."""
        dst = finfo.freereg
        assert e.type == ETMEMBER
        dst = code_suffix(finfo, OP_GETMET, e, dst, bfalse)
        be_code_allocregs(finfo, 2 if dst == finfo.freereg else 1)
        return dst

    # ========================================================================
    # be_code_call — Generate a CALL instruction
    # ========================================================================

    # void be_code_call(bfuncinfo *finfo, int base, int argc)
    # {
    #     codeABC(finfo, OP_CALL, base, argc, 0);
    #     be_code_freeregs(finfo, argc);
    # }
    def be_code_call(finfo, base, argc):
        """Generate a CALL instruction at base register with argc args."""
        codeABC(finfo, OP_CALL, base, argc, 0)
        be_code_freeregs(finfo, argc)

    # ========================================================================
    # be_code_proto — append proto to current function proto table
    # ========================================================================

    # int be_code_proto(bfuncinfo *finfo, bproto *proto)
    # {
    #     int idx = be_vector_count(&finfo->pvec);
    #     be_vector_push_c(finfo->lexer->vm, &finfo->pvec, &proto);
    #     finfo->proto->ptab = be_vector_data(&finfo->pvec);
    #     finfo->proto->nproto = be_vector_capacity(&finfo->pvec);
    #     return idx;
    # }
    def be_code_proto(finfo, proto):
        """Append proto to current function proto table."""
        idx = be_vector_count(finfo.pvec)
        be_vector_push_c(finfo.lexer.vm, finfo.pvec, proto)
        finfo.proto.ptab = be_vector_data(finfo.pvec)
        finfo.proto.nproto = be_vector_capacity(finfo.pvec)
        return idx


    # ========================================================================
    # be_code_closure — emit closure and store to variable
    # ========================================================================

    # void be_code_closure(bfuncinfo *finfo, bexpdesc *e, int idx)
    # {
    #     int reg = (e->type == ETGLOBAL || e->type == ETNGLOBAL) ?
    #               finfo->freereg : e->v.idx;
    #     code_closure(finfo, idx, reg);
    #     if (e->type == ETGLOBAL) {
    #         codeABx(finfo, OP_SETGBL, reg, e->v.idx);
    #     } else if (e->type == ETNGLOBAL) {
    #         codeABC(finfo, OP_SETNGBL, reg, e->v.idx, 0);
    #     }
    # }
    def be_code_closure(finfo, e, idx):
        """Emit closure and store to variable if global."""
        reg = finfo.freereg if (e.type == ETGLOBAL or e.type == ETNGLOBAL) else e.v.idx
        _code_closure(finfo, idx, reg)
        if e.type == ETGLOBAL:
            codeABx(finfo, OP_SETGBL, reg, e.v.idx)
        elif e.type == ETNGLOBAL:
            codeABC(finfo, OP_SETNGBL, reg, e.v.idx, 0)

    # ========================================================================
    # be_code_close — emit CLOSE instruction for upvalues
    # ========================================================================

    # void be_code_close(bfuncinfo *finfo, int isret)
    # {
    #     bblockinfo *binfo = finfo->binfo;
    #     if (isret) {
    #         while (binfo && !binfo->hasupval) {
    #             binfo = binfo->prev;
    #         }
    #         if (binfo) {
    #             codeABC(finfo, OP_CLOSE, 0, 0, 0);
    #         }
    #     } else if (binfo->prev) {
    #         if (binfo->hasupval) {
    #             codeABC(finfo, OP_CLOSE, binfo->nactlocals, 0, 0);
    #         }
    #     }
    # }
    def be_code_close(finfo, isret):
        """Emit CLOSE instruction for upvalues."""
        binfo = finfo.binfo
        if isret:
            while binfo is not None and not binfo.hasupval:
                binfo = binfo.prev
            if binfo is not None:
                codeABC(finfo, OP_CLOSE, 0, 0, 0)
        elif binfo.prev is not None:
            if binfo.hasupval:
                codeABC(finfo, OP_CLOSE, binfo.nactlocals, 0, 0)

    # ========================================================================
    # leave_function — emit EXBLK for exception blocks on return
    # ========================================================================

    # static void leave_function(bfuncinfo *finfo)
    # {
    #     int try_depth = 0;
    #     bblockinfo *binfo = finfo->binfo;
    #     for (; binfo; binfo = binfo->prev) {
    #         if (binfo->type & BLOCK_EXCEPT) {
    #             ++try_depth;
    #         }
    #     }
    #     if (try_depth) {
    #         be_code_exblk(finfo, try_depth);
    #     }
    # }
    def leave_function(finfo):
        """Emit EXBLK for exception catch blocks on return."""
        try_depth = 0
        binfo = finfo.binfo
        while binfo is not None:
            if binfo.type & BLOCK_EXCEPT:
                try_depth += 1
            binfo = binfo.prev
        if try_depth:
            be_code_exblk(finfo, try_depth)

    # ========================================================================
    # be_code_ret — emit return instruction
    # ========================================================================

    # void be_code_ret(bfuncinfo *finfo, bexpdesc *e)
    # {
    #     if (finfo->binfo->prev == NULL) {
    #         if (finfo->flags & FUNC_RET_FLAG) {
    #             return;
    #         }
    #         finfo->flags |= FUNC_RET_FLAG;
    #     }
    #     if (e) {
    #         int reg = exp2anyreg(finfo, e);
    #         be_code_close(finfo, 1);
    #         leave_function(finfo);
    #         codeABC(finfo, OP_RET, e->type != ETVOID, reg, 0);
    #         free_expreg(finfo, e);
    #     } else {
    #         be_code_close(finfo, 1);
    #         codeABC(finfo, OP_RET, 0, 0, 0);
    #     }
    # }
    def be_code_ret(finfo, e):
        """Emit return instruction."""
        if finfo.binfo.prev is None:
            if finfo.flags & FUNC_RET_FLAG:
                return
            finfo.flags |= FUNC_RET_FLAG
        if e is not None:
            reg = exp2anyreg(finfo, e)
            be_code_close(finfo, 1)
            leave_function(finfo)
            codeABC(finfo, OP_RET, 1 if e.type != ETVOID else 0, reg, 0)
            free_expreg(finfo, e)
        else:
            be_code_close(finfo, 1)
            codeABC(finfo, OP_RET, 0, 0, 0)

    # ========================================================================
    # package_suffix — Package a suffix object from c with key k
    # ========================================================================

    # static void package_suffix(bfuncinfo *finfo, bexpdesc *c, bexpdesc *k)
    # {
    #     c->v.ss.obj = exp2anyreg(finfo, c);
    #     int key = exp2anyreg(finfo, k);
    #     c->v.ss.tt = c->type;
    #     c->v.ss.idx = key;
    # }
    def package_suffix(finfo, c, k):
        """Package a suffix object from c with key k."""
        c.v.ss.obj = exp2anyreg(finfo, c)
        key = exp2anyreg(finfo, k)
        c.v.ss.tt = c.type
        c.v.ss.idx = key

    # ========================================================================
    # be_code_resolve — resolve expression to register
    # ========================================================================

    # int be_code_resolve(bfuncinfo *finfo, bexpdesc *k)
    # {
    #     return exp2anyreg(finfo, k);
    # }
    def be_code_resolve(finfo, k):
        """Resolve expression to register."""
        return exp2anyreg(finfo, k)

    # ========================================================================
    # be_code_member / be_code_index — Package suffix objects
    # ========================================================================

    # void be_code_member(bfuncinfo *finfo, bexpdesc *c, bexpdesc *k)
    # {
    #     package_suffix(finfo, c, k);
    #     c->type = ETMEMBER;
    # }
    def be_code_member(finfo, c, k):
        """Package a MEMBER suffix object from c with key k."""
        package_suffix(finfo, c, k)
        c.type = ETMEMBER

    # void be_code_index(bfuncinfo *finfo, bexpdesc *c, bexpdesc *k)
    # {
    #     package_suffix(finfo, c, k);
    #     c->type = ETINDEX;
    # }
    def be_code_index(finfo, c, k):
        """Package an INDEX suffix object from c with key k."""
        package_suffix(finfo, c, k)
        c.type = ETINDEX

    # ========================================================================
    # be_code_class — emit CLASS instruction
    # ========================================================================

    # void be_code_class(bfuncinfo *finfo, bexpdesc *dst, bclass *c)
    # {
    #     int src;
    #     bvalue var = {0};
    #     var_setclass(&var, c);
    #     src = newconst(finfo, &var);
    #     if (dst->type == ETLOCAL) {
    #         codeABx(finfo, OP_LDCONST, dst->v.idx, src);
    #     } else if (dst->type == ETGLOBAL) {
    #         codeABx(finfo, OP_LDCONST, finfo->freereg, src);
    #         codeABx(finfo, OP_SETGBL, finfo->freereg, dst->v.idx);
    #     } else if (dst->type == ETNGLOBAL) {
    #         codeABx(finfo, OP_LDCONST, finfo->freereg, src);
    #         codeABC(finfo, OP_SETNGBL, finfo->freereg, dst->v.idx, 0);
    #     }
    #     codeABx(finfo, OP_CLASS, 0, src);
    # }
    def be_code_class(finfo, dst, c):
        """Emit CLASS instruction to register class."""
        var = bvalue()
        var_setclass(var, c)
        src = newconst(finfo, var)
        if dst.type == ETLOCAL:
            codeABx(finfo, OP_LDCONST, dst.v.idx, src)
        elif dst.type == ETGLOBAL:
            codeABx(finfo, OP_LDCONST, finfo.freereg, src)
            codeABx(finfo, OP_SETGBL, finfo.freereg, dst.v.idx)
        elif dst.type == ETNGLOBAL:
            codeABx(finfo, OP_LDCONST, finfo.freereg, src)
            codeABC(finfo, OP_SETNGBL, finfo.freereg, dst.v.idx, 0)
        codeABx(finfo, OP_CLASS, 0, src)

    # ========================================================================
    # be_code_setsuper — emit SETSUPER instruction
    # ========================================================================

    # void be_code_setsuper(bfuncinfo *finfo, bexpdesc *c, bexpdesc *s)
    # {
    #     int self = exp2anyreg(finfo, c);
    #     int super = exp2anyreg(finfo, s);
    #     codeABC(finfo, OP_SETSUPER, self, super, 0);
    #     free_expreg(finfo, c);
    #     free_expreg(finfo, s);
    # }
    def be_code_setsuper(finfo, c, s):
        """Emit SETSUPER instruction."""
        self_ = exp2anyreg(finfo, c)
        super_ = exp2anyreg(finfo, s)
        codeABC(finfo, OP_SETSUPER, self_, super_, 0)
        free_expreg(finfo, c)
        free_expreg(finfo, s)


    # ========================================================================
    # be_code_import — emit IMPORT opcode
    # ========================================================================

    # void be_code_import(bfuncinfo *finfo, bexpdesc *m, bexpdesc *v)
    # {
    #     int dst, src = exp2anyreg(finfo, m);
    #     if (v->type == ETLOCAL) {
    #         dst = v->v.idx;
    #         codeABC(finfo, OP_IMPORT, dst, src, 0);
    #     } else {
    #         dst = be_code_allocregs(finfo, 1);
    #         codeABC(finfo, OP_IMPORT, dst, src, 0);
    #         m->type = ETREG;
    #         m->v.idx = dst;
    #         be_code_setvar(finfo, v, m, bfalse);
    #     }
    # }
    def be_code_import(finfo, m, v):
        """Emit IMPORT opcode for import module."""
        src = exp2anyreg(finfo, m)
        if v.type == ETLOCAL:
            dst = v.v.idx
            codeABC(finfo, OP_IMPORT, dst, src, 0)
        else:
            dst = be_code_allocregs(finfo, 1)
            codeABC(finfo, OP_IMPORT, dst, src, 0)
            m.type = ETREG
            m.v.idx = dst
            be_code_setvar(finfo, v, m, bfalse)

    # ========================================================================
    # be_code_exblk — emit EXBLK instruction
    # ========================================================================

    # int be_code_exblk(bfuncinfo *finfo, int depth)
    # {
    #     if (depth == 0) {
    #         return appendjump(finfo, OP_EXBLK, NULL);
    #     }
    #     codeABx(finfo, OP_EXBLK, 1, depth);
    #     return 0;
    # }
    def be_code_exblk(finfo, depth):
        """Emit EXBLK instruction for exception block management."""
        if depth == 0:
            return appendjump(finfo, OP_EXBLK, None)
        codeABx(finfo, OP_EXBLK, 1, depth)
        return 0

    # ========================================================================
    # be_code_catch — emit CATCH instruction
    # ========================================================================

    # void be_code_catch(bfuncinfo *finfo, int base, int ecnt, int vcnt, int *jmp)
    # {
    #     codeABC(finfo, OP_CATCH, base, ecnt, vcnt);
    #     if (jmp) {
    #         *jmp = NO_JUMP;
    #         be_code_conjump(finfo, jmp, be_code_jump(finfo));
    #     }
    # }
    #
    # Note: In Python, jmp is passed as a list [value] to allow mutation.
    # The caller passes [jmp_val] and reads back [0] after the call.
    def be_code_catch(finfo, base, ecnt, vcnt, jmp):
        """Emit CATCH instruction."""
        codeABC(finfo, OP_CATCH, base, ecnt, vcnt)
        if jmp is not None:
            jmp[0] = NO_JUMP
            jmp[0] = be_code_conjump(finfo, jmp[0], be_code_jump(finfo))

    # ========================================================================
    # be_code_raise — emit RAISE opcode
    # ========================================================================

    # void be_code_raise(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2)
    # {
    #     if (e1) {
    #         int src1 = exp2anyreg(finfo, e1);
    #         int src2 = e2 ? exp2anyreg(finfo, e2) : 0;
    #         codeABC(finfo, OP_RAISE, e2 != NULL, src1, src2);
    #     } else {
    #         codeABC(finfo, OP_RAISE, 2, 0, 0);
    #     }
    #     free_expreg(finfo, e1);
    #     free_expreg(finfo, e2);
    # }
    def be_code_raise(finfo, e1, e2):
        """Emit RAISE opcode."""
        if e1 is not None:
            src1 = exp2anyreg(finfo, e1)
            src2 = exp2anyreg(finfo, e2) if e2 is not None else 0
            codeABC(finfo, OP_RAISE, 1 if e2 is not None else 0, src1, src2)
        else:
            codeABC(finfo, OP_RAISE, 2, 0, 0)
        free_expreg(finfo, e1)
        free_expreg(finfo, e2)

    # ========================================================================
    # be_code_implicit_class — create implicit class constant
    # ========================================================================

    # void be_code_implicit_class(bfuncinfo *finfo, bexpdesc *e, bclass *c)
    # {
    #     bvalue k = {0};
    #     k.type = BE_CLASS;
    #     k.v.p = c;
    #     int idx = newconst(finfo, &k);
    #     e->type = ETCONST;
    #     e->v.idx = setK(idx);
    # }
    def be_code_implicit_class(finfo, e, c):
        """Create implicit class constant in the constant table."""
        k = bvalue()
        k.type = BE_CLASS
        k.v = c
        idx = newconst(finfo, k)
        e.type = ETCONST
        e.v.idx = setK(idx)

# end if BE_USE_SCRIPT_COMPILER
