# Parse tasmota/tasmota_defines_for_berry.h and emit
# tasmota/tasmota_defines_for_berry.be for Berry solidification.
#
# Runs as a POST-script, after dump-defines.py has produced the .h file
# and before gen-berry-structures.py runs solidification.
#
# Supported conversions (everything else becomes a comment):
#   #define NAME              → preproc.define('NAME')
#   #define NAME true/false   → preproc.define('NAME', true/false)
#   #define NAME "string"     → preproc.define('NAME', "string")
#   #define NAME 123          → preproc.define('NAME', 123)
#   #define NAME 0x1A         → preproc.define('NAME', 0x1A)
#   #define NAME -42          → preproc.define('NAME', -42)
#
# Ignored (kept as comments):
#   - Names starting with '__'
#   - Values that are expressions / identifiers / floats / other types

Import("env")

import pathlib
import re

from colorama import Fore

from tasmotapiolib import is_non_build_target

# Skip during the espidf libs compile phase that runs first when
# `custom_sdkconfig` is set. The Arduino-as-component / hybrid build
# triggers a recursive SConscript("espidf.py") which sets
# ARDUINO_LIB_COMPILE_FLAG="Build" - in that pass we don't want to
# regenerate Berry artifacts.
if env.subst("$ARDUINO_LIB_COMPILE_FLAG") == "Build":
    print("gen-berry-defines: skipped (ESP-IDF libs compile phase)")
    Return()

# Skip for non-compiling targets (upload, erase, monitor, ...).
if is_non_build_target(env):
    print("gen-berry-defines: skipped (non-build target)")
    Return()

# ---------------------------------------------------------------------------
# Regex patterns
# ---------------------------------------------------------------------------

# Full #define line: captures name and optional value (everything after name)
_RE_DEFINE = re.compile(r"^#define\s+(\S+)(?:\s+(.*?))?\s*$")

# Accepted value patterns
_RE_STRING  = re.compile(r'^"(?:[^"\\]|\\.)*"$')          # "..."
_RE_BOOL    = re.compile(r"^(true|false)$")                # true / false
_RE_INT     = re.compile(r"^-?\d+$")                       # decimal integer
_RE_HEX     = re.compile(r"^0[xX][0-9A-Fa-f]+$")          # hex 0x...


def _classify(name: str, value: str | None):
    """
    Returns ('emit', berry_value_str) or ('ignore', reason).
    berry_value_str is None for bare defines (no value).
    """
    # Skip compiler built-ins
    if name.startswith("__"):
        return ("ignore", f"#define {name}" + (f" {value}" if value else ""))

    # No value → bare define
    if value is None or value == "":
        return ("emit", None)

    # true / false
    if _RE_BOOL.match(value):
        return ("emit", value)

    # Quoted string
    if _RE_STRING.match(value):
        return ("emit", value)

    # Decimal integer
    if _RE_INT.match(value):
        return ("emit", value)

    # Hex integer
    if _RE_HEX.match(value):
        return ("emit", value)

    # Anything else (expressions, identifiers, floats, …) → comment
    return ("ignore", f"#define {name} {value}")


def _run():
    project_dir = pathlib.Path(env.subst("$PROJECT_DIR"))
    input_file  = project_dir / "tasmota" / "tasmota_defines_for_berry.h"
    output_file = project_dir / "tasmota" / "tasmota_defines_for_berry.be"

    if not input_file.exists():
        print(
            Fore.YELLOW
            + f"gen-berry-defines: {input_file.relative_to(project_dir)} not found, skipping"
        )
        return

    n_emitted = 0
    n_ignored = 0

    with open(input_file, encoding="utf-8") as fh_in, \
         open(output_file, "w", encoding="utf-8") as fh_out:

        fh_out.write("#  Generated code from tasmota_defines_for_berry.h, don't edit\n")
        fh_out.write("import preproc\n")
        fh_out.write("\n")

        for raw in fh_in:
            line = raw.rstrip("\r\n")
            m = _RE_DEFINE.match(line)
            if not m:
                # Not a #define line at all — skip silently (blank lines, etc.)
                continue

            name  = m.group(1)
            value = m.group(2)  # may be None

            action, payload = _classify(name, value)

            if action == "ignore":
                fh_out.write(f"    # ignored {payload}\n")
                n_ignored += 1
            else:
                # payload is the Berry value string, or None for bare define
                if payload is None:
                    fh_out.write(f"preproc.define('{name}')\n")
                else:
                    fh_out.write(f"preproc.define('{name}', {payload})\n")
                n_emitted += 1

        fh_out.write("\n")
        fh_out.write(f"# {n_emitted} defines emitted, {n_ignored} ignored\n")

    print(
        f"Berry converting defines: {output_file.relative_to(project_dir)}"
        f" ({n_emitted} defines, {n_ignored} ignored)"
    )

_run()
