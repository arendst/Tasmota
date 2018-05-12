Import("env")
from base64 import b64decode

env.Replace(UPLOADER="pscp")
env.Replace(UPLOADERFLAGS="-i C:\Users\robert\AppData\Local\.ssh\Private_key.ppk")
env.Replace(UPLOADCMD="$UPLOADER $SOURCES $PLOADERFLAGS " + b64decode(ARGUMENTS.get("UPLOAD_PORT")) + "/" + b64decode(ARGUMENTS.get("PIOENV")) + ".bin")
