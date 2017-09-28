Import("env")
from base64 import b64decode

env.Replace(UPLOADER="echo scp")
env.Replace(UPLOADERFLAGS="")
env.Replace(UPLOADCMD="$UPLOADER $SOURCES " + b64decode(ARGUMENTS.get("UPLOAD_PORT")))
