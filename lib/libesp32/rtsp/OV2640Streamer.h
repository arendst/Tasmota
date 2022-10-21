#pragma once

#include "CStreamer.h"
#include "OV2640.h"

class OV2640Streamer : public CStreamer
{
    bool m_showBig;
    OV2640 &m_cam;

public:
    OV2640Streamer(SOCKET aClient, OV2640 &cam);

    virtual void    streamImage(uint32_t curMsec);
};
