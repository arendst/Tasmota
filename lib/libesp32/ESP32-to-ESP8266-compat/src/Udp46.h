/*
 *  Udp.cpp: Library to send/receive UDP packets.
 *
 * NOTE: UDP is fast, but has some important limitations (thanks to Warren Gray for mentioning these)
 * 1) UDP does not guarantee the order in which assembled UDP packets are received. This
 * might not happen often in practice, but in larger network topologies, a UDP
 * packet can be received out of sequence.
 * 2) UDP does not guard against lost packets - so packets *can* disappear without the sender being
 * aware of it. Again, this may not be a concern in practice on small local networks.
 * For more information, see http://www.cafeaulait.org/course/week12/35.html
 *
 * MIT License:
 * Copyright (c) 2008 Bjoern Hartmann
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * bjoern@cs.stanford.edu 12/30/2008
 */

#ifndef udp46_h
#define udp46_h

#include <Stream.h>
#include <IPAddress46.h>

class UDP46: public Stream
{

public:
    virtual uint8_t begin(uint16_t) =0;	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    virtual uint8_t beginMulticast(IPAddress46, uint16_t) { return 0; }  // initialize, start listening on specified multicast IP address and port. Returns 1 if successful, 0 on failure
    virtual void stop() =0;  // Finish with the UDP socket

    // Sending UDP packets

    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    virtual int beginPacket(IPAddress46 ip, uint16_t port) =0;
    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    virtual int beginPacket(const char *host, uint16_t port) =0;
    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    virtual int endPacket() =0;
    // Write a single byte into the packet
    virtual size_t write(uint8_t) =0;
    // Write size bytes from buffer into the packet
    virtual size_t write(const uint8_t *buffer, size_t size) =0;

    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    virtual int parsePacket() =0;
    // Number of bytes remaining in the current packet
    virtual int available() =0;
    // Read a single byte from the current packet
    virtual int read() =0;
    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    virtual int read(unsigned char* buffer, size_t len) =0;
    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    virtual int read(char* buffer, size_t len) =0;
    // Return the next byte from the current packet without moving on to the next byte
    virtual int peek() =0;
    virtual void flush() =0;	// Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    virtual IPAddress46 remoteIP() =0;
    // Return the port of the host who sent the current incoming packet
    virtual uint16_t remotePort() =0;
protected:
    uint8_t* rawIPAddress(IPAddress46& addr)
    {
        return addr.raw_address();
    }
};

#endif
