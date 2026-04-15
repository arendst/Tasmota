"""
Berry protected execution and stack management.
Mirrors: src/be_exec.c / src/be_exec.h

In the C implementation, error handling uses setjmp/longjmp for
non-local jumps. The Python port replaces this with Python exceptions
(BerryException hierarchy). Protected calls wrap function execution
in try/except blocks, providing equivalent error containment.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import sys

from berry_port.be_object import (
    BE_OK, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
    BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR,
    bvalue, bcallframe, bcallsnapshot, bexecptframe,
    var_setclosure, var_setnil, var_isclosure, var_toobj,
    cast_int,
)
from berry_port.berry_conf import (
    BE_STACK_TOTAL_MAX, BE_STACK_FREE_MIN,
    BE_USE_SCRIPT_COMPILER, BE_USE_BYTECODE_LOADER,
    BE_USE_BYTECODE_SAVER, BE_USE_PERF_COUNTERS,
    BE_USE_DEBUG_STACK,
)
from berry_port.be_vector import (
    be_stack_push, be_stack_pop, be_stack_top, be_stack_base,
    be_stack_count, be_stack_clear,
    be_vector_resize, be_vector_at,
)
from berry_port.be_mem import be_realloc, be_malloc, be_free
from berry_port.be_decoder import IGET_sBx

# ============================================================================
# Observability hook event constants (from berry.h enum beobshookevents)
# ============================================================================
BE_OBS_PCALL_ERROR = 0
BE_OBS_GC_START = 1
BE_OBS_GC_END = 2
BE_OBS_VM_HEARTBEAT = 3
BE_OBS_STACK_RESIZE_START = 4
BE_OBS_MALLOC_FAIL = 5

# ============================================================================
# Constants (from be_exec.c)
# ============================================================================

# #define FILE_BUFFER_SIZE    256
FILE_BUFFER_SIZE = 256

# #define STACK_OVER_MSG(n) \
#     "stack overflow (maximum stack size is " STR(n) ")"
def _STACK_OVER_MSG(n):
    return "stack overflow (maximum stack size is %d)" % n


# ============================================================================
# BerryException class hierarchy (replaces setjmp/longjmp)
# ============================================================================

# struct blongjmp {
#     bjmpbuf b;
#     struct blongjmp *prev;
#     volatile int status; /* error code */
# };
class BerryException(Exception):
    """Replaces setjmp/longjmp for Berry error handling.

    In the C implementation, be_throw calls longjmp to unwind to the
    nearest setjmp point. In Python, we raise this exception and catch
    it in be_execprotected.
    """
    def __init__(self, errorcode):
        super().__init__(errorcode)
        self.errorcode = errorcode


# ============================================================================
# Internal helper structures (from be_exec.c)
# ============================================================================

# struct pparser {
#     const char *fname;
#     breader reader;
#     void *data;
#     bbyte islocal;
# };
class _pparser:
    """Internal: parser parameters for protected parser call."""
    __slots__ = ('fname', 'reader', 'data', 'islocal')
    def __init__(self, fname=None, reader=None, data=None, islocal=0):
        self.fname = fname
        self.reader = reader
        self.data = data
        self.islocal = islocal


# struct pcall {
#     bvalue *v;
#     int argc;
# };
class _pcall:
    """Internal: call parameters for protected call."""
    __slots__ = ('v', 'argc')
    def __init__(self, v=None, argc=0):
        self.v = v
        self.argc = argc


# struct vmstate {
#     int top, reg, depth;
#     int refcount;
# };
class _vmstate:
    """Internal: saved VM state for protected calls."""
    __slots__ = ('top', 'reg', 'depth', 'refcount')
    def __init__(self):
        self.top = 0
        self.reg = 0
        self.depth = 0
        self.refcount = 0


# struct strbuf {
#     size_t len;
#     const char *s;
# };
class _strbuf:
    """Internal: string buffer for be_loadbuffer reader callback."""
    __slots__ = ('len', 's')
    def __init__(self, s="", length=0):
        self.s = s
        self.len = length


# ============================================================================
# be_throw — raise a Berry error (replaces longjmp)
# ============================================================================

# void be_throw(bvm *vm, int errorcode)
# {
# #if BE_USE_PERF_COUNTERS
#     vm->counter_exc++;
# #endif
#     /* if BE_MALLOC_FAIL then call */
#     if (errorcode == BE_MALLOC_FAIL) {
#         if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_MALLOC_FAIL, vm->gc.usage);
#     }
#     if (vm->errjmp) {
#         vm->errjmp->status = errorcode;
#         exec_throw(vm->errjmp);
#     } else {
#         _os_abort();
#     }
# }
def be_throw(vm, errorcode):
    """Raise a Berry error.

    In C this calls longjmp to unwind to the nearest setjmp point.
    In Python we raise a BerryException that is caught by
    be_execprotected.
    """
    if BE_USE_PERF_COUNTERS:
        if hasattr(vm, 'counter_exc'):
            vm.counter_exc += 1

    # Notify observer hook on malloc failure
    if errorcode == BE_MALLOC_FAIL:
        if hasattr(vm, 'obshook') and vm.obshook is not None:
            vm.obshook(vm, BE_OBS_MALLOC_FAIL, vm.gc.usage)

    # In C, if vm->errjmp is set, longjmp to it; otherwise abort.
    # In Python, we always raise — be_execprotected will catch it.
    raise BerryException(errorcode)


# ============================================================================
# be_exit — fatal error exit
# ============================================================================

# /* Fatal error Exit */
# /* Raise a BE_EXIT exception if within a try/catch block, or exit VM */
# BERRY_API void be_exit(bvm *vm, int status)
# {
#     if (vm->errjmp) {
#         be_pushint(vm, status);
#         be_pop(vm, 1);
#         be_throw(vm, BE_EXIT);
#     } else {
#         _os_exit(status);
#     }
# }
def be_exit(vm, status):
    """Exit the VM.

    If inside a protected call, raises BE_EXIT. Otherwise calls
    sys.exit().
    """
    # In the Python port, we always raise — the outermost
    # be_execprotected will catch it. If there's no protection,
    # the exception propagates to the top level.
    # We push the status int onto the stack before throwing,
    # mirroring the C behavior.
    # Note: be_pushint/be_pop are in be_api.py which may not be
    # available yet. We do the minimal equivalent inline.
    v = bvalue()
    v.type = 1  # BE_INT
    v.v = status
    vm.stack[vm.top] = v
    # be_pop equivalent: vm.top stays (C pops then throws)
    be_throw(vm, BE_EXIT)


# ============================================================================
# be_throw_message — throw with an error message string
# ============================================================================

# void be_throw_message(bvm *vm, int errorcode, const char *msg)
# {
#     be_pushstring(vm, msg);
#     be_throw(vm, errorcode);
# }
def be_throw_message(vm, errorcode, msg):
    """Push an error message string onto the stack and throw.

    Mirrors C be_throw_message which calls be_pushstring then be_throw.
    We do the minimal stack push inline to avoid circular imports with
    be_api.py.
    """
    from berry_port.be_string import be_newstr
    v = bvalue()
    s = be_newstr(vm, msg)
    v.type = 16  # BE_STRING
    v.v = s
    # Push onto stack at vm.top, then increment top
    _ensure_stack_space(vm, 1)
    vm.stack[vm.top] = v
    vm.top += 1
    be_throw(vm, errorcode)


# ============================================================================
# be_execprotected — execute a function with error protection
# ============================================================================

# /* Exec protected: exec function and capture any exception and contain it within call */
# /* Exceptions or fatal errors are not propagated */
# int be_execprotected(bvm *vm, bpfunc f, void *data)
# {
#     struct blongjmp jmp;
#     jmp.status = 0;
#     jmp.prev = vm->errjmp; /* save long jump position */
#     vm->errjmp = &jmp;
#     exec_try(vm->errjmp) {
#         f(vm, data);
#     }
#     vm->errjmp = jmp.prev; /* restore long jump position */
#     return jmp.status;
# }
def be_execprotected(vm, f, data):
    """Execute f(vm, data) with error protection.

    In C this uses setjmp/longjmp. In Python we use try/except to
    catch BerryException. Returns 0 on success, or the error code.
    """
    prev_errjmp = getattr(vm, 'errjmp', None)
    # We use a simple sentinel object to track the "current" errjmp level.
    # In C, errjmp is a pointer to a blongjmp struct on the C stack.
    # In Python, we just need to save/restore the previous value.
    sentinel = object()  # unique marker for this protection level
    vm.errjmp = sentinel
    status = 0
    try:
        f(vm, data)
    except BerryException as e:
        status = e.errorcode
    vm.errjmp = prev_errjmp  # restore long jump position
    return status


# ============================================================================
# VM state save/restore (from be_exec.c)
# ============================================================================

# static void vm_state_save(bvm *vm, struct vmstate *state)
# {
#     state->depth = be_stack_count(&vm->callstack);
#     state->top = cast_int(vm->top - vm->stack);
#     state->reg = cast_int(vm->reg - vm->stack);
#     state->refcount = vm->refstack.count;
# }
def _vm_state_save(vm, state):
    """Save VM state for later restoration on error."""
    state.depth = be_stack_count(vm.callstack)
    state.top = vm.top       # int index in Python port
    state.reg = vm.reg       # int index in Python port
    state.refcount = vm.refstack.count


# static void copy_exception(bvm *vm, int res, int dstindex)
# {
#     bvalue *dst = vm->stack + dstindex;
#     if (res == BE_EXCEPTION || res == BE_EXIT) {
#         bvalue *src = vm->top;
#         *dst++ = *src++;
#         if (res == BE_EXCEPTION) {
#             *dst++ = *src++;
#         }
#     }
#     vm->top = dst;
# }
def _copy_exception(vm, res, dstindex):
    """Copy exception info from top of stack to dstindex.

    For BE_EXCEPTION: copies two values (exception value + argument).
    For BE_EXIT: copies one value (exit status).
    """
    dst = dstindex
    if res == BE_EXCEPTION or res == BE_EXIT:
        src = vm.top
        # Copy exception value
        if src < len(vm.stack):
            vm.stack[dst] = bvalue()
            vm.stack[dst].type = vm.stack[src].type
            vm.stack[dst].v = vm.stack[src].v
        dst += 1
        src += 1
        if res == BE_EXCEPTION:
            # Copy exception argument
            if src < len(vm.stack):
                if dst >= len(vm.stack):
                    vm.stack.append(bvalue())
                vm.stack[dst] = bvalue()
                vm.stack[dst].type = vm.stack[src].type
                vm.stack[dst].v = vm.stack[src].v
            dst += 1
    vm.top = dst


# static void vm_state_restore(bvm *vm, const struct vmstate *state, int res)
# {
#     vm->reg = vm->stack + state->reg;
#     be_vector_resize(vm, &vm->refstack, state->refcount);
#     /* copy exception information to top */
#     copy_exception(vm, res, state->top);
#     be_assert(be_stack_count(&vm->callstack) >= state->depth);
#     if (be_stack_count(&vm->callstack) > state->depth) {
#         be_vector_resize(vm, &vm->callstack, state->depth);
#         vm->cf = be_stack_top(&vm->callstack);
#     }
# }
def _vm_state_restore(vm, state, res):
    """Restore VM state after a protected call error."""
    vm.reg = state.reg
    be_vector_resize(vm, vm.refstack, state.refcount)
    # Copy exception information to the saved top position
    _copy_exception(vm, res, state.top)
    assert be_stack_count(vm.callstack) >= state.depth
    if be_stack_count(vm.callstack) > state.depth:
        be_vector_resize(vm, vm.callstack, state.depth)
        vm.cf = be_stack_top(vm.callstack)


# ============================================================================
# Protected parser (from be_exec.c)
# ============================================================================

# static void m_parser(bvm *vm, void *data)
# {
#     struct pparser *p = cast(struct pparser*, data);
#     bclosure *cl = be_parser_source(vm,
#         p->fname, p->reader, p->data, p->islocal);
#     var_setclosure(vm->top, cl);
#     be_incrtop(vm);
# }
def _m_parser(vm, data):
    """Internal: parse source code and push resulting closure."""
    from berry_port.be_parser import be_parser_source
    p = data  # _pparser instance
    cl = be_parser_source(vm, p.fname, p.reader, p.data, p.islocal)
    var_setclosure(vm.stack[vm.top], cl)
    be_incrtop(vm)


# int be_protectedparser(bvm *vm,
#     const char *fname, breader reader, void *data, bbool islocal)
# {
#     int res;
#     struct pparser s;
#     struct vmstate state;
#     s.fname = fname;
#     s.reader = reader;
#     s.data = data;
#     s.islocal = (bbyte)(islocal != 0);
#     vm_state_save(vm, &state);
#     res = be_execprotected(vm, m_parser, &s);
#     if (res) { /* restore call stack */
#         vm_state_restore(vm, &state, res);
#     }
#     return res;
# }
def be_protectedparser(vm, fname, reader, data, islocal):
    """Parse source code with error protection.

    Returns 0 on success, or an error code. On error, the VM state
    is restored and exception info is on the stack.
    """
    s = _pparser(fname=fname, reader=reader, data=data,
                 islocal=1 if islocal else 0)
    state = _vmstate()
    _vm_state_save(vm, state)
    res = be_execprotected(vm, _m_parser, s)
    if res:  # restore call stack
        _vm_state_restore(vm, state, res)
    return res


# ============================================================================
# String reader callback for be_loadbuffer (from be_exec.c)
# ============================================================================

# static const char* _sgets(struct blexer* lexer, void *data, size_t *size)
# {
#     (void)lexer;
#     struct strbuf *sb = data;
#     *size = sb->len;
#     if (sb->len) {
#         sb->len = 0;
#         return sb->s;
#     }
#     return NULL;
# }
def _sgets(lexer, data, size_ref):
    """Reader callback for string buffers.

    Returns (string_data, bytes_read) on first call, then (None, 0)
    on subsequent calls. The data parameter is a _strbuf instance.
    """
    sb = data
    length = sb.len
    size_ref[0] = length  # C: *size = sb->len
    if length:
        sb.len = 0
        return sb.s
    return None


# ============================================================================
# be_loadbuffer — load Berry source from a string buffer
# ============================================================================

# BERRY_API int be_loadbuffer(bvm *vm,
#     const char *name, const char *buffer, size_t length)
# {
#     struct strbuf sbuf;
#     sbuf.s = buffer;
#     sbuf.len = length;
#     return be_protectedparser(vm, name, _sgets, &sbuf, bfalse);
# }
def be_loadbuffer(vm, name, buffer, length):
    """Load Berry source code from a string buffer.

    Compiles the source and pushes the resulting closure onto the stack.
    Returns 0 on success, or an error code.
    """
    sbuf = _strbuf(s=buffer, length=length)
    return be_protectedparser(vm, name, _sgets, sbuf, False)


# ============================================================================
# be_loadmode — load from file (bytecode or source)
# ============================================================================

# BERRY_API int be_loadmode(bvm *vm, const char *name, bbool islocal)
# {
#     int res = load_bytecode(vm, name);
# #if BE_USE_SCRIPT_COMPILER
#     if (res && res != BE_IO_ERROR && res != BE_EXCEPTION) {
#         res = fileparser(vm, name, islocal);
#     }
# #else
#     (void)islocal;
# #endif
#     if (res == BE_IO_ERROR) {
#         be_pushfstring(vm, "cannot open file '%s'.", name);
#     }
#     return res;
# }
def be_loadmode(vm, name, islocal):
    """Load a Berry module from file (tries bytecode first, then source).

    Returns 0 on success, or an error code.
    """
    res = _load_bytecode(vm, name)
    if BE_USE_SCRIPT_COMPILER:
        if res and res != BE_IO_ERROR and res != BE_EXCEPTION:
            res = _fileparser(vm, name, islocal)
    if res == BE_IO_ERROR:
        # Push error message onto stack (do NOT throw — just push and return)
        # C code: be_pushfstring(vm, "cannot open file '%s'.", name);
        from berry_port.be_api import be_pushfstring
        be_pushfstring(vm, "cannot open file '%s'.", name)
    return res


# ============================================================================
# File reader callback and file parser (from be_exec.c)
# ============================================================================

# struct filebuf {
#     void *fp;
#     char buf[FILE_BUFFER_SIZE];
# };
class _filebuf:
    """Internal: file buffer for file reader callback."""
    __slots__ = ('fp', 'buf')
    def __init__(self, fp=None):
        self.fp = fp
        self.buf = ""


# static const char* _fgets(struct blexer* lexer, void *data, size_t *size)
# {
#     (void)lexer;
#     struct filebuf *fb = data;
#     *size = be_fread(fb->fp, fb->buf, sizeof(fb->buf));
#     if (*size) {
#         return fb->buf;
#     }
#     return NULL;
# }
def _fgets(lexer, data, size_ref):
    """Reader callback for file buffers.

    Reads up to FILE_BUFFER_SIZE bytes from the file.
    Sets size_ref[0] to the number of bytes read.
    Normalizes Python Unicode to UTF-8 byte representation (as Latin-1)
    to match C Berry's raw byte handling.
    """
    fb = data
    chunk = fb.fp.read(FILE_BUFFER_SIZE)
    if chunk:
        if isinstance(chunk, str):
            chunk = chunk.encode('utf-8').decode('latin-1')
        fb.buf = chunk
        size_ref[0] = len(chunk)
        return chunk
    return None


# static int fileparser(bvm *vm, const char *name, bbool islocal)
# {
#     int res = BE_IO_ERROR;
#     struct filebuf *fbuf = be_malloc(vm, sizeof(struct filebuf));
#     fbuf->fp = be_fopen(name, "r");
#     if (fbuf->fp) {
#         res = be_protectedparser(vm, name, _fgets, fbuf, islocal);
#         be_fclose(fbuf->fp);
#     }
#     be_free(vm, fbuf, sizeof(struct filebuf));
#     return res;
# }
def _fileparser(vm, name, islocal):
    """Parse a Berry source file with error protection."""
    res = BE_IO_ERROR
    try:
        fp = open(name, "r")
    except (IOError, OSError):
        return res
    fbuf = _filebuf(fp=fp)
    res = be_protectedparser(vm, name, _fgets, fbuf, islocal)
    fp.close()
    return res


# ============================================================================
# Bytecode loader (from be_exec.c)
# ============================================================================

# static void bytecode_loader(bvm *vm, void *data)
# {
#     bclosure *cl = be_bytecode_load(vm, (const char *)data);
#     if (cl != NULL) {
#         var_setclosure(vm->top, cl);
#     } else {
#         var_setnil(vm->top);
#     }
#     be_incrtop(vm);
# }
def _bytecode_loader(vm, data):
    """Internal: load bytecode file and push closure."""
    from berry_port.be_bytecode import be_bytecode_load
    name = data
    cl = be_bytecode_load(vm, name)
    if cl is not None:
        var_setclosure(vm.stack[vm.top], cl)
    else:
        var_setnil(vm.stack[vm.top])
    be_incrtop(vm)


# /* load bytecode file */
# static int load_bytecode(bvm *vm, const char *name)
# {
#     int res = BE_SYNTAX_ERROR;
#     if (be_bytecode_check(name)) {
#         struct vmstate state;
#         vm_state_save(vm, &state);
#         res = be_execprotected(vm, bytecode_loader, (void*)name);
#         if (res) { /* restore call stack */
#             vm_state_restore(vm, &state, res);
#         }
#     }
#     return res;
# }
def _load_bytecode(vm, name):
    """Load a bytecode file with error protection.

    Returns BE_SYNTAX_ERROR if the file is not a valid bytecode file.
    """
    if not BE_USE_BYTECODE_LOADER:
        return BE_SYNTAX_ERROR
    from berry_port.be_bytecode import be_bytecode_check
    res = BE_SYNTAX_ERROR
    if be_bytecode_check(name):
        state = _vmstate()
        _vm_state_save(vm, state)
        res = be_execprotected(vm, _bytecode_loader, name)
        if res:  # restore call stack
            _vm_state_restore(vm, state, res)
    return res


# ============================================================================
# Protected call (from be_exec.c)
# ============================================================================

# static void m_pcall(bvm *vm, void *data)
# {
#     struct pcall *p = cast(struct pcall*, data);
#     be_dofunc(vm, p->v, p->argc);
# }
def _m_pcall(vm, data):
    """Internal: execute a function call inside protected context."""
    from berry_port.be_vm import be_dofunc
    p = data  # _pcall instance
    be_dofunc(vm, p.v, p.argc)


# /* Protected call: contain any exception of fatal error and restore context
#    if something went wrong */
# int be_protectedcall(bvm *vm, bvalue *v, int argc)
# {
#     int res;
#     struct pcall s;
#     struct vmstate state;
#     s.v = v;
#     s.argc = argc;
#     vm_state_save(vm, &state);
#     res = be_execprotected(vm, m_pcall, &s);
#     if (res) { /* restore call stack */
#         vm_state_restore(vm, &state, res);
#     }
#     return res;
# }
def be_protectedcall(vm, v, argc):
    """Execute a function call with error protection.

    v is the stack index of the function value.
    argc is the number of arguments.
    Returns 0 on success, or an error code.
    """
    s = _pcall(v=v, argc=argc)
    state = _vmstate()
    _vm_state_save(vm, state)
    res = be_execprotected(vm, _m_pcall, s)
    if res:  # restore call stack
        _vm_state_restore(vm, state, res)
    return res


# ============================================================================
# be_incrtop — increment top register
# ============================================================================

# #if BE_DEBUG
# bvalue* be_incrtop(bvm *vm)
# {
#     bvalue *top = vm->top++;
#     be_assert(top < vm->stacktop);
#     return top;
# }
# #else
# /* increase top register */
# #define be_incrtop(vm)          ((vm)->top++)
# #endif
def be_incrtop(vm):
    """Increment the top-of-stack pointer.

    Returns the old top index (before increment).
    """
    old_top = vm.top
    vm.top += 1
    assert vm.top <= vm.stacktop, \
        "be_incrtop: stack overflow (top=%d, stacktop=%d)" % (vm.top, vm.stacktop)
    return old_top


# #define be_stackpop(vm, n)      ((vm)->top -= (n))
def be_stackpop(vm, n):
    """Pop n values from the stack (decrement top by n)."""
    vm.top -= n


# ============================================================================
# be_stackpush — push one value onto the stack (with space check)
# ============================================================================

# /* TODO what is the difference with be_stack_push? */
# void be_stackpush(bvm *vm)
# {
#     /* make sure there is enough stack space */
#     be_stack_require(vm, 1 + BE_STACK_FREE_MIN);
#     be_incrtop(vm);
# }
def be_stackpush(vm):
    """Push one nil value onto the VM value stack.

    Ensures there is enough stack space first (1 + BE_STACK_FREE_MIN).
    """
    be_stack_require(vm, 1 + BE_STACK_FREE_MIN)
    be_incrtop(vm)


# ============================================================================
# be_stack_require — ensure stack has room for `count` items
# ============================================================================

# /* check that the stack is able to store `count` items, and increase stack
#    if needed */
# BERRY_API void be_stack_require(bvm *vm, int count)
# {
# #if BE_USE_DEBUG_STACK == 0
#     if (vm->top + count >= vm->stacktop) {
#         be_stack_expansion(vm, count);
#     }
# #else
#     be_stack_expansion(vm, vm->top - vm->stacktop + count);
# #endif
# }
def be_stack_require(vm, count):
    """Ensure the VM value stack can hold `count` more items above top."""
    if not BE_USE_DEBUG_STACK:
        if vm.top + count >= vm.stacktop:
            be_stack_expansion(vm, count)
    else:
        be_stack_expansion(vm, vm.top - vm.stacktop + count)


# ============================================================================
# Stack resize internals (from be_exec.c)
# ============================================================================

# /* Scan the entire callstack and adjust all pointer by `offset` */
# static void update_callstack(bvm *vm, intptr_t offset)
# {
#     bcallframe *cf = be_stack_top(&vm->callstack);
#     bcallframe *base = be_stack_base(&vm->callstack);
#     for (; cf >= base; --cf) {
#         fixup_ptr(cf->func, offset);
#         fixup_ptr(cf->top, offset);
#         fixup_ptr(cf->reg, offset);
#     }
#     fixup_ptr(vm->top, offset);
#     fixup_ptr(vm->reg, offset);
# }
def _update_callstack(vm, old_stack, new_stack):
    """Update callstack pointers after stack reallocation.

    In the C implementation, this adjusts raw pointers by an offset.
    In the Python port, the stack is a Python list and func/top/reg
    are integer indices, so no fixup is needed when the list object
    changes identity. This function is a no-op in the Python port
    because we use integer indices rather than pointers.
    """
    # No-op: Python port uses integer indices for func, top, reg, etc.
    # The C code adjusts raw pointers; we don't need to.
    pass


# static void update_upvalues(bvm *vm, intptr_t offset)
# {
#     bupval *node = vm->upvalist;
#     /* update the value referenced by open upvalues */
#     for (; node != NULL; node = node->u.next) {
#         fixup_ptr(node->value, offset);
#     }
# }
def _update_upvalues(vm, old_stack, new_stack):
    """Update open upvalue pointers after stack reallocation.

    In the Python port, open upvalues store an integer index into
    the stack rather than a raw pointer, so no fixup is needed when
    the stack list is replaced. This is a no-op.
    """
    # No-op: Python port uses integer indices for upvalue references.
    pass


# /* Resize the stack to new `size` as number of elements */
# /* Then update all pointers in callstack and upvalues with the new
#    stack address */
# static void stack_resize(bvm *vm, size_t size)
# {
#     intptr_t offset;
#     bvalue *old = vm->stack;
#     size_t os = (vm->stacktop - old) * sizeof(bvalue);
# #if BE_USE_DEBUG_STACK == 0
#     vm->stack = be_realloc(vm, old, os, sizeof(bvalue) * size);
# #else   /* force a reallocation */
#     size_t ns = sizeof(bvalue) * size;
#     vm->stack = be_malloc(vm, ns);
#     size_t transf = (os < ns) ? os : ns;
#     memmove(vm->stack, old, transf);
#     memset(old, 0xFF, os);
#     be_free(vm, old, os);
# #endif
#     vm->stacktop = vm->stack + size;
#     offset = ptr_offset(vm->stack, old);
#     /* update callframes */
#     update_callstack(vm, offset);
#     /* update open upvalues */
#     update_upvalues(vm, offset);
# }
def _stack_resize(vm, size):
    """Resize the VM value stack to `size` elements.

    In the Python port, the stack is a Python list. We extend or
    shrink it as needed. Since we use integer indices (not pointers),
    callstack and upvalue fixup is a no-op.
    """
    old_size = vm.stacktop
    old_stack = vm.stack

    if size > old_size:
        # Extend: append new bvalue slots
        vm.stack.extend(bvalue() for _ in range(size - old_size))
    elif size < old_size:
        # Shrink (rare — only in debug mode)
        vm.stack = vm.stack[:size]

    vm.stacktop = size

    # In C, pointer fixup is needed. In Python with integer indices,
    # these are no-ops.
    _update_callstack(vm, old_stack, vm.stack)
    _update_upvalues(vm, old_stack, vm.stack)


# /* Stack resize internal API */
# /* Increases the stack by `n` elements, reallocate stack if needed and
#    update all callstacks and upvals */
# /* Check if we are above the max allowed stack */
# void be_stack_expansion(bvm *vm, int n)
# {
#     int size = vm->stacktop - vm->stack;
#     /* check new stack size */
#     if (size + n > BE_STACK_TOTAL_MAX) {
#         /* ensure the stack is enough when generating error messages. */
#         stack_resize(vm, size + 1);
#         be_raise(vm, "runtime_error",
#                  STACK_OVER_MSG(BE_STACK_TOTAL_MAX));
#     }
#     if (vm->obshook != NULL)
#         (*vm->obshook)(vm, BE_OBS_STACK_RESIZE_START,
#                        size * sizeof(bvalue),
#                        (size + n) * sizeof(bvalue));
#     stack_resize(vm, size + n);
# }
def be_stack_expansion(vm, n):
    """Expand the VM value stack by `n` elements.

    Checks against BE_STACK_TOTAL_MAX. Calls the observer hook if set.
    """
    size = vm.stacktop  # current stack size (int in Python port)

    # Check new stack size against maximum
    if size + n > BE_STACK_TOTAL_MAX:
        # Ensure enough stack for error message generation
        _stack_resize(vm, size + 1)
        # be_raise is in be_api.py; use be_throw_message as equivalent
        be_throw_message(vm, BE_EXEC_ERROR,
                         _STACK_OVER_MSG(BE_STACK_TOTAL_MAX))

    # Notify observer hook
    if hasattr(vm, 'obshook') and vm.obshook is not None:
        # In C, args are byte sizes; we pass element counts for simplicity
        vm.obshook(vm, BE_OBS_STACK_RESIZE_START, size, size + n)

    _stack_resize(vm, size + n)


# ============================================================================
# Exception stack management (from be_exec.c)
# ============================================================================

# static void fixup_exceptstack(bvm* vm, struct bexecptframe* lbase)
# {
#     struct bexecptframe *base = be_stack_base(&vm->exceptstack);
#     if (lbase != base) {
#         struct bexecptframe *top = be_stack_top(&vm->exceptstack);
#         bbyte *begin = (bbyte*)&lbase->errjmp;
#         bbyte *end = (bbyte*)&(lbase + (top - base))->errjmp;
#         intptr_t offset = ptr_offset(base, lbase);
#         struct blongjmp *errjmp = vm->errjmp;
#         while (errjmp) {
#             bbyte *prev = (bbyte*)errjmp->prev;
#             if (prev >= begin && prev < end) {
#                 fixup_ptr(prev, offset);
#                 errjmp->prev = (struct blongjmp*)prev;
#             }
#             errjmp = (struct blongjmp*)prev;
#         }
#     }
# }
def _fixup_exceptstack(vm, lbase_snapshot):
    """Fix up errjmp chain after exception stack reallocation.

    In the C implementation, this adjusts raw pointers in the errjmp
    linked list when the exception stack's backing memory moves.

    In the Python port, the exception stack uses a Python list with
    integer indices, and errjmp is managed via Python exception
    handling. The errjmp chain is maintained as a list of sentinel
    objects, so no pointer fixup is needed. This is a no-op.
    """
    # No-op in Python port: we don't have raw pointers to fix up.
    pass


# /* set an exception handling recovery point. To do this, we have to
#  * push some VM states into the exception stack. */
# void be_except_block_setup(bvm *vm)
# {
#     struct bexecptframe *frame;
#     struct bexecptframe *lbase = be_stack_base(&vm->exceptstack);
#     be_stack_push(vm, &vm->exceptstack, NULL);
#     frame = be_stack_top(&vm->exceptstack);
#     frame->depth = be_stack_count(&vm->callstack);
#     frame->ip = vm->ip;
#     /* set longjmp() jump point */
#     frame->errjmp.status = 0;
#     frame->errjmp.prev = vm->errjmp;
#     frame->refcount = vm->refstack.count;
#     vm->errjmp = &frame->errjmp;
#     fixup_exceptstack(vm, lbase);
# }
def be_except_block_setup(vm):
    """Set up an exception handling recovery point.

    Pushes a new exception frame onto the exception stack, saving
    the current call depth, instruction pointer, and errjmp chain.
    """
    # Snapshot the base before push (for fixup — no-op in Python)
    lbase_snapshot = be_stack_count(vm.exceptstack)

    # Push a new exception frame
    frame = bexecptframe()
    frame.depth = be_stack_count(vm.callstack)  # call stack depth
    frame.ip = vm.ip_idx                        # instruction index (next after OP_EXBLK)
    frame.ip_code = vm.ip                        # code array reference (for resume)
    frame.refcount = vm.refstack.count           # reference stack count

    # Set up errjmp chain: save previous errjmp in the frame
    frame.errjmp = vm.errjmp  # save previous errjmp
    vm.errjmp = frame         # point errjmp to this frame

    be_stack_push(vm, vm.exceptstack, frame)

    # Fixup (no-op in Python port)
    _fixup_exceptstack(vm, lbase_snapshot)


# /* resumes to the state of the previous frame when an exception occurs. */
# void be_except_block_resume(bvm *vm)
# {
#     int errorcode = vm->errjmp->status;
#     struct bexecptframe *frame = be_stack_top(&vm->exceptstack);
#     if (errorcode == BE_EXCEPTION) {
#         vm->errjmp = vm->errjmp->prev;
#         be_vector_resize(vm, &vm->refstack, frame->refcount);
#         /* jump to except instruction */
#         vm->ip = frame->ip + IGET_sBx(frame->ip[-1]);
#         if (be_stack_count(&vm->callstack) > frame->depth) {
#             bvalue *top = vm->top;
#             bcallframe *cf = be_vector_at(&vm->callstack, frame->depth);
#             vm->top = cf->top;
#             vm->reg = cf->reg;
#             vm->cf = frame->depth ? cf - 1 : NULL;
#             be_vector_resize(vm, &vm->callstack, frame->depth);
#             /* copy the exception value and argument to the top of
#              * the current function */
#             vm->top[0] = top[0]; /* exception value */
#             vm->top[1] = top[1]; /* exception argument */
#         }
#         be_stack_pop(&vm->exceptstack);
#     } else { /* other errors cannot be catch by the except block */
#         /* find the next error handling location */
#         while (vm->errjmp == &frame->errjmp) {
#             vm->errjmp = vm->errjmp->prev;
#             be_stack_pop(&vm->exceptstack);
#             frame = be_stack_top(&vm->exceptstack);
#         }
#         be_throw(vm, errorcode); /* rethrow this exception */
#     }
# }
def be_except_block_resume(vm, errorcode=None):
    """Resume to the previous exception frame state.

    Called when an exception occurs during VM execution. If the error
    is BE_EXCEPTION, restores state and jumps to the except handler.
    Otherwise, pops frames until a matching handler is found and
    rethrows.

    The errorcode parameter replaces C's vm->errjmp->status since
    in the Python port the error code comes from the BerryException.
    """
    if errorcode is None:
        # In C, errorcode comes from vm->errjmp->status.
        # In Python, the caller should pass it from the caught exception.
        errorcode = BE_EXCEPTION

    frame_idx = be_stack_count(vm.exceptstack) - 1
    frame = be_vector_at(vm.exceptstack, frame_idx)

    if errorcode == BE_EXCEPTION:
        # Restore errjmp to previous level
        vm.errjmp = frame.errjmp  # frame.errjmp holds the previous errjmp

        # Restore reference stack
        be_vector_resize(vm, vm.refstack, frame.refcount)

        # Jump to except instruction:
        # vm->ip = frame->ip + IGET_sBx(frame->ip[-1])
        # frame.ip is an int index into the code array
        # We need the code array from the current closure to read ip[-1]
        code = frame.ip_code if hasattr(frame, 'ip_code') else vm.ip
        prev_ins = code[frame.ip - 1] if frame.ip > 0 else 0
        vm.ip = code  # restore code array reference
        vm.ip_idx = frame.ip + IGET_sBx(prev_ins)

        if be_stack_count(vm.callstack) > frame.depth:
            old_top = vm.top
            cf = be_vector_at(vm.callstack, frame.depth)
            vm.top = cf.top
            vm.reg = cf.reg
            if frame.depth > 0:
                vm.cf = be_vector_at(vm.callstack, frame.depth - 1)
            else:
                vm.cf = None
            be_vector_resize(vm, vm.callstack, frame.depth)
            # Copy exception value and argument
            vm.stack[vm.top] = vm.stack[old_top]      # exception value
            if old_top + 1 < len(vm.stack):
                vm.stack[vm.top + 1] = vm.stack[old_top + 1]  # exception arg

        be_stack_pop(vm.exceptstack)
    else:
        # Other errors cannot be caught by the except block.
        # Pop frames until we find a different errjmp level.
        while be_stack_count(vm.exceptstack) > 0:
            frame = be_vector_at(vm.exceptstack,
                                 be_stack_count(vm.exceptstack) - 1)
            if vm.errjmp is not frame:
                break
            vm.errjmp = frame.errjmp  # restore previous
            be_stack_pop(vm.exceptstack)
        be_throw(vm, errorcode)  # rethrow


# /* only close the except block, no other operations */
# void be_except_block_close(bvm *vm, int count)
# {
#     struct bexecptframe *frame;
#     int size = be_stack_count(&vm->exceptstack);
#     be_assert(count > 0 && count <= size);
#     frame = be_vector_at(&vm->exceptstack, size - count);
#     vm->errjmp = frame->errjmp.prev;
#     be_vector_resize(vm, &vm->exceptstack, size - count);
# }
def be_except_block_close(vm, count):
    """Close `count` exception blocks without other operations.

    Restores the errjmp chain and shrinks the exception stack.
    """
    size = be_stack_count(vm.exceptstack)
    assert count > 0 and count <= size, \
        "be_except_block_close: invalid count %d (size=%d)" % (count, size)
    frame = be_vector_at(vm.exceptstack, size - count)
    # frame.errjmp holds the previous errjmp saved when this block was set up
    vm.errjmp = frame.errjmp
    be_vector_resize(vm, vm.exceptstack, size - count)


# ============================================================================
# be_save_stacktrace — save call stack trace for error reporting
# ============================================================================

# void be_save_stacktrace(bvm *vm)
# {
#     bstack *stack = &vm->tracestack;
#     be_stack_clear(stack);
#     if (be_stack_count(&vm->callstack)) {
#         bcallframe *cf;
#         bcallframe *base = be_stack_base(&vm->callstack);
#         bcallframe *top = be_stack_top(&vm->callstack);
#         for (cf = base; cf <= top; ++cf) {
#             bcallsnapshot *st;
#             be_stack_push(vm, stack, NULL);
#             st = be_stack_top(stack);
#             st->func = *cf->func;
#             st->ip = cf == top ? vm->ip : cf[1].ip;
#         }
#     }
# }
def be_save_stacktrace(vm):
    """Save the current call stack trace for error reporting.

    Iterates through the call stack and saves a snapshot of each
    frame (function value + instruction pointer) into vm.tracestack.
    """
    stack = vm.tracestack
    be_stack_clear(stack)

    count = be_stack_count(vm.callstack)
    if count > 0:
        for i in range(count):
            cf = be_vector_at(vm.callstack, i)
            snapshot = bcallsnapshot()
            # Copy the function value from the stack
            func_val = vm.stack[cf.func]
            snapshot.func = bvalue()
            snapshot.func.type = func_val.type
            snapshot.func.v = func_val.v
            # For the top frame, use vm.ip_idx; for others, use next frame's ip_idx
            if i == count - 1:
                snapshot.ip = vm.ip_idx
            else:
                next_cf = be_vector_at(vm.callstack, i + 1)
                snapshot.ip = next_cf.ip_idx
            be_stack_push(vm, stack, snapshot)


# ============================================================================
# Bytecode saver (from be_exec.c)
# ============================================================================

# #if BE_USE_BYTECODE_SAVER
# static void _bytecode_save(bvm *vm, void *data)
# {
#     if (be_top(vm) > 0 && var_isclosure(vm->top - 1)) {
#         bclosure *cl = var_toobj(vm->top - 1);
#         be_bytecode_save(vm, (const char *)data, cl->proto);
#     }
# }
#
# /* save bytecode file */
# BERRY_API int be_savecode(bvm *vm, const char *name)
# {
#     int res;
#     struct vmstate state;
#     vm_state_save(vm, &state);
#     res = be_execprotected(vm, _bytecode_save, (void *)name);
#     if (res) { /* restore call stack */
#         vm_state_restore(vm, &state, res);
#     }
#     return res;
# }
# #endif
def _bytecode_save_func(vm, data):
    """Internal: save bytecode for the closure at top of stack."""
    from berry_port.be_bytecode import be_bytecode_save as _bc_save
    # be_top equivalent: vm.top - stack base (simplified)
    if vm.top > 0 and var_isclosure(vm.stack[vm.top - 1]):
        cl = var_toobj(vm.stack[vm.top - 1])
        _bc_save(vm, data, cl.proto)


def be_savecode(vm, name):
    """Save bytecode to a file with error protection.

    Returns 0 on success, or an error code.
    """
    if not BE_USE_BYTECODE_SAVER:
        return BE_IO_ERROR
    state = _vmstate()
    _vm_state_save(vm, state)
    res = be_execprotected(vm, _bytecode_save_func, name)
    if res:  # restore call stack
        _vm_state_restore(vm, state, res)
    return res


# ============================================================================
# Internal helper: ensure stack space without circular import
# ============================================================================

def _ensure_stack_space(vm, count):
    """Ensure the VM stack has room for `count` more items.

    Lightweight version that extends the Python list directly,
    used internally to avoid circular imports with be_stack_require.
    """
    needed = vm.top + count
    if needed >= len(vm.stack):
        vm.stack.extend(bvalue() for _ in range(needed - len(vm.stack) + 1))
    if needed >= vm.stacktop:
        vm.stacktop = needed + 1

def be_loadmodule(vm, name):
    """Load a Berry module from file (macro equivalent: be_loadmode(vm, name, 1))."""
    return be_loadmode(vm, name, 1)
