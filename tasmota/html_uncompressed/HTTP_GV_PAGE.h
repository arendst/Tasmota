const char HTTP_GV_PAGE[] PROGMEM =
  "<!DOCTYPE html>"
  "<html lang='en'>"
  "<head>"
    "<title>%s GPIO Viewer</title>"                             // SettingsTextEscaped(SET_DEVICENAME).c_str()
    "<meta charset='UTF-8'>"
    "<base href='%s'>"                                          // GV_BASE_URL
    "<link rel='icon' href='favicon.ico'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<script type='module' crossorigin src='GPIOViewerVue.js'>"
    "</script>"
    "<link rel='stylesheet' crossorigin href='assets/main.css'>"
  "</head>"
  "<body>"
    "<div id='app'></div>"
    "<script>"
      "window.gpio_settings = {"
        "ip:'%s',"                                              // WiFi.localIP().toString().c_str()
        "port:'%d',"                                            // GV_PORT
        "freeSketchRam:'%d KB'"                                 // ESP_getFreeSketchSpace() / 1024
      "};"
    "</script>"
  "</body>"
  "</html>";
