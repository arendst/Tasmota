"""Supporting library for pio-tools scripts

This also provides functions to allow overrides of some settings, see the available
overides below.

Overrides can be set using environment variables or .ini file settings for controlling
build output file locations and formats.

To set a value using an environment variable, prefix the value with "TASMOTA_" and
ensure the entire value is UPPER CASE, for example in bash, it would be:

      export TASMOTA_DISABLE_MAP_GZ=1

To set a value in your .ini file, such as in platformio_override.ini, create a
[tasmota] section, and put the key ensuring it is all lower case, for example:

[tasmota]
disable_map_gz = 1
map_dir = /tmp/map_files/

Values in .ini files override environment variables

"""
import pathlib
import os

# === AVAILABLE OVERRIDES ===
# if set to 1, will not gzip bin files at all
DISABLE_BIN_GZ = "disable_bin_gz"
# if set to 1, will gzip esp32 bin files
ENABLE_ESP32_GZ = "enable_esp32_gz"
# if set, an alternative ptah to put generated .bin files, relative to project directory
BIN_DIR = "bin_dir"
# if set to 1, will not gzip generated .map files
DISABLE_MAP_GZ = "disable_map_gz"
# if set, an alternative path to put generated .map files, relative to project directory
MAP_DIR = "map_dir"

# === END AVAILABLE OVERRIDES ===


# This is the default output directory
OUTPUT_DIR = pathlib.Path("build_output")


def get_variant(env) -> str:
    """Get the current build variant."""
    return env["PIOENV"]


def get_final_bin_path(env) -> pathlib.Path:
    """Path to the final destination for the .bin

    If the parent directory does not exist, it will be created"""
    firmware_dir = get_override_path(BIN_DIR, env)
    firmware_dir.mkdir(parents=True, exist_ok=True)
    return firmware_dir / "{}.bin".format(get_variant(env))


def get_final_map_path(env) -> pathlib.Path:
    """Path to the final destination for the .map file

    If the parent directory does not exist, it will be created"""
    map_dir = get_override_path(MAP_DIR, env)
    map_dir.mkdir(parents=True, exist_ok=True)
    return map_dir / "{}.map".format(get_variant(env))


def get_source_map_path(env) -> pathlib.Path:
    """Path to the built .map file.

    Tests potential locations, returning the first match.
    Raises FileNotFoundError if no match found"""
    fwmap_path = pathlib.Path("firmware.map")
    if fwmap_path.is_file():
        return fwmap_path

    # firmware maybe in project build directory
    # PIO env variables see: https://github.com/platformio/platformio-core/blob/develop/platformio/builder/main.py#L108:L128
    proj_build_dir = pathlib.Path(env["PROJECT_BUILD_DIR"])
    proj_dir = pathlib.Path(env["PROJECT_DIR"])
    map_name = proj_dir.parts[-1] + ".map"
    fwmap_path = proj_build_dir / get_variant(env) / map_name
    if fwmap_path.is_file():
        return fwmap_path

    map_name = "firmware.map"
    fwmap_path = proj_build_dir / get_variant(env) / map_name
    if fwmap_path.is_file():
        return fwmap_path

    raise FileNotFoundError


def get_tasmota_override_option(name: str, env):
    """Gets a set override option from a .ini or env variable, None if no match"""
    config = env.GetProjectConfig()
    override = config.get("tasmota", name.lower(), None)
    if override is not None:
        return override
    # Return env if available
    return os.environ.get("TASMOTA_" + name.upper())


def get_override_path(pathtype: str, env) -> pathlib.Path:
    """
    Returns a path to a givens override path if set, otherwise OUTPUT_DIR is used

    pathtype must be either MAP_DIR or BIN_DIR.
    """
    override = get_tasmota_override_option(pathtype, env)
    if override:
        return pathlib.Path(override)
    if pathtype == BIN_DIR:
        return OUTPUT_DIR / "firmware"
    elif pathtype == MAP_DIR:
        return OUTPUT_DIR / "map"
    raise ValueError


def is_env_set(name: str, env):
    """True if the enviornment variable <name> is set to `1`"""
    val = get_tasmota_override_option(name, env)
    if val:
        val = val.strip()
        return val == "1"
    return False
