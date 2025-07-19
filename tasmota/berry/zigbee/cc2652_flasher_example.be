import cc2652_flasher as fl

fl.start(true)
print("CCFG=",fl.cmd_memory_read(0x57FD8,4))
print("crc32=",fl.cmd_crc32(0x0,0x30000))            # bytes('1598929A')
