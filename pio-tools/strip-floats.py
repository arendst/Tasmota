Import('env')

link_flags = " ".join(env['LINKFLAGS'])
build_flags = " ".join(env['BUILD_FLAGS'])

print("---------------------------------------")

print("build_flags: ", build_flags)

#
# Dump build environment (for debug)
#print(env.Dump())
#

link_flags = link_flags.replace("-u _printf_float", "")
link_flags = link_flags.replace("-u _scanf_float", "")
if "FIRMWARE_SAFEBOOT" in build_flags:
  # Crash Recorder is not included in safeboot firmware -> remove Linker wrap
  link_flags = link_flags.replace("-Wl,--wrap=panicHandler", "")
  link_flags = link_flags.replace("-Wl,--wrap=xt_unhandled_exception", "")

new_link_flags = link_flags.split()

env.Replace(
  LINKFLAGS=new_link_flags
)