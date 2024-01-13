const char HTTP_GV_PAGE[] PROGMEM =
  "<!DOCTYPE HTML>"
  "<html>"
    "<head>"
    "<title>%s - GPIO Viewer</title>"                           // SettingsTextEscaped(SET_DEVICENAME).c_str()
    "<base href='%s'>"                                          // GV_BASE_URL
    "<link id='defaultStyleSheet' rel='stylesheet' href=''>"
    "<link id='boardStyleSheet' rel='stylesheet' href=''>"
    "<link rel='icon' href='favicon.ico' type='image/x-icon'>"
    "<script src='script/webSocket.js'></script>"
    "<script src='script/boardSwitcher.js'></script>"
    "<script>"
      "var serverPort=%d;"                                      // GV_PORT
      "var ip='%s';"                                            // WiFi.localIP().toString().c_str()
      "var source=new EventSource('http://%s:%d/events');"      // WiFi.localIP().toString().c_str(), GV_PORT
      "var sampling_interval='%d';"                             // Gv.sampling
      "var psramSize='%d KB';"                                  // ESP.getPsramSize() / 1024
      "var freeSketchSpace='%d KB';"                            // ESP_getFreeSketchSpace() / 1024
    "</script>"
  "</head>"
  "<body>"
    "<div class='grid-container'>"
      "<div id='messageBox' class='message-box hidden'></div>"
      "<header class='header'></header>"
      "<div class='image-container'>"
        "<div id='imageWrapper' class='image-wrapper'>"
          "<img id='boardImage' src='' alt='Board Image'>"
          "<div id='indicators'></div>"
        "</div>"
      "</div>"
    "</div>"
  "</body>"
  "</html>";