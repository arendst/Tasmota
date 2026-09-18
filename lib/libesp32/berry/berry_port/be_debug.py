#/********************************************************************
#** Copyright (c) 2018-2020 Guan Wenliang
#** This file is part of the Berry default interpreter.
#** skiars@qq.com, https://github.com/Skiars/berry
#** See Copyright Notice in the LICENSE file or at
#** https://github.com/Skiars/berry/blob/master/LICENSE
#********************************************************************/
#
# Python port of src/be_debug.c and src/be_debug.h
# All C macro code is included as comments.
# C function/macro names are preserved as Python function names.

import sys

from berry_port.berry_conf import (
    BE_USE_DEBUG_MODULE,
    BE_USE_DEBUG_HOOK,
    BE_DEBUG_RUNTIME_INFO,
    BE_DEBUG_SOURCE_FILE,
    BE_DEBUG_VAR_INFO,
    BE_STACK_FREE_MIN,
)

from berry_port.be_object import (
    BE_CLOSURE, BE_COMPTR,
    PRIM_FUNC,
    bvalue, bclosure, bcallsnapshot,
    var_isclosure, var_istype, var_toobj, var_setstr, var_setnil, var_setobj,
)

from berry_port.be_decoder import (
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx, IGET_sBx,
    isKB, isKC, KR_MASK,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
    OP_CONNECT, OP_NEG, OP_FLIP,
    OP_LDNIL, OP_LDBOOL, OP_LDINT, OP_LDCONST,
    OP_MOVE,
    OP_GETGBL, OP_SETGBL, OP_GETUPV, OP_SETUPV,
    OP_JMP, OP_JMPT, OP_JMPF,
    OP_CALL, OP_RET, OP_CLOSURE,
    OP_GETMBR, OP_GETMET, OP_SETMBR,
    OP_GETIDX, OP_SETIDX,
    OP_SETSUPER, OP_CLOSE, OP_IMPORT,
    OP_EXBLK, OP_CATCH, OP_RAISE,
    OP_CLASS,
    OP_GETNGBL, OP_SETNGBL,
    opcode_names,
)

from berry_port.be_string import be_str2cstr


# ============================================================================
# Opcode-to-string helper
# ============================================================================

# static const char* opc2str(bopcode op)
# {
#     static const char* const opc_tab[] = {
#         #define OPCODE(opc) #opc
#         #include "be_opcodes.h"
#         #undef OPCODE
#     };
#     return op < array_count(opc_tab) ? opc_tab[op] : "ERROP";
# }
def opc2str(op):
    """Return the string name for an opcode, or 'ERROP' if out of range."""
    if 0 <= op < len(opcode_names):
        return opcode_names[op]
    return "ERROP"


# ============================================================================
# Instruction disassembler
# ============================================================================

if BE_USE_DEBUG_MODULE:

    # void be_print_inst(binstruction ins, int pc, void* fout)
    # {
    #     char __lbuf[INST_BUF_SIZE + 1], __lbuf_tmp[INST_BUF_SIZE];
    #     bopcode op = IGET_OP(ins);
    #
    #     logbuf("  %.4X  ", pc);
    #     if (fout) {
    #         be_fwrite(fout, __lbuf, strlen(__lbuf));
    #     } else {
    #         be_writestring(__lbuf);
    #     }
    #     switch (op) {
    #     case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
    #     case OP_MOD: case OP_LT: case OP_LE: case OP_EQ:
    #     case OP_NE:  case OP_GT:  case OP_GE: case OP_CONNECT:
    #     case OP_GETMBR: case OP_SETMBR:  case OP_GETMET:
    #     case OP_GETIDX: case OP_SETIDX: case OP_AND:
    #     case OP_OR: case OP_XOR: case OP_SHL: case OP_SHR:
    #         logbuf("%s\tR%d\t%c%d\t%c%d", opc2str(op), IGET_RA(ins),
    #                 isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK,
    #                 isKC(ins) ? 'K' : 'R', IGET_RKC(ins) & KR_MASK);
    #         break;
    #     case OP_GETNGBL: case OP_SETNGBL:
    #         logbuf("%s\tR%d\t%c%d", opc2str(op), IGET_RA(ins),
    #                 isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
    #         break;
    #     case OP_GETGBL: case OP_SETGBL:
    #         logbuf("%s\tR%d\tG%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
    #         break;
    #     case OP_MOVE: case OP_SETSUPER: case OP_NEG: case OP_FLIP: case OP_IMPORT:
    #         logbuf("%s\tR%d\t%c%d", opc2str(op), IGET_RA(ins),
    #                 isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
    #         break;
    #     case OP_JMP:
    #         logbuf("%s\t\t#%.4X", opc2str(op), IGET_sBx(ins) + pc + 1);
    #         break;
    #     case OP_JMPT: case OP_JMPF:
    #         logbuf("%s\tR%d\t#%.4X", opc2str(op), IGET_RA(ins), IGET_sBx(ins) + pc + 1);
    #         break;
    #     case OP_LDINT:
    #         logbuf("%s\tR%d\t%d", opc2str(op), IGET_RA(ins), IGET_sBx(ins));
    #         break;
    #     case OP_LDBOOL:
    #         logbuf("%s\tR%d\t%d\t%d", opc2str(op),  IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins));
    #         break;
    #     case OP_RET:
    #         if (IGET_RA(ins)) {
    #             logbuf("%s\t%d\t%c%d", opc2str(op), IGET_RA(ins),
    #                 isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK);
    #         } else {
    #             logbuf("%s\t%d", opc2str(op), IGET_RA(ins));
    #         }
    #         break;
    #     case OP_GETUPV: case OP_SETUPV:
    #         logbuf("%s\tR%d\tU%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
    #         break;
    #     case OP_LDCONST:
    #         logbuf("%s\tR%d\tK%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
    #         break;
    #     case OP_CALL:
    #         logbuf("%s\tR%d\t%d", opc2str(op), IGET_RA(ins), IGET_RKB(ins));
    #         break;
    #     case OP_CLOSURE:
    #         logbuf("%s\tR%d\tP%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
    #         break;
    #     case OP_CLASS:
    #         logbuf("%s\tK%d", opc2str(op), IGET_Bx(ins));
    #         break;
    #     case OP_CLOSE: case OP_LDNIL:
    #         logbuf("%s\tR%d", opc2str(op), IGET_RA(ins));
    #         break;
    #     case OP_RAISE:
    #         logbuf("%s\t%d\t%c%d\t%c%d", opc2str(op), IGET_RA(ins),
    #                 isKB(ins) ? 'K' : 'R', IGET_RKB(ins) & KR_MASK,
    #                 isKC(ins) ? 'K' : 'R', IGET_RKC(ins) & KR_MASK);
    #         break;
    #     case OP_EXBLK:
    #         if (IGET_RA(ins)) {
    #             logbuf("%s\t%d\t%d", opc2str(op), IGET_RA(ins), IGET_Bx(ins));
    #         } else {
    #             logbuf("%s\t%d\t#%.4X", opc2str(op), IGET_RA(ins), IGET_sBx(ins) + pc + 1);
    #         }
    #         break;
    #     case OP_CATCH:
    #         logbuf("%s\tR%d\t%d\t%d", opc2str(op), IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins));
    #         break;
    #     default:
    #         logbuf("%s", opc2str(op));
    #         break;
    #     }
    #     memcpy(__lbuf_tmp, __lbuf, strlen(__lbuf) + 1);
    #     logbuf("%s\n", __lbuf_tmp);
    #     if (fout) {
    #         be_fwrite(fout, __lbuf, strlen(__lbuf));
    #     } else {
    #         be_writestring(__lbuf);
    #     }
    # }
    def be_print_inst(ins, pc, fout=None):
        """Disassemble a single Berry instruction.

        Produces output matching the C implementation's format exactly.
        The first part is the PC prefix "  %.4X  ", then the instruction
        mnemonic and operands, then a newline.

        If *fout* is a file-like object, output is written there;
        otherwise it is written to stdout.
        """
        op = IGET_OP(ins)

        # PC prefix: "  %.4X  "
        prefix = "  %04X  " % (pc & 0xFFFF)

        # Build the operand string based on opcode
        _ABC_OPS = {
            OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
            OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
            OP_CONNECT,
            OP_GETMBR, OP_SETMBR, OP_GETMET,
            OP_GETIDX, OP_SETIDX,
            OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
        }

        if op in _ABC_OPS:
            operands = "%s\tR%d\t%c%d\t%c%d" % (
                opc2str(op), IGET_RA(ins),
                'K' if isKB(ins) else 'R', IGET_RKB(ins) & KR_MASK,
                'K' if isKC(ins) else 'R', IGET_RKC(ins) & KR_MASK,
            )
        elif op in (OP_GETNGBL, OP_SETNGBL):
            operands = "%s\tR%d\t%c%d" % (
                opc2str(op), IGET_RA(ins),
                'K' if isKB(ins) else 'R', IGET_RKB(ins) & KR_MASK,
            )
        elif op in (OP_GETGBL, OP_SETGBL):
            operands = "%s\tR%d\tG%d" % (opc2str(op), IGET_RA(ins), IGET_Bx(ins))
        elif op in (OP_MOVE, OP_SETSUPER, OP_NEG, OP_FLIP, OP_IMPORT):
            operands = "%s\tR%d\t%c%d" % (
                opc2str(op), IGET_RA(ins),
                'K' if isKB(ins) else 'R', IGET_RKB(ins) & KR_MASK,
            )
        elif op == OP_JMP:
            operands = "%s\t\t#%04X" % (opc2str(op), (IGET_sBx(ins) + pc + 1) & 0xFFFF)
        elif op in (OP_JMPT, OP_JMPF):
            operands = "%s\tR%d\t#%04X" % (
                opc2str(op), IGET_RA(ins),
                (IGET_sBx(ins) + pc + 1) & 0xFFFF,
            )
        elif op == OP_LDINT:
            operands = "%s\tR%d\t%d" % (opc2str(op), IGET_RA(ins), IGET_sBx(ins))
        elif op == OP_LDBOOL:
            operands = "%s\tR%d\t%d\t%d" % (
                opc2str(op), IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins),
            )
        elif op == OP_RET:
            if IGET_RA(ins):
                operands = "%s\t%d\t%c%d" % (
                    opc2str(op), IGET_RA(ins),
                    'K' if isKB(ins) else 'R', IGET_RKB(ins) & KR_MASK,
                )
            else:
                operands = "%s\t%d" % (opc2str(op), IGET_RA(ins))
        elif op in (OP_GETUPV, OP_SETUPV):
            operands = "%s\tR%d\tU%d" % (opc2str(op), IGET_RA(ins), IGET_Bx(ins))
        elif op == OP_LDCONST:
            operands = "%s\tR%d\tK%d" % (opc2str(op), IGET_RA(ins), IGET_Bx(ins))
        elif op == OP_CALL:
            operands = "%s\tR%d\t%d" % (opc2str(op), IGET_RA(ins), IGET_RKB(ins))
        elif op == OP_CLOSURE:
            operands = "%s\tR%d\tP%d" % (opc2str(op), IGET_RA(ins), IGET_Bx(ins))
        elif op == OP_CLASS:
            operands = "%s\tK%d" % (opc2str(op), IGET_Bx(ins))
        elif op in (OP_CLOSE, OP_LDNIL):
            operands = "%s\tR%d" % (opc2str(op), IGET_RA(ins))
        elif op == OP_RAISE:
            operands = "%s\t%d\t%c%d\t%c%d" % (
                opc2str(op), IGET_RA(ins),
                'K' if isKB(ins) else 'R', IGET_RKB(ins) & KR_MASK,
                'K' if isKC(ins) else 'R', IGET_RKC(ins) & KR_MASK,
            )
        elif op == OP_EXBLK:
            if IGET_RA(ins):
                operands = "%s\t%d\t%d" % (opc2str(op), IGET_RA(ins), IGET_Bx(ins))
            else:
                operands = "%s\t%d\t#%04X" % (
                    opc2str(op), IGET_RA(ins),
                    (IGET_sBx(ins) + pc + 1) & 0xFFFF,
                )
        elif op == OP_CATCH:
            operands = "%s\tR%d\t%d\t%d" % (
                opc2str(op), IGET_RA(ins), IGET_RKB(ins), IGET_RKC(ins),
            )
        else:
            operands = "%s" % opc2str(op)

        # The C code outputs prefix first, then operands + newline
        line = prefix + operands + "\n"
        if fout is not None:
            fout.write(line)
        else:
            sys.stdout.write(line)


# ============================================================================
# Closure disassembler
# ============================================================================

if BE_USE_DEBUG_MODULE:

    # void be_dumpclosure(bclosure *cl)
    # {
    #     int pc;
    #     bproto *proto = cl->proto;
    #     binstruction *code = proto->code;
    # #if BE_DEBUG_RUNTIME_INFO
    #     blineinfo *lineinfo = proto->lineinfo;
    # #endif
    # #if BE_DEBUG_SOURCE_FILE
    #     logfmt("source '%s', ", str(proto->source));
    # #endif
    #     logfmt("function '%s':\n", str(proto->name));
    # #if BE_DEBUG_RUNTIME_INFO
    #     if (lineinfo) { /* first line */
    #         logfmt("; line %d\n", lineinfo->linenumber);
    #     }
    # #endif
    #     for (pc = 0; pc < proto->codesize; pc++) {
    # #if BE_DEBUG_RUNTIME_INFO
    #         if (lineinfo && pc > lineinfo->endpc) {
    #             logfmt("; line %d\n", (++lineinfo)->linenumber);
    #         }
    # #endif
    #         be_print_inst(*code++, pc, NULL);
    #     }
    # }
    def be_dumpclosure(cl):
        """Disassemble a Berry closure, printing source, function name,
        line info, and all instructions."""
        proto = cl.proto
        code = proto.code

        if BE_DEBUG_RUNTIME_INFO:
            lineinfo = proto.lineinfo
            lineinfo_idx = 0
        else:
            lineinfo = None
            lineinfo_idx = 0

        if BE_DEBUG_SOURCE_FILE and proto.source is not None:
            sys.stdout.write("source '%s', " % be_str2cstr(proto.source))

        sys.stdout.write("function '%s':\n" % be_str2cstr(proto.name))

        if BE_DEBUG_RUNTIME_INFO:
            if lineinfo and len(lineinfo) > 0:
                sys.stdout.write("; line %d\n" % lineinfo[lineinfo_idx].linenumber)

        for pc in range(proto.codesize):
            if BE_DEBUG_RUNTIME_INFO:
                if lineinfo and lineinfo_idx < len(lineinfo) and pc > lineinfo[lineinfo_idx].endpc:
                    lineinfo_idx += 1
                    if lineinfo_idx < len(lineinfo):
                        sys.stdout.write("; line %d\n" % lineinfo[lineinfo_idx].linenumber)
            be_print_inst(code[pc], pc)


# ============================================================================
# Source info helper (for stack traceback)
# ============================================================================

# static void sourceinfo(bproto *proto, binstruction *ip)
# {
# #if BE_DEBUG_RUNTIME_INFO
#     char buf[24];
#     be_assert(proto != NULL);
#     if (proto->lineinfo && proto->nlineinfo) {
#         blineinfo *it = proto->lineinfo;
#         blineinfo *end = it + proto->nlineinfo;
#         int pc = cast_int(ip - proto->code - 1);
#         for (; it < end && pc > it->endpc; ++it);
#         snprintf(buf, sizeof(buf), ":%d:", it->linenumber);
# #if BE_DEBUG_SOURCE_FILE
#         be_writestring(str(proto->source));
# #endif
#         be_writestring(buf);
#     } else {
#         be_writestring("<unknown source>:");
#     }
# #else
#     (void)proto; (void)ip;
#     be_writestring("<unknown source>:");
# #endif
# }
def _sourceinfo(proto, ip_idx):
    """Write source file and line number info for a given instruction index.

    *ip_idx* is the instruction pointer index into proto.code.
    In the C code, ip has already been incremented past the current
    instruction, so we use (ip_idx - 1) as the effective PC.
    """
    if BE_DEBUG_RUNTIME_INFO:
        if proto.lineinfo and proto.nlineinfo:
            lineinfo = proto.lineinfo
            nlineinfo = proto.nlineinfo
            pc = ip_idx - 1  # vm->ip has been incremented
            # Find the lineinfo entry covering this PC
            line_idx = 0
            while line_idx < nlineinfo and pc > lineinfo[line_idx].endpc:
                line_idx += 1
            if line_idx < nlineinfo:
                buf = ":%d:" % lineinfo[line_idx].linenumber
            else:
                buf = ":%d:" % lineinfo[nlineinfo - 1].linenumber
            if BE_DEBUG_SOURCE_FILE and proto.source is not None:
                sys.stdout.write(be_str2cstr(proto.source))
            sys.stdout.write(buf)
        else:
            sys.stdout.write("<unknown source>:")
    else:
        sys.stdout.write("<unknown source>:")


# ============================================================================
# Stack traceback
# ============================================================================

# static void tracestack(bvm *vm)
# {
#     bcallsnapshot *cf;
#     bcallsnapshot *base = be_stack_base(&vm->tracestack);
#     bcallsnapshot *top = be_stack_top(&vm->tracestack);
#     be_writestring("stack traceback:\n");
#     for (cf = top; cf >= base; --cf) {
#         if (cf <= top - 10 && cf >= base + 10) {
#             if (cf == top - 10)
#                 be_writestring("\t...\n");
#             continue;
#         }
#         if (var_isclosure(&cf->func)) {
#             bclosure *cl = var_toobj(&cf->func);
#             be_writestring("\t");
#             sourceinfo(cl->proto, cf->ip);
#             be_writestring(" in function `");
#             be_writestring(str(cl->proto->name));
#             be_writestring("`\n");
#         } else {
#             be_writestring("\t<native>: in native function\n");
#         }
#     }
# }
def _tracestack(vm):
    """Print the stack traceback from the tracestack snapshot.

    Iterates from top to base. If there are more than 20 frames,
    the middle frames are elided with '\\t...\\n'.
    """
    from berry_port.be_vector import be_stack_count as _be_stack_count

    tracestack = vm.tracestack
    count = _be_stack_count(tracestack)
    if count == 0:
        return

    data = tracestack.data
    # base_idx = 0, top_idx = count - 1
    base_idx = 0
    top_idx = count - 1

    sys.stdout.write("stack traceback:\n")

    # Iterate from top to base (reverse order)
    cf_idx = top_idx
    while cf_idx >= base_idx:
        # Elide middle frames when there are many
        if cf_idx <= top_idx - 10 and cf_idx >= base_idx + 10:
            if cf_idx == top_idx - 10:
                sys.stdout.write("\t...\n")
            cf_idx -= 1
            continue

        cf = data[cf_idx]
        if var_isclosure(cf.func):
            cl = var_toobj(cf.func)
            sys.stdout.write("\t")
            _sourceinfo(cl.proto, cf.ip)
            sys.stdout.write(" in function `")
            sys.stdout.write(be_str2cstr(cl.proto.name))
            sys.stdout.write("`\n")
        else:
            sys.stdout.write("\t<native>: in native function\n")

        cf_idx -= 1


# static void repair_stack(bvm *vm)
# {
#     bcallsnapshot *cf;
#     bcallsnapshot *base = be_stack_base(&vm->tracestack);
#     bcallsnapshot *top = be_stack_top(&vm->tracestack);
#     for (cf = top; cf >= base; --cf) {
#         if (!var_isclosure(&cf->func)) {
#             binstruction *ip = cf->ip;
#             for (; cf >= base && !var_isclosure(&cf->func); --cf);
#             if (cf >= base) cf->ip = ip;
#         }
#     }
# }
def _repair_stack(vm):
    """Repair instruction pointers in the trace stack.

    Because native functions do not push `ip` to the stack, the ip on
    a native function frame corresponds to the previous Berry closure.
    This function copies the native frame's ip down to the nearest
    Berry closure frame below it.
    """
    from berry_port.be_vector import be_stack_count as _be_stack_count

    tracestack = vm.tracestack
    count = _be_stack_count(tracestack)
    if count == 0:
        return

    data = tracestack.data
    base_idx = 0
    top_idx = count - 1

    cf_idx = top_idx
    while cf_idx >= base_idx:
        if not var_isclosure(data[cf_idx].func):
            # Save the ip from this native frame
            ip = data[cf_idx].ip
            # Skip past consecutive native frames
            while cf_idx >= base_idx and not var_isclosure(data[cf_idx].func):
                cf_idx -= 1
            # Fix the Berry closure frame's ip
            if cf_idx >= base_idx:
                data[cf_idx].ip = ip
        cf_idx -= 1


# void be_tracestack(bvm *vm)
# {
#     if (be_stack_count(&vm->tracestack)) {
#         repair_stack(vm);
#         tracestack(vm);
#     }
# }
def be_tracestack(vm):
    """Print stack traceback if the trace stack is non-empty."""
    from berry_port.be_vector import be_stack_count as _be_stack_count

    if _be_stack_count(vm.tracestack):
        _repair_stack(vm)
        _tracestack(vm)


# ============================================================================
# Debug hooks (BE_USE_DEBUG_HOOK)
# ============================================================================

if BE_USE_DEBUG_HOOK:

    # #define BE_HOOK_LINE    1
    # #define BE_HOOK_CALL    2
    # #define BE_HOOK_RET     4
    BE_HOOK_LINE = 1
    BE_HOOK_CALL = 2
    BE_HOOK_RET  = 4

    # struct bhookblock {
    #     void *data;
    #     bntvhook hook;
    # };
    class bhookblock:
        """Native debug hook block."""
        __slots__ = ('data', 'hook')
        def __init__(self):
            self.data = None
            self.hook = None

    # typedef struct {
    #     int type;
    #     int line;
    #     const char *source;
    #     const char *func_name;
    #     void *data;
    # } bhookinfo_t;
    class bhookinfo_t:
        """Hook callback info structure."""
        __slots__ = ('type', 'line', 'source', 'func_name', 'data')
        def __init__(self):
            self.type = 0
            self.line = 0
            self.source = None
            self.func_name = None
            self.data = None

    # static void hook_callnative(bvm *vm, int mask)
    # {
    #     bhookinfo_t info;
    #     int top = be_top(vm);
    #     bcallframe *cf = vm->cf;
    #     bclosure *cl = var_toobj(cf->func);
    #     struct bhookblock *hb = var_toobj(&vm->hook);
    #     be_stack_require(vm, BE_STACK_FREE_MIN + 2);
    #     info.type = mask;
    #     info.line = cf->lineinfo->linenumber;
    # #if BE_DEBUG_SOURCE_FILE
    #     info.source = str(cl->proto->source);
    # #endif
    #     info.func_name = str(cl->proto->name);
    #     info.data = hb->data;
    #     hb->hook(vm, &info);
    #     vm->top += 2;
    #     be_pop(vm, be_top(vm) - top);
    # }
    def _hook_callnative(vm, mask):
        """Call a native debug hook."""
        from berry_port.be_api import be_top, be_pop, be_stack_require
        top = be_top(vm)
        cf = vm.cf
        func_val = vm.stack[cf.func]
        cl = var_toobj(func_val)
        hb = var_toobj(vm.hook)
        be_stack_require(vm, BE_STACK_FREE_MIN + 2)
        info = bhookinfo_t()
        info.type = mask
        if cf.lineinfo is not None:
            info.line = cf.lineinfo.linenumber
        if BE_DEBUG_SOURCE_FILE and cl.proto.source is not None:
            info.source = be_str2cstr(cl.proto.source)
        info.func_name = be_str2cstr(cl.proto.name)
        info.data = hb.data
        hb.hook(vm, info)
        vm.top_idx += 2
        be_pop(vm, be_top(vm) - top)

    # static int hook_pushargs(bvm *vm, int mask)
    # {
    #     bcallframe *cf = vm->cf;
    #     if (mask == BE_HOOK_LINE) {
    #         be_pushstring(vm, "line");
    #         be_pushint(vm, cf->lineinfo->linenumber);
    #         return 2;
    #     }
    #     if (mask == BE_HOOK_CALL) {
    #         bclosure *cl = var_toobj(cf->func);
    #         be_pushstring(vm, "call");
    #         var_setstr(vm->top, cl->proto->name);
    #         vm->top++;
    #         return 2;
    #     }
    #     if (mask == BE_HOOK_RET) {
    #         be_pushstring(vm, "return");
    #         return 1;
    #     }
    #     return 0;
    # }
    def _hook_pushargs(vm, mask):
        """Push hook arguments onto the Berry stack. Returns argc."""
        from berry_port.be_api import be_pushstring, be_pushint
        cf = vm.cf
        if mask == BE_HOOK_LINE:
            be_pushstring(vm, "line")
            if cf.lineinfo is not None:
                be_pushint(vm, cf.lineinfo.linenumber)
            else:
                be_pushint(vm, 0)
            return 2
        if mask == BE_HOOK_CALL:
            func_val = vm.stack[cf.func]
            cl = var_toobj(func_val)
            be_pushstring(vm, "call")
            # var_setstr(vm->top, cl->proto->name); vm->top++;
            top_val = vm.stack[vm.top_idx]
            var_setstr(top_val, cl.proto.name)
            vm.top_idx += 1
            return 2
        if mask == BE_HOOK_RET:
            be_pushstring(vm, "return")
            return 1
        return 0

    # static void hook_call(bvm *vm, int mask)
    # {
    #     int argc;
    #     vm->top[2] = vm->hook;
    #     be_stack_require(vm, 5);
    #     vm->top += 3;
    #     argc = hook_pushargs(vm, mask);
    #     be_call(vm, argc);
    #     be_pop(vm, 3 + argc);
    # }
    def _hook_call(vm, mask):
        """Call a Berry-level debug hook."""
        from berry_port.be_api import be_call, be_pop, be_stack_require
        # vm->top[2] = vm->hook
        while vm.top_idx + 3 > len(vm.stack):
            vm.stack.append(bvalue())
        vm.stack[vm.top_idx + 2] = vm.hook
        be_stack_require(vm, 5)
        vm.top_idx += 3
        argc = _hook_pushargs(vm, mask)
        be_call(vm, argc)
        be_pop(vm, 3 + argc)

    # void be_callhook(bvm *vm, int mask)
    # {
    #     if (vm->hookmask & mask) {
    #         int hookmask = vm->hookmask;
    #         vm->hookmask = 0;
    #         if (var_istype(&vm->hook, BE_COMPTR)) {
    #             hook_callnative(vm, mask);
    #         } else {
    #             hook_call(vm, mask);
    #         }
    #         vm->hookmask = (bbyte)hookmask;
    #     }
    # }
    def be_callhook(vm, mask):
        """Invoke the debug hook if the mask matches."""
        if vm.hookmask & mask:
            hookmask = vm.hookmask
            vm.hookmask = 0
            if var_istype(vm.hook, BE_COMPTR):
                _hook_callnative(vm, mask)
            else:
                _hook_call(vm, mask)
            vm.hookmask = hookmask & 0xFF

    # static bbyte parse_hookmask(const char *mask)
    # {
    #     int c, res = 0;
    #     if (mask) {
    #         while ((c = *mask++) != '\0') {
    #             switch (c) {
    #             case 'l': res |= BE_HOOK_LINE; break;
    #             case 'c': res |= BE_HOOK_CALL; break;
    #             case 'r': res |= BE_HOOK_RET; break;
    #             default: break;
    #             }
    #         }
    #     }
    #     return (bbyte)res;
    # }
    def _parse_hookmask(mask):
        """Parse a hook mask string ('l', 'c', 'r') into a bitmask."""
        res = 0
        if mask:
            for c in mask:
                if c == 'l':
                    res |= BE_HOOK_LINE
                elif c == 'c':
                    res |= BE_HOOK_CALL
                elif c == 'r':
                    res |= BE_HOOK_RET
        return res & 0xFF

    # BERRY_API void be_sethook(bvm *vm, const char *mask)
    # {
    #     vm->hookmask = parse_hookmask(mask);
    #     if (vm->hookmask && var_istype(&vm->hook, BE_COMPTR))
    #         be_free(vm, var_toobj(&vm->hook), sizeof(struct bhookblock));
    #     if (vm->hookmask) {
    #         vm->hook = *be_indexof(vm, -1);
    #     } else if (!var_istype(&vm->hook, BE_COMPTR)) {
    #         var_setnil(&vm->hook);
    #     }
    # }
    def be_sethook(vm, mask):
        """Set a Berry-level debug hook from the top of the stack."""
        from berry_port.be_api import be_indexof
        vm.hookmask = _parse_hookmask(mask)
        if vm.hookmask and var_istype(vm.hook, BE_COMPTR):
            # Free native hook block — in Python just drop the reference
            pass
        if vm.hookmask:
            vm.hook = vm.stack[be_indexof(vm, -1)]
        elif not var_istype(vm.hook, BE_COMPTR):
            vm.hook = bvalue()
            var_setnil(vm.hook)

    # BERRY_API void be_setntvhook(bvm *vm, bntvhook hook, void *data, int mask)
    # {
    #     struct bhookblock *hb;
    #     if (mask) {
    #         if (!var_istype(&vm->hook, BE_COMPTR)) {
    #             var_setobj(&vm->hook, BE_COMPTR,
    #                 be_malloc(vm, sizeof(struct bhookblock)));
    #         }
    #         hb = var_toobj(&vm->hook);
    #         be_assert(hb != NULL);
    #         hb->hook = hook;
    #         hb->data = data;
    #     } else if (!var_istype(&vm->hook, BE_COMPTR)) {
    #         var_setnil(&vm->hook);
    #     }
    #     vm->hookmask = (bbyte)mask;
    # }
    def be_setntvhook(vm, hook, data, mask):
        """Set a native debug hook."""
        if mask:
            if not var_istype(vm.hook, BE_COMPTR):
                hb = bhookblock()
                vm.hook = bvalue()
                var_setobj(vm.hook, BE_COMPTR, hb)
            hb = var_toobj(vm.hook)
            hb.hook = hook
            hb.data = data
        elif not var_istype(vm.hook, BE_COMPTR):
            vm.hook = bvalue()
            var_setnil(vm.hook)
        vm.hookmask = mask & 0xFF

else:
    # Stubs when debug hooks are disabled
    def be_callhook(vm, mask):
        pass

    def be_sethook(vm, mask):
        pass

    def be_setntvhook(vm, hook, data, mask):
        pass


# ============================================================================
# Variable name lookup (BE_DEBUG_VAR_INFO)
# ============================================================================

if BE_DEBUG_VAR_INFO:

    # static binstruction* callstack_fixip(bvm *vm, int level)
    # {
    #     bcallframe *top = (bcallframe*)be_stack_top(&vm->callstack);
    #     bcallframe *cf = top - level + 2;
    #     for (; cf <= top && cf->status & PRIM_FUNC; ++cf);
    #     return cf <= top ? cf->ip : vm->ip;
    # }
    def _callstack_fixip(vm, level):
        """Find the correct instruction pointer for a given call stack level.

        Native function frames don't have their own ip, so we walk up
        from the expected frame until we find a Berry closure frame.
        """
        from berry_port.be_vector import (
            be_stack_top as _be_stack_top,
            be_stack_count as _be_stack_count,
        )
        depth = _be_stack_count(vm.callstack)
        top_idx = depth - 1
        # cf = top - level + 2
        cf_idx = top_idx - level + 2
        # Walk forward past native function frames
        while cf_idx <= top_idx and (vm.callstack.data[cf_idx].status & PRIM_FUNC):
            cf_idx += 1
        if cf_idx <= top_idx:
            return vm.callstack.data[cf_idx].ip
        return vm.ip_idx

    # bbool be_debug_varname(bvm *vm, int level, int index)
    # {
    #     int depth = be_stack_count(&vm->callstack);
    #     if (level > 0 && level <= depth) {
    #         bcallframe *cf = be_vector_at(&vm->callstack, depth - level);
    #         if ((cf->status & PRIM_FUNC) == 0) {
    #             bproto *proto = cast(bclosure*, var_toobj(cf->func))->proto;
    #             int pc = (int)(callstack_fixip(vm, level) - proto->code);
    #             bstring *name = be_func_varname(proto, index, pc);
    #             if (name) {
    #                 bvalue *reg = be_incrtop(vm);
    #                 var_setstr(reg, name);
    #                 return btrue;
    #             }
    #         }
    #     }
    #     return bfalse;
    # }
    def be_debug_varname(vm, level, index):
        """Look up a local variable name at a given call stack level.

        If found, pushes the name string onto the VM stack and returns True.
        Returns False if the level is invalid, the frame is native, or
        no variable name is found.
        """
        from berry_port.be_vector import be_stack_count as _be_stack_count
        from berry_port.be_func import be_func_varname
        from berry_port.be_exec import be_incrtop

        depth = _be_stack_count(vm.callstack)
        if level > 0 and level <= depth:
            cf = vm.callstack.data[depth - level]
            if (cf.status & PRIM_FUNC) == 0:
                func_val = vm.stack[cf.func]
                cl = var_toobj(func_val)
                proto = cl.proto
                ip = _callstack_fixip(vm, level)
                # In C: pc = ip - proto->code
                # ip is an index into proto.code (or vm.ip_idx)
                pc = ip
                name = be_func_varname(proto, index, pc)
                if name is not None:
                    reg_idx = be_incrtop(vm)
                    reg = vm.stack[reg_idx]
                    var_setstr(reg, name)
                    return True
        return False

    # bbool be_debug_upvname(bvm *vm, int level, int index)
    # {
    #     int depth = be_stack_count(&vm->callstack);
    #     if (level > 0 && level <= depth) {
    #         bcallframe *cf = be_vector_at(&vm->callstack, depth - level);
    #         if ((cf->status & PRIM_FUNC) == 0) {
    #             bproto *proto = cast(bclosure*, var_toobj(cf->func))->proto;
    #             if (index >= 0 && index < proto->nupvals) {
    #                 bvalue *reg = be_incrtop(vm);
    #                 var_setstr(reg, proto->upvals[index].name);
    #                 return btrue;
    #             }
    #         }
    #     }
    #     return bfalse;
    # }
    def be_debug_upvname(vm, level, index):
        """Look up an upvalue name at a given call stack level.

        If found, pushes the name string onto the VM stack and returns True.
        Returns False if the level is invalid, the frame is native, or
        the index is out of range.
        """
        from berry_port.be_vector import be_stack_count as _be_stack_count
        from berry_port.be_exec import be_incrtop

        depth = _be_stack_count(vm.callstack)
        if level > 0 and level <= depth:
            cf = vm.callstack.data[depth - level]
            if (cf.status & PRIM_FUNC) == 0:
                func_val = vm.stack[cf.func]
                cl = var_toobj(func_val)
                proto = cl.proto
                if index >= 0 and index < proto.nupvals:
                    reg_idx = be_incrtop(vm)
                    reg = vm.stack[reg_idx]
                    var_setstr(reg, proto.upvals[index].name)
                    return True
        return False

else:
    # Stubs when variable debug info is disabled
    def be_debug_varname(vm, level, index):
        return False

    def be_debug_upvname(vm, level, index):
        return False
