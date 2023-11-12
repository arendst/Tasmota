Import("env")

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])
