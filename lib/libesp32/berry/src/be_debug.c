/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_debug.h"
#include "be_func.h"
#include "be_decoder.h"
#include "be_string.h"
#include "be_class.h"
#include "be_vm.h"
#include "be_vector.h"
#include "be_strlib.h"
#include "be_exec.h"
#include "be_mem.h"
#include "be_sys.h"
#include <stdio.h>
#include <string.h>

#if BE_USE_DEBUG_HOOK && !BE_DEBUG_RUNTIME_INFO
  #error This macro BE_USE_DEBUG_HOOK requires BE_DEBUG_RUNTIME_INFO != 0
#endif

#ifndef INST_BUF_SIZE
#define INST_BUF_SIZE   96
#endif

#define logbuf(...)     snprintf(__lbuf, sizeof(__lbuf), __VA_ARGS__)

#define logfmt(...)                     \
    do {                                \
        char __lbuf[INST_BUF_SIZE];     \
        logbuf(__VA_ARGS__);            \
        be_writestring(__lbuf);         \
    } while (0)

#if BE_USE_DEBUG_MODULE
static const char* opc2str(bopcode op)
{
    static const char* const opc_tab[] = {
        #define OPCODE(opc) #opc
        #include "be_opcodes.h"
        #undef OPCODE
    };
    return op < array_count(opc_tab) ? opc_tab[op] : "ERROP";
}

void be_print_inst(binstruction ins, int pc, void* fout)
{
    char __lbuf[INST_BUF_SIZE + 1], __lbuf_tmp[INST_BUF_SIZE];
    bopcode op = IGET_OP(ins);

    logbuf("  %.4X  ", pc);
    if (fout) {
        be_fwrite(fout, __lbuf, strlen(__lbuf));
    } else {
        be_writestring(__lbuf);
    }                            
    switch (op) {
    case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
    case OP_MOD: case OP_LT: case OP_LE: case OP_EQ:
    case OP_NE:  case OP_GT:  case OP_GE: case OP_CONNECT:
    case OP_GETMBR: case OP_SETMBR:  case OP_GETMET: case OP_SETMET:
    case OP_GETIDX: case OP_SETIDX: case OP_AND:
    case OP_OR: case OP_XOR: case OP_SHL: case OP_SHR:
        logbuf("%s\tR%d\t%c%d\t%c%d", opc2str(op), IGET_RA(ins),
                isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK,
                isKC(ins) ? 'K' : 'R', IGET_RKC(ins) & KR_MASK);
        break;
    case OP_GETNGBL: case OP_SETNGBL:
        logbuf("%s\tR%d\t%c%d", opc2str(op), IGET_RA(ins),
                isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
        break;
    case OP_GETGBL: case OP_SETGBL:
        logbuf("%s\tR%d\tG%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
        break;
    case OP_MOVE: case OP_SETSUPER: case OP_NEG: case OP_FLIP: case OP_IMPORT:
        logbuf("%s\tR%d\t%c%d", opc2str(op), IGET_RA(ins),
                isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
        break;
    case OP_JMP:
        logbuf("%s\t\t#%.4X", opc2str(op), IGET_sBx(ins) + pc + 1);
        break;
    case OP_JMPT: case OP_JMPF:
        logbuf("%s\tR%d\t#%.4X", opc2str(op), IGET_RA(ins), IGET_sBx(ins) + pc + 1);
        break;
    case OP_LDINT:
        logbuf("%s\tR%d\t%d", opc2str(op), IGET_RA(ins), IGET_sBx(ins));
        break;
    case OP_LDBOOL:
        logbuf("%s\tR%d\t%d\t%d", opc2str(op),  IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins));
        break;
    case OP_RET:
        if (IGET_RA(ins)) {
            logbuf("%s\t%d\t%c%d", opc2str(op), IGET_RA(ins),
                isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
        } else {
            logbuf("%s\t%d", opc2str(op), IGET_RA(ins)); /* RET 0 does not take an additional parameter */
        }
        break;
    case OP_GETUPV: case OP_SETUPV:
        logbuf("%s\tR%d\tU%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
        break;
    case OP_LDCONST:
        logbuf("%s\tR%d\tK%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
        break;
    case OP_CALL:
        logbuf("%s\tR%d\t%d", opc2str(op), IGET_RA(ins), IGET_RKB(ins));
        break;
    case OP_CLOSURE:
        logbuf("%s\tR%d\tP%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
        break;
    case OP_CLASS:
        logbuf("%s\tK%d", opc2str(op), IGET_Bx(ins));
        break;
    case OP_CLOSE: case OP_LDNIL:
        logbuf("%s\tR%d", opc2str(op), IGET_RA(ins));
        break;
    case OP_RAISE:
        logbuf("%s\t%d\t%c%d\t%c%d", opc2str(op), IGET_RA(ins),
                isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK,
                isKC(ins) ? 'K' : 'R', IGET_RKC(ins) & KR_MASK);
        break;
    case OP_EXBLK:
        if (IGET_RA(ins)) {
            logbuf("%s\t%d\t%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
        } else {
            logbuf("%s\t%d\t#%.4X", opc2str(op), IGET_RA(ins), IGET_sBx(ins) + pc + 1);
        }
        break;
    case OP_CATCH:
        logbuf("%s\tR%d\t%d\t%d", opc2str(op), IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins));
        break;
    default:
        logbuf("%s", opc2str(op));
        break;
    }
    memcpy(__lbuf_tmp, __lbuf, strlen(__lbuf) + 1);
    logbuf("%s\n", __lbuf_tmp);
    if (fout) {
        be_fwrite(fout, __lbuf, strlen(__lbuf));
    } else {
        be_writestring(__lbuf);
    }                   
}
#endif

#if BE_USE_DEBUG_MODULE
void be_dumpclosure(bclosure *cl)
{
    int pc;
    bproto *proto = cl->proto;
    binstruction *code = proto->code;
#if BE_DEBUG_RUNTIME_INFO
    blineinfo *lineinfo = proto->lineinfo;
#endif
#if BE_DEBUG_SOURCE_FILE
    logfmt("source '%s', ", str(proto->source));
#endif
    logfmt("function '%s':\n", str(proto->name));
#if BE_DEBUG_RUNTIME_INFO
    if (lineinfo) { /* first line */
        logfmt("; line %d\n", lineinfo->linenumber);
    }
#endif
    for (pc = 0; pc < proto->codesize; pc++) {
#if BE_DEBUG_RUNTIME_INFO
        if (lineinfo && pc > lineinfo->endpc) {
            logfmt("; line %d\n", (++lineinfo)->linenumber);
        }
#endif
        be_print_inst(*code++, pc, NULL);
    }
}
#endif

static void sourceinfo(bproto *proto, binstruction *ip)
{
#if BE_DEBUG_RUNTIME_INFO
    char buf[24];
    be_assert(proto != NULL);
    if (proto->lineinfo && proto->nlineinfo) {
        blineinfo *it = proto->lineinfo;
        blineinfo *end = it + proto->nlineinfo;
        int pc = cast_int(ip - proto->code - 1); /* now vm->ip has been increased */
        for (; it < end && pc > it->endpc; ++it);
        snprintf(buf, sizeof(buf), ":%d:", it->linenumber);
#if BE_DEBUG_SOURCE_FILE
        be_writestring(str(proto->source));
#endif
        be_writestring(buf);
    } else {
        be_writestring("<unknown source>:");
    }
#else
    (void)proto; (void)ip;
    be_writestring("<unknown source>:");
#endif
}

static void tracestack(bvm *vm)
{
    bcallsnapshot *cf;
    bcallsnapshot *base = be_stack_base(&vm->tracestack);
    bcallsnapshot *top = be_stack_top(&vm->tracestack);
    be_writestring("stack traceback:\n");
    for (cf = top; cf >= base; --cf) {
        if (cf <= top - 10 && cf >= base + 10) {
            if (cf == top - 10)
                be_writestring("\t...\n");
            continue;
        }
        if (var_isclosure(&cf->func)) {
            bclosure *cl = var_toobj(&cf->func);
            be_writestring("\t");
            sourceinfo(cl->proto, cf->ip);
            be_writestring(" in function `");
            be_writestring(str(cl->proto->name));
            be_writestring("`\n");
        } else {
            be_writestring("\t<native>: in native function\n");
        }
    }
}

static void repair_stack(bvm *vm)
{
    bcallsnapshot *cf;
    bcallsnapshot *base = be_stack_base(&vm->tracestack);
    bcallsnapshot *top = be_stack_top(&vm->tracestack);
    /* Because the native function does not push `ip` to the
     * stack, the ip on the native function frame corresponds
     * to the previous Berry closure. */
    for (cf = top; cf >= base; --cf) {
        if (!var_isclosure(&cf->func)) {
            /* the last native function stack frame has the `ip` of
             * the previous Berry frame */
            binstruction *ip = cf->ip;
            /* skip native function stack frames */
            for (; cf >= base && !var_isclosure(&cf->func); --cf);
            /* fixed `ip` of Berry closure frame near native function frame */
            if (cf >= base) cf->ip = ip;
        }
    }
}

void be_tracestack(bvm *vm)
{
    if (be_stack_count(&vm->tracestack)) {
        repair_stack(vm);
        tracestack(vm);
    }
}

#if BE_USE_DEBUG_HOOK

static void hook_callnative(bvm *vm, int mask)
{
    bhookinfo_t info;
    int top = be_top(vm);
    bcallframe *cf = vm->cf;
    bclosure *cl = var_toobj(cf->func);
    struct bhookblock *hb = var_toobj(&vm->hook);
    be_stack_require(vm, BE_STACK_FREE_MIN + 2);
    info.type = mask;
    info.line = cf->lineinfo->linenumber;
#if BE_DEBUG_SOURCE_FILE
    info.source = str(cl->proto->source);
#endif
    info.func_name = str(cl->proto->name);
    info.data = hb->data;
    hb->hook(vm, &info);
    vm->top += 2;
    be_pop(vm, be_top(vm) - top);
}

static int hook_pushargs(bvm *vm, int mask)
{
    bcallframe *cf = vm->cf;
    if (mask == BE_HOOK_LINE) {
        be_pushstring(vm, "line");
        be_pushint(vm, cf->lineinfo->linenumber);
        return 2;
    }
    if (mask == BE_HOOK_CALL) {
        bclosure *cl = var_toobj(cf->func);
        be_pushstring(vm, "call");
        var_setstr(vm->top, cl->proto->name);
        vm->top++;
        return 2;
    }
    if (mask == BE_HOOK_RET) {
        be_pushstring(vm, "return");
        return 1;
    }
    return 0;
}

static void hook_call(bvm *vm, int mask)
{
    int argc;
    vm->top[2] = vm->hook;
    be_stack_require(vm, 5);
    vm->top += 3;
    argc = hook_pushargs(vm, mask);
    be_call(vm, argc);
    be_pop(vm, 3 + argc);
}

void be_callhook(bvm *vm, int mask)
{
    if (vm->hookmask & mask) {
        int hookmask = vm->hookmask;
        vm->hookmask = 0;
        if (var_istype(&vm->hook, BE_COMPTR)) {
            hook_callnative(vm, mask);
        } else {
            hook_call(vm, mask);
        }
        vm->hookmask = (bbyte)hookmask;
    }
}

static bbyte parse_hookmask(const char *mask)
{
    int c, res = 0;
    if (mask) {
        while ((c = *mask++) != '\0') {
            switch (c) {
            case 'l': res |= BE_HOOK_LINE; break;
            case 'c': res |= BE_HOOK_CALL; break;
            case 'r': res |= BE_HOOK_RET; break;
            default: break;
            }
        }
    }
    return (bbyte)res;
}

BERRY_API void be_sethook(bvm *vm, const char *mask)
{
    vm->hookmask = parse_hookmask(mask);
    if (vm->hookmask && var_istype(&vm->hook, BE_COMPTR)) /* free native hook */
        be_free(vm, var_toobj(&vm->hook), sizeof(struct bhookblock));
    if (vm->hookmask) {
        vm->hook = *be_indexof(vm, -1);
    } else if (!var_istype(&vm->hook, BE_COMPTR)) {
        var_setnil(&vm->hook);
    }
}

BERRY_API void be_setntvhook(bvm *vm, bntvhook hook, void *data, int mask)
{
    struct bhookblock *hb;
    if (mask) {
        if (!var_istype(&vm->hook, BE_COMPTR)) {
            var_setobj(&vm->hook, BE_COMPTR,
                be_malloc(vm, sizeof(struct bhookblock)));
        }
        hb = var_toobj(&vm->hook);
        be_assert(hb != NULL);
        hb->hook = hook;
        hb->data = data;
    } else if (!var_istype(&vm->hook, BE_COMPTR)) {
        var_setnil(&vm->hook);
    }
    vm->hookmask = (bbyte)mask;
}

#endif

#if BE_DEBUG_VAR_INFO
static binstruction* callstack_fixip(bvm *vm, int level)
{
    bcallframe *top = (bcallframe*)be_stack_top(&vm->callstack);
    bcallframe *cf = top - level + 2;
    for (; cf <= top && cf->status & PRIM_FUNC; ++cf);
    return cf <= top ? cf->ip : vm->ip;
}

bbool be_debug_varname(bvm *vm, int level, int index)
{
    int depth = be_stack_count(&vm->callstack);
    if (level > 0 && level <= depth) {
        bcallframe *cf = be_vector_at(&vm->callstack, depth - level);
        if ((cf->status & PRIM_FUNC) == 0) {
            bproto *proto = cast(bclosure*, var_toobj(cf->func))->proto;
            int pc = (int)(callstack_fixip(vm, level) - proto->code);
            bstring *name = be_func_varname(proto, index, pc);
            if (name) {
                bvalue *reg = be_incrtop(vm);
                var_setstr(reg, name);
                return btrue;
            }
        }
    }
    return bfalse;
}

bbool be_debug_upvname(bvm *vm, int level, int index)
{
    int depth = be_stack_count(&vm->callstack);
    if (level > 0 && level <= depth) {
        bcallframe *cf = be_vector_at(&vm->callstack, depth - level);
        if ((cf->status & PRIM_FUNC) == 0) {
            bproto *proto = cast(bclosure*, var_toobj(cf->func))->proto;
            if (index >= 0 && index < proto->nupvals) {
                bvalue *reg = be_incrtop(vm);
                var_setstr(reg, proto->upvals[index].name);
                return btrue;
            }
        }
    }
    return bfalse;
}
#endif
