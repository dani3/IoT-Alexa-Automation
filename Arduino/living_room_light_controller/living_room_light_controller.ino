#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "Utils.h"

#define GPIO_RELAY          D2

#define ONCE      1
#define TWICE     2
#define THRICE    3

// Prototypes
boolean _connectToWiFi();

// On/Off callbacks 
bool toggleLight();

// Change this before you flash
const char* SSID = "DMA";
const char* PWD = "1123581321";

boolean _wifiConnected;

UpnpBroadcastResponder upnpBroadcastResponder;

// Switches attached to this node
Switch* livingRoomSwitch = NULL;

// State of each switch
boolean isLivingRoomLightsOn;

bool toggleLight() 
{
    Serial.println("Toggling lights ...");
    
    isLivingRoomLightsOn = !isLivingRoomLightsOn;   

    digitalWrite(GPIO_RELAY, isLivingRoomLightsOn);

    return isLivingRoomLightsOn;
}

// Connect to wifi – returns true if successful or false if not
boolean _connectToWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(SSID);

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

    Serial.print(".");
  }
  
  if (state)
  {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    Utils::quickLEDFlashing(THRICE);
  }
  else 
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void setup()
{
  Serial.begin(9600);
   
  isLivingRoomLightsOn = false;
  
  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_RELAY, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, LOW);

  // Initialise WiFi connection
  _wifiConnected = _connectToWiFi();
  
  if (_wifiConnected)
  {
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    livingRoomSwitch = new Switch("Living room light", 80, toggleLight, toggleLight);

    Serial.println("Adding switches to UPnP broadcast responder");

    upnpBroadcastResponder.addDevice(*livingRoomSwitch);
  }
}
 
void loop()
{
	 if (WiFi.status() == WL_CONNECTED)
   {
      upnpBroadcastResponder.serverLoop();
      
      livingRoomSwitch->serverLoop();
	 }
   else
   {
      // Reinitialise WiFi connection
      _wifiConnected = _connectToWiFi();

      if (_wifiConnected)
      {
        upnpBroadcastResponder.beginUdpMulticast();
      }
   }
}
