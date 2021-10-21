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
#include <inttypes.h>

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

// pass key name in case of class, or NULL if none
static void m_solidify_bvalue(bvm *vm, bvalue * value, const char *classname, const char *key)
{
    int type = var_type(value);
    switch (type) {
    case BE_NIL:
        logfmt("be_const_nil()");
        break;
    case BE_BOOL:
        logfmt("be_const_bool(%i)", var_tobool(value));
        break;
    case BE_INT:
#if BE_INTGER_TYPE == 2
        logfmt("be_const_int(%lli)", var_toint(value));
#else
        logfmt("be_const_int(%li)", var_toint(value));
#endif
        break;
    case BE_INDEX:
#if BE_INTGER_TYPE == 2
        logfmt("be_const_var(%lli)", var_toint(value));
#else
        logfmt("be_const_var(%li)", var_toint(value));
#endif
        break;
    case BE_REAL:
#if BE_USE_SINGLE_FLOAT
        logfmt("be_const_real_hex(0x%08X)", (uint32_t) var_toobj(value));
#else
        logfmt("be_const_real_hex(0x%016" PRIx64 ")", (uint64_t)var_toobj(value));
#endif
        break;
    case BE_STRING:
        {
            logfmt("be_nested_string(\"");
            be_writestring(str(var_tostr(value)));
            size_t len = strlen(str(var_tostr(value)));
            if (len >= 255) {
                be_raise(vm, "internal_error", "Strings greater than 255 chars not supported yet");
            }
            logfmt("\", %i, %zu)", be_strhash(var_tostr(value)), len >= 255 ? 255 : len);
        }
        break;
    case BE_CLOSURE:
        logfmt("be_const_closure(%s_closure)", str(((bclosure*) var_toobj(value))->proto->name));
        break;
    case BE_CLASS:
        logfmt("be_const_class(be_class_%s)", str(((bclass*) var_toobj(value))->name));
        break;
    case BE_COMPTR:
        logfmt("be_const_comptr(&be_ntv_%s_%s)", classname ? classname : "unknown", key ? key : "unknown");
        break;
    case BE_NTVFUNC:
        logfmt("be_const_func(be_ntv_%s_%s)", classname ? classname : "unknown", key ? key : "unknown");
        break;
    default:
        {
            char error[64];
            snprintf(error, sizeof(error), "Unsupported type in function constants: %i", type);
            be_raise(vm, "internal_error", error);
        }
    }
}

static void m_solidify_subclass(bvm *vm, bclass *cl, int builtins);

/* solidify any inner class */
static void m_solidify_proto_inner_class(bvm *vm, bproto *pr, int builtins)
{
    // parse any class in constants to output it first
    if (pr->nconst > 0) {
        for (int k = 0; k < pr->nconst; k++) {
            if (var_type(&pr->ktab[k]) == BE_CLASS) {
                // output the class
                m_solidify_subclass(vm, (bclass*) var_toobj(&pr->ktab[k]), builtins);
            }
        }
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
    logfmt("%*s%d,                          /* varg */\n", indent, "", pr->varg);
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
        logfmt("%*s( &(const struct bproto*[%2d]) {\n", indent, "", pr->nproto);
        for (int32_t i = 0; i < pr->nproto; i++) {
            size_t sub_len = strlen(func_name) + 10;
            char sub_name[sub_len];
            snprintf(sub_name, sizeof(sub_name), "%s_%d", func_name, i);
            m_solidify_proto(vm, pr->ptab[i], sub_name, builtins, indent+2);
            logfmt(",\n");
        }
        logfmt("%*s}),\n", indent, "");
    } else {
        logfmt("%*sNULL,                       /* no sub protos */\n", indent, "");
    }

    logfmt("%*s%d,                          /* has constants */\n", indent, "", (pr->nconst > 0) ? 1 : 0);
    if (pr->nconst > 0) {
        logfmt("%*s( &(const bvalue[%2d]) {     /* constants */\n", indent, "", pr->nconst);
        for (int k = 0; k < pr->nconst; k++) {
            logfmt("%*s/* K%-3d */  ", indent, "", k);
            m_solidify_bvalue(vm, &pr->ktab[k], NULL, NULL);
            logfmt(",\n");
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
        logfmt("%*s  0x%08X,  //", indent, "", ins);
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

    int indent = 2;

    m_solidify_proto_inner_class(vm, pr, builtins);

    logfmt("\n");
    logfmt("/********************************************************************\n");
    logfmt("** Solidified function: %s\n", func_name);
    logfmt("********************************************************************/\n");

    logfmt("be_local_closure(%s,   /* name */\n", func_name);

    m_solidify_proto(vm, pr, func_name, builtins, indent);
    logfmt("\n");

    // closure
    logfmt(");\n");
    logfmt("/*******************************************************************/\n\n");
}

static void m_solidify_subclass(bvm *vm, bclass *cl, int builtins)
{
    const char * class_name = str(cl->name);

    /* iterate on members to dump closures */
    if (cl->members) {
        bmapnode *node;
        bmapiter iter = be_map_iter();
        while ((node = be_map_next(cl->members, &iter)) != NULL) {
            if (var_isstr(&node->key) && var_isclosure(&node->value)) {
                bclosure *f = var_toobj(&node->value);
                m_solidify_closure(vm, f, builtins);
            }
        }
    }


    logfmt("\n");
    logfmt("/********************************************************************\n");
    logfmt("** Solidified class: %s\n", class_name);
    logfmt("********************************************************************/\n");

    if (cl->super) {
        logfmt("extern const bclass be_class_%s;\n", str(cl->super->name));
    }

    logfmt("be_local_class(%s,\n", class_name);
    logfmt("    %i,\n", cl->nvar);
    if (cl->super) {
        logfmt("    &be_class_%s,\n", str(cl->super->name));
    } else {
        logfmt("    NULL,\n");
    }

    if (cl->members) {
        logfmt("    be_nested_map(%i,\n", cl->members->count);

        logfmt("    ( (struct bmapnode*) &(const bmapnode[]) {\n");
        for (int i = 0; i < cl->members->count; i++) {
            bmapnode * node = &cl->members->slots[i];
            if (node->key.type != BE_STRING) {
                char error[64];
                snprintf(error, sizeof(error), "Unsupported type in key: %i", node->key.type);
                be_raise(vm, "internal_error", error);
            }
            int key_next = node->key.next;
            size_t len = strlen(str(node->key.v.s));
            if (0xFFFFFF == key_next) {
                key_next = -1;      /* more readable */
            }
            logfmt("        { be_nested_key(\"%s\", %i, %zu, %i), ", str(node->key.v.s), be_strhash(node->key.v.s), len >= 255 ? 255 : len, key_next);
            m_solidify_bvalue(vm, &node->value, class_name, str(node->key.v.s));

            logfmt(" },\n");
        }
        logfmt("    })),\n");
    } else {
        logfmt("    NULL,\n");
    }

    logfmt("    (be_nested_const_str(\"%s\", %i, %i))\n", class_name, be_strhash(cl->name), str_len(cl->name));
    logfmt(");\n");

}


static void m_solidify_class(bvm *vm, bclass *cl, int builtins)
{
    const char * class_name = str(cl->name);
    m_solidify_subclass(vm, cl, builtins);
    logfmt("/*******************************************************************/\n\n");

    logfmt("void be_load_%s_class(bvm *vm) {\n", class_name);
    logfmt("    be_pushntvclass(vm, &be_class_%s);\n", class_name);
    logfmt("    be_setglobal(vm, \"%s\");\n", class_name);
    logfmt("    be_pop(vm, 1);\n");
    logfmt("}\n");
}

#define be_builtin_count(vm) \
    be_vector_count(&(vm)->gbldesc.builtin.vlist)

static int m_dump(bvm *vm)
{
    if (be_top(vm) >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isclosure(v)) {
            m_solidify_closure(vm, var_toobj(v), be_builtin_count(vm));
        } else if (var_isclass(v)) {
            m_solidify_class(vm, var_toobj(v), be_builtin_count(vm));
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
