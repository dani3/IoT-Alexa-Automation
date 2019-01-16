#include <functional>

#include "UpnpBroadcastResponder.h"
#include "Switch.h"
#include "Utils.h"
#include "Defines.h"

#define MAX_SWITCHES 14

// Multicast declarations
IPAddress ipMulti(239, 255, 255, 250);
const unsigned int portMulti = 1900;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

Switch switches[MAX_SWITCHES] = {};
int numOfSwitches = 0;
 
UpnpBroadcastResponder::UpnpBroadcastResponder() {} 

UpnpBroadcastResponder::~UpnpBroadcastResponder() {}
 
bool UpnpBroadcastResponder::beginUdpMulticast()
{
  boolean state = false;
  
  #ifdef DEBUG
    Serial.println("Beginning multicast ...");
  #endif
  
  if (_UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) 
  {
    #ifdef DEBUG
      Serial.print("Udp multicast _server started at: ");
      Serial.print(ipMulti);
      Serial.print(":");
      Serial.println(portMulti);
    #endif

    state = true;
  }
  else
  {
    #ifdef DEBUG
      Serial.println("Connection failed");
    #endif
  }
  
  return state;
}

void UpnpBroadcastResponder::addDevice(Switch& device) 
{
  #ifdef DEBUG
    Serial.print("Adding switch: ");
    Serial.print(device.getAlexaInvokeName());
    Serial.print("  Index: ");
    Serial.println(numOfSwitches);
  #endif
  
  switches[numOfSwitches++] = device;
}

void UpnpBroadcastResponder::serverLoop()
{
  int packetSize = _UDP.parsePacket();
  if (packetSize <= 0)
  {
    return;
  }
  
  IPAddress senderIP = _UDP.remoteIP();
  unsigned int senderPort = _UDP.remotePort();
  
  // Read the packet into the buffer
  _UDP.read(packetBuffer, packetSize);
  
  // Check if this is a M-SEARCH for WeMo device
  String request = String((char *) packetBuffer);

  if (request.indexOf("M-SEARCH") >= 0) 
  {
    if ((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) 
    {      
      Utils::quickLEDFlashing(ONCE);

      #ifdef DEBUG
        Serial.println("Got _UDP Belkin Request ...");
      #endif
    
      for (int n = 0; n < numOfSwitches; n++) 
      {
        Switch &sw = switches[n];

        if (&sw != NULL) 
        {
          sw.respondToSearch(senderIP, senderPort);              
        }
      }
    }
  }
}