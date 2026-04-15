"""
Tests for the Berry os module (be_oslib.py).

Covers: os.getcwd(), os.chdir(), os.mkdir(), os.remove(), os.listdir(),
        os.system(), os.exit(), os.path.isdir(), os.path.isfile(),
        os.path.exists(), os.path.split(), os.path.splitext(), os.path.join()
"""

import sys
import os
import tempfile
import shutil

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


# ---------------------------------------------------------------------------
# Helper: create a fresh VM for each test
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
# os.getcwd()
# ---------------------------------------------------------------------------
def test_os_getcwd(vm):
    """os.getcwd() should return the current working directory."""
    code = "import os\nassert(type(os.getcwd()) == 'string')"
    assert be_dostring(vm, code) == BE_OK


def test_os_getcwd_matches_python(vm, tmpdir):
    """os.getcwd() after chdir should match the target directory."""
    old_cwd = os.getcwd()
    try:
        real_tmpdir = os.path.realpath(tmpdir)
        os.chdir(real_tmpdir)
        escaped = real_tmpdir.replace("'", "\\'")
        code = "import os\nassert(os.getcwd() == '" + escaped + "')"
        assert be_dostring(vm, code) == BE_OK
    finally:
        os.chdir(old_cwd)


# ---------------------------------------------------------------------------
# os.chdir()
# ---------------------------------------------------------------------------
def test_os_chdir(vm, tmpdir):
    """os.chdir() should change directory and return true."""
    old_cwd = os.getcwd()
    try:
        real_tmpdir = os.path.realpath(tmpdir)
        escaped = real_tmpdir.replace("'", "\\'")
        code = (
            "import os\n"
            "assert(os.chdir('" + escaped + "') == true)\n"
            "assert(os.getcwd() == '" + escaped + "')"
        )
        assert be_dostring(vm, code) == BE_OK
    finally:
        os.chdir(old_cwd)


def test_os_chdir_nonexistent(vm):
    """os.chdir() with nonexistent path should return false."""
    code = "import os\nassert(os.chdir('/nonexistent_path_xyz_12345') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.mkdir()
# ---------------------------------------------------------------------------
def test_os_mkdir(vm, tmpdir):
    """os.mkdir() should create a directory and return true."""
    target = os.path.join(tmpdir, "newdir")
    escaped = target.replace("'", "\\'")
    code = (
        "import os\n"
        "assert(os.mkdir('" + escaped + "') == true)"
    )
    assert be_dostring(vm, code) == BE_OK
    assert os.path.isdir(target)


def test_os_mkdir_existing(vm, tmpdir):
    """os.mkdir() on existing directory should return false."""
    escaped = tmpdir.replace("'", "\\'")
    code = "import os\nassert(os.mkdir('" + escaped + "') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.remove()
# ---------------------------------------------------------------------------
def test_os_remove(vm, tmpdir):
    """os.remove() should delete a file and return true."""
    target = os.path.join(tmpdir, "testfile.txt")
    with open(target, 'w') as f:
        f.write("hello")
    escaped = target.replace("'", "\\'")
    code = "import os\nassert(os.remove('" + escaped + "') == true)"
    assert be_dostring(vm, code) == BE_OK
    assert not os.path.exists(target)


def test_os_remove_nonexistent(vm):
    """os.remove() on nonexistent file should return false."""
    code = "import os\nassert(os.remove('/nonexistent_file_xyz_12345') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.listdir()
# ---------------------------------------------------------------------------
def test_os_listdir(vm, tmpdir):
    """os.listdir() should return a list of entries (excluding . and ..)."""
    # Create some files
    for name in ["a.txt", "b.txt", "c.txt"]:
        with open(os.path.join(tmpdir, name), 'w') as f:
            f.write("")
    escaped = tmpdir.replace("'", "\\'")
    code = (
        "import os\n"
        "var l = os.listdir('" + escaped + "')\n"
        "assert(classname(l) == 'list')\n"
        "assert(l.size() == 3)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_listdir_default(vm):
    """os.listdir() with no args should list current directory."""
    code = (
        "import os\n"
        "var l = os.listdir()\n"
        "assert(classname(l) == 'list')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.system()
# ---------------------------------------------------------------------------
def test_os_system(vm):
    """os.system() should execute a command and return exit code."""
    code = (
        "import os\n"
        "var r = os.system('true')\n"
        "assert(type(r) == 'int')\n"
        "assert(r == 0)\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.exit() — we test indirectly since it calls be_exit which raises
# ---------------------------------------------------------------------------
def test_os_exit(vm):
    """os.exit() should trigger a VM exit."""
    # be_exit raises BerryExit, which be_dostring should handle
    # The return code should be BE_EXIT (1)
    from berry_port.berry import BE_EXIT
    code = "import os\nos.exit(0)"
    result = be_dostring(vm, code)
    assert result == BE_EXIT


# ---------------------------------------------------------------------------
# os.path.isdir()
# ---------------------------------------------------------------------------
def test_os_path_isdir(vm, tmpdir):
    """os.path.isdir() should return true for directories."""
    escaped = tmpdir.replace("'", "\\'")
    code = "import os\nassert(os.path.isdir('" + escaped + "') == true)"
    assert be_dostring(vm, code) == BE_OK


def test_os_path_isdir_file(vm, tmpdir):
    """os.path.isdir() should return false for files."""
    target = os.path.join(tmpdir, "file.txt")
    with open(target, 'w') as f:
        f.write("")
    escaped = target.replace("'", "\\'")
    code = "import os\nassert(os.path.isdir('" + escaped + "') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.path.isfile()
# ---------------------------------------------------------------------------
def test_os_path_isfile(vm, tmpdir):
    """os.path.isfile() should return true for files."""
    target = os.path.join(tmpdir, "file.txt")
    with open(target, 'w') as f:
        f.write("")
    escaped = target.replace("'", "\\'")
    code = "import os\nassert(os.path.isfile('" + escaped + "') == true)"
    assert be_dostring(vm, code) == BE_OK


def test_os_path_isfile_dir(vm, tmpdir):
    """os.path.isfile() should return false for directories."""
    escaped = tmpdir.replace("'", "\\'")
    code = "import os\nassert(os.path.isfile('" + escaped + "') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.path.exists()
# ---------------------------------------------------------------------------
def test_os_path_exists_dir(vm, tmpdir):
    """os.path.exists() should return true for existing directories."""
    escaped = tmpdir.replace("'", "\\'")
    code = "import os\nassert(os.path.exists('" + escaped + "') == true)"
    assert be_dostring(vm, code) == BE_OK


def test_os_path_exists_file(vm, tmpdir):
    """os.path.exists() should return true for existing files."""
    target = os.path.join(tmpdir, "file.txt")
    with open(target, 'w') as f:
        f.write("")
    escaped = target.replace("'", "\\'")
    code = "import os\nassert(os.path.exists('" + escaped + "') == true)"
    assert be_dostring(vm, code) == BE_OK


def test_os_path_exists_nonexistent(vm):
    """os.path.exists() should return false for nonexistent paths."""
    code = "import os\nassert(os.path.exists('/nonexistent_xyz_12345') == false)"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.path.split()
# ---------------------------------------------------------------------------
def test_os_path_split_basic(vm):
    """os.path.split() should split path into [dir, filename]."""
    code = (
        "import os\n"
        "var r = os.path.split('/home/user/file.txt')\n"
        "assert(r[0] == '/home/user')\n"
        "assert(r[1] == 'file.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_path_split_no_dir(vm):
    """os.path.split() with no directory should return ['', filename]."""
    code = (
        "import os\n"
        "var r = os.path.split('file.txt')\n"
        "assert(r[0] == '')\n"
        "assert(r[1] == 'file.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_path_split_root(vm):
    """os.path.split() with root path should handle correctly."""
    code = (
        "import os\n"
        "var r = os.path.split('/file.txt')\n"
        "assert(r[1] == 'file.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.path.splitext()
# ---------------------------------------------------------------------------
def test_os_path_splitext_basic(vm):
    """os.path.splitext() should split into [stem, extension]."""
    code = (
        "import os\n"
        "var r = os.path.splitext('/home/user/file.txt')\n"
        "assert(r[0] == '/home/user/file')\n"
        "assert(r[1] == '.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_path_splitext_no_ext(vm):
    """os.path.splitext() with no extension should return [path, '']."""
    code = (
        "import os\n"
        "var r = os.path.splitext('/home/user/file')\n"
        "assert(r[0] == '/home/user/file')\n"
        "assert(r[1] == '')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# os.path.join()
# ---------------------------------------------------------------------------
def test_os_path_join_basic(vm):
    """os.path.join() should join path components with /."""
    code = (
        "import os\n"
        "assert(os.path.join('home', 'user', 'file.txt') == 'home/user/file.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_path_join_absolute(vm):
    """os.path.join() with absolute component should reset."""
    code = (
        "import os\n"
        "assert(os.path.join('home', '/usr', 'file.txt') == '/usr/file.txt')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_os_path_join_trailing_slash(vm):
    """os.path.join() should not double slashes."""
    code = (
        "import os\n"
        "assert(os.path.join('home/', 'user') == 'home/user')\n"
    )
    assert be_dostring(vm, code) == BE_OK
