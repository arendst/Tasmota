#!/usr/bin/env python3
"""
Compare the output of native C Berry solidification against the new
Python-port solidification.

For each Berry module under lib/libesp32{,_lvgl}/<module>/ that has both a
solidify_all.be (C) and a solidify_all_python.be (Python) script:

  1. Run the native C berry binary against solidify_all.be, capturing every
     generated .h file under src/solidify/ into a per-module temp dir.
  2. Run "python3 -m berry_port -s -g solidify_all_python.be", capturing
     every generated .h file under src/solidify/ into another temp dir.
  3. Diff the two trees and report any difference (missing / extra / changed
     files) per module.

The original src/solidify/ contents are preserved: we snapshot, restore, and
clean up between runs.

Usage:
    python3 pio-tools/compare-berry-solidify.py            # all modules
    python3 pio-tools/compare-berry-solidify.py -m <name>  # single module
    python3 pio-tools/compare-berry-solidify.py --keep     # keep temp dirs

Exit code: 0 if every module is identical, 1 if any difference is found.
"""

from __future__ import annotations

import argparse
import difflib
import filecmp
import os
import shutil
import subprocess
import sys
import tempfile
from os.path import join, isfile, isdir, basename, dirname, abspath


# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
SCRIPT_DIR = dirname(abspath(__file__))
PROJECT_DIR = abspath(join(SCRIPT_DIR, ".."))
BERRY_ROOT = join(PROJECT_DIR, "lib", "libesp32", "berry")
BERRY_PORT_DIR = BERRY_ROOT  # parent of berry_port package

# tasmota_defines_for_berry.be is imported only by solidify_all_python.be.
# The native C berry never sees those defines, so for a fair comparison we
# replace the file with a minimal stub during the Python run.
DEFINES_FILE = join(PROJECT_DIR, "tasmota", "tasmota_defines_for_berry.be")
DEFINES_STUB = (
    "# Empty stub used by compare-berry-solidify.py to neutralize\n"
    "# tasmota_defines_for_berry.be while diffing native vs. python output.\n"
    "import preproc\n"
    "preproc.clear()\n"
)

# Module dirs to compare (path is relative to PROJECT_DIR)
MODULES = [
    join("lib", "libesp32", "berry_tasmota"),
    join("lib", "libesp32", "berry_matter"),
    join("lib", "libesp32", "berry_animation"),
    join("lib", "libesp32", "berry_custom"),
    join("lib", "libesp32_lvgl", "lv_binding_berry"),
    join("lib", "libesp32_lvgl", "lv_haspmota"),
]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _native_berry_binary() -> str:
    """Return the native Berry binary appropriate for the host."""
    if sys.platform == "win32":
        bin_path = join(BERRY_ROOT, "berry.exe")
    else:
        bin_path = join(BERRY_ROOT, "berry")
    if not isfile(bin_path):
        sys.exit(
            f"error: native Berry binary not found at {bin_path}\n"
            f"       build it first with `make -C {BERRY_ROOT}`"
        )
    if not os.access(bin_path, os.X_OK):
        sys.exit(f"error: {bin_path} is not executable")
    return bin_path


def _snapshot_dir(src: str) -> str | None:
    """Move the contents of src to a fresh temp dir, return the temp dir.
    Returns None if src does not exist."""
    if not isdir(src):
        return None
    tmp = tempfile.mkdtemp(prefix="solidify_backup_")
    for fn in os.listdir(src):
        shutil.move(join(src, fn), join(tmp, fn))
    return tmp


def _restore_dir(src: str, backup: str | None) -> None:
    """Restore a previously snapshotted directory (and remove the backup)."""
    if backup is None:
        return
    # clear src first
    if isdir(src):
        for fn in os.listdir(src):
            p = join(src, fn)
            if os.path.isdir(p):
                shutil.rmtree(p)
            else:
                os.remove(p)
    else:
        os.makedirs(src, exist_ok=True)
    for fn in os.listdir(backup):
        shutil.move(join(backup, fn), join(src, fn))
    shutil.rmtree(backup, ignore_errors=True)


def _clear_dir(d: str) -> None:
    """Empty (but keep) a directory."""
    if not isdir(d):
        os.makedirs(d, exist_ok=True)
        return
    for fn in os.listdir(d):
        p = join(d, fn)
        if os.path.isdir(p):
            shutil.rmtree(p)
        else:
            os.remove(p)


def _copy_dir(src: str, dst: str) -> None:
    """Recursively copy src -> dst (dst must not exist)."""
    shutil.copytree(src, dst)


class _StubDefines:
    """Context manager: replace tasmota_defines_for_berry.be with an empty
    stub for the duration of the with-block, restoring the original on exit
    (even on exception)."""

    def __init__(self, target: str, stub_content: str):
        self.target = target
        self.stub_content = stub_content
        self.backup = None  # type: str | None

    def __enter__(self):
        if isfile(self.target):
            fd, self.backup = tempfile.mkstemp(prefix="defines_backup_",
                                               suffix=".be")
            os.close(fd)
            shutil.copy2(self.target, self.backup)
        else:
            self.backup = None
        os.makedirs(dirname(self.target), exist_ok=True)
        with open(self.target, "w", encoding="utf-8") as f:
            f.write(self.stub_content)
        return self

    def __exit__(self, exc_type, exc, tb):
        try:
            if self.backup is not None:
                shutil.copy2(self.backup, self.target)
                os.remove(self.backup)
            else:
                # original did not exist – remove stub we created
                if isfile(self.target):
                    os.remove(self.target)
        except OSError as e:
            print(f"WARN: could not restore {self.target}: {e}",
                  file=sys.stderr)
        return False  # never swallow exceptions


def _filter_chatter(line: str) -> bool:
    """Return True if the line is routine chatter that should be silenced."""
    s = line.strip()
    if s.startswith("Parsing:"):
        return True
    if s.startswith("Skipping:"):
        return True
    if s.startswith("# Output directory"):
        return True
    return False


def _run(cmd: list[str], cwd: str, env: dict[str, str], label: str) -> int:
    """Run a subprocess, streaming filtered output. Returns exit code."""
    print(f"  [{label}] $ {' '.join(cmd)}")
    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    other = []
    assert proc.stdout is not None
    for raw in proc.stdout:
        line = raw.rstrip("\r\n")
        if _filter_chatter(line):
            continue
        other.append(line)
    rc = proc.wait()
    for line in other:
        if line:
            print(f"  [{label}] {line}")
    return rc


# ---------------------------------------------------------------------------
# Solidification runners
# ---------------------------------------------------------------------------
def run_native(module_dir: str, out_dir: str, native_bin: str) -> bool:
    """Run the native C berry against solidify_all.be. Return True on success."""
    script = join(module_dir, "solidify_all.be")
    if not isfile(script):
        print(f"  [native] no solidify_all.be in {module_dir} – skipping")
        return False

    solidify_dir = join(module_dir, "src", "solidify")
    os.makedirs(solidify_dir, exist_ok=True)

    env = os.environ.copy()
    env["PYTHONUTF8"] = "1"  # harmless for the C binary
    rc = _run([native_bin, "-s", "-g", "solidify_all.be"],
              cwd=module_dir, env=env, label="native")
    if rc != 0:
        print(f"  [native] FAILED (rc={rc})")
        return False

    # snapshot output
    if isdir(out_dir):
        shutil.rmtree(out_dir)
    _copy_dir(solidify_dir, out_dir)
    return True


def run_python(module_dir: str, out_dir: str) -> bool:
    """Run the Python port against solidify_all_python.be. Return True on success."""
    script = join(module_dir, "solidify_all_python.be")
    if not isfile(script):
        print(f"  [python] no solidify_all_python.be in {module_dir} – skipping")
        return False

    solidify_dir = join(module_dir, "src", "solidify")
    os.makedirs(solidify_dir, exist_ok=True)

    env = os.environ.copy()
    existing_pp = env.get("PYTHONPATH", "")
    env["PYTHONPATH"] = (
        BERRY_PORT_DIR + (os.pathsep + existing_pp if existing_pp else "")
    )
    env["PYTHONUTF8"] = "1"

    rc = _run([sys.executable, "-m", "berry_port", "-s", "-g",
               "solidify_all_python.be"],
              cwd=module_dir, env=env, label="python")
    if rc != 0:
        print(f"  [python] FAILED (rc={rc})")
        return False

    if isdir(out_dir):
        shutil.rmtree(out_dir)
    _copy_dir(solidify_dir, out_dir)
    return True


# ---------------------------------------------------------------------------
# Diff logic
# ---------------------------------------------------------------------------
def _list_h(d: str) -> set[str]:
    if not isdir(d):
        return set()
    return {fn for fn in os.listdir(d) if fn.endswith(".h")}


def diff_trees(native_dir: str, python_dir: str, max_diff_lines: int = 60) -> bool:
    """Compare two trees of .h files. Returns True if identical."""
    a = _list_h(native_dir)
    b = _list_h(python_dir)

    only_native = sorted(a - b)
    only_python = sorted(b - a)
    common = sorted(a & b)

    differing = []
    for fn in common:
        if not filecmp.cmp(join(native_dir, fn), join(python_dir, fn),
                           shallow=False):
            differing.append(fn)

    identical = (not only_native and not only_python and not differing)

    print(f"    files: native={len(a)}, python={len(b)}, common={len(common)}, "
          f"differing={len(differing)}")
    if only_native:
        print(f"    only in native ({len(only_native)}): "
              f"{', '.join(only_native[:10])}"
              + (" ..." if len(only_native) > 10 else ""))
    if only_python:
        print(f"    only in python ({len(only_python)}): "
              f"{', '.join(only_python[:10])}"
              + (" ..." if len(only_python) > 10 else ""))

    for fn in differing:
        print(f"    DIFF: {fn}")
        with open(join(native_dir, fn), "r", errors="replace") as f:
            a_lines = f.readlines()
        with open(join(python_dir, fn), "r", errors="replace") as f:
            b_lines = f.readlines()
        diff = list(difflib.unified_diff(
            a_lines, b_lines,
            fromfile=f"native/{fn}",
            tofile=f"python/{fn}",
            n=2,
        ))
        if len(diff) > max_diff_lines:
            for line in diff[:max_diff_lines]:
                sys.stdout.write("      " + line)
            print(f"      ... ({len(diff) - max_diff_lines} more lines)")
        else:
            for line in diff:
                sys.stdout.write("      " + line)

    return identical


# ---------------------------------------------------------------------------
# Per-module driver
# ---------------------------------------------------------------------------
def compare_module(rel_module: str, work_root: str, native_bin: str) -> bool:
    """Run both solidifiers for one module and diff results. Returns True
    if the trees are identical."""
    module_dir = join(PROJECT_DIR, rel_module)
    solidify_dir = join(module_dir, "src", "solidify")
    print(f"\n=== {rel_module} ===")

    native_out = join(work_root, basename(rel_module) + "__native")
    python_out = join(work_root, basename(rel_module) + "__python")

    backup = _snapshot_dir(solidify_dir)
    try:
        # ensure a clean output dir for the native run
        _clear_dir(solidify_dir)
        ok_n = run_native(module_dir, native_out, native_bin)
        # ensure a clean output dir for the python run
        _clear_dir(solidify_dir)
        # Neutralize tasmota_defines_for_berry.be so the Python run sees no
        # preproc defines (matching what the native C berry sees, since it
        # doesn't import that file at all).
        with _StubDefines(DEFINES_FILE, DEFINES_STUB):
            ok_p = run_python(module_dir, python_out)
    finally:
        _restore_dir(solidify_dir, backup)

    if not ok_n or not ok_p:
        return False

    return diff_trees(native_out, python_out)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    p = argparse.ArgumentParser(
        description="Compare native C and Python-port Berry solidification.")
    p.add_argument("-m", "--module", action="append",
                   help="restrict to a module (basename, may be repeated). "
                        "Default: all known modules.")
    p.add_argument("--keep", action="store_true",
                   help="keep the temp directory with both output trees.")
    args = p.parse_args()

    selected = MODULES
    if args.module:
        wanted = set(args.module)
        selected = [m for m in MODULES if basename(m) in wanted]
        if not selected:
            sys.exit(f"error: none of {sorted(wanted)} matches known modules: "
                     f"{[basename(m) for m in MODULES]}")

    native_bin = _native_berry_binary()
    print(f"Native berry: {native_bin}")
    print(f"Python:       {sys.executable}")

    work_root = tempfile.mkdtemp(prefix="solidify_compare_")
    print(f"Workdir:      {work_root}")

    overall_ok = True
    summary = []
    for rel in selected:
        ok = compare_module(rel, work_root, native_bin)
        summary.append((rel, ok))
        if not ok:
            overall_ok = False

    print("\n=== Summary ===")
    for rel, ok in summary:
        print(f"  {'OK    ' if ok else 'DIFFER'}  {rel}")

    if args.keep:
        print(f"\nKept output trees in: {work_root}")
    else:
        shutil.rmtree(work_root, ignore_errors=True)

    sys.exit(0 if overall_ok else 1)


if __name__ == "__main__":
    main()
