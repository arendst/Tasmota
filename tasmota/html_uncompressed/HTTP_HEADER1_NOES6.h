const char HTTP_HEADER1[] PROGMEM =
  "<!DOCTYPE html><html lang=\"%s\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<link rel=\"icon\" href=\"data:image/x-icon;base64,AAABAAEAEBACAAEAAQCwAAAAFgAAACgAAAAQAAAAIAAAAAEAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA////AP5/b+H6X2/h8k9v4eZnb+Hud2/h7ndv4e53b+FmZm/hMkxv4ZgZb+HOc2/h5+dv4fPPb+H5n2/h/D9v4f5/b+EAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4E\">"
  "<title>%s - %s</title>"

  "<script>"
  "var x=null,lt,to,tp,pc='';"            // x=null allow for abortion

  "function eb(s){"
    "return document.getElementById(s);"  // Alias to save code space
  "}"
  "function qs(s){"                       // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function sp(i){"                       // Toggle password visibility
    "eb(i).type=(eb(i).type==='text'?'password':'text');"
  "}"
  "function wl(f){"                       // Execute multiple window.onload
    "window.addEventListener('load',f);"
  "}";
