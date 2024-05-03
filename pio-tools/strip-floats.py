Import('env')

build_flags = ''.join([element.replace("-D", " ") for element in env.GetProjectOption("build_flags")])

#
# Dump build environment (for debug)
#print env.Dump()
#

flags = " ".join(env['LINKFLAGS'])
flags = flags.replace("-u _printf_float", "")
flags = flags.replace("-u _scanf_float", "")
if "FIRMWARE_SAFEBOOT" in build_flags:
  # Crash Recorder is not included in safeboot firmware -> remove Linker wrap
  flags = flags.replace("-Wl,--wrap=panicHandler", "")
  flags = flags.replace("-Wl,--wrap=xt_unhandled_exception", "")
newflags = flags.split()

env.Replace(
  LINKFLAGS=newflags
)