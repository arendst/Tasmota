Import('env')


import os
import pathlib
import shutil
from colorama import Fore, Back, Style


project_dir = os.path.normpath(env["PROJECT_DIR"])
if " " in project_dir:
    print(Fore.RED + "*** Whitespace(s) in project path, unexpected issues/errors can happen ***")

# copy tasmota/user_config_override_sample.h to tasmota/user_config_override.h
uc_override = pathlib.Path(os.path.normpath("tasmota/user_config_override.h"))
uc_override_sample = pathlib.Path(os.path.normpath("tasmota/user_config_override_sample.h"))
if uc_override.is_file():
    print(Fore.GREEN + "*** use provided user_config_override.h as planned ***")
else:
    shutil.copy(str(uc_override_sample), str(uc_override))

# copy platformio_override_sample.ini to platformio_override.ini
pio_override = pathlib.Path(os.path.normpath("platformio_override.ini"))
pio_override_sample = pathlib.Path(os.path.normpath("platformio_override_sample.ini"))
if pio_override.is_file():
    print(Fore.GREEN + "*** use provided platformio_override.ini as planned ***")
else:
    shutil.copy(str(pio_override_sample), str(pio_override))

# copy platformio_tasmota_cenv_sample.ini to platformio_tasmota_cenv.ini
pio_cenv = pathlib.Path(os.path.normpath("platformio_tasmota_cenv.ini"))
pio_cenv_sample = pathlib.Path(os.path.normpath("platformio_tasmota_cenv_sample.ini"))
if pio_cenv.is_file():
    print(Fore.GREEN + "*** use provided platformio_tasmota_cenv.ini as planned ***")
else:
    shutil.copy(str(pio_cenv_sample), str(pio_cenv))
