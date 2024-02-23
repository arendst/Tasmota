Writeable Flash Region Test App
===============================

This app simply uses the `memop` syscalls relevant to app-specific flash regions
and prints the address of the regions that are defined in the Tock Binary Format
header for the app. See the
[documentation](https://github.com/tock/tock/blob/master/doc/Compilation.md#tock-binary-format)
for more about these regions.
