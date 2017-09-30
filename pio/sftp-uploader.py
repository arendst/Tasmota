Import("env")
from base64 import b64decode

env.Replace(UPLOADER="scp")
env.Replace(UPLOADERFLAGS="")
env.Replace(UPLOADCMD="$UPLOADER $SOURCES " + b64decode(ARGUMENTS.get("UPLOAD_PORT")) + "/" + b64decode(ARGUMENTS.get("PIOENV")) + ".bin")
