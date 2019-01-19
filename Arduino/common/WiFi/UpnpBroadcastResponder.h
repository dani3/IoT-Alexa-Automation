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
