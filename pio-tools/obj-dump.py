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
    if mcu == "esp32c2":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32c3":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32c5":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32c6":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32p4":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")
    if mcu == "esp32h2":
        env.Execute("riscv32-esp-elf-objdump "+ "-D -C " + str(target[0]) + " > "+ "$BUILD_DIR/${PROGNAME}.asm")

silent_action = env.Action([obj_dump_after_elf])
silent_action.strfunction = lambda target, source, env: '' # hack to silence scons command output
env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", silent_action)
