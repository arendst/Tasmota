/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_mem.h"
#include "be_sys.h"
#include "be_gc.h"
#include "be_bytecode.h"
#include "be_vm.h"
#include <string.h>

#define READLINE_STEP           100

static int i_write(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if(be_iscomptr(vm, -1) && (be_isstring(vm, 2) || be_isbytes(vm, 2))) {
        void *fh = be_tocomptr(vm, -1);
        size_t size = 0;
        const char *data = NULL;
        if (be_isstring(vm, 2)) {
            data = be_tostring(vm, 2);
            size = be_strlen(vm, 2);
        } else {
            data = be_tobytes(vm, 2, &size);
        }
        size_t bw = be_fwrite(fh, data, size);
        if (bw != size) {
            be_raise(vm, "io_error", "write failed");
        }
    }
    be_return_nil(vm);
}

static size_t readsize(bvm *vm, int argc, void *fh)
{
    if (argc >=2 && be_isint(vm, 2)) {
        return be_toindex(vm, 2);
    }
    return be_fsize(fh) - be_ftell(fh);
}

static int i_read(bvm *vm)
{
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        size_t size = readsize(vm, argc, fh);
        if (size) {
            char *buffer = be_malloc(vm, size);
            size = be_fread(fh, buffer, size);
            be_pushnstring(vm, buffer, size);
            be_free(vm, buffer, size);
        } else {
            be_pushstring(vm, "");
        }
        be_return(vm);
    }
    be_return_nil(vm);
}

static int i_readbytes(bvm *vm)
{
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        size_t size = readsize(vm, argc, fh);
        if (size) {
            if (size > vm->bytesmaxsize) {
                be_raise(vm, "memory_error", "size exceeds maximum allowed for bytes");
            }
            /* avoid double allocation, using directly the internal buffer of bytes() */
            be_getbuiltin(vm, "bytes");
            be_pushint(vm, size);
            be_call(vm, 1);  /* call bytes() constructor with pre-sized buffer */
            be_pop(vm, 1);  /* bytes() instance is at top */

            /* read back the actual buffer size */
            be_getmember(vm, -1, ".size");
            int32_t bytes_size = be_toint(vm, -1);
            be_pop(vm, 1);
            if (bytes_size < (int32_t)size) {
                be_raise(vm, "memory_error", "could not allocated buffer");
            }

            be_getmember(vm, -1, "resize");
            be_pushvalue(vm, -2);
            be_pushint(vm, size);
            be_call(vm, 2); /* call b.resize(size) */
            be_pop(vm, 3);  /* bytes() instance is at top */

            char *buffer = (char*) be_tobytes(vm, -1, NULL); /* we get the address of the internal buffer of size 'size' */
            size_t read_size = be_fread(fh, buffer, size);

            if (size != read_size) {
                /* resize if something went wrong */
                be_getmember(vm, -1, "resize");
                be_pushvalue(vm, -2);
                be_pushint(vm, read_size);
                be_call(vm, 2); /* call b.resize(size) */
                be_pop(vm, 3);  /* bytes() instance is at top */
            }
        } else {
            be_pushbytes(vm, NULL, 0);
        }
        be_return(vm);
    }
    be_return_nil(vm);
}

static int i_readline(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        size_t pos = 0, size = READLINE_STEP;
        char *buffer = be_malloc(vm, size);
        char *res = be_fgets(fh, buffer, (int)size);
        while (res) {
            pos += strlen(buffer + pos);
            if (!pos || buffer[pos - 1] == '\n') {
                break;
            }
            buffer = be_realloc(vm, buffer, size, size + READLINE_STEP);
            res = be_fgets(fh, buffer + pos, READLINE_STEP);
            size += READLINE_STEP;
        }
        be_pushnstring(vm, buffer, pos);
        be_free(vm, buffer, size);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int i_seek(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1) && be_isint(vm, 2)) {
        void *fh = be_tocomptr(vm, -1);
        be_fseek(fh, be_toindex(vm, 2));
    }
    be_return_nil(vm);
}

static int i_tell(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        size_t pos = be_ftell(fh);
        be_pushint(vm, cast(bint, pos));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int i_size(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        size_t pos = be_fsize(fh);
        be_pushint(vm, cast(bint, pos));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int i_flush(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        be_fflush(fh);
    }
    be_return_nil(vm);
}

static int i_close(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    if (be_iscomptr(vm, -1)) {
        void *fh = be_tocomptr(vm, -1);
        be_fclose(fh);
        be_pushnil(vm);
        be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
}

static int i_savecode(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_isclosure(vm, 2)) {
        be_getmember(vm, 1, ".p");
        if (be_iscomptr(vm, -1)) {
            void *fh = be_tocomptr(vm, -1);
            bvalue *v = be_indexof(vm, 2);
            if (var_isclosure(v)) {
                bclosure *cl = var_toobj(v);
                bproto *pr = cl->proto;
                be_bytecode_save_to_fs(vm, fh, pr);
            }
        }
    } else {
        be_raise(vm, "type_error", "closure expected");
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
static int m_open(bvm *vm)
#else
int be_nfunc_open(bvm *vm)
#endif
{
    int argc = be_top(vm);
    const char *fname, *mode;
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { "write", i_write },
        { "read", i_read },
        { "readbytes", i_readbytes },
        { "readline", i_readline },
        { "seek", i_seek },
        { "tell", i_tell },
        { "size", i_size },
        { "flush", i_flush },
        { "close", i_close },
        { "deinit", i_close },
        { "savecode", i_savecode },
        { NULL, NULL }
    };
    fname = argc >= 1 && be_isstring(vm, 1) ? be_tostring(vm, 1) : NULL;
    mode = argc >= 2 && be_isstring(vm, 2) ? be_tostring(vm, 2) : "r";
    if (fname) {
        void *fh = be_fopen(fname, mode);
        if (fh == NULL) {
            be_raise(vm, "io_error",
                be_pushfstring(vm, "cannot open file '%s'", fname));
        }
        be_pushclass(vm, "file", members);
        be_call(vm, 0);
        be_pushcomptr(vm, fh);
        be_setmember(vm, -2, ".p");
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_filelib(bvm *vm)
{
    be_regfunc(vm, "open", m_open);
}
#endif
