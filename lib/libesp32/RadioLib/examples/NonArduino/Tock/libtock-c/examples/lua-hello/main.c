#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <timer.h>

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

// POSIX wrapper for `nanosleep` to make this compilable and runnable on
// Linux/OS X/BSD for testing.
#if defined(__unix__)
#include <time.h>
void delay_ms(int ms) {
  struct timespec ts;
  ts.tv_sec  = 0;
  ts.tv_nsec = (long)ms * 1000 * 1000;
  nanosleep(&ts, NULL);
}
#endif

int main(void) {
  // Open lua
  lua_State *L = luaL_newstate();

  luaL_requiref(L, "_G", luaopen_base, true);

  // Execution of a lua string
  int err;
  err = luaL_loadstring(L, "main = function() print(\"Hello from Lua!\") end");

  if (err != LUA_OK) {
    switch (err) {
      case LUA_ERRSYNTAX:
        printf("Syntax error\n");
        break;
      case LUA_ERRMEM:
        printf("Out of memory\n");
        break;
      case LUA_ERRGCMM:
        printf("Garbage collection error\n");
        break;
      default:
        printf("Unknown error %d\n", err);
        break;
    }
    return -1;
  }

  lua_call(L, 0, 0);

  while (1) {
    lua_getglobal(L, "main");
    lua_call(L, 0, 0);

    int kb    = lua_gc(L, LUA_GCCOUNT, 0);
    int bytes = lua_gc(L, LUA_GCCOUNT, 0);
    printf("> %dKB and %d bytes used\n", kb, bytes);
    delay_ms(500);
  }

  // Close lua
  lua_close(L);

  return 0;
}
