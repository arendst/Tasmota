"""
Tests for the Berry file library (be_filelib.py).

Covers: open(), file.read(), file.readbytes(), file.readline(),
        file.write(), file.seek(), file.tell(), file.size(),
        file.flush(), file.close(), file.deinit(), file.savecode(),
        error handling for missing files and type() of file instances.
"""

import sys
import os
import tempfile
import shutil

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------
@pytest.fixture
def vm():
    v = be_vm_new()
    yield v
    be_vm_delete(v)


@pytest.fixture
def tmpdir():
    d = tempfile.mkdtemp()
    yield d
    shutil.rmtree(d, ignore_errors=True)


# ---------------------------------------------------------------------------
# open() basics
# ---------------------------------------------------------------------------
def test_open_returns_instance(vm, tmpdir):
    """open() should return a file instance."""
    path = os.path.join(tmpdir, "test.txt")
    with open(path, 'w') as f:
        f.write("hello")
    code = f"""
var f = open('{path}')
assert(type(f) == 'instance')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_open_nonexistent_raises(vm):
    """open() on a nonexistent file should raise io_error."""
    code = """
var ok = false
try
    open('/nonexistent/path/no_such_file.txt')
except 'io_error' as e, m
    ok = true
end
assert(ok)
"""
    assert be_dostring(vm, code) == BE_OK


def test_open_no_args_returns_nil(vm):
    """open() with no arguments should return nil."""
    code = """
var f = open()
assert(f == nil)
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.read()
# ---------------------------------------------------------------------------
def test_read_entire_file(vm, tmpdir):
    """file.read() with no args reads the entire file."""
    path = os.path.join(tmpdir, "read_all.txt")
    with open(path, 'w') as f:
        f.write("hello world")
    code = f"""
var f = open('{path}')
var s = f.read()
assert(s == 'hello world')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_read_partial(vm, tmpdir):
    """file.read(n) reads exactly n bytes."""
    path = os.path.join(tmpdir, "read_partial.txt")
    with open(path, 'w') as f:
        f.write("abcdefghij")
    code = f"""
var f = open('{path}')
var s = f.read(5)
assert(s == 'abcde')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_read_empty_file(vm, tmpdir):
    """file.read() on an empty file returns empty string."""
    path = os.path.join(tmpdir, "empty.txt")
    with open(path, 'w') as f:
        pass
    code = f"""
var f = open('{path}')
var s = f.read()
assert(s == '')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.readline()
# ---------------------------------------------------------------------------
def test_readline(vm, tmpdir):
    """file.readline() reads one line including the newline."""
    path = os.path.join(tmpdir, "lines.txt")
    with open(path, 'w') as f:
        f.write("line1\nline2\nline3\n")
    code = f"""
var f = open('{path}')
var l1 = f.readline()
assert(l1 == 'line1\\n')
var l2 = f.readline()
assert(l2 == 'line2\\n')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_readline_no_trailing_newline(vm, tmpdir):
    """file.readline() on a file without trailing newline."""
    path = os.path.join(tmpdir, "noeol.txt")
    with open(path, 'w') as f:
        f.write("only line")
    code = f"""
var f = open('{path}')
var l = f.readline()
assert(l == 'only line')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.write()
# ---------------------------------------------------------------------------
def test_write_string(vm, tmpdir):
    """file.write() writes a string to the file."""
    path = os.path.join(tmpdir, "write_test.txt")
    code = f"""
var f = open('{path}', 'w')
f.write('hello berry')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    with open(path, 'rb') as f:
        assert f.read() == b'hello berry'


def test_write_bytes(vm, tmpdir):
    """file.write() can write bytes objects."""
    path = os.path.join(tmpdir, "write_bytes.bin")
    code = f"""
var f = open('{path}', 'w')
var b = bytes().fromstring('\\x00\\x01\\x02')
f.write(b)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    with open(path, 'rb') as f:
        content = f.read()
    assert len(content) == 3


def test_write_and_read_roundtrip(vm, tmpdir):
    """Write then read back should produce the same content."""
    path = os.path.join(tmpdir, "roundtrip.txt")
    code = f"""
var f = open('{path}', 'w')
f.write('roundtrip test 123')
f.close()
var f2 = open('{path}')
var s = f2.read()
assert(s == 'roundtrip test 123')
f2.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.seek() / file.tell()
# ---------------------------------------------------------------------------
def test_tell_starts_at_zero(vm, tmpdir):
    """file.tell() should be 0 at the start."""
    path = os.path.join(tmpdir, "tell.txt")
    with open(path, 'w') as f:
        f.write("content")
    code = f"""
var f = open('{path}')
assert(f.tell() == 0)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_seek_and_tell(vm, tmpdir):
    """file.seek() moves position, file.tell() reports it."""
    path = os.path.join(tmpdir, "seek.txt")
    with open(path, 'w') as f:
        f.write("0123456789")
    code = f"""
var f = open('{path}')
f.seek(5)
assert(f.tell() == 5)
var s = f.read(3)
assert(s == '567')
assert(f.tell() == 8)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_seek_to_beginning(vm, tmpdir):
    """file.seek(0) rewinds to the start."""
    path = os.path.join(tmpdir, "rewind.txt")
    with open(path, 'w') as f:
        f.write("abcdef")
    code = f"""
var f = open('{path}')
f.read(3)
f.seek(0)
var s = f.read(2)
assert(s == 'ab')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.size()
# ---------------------------------------------------------------------------
def test_size(vm, tmpdir):
    """file.size() returns the file size in bytes."""
    path = os.path.join(tmpdir, "sized.txt")
    with open(path, 'w') as f:
        f.write("12345")
    code = f"""
var f = open('{path}')
assert(f.size() == 5)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_size_preserves_position(vm, tmpdir):
    """file.size() should not change the current position."""
    path = os.path.join(tmpdir, "sizepos.txt")
    with open(path, 'w') as f:
        f.write("0123456789")
    code = f"""
var f = open('{path}')
f.seek(3)
var sz = f.size()
assert(sz == 10)
assert(f.tell() == 3)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.flush()
# ---------------------------------------------------------------------------
def test_flush(vm, tmpdir):
    """file.flush() should not raise."""
    path = os.path.join(tmpdir, "flush.txt")
    code = f"""
var f = open('{path}', 'w')
f.write('data')
f.flush()
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    with open(path, 'rb') as f:
        assert f.read() == b'data'


# ---------------------------------------------------------------------------
# file.close() / file.deinit()
# ---------------------------------------------------------------------------
def test_close(vm, tmpdir):
    """file.close() should work without error."""
    path = os.path.join(tmpdir, "close.txt")
    with open(path, 'w') as f:
        f.write("x")
    code = f"""
var f = open('{path}')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.readbytes()
# ---------------------------------------------------------------------------
def test_readbytes_entire(vm, tmpdir):
    """file.readbytes() reads the entire file as bytes."""
    path = os.path.join(tmpdir, "readbytes.bin")
    with open(path, 'wb') as f:
        f.write(b'\x00\x01\x02\x03\x04')
    code = f"""
var f = open('{path}')
var b = f.readbytes()
assert(size(b) == 5)
assert(b[0] == 0)
assert(b[4] == 4)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


def test_readbytes_partial(vm, tmpdir):
    """file.readbytes(n) reads exactly n bytes."""
    path = os.path.join(tmpdir, "readbytes_partial.bin")
    with open(path, 'wb') as f:
        f.write(b'\x10\x20\x30\x40\x50')
    code = f"""
var f = open('{path}')
var b = f.readbytes(3)
assert(size(b) == 3)
assert(b[0] == 0x10)
assert(b[2] == 0x30)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.savecode()
# ---------------------------------------------------------------------------
def test_savecode(vm, tmpdir):
    """file.savecode() saves a closure as bytecode that can be loaded back."""
    bec_path = os.path.join(tmpdir, "func.bec")
    code = f"""
def myfunc()
    return 42
end
var f = open('{bec_path}', 'w')
f.savecode(myfunc)
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    assert os.path.exists(bec_path)
    assert os.path.getsize(bec_path) > 0


def test_savecode_type_error(vm, tmpdir):
    """file.savecode() with non-closure should raise type_error."""
    path = os.path.join(tmpdir, "bad.bec")
    code = f"""
var ok = false
var f = open('{path}', 'w')
try
    f.savecode(42)
except 'type_error' as e, m
    ok = true
end
f.close()
assert(ok)
"""
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Write mode variations
# ---------------------------------------------------------------------------
def test_open_write_mode(vm, tmpdir):
    """open(path, 'w') creates/truncates a file."""
    path = os.path.join(tmpdir, "wmode.txt")
    with open(path, 'w') as f:
        f.write("old content")
    code = f"""
var f = open('{path}', 'w')
f.write('new')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    with open(path, 'rb') as f:
        assert f.read() == b'new'


def test_open_append_mode(vm, tmpdir):
    """open(path, 'a') appends to existing content."""
    path = os.path.join(tmpdir, "amode.txt")
    with open(path, 'wb') as f:
        f.write(b'first')
    code = f"""
var f = open('{path}', 'a')
f.write('second')
f.close()
"""
    assert be_dostring(vm, code) == BE_OK
    with open(path, 'rb') as f:
        assert f.read() == b'firstsecond'


# ---------------------------------------------------------------------------
# _BinaryFileWrapper tests
# ---------------------------------------------------------------------------

def test_binary_file_wrapper_write_str(tmpdir):
    """_BinaryFileWrapper.write() should accept str and encode to UTF-8."""
    from berry_port.be_filelib import _BinaryFileWrapper
    path = os.path.join(tmpdir, "wrapper_str.bin")
    raw = open(path, 'wb')
    w = _BinaryFileWrapper(raw)
    w.write("hello world")
    w.close()
    with open(path, 'rb') as f:
        assert f.read() == b'hello world'


def test_binary_file_wrapper_write_bytes(tmpdir):
    """_BinaryFileWrapper.write() should pass bytes through unchanged."""
    from berry_port.be_filelib import _BinaryFileWrapper
    path = os.path.join(tmpdir, "wrapper_bytes.bin")
    raw = open(path, 'wb')
    w = _BinaryFileWrapper(raw)
    w.write(b'\x00\xff\x80')
    w.close()
    with open(path, 'rb') as f:
        assert f.read() == b'\x00\xff\x80'


def test_binary_file_wrapper_delegates_attrs(tmpdir):
    """_BinaryFileWrapper should delegate attributes like seek/tell/flush."""
    from berry_port.be_filelib import _BinaryFileWrapper
    path = os.path.join(tmpdir, "wrapper_delegate.bin")
    raw = open(path, 'wb')
    w = _BinaryFileWrapper(raw)
    w.write("abcdef")
    w.flush()
    assert w.tell() == 6
    w.seek(0)
    assert w.tell() == 0
    w.close()


def test_binary_file_wrapper_utf8(tmpdir):
    """_BinaryFileWrapper should correctly encode non-ASCII str to UTF-8."""
    from berry_port.be_filelib import _BinaryFileWrapper
    path = os.path.join(tmpdir, "wrapper_utf8.bin")
    raw = open(path, 'wb')
    w = _BinaryFileWrapper(raw)
    w.write("café")
    w.close()
    with open(path, 'rb') as f:
        assert f.read() == "café".encode('utf-8')
