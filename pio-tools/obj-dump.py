# Little convenience script to get an object dump

Import('env')

def obj_dump_after_elf(source, target, env):
    platform = env.PioPlatform()
    board = env.BoardConfig()
    mcu = board.get("build.mcu", "esp32")

    print("Create firmware.asm")
    if mcu == "esp8266":
        env.Execute("xtensa-lx106-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32":
        env.Execute("xtensa-esp32-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32s2":
        env.Execute("xtensa-esp32s2-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32s3":
        env.Execute("xtensa-esp32s3-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32c3":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", [obj_dump_after_elf])
