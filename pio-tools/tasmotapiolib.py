"""Supporting library for pio-tools scripts"""
import pathlib

OUTPUT_DIR = pathlib.Path("build_output")


def get_variant(env) -> str:
    """Get the current build variant."""
    return env["PIOENV"]


def get_final_bin_path(env) -> pathlib.Path:
    """Path to the final destination for the .bin

    If the parent directory does not exist, it will be created"""
    firmware_dir = OUTPUT_DIR / "firmware"
    firmware_dir.mkdir(parents=True, exist_ok=True)
    return firmware_dir / "{}.bin".format(get_variant(env))


def get_final_map_path(env) -> pathlib.Path:
    """Path to the final destination for the .map file

    If the parent directory does not exist, it will be created"""
    map_dir = OUTPUT_DIR / "map"
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
