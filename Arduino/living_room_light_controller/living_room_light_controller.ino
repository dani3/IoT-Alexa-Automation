#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

#include "Switch.h"
#include "LightSensor.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "Defines.h"

// Prototypes
boolean _connectToWiFi();

// On/Off callbacks 
bool turnOnLight();
bool turnOffLight();

// Change this before you flash
const char* SSID = "DMA";
const char* PWD = "1123581321";

boolean _wifiConnected;
boolean _switchesInitialized;
boolean _relayState;

UpnpBroadcastResponder _upnpBroadcastResponder;

// Switches attached to this node
Switch * _livingRoomSwitch = NULL;

// Light Sensor 
LightSensor * _lightSensor = NULL;

bool turnOnLight()
{
  #ifdef DEBUG
    Serial.println("Request to turn ON the light ...");
  #endif 

  if (_lightSensor->isLightOn())
  {
    #ifdef DEBUG
      Serial.println("Light is already ON ...");
    #endif 
  }
  else
  {
    _relayState = !_relayState;
    digitalWrite(GPIO_RELAY, _relayState);
  }  

  return true;
}

bool turnOffLight()
{
  #ifdef DEBUG
    Serial.println("Request to turn OFF the light ...");
  #endif 

  if (!_lightSensor->isLightOn())
  {
    #ifdef DEBUG
      Serial.println("Light is already OFF ...");
    #endif 
  }
  else
  {
    _relayState = !_relayState;
    digitalWrite(GPIO_RELAY, _relayState);
  }  

  return false;
}

// Connect to wifi – returns true if successful or false if not
boolean _connectToWiFi()
{
  #ifdef DEBUG
    Serial.print("Connecting to ");
    Serial.println(SSID);
  #endif

  boolean state = true;
  int retries = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Utils::quickLEDFlashing(ONCE);

    if (retries > 10)
    {
      state = false;
      break;
    }

    retries++;

    delay(500);

    #ifdef DEBUG
      Serial.print(".");
    #endif
  }
  
  if (state)
  {
    #ifdef DEBUG
      Serial.println("WiFi connected.");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.print("MAC address: ");
      Serial.println(WiFi.macAddress());
    #endif

    Utils::quickLEDFlashing(THRICE);
  }
  else 
  {
    #ifdef DEBUG
      Serial.println("Connection failed.");
    #endif
  }
  
  return state;
}

void setup()
{
  Serial.begin(9600);
   
  _switchesInitialized = false;
  
  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_RELAY, OUTPUT);
  pinMode(GPIO_PHOTORESISTOR, INPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, HIGH);

  _relayState = HIGH;

  // Initialise WiFi connection
  _wifiConnected = _connectToWiFi();
  
  if (_wifiConnected)
  {
    _upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    _livingRoomSwitch = new Switch("Living room light", 80, turnOnLight, turnOffLight);
    _upnpBroadcastResponder.addDevice(*_livingRoomSwitch);

    _lightSensor = new LightSensor();

    _switchesInitialized = true;
  }
}
 
void loop()
{
	 if (WiFi.status() == WL_CONNECTED)
   {
      _upnpBroadcastResponder.serverLoop();
      
      _livingRoomSwitch->serverLoop();
	 }
   else
   {
      // Reinitialise WiFi connection
      _wifiConnected = _connectToWiFi();

      if (_wifiConnected)
      {
        _upnpBroadcastResponder.beginUdpMulticast();

        if (!_switchesInitialized)
        {
          _livingRoomSwitch = new Switch("Living room light", 80, turnOnLight, turnOffLight);
          _upnpBroadcastResponder.addDevice(*_livingRoomSwitch);

          _switchesInitialized = true;
        }
      }
   }
}
