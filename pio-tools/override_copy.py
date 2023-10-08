Import('env')

import os
import pathlib
from os.path import join
import shutil

if " " in join(pathlib.Path(env["PROJECT_DIR"])):
    print ("\u001b[31;1m*** Whitespace(s) in project path, unexpected issues/errors can happen ***\u001b[0m")

# copy tasmota/user_config_override_sample.h to tasmota/user_config_override.h
if os.path.isfile("tasmota/user_config_override.h"):
    print ("*** use provided user_config_override.h as planned ***")
else:
    shutil.copy("tasmota/user_config_override_sample.h", "tasmota/user_config_override.h")

# copy platformio_override_sample.ini to platformio_override.ini
if os.path.isfile("platformio_override.ini"):
    print ("*** use provided platformio_override.ini as planned ***")
else:
    shutil.copy("platformio_override_sample.ini", "platformio_override.ini")

# copy platformio_tasmota_cenv_sample.ini to platformio_tasmota_cenv.ini
if os.path.isfile("platformio_tasmota_cenv.ini"):
    print ("*** use provided platformio_tasmota_cenv.ini as planned ***")
else:
    shutil.copy("platformio_tasmota_cenv_sample.ini", "platformio_tasmota_cenv.ini")

# copy platformio_tasmota_core3_env_sample.ini to platformio_tasmota_core3_env.ini
if os.path.isfile("platformio_tasmota_core3_env.ini"):
    print ("*** use provided platformio_tasmota_core3_env.ini as planned ***")
else:
    shutil.copy("platformio_tasmota_core3_env_sample.ini", "platformio_tasmota_core3_env.ini")