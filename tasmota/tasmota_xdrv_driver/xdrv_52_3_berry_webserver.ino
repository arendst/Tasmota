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
const be_const_member_t webserver_constants[] = {
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

      int32_t constant_idx = be_map_bin_search(needle, &webserver_constants[0].name, sizeof(webserver_constants[0]), ARRAY_SIZE(webserver_constants));
    
      if (constant_idx >= 0) {
        // we did have a match, low == high
        be_pushint(vm, webserver_constants[constant_idx].value);
        be_return(vm);
      }
    }
    be_module_load(vm, be_newstr(vm, "undefined"));
    be_return(vm);
  }
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import webserver
 * 
\*********************************************************************************************/

#define WEBSERVER_REQ_HANDLER_HOOK_MAX       16      // max number of callbacks, each callback requires a distinct address
static String be_webserver_prefix[WEBSERVER_REQ_HANDLER_HOOK_MAX];
static uint8_t be_webserver_method[WEBSERVER_REQ_HANDLER_HOOK_MAX];

extern "C" {
  typedef void (*berry_webserver_cb_t)(void);
  extern berry_webserver_cb_t be_webserver_allocate_hook(bvm *vm, int32_t num, bvalue *f);
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

      // find if the prefix/method is already defined
      int32_t slot;
      for (slot = 0; slot < WEBSERVER_REQ_HANDLER_HOOK_MAX; slot++) {
        // AddLog(LOG_LEVEL_INFO, ">>>: slot [%i] prefix='%s' method=%i", slot, be_webserver_prefix[slot] ? be_webserver_prefix[slot].c_str() : "<empty>", be_webserver_method[slot]);
        if (be_webserver_prefix[slot] == prefix && be_webserver_method[slot] == method) {
          break;
        }
      }

      if (slot >= WEBSERVER_REQ_HANDLER_HOOK_MAX) {
        // we didn't find a duplicate, let's find a free slot
        for (slot = 0; slot < WEBSERVER_REQ_HANDLER_HOOK_MAX; slot++) {
          // AddLog(LOG_LEVEL_INFO, ">>>2: slot [%i] prefix='%s' method=%i", slot, be_webserver_prefix[slot] ? be_webserver_prefix[slot].c_str() : "<empty>", be_webserver_method[slot]);
          if (be_webserver_prefix[slot].equals("")) {
            break;
          }
        }
        if (slot >= WEBSERVER_REQ_HANDLER_HOOK_MAX) {
          be_raise(vm, "internal_error", "no more slots for webserver hooks");
        }
      }
      // AddLog(LOG_LEVEL_INFO, ">>>: slot found = %i", slot);

      bvalue *v = be_indexof(vm, 2);
      if (be_isgcobj(v)) {
        be_gc_fix_set(vm, v->v.gc, btrue);    // mark the function as non-gc
      }
      berry_webserver_cb_t cb = be_webserver_allocate_hook(vm, slot, v);
      if (cb == NULL) { be_raise(vm, kInternalError, nullptr); }
      be_webserver_prefix[slot] = prefix;
      be_webserver_method[slot] = method;

      WebServer_on(prefix, cb, method);
      be_return_nil(vm);    // return, all good
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

  // Berry: `webserver.redirect(string) -> nil`
  //
  int32_t w_webserver_redirect(struct bvm *vm);
  int32_t w_webserver_redirect(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * uri = be_tostring(vm, 1);
      Webserver->sendHeader("Location", uri, true);
      Webserver->send(302, "text/plain", "");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.content_start(title:string) -> nil`
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

  // Berry: `webserver.content_open(http_code:int, mimetype:string) -> nil`
  //
  int32_t w_webserver_content_open(struct bvm *vm);
  int32_t w_webserver_content_open(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isint(vm, 1) && be_isstring(vm, 2)) {
      int32_t httpcode = be_toint(vm, 1);
      const char * mimetype = be_tostring(vm, 2);
      Webserver->client().flush();
      WSHeaderSend();
      Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN);
      Webserver->send(httpcode, mimetype, "");
      Web.chunk_buffer = "";
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.content_send(string or bytes) -> nil`
  //
  int32_t w_webserver_content_send(struct bvm *vm);
  int32_t w_webserver_content_send(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && (be_isstring(vm, 1) || be_iscomptr(vm, 1) || be_isbytes(vm, 1))) {
      const char * html;
      if (be_isstring(vm, 1)) {
        html = be_tostring(vm, 1);
      }
      else if(be_isbytes(vm, 1)) {
        size_t buf_len;
        const char* buf_ptr = (const char*) be_tobytes(vm, 1, &buf_len);
        WSContentSend(buf_ptr, buf_len);
        be_return_nil(vm);
      } else {
        html = (const char*) be_tocomptr(vm, 1);
      }
      WSContentSend_P(PSTR("%s"), html);
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `webserver.content_response(string) -> nil`
  //
  int32_t w_webserver_content_response(struct bvm *vm);
  int32_t w_webserver_content_response(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * response = be_tostring(vm, 1);
      WSReturnSimpleString(response);
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

  // Berry: `webserver.content_close() -> nil`
  //
  int32_t w_webserver_content_close(struct bvm *vm);
  int32_t w_webserver_content_close(struct bvm *vm) {
    WSContentEnd();
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

  // Berry: `webserver.html_escape(string) -> string`
  //
  int32_t w_webserver_html_escape(struct bvm *vm);
  int32_t w_webserver_html_escape(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * text = be_tostring(vm, 1);
      String html = HtmlEscape(text);
      be_pushstring(vm, html.c_str());
      be_return(vm);
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

  // Berry: `webserver.arg_name(int) -> string`
  //
  // takes an int (index 0..args-1)
  int32_t w_webserver_arg_name(struct bvm *vm);
  int32_t w_webserver_arg_name(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isint(vm, 1)) {
      int32_t idx = be_toint(vm, 1);
      be_pushstring(vm, Webserver->argName(idx).c_str());
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

  // Berry: `webserver.header(name:string) -> string or nil`
  int32_t w_webserver_header(struct bvm *vm);
  int32_t w_webserver_header(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isstring(vm, 1)) {
      const char * header_name = be_tostring(vm, 1);
      String header = Webserver->header(header_name);
      if (header.length() > 0) {
        be_pushstring(vm, header.c_str());
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

}

#endif // USE_WEBSERVER
#endif  // USE_BERRY
