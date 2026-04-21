Import("env")

import os
import glob
import subprocess
from os.path import join, isfile

# generate all precompiled Berry structures from multiple modules
CURRENT_DIR = os.getcwd()
PROJECT_DIR = env.subst("$PROJECT_DIR")
BERRY_GEN_DIR = join(PROJECT_DIR, "lib", "libesp32", "berry")
BERRY_PORT_DIR = BERRY_GEN_DIR  # PYTHONPATH root for `berry_port` module

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

    for solidify_dir in SOLIDIFY_DIRS:
        script = join(solidify_dir, "solidify_all_python.be")
        if not isfile(script):
            continue
        rel_script = os.path.relpath(script, PROJECT_DIR)
        print(f"Solidifying: {rel_script}")
        os.chdir(solidify_dir)
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
cmd = (env["PYTHONEXE"],join("tools","coc","coc"),"-o","generate","src","default",join("..","berry_tasmota","src"),join("..","berry_matter","src","solidify"),join("..","berry_matter","src"),join("..","berry_custom","src","solidify"),join("..","berry_custom","src"),join("..","berry_animation","src","solidify"),join("..","berry_animation","src"),join("..","berry_tasmota","src","solidify"),join("..","berry_mapping","src"),join("..","berry_int64","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src","solidify"),join("..","..","libesp32_lvgl","lv_binding_berry","generate"),join("..","..","libesp32_lvgl","lv_haspmota","src","solidify"),"-c",join("default","berry_conf.h"))
returncode = subprocess.call(cmd, shell=False)
os.chdir(CURRENT_DIR)
