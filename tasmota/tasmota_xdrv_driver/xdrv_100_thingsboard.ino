#ifdef USE_THINGSBOARD_HTTP

#define XDRV_100 100

/*const char HTTP_BTN_MENU_TB[] PROGMEM =
    "<p><form action='tb' method='get'><button>" D_CONFIGURE_MQTT "</button></form></p>";
*/
bool change = false;

struct Telementary_data
{
    char key[10];
    char value[20];
};

struct Telementary_data Tele[] = {
    {"POWER", ""},
    {"Color", ""},
    {"CT", ""}};

bool Xdrv100(uint32_t function)
{
    switch (function)
    {
    case FUNC_INIT:
        AddLog(LOG_LEVEL_INFO, PSTR("TB : ThingsBoard HTTP Initialized"));
        // SettingsUpdateText(SET_TB_HOST, PSTR(THINGSBOARD_HOST));
        // SettingsUpdateText(SET_TB_TOKEN, PSTR(THINGSBOARD_TOKEN));
        break;

    case FUNC_SET_POWER:
    {
        snprintf_P(Tele[0].value, sizeof(Tele[0].value), PSTR("%s"), (XdrvMailbox.index == 1) ? "ON" : "OFF");
        change = true;
        break;
    }
    case FUNC_EVERY_SECOND:
    {
        char color_str[20];
        LightGetColor(color_str, sizeof(color_str));

        if (strcmp(Tele[1].value, color_str) != 0)
        {
            snprintf_P(Tele[1].value, sizeof(Tele[1].value), PSTR("%s"), color_str);
            if (LightGetColorTemp() != 0)
                snprintf_P(Tele[2].value, sizeof(Tele[2].value), PSTR("%d"), LightGetColorTemp());
            change = true;
        }

        if (change)
        {
            SendThingsBoardTelemetry();
            change = false;
        }

        FetchThingsBoardRPC();
        break;
    }
#ifdef USE_WEBSERVER
    // case FUNC_WEB_ADD_BUTTON:
    // {
    //     WSContentSend_P()
    //     break;
    // }
    // case FUNC_WEB_ADD_HANDLER:
    // {
    //     WebServer.on("/thingsboard", HandleTB);
    //     break;
    // }
#endif // USE_WEBSERVER
    }
    return false;
}

void SendThingsBoardTelemetry()
{
    if (!WiFi.isConnected())
        return;

    WiFiClient client;
    HTTPClient http;

    String url = "http://";
    url += THINGSBOARD_HOST;
    url += "/api/v1/";
    url += THINGSBOARD_TOKEN;
    url += "/telemetry ";

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    uint8_t size = sizeof(Tele) / sizeof(Tele[0]);

    for (uint8_t i = 0; i < size; i++)
    {
        payload += "\"" + String(Tele[i].key) + "\":\"" + String(Tele[i].value);
        if (i < size - 1)
            payload += "\",";
    }
    payload += "\"}";

    uint16_t httpCode = http.POST(payload);

    if (httpCode > 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("TB : HTTP %d - Sent %s"), httpCode, payload.c_str());
    }
    else
    {
        AddLog(LOG_LEVEL_ERROR, PSTR("TB : HTTP failed (%d)"), httpCode);
    }

    http.end();
}

void FetchThingsBoardRPC()
{
    if (!WiFi.isConnected())
        return;

    WiFiClient client;
    HTTPClient http;

    String url = "http://";
    url += THINGSBOARD_HOST;
    url += "/api/v1/";
    url += THINGSBOARD_TOKEN;
    url += "/rpc?timeout=1000";

    http.begin(client, url);

    uint16_t httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("aha"));
        String payload = http.getString();
        AddLog(LOG_LEVEL_DEBUG, PSTR("TB: RPC %s"), payload.c_str());

        JsonParser parser((char *)payload.c_str());
        JsonParserObject root = parser.getRootObject();
        String method = root[PSTR("method")].getStr();

        if (method == "setCT")
            LightSetColorTemp(root[PSTR("params")].getUInt());
    }
    // else if (httpCode != HTTP_CODE_NO_CONTENT)
    // {
    //     AddLog(LOG_LEVEL_ERROR, PSTR("TB : RPC HTTP %d"), httpCode);
    // }
    http.end();
}

/*void HandleTB(void)
{
    if (!HttpCheckPriviledgedAccess())
    {
        return;
    }
}
*/

#endif