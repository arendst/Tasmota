# Tasmota defines for Berry solidification

## Why this exists

Berry solidification compiles `.be` scripts into pre-built C structures that are
embedded directly in the firmware. The solidifier runs at **build time** on the
host machine (via `berry_port`, a Python re-implementation of the Berry VM).

For solidified code to be correct it must know which Tasmota features are
compiled into the firmware. For example, a solidified class that references
`USE_MATTER_DEVICE` or a `D_JSON_*` string constant must see the same values
that the C++ compiler sees when it builds the firmware. Without this, the
solidified bytecode can embed wrong constants or include dead code paths.

## How the dump is produced

`pio-tools/dump-defines.py` runs as a **pre-script** in the PlatformIO build
(registered in `platformio_tasmota32.ini`, before `gen-berry-structures.py`).
It invokes the host C preprocessor in macro-dump mode:

```
xtensa-esp32-elf-g++ -E -dM -x c++
   -DESP32 <build_flags -D entries>
   -I include/ -I tasmota/include/ -I tasmota/
   -include tasmota/include/tasmota.h
   -include tasmota/my_user_config.h
   -include include/tasmota_options.h
   -include tasmota/include/i18n.h
   - < /dev/null
   > tasmota/tasmota_defines_for_berry.h
```

Key design decisions:

| Decision | Reason |
|---|---|
| Post-script (`post:`) | `$CXX` is set to the real cross-compiler after the platform builder runs. No `-m32` hack needed. |
| Cross-compiler (`$CXX`) | Correct int/pointer widths, endianness, and target-specific built-in macros for ESP32. |
| `-E -dM` | Preprocessor-only mode; outputs every `#define` visible after expanding all headers. |
| Only config/i18n headers, not `tasmota.ino` | `tasmota.ino` pulls in Arduino/framework library headers (`EEPROM.h`, `WiFiHelper.h`, …) that aren't resolvable before PlatformIO's LDF runs. Those libraries don't contribute macros the solidifier needs. |
| Empty `sdkconfig.h` stub | `tasmota_configurations_ESP32.h` includes `sdkconfig.h` which is generated per-MCU at build time. Its `CONFIG_*` values aren't needed for Berry solidification, so an empty stub is supplied. |
| `-D` flags from `BUILD_FLAGS` | Ensures `USE_CONFIG_OVERRIDE`, `MY_LANGUAGE`, firmware-variant flags, etc. are defined exactly as in the real build. |

The output is written to `tasmota/tasmota_defines_for_berry.h` which is
**gitignored** — it is a build artefact, regenerated on every build.

## What the output looks like

The file is a flat list of `#define` lines, one per macro, in the format
produced by `gcc -dM`. Approximately **3 200 lines** for a typical
`tasmota32` build, split roughly as:

| Category | Count | Example |
|---|---|---|
| `USE_*` feature flags | ~220 | `#define USE_BERRY ` |
| `D_*` strings (JSON keys, commands, sensor names) | ~2 000 | `#define D_JSON_TEMPERATURE "Temperature"` |
| Firmware metadata | a few | `#define CODE_IMAGE_STR "tasmota32"` |
| Language / locale | a few | `#define LANGUAGE_LCID 2057` |
| Compiler built-ins + misc | remainder | `#define __INT_MAX__ 2147483647` |

A representative sample:

```c
/* Feature flags */
#define USE_BERRY
#define USE_MATTER_DEVICE
#define USE_RULES
#define USE_TLS
#define USE_WEBSERVER
#define USE_IPV6 1
#define USE_ZIGBEE_ZNP

/* Language / locale */
#define LANGUAGE_LCID 2057          /* en_GB */
#define CODE_IMAGE_STR "tasmota32"

/* JSON key strings */
#define D_JSON_TEMPERATURE "Temperature"
#define D_JSON_TEMPERATURE_UNIT "TempUnit"

/* Command strings */
#define D_CMND_STATUS "Status"

/* Sensor name strings */
#define D_SENSOR_SWITCH "Switch"
```

Flags that are **not** defined simply do not appear in the file. For example,
if `USE_ZIGBEE` (the master gate) is commented out in `my_user_config.h`, there
will be no `#define USE_ZIGBEE` line — only the unconditional sub-configuration
constants (`USE_ZIGBEE_ZNP`, `USE_ZIGBEE_CHANNEL`, …) that are declared as
defaults regardless of the master gate.

## How the solidifier should consume this file

The intended use is to parse it
as a flat text file inside the Berry solidification toolchain so that:

1. **Conditional solidification** — a `.be` class or module is only solidified
   when the corresponding `USE_*` flag is present.
2. **Constant folding** — `D_JSON_*` and `D_CMND_*` string constants can be
   embedded directly into solidified bytecode rather than looked up at runtime.

The file is regenerated at the start of every esp32 build, so it always
reflects the current `user_config_override.h` and `platformio_override.ini`
settings.
