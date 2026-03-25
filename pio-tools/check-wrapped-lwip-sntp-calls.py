Import("env")

import subprocess
import sys
import os


def parse_nm_symbols(nm_output):
    symbols = set()
    for line in nm_output.splitlines():
        parts = line.strip().split()
        if len(parts) >= 3:
            symbols.add(parts[-1])
    return symbols


def check_sntp_wrap(target, source, env):
    print("SNTP WRAP SYMBOL CHECK: Checking firmware")

    board = env.BoardConfig()
    mcu = board.get("build.mcu", "esp32").lower()

    if mcu != "esp8266":
        print("SNTP WRAP SYMBOL CHECK: Skipped for MCU =", mcu)
        return

    firmware = str(target[0])

    if not os.path.exists(firmware):
        print("ERROR: Firmware ELF not found:", firmware)
        sys.exit(1)
    else:
        print("Firmware ELF found:", firmware)

    nm = "xtensa-lx106-elf-nm"

    result = subprocess.run(
        [nm, "-n", firmware],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("ERROR: nm failed to execute")
        print(result.stderr)
        sys.exit(1)

    symbols = parse_nm_symbols(result.stdout)

    required = [
        "__wrap_sntp_init",
        "__wrap_sntp_stop",
    ]

    forbidden = [
        "sntp_init",
        "sntp_stop",
    ]

    found_required = [s for s in required if s in symbols]
    missing_required = [s for s in required if s not in symbols]
    found_forbidden = [s for s in forbidden if s in symbols]

    print("Required wrap symbols:")
    for s in required:
        status = "OK" if s in symbols else "MISSING"
        print(" ", status + ":", s)

    print("Forbidden original symbols:")
    if found_forbidden:
        for s in found_forbidden:
            print("  FORBIDDEN:", s)
    else:
        print("  None")

    if missing_required or found_forbidden:
        print("FAILED: lwIP SNTP wrap symbol check failed.")

        if missing_required:
            print("Missing required wrap symbols:")
            for s in missing_required:
                print("  MISSING:", s)

        if found_forbidden:
            print("Forbidden original symbols detected:")
            for s in found_forbidden:
                print("  FORBIDDEN:", s)

        sys.exit(1)

    print("PASSED: All lwIP SNTP wrap symbols are present and original symbols are not found.")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", check_sntp_wrap)
