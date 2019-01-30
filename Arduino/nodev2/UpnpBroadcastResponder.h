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
    private:
        WiFiUDP _UDP;  
        
    public:
        UpnpBroadcastResponder();
        ~UpnpBroadcastResponder();

        bool beginUdpMulticast();
        void serverLoop();
        void addDevice(Wemo& device);
};
 
#endif
