# createt by Theo Arends at 2017-09-30
# modified by mtfurlan at 2010-01-03
# modified by Pascal Gollor at 2022-12-13

Import("env")
import os
import tasmotapiolib


# clear upload flags
env.Replace(UPLOADERFLAGS="")

# get espupload script
uploader=os.path.join("tools", "espupload.py")

# get firmware file
upload_file = tasmotapiolib.get_final_bin_path(env)

# create upload command like:
# esptool.py -i <server ip> -p <server port> -u <uploader url script> -f <source file>
# so you need to specify 'upload_port' in platform_override.ini at '[env]' section
env.Replace(UPLOADCMD="{} $UPLOAD_PORT -f {}".format(uploader, upload_file))

