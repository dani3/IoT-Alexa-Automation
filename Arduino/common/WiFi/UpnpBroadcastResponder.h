/** @file UpnpBroadcastResponder.h
 *  @brief Class that takes care of the multicasting to be able to
 *         set up every device with Alexa.
 *
 *  @author Daniel Mancebo
 *  @bug No known bugs.
 */

#ifndef UPNPBROADCASTRESPONDER_H
#define UPNPBROADCASTRESPONDER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "Wemo.h"

class UpnpBroadcastResponder
{
public:
    UpnpBroadcastResponder();
    ~UpnpBroadcastResponder();

    bool BeginUdpMulticast();
    void ServerLoop();
    void AddDevice(Wemo& device);

private:
    WiFiUDP m_UDP;
};

#endif
