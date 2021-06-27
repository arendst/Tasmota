/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_module.h"
#include "be_string.h"
#include "be_vector.h"
#include "be_class.h"
#include "be_debug.h"
#include "be_map.h"
#include "be_vm.h"
#include "be_decoder.h"
#include <string.h>
#include <stdio.h>

#if BE_USE_SOLIDIFY_MODULE

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

/* output only valid types for ktab, or NULL */
static const char * m_type_ktab(int type)
{
    switch (type){
        case BE_NIL:    return "BE_NIL";
        case BE_INT:    return "BE_INT";
        case BE_REAL:   return "BE_REAL";
        case BE_BOOL:   return "BE_BOOL";
        case BE_STRING: return "BE_STRING";
        default:        return NULL;
    }
}

static void m_solidify_proto(bvm *vm, bproto *pr, const char * func_name, int builtins, int indent)
{
    // const char * func_name = str(pr->name);
    const char * func_source = str(pr->source);

    logfmt("%*sbe_nested_proto(\n", indent, "");
    indent += 2;

    logfmt("%*s%d,                          /* nstack */\n", indent, "", pr->nstack);
    logfmt("%*s%d,                          /* argc */\n", indent, "", pr->argc);
    logfmt("%*s%d,                          /* has upvals */\n", indent, "", (pr->nupvals > 0) ? 1 : 0);

    if (pr->nupvals > 0) {
        logfmt("%*s( &(const bupvaldesc[%2d]) {  /* upvals */\n", indent, "", pr->nupvals);
        for (int32_t i = 0; i < pr->nupvals; i++) {
            logfmt("%*s  be_local_const_upval(%i, %i),\n", indent, "", pr->upvals[i].instack, pr->upvals[i].idx);
        }
        logfmt("%*s}),\n", indent, "");
    } else {
        logfmt("%*sNULL,                       /* no upvals */\n", indent, "");
    }

    logfmt("%*s%d,                          /* has sup protos */\n", indent, "", (pr->nproto > 0) ? 1 : 0);
    if (pr->nproto > 0) {
        for (int32_t i = 0; i < pr->nproto; i++) {
            size_t sub_len = strlen(func_name) + 10;
            char sub_name[sub_len];
            snprintf(sub_name, sizeof(sub_name), "%s_%d", func_name, i);
            m_solidify_proto(vm, pr->ptab[i], sub_name, builtins, indent);
            logfmt(",\n");
        }
    } else {
        logfmt("%*sNULL,                       /* no sub protos */\n", indent, "");
    }

    logfmt("%*s%d,                          /* has constants */\n", indent, "", (pr->nconst > 0) ? 1 : 0);
    if (pr->nconst > 0) {
        logfmt("%*s( &(const bvalue[%2d]) {     /* upvals */\n", indent, "", pr->nconst);
        for (int k = 0; k < pr->nconst; k++) {
            int type = pr->ktab[k].type;
            const char *type_name = m_type_ktab(type);
            if (type_name == NULL) {
                char error[64];
                snprintf(error, sizeof(error), "Unsupported type in function constants: %i", type);
                be_raise(vm, "internal_error", error);
            }
            if (type == BE_STRING) {
                logfmt("%*s  { { .s=be_nested_const_str(\"", indent, "");
                be_writestring(str(pr->ktab[k].v.s));
                size_t len = strlen(str(pr->ktab[k].v.s));
                if (len >= 255) {
                    be_raise(vm, "internal_error", "Strings greater than 255 chars not supported yet");
                }
                logfmt("\", %i, %zu) }, %s},\n", be_strhash(pr->ktab[k].v.s), len >= 255 ? 255 : len, type_name);
            } else if (type == BE_INT) {
                logfmt("%*s  { { .i=%" BE_INT_FMTLEN "i }, %s},\n", indent, "", pr->ktab[k].v.i, type_name);
            } else if (type == BE_REAL) {
    #if BE_USE_SINGLE_FLOAT
                logfmt("%*s  { { .p=(void*)0x%08X }, %s},\n", indent, "", (uint32_t) pr->ktab[k].v.p, type_name);
    #else
                logfmt("%*s  { { .p=(void*)0x%016llX }, %s},\n", indent, "", (uint64_t) pr->ktab[k].v.p, type_name);
    #endif
            } else if (type == BE_BOOL) {
                logfmt("%*s  { { .b=%i }, %s},\n", indent, "", pr->ktab[k].v.b, type_name);
            }
        }
        logfmt("%*s}),\n", indent, "");
    } else {
        logfmt("%*sNULL,                       /* no const */\n", indent, "");
    }

    logfmt("%*s(be_nested_const_str(\"%s\", %i, %i)),\n", indent, "", str(pr->name), be_strhash(pr->name), str_len(pr->name));
    logfmt("%*s(be_nested_const_str(\"%s\", %i, %i)),\n", indent, "", func_source, be_strhash(pr->source), str_len(pr->source));

    logfmt("%*s( &(const binstruction[%2d]) {  /* code */\n", indent, "", pr->codesize);
    for (int pc = 0; pc < pr->codesize; pc++) {
        uint32_t ins = pr->code[pc];
        logfmt("%*s  0x%04X,  //", indent, "", ins);
        be_print_inst(ins, pc);
        bopcode op = IGET_OP(ins);
        if (op == OP_GETGBL || op == OP_SETGBL) {
            // check if the global is in built-ins
            int glb = IGET_Bx(ins);
            if (glb > builtins) {
                // not supported
                logfmt("\n===== unsupported global G%d\n", glb);
                be_raise(vm, "internal_error", "Unsupported access to non-builtin global");
            }
        }
    }
    logfmt("%*s})\n", indent, "");
    indent -= 2;
    logfmt("%*s)", indent, "");

}

static void m_solidify_closure(bvm *vm, bclosure *cl, int builtins)
{   
    bproto *pr = cl->proto;
    const char * func_name = str(pr->name);

    if (cl->nupvals > 0) {
        be_raise(vm, "internal_error", "Unsupported upvals in closure");
    }

    logfmt("\n");
    logfmt("/********************************************************************\n");
    logfmt("** Solidified function: %s\n", func_name);
    logfmt("********************************************************************/\n");

    int indent = 2;
    logfmt("be_local_closure(%s,   /* name */\n", func_name);

    m_solidify_proto(vm, pr, func_name, builtins, indent);
    logfmt("\n");

    // closure
    logfmt(");\n");
    logfmt("/*******************************************************************/\n\n");
}

#define be_builtin_count(vm) \
    be_vector_count(&(vm)->gbldesc.builtin.vlist)

static int m_dump(bvm *vm)
{
    if (be_top(vm) >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isclosure(v)) {
            m_solidify_closure(vm, var_toobj(v), be_builtin_count(vm));
        }
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(solidify) {
    be_native_module_function("dump", m_dump),
};

be_define_native_module(solidify, NULL);
#else
/* @const_object_info_begin
module solidify (scope: global, depend: BE_USE_SOLIDIFY_MODULE) {
    dump, func(m_dump)
}
@const_object_info_end */
#include "../generate/be_fixed_solidify.h"
#endif

#endif /* BE_USE_SOLIFIDY_MODULE */
