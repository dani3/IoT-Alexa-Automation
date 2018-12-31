#include "UpnpBroadcastResponder.h"
#include "Switch.h"
#include "Utils.h"
#include <functional>
 
#define MAX_SWITCHES 14

#define ONCE      1
#define TWICE     2
#define THRICE    3

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
  
  Serial.println("Beginning multicast ...");
  
  if (UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) 
  {
    Serial.print("Udp multicast server started at: ");
    Serial.print(ipMulti);
    Serial.print(":");
    Serial.println(portMulti);

    state = true;
  }
  else
  {
    Serial.println("Connection failed");
  }
  
  return state;
}

void UpnpBroadcastResponder::addDevice(Switch& device) 
{
  Serial.print("Adding switch: ");
  Serial.print(device.getAlexaInvokeName());
  Serial.print("  Index: ");
  Serial.println(numOfSwitches);
  
  switches[numOfSwitches++] = device;
}

void UpnpBroadcastResponder::serverLoop()
{
  int packetSize = UDP.parsePacket();
  if (packetSize <= 0)
  {
    return;
  }
  
  IPAddress senderIP = UDP.remoteIP();
  unsigned int senderPort = UDP.remotePort();
  
  // Read the packet into the buffer
  UDP.read(packetBuffer, packetSize);
  
  // Check if this is a M-SEARCH for WeMo device
  String request = String((char *) packetBuffer);

  if (request.indexOf("M-SEARCH") >= 0) 
  {
    if ((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) 
    {      
      Utils::quickLEDFlashing(ONCE);

      Serial.println("Got UDP Belkin Request ...");
    
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
