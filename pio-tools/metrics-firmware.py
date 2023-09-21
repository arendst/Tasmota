Import("env")

import os
import tasmotapiolib
from os.path import join

def firm_metrics(source, target, env):
    if env["PIOPLATFORM"] == "espressif32":
        import tasmota_metrics
        env.Execute("$PYTHONEXE -m tasmota_metrics \"" + str(tasmotapiolib.get_source_map_path(env).resolve()) + "\"")
    elif env["PIOPLATFORM"] == "espressif8266":
        map_file = join(env.subst("$BUILD_DIR")) + os.sep + "firmware.map"
        with open(map_file,'r', encoding='utf-8') as f:
            phrase = "_text_end = ABSOLUTE (.)"
            for line in f:
                if  phrase in line:
                    address = line.strip().split(" ")[0]
                    if int(address, 16) < 0x40108000:
                        used_bytes = int(address, 16) - 0x40100000
                        remaining_bytes = 0x8000 - used_bytes
                        percentage = round(used_bytes / 0x8000 * 100,1)
                        print("Used static IRAM:",used_bytes,"bytes (",remaining_bytes,"remain,",percentage,"% used)")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin",firm_metrics)
