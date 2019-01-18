#include "Switch.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "string"
#include "Defines.h"

Switch::Switch() : Wemo() {}

Switch::Switch(
    String alexaInvokeName
  , unsigned int port
  , LightSensor * lightSensor
  , CallbackFunction onCallback
  , CallbackFunction offCallback)
  : Wemo(alexaInvokeName, port, onCallback, offCallback)
{  
  _lightSensor = lightSensor;
    
  _startWebServer();
}

Switch::~Switch() {}

void Switch::_startWebServer()
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

    if (_lightSensor->isLightOn())
    {
      _offCallback();
    }
    else
    {
      _onCallback();      
    }

    _server->send(200, "text/plain", "Done");
  });

  _server->on("/getLight", [&]()
  {
    Utils::quickLEDFlashing(ONCE);

    _handleGetLight();
  });

  _server->on("/setLightThreshold", [&]()
  {
    Utils::quickLEDFlashing(ONCE);

    _handleSetThreshold();    
  });

  _server->on("/getThreshold", [&]()
  {
    Utils::quickLEDFlashing(ONCE);

    _handleGetThreshold();    
  });

  _server->begin();

  #ifdef DEBUG
    Serial.print("WebServer started on port: ");
    Serial.println(_localPort);
  #endif
}

void Switch::_handleGetLight()
{
#ifdef DEBUG
  Serial.println("Got Request to get the light ...\n");
#endif

  short light = _lightSensor->getLight();

  String lightStr = String("Light value: " + String(light));

  _server->send(200, "text/plain", lightStr);
}

void Switch::_handleGetThreshold()
{
#ifdef DEBUG
  Serial.println("Got Request to get the light threshold ...\n");
#endif

  short light = _lightSensor->getThreshold();

  String lightStr = String("Threshold value: " + String(light));

  _server->send(200, "text/plain", lightStr);
}
 
void Switch::_handleSetThreshold()
{
#ifdef DEBUG
  Serial.print("Got Request to set the light threshold to : ");
  Serial.println(_server->arg("Threshold"));
  Serial.println("");
#endif

  if (_server->arg("Threshold") != "")
  {
    short value = strtoul(_server->arg("Threshold").c_str(), NULL, 10);
    _lightSensor->setThreshold(value);
  }

  _server->send(200, "text/plain", "Done");
}
