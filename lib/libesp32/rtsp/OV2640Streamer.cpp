
#include "OV2640Streamer.h"
#include <assert.h>



OV2640Streamer::OV2640Streamer(SOCKET aClient, OV2640 &cam) : CStreamer(aClient, cam.getWidth(), cam.getHeight()), m_cam(cam)
{
//    Serial.printf("Created streamer width=%d, height=%d\n", cam.getWidth(), cam.getHeight());
}

void OV2640Streamer::streamImage(uint32_t curMsec)
{
    m_cam.run();// queue up a read for next time

    BufPtr bytes = m_cam.getfb();
    streamFrame(bytes, m_cam.getSize(), curMsec);
//    Serial.printf("get frame: %d\n",m_cam.getSize());
}
