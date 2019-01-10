#ifndef SWITCH_H
#define SWITCH_H
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>

#include "LightSensor.h"
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
        
        LightSensor * _lightSensor;

        bool _lightStatus;
          
        void _startWebServer();
        void _handleEventservice();
        void _handleUpnpControl();
        void _handleRoot();
        void _handleSetupXml();
        void _handleGetLight();
        void _handleSetThreshold();
        void _handleGetThreshold();

    public:
        Switch();
        Switch(String alexaInvokeName, unsigned int port, LightSensor * lightSensor, CallbackFunction onCallback, CallbackFunction offCallback);
        ~Switch();

        String getAlexaInvokeName();
        
        void serverLoop();
        void respondToSearch(IPAddress& senderIP, unsigned int senderPort);
        void sendRelayState();
};
 
#endif
