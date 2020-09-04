Import("env")
import os

# pio < 4.0.0
# from base64 import b64decode
# env.Replace(UPLOADER="pio\espupload.py")
# env.Replace(UPLOADERFLAGS="")
# env.Replace(UPLOADCMD="$UPLOADER -u " + b64decode(ARGUMENTS.get("UPLOAD_PORT")) + " -f $SOURCES")

# pio >= 4.0.0
env.Replace(UPLOADER=os.path.join("pio", "espupload.py"))
env.Replace(UPLOADERFLAGS="")
env.Replace(UPLOADCMD="$UPLOADER -u $UPLOAD_PORT -f $SOURCES")
