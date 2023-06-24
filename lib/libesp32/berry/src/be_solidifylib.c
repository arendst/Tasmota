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
#include "be_list.h"
#include "be_debug.h"
#include "be_map.h"
#include "be_vm.h"
#include "be_decoder.h"
#include "be_sys.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>

extern const bclass be_class_list;
extern const bclass be_class_map;

#if BE_USE_SOLIDIFY_MODULE
#include <inttypes.h>

#define be_builtin_count(vm) \
    be_vector_count(&(vm)->gbldesc.builtin.vlist)

#ifndef INST_BUF_SIZE
#define INST_BUF_SIZE   768
#endif

#define logfmt(...)                                     \
    do {                                                \
        char __lbuf[INST_BUF_SIZE];                     \
        snprintf(__lbuf, sizeof(__lbuf), __VA_ARGS__);  \
        if (fout) {                                     \
            be_fwrite(fout, __lbuf, strlen(__lbuf));    \
        } else {                                        \
            be_writestring(__lbuf);                     \
        }                                               \
    } while (0)

/********************************************************************\
 * Encode string to identifiers
 * 
 * `_X` is used as an escape marker
\********************************************************************/
static unsigned toidentifier_length(const char *s)
{
    unsigned len = 1;
    const char * p = s;
    while (*p) {
        if (p[0] == '_' && p[1] == 'X') {
            len += 3;
            p += 2;
        } else if (isalnum(p[0]) || p[0] == '_') {
            p++;
            len++;
        } else {        // escape
            p++;
            len += 4;
        }
    }
    return len;
}

inline static char hexdigit(int v)
{
    v = v & 0xF;
    if (v >= 10)    return v - 10 + 'A';
    return v + '0';
}

static void toidentifier(char *to, const char *p)
{
    while (*p) {
        if (p[0] == '_' && p[1] == 'X') {
            to[0] = '_';
            to[1] = 'X';
            to[2] = '_';
            p += 2;
            to += 3;
        } else if (isalnum(p[0]) || p[0] == '_') {
            *to = *p;
            to++;
            p++;
        } else {        // escape
            to[0] = '_';
            to[1] = 'X';
            to[2] = hexdigit((*p & 0xF0) >> 4);
            to[3] = hexdigit(*p & 0x0F);
            p++;
            to += 4;
        }
    }
    *to = 0;      // final NULL
}

static void m_solidify_bvalue(bvm *vm, bbool str_literal, bvalue * value, const char *classname, const char *key, void* fout);

static void m_solidify_map(bvm *vm, bbool str_literal, bmap * map, const char *class_name, void* fout)
{
    // compact first
    be_map_compact(vm, map);
    
    logfmt("    be_nested_map(%i,\n", map->count);

    logfmt("    ( (struct bmapnode*) &(const bmapnode[]) {\n");
    for (int i = 0; i < map->size; i++) {
        bmapnode * node = &map->slots[i];
        if (node->key.type == BE_NIL) {
            continue;   /* key not used */
        }
        int key_next = node->key.next;
        if (0xFFFFFF == key_next) {
            key_next = -1;      /* more readable */
        }
        if (node->key.type == BE_STRING) {
            /* convert the string literal to identifier */
            const char * key = str(node->key.v.s);
            size_t id_len = toidentifier_length(key);
            char id_buf[id_len];
            toidentifier(id_buf, key);
            if (!str_literal) {
                logfmt("        { be_const_key(%s, %i), ", id_buf, key_next);
            } else {
                logfmt("        { be_const_key_weak(%s, %i), ", id_buf, key_next);
            }
            m_solidify_bvalue(vm, str_literal, &node->value, class_name, str(node->key.v.s), fout);
        } else if (node->key.type == BE_INT) {
#if BE_INTGER_TYPE == 2
            logfmt("        { be_const_key_int(%lli, %i), ", node->key.v.i, key_next);
#else
            logfmt("        { be_const_key_int(%i, %i), ", node->key.v.i, key_next);
#endif
            m_solidify_bvalue(vm, str_literal, &node->value, class_name, NULL, fout);
        } else {
            char error[64];
            snprintf(error, sizeof(error), "Unsupported type in key: %i", node->key.type);
            be_raise(vm, "internal_error", error);
        }

        logfmt(" },\n");
    }
    logfmt("    }))");        // TODO need terminal comma?

}

static void m_solidify_list(bvm *vm, bbool str_literal, blist * list, const char *class_name, void* fout)
{
    logfmt("    be_nested_list(%i,\n", list->count);

    logfmt("    ( (struct bvalue*) &(const bvalue[]) {\n");
    for (int i = 0; i < list->count; i++) {
        logfmt("        ");
        m_solidify_bvalue(vm, str_literal, &list->data[i], class_name, "", fout);
        logfmt(",\n");
    }
    logfmt("    }))");        // TODO need terminal comma?
}

// pass key name in case of class, or NULL if none
static void m_solidify_bvalue(bvm *vm, bbool str_literal, bvalue * value, const char *classname, const char *key, void* fout)
{
    int type = var_primetype(value);
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
        logfmt("be_const_int(%i)", var_toint(value));
#endif
        break;
    case BE_INDEX:
#if BE_INTGER_TYPE == 2
        logfmt("be_const_var(%lli)", var_toint(value));
#else
        logfmt("be_const_var(%i)", var_toint(value));
#endif
        break;
    case BE_REAL:
#if BE_USE_SINGLE_FLOAT
        logfmt("be_const_real_hex(0x%08" PRIX32 ")", (uint32_t)(uintptr_t)var_toobj(value));
#else
        logfmt("be_const_real_hex(0x%016" PRIx64 ")", (uint64_t)var_toobj(value));
#endif
        break;
    case BE_STRING:
        {
            const char * str = str(var_tostr(value));
            size_t len = strlen(str);
            if (len >= 255) {
                be_raise(vm, "internal_error", "Strings greater than 255 chars not supported yet");
            }
            size_t id_len = toidentifier_length(str);
            char id_buf[id_len];
            toidentifier(id_buf, str);
            if (!str_literal) {
                logfmt("be_nested_str(%s)", id_buf);
            } else {
                logfmt("be_nested_str_weak(%s)", id_buf);
            }
        }
        break;
    case BE_CLOSURE:
        {
            const char * func_name = str(((bclosure*) var_toobj(value))->proto->name);
            size_t id_len = toidentifier_length(func_name);
            char func_name_id[id_len];
            toidentifier(func_name_id, func_name);
            logfmt("be_const_%sclosure(%s%s%s_closure)",
                var_isstatic(value) ? "static_" : "",
                classname ? classname : "", classname ? "_" : "",
                func_name_id);
        }
        break;
    case BE_CLASS:
        logfmt("be_const_class(be_class_%s)", str(((bclass*) var_toobj(value))->name));
        break;
    case BE_COMPTR:
        logfmt("be_const_comptr(&be_ntv_%s_%s)", classname ? classname : "unknown", key ? key : "unknown");
        break;
    case BE_NTVFUNC:
        logfmt("be_const_%sfunc(be_ntv_%s_%s)",
            var_isstatic(value) ? "static_" : "",
            classname ? classname : "unknown", key ? key : "unknown");
        break;
    case BE_INSTANCE:
    {
        binstance * ins = (binstance *) var_toobj(value);
        bclass * cl = ins->_class;
        if (ins->super || ins->sub) {
            be_raise(vm, "internal_error", "instance must not have a super/sub class");
        } else if (cl->nvar != 1) {
            be_raise(vm, "internal_error", "instance must have only one instance variable");
        } else if ((cl != &be_class_map && cl != &be_class_list) || 1) {   // TODO
            const char * cl_ptr = "";
            if (cl == &be_class_map) { cl_ptr = "map"; }
            if (cl == &be_class_list) { cl_ptr = "list"; }
            logfmt("be_const_simple_instance(be_nested_simple_instance(&be_class_%s, {\n", cl_ptr);
            if (cl == &be_class_map) {
                logfmt("        be_const_map( * ");
            } else {
                logfmt("        be_const_list( * ");
            }
            m_solidify_bvalue(vm, str_literal, &ins->members[0], classname, key, fout);
            logfmt("    ) } ))");
        }
    }
        break;
    case BE_MAP:
        m_solidify_map(vm, str_literal, (bmap *) var_toobj(value), classname, fout);
        break;
    case BE_LIST:
        m_solidify_list(vm, str_literal, (blist *) var_toobj(value), classname, fout);
        break;
    default:
        {
            char error[64];
            snprintf(error, sizeof(error), "Unsupported type in function constants: %i", type);
            be_raise(vm, "internal_error", error);
        }
    }
}

static void m_solidify_subclass(bvm *vm, bbool str_literal, bclass *cl, void* fout);

/* solidify any inner class */
static void m_solidify_proto_inner_class(bvm *vm, bbool str_literal, bproto *pr, void* fout)
{
    // parse any class in constants to output it first
    if (pr->nconst > 0) {
        for (int k = 0; k < pr->nconst; k++) {
            if (var_type(&pr->ktab[k]) == BE_CLASS) {
                if ((k == 0) && (pr->varg & BE_VA_STATICMETHOD)) {
                    // it is the implicit '_class' variable from a static method, don't dump the class
                } else {
                    // output the class
                    m_solidify_subclass(vm, str_literal, (bclass*) var_toobj(&pr->ktab[k]), fout);
                }
            }
        }
    }
}


static void m_solidify_proto(bvm *vm, bbool str_literal, bproto *pr, const char * func_name, int indent, void* fout)
{
    // const char * func_name = str(pr->name);
    // const char * func_source = str(pr->source);

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
            snprintf(sub_name, sizeof(sub_name), "%s_%"PRId32, func_name, i);
            m_solidify_proto(vm, str_literal, pr->ptab[i], sub_name, indent+2, fout);
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
            m_solidify_bvalue(vm, str_literal, &pr->ktab[k], NULL, NULL, fout);
            logfmt(",\n");
        }
        logfmt("%*s}),\n", indent, "");
    } else {
        logfmt("%*sNULL,                       /* no const */\n", indent, "");
    }

    /* convert the string literal to identifier */
    const char * key = str(pr->name);
    size_t id_len = toidentifier_length(key);
    char id_buf[id_len];
    toidentifier(id_buf, key);
    if (!str_literal) {
        logfmt("%*s&be_const_str_%s,\n", indent, "", id_buf);
    } else {
        logfmt("%*sbe_str_weak(%s),\n", indent, "", id_buf);
    }
    // hard-code source as "solidified" for solidified
    logfmt("%*s&be_const_str_solidified,\n", indent, "");

    logfmt("%*s( &(const binstruction[%2d]) {  /* code */\n", indent, "", pr->codesize);
    for (int pc = 0; pc < pr->codesize; pc++) {
        uint32_t ins = pr->code[pc];
        logfmt("%*s  0x%08"PRIX32",  //", indent, "", ins);
        be_print_inst(ins, pc, fout);
        bopcode op = IGET_OP(ins);
        if (op == OP_GETGBL || op == OP_SETGBL) {
            // check if the global is in built-ins
            int glb = IGET_Bx(ins);
            if (glb > be_builtin_count(vm)) {
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

static void m_solidify_closure(bvm *vm, bbool str_literal, bclosure *cl, const char * classname, void* fout)
{   
    bproto *pr = cl->proto;
    const char * func_name = str(pr->name);

    if (cl->nupvals > 0) {
        logfmt("--> Unsupported upvals in closure <---");
        // be_raise(vm, "internal_error", "Unsupported upvals in closure");
    }

    int indent = 2;

    m_solidify_proto_inner_class(vm, str_literal, pr, fout);

    logfmt("\n");
    logfmt("/********************************************************************\n");
    logfmt("** Solidified function: %s\n", func_name);
    logfmt("********************************************************************/\n");

    {
        size_t id_len = toidentifier_length(func_name);
        char func_name_id[id_len];
        toidentifier(func_name_id, func_name);
        logfmt("be_local_closure(%s%s%s,   /* name */\n",
            classname ? classname : "", classname ? "_" : "",
            func_name_id);
    }

    m_solidify_proto(vm, str_literal, pr, func_name, indent, fout);
    logfmt("\n");

    // closure
    logfmt(");\n");
    logfmt("/*******************************************************************/\n\n");
}

static void m_solidify_subclass(bvm *vm, bbool str_literal, bclass *cl, void* fout)
{
    const char * class_name = str(cl->name);

    /* pre-declare class to support '_class' implicit variable */
    logfmt("\nextern const bclass be_class_%s;\n", class_name);

    /* iterate on members to dump closures */
    if (cl->members) {
        bmapnode *node;
        bmapiter iter = be_map_iter();
        while ((node = be_map_next(cl->members, &iter)) != NULL) {
            if (var_isstr(&node->key) && var_isclosure(&node->value)) {
                bclosure *f = var_toobj(&node->value);
                m_solidify_closure(vm, str_literal, f, class_name, fout);
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
        m_solidify_map(vm, str_literal, cl->members, class_name, fout);
        logfmt(",\n");
    } else {
        logfmt("    NULL,\n");
    }

    size_t id_len = toidentifier_length(class_name);
    char id_buf[id_len];
    toidentifier(id_buf, class_name);
    if (!str_literal) {
        logfmt("    (bstring*) &be_const_str_%s\n", id_buf);
    } else {
        logfmt("    be_str_weak(%s)\n", id_buf);
    }
    logfmt(");\n");

}

static void m_solidify_class(bvm *vm, bbool str_literal, bclass *cl, void* fout)
{
    const char * class_name = str(cl->name);
    m_solidify_subclass(vm, str_literal, cl, fout);
    logfmt("/*******************************************************************/\n\n");

    logfmt("void be_load_%s_class(bvm *vm) {\n", class_name);
    logfmt("    be_pushntvclass(vm, &be_class_%s);\n", class_name);
    logfmt("    be_setglobal(vm, \"%s\");\n", class_name);
    logfmt("    be_pop(vm, 1);\n");
    logfmt("}\n");
}

static void m_solidify_module(bvm *vm, bbool str_literal, bmodule *ml, void* fout)
{
    const char * module_name = be_module_name(ml);
    if (!module_name) { module_name = ""; }

    /* iterate on members to dump closures and classes */
    if (ml->table) {
        bmapnode *node;
        bmapiter iter = be_map_iter();
        while ((node = be_map_next(ml->table, &iter)) != NULL) {
            if (var_isstr(&node->key) && var_isclosure(&node->value)) {
                bclosure *f = var_toobj(&node->value);
                m_solidify_closure(vm, str_literal, f, module_name, fout);
            }
            if (var_isstr(&node->key) && var_isclass(&node->value)) {
                bclass *cl = var_toobj(&node->value);
                m_solidify_subclass(vm, str_literal, cl, fout);
            }
        }
    }


    logfmt("\n");
    logfmt("/********************************************************************\n");
    logfmt("** Solidified module: %s\n", module_name);
    logfmt("********************************************************************/\n");

    logfmt("be_local_module(%s,\n", module_name);
    logfmt("    \"%s\",\n", module_name);

    if (ml->table) {
        m_solidify_map(vm, str_literal, ml->table, module_name, fout);
        logfmt("\n");
    } else {
        logfmt("    NULL,\n");
    }
    logfmt(");\n");
    logfmt("BE_EXPORT_VARIABLE be_define_const_native_module(%s);\n", module_name);
    logfmt("/********************************************************************/\n");

}

static int m_dump(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        bbool str_literal = bfalse;
        if (top >= 2) {
            str_literal = be_tobool(vm, 2);
        }
        void* fout = NULL;      /* output file */
        if (top >= 3 && be_isinstance(vm, 3)) {
            be_getmember(vm, 3, ".p");
            if (be_iscomptr(vm, -1)) {
                fout = be_tocomptr(vm, -1);
            }
            be_pop(vm, 1);
        }
        const char *classname = NULL;  /* allow to specify an explicit prefix */
        if (top >= 4 && be_isstring(vm, 4)) {
            classname = be_tostring(vm, 4);
        }
        if (var_isclosure(v)) {
            m_solidify_closure(vm, str_literal, var_toobj(v), classname, fout);
        } else if (var_isclass(v)) {
            m_solidify_class(vm, str_literal, var_toobj(v), fout);
        } else if (var_ismodule(v)) {
            m_solidify_module(vm, str_literal, var_toobj(v), fout);
        } else {
            be_raise(vm, "value_error", "unsupported type");
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
