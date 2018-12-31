#ifndef SWITCH_H
#define SWITCH_H
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>

#include "CallbackFunction.h"
 
class Switch 
{
    private:
        ESP8266WebServer * _server = NULL;
        WiFiUDP _UDP;

        String _serialNumber;
        String _persistenUUID;
        String _deviceName;

        unsigned int _localPort;

        CallbackFunction _onCallback;
        CallbackFunction _offCallback;

        bool _lightStatus;
          
        void _startWebServer();
        void _handleEventservice();
        void _handleUpnpControl();
        void _handleRoot();
        void _handleSetupXml();

    public:
        Switch();
        Switch(String alexaInvokeName, unsigned int port, CallbackFunction onCallback, CallbackFunction offCallback);
        ~Switch();

        String getAlexaInvokeName();
        
        void serverLoop();
        void respondToSearch(IPAddress& senderIP, unsigned int senderPort);
        void sendRelayState();
};
 
#endif
