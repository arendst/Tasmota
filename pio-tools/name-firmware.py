Import("env")

import os
import shutil
import pathlib
import tasmotapiolib
from os.path import join


def bin_map_copy(source, target, env):
    firsttarget = pathlib.Path(target[0].path)

    # get locations and file names based on variant
    map_file = tasmotapiolib.get_final_map_path(env)
    bin_file = tasmotapiolib.get_final_bin_path(env)
    one_bin_file = bin_file
    firmware_name = env.subst("$BUILD_DIR/${PROGNAME}.bin")

    if env["PIOPLATFORM"] == "espressif32":
        if("safeboot" not in firmware_name):
            factory_tmp = pathlib.Path(firsttarget).with_suffix("")
            factory = factory_tmp.with_suffix(factory_tmp.suffix + ".factory.bin")
            one_bin_tmp = pathlib.Path(bin_file).with_suffix("")
            one_bin_file = one_bin_tmp.with_suffix(one_bin_tmp.suffix + ".factory.bin")

    # check if new target files exist and remove if necessary
    for f in [map_file, bin_file, one_bin_file]:
        if f.is_file():
            f.unlink()

    # copy firmware.bin and map to final destination
    shutil.copy(firsttarget, bin_file)
    if env["PIOPLATFORM"] == "espressif32":
        # the map file is needed later for firmware-metrics.py
        shutil.copy(tasmotapiolib.get_source_map_path(env), map_file)
        if("safeboot" not in firmware_name):
            shutil.copy(factory, one_bin_file)
    else:
        map_firm = join(env.subst("$BUILD_DIR")) + os.sep + "firmware.map"
        shutil.copy(tasmotapiolib.get_source_map_path(env), map_firm)
        shutil.move(tasmotapiolib.get_source_map_path(env), map_file)
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", bin_map_copy)
