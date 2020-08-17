/*
 * File: NTPPacket.h
 * Description:
 *   NTP packet representation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#ifndef NTP_PACKET_H
#define NTP_PACKET_H

#include "Arduino.h"


/*
 * Contains the data in a typical NTP packet.
 */
struct NtpPacket
{
    static const int PACKET_SIZE = 48;
    
    unsigned char leapVersionMode;
    
    unsigned int leapIndicator() const { return leapVersionMode >> 6; }
    void leapIndicator(unsigned int newValue) { leapVersionMode = (0x3F & leapVersionMode) | ((newValue & 0x03) << 6); }
    
    unsigned int versionNumber() const { return (leapVersionMode >> 3) & 0x07; }
    void versionNumber(unsigned int newValue) { leapVersionMode = (0xC7 & leapVersionMode) | ((newValue & 0x07) << 3); }
    
    unsigned int mode() const { return (leapVersionMode & 0x07); }
    void mode(unsigned int newValue) { leapVersionMode = (leapVersionMode & 0xF8) | (newValue & 0x07); }
    
    char stratum;
    char poll;
    char precision;
    uint32_t rootDelay;
    uint32_t rootDispersion;
    char referenceId[4];
    uint32_t referenceTimestampSeconds;
    uint32_t referenceTimestampFraction;
    uint32_t originTimestampSeconds;
    uint32_t originTimestampFraction;
    uint32_t receiveTimestampSeconds;
    uint32_t receiveTimestampFraction;
    uint32_t transmitTimestampSeconds;
    uint32_t transmitTimestampFraction;
    
    /*
     * Rearranges bytes in 32 bit values from big-endian (NTP protocol)
     * to little-endian (Arduino/PC), or vice versa. Must be called before 
     * modifying the structure or sending the packet.
     */
    void swapEndian();

    /*
     * Returns packet as a char array for transmission via network.
     * WARNING: modifying the return value is unsafe.
     */
    const char *packet() { return (const char*)this; }
    
    /*
     * Copies packet buffer to packet object.
     */
    void populatePacket(const char *buffer)
    {
        memcpy(this, buffer, PACKET_SIZE);
    }
private:
    /*
     * Reverses bytes in a number.
     */
    void reverseBytes_(uint32_t *number);
};

#endif // NTP_PACKET_H
