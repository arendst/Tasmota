#!/usr/bin/env sh
# decode-esp8266-stack.sh — decode ESP8266 crash stack dump using xtensa-lx106-elf-addr2line
#
# Usage:
#   ./decode-esp8266-stack.sh crash.log
#   cat crash.log | ./decode-esp8266-stack.sh
#
# The firmware ELF is expected at:
#   <repo-root>/.pio/build/<ELF_TARGET>/firmware.elf
#
# Override the build target via environment variable:
#   ELF_TARGET=tasmota-minimal ./decode-esp8266-stack.sh crash.log
#
# NOTE: The ELF must contain debug info. Build with the debug_build_flags
# option in platformio.ini (or a build_flags = -g override in your env):
#
#   [env:tasmota-4M]
#   build_flags = ${common.build_flags} -g
#
# Without -g, addr2line will resolve addresses but cannot report source
# file names and line numbers.

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

: "${ELF_TARGET:=tasmota-4M}"

ADDR2LINE="$(find ~/.platformio/packages -name "xtensa-lx106-elf-addr2line" 2>/dev/null | head -1)"
if [ -z "$ADDR2LINE" ]; then
    ADDR2LINE="$SCRIPT_DIR/toolchain-bin/xtensa-lx106-elf-addr2line"
fi
if [ ! -x "$ADDR2LINE" ]; then
    echo "ERROR: addr2line not found or not executable: $ADDR2LINE" >&2
    exit 1
fi

CXXFILT="$(find ~/.platformio/packages -name "xtensa-lx106-elf-c++filt" 2>/dev/null | head -1)"
if [ -z "$CXXFILT" ]; then
    CXXFILT="$SCRIPT_DIR/toolchain-bin/xtensa-lx106-elf-c++filt"
fi
if [ ! -x "$CXXFILT" ]; then
    echo "ERROR: c++filt not found or not executable: $CXXFILT" >&2
    exit 1
fi

ELF="$SCRIPT_DIR/../.pio/build/${ELF_TARGET}/firmware.elf"

if [ ! -f "$ELF" ]; then
    echo "ERROR: firmware ELF not found: $ELF" >&2
    echo "       Set ELF_TARGET to the correct PlatformIO environment name." >&2
    exit 1
fi

# Read from file argument or stdin
if [ -n "$1" ]; then
    INPUT="$1"
    if [ ! -f "$INPUT" ]; then
        echo "ERROR: file not found: $INPUT" >&2
        exit 1
    fi
    exec < "$INPUT"
fi

# Slurp all of stdin into a temp file so we can do two passes
TMPFILE="$(mktemp /tmp/tasmota-crash-XXXXXX.txt)"
trap 'rm -f "$TMPFILE"' EXIT
cat > "$TMPFILE"

decode_addr() {
    local addr="$1" label="$2"
    local result
    result="$("$ADDR2LINE" -e "$ELF" -f -p "$addr" 2>/dev/null)"
    # Skip fully unresolved addresses (data words misidentified as code)
    case "$result" in
        "?? at"*) return ;;
    esac
    result="$(printf '%s' "$result" | "$CXXFILT")"
    if [ -n "$label" ]; then
        printf "  %s=%s  =>  %s\n" "$label" "$addr" "$result"
    else
        printf "  %s  =>  %s\n" "$addr" "$result"
    fi
}

echo "=== Decoding crash dump ==="
echo "ELF: $ELF"
echo ""

# --- epc1 and other exception program counters ---
echo "--- Exception PC ---"
for tag in epc1 epc2 epc3 excvaddr depc; do
    addr="$(grep -oE "${tag}=0x4[0-9a-fA-F]{7}" "$TMPFILE" | grep -oE '0x4[0-9a-fA-F]{7}' | head -1)"
    [ -n "$addr" ] && decode_addr "$addr" "$tag"
done
echo ""

# --- Stack addresses ---
# The stack dump section is delimited by >>>stack>>> ... <<<stack<<<
# All four words per line are scanned; addr2line ??-results are filtered out.
echo "--- Stack trace ---"
IN_STACK=0
while IFS= read -r line; do
    case "$line" in
        *">>>stack>>>"*) IN_STACK=1; continue ;;
        *"<<<stack<<<"*) IN_STACK=0; continue ;;
    esac
    if [ "$IN_STACK" -eq 1 ]; then
        for addr in $(printf '%s\n' "$line" | grep -oE '(0x)?40[0-9a-fA-F]{6}'); do
            case "$addr" in 0x*) ;; *) addr="0x$addr" ;; esac
            decode_addr "$addr"
        done
    fi
done < "$TMPFILE"

echo ""
echo "=== Done ==="
