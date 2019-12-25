/*
 * File: NTPServer.h
 * Description:
 *   NTP server implementation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#ifndef NTP_SERVER_H
#define NTP_SERVER_H

class NtpServer
{
public:
    NtpServer(WiFiUDP Port)
    {
        timeServerPort_=Port;
    }
    
    /*
     * Begins listening for NTP requests.
     */
    bool beginListening(void);
    
    
    /*
     * Processes a single NTP request.
     */
    bool processOneRequest(uint32_t utc, uint32_t millisecs);
    
private:
    WiFiUDP timeServerPort_;
};

#endif // NTP_SERVER_H
