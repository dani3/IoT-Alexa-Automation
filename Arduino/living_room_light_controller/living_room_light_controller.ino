// Load Wi-Fi library.
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

#define DEBUG

#define GPIO_RELAY          D2

#define ONCE      1
#define TWICE     2
#define THRICE    3

// Network information.
const char * SSID = "DMA";
const char * PWD  = "1123581321";

String deviceName= "Living room light";

int relayState;

bool _wifiConnected;
bool _serverStarted;
bool _udpConnected;

// Set web server port number to 80.
ESP8266WebServer server(80);
WiFiUDP UDP;

IPAddress ipMulticast(239, 255, 255, 250);
// Local port to listen on
unsigned int portMulticast = 1900;      

// Buffer to hold incoming packet
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 

String serial;
String persistentUUID;
String light;

void prepareIds() 
{
  uint32_t chipId = ESP.getChipId();

  char uuid[64];

  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x")
    , (uint16_t) ((chipId >> 16) & 0xff)
    , (uint16_t) ((chipId >>  8) & 0xff)
    , (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistentUUID = "Socket-1_0-" + serial;
}

void _quickLEDFlashing(int times)
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);

  if (times == ONCE) return;

  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);

  if (times == TWICE) return;

  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
}

void _connectToWiFi()
{
  // WiFi setup
#ifdef DEBUG
  Serial.print("Connecting to ");
  Serial.println(SSID);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    _wifiConnected = false;

    _quickLEDFlashing(ONCE);

    delay(500);

#ifdef DEBUG
    Serial.print(".");
#endif
  }

  _wifiConnected = true;

  // Let people know we're connected!
  _quickLEDFlashing(THRICE);

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
#endif
}

void _connectUDP()
{
  boolean state = false;
  
#ifdef DEBUG
  Serial.println("Connecting to UDP");
#endif

  if (UDP.beginMulticast(WiFi.localIP(), ipMulticast, portMulticast)) 
  {
#ifdef DEBUG
    Serial.println("Connection successful");
#endif
    state = true;
  }
  else
  {
#ifdef DEBUG
    Serial.println("Connection failed");
#endif
  }
  
  _udpConnected = state;
}

void _startHTTPServer()
{
  // Toggle light from Android App
  server.on("/toggleLight", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to toggle the light ...\n");
#endif

    _quickLEDFlashing(ONCE);

    _toggleRelay();

    server.send(200, "text/plain", "Done");
  });

  // Get Status from Android App
  server.on("/getStatus", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to get the status ...\n");
#endif

    _quickLEDFlashing(ONCE);

    float humidity = 32.0f;
    float temperature = 21.3f;

    String temperatureStr = String("Temperature: ") + String(temperature) + String("\n");
    String humidityStr = String("Humidity: ") + String(humidity) + String("\n");

    String status = temperatureStr + humidityStr;

    server.send(200, "text/plain", status);
  });

  // Toggle light from Alexa
  server.on("/upnp/control/basicevent1", HTTP_POST, []() 
  {
#ifdef DEBUG
    Serial.println("\nResponding to  /upnp/control/basicevent1...");      
#endif

    String request = server.arg(0);  
#ifdef DEBUG
    Serial.println("Request: " + request);      
#endif

    if (request.indexOf("SetBinaryState") >= 0)
    {
      if (request.indexOf("<BinaryState>1</BinaryState>") > 0) 
      {
  #ifdef DEBUG
        Serial.println("Got Request to toggle the light ...\n");
  #endif
  
        _quickLEDFlashing(ONCE);
  
        _toggleRelay();
      }
  
      if (request.indexOf("<BinaryState>0</BinaryState>") > 0) 
      {        
  #ifdef DEBUG
        Serial.println("Got Request to toggle the light ...\n");
  #endif
  
        _quickLEDFlashing(ONCE);
  
        _toggleRelay();
      }
    }

    server.send(200, "text/plain", "");
  });

  server.on("/eventservice.xml", HTTP_GET, []()
  {    
#ifdef DEBUG
    Serial.println("\nResponding to eventservice.xml ...");
#endif

    _quickLEDFlashing(ONCE);

    String eventservice_xml = 
      "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
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
        "</action>"
      "</scpd>\r\n"
      "\r\n";
          
    server.send(200, "text/plain", eventservice_xml.c_str());
  });

  server.on("/setup.xml", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("\nResponding to setup.xml ...");
#endif

    _quickLEDFlashing(ONCE);

    IPAddress localIP = WiFi.localIP();
    char s[16];
    sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  
    String setup_xml = 
      "<?xml version=\"1.0\"?>"
      "<root>"
        "<device>"
          "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
          "<friendlyName>"+ deviceName +"</friendlyName>"
          "<manufacturer>Belkin International Inc.</manufacturer>"
          "<modelName>Emulated Socket</modelName>"
          "<modelNumber>3.1415</modelNumber>"
          "<UDN>uuid:"+ persistentUUID +"</UDN>"
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
          
    server.send(200, "text/xml", setup_xml.c_str());

#ifdef DEBUG 
    Serial.print("Sending :");
    Serial.println(setup_xml);
#endif
  });

  server.begin();

#ifdef DEBUG
  Serial.println("HTTP Server started!");
#endif
}

void respondToSearch() 
{
#ifdef DEBUG
  Serial.print("\nSending response to ");
  Serial.println(UDP.remoteIP());
  Serial.print("Port : ");
  Serial.println(UDP.remotePort());
#endif

  _quickLEDFlashing(ONCE);

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String response = 
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":80/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persistentUUID + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(response.c_str());
  UDP.endPacket();                    

#ifdef DEBUG
  Serial.println("Response sent!");
#endif
}

void _toggleRelay()
{
  relayState = (relayState == LOW) ? HIGH : LOW;
  
  digitalWrite(GPIO_RELAY, relayState);
}

void setup()
{
  Serial.begin(9600);

  prepareIds();

  _wifiConnected = false;
  _serverStarted = false;

  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_RELAY, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, LOW);

  relayState = LOW;

  _connectToWiFi();
  if (_wifiConnected)
  {
    _connectUDP();
    if (_udpConnected)
    {
      _startHTTPServer();
    }
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    server.handleClient();

    if (_udpConnected)
    {
      // Ff there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      
      if (packetSize) 
      {
#ifdef DEBUG
        Serial.print("\nReceived packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
#endif
        IPAddress remote = UDP.remoteIP();
        
#ifdef DEBUG
        for (int i = 0; i < 4; i++) 
        {
          Serial.print(remote[i], DEC);
          if (i < 3) 
          {
            Serial.print(".");
          }
        }
        
        Serial.print(", port ");
        Serial.println(UDP.remotePort());
#endif        
        int len = UDP.read(packetBuffer, 255);
        
        if (len > 0) 
        {
          packetBuffer[len] = 0;
        }

        String request = packetBuffer;

#ifdef DEBUG
        Serial.println("Request:");
        Serial.println(request);
#endif    
         
        if (request.indexOf('M-SEARCH') > 0) 
        {
          if (request.indexOf("Man: \"ssdp:discover\"") > 0) 
          {
#ifdef DEBUG
            Serial.println("Responding to search request ...");
#endif    
            respondToSearch();
          }
        }
      }
        
      delay(1);
    }
  }
  else
  {
#ifdef DEBUG
    Serial.println("WiFi disconnected");
#endif

    _connectToWiFi();
    if (_wifiConnected)
    {
      _connectUDP();
      if (_udpConnected)
      {
        _startHTTPServer();
      }
    }
  }

  delay(1);
}
