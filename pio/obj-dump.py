# Little convenience script to get an object dump

Import('env')

def obj_dump_after_elf(source, target, env):
    print("Create firmware.asm")
    env.Execute("xtensa-lx106-elf-objdump "+ "-D " + str(target[0]) + " > "+ "${PROGNAME}.asm")
    
env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", [obj_dump_after_elf])
