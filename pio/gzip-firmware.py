Import('env')
import os
import shutil
import gzip

OUTPUT_DIR = "build_output{}".format(os.path.sep)

def bin_gzip(source, target, env):
    variant = str(target[0]).split(os.path.sep)[1]
    
    # create string with location and file names based on variant
    bin_file = "{}firmware{}{}.bin".format(OUTPUT_DIR, os.path.sep, variant)
    gzip_file = "{}firmware{}{}.bin.gz".format(OUTPUT_DIR, os.path.sep, variant)

    # check if new target files exist and remove if necessary
    for f in [gzip_file]:
        if os.path.isfile(f):
            os.remove(f)

    # write gzip firmware file
    fp = open(bin_file,"rb")
    data = fp.read()
    bindata = bytearray(data)
    with gzip.open(gzip_file, "wb") as f:
        f.write(bindata)
    fp.close()
    f.close()

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", [bin_gzip])
