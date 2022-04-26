import os
import shutil
from os.path import join
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoespressif32")

safemode_dir = join(env["PROJECT_DIR"], "safemode")
variants_dir = join(FRAMEWORK_DIR, "variants", "tasmota")

if env["PIOPLATFORM"] == "espressif32":
    if os.path.exists(safemode_dir):
#        print("safemode.bin dir exists")
        if os.path.exists(variants_dir):
            print("") # dummy print
#            print("variants/tasmota exists")
#            os.remove(variants_dir)  # todo PermissionError: [Errno 1] Operation not permitted:
        else:
            shutil.copytree(safemode_dir, variants_dir)
