# Dump preprocessor `#define` macros that matter for Berry solidification:
#   - USE_* feature flags (from my_user_config.h, user_config_override.h,
#                           tasmota_options.h, tasmota_configurations.h)
#   - Language strings D_JSON_*, D_CMND_*, ... (from i18n.h + the language
#                                                file selected via MY_LANGUAGE)
#   - Tasmota enums (from tasmota.h)
#
# Output: tasmota/tasmota_defines_for_berry.h (gitignored)
#
# Runs as a POST-script so $CXX is already set to the real cross-compiler
# (xtensa-esp32-elf-g++, riscv32-esp-elf-g++, etc.) by the time this
# script executes. This guarantees correct int/pointer widths and
# target-specific built-in macros without any -m32 hack.
#
# We pass `-D` flags from the project's build_flags so that USE_* gates,
# MY_LANGUAGE, ESP32, etc. are defined the same way the real build sees
# them.

Import("env")

import pathlib
import re
import subprocess

from colorama import Fore

from tasmotapiolib import is_non_build_target

# Skip during the espidf libs compile phase that runs first when
# `custom_sdkconfig` is set. The Arduino-as-component / hybrid build
# triggers a recursive SConscript("espidf.py") which sets
# ARDUINO_LIB_COMPILE_FLAG="Build" - in that pass we don't want any
# Berry action.
if env.subst("$ARDUINO_LIB_COMPILE_FLAG") == "Build":
    print("berry-dump-defines: skipped (ESP-IDF libs compile phase)")
    Return()

# Skip for non-compiling targets (upload, erase, monitor, ...).
if is_non_build_target(env):
    print("berry-dump-defines: skipped (non-build target)")
    Return()

_IS_ESP32 = env["PIOPLATFORM"] == "espressif32"


def _run_dump():
    project_dir = pathlib.Path(env.subst("$PROJECT_DIR"))

    output_file = project_dir / "tasmota" / "tasmota_defines_for_berry.h"
    output_file.parent.mkdir(parents=True, exist_ok=True)

    # Working dir for the sdkconfig.h stub.
    stub_parent = pathlib.Path(env.subst("$BUILD_DIR"))
    stub_parent.mkdir(parents=True, exist_ok=True)

    cxx = env.subst("$CXX")
    if not cxx:
        print(Fore.YELLOW + "dump-defines: $CXX not set, skipping")
        return

    forced_includes = [
        project_dir / "tasmota" / "include" / "tasmota.h",
        project_dir / "tasmota" / "my_user_config.h",
        project_dir / "include" / "tasmota_options.h",
        project_dir / "tasmota" / "include" / "i18n.h",
    ]

    # Extract -D flags from build_flags. Other flags (-I, -W, -f, ...) we
    # don't need: our forced includes are absolute paths and we supply
    # explicit -I for the Tasmota header roots below.
    build_flags = env.subst(" ".join(env.get("BUILD_FLAGS") or []))
    define_flags = re.findall(r"-D\s*\S+", build_flags)

    # Also surface ESP32 unconditionally (real build always defines it via
    # the platform) so #ifdef ESP32 in tasmota_configurations.h fires.
    if not any(f.startswith("-DESP32") for f in define_flags):
        define_flags.append("-DESP32")

    include_flags = [
        "-I" + str(project_dir / "include"),
        "-I" + str(project_dir / "tasmota" / "include"),
        "-I" + str(project_dir / "tasmota"),
    ]

    # tasmota_configurations_ESP32.h and the ESP-IDF headers pull in
    # `sdkconfig.h`, which is generated per MCU variant by the framework
    # and not present at pre-script time. For Berry solidification we
    # don't need its CONFIG_* values - the macros the solidifier cares
    # about live in Tasmota headers. Supply an empty stub so the
    # preprocessor stops complaining.
    stub_dir = stub_parent / "dump-defines-stubs"
    stub_dir.mkdir(parents=True, exist_ok=True)
    (stub_dir / "sdkconfig.h").write_text(
        "/* auto-generated empty stub for dump-defines.py */\n"
    )
    # Prepend so the stub shadows any real header on CPPPATH.
    include_flags.insert(0, "-I" + str(stub_dir))

    cmd = [cxx, "-E", "-dM", "-x", "c++"]
    cmd += define_flags
    cmd += include_flags
    for header in forced_includes:
        cmd += ["-include", str(header)]
    cmd.append("-")  # read empty stdin

    print(f"Berry dumping defines: {output_file.relative_to(project_dir)}")

    try:
        with open(output_file, "w", encoding="utf-8") as out:
            proc = subprocess.run(
                cmd,
                input="",
                stdout=out,
                stderr=subprocess.PIPE,
                text=True,
                check=False,
            )
    except FileNotFoundError as e:
        print(Fore.YELLOW + f"dump-defines: {cxx} not runnable ({e}), skipping")
        return

    if proc.returncode != 0:
        print(Fore.YELLOW + f"dump-defines: {cxx} returned {proc.returncode}")
        if proc.stderr:
            for line in proc.stderr.splitlines()[:10]:
                print(Fore.YELLOW + f"  {line}")


def _action(*args, **kwargs):
    _run_dump()


if _IS_ESP32:
    _run_dump()

    env.AddCustomTarget(
        name="dump_defines",
        dependencies=None,
        actions=[_action],
        title="Dump preprocessor defines",
        description="Run the preprocessor over Tasmota config/i18n headers and write macros to tasmota/tasmota_defines_for_berry.h",
    )
