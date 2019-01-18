#include <Arduino.h>

#include "Utils.h"
#include "UpnpBroadcastResponder.h"
#include "LEDController.h"
#include "Defines.h"

// Prototypes
boolean _connectToWiFi();

// Change this before you flash
const char* SSID = "DMA";
const char* PWD = "1123581321";

boolean _wifiConnected;
boolean _controllersInitialized;

// LEDs controllers attached to this node
LEDController * _tvLedController = NULL;

UpnpBroadcastResponder _upnpBroadcastResponder;

/** @brief Turns on the TV ambient light checking if it's not already.
 * 
 * @return true if it's on.
 */
bool turnOnTVAmbientLight()
{
  #ifdef DEBUG
    Serial.println("Request to turn ON the TV ambient light ...");
  #endif 

  // Check if the light is already on.
  if (_tvLedController->isLightOn())
  {
    #ifdef DEBUG
      Serial.println("Ambient light is already ON ...");
    #endif 
  }
  else
  {
    // Turn on the LEDs
    _tvLedController->fadeIn(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
  }  

  return true;
}

/** @brief Turns off the TV ambient light checking if it's not already.
 * 
 * @return true if it's off.
 */
bool turnOffTVAmbientLight()
{
  #ifdef DEBUG
    Serial.println("Request to turn OFF the TV ambient light ...");
  #endif 

  if (!_tvLedController->isLightOn())
  {
    #ifdef DEBUG
      Serial.println("Light is already OFF ...");
    #endif 
  }
  else
  {
    // Turn off the LEDs
    _tvLedController->fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
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

  _controllersInitialized = false;
  
  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialise WiFi connection
  _wifiConnected = _connectToWiFi();
  
  if (_wifiConnected)
  {
    _upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here.
    // Format: Alexa invocation name, local port no, on callback, off callback
    _tvLedController = new LEDController(
        NODE_NAME
      , 80
      , turnOnTVAmbientLight
      , turnOffTVAmbientLight
      , TV_LED_STRIP_PIN
      , TV_LED_STRIP_NUM_LEDS
      , TV_LED_STRIP_INDEX
      , TV_LED_STRIP_BRIGHTNESS);

    _upnpBroadcastResponder.addDevice(*_tvLedController);

    _controllersInitialized = true;
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

    // Check if it's a command for the controller.
    _tvLedController->serverLoop();
  }
  else
  {
    // Reinitialise WiFi connection
    _wifiConnected = _connectToWiFi();
    
    if (_wifiConnected)
    {
      _upnpBroadcastResponder.beginUdpMulticast();

      // Define your switches here.
      // Format: Alexa invocation name, local port no, on callback, off callback
      _tvLedController = new LEDController(
          NODE_NAME
        , 80
        , turnOnTVAmbientLight
        , turnOffTVAmbientLight
        , TV_LED_STRIP_PIN
        , TV_LED_STRIP_NUM_LEDS
        , TV_LED_STRIP_INDEX
        , TV_LED_STRIP_BRIGHTNESS);

      _upnpBroadcastResponder.addDevice(*_tvLedController);

      _controllersInitialized = true;
    }
  }
}
