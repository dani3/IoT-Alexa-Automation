#include "AmbientLightController.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "string"
#include "Defines.h"

AmbientLightController::AmbientLightController() : Wemo() {}

AmbientLightController::AmbientLightController(
    String alexaInvokeName
  , unsigned int port
  , CallbackFunction onCallback
  , CallbackFunction offCallback
  , int pin)
  : Wemo(alexaInvokeName, port, onCallback, offCallback)
{
    _pin = pin;
    _deviceStatus = false;

    _startWebServer();
}
        
AmbientLightController::~AmbientLightController() {}

void AmbientLightController::_startWebServer()
{
    _server = new ESP8266WebServer(_localPort);

    _server->on("/", [&]() 
    {
        Utils::quickLEDFlashing(ONCE);

        _handleRoot();
    }); 

    _server->on("/setup.xml", [&]() 
    {
        Utils::quickLEDFlashing(ONCE);

        _handleSetupXml();
    });

    _server->on("/upnp/control/basicevent1", [&]() 
    {
        Utils::quickLEDFlashing(ONCE);

        _handleUpnpControl();
    });

    _server->on("/eventservice.xml", [&]() 
    {
        Utils::quickLEDFlashing(ONCE);

        _handleEventservice();
    });

    _server->on("/toggleLight", [&]()
    {
        Utils::quickLEDFlashing(ONCE);

        if (_deviceStatus)
        {
            _offCallback();
        }
        else
        {
            _onCallback();      
        }

        _server->send(200, "text/plain", "Done");
    });

    _server->begin();

    #ifdef DEBUG
        Serial.print("WebServer started on port: ");
        Serial.println(_localPort);
    #endif
}

void AmbientLightController::turnOnOff(bool on)
{
    digitalWrite(_pin, on);

    _deviceStatus = on;
}
