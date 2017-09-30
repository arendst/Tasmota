Import("env")

from base64 import b64decode

env.Replace(UPLOADER="pio\espupload.py")
env.Replace(UPLOADERFLAGS="")
env.Replace(UPLOADCMD="$UPLOADER -u " + b64decode(ARGUMENTS.get("UPLOAD_PORT")) + " -f $SOURCES")

'''
env.Replace(UPLOADCMD="pio\espupload.py -f $SOURCES") # Windows
env.Replace(UPLOADCMD="pio/espupload.py -f $SOURCES") # Linux
'''