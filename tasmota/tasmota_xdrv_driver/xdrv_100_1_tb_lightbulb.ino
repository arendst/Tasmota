#ifdef USE_TB_LIGHTBULB

Tele = new Telementary_data[3]{
    {"POWER", "", false},
    {"Color", "", false},
    {"CT", "", false}};

#define XDRV_100 100

bool Xdrv100(uint32_t function)
{
    switch (function)
    {
    case FUNC_INIT:
        AddLog(LOG_LEVEL_INFO, PSTR("TB : ThingsBoard HTTP Initialized"));
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
    }
    return false;
}

#endif // USE_TB_LIGHTBULB