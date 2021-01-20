#include "CRtspSession.h"
#include <stdio.h>
#include <time.h>

CRtspSession::CRtspSession(SOCKET aRtspClient, CStreamer * aStreamer) : m_RtspClient(aRtspClient),m_Streamer(aStreamer)
{
    printf("Creating RTSP session\n");
    Init();

    m_RtspSessionID  = getRandom();         // create a session ID
    m_RtspSessionID |= 0x80000000;
    m_StreamID       = -1;
    m_ClientRTPPort  =  0;
    m_ClientRTCPPort =  0;
    m_TcpTransport   =  false;
    m_streaming = false;
    m_stopped = false;
};

CRtspSession::~CRtspSession()
{
    closesocket(m_RtspClient);
};

void CRtspSession::Init()
{
    m_RtspCmdType   = RTSP_UNKNOWN;
    memset(m_URLPreSuffix, 0x00, sizeof(m_URLPreSuffix));
    memset(m_URLSuffix,    0x00, sizeof(m_URLSuffix));
    memset(m_CSeq,         0x00, sizeof(m_CSeq));
    memset(m_URLHostPort,  0x00, sizeof(m_URLHostPort));
    m_ContentLength  =  0;
};

bool CRtspSession::ParseRtspRequest(char const * aRequest, unsigned aRequestSize)
{
    // char CmdName[RTSP_PARAM_STRING_MAX];
    //char CurRequest[RTSP_BUFFER_SIZE]; // Note: we assume single threaded, this large buf we keep off of the tiny stack
    unsigned CurRequestSize;

    Init();
    CurRequestSize = aRequestSize;
    memcpy(CurRequest,aRequest,aRequestSize);

    // check whether the request contains information about the RTP/RTCP UDP client ports (SETUP command)
    char * ClientPortPtr;
    char * TmpPtr;
    char CP[128]; //static char CP[1024];
    char * pCP;

    ClientPortPtr = strstr(CurRequest,"client_port");
    if (ClientPortPtr != nullptr)
    {
        TmpPtr = strstr(ClientPortPtr,"\r\n");
        if (TmpPtr != nullptr)
        {
            TmpPtr[0] = 0x00;
            strcpy(CP,ClientPortPtr);
            pCP = strstr(CP,"=");
            if (pCP != nullptr)
            {
                pCP++;
                strcpy(CP,pCP);
                pCP = strstr(CP,"-");
                if (pCP != nullptr)
                {
                    pCP[0] = 0x00;
                    m_ClientRTPPort  = atoi(CP);
                    m_ClientRTCPPort = m_ClientRTPPort + 1;
                };
            };
        };
    };

    // Read everything up to the first space as the command name
    bool parseSucceeded = false;
    unsigned i;
    for (i = 0; i < sizeof(CmdName)-1 && i < CurRequestSize; ++i)
    {
        char c = CurRequest[i];
        if (c == ' ' || c == '\t')
        {
            parseSucceeded = true;
            break;
        }
        CmdName[i] = c;
    }
    CmdName[i] = '\0';
    if (!parseSucceeded) {
        printf("failed to parse RTSP\n");
        return false;
    }

    printf("RTSP received %s\n", CmdName);

    // find out the command type
    if (strstr(CmdName,"OPTIONS")   != nullptr) m_RtspCmdType = RTSP_OPTIONS; else
    if (strstr(CmdName,"DESCRIBE")  != nullptr) m_RtspCmdType = RTSP_DESCRIBE; else
    if (strstr(CmdName,"SETUP")     != nullptr) m_RtspCmdType = RTSP_SETUP; else
    if (strstr(CmdName,"PLAY")      != nullptr) m_RtspCmdType = RTSP_PLAY; else
    if (strstr(CmdName,"TEARDOWN")  != nullptr) m_RtspCmdType = RTSP_TEARDOWN;

    // check whether the request contains transport information (UDP or TCP)
    if (m_RtspCmdType == RTSP_SETUP)
    {
        TmpPtr = strstr(CurRequest,"RTP/AVP/TCP");
        if (TmpPtr != nullptr) m_TcpTransport = true; else m_TcpTransport = false;
    };

    // Skip over the prefix of any "rtsp://" or "rtsp:/" URL that follows:
    unsigned j = i+1;
    while (j < CurRequestSize && (CurRequest[j] == ' ' || CurRequest[j] == '\t')) ++j; // skip over any additional white space
    for (; (int)j < (int)(CurRequestSize-8); ++j)
    {
        if ((CurRequest[j]   == 'r' || CurRequest[j]   == 'R')   &&
            (CurRequest[j+1] == 't' || CurRequest[j+1] == 'T') &&
            (CurRequest[j+2] == 's' || CurRequest[j+2] == 'S') &&
            (CurRequest[j+3] == 'p' || CurRequest[j+3] == 'P') &&
            CurRequest[j+4] == ':' && CurRequest[j+5] == '/')
        {
            j += 6;
            if (CurRequest[j] == '/')
            {   // This is a "rtsp://" URL; skip over the host:port part that follows:
                ++j;
                unsigned uidx = 0;
                while (j < CurRequestSize && CurRequest[j] != '/' && CurRequest[j] != ' ' && uidx < sizeof(m_URLHostPort) - 1)
                {   // extract the host:port part of the URL here
                    m_URLHostPort[uidx] = CurRequest[j];
                    uidx++;
                    ++j;
                };
            }
            else --j;
            i = j;
            break;
        }
    }

    // Look for the URL suffix (before the following "RTSP/"):
    parseSucceeded = false;
    for (unsigned k = i+1; (int)k < (int)(CurRequestSize-5); ++k)
    {
        if (CurRequest[k]   == 'R'   && CurRequest[k+1] == 'T'   &&
            CurRequest[k+2] == 'S'   && CurRequest[k+3] == 'P'   &&
            CurRequest[k+4] == '/')
        {
            while (--k >= i && CurRequest[k] == ' ') {}
            unsigned k1 = k;
            while (k1 > i && CurRequest[k1] != '/') --k1;
            if (k - k1 + 1 > sizeof(m_URLSuffix)) return false;
            unsigned n = 0, k2 = k1+1;

            while (k2 <= k) m_URLSuffix[n++] = CurRequest[k2++];
            m_URLSuffix[n] = '\0';

            if (k1 - i > sizeof(m_URLPreSuffix)) return false;
            n = 0; k2 = i + 1;
            while (k2 <= k1 - 1) m_URLPreSuffix[n++] = CurRequest[k2++];
            m_URLPreSuffix[n] = '\0';
            i = k + 7;
            parseSucceeded = true;
            break;
        }
    }
    if (!parseSucceeded) return false;

    // Look for "CSeq:", skip whitespace, then read everything up to the next \r or \n as 'CSeq':
    parseSucceeded = false;
    for (j = i; (int)j < (int)(CurRequestSize-5); ++j)
    {
        if (CurRequest[j]   == 'C' && CurRequest[j+1] == 'S' &&
            CurRequest[j+2] == 'e' && CurRequest[j+3] == 'q' &&
            CurRequest[j+4] == ':')
        {
            j += 5;
            while (j < CurRequestSize && (CurRequest[j] ==  ' ' || CurRequest[j] == '\t')) ++j;
            unsigned n;
            for (n = 0; n < sizeof(m_CSeq)-1 && j < CurRequestSize; ++n,++j)
            {
                char c = CurRequest[j];
                if (c == '\r' || c == '\n')
                {
                    parseSucceeded = true;
                    break;
                }
                m_CSeq[n] = c;
            }
            m_CSeq[n] = '\0';
            break;
        }
    }
    if (!parseSucceeded) return false;

    // Also: Look for "Content-Length:" (optional)
    for (j = i; (int)j < (int)(CurRequestSize-15); ++j)
    {
        if (CurRequest[j]    == 'C'  && CurRequest[j+1]  == 'o'  &&
            CurRequest[j+2]  == 'n'  && CurRequest[j+3]  == 't'  &&
            CurRequest[j+4]  == 'e'  && CurRequest[j+5]  == 'n'  &&
            CurRequest[j+6]  == 't'  && CurRequest[j+7]  == '-'  &&
            (CurRequest[j+8] == 'L' || CurRequest[j+8]   == 'l') &&
            CurRequest[j+9]  == 'e'  && CurRequest[j+10] == 'n' &&
            CurRequest[j+11] == 'g' && CurRequest[j+12]  == 't' &&
            CurRequest[j+13] == 'h' && CurRequest[j+14] == ':')
        {
            j += 15;
            while (j < CurRequestSize && (CurRequest[j] ==  ' ' || CurRequest[j] == '\t')) ++j;
            unsigned num;
            if (sscanf(&CurRequest[j], "%u", &num) == 1) m_ContentLength = num;
        }
    }
    return true;
};

RTSP_CMD_TYPES CRtspSession::Handle_RtspRequest(char const * aRequest, unsigned aRequestSize)
{
    if (ParseRtspRequest(aRequest,aRequestSize))
    {
        switch (m_RtspCmdType)
        {
        case RTSP_OPTIONS:  { Handle_RtspOPTION();   break; };
        case RTSP_DESCRIBE: { Handle_RtspDESCRIBE(); break; };
        case RTSP_SETUP:    { Handle_RtspSETUP();    break; };
        case RTSP_PLAY:     { Handle_RtspPLAY();     break; };
        default: {};
        };
    };
    return m_RtspCmdType;
};

void CRtspSession::Handle_RtspOPTION()
{
    //static char Response[1024]; // Note: we assume single threaded, this large buf we keep off of the tiny stack

    snprintf(Response,sizeof(Response),
             "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
             "Public: DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE\r\n\r\n",m_CSeq);

    socketsend(m_RtspClient,Response,strlen(Response));
}

void CRtspSession::Handle_RtspDESCRIBE()
{
    //static char Response[1024]; // Note: we assume single threaded, this large buf we keep off of the tiny stack
    char SDPBuf[128]; //static char SDPBuf[1024];
    char URLBuf[128];  //static char URLBuf[1024];

    // check whether we know a stream with the URL which is requested
    m_StreamID = -1;        // invalid URL
    if ((strcmp(m_URLPreSuffix,"mjpeg") == 0) && (strcmp(m_URLSuffix,"1") == 0)) m_StreamID = 0; else
    if ((strcmp(m_URLPreSuffix,"mjpeg") == 0) && (strcmp(m_URLSuffix,"2") == 0)) m_StreamID = 1;
    if (m_StreamID == -1)
    {   // Stream not available
        snprintf(Response,sizeof(Response),
                 "RTSP/1.0 404 Stream Not Found\r\nCSeq: %s\r\n%s\r\n",
                 m_CSeq,
                 DateHeader());

        socketsend(m_RtspClient,Response,strlen(Response));
        return;
    };

    // simulate DESCRIBE server response
    // static char OBuf[256];
    char * ColonPtr;
    strcpy(OBuf,m_URLHostPort);
    ColonPtr = strstr(OBuf,":");
    if (ColonPtr != nullptr) ColonPtr[0] = 0x00;

    snprintf(SDPBuf,sizeof(SDPBuf),
             "v=0\r\n"
             "o=- %d 1 IN IP4 %s\r\n"
             "s=\r\n"
             "t=0 0\r\n"                                       // start / stop - 0 -> unbounded and permanent session
             "m=video 0 RTP/AVP 26\r\n"                        // currently we just handle UDP sessions
             // "a=x-dimensions: 640,480\r\n"
             "c=IN IP4 0.0.0.0\r\n",
             rand(),
             OBuf);
    char StreamName[64];
    switch (m_StreamID)
    {
    case 0: strcpy(StreamName,"mjpeg/1"); break;
    case 1: strcpy(StreamName,"mjpeg/2"); break;
    };
    snprintf(URLBuf,sizeof(URLBuf),
             "rtsp://%s/%s",
             m_URLHostPort,
             StreamName);
    snprintf(Response,sizeof(Response),
             "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
             "%s\r\n"
             "Content-Base: %s/\r\n"
             "Content-Type: application/sdp\r\n"
             "Content-Length: %d\r\n\r\n"
             "%s",
             m_CSeq,
             DateHeader(),
             URLBuf,
             (int) strlen(SDPBuf),
             SDPBuf);

    socketsend(m_RtspClient,Response,strlen(Response));
}

void CRtspSession::Handle_RtspSETUP()
{
    //static char Response[1024];
    //static char Transport[255];

    // init RTP streamer transport type (UDP or TCP) and ports for UDP transport
    m_Streamer->InitTransport(m_ClientRTPPort,m_ClientRTCPPort,m_TcpTransport);

    // simulate SETUP server response
    if (m_TcpTransport)
        snprintf(Transport,sizeof(Transport),"RTP/AVP/TCP;unicast;interleaved=0-1");
    else
        snprintf(Transport,sizeof(Transport),
                 "RTP/AVP;unicast;destination=127.0.0.1;source=127.0.0.1;client_port=%i-%i;server_port=%i-%i",
                 m_ClientRTPPort,
                 m_ClientRTCPPort,
                 m_Streamer->GetRtpServerPort(),
                 m_Streamer->GetRtcpServerPort());
    snprintf(Response,sizeof(Response),
             "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
             "%s\r\n"
             "Transport: %s\r\n"
             "Session: %i\r\n\r\n",
             m_CSeq,
             DateHeader(),
             Transport,
             m_RtspSessionID);

    socketsend(m_RtspClient,Response,strlen(Response));
}

void CRtspSession::Handle_RtspPLAY()
{
    //static char Response[1024];

    // simulate SETUP server response
    snprintf(Response,sizeof(Response),
             "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
             "%s\r\n"
             "Range: npt=0.000-\r\n"
             "Session: %i\r\n"
             "RTP-Info: url=rtsp://127.0.0.1:8554/mjpeg/1/track1\r\n\r\n",
             m_CSeq,
             DateHeader(),
             m_RtspSessionID);

    socketsend(m_RtspClient,Response,strlen(Response));
}

char const * CRtspSession::DateHeader()
{
    //static char buf[200];
    time_t tt = time(NULL);
    strftime(session_buf, sizeof(session_buf), "Date: %a, %b %d %Y %H:%M:%S GMT", gmtime(&tt));
    return session_buf;
}

int CRtspSession::GetStreamID()
{
    return m_StreamID;
};



/**
   Read from our socket, parsing commands as possible.
 */
bool CRtspSession::handleRequests(uint32_t readTimeoutMs)
{
    if(m_stopped)
        return false; // Already closed down

    //char RecvBuf[RTSP_BUFFER_SIZE];   // Note: we assume single threaded, this large buf we keep off of the tiny stack

    memset(RecvBuf,0x00,sizeof(RecvBuf));
    int res = socketread(m_RtspClient,RecvBuf,sizeof(RecvBuf), readTimeoutMs);
    if(res > 0) {
        // we filter away everything which seems not to be an RTSP command: O-ption, D-escribe, S-etup, P-lay, T-eardown
        if ((RecvBuf[0] == 'O') || (RecvBuf[0] == 'D') || (RecvBuf[0] == 'S') || (RecvBuf[0] == 'P') || (RecvBuf[0] == 'T'))
        {
            RTSP_CMD_TYPES C = Handle_RtspRequest(RecvBuf,res);
            if (C == RTSP_PLAY)
                m_streaming = true;
            else if (C == RTSP_TEARDOWN)
                m_stopped = true;
        }
        return true;
    }
    else if(res == 0) {
        printf("client closed socket, exiting\n");
        m_stopped = true;
        return true;
    }
    else  {
        // Timeout on read

        return false;
    }
}

void CRtspSession::broadcastCurrentFrame(uint32_t curMsec) {
    // Send a frame
    if (m_streaming && !m_stopped) {
        // printf("serving a frame\n");
        m_Streamer->streamImage(curMsec);
    }
}
