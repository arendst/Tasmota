"""
Test: module init() return value substitution.

When importing a module that has an 'init' member function, the VM calls
init(module) and the return value should replace the original module on
the stack.  A bug in _module_init caused the original module to remain
instead of being substituted by the init() return value.

Regression test for the _module_init stack manipulation fix.
"""

import sys
import os
import io
import tempfile
import shutil

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_dostring


def _run_python(source, extra_files=None):
    """Run Berry source on the Python port, return (rc, stdout_str).

    extra_files: dict of {filename: content} to write into a temp dir
    that is used as cwd so 'import' can find them.
    """
    tmpdir = tempfile.mkdtemp()
    try:
        if extra_files:
            for name, content in extra_files.items():
                with open(os.path.join(tmpdir, name), 'w') as f:
                    f.write(content)

        old_cwd = os.getcwd()
        os.chdir(tmpdir)
        try:
            vm = be_vm_new()
            old_stdout = sys.stdout
            sys.stdout = captured = io.StringIO()
            try:
                rc = be_dostring(vm, source)
            except Exception:
                rc = -1
            finally:
                sys.stdout = old_stdout
        finally:
            os.chdir(old_cwd)
    finally:
        shutil.rmtree(tmpdir)
    return rc, captured.getvalue()


# ============================================================================
# Module init() substitution tests
# ============================================================================

_TESTM_SOURCE = (
    "var m1 = module('m1')\n"
    "m1.a = 'a'\n"
    "m1.init = def(m)\n"
    "  var m2 = module('m2')\n"
    "  m2.m1 = m\n"
    "  m2.b = 'b'\n"
    "  return m2\n"
    "end\n"
    "return m1\n"
)

_VECTORS = [
    (
        "init returns substitute module",
        {"testm.be": _TESTM_SOURCE},
        "import testm\nprint(testm)",
        "<module: m2>\n",
    ),
    (
        "substitute module has new attributes",
        {"testm.be": _TESTM_SOURCE},
        "import testm\nprint(testm.b)",
        "b\n",
    ),
    (
        "substitute module retains reference to original",
        {"testm.be": _TESTM_SOURCE},
        "import testm\nprint(testm.m1)",
        "<module: m1>\n",
    ),
    (
        "original module attributes accessible via reference",
        {"testm.be": _TESTM_SOURCE},
        "import testm\nprint(testm.m1.a)",
        "a\n",
    ),
    (
        "module without init stays unchanged",
        {
            "noinit.be": (
                "var m = module('plain')\n"
                "m.x = 123\n"
                "return m\n"
            ),
        },
        "import noinit\nprint(noinit)\nprint(noinit.x)",
        "<module: plain>\n123\n",
    ),
]


@pytest.mark.parametrize("desc,files,source,expected", _VECTORS,
                         ids=[v[0] for v in _VECTORS])
def test_module_init_substitution(desc, files, source, expected):
    """Importing a module with init() must substitute the return value."""
    rc, output = _run_python(source, extra_files=files)
    assert rc == 0, f"[{desc}] be_dostring returned rc={rc}"
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )
