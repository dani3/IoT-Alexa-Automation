#include "Wemo.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "string"
#include "Defines.h"
        
Wemo::Wemo()
{
  #ifdef DEBUG
    Serial.println("default constructor called");
  #endif
}

Wemo::Wemo(
    String alexaInvokeName
  , unsigned int port
  , CallbackFunction onCallback
  , CallbackFunction offCallback)
{
  uint32_t chipId = ESP.getChipId();
  char uuid[64];

  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x")
    , (uint16_t) ((chipId >> 16) & 0xff)
    , (uint16_t) ((chipId >>  8) & 0xff)
    , (uint16_t)   chipId        & 0xff);
  
  _serialNumber = String(uuid);
  _persistenUUID = "Socket-1_0-" + _serialNumber + "-" + String(port);
      
  _deviceName = alexaInvokeName;
  _localPort = port;
  _onCallback = onCallback;
  _offCallback = offCallback;
}
 
Wemo::~Wemo() {}

void Wemo::_startWebServer() {}

void Wemo::serverLoop()
{
  if (_server != NULL) 
  {
    _server->handleClient();
    delay(1);
  }
}

void Wemo::_handleEventservice()
{
  #ifdef DEBUG
    Serial.println("Responding to eventservice.xml ...\n");
  #endif

  String eventserviceXML = 
    "<scpd xmlns=\"urn:Belkin:service-1-0\">"
    "<actionList>"
      "<action>"
        "<name>SetBinaryState</name>"
        "<argumentList>"
          "<argument>"
            "<retval/>"
            "<name>BinaryState</name>"
            "<relatedStateVariable>BinaryState</relatedStateVariable>"
            "<direction>in</direction>"
            "</argument>"
        "</argumentList>"
      "</action>"
      "<action>"
        "<name>GetBinaryState</name>"
        "<argumentList>"
          "<argument>"
            "<retval/>"
            "<name>BinaryState</name>"
            "<relatedStateVariable>BinaryState</relatedStateVariable>"
            "<direction>out</direction>"
            "</argument>"
        "</argumentList>"
      "</action>"
  "</actionList>"
    "<serviceStateTable>"
      "<stateVariable sendEvents=\"yes\">"
        "<name>BinaryState</name>"
        "<dataType>Boolean</dataType>"
        "<defaultValue>0</defaultValue>"
        "</stateVariable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>level</name>"
          "<dataType>string</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
    "</serviceStateTable>"
    "</scpd>\r\n"
    "\r\n";
          
    _server->send(200, "text/plain", eventserviceXML.c_str());
}
 
void Wemo::_handleUpnpControl()
{
  #ifdef DEBUG
    Serial.println("Responding to /upnp/control/basicevent1 ...");      
  #endif

  String request = _server->arg(0); 
  if (request.indexOf("SetBinaryState") >= 0) 
  {
    if (request.indexOf("<BinaryState>1</BinaryState>") >= 0) 
    {
      #ifdef DEBUG
        Serial.println(" - Got Turn on request");
      #endif

      _deviceStatus = _onCallback();

      _sendDeviceState();
    }

    if (request.indexOf("<BinaryState>0</BinaryState>") >= 0) 
    {
      #ifdef DEBUG
        Serial.println(" - Got Turn off request");
      #endif

      _deviceStatus = _offCallback();
      
      _sendDeviceState();
    }
  }

  if (request.indexOf("GetBinaryState") >= 0) 
  {
    #ifdef DEBUG
      Serial.println(" - Got device state request");
    #endif

    _sendDeviceState();
  }
  
  _server->send(200, "text/plain", "");
}

void Wemo::_handleRoot()
{
  _server->send(200, "text/plain", "You should tell Alexa to discover devices");
}

void Wemo::_handleSetupXml()
{
  #ifdef DEBUG
    Serial.println("Responding to setup.xml ...\n");
  #endif

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String setupXML = 
    "<?xml version=\"1.0\"?>"
    "<root>"
      "<device>"
        "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
        "<friendlyName>"+ _deviceName +"</friendlyName>"
        "<manufacturer>Belkin International Inc.</manufacturer>"
        "<modelName>Socket</modelName>"
        "<modelNumber>3.1415</modelNumber>"
        "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
        "<UDN>uuid:"+ _persistenUUID +"</UDN>"
        "<serialNumber>221517K0101769</serialNumber>"
        "<binaryState>0</binaryState>"
        "<serviceList>"
          "<service>"
              "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
              "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
              "<controlURL>/upnp/control/basicevent1</controlURL>"
              "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
              "<SCPDURL>/eventservice.xml</SCPDURL>"
          "</service>"
      "</serviceList>" 
      "</device>"
    "</root>\r\n"
    "\r\n"; 
        
    _server->send(200, "text/xml", setupXML.c_str());
    
    #ifdef DEBUG
      Serial.print(" - Sending: ");
      Serial.println(setupXML);
    #endif
}

bool Wemo::isDeviceOn()
{
  return _deviceStatus;
}

String Wemo::getAlexaInvokeName() 
{
  return _deviceName;
}

void Wemo::_sendDeviceState() 
{
  String body = 
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
    "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
    "<BinaryState>";
      
  body += (_deviceStatus ? "1" : "0");
  
  body += 
    "</BinaryState>\r\n"
    "</u:GetBinaryStateResponse>\r\n"
    "</s:Body> </s:Envelope>\r\n";
  
  _server->send(200, "text/xml", body.c_str());

  #ifdef DEBUG
    Serial.print("Sending: ");
    Serial.println(body);
  #endif
}

void Wemo::respondToSearch(IPAddress& senderIP, unsigned int senderPort) 
{
  #ifdef DEBUG
    Serial.print(" - Sending response to ");
    Serial.println(senderIP);
    Serial.print("Port : ");
    Serial.println(senderPort);
  #endif

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String response = 
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Sat, 26 Nov 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":" + String(_localPort) + "/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + _persistenUUID + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  _UDP.beginPacket(senderIP, senderPort);
  _UDP.write(response.c_str());
  _UDP.endPacket();

  yield();      

  #ifdef DEBUG
    Serial.println(response);
  #endif

  #ifdef DEBUG
    Serial.println("Response sent.\n");
  #endif
}
