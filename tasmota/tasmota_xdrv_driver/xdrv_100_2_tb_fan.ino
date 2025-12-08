#ifdef USE_TB_FAN

#define XDRV_100 100

bool Xdrv100(uint32_t function)
{
    switch (function)
    {
    case FUNC_INIT:
        // inițializări PWM/tach aici
        break;

    case FUNC_EVERY_SECOND:
        // măsori RPM la fiecare secundă, dacă vrei
        break;

    case FUNC_COMMAND:
        // interpretezi comenzi de tip "FanSpeed" etc. trimise din consolă / MQTT / HTTP
        break;
    }
    return false;
}

#endif // USE_TB_FAN