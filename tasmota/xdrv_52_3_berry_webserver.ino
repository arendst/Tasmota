/*
  xdrv_52_3_berry_webserver.ino - Berry scripting language, webserver module

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_BERRY

#ifdef USE_WEBSERVER

#include <berry.h>

/*********************************************************************************************\
 * Int constants
 *********************************************************************************************/
const be_constint_t webserver_constants[] = {
    { "BUTTON_CONFIGURATION", BUTTON_CONFIGURATION },
    { "BUTTON_INFORMATION", BUTTON_INFORMATION },
    { "BUTTON_MAIN", BUTTON_MAIN },
    { "BUTTON_MANAGEMENT", BUTTON_MANAGEMENT },
    { "BUTTON_MODULE", BUTTON_MODULE },
    { "HTTP_ADMIN", HTTP_ADMIN },
    { "HTTP_ANY", HTTP_ANY },
    { "HTTP_GET", HTTP_GET },
    { "HTTP_MANAGER", HTTP_MANAGER },
    { "HTTP_MANAGER_RESET_ONLY", HTTP_MANAGER_RESET_ONLY },
    { "HTTP_OFF", HTTP_OFF },
    { "HTTP_OPTIONS", HTTP_OPTIONS },
    { "HTTP_POST", HTTP_POST },
    { "HTTP_USER", HTTP_USER },
};

extern "C" {
  int w_webserver_member(bvm *vm);
  int w_webserver_member(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * needle = be_tostring(vm, 1);

      int32_t constant_idx = bin_search(needle, &webserver_constants[0].name, sizeof(webserver_constants[0]), ARRAY_SIZE(webserver_constants));
    
      if (constant_idx >= 0) {
        // we did have a match, low == high
        be_pushint(vm, webserver_constants[constant_idx].value);
        be_return(vm);
      }
    }
    be_raise(vm, "attribute_error", "module 'webserver' has no such attribute");
  }
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import webserver
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `webserver.on(prefix:string, callback:closure) -> nil`
  //
  // WARNING - this should be called only when receiving `web_add_handler` event.
  // If called before the WebServer is set up and Wifi on, it will crash.
  // For debug purpose, it can be called later when you are sure that Wifi/Eth is up
  //
  int32_t w_webserver_on(struct bvm *vm);
  int32_t w_webserver_on(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isstring(vm, 1) && be_isclosure(vm, 2) &&
        (argc < 3 || be_isint(vm, 3)) ) {    // optional third argument must be int
      uint8_t method =  HTTP_ANY;   // default method if not specified
      const char * prefix = be_tostring(vm, 1);
      if (argc >= 3) {
        method = be_toint(vm, 3);
      }

      be_getglobal(vm, PSTR("tasmota"));
      if (!be_isnil(vm, -1)) {
        be_getmethod(vm, -1, PSTR("gen_cb"));
        if (!be_isnil(vm, -1)) {
          be_pushvalue(vm, -2); // add instance as first arg
          be_pushvalue(vm, 2);  // push closure as second arg
          be_pcall(vm, 2);   // 2 arguments
          be_pop(vm, 2);

          if (be_iscomptr(vm, -1)) {  // sanity check
            const void * cb = be_tocomptr(vm, -1);
            // All good, we can proceed

            WebServer_on(prefix, (void (*)()) cb, method);
            be_return_nil(vm);    // return, all good
          }
        }
        be_pop(vm, 1);
      }
      // be_pop(vm, 1);   // not really useful since we raise an exception anyways
      be_raise(vm, kInternalError, nullptr);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.state() -> int`
  //
  int32_t w_webserver_state(struct bvm *vm);
  int32_t w_webserver_state(struct bvm *vm) {
    be_pushint(vm, Web.state);
    be_return(vm);
  }

  // Berry: `webserver.check_privileged_access() -> bool`
  //
  int32_t w_webserver_check_privileged_access(struct bvm *vm);
  int32_t w_webserver_check_privileged_access(struct bvm *vm) {
    be_pushbool(vm, HttpCheckPriviledgedAccess());
    be_return(vm);
  }

  // Berry: `webserver.content_start() -> nil`
  //
  int32_t w_webserver_content_start(struct bvm *vm);
  int32_t w_webserver_content_start(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * html = be_tostring(vm, 1);
      WSContentStart_P(html);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.content_send() -> nil`
  //
  int32_t w_webserver_content_send(struct bvm *vm);
  int32_t w_webserver_content_send(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * html = be_tostring(vm, 1);
      WSContentSend_P(PSTR("%s"), html);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.content_send_style() -> nil`
  //
  int32_t w_webserver_content_send_style(struct bvm *vm);
  int32_t w_webserver_content_send_style(struct bvm *vm) {
    WSContentSendStyle();
    be_return_nil(vm);
  }

  // Berry: `webserver.content_flush() -> nil`
  //
  int32_t w_webserver_content_flush(struct bvm *vm);
  int32_t w_webserver_content_flush(struct bvm *vm) {
    WSContentFlush();
    be_return_nil(vm);
  }

  // Berry: `webserver.content_stop() -> nil`
  //
  int32_t w_webserver_content_stop(struct bvm *vm);
  int32_t w_webserver_content_stop(struct bvm *vm) {
    WSContentStop();
    be_return_nil(vm);
  }

  // Berry: `webserver.content_button([button:int]) -> nil`
  // Default button is BUTTON_MAIN
  //
  int32_t w_webserver_content_button(struct bvm *vm);
  int32_t w_webserver_content_button(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc < 1 || be_isint(vm, 1)) {
      int32_t button = BUTTON_MAIN;
      if (argc > 0) {
        button = be_toint(vm, 1);
      }
      WSContentSpaceButton(button);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.args() -> int`
  //
  // Returns the number of arguments
  int32_t w_webserver_argsize(struct bvm *vm);
  int32_t w_webserver_argsize(struct bvm *vm) {
    be_pushint(vm, Webserver->args());
    be_return(vm);
  }
  
  // Berry: `webserver.arg(int or string) -> string`
  //
  // takes either an int (index 0..args-1) or a string (name of arg)
  int32_t w_webserver_arg(struct bvm *vm);
  int32_t w_webserver_arg(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && (be_isint(vm, 1) || be_isstring(vm, 1))) {
      if (be_isint(vm, 1)) {
        int32_t idx = be_toint(vm, 1);
        be_pushstring(vm, Webserver->arg(idx).c_str());
      } else {
        const char * arg_name = be_tostring(vm, 1);
        be_pushstring(vm, Webserver->arg(arg_name).c_str());
      }
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.has_arg(name:string) -> bool`
  //
  int32_t w_webserver_has_arg(struct bvm *vm);
  int32_t w_webserver_has_arg(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * arg_name = be_tostring(vm, 1);
      be_pushbool(vm, Webserver->hasArg(arg_name));
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

}

#endif // USE_WEBSERVER
#endif  // USE_BERRY
