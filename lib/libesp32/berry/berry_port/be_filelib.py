# Port of src/be_filelib.c to Python
# Implements the 'file' class and 'open' builtin function.

# -- lazy imports to avoid circular dependencies -----------------------
def _lazy_be_api():
    from berry_port import be_api
    return be_api

def _lazy_be_byteslib():
    from berry_port import be_byteslib
    return be_byteslib

def _lazy_be_bytecode():
    from berry_port import be_bytecode
    return be_bytecode

from berry_port.be_object import (
    var_isclosure, var_toobj,
)
from berry_port.berry_conf import BE_USE_BYTECODE_SAVER


class _BinaryFileWrapper:
    """Wrap a binary-mode file so .write() accepts both str and bytes.

    Berry opens all files in binary mode (matching C semantics), but
    Python code that serialises to C source (solidify, debug, etc.)
    produces str objects.  This wrapper transparently encodes str to
    UTF-8 on write while forwarding every other attribute to the
    underlying file object.
    """

    __slots__ = ('_fh',)

    def __init__(self, fh):
        self._fh = fh

    def write(self, data):
        if isinstance(data, str):
            data = data.encode('utf-8')
        return self._fh.write(data)

    def __getattr__(self, name):
        return getattr(self._fh, name)


def i_write(vm):
    """file.write(data) — write string or bytes to file."""
    be_api = _lazy_be_api()
    be_byteslib = _lazy_be_byteslib()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        if be_api.be_isstring(vm, 2):
            data = be_api.be_tostring(vm, 2)
            raw = data.encode('utf-8') if isinstance(data, str) else data
            try:
                bw = fh.write(raw)
            except Exception:
                be_api.be_raise(vm, "io_error", "write failed")
                return be_api.be_returnnilvalue(vm)
            if bw != len(raw):
                be_api.be_raise(vm, "io_error", "write failed")
                return be_api.be_returnnilvalue(vm)
        elif be_byteslib.be_isbytes(vm, 2):
            buf, size = be_byteslib.be_tobytes(vm, 2)
            if buf is not None and size > 0:
                raw = bytes(buf[:size])
                try:
                    bw = fh.write(raw)
                except Exception:
                    be_api.be_raise(vm, "io_error", "write failed")
                    return be_api.be_returnnilvalue(vm)
                if bw != len(raw):
                    be_api.be_raise(vm, "io_error", "write failed")
                    return be_api.be_returnnilvalue(vm)
    return be_api.be_returnnilvalue(vm)


def _readsize(vm, argc, fh):
    """Determine how many bytes to read."""
    be_api = _lazy_be_api()
    if argc >= 2 and be_api.be_isint(vm, 2):
        return be_api.be_toint(vm, 2)
    pos = fh.tell()
    fh.seek(0, 2)
    end = fh.tell()
    fh.seek(pos)
    return end - pos


def i_read(vm):
    """file.read([size]) — read string from file."""
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        size = _readsize(vm, argc, fh)
        if size:
            data = fh.read(size)
            if isinstance(data, bytes):
                # Decode as latin-1 to preserve raw byte values.
                # Berry strings store UTF-8 byte sequences as latin-1
                # Python strings (one char per byte).  Decoding as UTF-8
                # would collapse multi-byte sequences (e.g. \xC2\xB0 for
                # '°') into single codepoints, losing bytes.
                text = data.decode('latin-1')
            else:
                text = data
            be_api.be_pushstring(vm, text)
        else:
            be_api.be_pushstring(vm, "")
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def i_readbytes(vm):
    """file.readbytes([size]) — read bytes from file."""
    be_api = _lazy_be_api()
    be_byteslib = _lazy_be_byteslib()
    argc = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        size = _readsize(vm, argc, fh)
        if size:
            data = fh.read(size)
            if not isinstance(data, (bytes, bytearray)):
                data = data.encode('utf-8')
            be_byteslib.be_pushbytes(vm, data, len(data))
        else:
            be_byteslib.be_pushbytes(vm, b'', 0)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def i_readline(vm):
    """file.readline() — read one line from file."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        line = b''
        while True:
            ch = fh.read(1)
            if not ch:
                break
            line += ch
            if ch == b'\n':
                break
        if isinstance(line, bytes):
            # Decode as latin-1 to preserve raw byte values (see i_read).
            text = line.decode('latin-1')
        else:
            text = line
        be_api.be_pushstring(vm, text)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def i_seek(vm):
    """file.seek(offset) — seek to position."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1) and be_api.be_isint(vm, 2):
        fh = be_api.be_tocomptr(vm, -1)
        fh.seek(be_api.be_toint(vm, 2))
    return be_api.be_returnnilvalue(vm)


def i_tell(vm):
    """file.tell() — return current position."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        pos = fh.tell()
        be_api.be_pushint(vm, pos)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def i_size(vm):
    """file.size() — return file size."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        cur = fh.tell()
        fh.seek(0, 2)
        size = fh.tell()
        fh.seek(cur)
        be_api.be_pushint(vm, size)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def i_flush(vm):
    """file.flush() — flush file buffers."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        fh.flush()
    return be_api.be_returnnilvalue(vm)


def i_close(vm):
    """file.close() / file.deinit() — close the file handle."""
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if be_api.be_iscomptr(vm, -1):
        fh = be_api.be_tocomptr(vm, -1)
        fh.close()
        be_api.be_pushnil(vm)
        be_api.be_setmember(vm, 1, ".p")
    return be_api.be_returnnilvalue(vm)


def i_savecode(vm):
    """file.savecode(closure) — save bytecode to file."""
    be_api = _lazy_be_api()
    be_bytecode = _lazy_be_bytecode()
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isclosure(vm, 2):
        be_api.be_getmember(vm, 1, ".p")
        if be_api.be_iscomptr(vm, -1):
            fh = be_api.be_tocomptr(vm, -1)
            v = vm.stack[be_api.be_indexof(vm, 2)]
            if var_isclosure(v):
                cl = var_toobj(v)
                pr = cl.proto
                be_bytecode.be_bytecode_save_to_fs(vm, fh, pr)
    else:
        be_api.be_raise(vm, "type_error", "closure expected")
    return be_api.be_returnnilvalue(vm)


def be_nfunc_open(vm):
    """open(filename [, mode]) — builtin function to open a file."""
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    fname = None
    mode = "r"
    if argc >= 1 and be_api.be_isstring(vm, 1):
        fname = be_api.be_tostring(vm, 1)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        mode = be_api.be_tostring(vm, 2)
    if fname:
        # Berry uses C-style modes; always open in binary to match C semantics
        py_mode = mode
        if 'b' not in py_mode:
            py_mode = py_mode + 'b'
        try:
            fh = _BinaryFileWrapper(open(fname, py_mode))
        except OSError:
            be_api.be_raise(vm, "io_error",
                be_api.be_pushfstring(vm, "cannot open file '%s'", fname))
            return be_api.be_returnnilvalue(vm)

        members = [
            (".p", None),
            ("write", i_write),
            ("read", i_read),
            ("readbytes", i_readbytes),
            ("readline", i_readline),
            ("seek", i_seek),
            ("tell", i_tell),
            ("size", i_size),
            ("flush", i_flush),
            ("close", i_close),
            ("deinit", i_close),
        ]
        if BE_USE_BYTECODE_SAVER:
            members.append(("savecode", i_savecode))
        members.append((None, None))

        be_api.be_pushclass(vm, "file", members)
        be_api.be_call(vm, 0)
        be_api.be_pushcomptr(vm, fh)
        be_api.be_setmember(vm, -2, ".p")
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def be_load_filelib(vm):
    """Register the 'open' builtin function."""
    be_api = _lazy_be_api()
    be_api.be_regfunc(vm, "open", be_nfunc_open)
