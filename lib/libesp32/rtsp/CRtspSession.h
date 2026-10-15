#pragma once

#include "CStreamer.h"
#include "platglue.h"

// supported command types
enum RTSP_CMD_TYPES
{
    RTSP_OPTIONS,
    RTSP_DESCRIBE,
    RTSP_SETUP,
    RTSP_PLAY,
    RTSP_TEARDOWN,
    RTSP_UNKNOWN
};

#define RTSP_BUFFER_SIZE       10000    // for incoming requests, and outgoing responses
#define RTSP_PARAM_STRING_MAX  200
#define MAX_HOSTNAME_LEN       256

class CRtspSession
{
public:
    CRtspSession(SOCKET aRtspClient, CStreamer * aStreamer);
    ~CRtspSession();

    RTSP_CMD_TYPES Handle_RtspRequest(char const * aRequest, unsigned aRequestSize);
    int            GetStreamID();

    /**
       Read from our socket, parsing commands as possible.

       return false if the read timed out
     */
    bool handleRequests(uint32_t readTimeoutMs);

    /**
       broadcast a current frame
     */
    void broadcastCurrentFrame(uint32_t curMsec);

    bool m_streaming;
    bool m_stopped;

private:
    void Init();
    bool ParseRtspRequest(char const * aRequest, unsigned aRequestSize);
    char const * DateHeader();

    // RTSP request command handlers
    void Handle_RtspOPTION();
    void Handle_RtspDESCRIBE();
    void Handle_RtspSETUP();
    void Handle_RtspPLAY();

    // global session state parameters
    int m_RtspSessionID;
    SOCKET m_RtspClient;                                      // RTSP socket of that session
    int m_StreamID;                                           // number of simulated stream of that session
    IPPORT m_ClientRTPPort;                                  // client port for UDP based RTP transport
    IPPORT m_ClientRTCPPort;                                 // client port for UDP based RTCP transport
    bool m_TcpTransport;                                      // if Tcp based streaming was activated
    CStreamer    * m_Streamer;                                // the UDP or TCP streamer of that session

    // parameters of the last received RTSP request

    RTSP_CMD_TYPES m_RtspCmdType;                             // command type (if any) of the current request
    char m_URLPreSuffix[RTSP_PARAM_STRING_MAX];               // stream name pre suffix
    char m_URLSuffix[RTSP_PARAM_STRING_MAX];                  // stream name suffix
    char m_CSeq[RTSP_PARAM_STRING_MAX];                       // RTSP command sequence number
    char m_URLHostPort[MAX_HOSTNAME_LEN];                     // host:port part of the URL
    unsigned m_ContentLength;                                 // SDP string size
    char CurRequest[RTSP_BUFFER_SIZE];
    char RecvBuf[RTSP_BUFFER_SIZE];
    char session_buf[128];
    char CmdName[RTSP_PARAM_STRING_MAX];
    char Transport[255];
    char Response[1024];
    char OBuf[256];
};
