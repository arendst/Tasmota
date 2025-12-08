#ifdef USE_THINGSBOARD

struct Telementary_data
{
    const char *key;
    char value[20];
    bool change;
};

Telementary_data *Tele = nullptr;

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
    url += "/rpc?timeout=5000&limit=5";

    http.begin(client, url);

    uint16_t httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();
        http.end();

        JsonParser parser((char *)payload.c_str());
        JsonParserObject root = parser.getRootObject();
        String method = root[PSTR("method")].getStr();

        AddLog(LOG_LEVEL_INFO, PSTR("TB : RPC %s"), method.c_str());

        if (method == "setCT")
        { // params: 153-500
            LightSetColorTemp(root[PSTR("params")].getUInt());
            LightPreparePower(2);
        }
        else if (method == "setHue")
        { // params: 0-359
            uint16_t hue = root[PSTR("params")].getUInt();
            char cmd[30];
            snprintf(cmd, sizeof(cmd), "HsbColor1 %u", hue);
            ExecuteCommand(cmd, SRC_WEBGUI); // updates light & telemetry
        }
        else if (method == "setSaturation")
        { // params: 0-100
            uint8_t sat = root[PSTR("params")].getUInt();
            char cmd[30];
            snprintf(cmd, sizeof(cmd), "HsbColor2 %u", sat);
            ExecuteCommand(cmd, SRC_WEBGUI);
        }
        else if (method == "setDimmer")
        { // params: 0-100
            uint8_t dimm = root[PSTR("params")].getUInt();
            LightSetDimmer(dimm);
            LightPreparePower(2);
        }
        else if (method == "setPower")
        { // params: true/false or 1/0
            bool on = root[PSTR("params")].getBool();
            char cmd[30];
            snprintf(cmd, sizeof(cmd), "Power %u", on);
            ExecuteCommand(cmd, SRC_WEBGUI);
        }
    }
    http.end();
}

#endif // USE_THINGSBOARD