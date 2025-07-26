#ifdef USE_THINGSBOARD_HTTP_LIGHTBULB

#define XDRV_100 100

/*const char HTTP_BTN_MENU_TB[] PROGMEM =
    "<p><form action='tb' method='get'><button>" D_CONFIGURE_MQTT "</button></form></p>";
*/

struct Telementary_data
{
    const char *key;
    char value[20];
    bool change;
};

struct Telementary_data Tele[] = {
    {"POWER", "", false},
    {"Color", "", false},
    {"CT", "", false}};

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
        Tele[0].change = true;
        break;
    }
    case FUNC_EVERY_SECOND:
    {
        char color_str[20];

        FetchThingsBoardRPC();

        LightGetColor(color_str, sizeof(color_str));

        if (strcmp(Tele[1].value, color_str) != 0)
        {
            snprintf_P(Tele[1].value, sizeof(Tele[1].value), PSTR("%s"), color_str);
            Tele[1].change = true;
            if (LightGetColorTemp() != 0)
            {
                snprintf_P(Tele[2].value, sizeof(Tele[2].value), PSTR("%d"), LightGetColorTemp());
                Tele[2].change = true;
            }
        }

        SendThingsBoardTelemetry();

        break;
    }
        // #ifdef USE_WEBSERVER
        //     // case FUNC_WEB_ADD_BUTTON:
        //     // {
        //     //     WSContentSend_P()
        //     //     break;
        //     // }
        //     // case FUNC_WEB_ADD_HANDLER:
        //     // {
        //     //     WebServer.on("/thingsboard", HandleTB);
        //     //     break;
        //     // }
        // #endif // USE_WEBSERVER
    }
    return false;
}

void SendThingsBoardTelemetry()
{
    if (!WiFi.isConnected())
        return;

    String payload = "{";
    uint8_t size = sizeof(Tele) / sizeof(Tele[0]);

    for (uint8_t i = 0; i < size; i++)
    {
        if (Tele[i].change == true)
        {
            if (payload != "{")
                payload += ",";

            payload += "\"" + String(Tele[i].key) + "\":\"" + String(Tele[i].value) + "\"";
            Tele[i].change = false;
        }
    }

    if (payload == "{")
        return;

    payload += "}";

    WiFiClient client;
    HTTPClient http;

    String url = "http://";
    url += THINGSBOARD_HOST;
    url += "/api/v1/";
    url += THINGSBOARD_TOKEN;
    url += "/telemetry";

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    uint16_t httpCode = http.POST(payload);

    if (httpCode > 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("TB : HTTP %d - Sent Telementary %s"), httpCode, payload.c_str());
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
    url += "/rpc?timeout=500";

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

        if (method == "setCT") // params: 153-500
            LightSetColorTemp(root[PSTR("params")].getUInt());
        else if (method == "setHue")
        { // params: 0-359
            uint16_t hue = root[PSTR("params")].getUInt();
            char cmd[24];
            snprintf(cmd, sizeof(cmd), "HsbColor1 %u", hue);
            ExecuteCommand(cmd, SRC_WEBGUI); // updates light & telemetry
        }
        else if (method == "setSaturation")
        { // params: 0-100
            uint8_t sat = root[PSTR("params")].getUInt();
            char cmd[24];
            snprintf(cmd, sizeof(cmd), "HsbColor2 %u", sat);
            ExecuteCommand(cmd, SRC_WEBGUI);
        }
        else if (method == "setDimmer")
        { // params: 0-100
            uint8_t dimm = root[PSTR("params")].getUInt();
            LightSetDimmer(dimm);
        }
        else if (method == "setPower")
        { // params: true/false or 1/0
            bool on = root[PSTR("params")].getBool();
            char cmd[12];
            snprintf(cmd, sizeof(cmd), "Power %u", on);
            ExecuteCommand(cmd, SRC_WEBGUI);
        }
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