# created by Pascal Gollor at 2022-12-13
# works only with gzip compression enabled

Import("env")
import os
import tasmotapiolib


# clear upload flags
env.Replace(UPLOADERFLAGS="")

if not tasmotapiolib.is_env_set(tasmotapiolib.DISABLE_BIN_GZ, env):
    # get espupload script
    uploader=os.path.join("tools", "espupload.py")

    bin_file = tasmotapiolib.get_final_bin_path(env)
    upload_file = bin_file.with_suffix(".bin.gz")

    # create upload command like:
    # esptool.py -i <server ip> -p <server port> -u <uploader url script> -f <source file>
    # so you need to specify 'upload_port' in platform_override.ini at '[env]' section
    env.Replace(UPLOADCMD="{} $UPLOAD_PORT -f {}".format(uploader, upload_file))
else:
    print("Can not upload nonexistent gzip file")
    env.Replace(UPLOADCMD="")

