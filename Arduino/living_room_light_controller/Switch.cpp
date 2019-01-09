#include "Switch.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "string"
#include "Defines.h"
        
Switch::Switch()
{
  #ifdef DEBUG
    Serial.println("default constructor called");
  #endif
}

Switch::Switch(String alexaInvokeName, unsigned int port, LightSensor * lightSensor, CallbackFunction onCallback, CallbackFunction offCallback)
{
  _lightStatus = false;

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
  _lightSensor = lightSensor;
    
  _startWebServer();
}
 
Switch::~Switch() {}

void Switch::serverLoop()
{
  if (_server != NULL) 
  {
    _server->handleClient();
    delay(1);
  }
}

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

  _server->on("/getStatus", [&]()
  {
    Utils::quickLEDFlashing(ONCE);

    _handleGetStatus();    
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

void Switch::_handleGetStatus()
{
#ifdef DEBUG
  Serial.println("Got Request to get the status ...\n");
#endif

  float humidity = 32.0f;
  float temperature = 21.3f;

  String temperatureStr = String("Temperature: ") + String(temperature) + String("\n");
  String humidityStr = String("Humidity: ") + String(humidity) + String("\n");

  String status = temperatureStr + humidityStr;

  _server->send(200, "text/plain", status);
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

void Switch::_handleEventservice()
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
 
void Switch::_handleUpnpControl()
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

      _lightStatus = _onCallback();

      sendRelayState();
    }

    if (request.indexOf("<BinaryState>0</BinaryState>") >= 0) 
    {
      #ifdef DEBUG
        Serial.println(" - Got Turn off request");
      #endif

      _lightStatus = _offCallback();
      
      sendRelayState();
    }
  }

  if (request.indexOf("GetBinaryState") >= 0) 
  {
    #ifdef DEBUG
      Serial.println(" - Got relay state request");
    #endif

    sendRelayState();
  }
  
  _server->send(200, "text/plain", "");
}

void Switch::_handleRoot()
{
  _server->send(200, "text/plain", "You should tell Alexa to discover devices");
}

void Switch::_handleSetupXml()
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

String Switch::getAlexaInvokeName() 
{
  return _deviceName;
}

void Switch::sendRelayState() 
{
  String body = 
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
    "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
    "<BinaryState>";
      
  body += (_lightStatus ? "1" : "0");
  
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

void Switch::respondToSearch(IPAddress& senderIP, unsigned int senderPort) 
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
    Serial.println("Response sent.\n");
  #endif
}
