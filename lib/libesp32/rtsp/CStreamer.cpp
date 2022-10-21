#include "CStreamer.h"

#include <stdio.h>

//#define STREAM_DEBUG

CStreamer::CStreamer(SOCKET aClient, u_short width, u_short height) : m_Client(aClient)
{
    printf("Creating TSP streamer\n");
    m_RtpServerPort  = 0;
    m_RtcpServerPort = 0;
    m_RtpClientPort  = 0;
    m_RtcpClientPort = 0;

    m_SequenceNumber = 0;
    m_Timestamp      = 0;
    m_SendIdx        = 0;
    m_TCPTransport   = false;

    m_RtpSocket = NULLSOCKET;
    m_RtcpSocket = NULLSOCKET;

    m_width = width;
    m_height = height;
    m_prevMsec = 0;
};

CStreamer::~CStreamer()
{
    udpsocketclose(m_RtpSocket);
    udpsocketclose(m_RtcpSocket);
};

int CStreamer::SendRtpPacket(unsigned const char * jpeg, int jpegLen, int fragmentOffset, BufPtr quant0tbl, BufPtr quant1tbl)
{
#define KRtpHeaderSize 12           // size of the RTP header
#define KJpegHeaderSize 8           // size of the special JPEG payload header

#define MAX_FRAGMENT_SIZE 1100 // FIXME, pick more carefully
    int fragmentLen = MAX_FRAGMENT_SIZE;
    if(fragmentLen + fragmentOffset > jpegLen) // Shrink last fragment if needed
        fragmentLen = jpegLen - fragmentOffset;

    bool isLastFragment = (fragmentOffset + fragmentLen) == jpegLen;

    // Do we have custom quant tables? If so include them per RFC

    bool includeQuantTbl = quant0tbl && quant1tbl && fragmentOffset == 0;
    uint8_t q = includeQuantTbl ? 128 : 0x5e;

    //static char RtpBuf[2048]; // Note: we assume single threaded, this large buf we keep off of the tiny stack
    int RtpPacketSize = fragmentLen + KRtpHeaderSize + KJpegHeaderSize + (includeQuantTbl ? (4 + 64 * 2) : 0);

    memset(RtpBuf,0x00,sizeof(RtpBuf));
    // Prepare the first 4 byte of the packet. This is the Rtp over Rtsp header in case of TCP based transport
    RtpBuf[0]  = '$';        // magic number
    RtpBuf[1]  = 0;          // number of multiplexed subchannel on RTPS connection - here the RTP channel
    RtpBuf[2]  = (RtpPacketSize & 0x0000FF00) >> 8;
    RtpBuf[3]  = (RtpPacketSize & 0x000000FF);
    // Prepare the 12 byte RTP header
    RtpBuf[4]  = 0x80;                               // RTP version
    RtpBuf[5]  = 0x1a | (isLastFragment ? 0x80 : 0x00);                               // JPEG payload (26) and marker bit
    RtpBuf[7]  = m_SequenceNumber & 0x0FF;           // each packet is counted with a sequence counter
    RtpBuf[6]  = m_SequenceNumber >> 8;
    RtpBuf[8]  = (m_Timestamp & 0xFF000000) >> 24;   // each image gets a timestamp
    RtpBuf[9]  = (m_Timestamp & 0x00FF0000) >> 16;
    RtpBuf[10] = (m_Timestamp & 0x0000FF00) >> 8;
    RtpBuf[11] = (m_Timestamp & 0x000000FF);
    RtpBuf[12] = 0x13;                               // 4 byte SSRC (sychronization source identifier)
    RtpBuf[13] = 0xf9;                               // we just an arbitrary number here to keep it simple
    RtpBuf[14] = 0x7e;
    RtpBuf[15] = 0x67;

    // Prepare the 8 byte payload JPEG header
    RtpBuf[16] = 0x00;                               // type specific
    RtpBuf[17] = (fragmentOffset & 0x00FF0000) >> 16;                               // 3 byte fragmentation offset for fragmented images
    RtpBuf[18] = (fragmentOffset & 0x0000FF00) >> 8;
    RtpBuf[19] = (fragmentOffset & 0x000000FF);

    /*    These sampling factors indicate that the chrominance components of
       type 0 video is downsampled horizontally by 2 (often called 4:2:2)
       while the chrominance components of type 1 video are downsampled both
       horizontally and vertically by 2 (often called 4:2:0). */
    RtpBuf[20] = 0x00;                               // type (fixme might be wrong for camera data) https://tools.ietf.org/html/rfc2435
    RtpBuf[21] = q;                               // quality scale factor was 0x5e
    RtpBuf[22] = m_width / 8;                           // width  / 8
    RtpBuf[23] = m_height / 8;                           // height / 8

    int headerLen = 24; // Inlcuding jpeg header but not qant table header
    if(includeQuantTbl) { // we need a quant header - but only in first packet of the frame
        //printf("inserting quanttbl\n");
        RtpBuf[24] = 0; // MBZ
        RtpBuf[25] = 0; // 8 bit precision
        RtpBuf[26] = 0; // MSB of lentgh

        int numQantBytes = 64; // Two 64 byte tables
        RtpBuf[27] = 2 * numQantBytes; // LSB of length

        headerLen += 4;

        memcpy(RtpBuf + headerLen, quant0tbl, numQantBytes);
        headerLen += numQantBytes;

        memcpy(RtpBuf + headerLen, quant1tbl, numQantBytes);
        headerLen += numQantBytes;
    }
    // printf("Sending timestamp %d, seq %d, fragoff %d, fraglen %d, jpegLen %d\n", m_Timestamp, m_SequenceNumber, fragmentOffset, fragmentLen, jpegLen);

    // append the JPEG scan data to the RTP buffer
    memcpy(RtpBuf + headerLen,jpeg + fragmentOffset, fragmentLen);
    fragmentOffset += fragmentLen;

    m_SequenceNumber++;                              // prepare the packet counter for the next packet

    IPADDRESS otherip;
    IPPORT otherport;
    socketpeeraddr(m_Client, &otherip, &otherport);

    // RTP marker bit must be set on last fragment
    if (m_TCPTransport) // RTP over RTSP - we send the buffer + 4 byte additional header
        socketsend(m_Client,RtpBuf,RtpPacketSize + 4);
    else                // UDP - we send just the buffer by skipping the 4 byte RTP over RTSP header
        udpsocketsend(m_RtpSocket,&RtpBuf[4],RtpPacketSize, otherip, m_RtpClientPort);

    return isLastFragment ? 0 : fragmentOffset;
};

void CStreamer::InitTransport(u_short aRtpPort, u_short aRtcpPort, bool TCP)
{
    m_RtpClientPort  = aRtpPort;
    m_RtcpClientPort = aRtcpPort;
    m_TCPTransport   = TCP;

    if (!m_TCPTransport)
    {   // allocate port pairs for RTP/RTCP ports in UDP transport mode
        for (u_short P = 6970; P < 0xFFFE; P += 2)
        {
            m_RtpSocket     = udpsocketcreate(P);
            if (m_RtpSocket)
            {   // Rtp socket was bound successfully. Lets try to bind the consecutive Rtsp socket
                m_RtcpSocket = udpsocketcreate(P + 1);
                if (m_RtcpSocket)
                {
                    m_RtpServerPort  = P;
                    m_RtcpServerPort = P+1;
                    break;
                }
                else
                {
                    udpsocketclose(m_RtpSocket);
                    udpsocketclose(m_RtcpSocket);
                };
            }
        };
    };
};

u_short CStreamer::GetRtpServerPort()
{
    return m_RtpServerPort;
};

u_short CStreamer::GetRtcpServerPort()
{
    return m_RtcpServerPort;
};

void CStreamer::streamFrame(unsigned const char *data, uint32_t dataLen, uint32_t curMsec)
{
    if(m_prevMsec == 0) // first frame init our timestamp
        m_prevMsec = curMsec;

    // compute deltat (being careful to handle clock rollover with a little lie)
    uint32_t deltams = (curMsec >= m_prevMsec) ? curMsec - m_prevMsec : 100;
    m_prevMsec = curMsec;

    // locate quant tables if possible
    BufPtr qtable0, qtable1;

    if (!decodeJPEGfile(&data, &dataLen, &qtable0, &qtable1)) {
#ifdef STREAM_DEBUG
        printf("can't decode jpeg data\n");
#endif
        return;
    }

    int offset = 0;
    do {
        offset = SendRtpPacket(data, dataLen, offset, qtable0, qtable1);
    } while(offset != 0);

    // Increment ONLY after a full frame
    uint32_t units = 90000; // Hz per RFC 2435
    m_Timestamp += (units * deltams / 1000);                             // fixed timestamp increment for a frame rate of 25fps

    m_SendIdx++;
    if (m_SendIdx > 1) m_SendIdx = 0;

#ifdef STREAM_DEBUG
    printf("frame sent\n");
#endif
};

#include <assert.h>

// search for a particular JPEG marker, moves *start to just after that marker
// This function fixes up the provided start ptr to point to the
// actual JPEG stream data and returns the number of bytes skipped
// APP0 e0
// DQT db
// DQT db
// DHT c4
// DHT c4
// DHT c4
// DHT c4
// SOF0 c0 baseline (not progressive) 3 color 0x01 Y, 0x21 2h1v, 0x00 tbl0
// - 0x02 Cb, 0x11 1h1v, 0x01 tbl1 - 0x03 Cr, 0x11 1h1v, 0x01 tbl1
// therefore 4:2:2, with two separate quant tables (0 and 1)
// SOS da
// EOI d9 (no need to strip data after this RFC says client will discard)
bool findJPEGheader(BufPtr *start, uint32_t *len, uint8_t marker) {
    // per https://en.wikipedia.org/wiki/JPEG_File_Interchange_Format
    unsigned const char *bytes = *start;

    // kinda skanky, will break if unlucky and the headers inxlucde 0xffda
    // might fall off array if jpeg is invalid
    // FIXME - return false instead
    while(bytes - *start < *len) {
        uint8_t framing = *bytes++; // better be 0xff
        if(framing != 0xff) {
            printf("malformed jpeg, framing=%x\n", framing);
            return false;
        }
        uint8_t typecode = *bytes++;
        if(typecode == marker) {
            unsigned skipped = bytes - *start;
            //printf("found marker 0x%x, skipped %d\n", marker, skipped);

            *start = bytes;

            // shrink len for the bytes we just skipped
            *len -= skipped;

            return true;
        }
        else {
            // not the section we were looking for, skip the entire section
            switch(typecode) {
            case 0xd8:     // start of image
            {
                break;   // no data to skip
            }
            case 0xe0:   // app0
            case 0xdb:   // dqt
            case 0xc4:   // dht
            case 0xc0:   // sof0
            case 0xda:   // sos
            {
                // standard format section with 2 bytes for len.  skip that many bytes
                uint32_t len = bytes[0] * 256 + bytes[1];
                //printf("skipping section 0x%x, %d bytes\n", typecode, len);
                bytes += len;
                break;
            }
            default:
                printf("unexpected jpeg typecode 0x%x\n", typecode);
                break;
            }
        }
    }

    printf("failed to find jpeg marker 0x%x", marker);
    return false;
}

// the scan data uses byte stuffing to guarantee anything that starts with 0xff
// followed by something not zero, is a new section.  Look for that marker and return the ptr
// pointing there
void skipScanBytes(BufPtr *start) {
    BufPtr bytes = *start;

    while(true) { // FIXME, check against length
        while(*bytes++ != 0xff);
        if(*bytes++ != 0) {
            *start = bytes - 2; // back up to the 0xff marker we just found
            return;
        }
    }
}
void  nextJpegBlock(BufPtr *bytes) {
    uint32_t len = (*bytes)[0] * 256 + (*bytes)[1];
    //printf("going to next jpeg block %d bytes\n", len);
    *bytes += len;
}

// When JPEG is stored as a file it is wrapped in a container
// This function fixes up the provided start ptr to point to the
// actual JPEG stream data and returns the number of bytes skipped
bool decodeJPEGfile(BufPtr *start, uint32_t *len, BufPtr *qtable0, BufPtr *qtable1) {
    // per https://en.wikipedia.org/wiki/JPEG_File_Interchange_Format
    unsigned const char *bytes = *start;

    if(!findJPEGheader(&bytes, len, 0xd8)) // better at least look like a jpeg file
        return false; // FAILED!

    // Look for quant tables if they are present
    *qtable0 = NULL;
    *qtable1 = NULL;
    BufPtr quantstart = *start;
    uint32_t quantlen = *len;
    if(!findJPEGheader(&quantstart, &quantlen, 0xdb)) {
        printf("error can't find quant table 0\n");
    }
    else {
        // printf("found quant table %x\n", quantstart[2]);

        *qtable0 = quantstart + 3;     // 3 bytes of header skipped
        nextJpegBlock(&quantstart);
        if(!findJPEGheader(&quantstart, &quantlen, 0xdb)) {
            printf("error can't find quant table 1\n");
        }
        else {
            // printf("found quant table %x\n", quantstart[2]);
        }
        *qtable1 = quantstart + 3;
        nextJpegBlock(&quantstart);
    }

    if(!findJPEGheader(start, len, 0xda))
        return false; // FAILED!

    // Skip the header bytes of the SOS marker FIXME why doesn't this work?
    uint32_t soslen = (*start)[0] * 256 + (*start)[1];
    *start += soslen;
    *len -= soslen;

    // start scanning the data portion of the scan to find the end marker
    BufPtr endmarkerptr = *start;
    uint32_t endlen = *len;

    skipScanBytes(&endmarkerptr);
    if(!findJPEGheader(&endmarkerptr, &endlen, 0xd9))
        return false; // FAILED!

    // endlen must now be the # of bytes between the start of our scan and
    // the end marker, tell the caller to ignore bytes afterwards
    *len = endmarkerptr - *start;

    return true;
}
