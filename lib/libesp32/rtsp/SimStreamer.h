#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3

#pragma once

#include "JPEGSamples.h"
#include "CStreamer.h"

#ifdef INCLUDE_SIMDATA
class SimStreamer : public CStreamer
{
    bool m_showBig;
public:
    SimStreamer(SOCKET aClient, bool showBig);

    virtual void    streamImage(uint32_t curMsec);
};
#endif

#endif  // CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3