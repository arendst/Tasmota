Import("env")

# pio < 4.0.0
# from base64 import b64decode
# env.Replace(UPLOADER="scp")
# env.Replace(UPLOADERFLAGS="")
# env.Replace(UPLOADCMD="$UPLOADER $SOURCES " + b64decode(ARGUMENTS.get("UPLOAD_PORT")) + "/" + b64decode(ARGUMENTS.get("PIOENV")) + ".bin")

# pio >= 4.0.0
env.Replace(UPLOADER="scp")
env.Replace(UPLOADERFLAGS="")
env.Replace(UPLOADCMD='$UPLOADER $SOURCES "$UPLOAD_PORT/${PIOENV}.bin"')
