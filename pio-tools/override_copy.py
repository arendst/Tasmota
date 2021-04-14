Import('env')
import os
import shutil

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
