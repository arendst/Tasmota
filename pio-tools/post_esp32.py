# From: https://github.com/letscontrolit/ESPEasy/blob/mega/tools/pio/post_esp32.py
# Thanks TD-er :)

Import("env")

def esp32_create_factory_bin(source, target, env):
    #print("Generating one file initial flash firmware for esp32 units")
    offset = 0x0
    new_file_name = env.subst("$BUILD_DIR/${PROGNAME}-factory.bin")
    sections = env.subst(env.get('FLASH_EXTRA_IMAGES'))
    new_file = open(new_file_name,"wb")
    print("    {} | {}".format("Offset", "File"))
    for section in sections:
      sect_adr,sect_file = section.split(" ",1)
      print(" -  {} | {}".format(sect_adr,sect_file))
      source = open(sect_file,"rb")
      new_file.seek(int(sect_adr,0)-offset)
      new_file.write(source.read());
      source.close()

    firmware_name = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    firmware_start = 0x10000-offset
    firmware = open(firmware_name,"rb")
    print(" - {} | {}".format(hex(firmware_start),firmware_name))
    new_file.seek(firmware_start)
    new_file.write(firmware.read())
    new_file.close()
    firmware.close()

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", esp32_create_factory_bin)
