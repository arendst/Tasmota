#include "<module_name>.h"
#if !defined(RADIOLIB_EXCLUDE_<module_name>)

<module_name>::<module_name>(Module* mod) {
  /*
    Constructor implementation MUST assign the provided "mod" pointer to the private "_mod" pointer.
  */
  _mod = mod;
}

int16_t <module_name>::begin() {
  /*
    "begin" method implementation MUST call the "init" method with appropriate settings.
  */
  _mod->init();

  /*
    "begin" method SHOULD implement some sort of mechanism to verify the connection between Arduino and the module.

    For example, reading a version register
  */
}
