#include "Switch.h"
#include "CallbackFunction.h"
#include "Utils.h"

#define ONCE      1
#define TWICE     2
#define THRICE    3
        
Switch::Switch()
{
    Serial.println("default constructor called");
}

Switch::Switch(String alexaInvokeName, unsigned int port, CallbackFunction oncb, CallbackFunction offcb)
{
  switchStatus = false;

  uint32_t chipId = ESP.getChipId();
  char uuid[64];

  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x")
    , (uint16_t) ((chipId >> 16) & 0xff)
    , (uint16_t) ((chipId >>  8) & 0xff)
    , (uint16_t)   chipId        & 0xff);
  
  serial = String(uuid);
  persistenUUID = "Socket-1_0-" + serial+"-"+ String(port);
      
  deviceName = alexaInvokeName;
  localPort = port;
  onCallback = oncb;
  offCallback = offcb;
    
  _startWebServer();
}
 
Switch::~Switch() {}

void Switch::serverLoop()
{
  if (server != NULL) 
  {
    server->handleClient();
    delay(1);
  }
}

void Switch::_startWebServer()
{
  server = new ESP8266WebServer(localPort);

  server->on("/", [&]() 
  {
    Utils::quickLEDFlashing(ONCE);

    _handleRoot();
  }); 

  server->on("/setup.xml", [&]() 
  {
    Utils::quickLEDFlashing(ONCE);

    _handleSetupXml();
  });

  server->on("/upnp/control/basicevent1", [&]() 
  {
    Utils::quickLEDFlashing(ONCE);

    _handleUpnpControl();
  });

  server->on("/eventservice.xml", [&]() 
  {
    Utils::quickLEDFlashing(ONCE);

    _handleEventservice();
  });

  server->on("/toggleLight", [&]()
  {
    Serial.println("Got Request to toggle the light ...\n");

    Utils::quickLEDFlashing(ONCE);

    onCallback();

    server->send(200, "text/plain", "Done");
  });

  server->on("/getStatus", [&]()
  {
    Serial.println("Got Request to get the status ...\n");

    Utils::quickLEDFlashing(ONCE);

    float humidity = 32.0f;
    float temperature = 21.3f;

    String temperatureStr = String("Temperature: ") + String(temperature) + String("\n");
    String humidityStr = String("Humidity: ") + String(humidity) + String("\n");

    String status = temperatureStr + humidityStr;

    server->send(200, "text/plain", status);
  });

  server->begin();

  Serial.print("WebServer started on port: ");
  Serial.println(localPort);
}
 
void Switch::_handleEventservice()
{
  Serial.println("Responding to eventservice.xml ...\n");
  
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
          
    server->send(200, "text/plain", eventserviceXML.c_str());
}
 
void Switch::_handleUpnpControl()
{
  Serial.println("Responding to /upnp/control/basicevent1 ...");      

  String request = server->arg(0); 
  if (request.indexOf("SetBinaryState") >= 0) 
  {
    if (request.indexOf("<BinaryState>1</BinaryState>") >= 0) 
    {
        Serial.println(" - Got Turn on request");

        switchStatus = onCallback();

        sendRelayState();
    }

    if (request.indexOf("<BinaryState>0</BinaryState>") >= 0) 
    {
        Serial.println(" - Got Turn off request");

        switchStatus = offCallback();
        
        sendRelayState();
    }
  }

  if (request.indexOf("GetBinaryState") >= 0) 
  {
    Serial.println(" - Got relay state request");

    sendRelayState();
  }
  
  server->send(200, "text/plain", "");
}

void Switch::_handleRoot()
{
  server->send(200, "text/plain", "You should tell Alexa to discover devices");
}

void Switch::_handleSetupXml()
{
  Serial.println("Responding to setup.xml ...\n");
  
  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String setupXML = 
    "<?xml version=\"1.0\"?>"
    "<root>"
      "<device>"
        "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
        "<friendlyName>"+ deviceName +"</friendlyName>"
        "<manufacturer>Belkin International Inc.</manufacturer>"
        "<modelName>Socket</modelName>"
        "<modelNumber>3.1415</modelNumber>"
        "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
        "<UDN>uuid:"+ persistenUUID +"</UDN>"
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
        
    server->send(200, "text/xml", setupXML.c_str());
    
    Serial.print(" - Sending: ");
    Serial.println(setupXML);
}

String Switch::getAlexaInvokeName() 
{
  return deviceName;
}

void Switch::sendRelayState() 
{
  String body = 
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
    "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
    "<BinaryState>";
      
  body += (switchStatus ? "1" : "0");
  
  body += 
    "</BinaryState>\r\n"
    "</u:GetBinaryStateResponse>\r\n"
    "</s:Body> </s:Envelope>\r\n";
  
  server->send(200, "text/xml", body.c_str());

  Serial.print("Sending: ");
  Serial.println(body);
}

void Switch::respondToSearch(IPAddress& senderIP, unsigned int senderPort) 
{
  Serial.print(" - Sending response to ");
  Serial.println(senderIP);
  Serial.print("Port : ");
  Serial.println(senderPort);

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String response = 
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Sat, 26 Nov 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":" + String(localPort) + "/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persistenUUID + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  UDP.beginPacket(senderIP, senderPort);
  UDP.write(response.c_str());
  UDP.endPacket();

  yield();      

  Serial.println("Response sent.\n");
}