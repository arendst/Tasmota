Import("env")

import os
import glob
import hashlib
import json
import subprocess
from os.path import join, isfile

from tasmotapiolib import is_non_build_target

# Skip during the espidf libs compile phase that runs first when
# `custom_sdkconfig` is set. The Arduino-as-component / hybrid build
# triggers a recursive SConscript("espidf.py") which sets
# ARDUINO_LIB_COMPILE_FLAG="Build" - in that pass we don't want to
# regenerate Berry artifacts.
if env.subst("$ARDUINO_LIB_COMPILE_FLAG") == "Build":
    print("gen-berry-structures: skipped (ESP-IDF libs compile phase)")
    Return()

# Skip for non-compiling targets (upload, erase, monitor, ...).
if is_non_build_target(env):
    print("gen-berry-structures: skipped (non-build target)")
    Return()

# generate all precompiled Berry structures from multiple modules
CURRENT_DIR = os.getcwd()
PROJECT_DIR = env.subst("$PROJECT_DIR")
BERRY_GEN_DIR = join(PROJECT_DIR, "lib", "libesp32", "berry")
BERRY_PORT_DIR = BERRY_GEN_DIR  # PYTHONPATH root for `berry_port` module

# ---------------------------------------------------------------------------
# Solidification cache helpers
#
# Two-tier validation inspired by ldf_cache.py:
#   Fast path  – compare st_mtime_ns of every input file against the cached
#                values.  If all match AND every expected output .h still
#                exists → skip solidification for this module.
#   Slow path  – if mtimes differ (e.g. after `git checkout`), fall back to
#                MD5 content hashes.  If hashes match → skip, and update the
#                cached mtimes so the next build takes the fast path again.
#
# One JSON cache file per module, stored under
# .pio/build/<env>/berry_solidify_cache/.
# ---------------------------------------------------------------------------

# Cache lives under the per-environment build directory so each variant keeps
# its own cache (and it survives `pio run -t clean` of other envs).
BUILD_DIR = env.subst("$BUILD_DIR")
SOLIDIFY_CACHE_DIR = join(BUILD_DIR, "berry_solidify_cache")
# Shared dependency imported by every solidify_all_python.be script
SHARED_DEFINES_FILE = join(PROJECT_DIR, "tasmota", "tasmota_defines_for_berry.be")


def _collect_be_files(root_dir):
    """Recursively collect all .be files under *root_dir*, sorted."""
    be_files = []
    for dirpath, dirnames, filenames in os.walk(root_dir):
        # skip hidden dirs and the solidify output directory
        dirnames[:] = [
            d for d in dirnames
            if not d.startswith(".") and d != "solidify"
        ]
        for fn in filenames:
            if fn.endswith(".be"):
                be_files.append(join(dirpath, fn))
    be_files.sort()
    return be_files


def _collect_berry_port_py_files():
    """Collect all .py files in the berry_port module (sorted)."""
    bp_dir = join(BERRY_PORT_DIR, "berry_port")
    py_files = []
    for fn in os.listdir(bp_dir):
        if fn.endswith(".py"):
            py_files.append(join(bp_dir, fn))
    py_files.sort()
    return py_files


def _collect_solidify_outputs(solidify_dir):
    """Collect all .h files in the module's src/solidify/ directory."""
    # Most modules use src/solidify/; detect it relative to the module root.
    out_dir = join(solidify_dir, "src", "solidify")
    if not os.path.isdir(out_dir):
        return []
    return sorted(
        join(out_dir, fn)
        for fn in os.listdir(out_dir)
        if fn.endswith(".h")
    )


def _file_mtime_ns(path):
    """Return st_mtime_ns, or None if the file doesn't exist."""
    try:
        return os.stat(path).st_mtime_ns
    except OSError:
        return None


def _file_md5(path):
    """Return hex MD5 of file contents, or None on error."""
    try:
        h = hashlib.md5()
        with open(path, "rb") as f:
            for chunk in iter(lambda: f.read(65536), b""):
                h.update(chunk)
        return h.hexdigest()
    except OSError:
        return None


def _cache_path_for(solidify_dir):
    """Return the JSON cache file path for a given module directory."""
    module_name = os.path.basename(solidify_dir)
    return join(SOLIDIFY_CACHE_DIR, module_name + ".json")


def _load_cache(solidify_dir):
    """Load and return the cache dict, or None."""
    cp = _cache_path_for(solidify_dir)
    if not isfile(cp):
        return None
    try:
        with open(cp, "r") as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError):
        return None


def _save_cache(solidify_dir, input_files, berry_port_files):
    """Build and persist the cache for a module after successful solidification."""
    os.makedirs(SOLIDIFY_CACHE_DIR, exist_ok=True)
    all_inputs = list(input_files) + list(berry_port_files)
    if isfile(SHARED_DEFINES_FILE):
        all_inputs.append(SHARED_DEFINES_FILE)

    mtimes = {}
    hashes = {}
    for fp in all_inputs:
        key = os.path.relpath(fp, PROJECT_DIR)
        mtimes[key] = _file_mtime_ns(fp)
        hashes[key] = _file_md5(fp)

    outputs = [
        os.path.relpath(p, PROJECT_DIR)
        for p in _collect_solidify_outputs(solidify_dir)
    ]

    cache = {
        "version": 1,
        "input_mtimes": mtimes,
        "input_hashes": hashes,
        "output_files": outputs,
    }
    cp = _cache_path_for(solidify_dir)
    with open(cp, "w") as f:
        json.dump(cache, f, indent=2)


def _is_solidify_cached(solidify_dir, berry_port_files):
    """Return True if the module can be skipped (sources unchanged, outputs present)."""
    cache = _load_cache(solidify_dir)
    if cache is None or cache.get("version") != 1:
        return False

    cached_mtimes = cache.get("input_mtimes", {})
    cached_hashes = cache.get("input_hashes", {})
    cached_outputs = cache.get("output_files", [])

    # --- check that all expected output files still exist ---
    for rel in cached_outputs:
        if not isfile(join(PROJECT_DIR, rel)):
            return False
    if not cached_outputs:
        # no recorded outputs → must run
        return False

    # --- collect current input files ---
    input_files = _collect_be_files(solidify_dir)
    if isfile(SHARED_DEFINES_FILE):
        input_files.append(SHARED_DEFINES_FILE)
    input_files.extend(berry_port_files)

    current_keys = {os.path.relpath(fp, PROJECT_DIR) for fp in input_files}
    cached_keys = set(cached_mtimes.keys())

    # files added or removed → invalidate
    if current_keys != cached_keys:
        return False

    # --- fast path: mtime comparison ---
    all_mtimes_match = True
    for fp in input_files:
        key = os.path.relpath(fp, PROJECT_DIR)
        if _file_mtime_ns(fp) != cached_mtimes.get(key):
            all_mtimes_match = False
            break

    if all_mtimes_match:
        return True

    # --- slow path: content hash comparison ---
    for fp in input_files:
        key = os.path.relpath(fp, PROJECT_DIR)
        if _file_md5(fp) != cached_hashes.get(key):
            return False

    # Content unchanged despite mtime change (e.g. git checkout).
    # Update cached mtimes so the next build takes the fast path.
    for fp in input_files:
        key = os.path.relpath(fp, PROJECT_DIR)
        cached_mtimes[key] = _file_mtime_ns(fp)
    cache["input_mtimes"] = cached_mtimes
    try:
        cp = _cache_path_for(solidify_dir)
        with open(cp, "w") as f:
            json.dump(cache, f, indent=2)
    except OSError:
        pass  # non-fatal, next build will just take the slow path again

    return True


# ---------------------------------------------------------------------------
# Step 1 - run all `solidify_all_python.be` scripts in pure Python
#
# Each script has a shebang of the form:
#   #!/usr/bin/env -S PYTHONPATH=<relative>/berry python3 -m berry_port -s -g
# We replicate this here by invoking `python3 -m berry_port -s -g <script>`
# from the script's own directory, with PYTHONPATH pointing to
# lib/libesp32/berry (so that the `berry_port` module is importable).
#
# This step can be skipped by adding `-DDISABLE_BERRY_SOLIDIFY` to
# `build_flags` (useful when iterating on C/C++ code without touching Berry
# sources, or when `python3` is unavailable in the build environment).
# ---------------------------------------------------------------------------

build_flags = " ".join(env['BUILD_FLAGS'])
if "DISABLE_BERRY_SOLIDIFY" in build_flags:
    print("Berry solidification disabled via -DDISABLE_BERRY_SOLIDIFY")
else:
    SOLIDIFY_DIRS = [
        join(PROJECT_DIR, "lib", "libesp32", "berry_tasmota"),
        join(PROJECT_DIR, "lib", "libesp32", "berry_matter"),
        join(PROJECT_DIR, "lib", "libesp32", "berry_animation"),
        join(PROJECT_DIR, "lib", "libesp32", "berry_custom"),
        join(PROJECT_DIR, "lib", "libesp32_lvgl", "lv_binding_berry"),
        join(PROJECT_DIR, "lib", "libesp32_lvgl", "lv_haspmota"),
    ]

    solidify_env = os.environ.copy()
    # Prepend our berry_port path so it wins over any pre-existing PYTHONPATH
    existing_pp = solidify_env.get("PYTHONPATH", "")
    solidify_env["PYTHONPATH"] = (
        BERRY_PORT_DIR + (os.pathsep + existing_pp if existing_pp else "")
    )
    # Force UTF-8 mode (PEP 540) so Python uses UTF-8 for open()/stdio
    # regardless of the host locale. This is required on Windows where the
    # default code page (e.g. cp1252) otherwise fails to read some Berry
    # source files (notably in lib/libesp32/berry_animation).
    solidify_env["PYTHONUTF8"] = "1"

    # Collect berry_port .py files once (shared across all modules)
    berry_port_files = _collect_berry_port_py_files()

    for solidify_dir in SOLIDIFY_DIRS:
        script = join(solidify_dir, "solidify_all_python.be")
        if not isfile(script):
            continue
        rel_script = os.path.relpath(script, PROJECT_DIR)

        # --- cache check: skip if sources unchanged ---
        if _is_solidify_cached(solidify_dir, berry_port_files):
            print(f"Berry solidification: {rel_script} (cached, skipped)")
            continue

        print(f"Berry solidification: {rel_script}")
        os.chdir(solidify_dir)
        # Ensure the output directory exists – the .be script calls
        # os.listdir('src/solidify/') which would otherwise fail on a
        # fresh checkout.
        os.makedirs(join("src", "solidify"), exist_ok=True)
        solidify_cmd = (
            env["PYTHONEXE"],
            "-m", "berry_port",
            "-s", "-g",
            "solidify_all_python.be",
        )
        proc = subprocess.Popen(
            solidify_cmd,
            shell=False,
            env=solidify_env,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
        )
        other_lines = []
        for raw in proc.stdout:
            line = raw.rstrip("\r\n")
            stripped = line.strip()
            # Silence routine per-file chatter; keep anything that looks
            # like an error, warning, or traceback.
            if stripped.startswith("Parsing:"):
                continue
            if stripped.startswith("Skipping:"):
                continue
            if stripped.startswith("# Output directory"):
                continue
            other_lines.append(line)
        rc = proc.wait()
        # Surface anything the script emitted that wasn't routine chatter
        # - warnings, errors, tracebacks, etc.
        for line in other_lines:
            if line:
                print(line)
        if rc != 0:
            print(f"ERROR: solidification failed for {rel_script} (rc={rc})")
            os.chdir(CURRENT_DIR)
            env.Exit(rc)

        # --- update cache after successful solidification ---
        input_files = _collect_be_files(solidify_dir)
        _save_cache(solidify_dir, input_files, berry_port_files)

    os.chdir(CURRENT_DIR)

# ---------------------------------------------------------------------------
# Step 2 - run `coc` to generate the precompiled Berry structures
# ---------------------------------------------------------------------------

os.chdir(BERRY_GEN_DIR)
fileList = glob.glob(join(BERRY_GEN_DIR, "generate", "*"))
for filePath in fileList:
    try:
        os.remove(filePath)
        # print("Deleting file : ", filePath)
    except:
        print("Error while deleting file : ", filePath)

print(f"Berry coc compiler")
cmd = (env["PYTHONEXE"],join("tools","coc","coc"),"-o","generate","src","default",join("..","berry_tasmota","src"),join("..","berry_matter","src","solidify"),join("..","berry_matter","src"),join("..","berry_custom","src","solidify"),join("..","berry_custom","src"),join("..","berry_animation","src","solidify"),join("..","berry_animation","src"),join("..","berry_tasmota","src","solidify"),join("..","berry_mapping","src"),join("..","berry_int64","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src","solidify"),join("..","..","libesp32_lvgl","lv_binding_berry","generate"),join("..","..","libesp32_lvgl","lv_haspmota","src","solidify"),"-c",join("default","berry_conf.h"),join("..","..","..","tasmota","tasmota_defines_for_berry.h"))
returncode = subprocess.call(cmd, shell=False)
os.chdir(CURRENT_DIR)
