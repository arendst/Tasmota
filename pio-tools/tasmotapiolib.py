"""Supporting library for pio-tools scripts"""
import pathlib
import os

# Constants for environment variables that can be set to control build output file
# locations and formats

# if set to 1, will not gzip esp8266 bin files
DISABLE_BIN_GZ = "TASMOTA_DISABLE_BIN_GZ"
# if set, an alternative ptah to put generated .bin files
BIN_DIR = "TASMOTA_BIN_DIR"
# if set to 1, will not gzip generated .map files
DISABLE_MAP_GZ = "TASMOTA_DISABLE_MAP_GZ"
# if set, an alternative path to put generated .map files
MAP_DIR = "TASMOTA_MAP_DIR"

OUTPUT_DIR = pathlib.Path("build_output")


def get_variant(env) -> str:
    """Get the current build variant."""
    return env["PIOENV"]


def get_final_bin_path(env) -> pathlib.Path:
    """Path to the final destination for the .bin

    If the parent directory does not exist, it will be created"""
    firmware_dir = get_override_path(BIN_DIR)
    firmware_dir.mkdir(parents=True, exist_ok=True)
    return firmware_dir / "{}.bin".format(get_variant(env))


def get_final_map_path(env) -> pathlib.Path:
    """Path to the final destination for the .map file

    If the parent directory does not exist, it will be created"""
    map_dir = get_override_path(MAP_DIR)
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

    raise FileNotFoundError


def get_override_path(pathtype) -> pathlib.Path:
    """
    Returns a path to a givens override path if set, otherwise OUTPUT_DIR is used

    pathtype must be either MAP_DIR or BIN_DIR.
    """
    override = os.environ.get(pathtype)
    if override:
        return pathlib.Path(override)
    if pathtype == BIN_DIR:
        return OUTPUT_DIR / "firmware"
    elif pathtype == MAP_DIR:
        return OUTPUT_DIR / "map"
    raise ValueError


def is_env_set(name: str):
    """True if the enviornment variable <name> is set to `1`"""
    val = os.environ.get(name.upper())
    if val:
        val = val.strip()
        return val == "1"
    return False
