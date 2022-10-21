Import('env')

#
# Dump build environment (for debug)
#print env.Dump()
#

flags = " ".join(env['LINKFLAGS'])
flags = flags.replace("-u _printf_float", "")
flags = flags.replace("-u _scanf_float", "")
newflags = flags.split()

env.Replace(
  LINKFLAGS=newflags
)