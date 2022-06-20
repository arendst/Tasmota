/*
  DnsClient.h - DNS client for Arduino

  SPDX-FileCopyrightText: 2009-2010 MCQN Ltd. and Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

// Arduino DNS client for WizNet5100-based Ethernet shield
// (c) Copyright 2009-2010 MCQN Ltd.
// Released under Apache License, version 2.0

#ifndef DNSClient_h
#define DNSClient_h

#include <Arduino.h>
#include <IPAddress.h>
#include <WiFiUdp.h>

class DNSClient {
public:
  void begin(const IPAddress& aDNSServer);
  void setTimeout(uint32_t aTimeout = 1000);

  /* Resolve the given hostname to an IP address.
     @param aHostname Name to be resolved
     @param aResult IPAddress structure to store the returned IP address
     @result 1 if aIPAddrString was successfully converted to an IP address, else error code
  */
  int getHostByName(const char* aHostname, IPAddress& aResult);

protected:
  int BuildRequest(const char* aName);
  int ProcessResponse(uint32_t aTimeout, IPAddress& aAddress);

  IPAddress iDNSServer;
  uint16_t iRequestId;
  uint16_t iTimeout = 1000;
  WiFiUDP iUdp;
};

#endif
