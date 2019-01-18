#ifndef SWITCH_H
#define SWITCH_H
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>

#include "Wemo.h"
#include "LightSensor.h"
#include "CallbackFunction.h"
 
class Switch : public Wemo
{
    private:        
        LightSensor * _lightSensor;

        void _handleGetLight();
        void _handleSetThreshold();
        void _handleGetThreshold();

    public:
        Switch();
        Switch(
              String alexaInvokeName
            , unsigned int port
            , LightSensor * lightSensor
            , CallbackFunction onCallback
            , CallbackFunction offCallback);
        ~Switch();
        
        virtual void _startWebServer() override;
};
 
#endif
