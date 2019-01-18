#ifndef WEMO_H
#define WEMO_H
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>

#include "CallbackFunction.h"
 
class Wemo 
{
    protected:
        ESP8266WebServer * _server = NULL;
        WiFiUDP _UDP;

        String _serialNumber;
        String _persistenUUID;
        String _deviceName;

        unsigned int _localPort;

        CallbackFunction _onCallback;
        CallbackFunction _offCallback;

        void _handleEventservice();
        void _handleUpnpControl();
        void _handleRoot();
        void _handleSetupXml();

        bool _deviceStatus;

        void _sendDeviceState();

    public:
        Wemo();
        Wemo(String alexaInvokeName, unsigned int port, CallbackFunction onCallback, CallbackFunction offCallback);
        ~Wemo();

        String getAlexaInvokeName();
        
        void serverLoop();
        void respondToSearch(IPAddress& senderIP, unsigned int senderPort);
          
        virtual void _startWebServer();
};
 
#endif
