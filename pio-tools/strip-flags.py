Import('env')

link_flags = env['LINKFLAGS']
build_flags = " ".join(env['BUILD_FLAGS'])

if "FIRMWARE_SAFEBOOT" in build_flags:
  # Crash Recorder is not included in safeboot firmware -> remove Linker wrap
  try:
    link_flags.pop(link_flags.index("-Wl,--wrap=panicHandler"))
  except:
    do_nothing=""
  try:
    link_flags.pop(link_flags.index("-Wl,--wrap=xt_unhandled_exception"))
  except:
    do_nothing=""