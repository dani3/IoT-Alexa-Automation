/** @file living_room_light_controller.ino
 *  @brief Main sketch for the ESP8266-12E on the Living room
 * in charge of controlling the foot lamp.
 *
 *  @author Daniel Mancebo
 *  @bug No known bugs.
 */

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

/** @brief Turns on the light checking if it's not already.
 * 
 * @return true if it's on.
 */
bool turnOnLight()
{
  #ifdef DEBUG
    Serial.println("Request to turn ON the light ...");
  #endif 

  // Check if the light is already on.
  if (_lightSensor->isLightOn())
  {
    #ifdef DEBUG
      Serial.println("Light is already ON ...");
    #endif 
  }
  else
  {
    // Switch the relay state.
    _relayState = !_relayState;
    digitalWrite(GPIO_RELAY, _relayState);
  }  

  return true;
}

/** @brief Turns off the light checking if it's not already.
 * 
 * @return true if it's off.
 */
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

/** @brief Connects to the WiFi with a retry mechanism.
 * 
 * @return true if succesfully connected, false otherwise.
 */
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

/** @brief General setup of the node.
 * 
 * @return Void.
 */
void setup()
{
  Serial.begin(9600);
   
  _switchesInitialized = false;
  
  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_RELAY, OUTPUT);
  pinMode(GPIO_PHOTORESISTOR, INPUT);

  // Write a certain state.
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, HIGH);

  _relayState = HIGH;

  // Initialise WiFi connection
  _wifiConnected = _connectToWiFi();
  
  if (_wifiConnected)
  {
    _upnpBroadcastResponder.beginUdpMulticast();

    _lightSensor = new LightSensor();
    
    // Define your switches here.
    // Format: Alexa invocation name, local port no, on callback, off callback
    _livingRoomSwitch = new Switch("Living room light", 80, _lightSensor, turnOnLight, turnOffLight);
    _upnpBroadcastResponder.addDevice(*_livingRoomSwitch);

    _switchesInitialized = true;
  }
}
 
/** @brief Main loop.
 * 
 * @return Void.
 */
void loop()
{
  // Check if it's still connected to the WiFi.
  if (WiFi.status() == WL_CONNECTED)
  {
    // Check for some multicast packets. (M-SEARCH mainly)
    _upnpBroadcastResponder.serverLoop();

    // Check if it's a command for the lamp.
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
        _lightSensor = new LightSensor();
        
        _livingRoomSwitch = new Switch("Living room light", 80, _lightSensor, turnOnLight, turnOffLight);
        _upnpBroadcastResponder.addDevice(*_livingRoomSwitch);

        _switchesInitialized = true;
      }
    }
  }
}
