# Produce an object dump (.asm) after building the ELF

Import("env")

# Explicit mapping for Xtensa targets; everything else defaults to RISC-V
XTENSA_OBJDUMP = {
    "esp8266": "xtensa-lx106-elf-objdump",
    "esp32":   "xtensa-esp32-elf-objdump",
    "esp32s2": "xtensa-esp32s2-elf-objdump",
    "esp32s3": "xtensa-esp32s3-elf-objdump",
}

def resolve_objdump_tool(mcu: str) -> str:
    """
    Return objdump tool for MCU.
    Known Xtensa MCUs use explicit tools; all others use RISC-V objdump.
    """
    mcu = (mcu or "").lower().strip()
    return XTENSA_OBJDUMP.get(mcu, "riscv32-esp-elf-objdump")

def obj_dump_after_elf(source, target, env):
    """
    Post-build action: run objdump on the ELF and write ${PROGNAME}.asm.
    """
    board = env.BoardConfig()
    mcu = board.get("build.mcu", "esp32").lower()

    objdump_tool = resolve_objdump_tool(mcu)
    out_file = "$BUILD_DIR/${PROGNAME}.asm"
    print(f"Create {out_file} using {objdump_tool}")

    cmd = f"{objdump_tool} -D -C {target[0]} > {out_file}"
    env.Execute(cmd)

# Silent post-build action
silent_action = env.Action([obj_dump_after_elf])
silent_action.strfunction = lambda target, source, env: ""
env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", silent_action)
