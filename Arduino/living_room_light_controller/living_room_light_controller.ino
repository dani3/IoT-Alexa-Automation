// Load Wi-Fi library.
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "current_meter.h"

#define DEBUG

#define GPIO_CURRENT_SENSOR     0
#define GPIO_RELAY              1

#define ONCE      1
#define TWICE     2
#define THRICE    3

// Network information.
const char * SSID = "OOV52-STH";
const char * PWD  = "1123581321";

int deviceState;
int relayState;

bool _wifiConnected;
bool _serverStarted;

// Set web server port number to 80.
ESP8266WebServer server(80);

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
    _quickLEDFlashing(ONCE);

    delay(500);

#ifdef DEBUG
    Serial.print(".");
#endif
  }

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

void _startHTTPServer()
{
  server.on("/lightOn", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to switch light on ...\n");
#endif

    _quickLEDFlashing(ONCE);

    _turnOnRelay();

    server.send(200, "text/plain", "Done");
  });

  server.on("/lightOff", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to switch light off ...\n");
#endif

    _quickLEDFlashing(ONCE);

    _turnOffRelay();

    server.send(200, "text/plain", "Done");
  });

  server.on("/getStatus", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to get the status ...\n");
#endif

    _quickLEDFlashing(ONCE);

    float humidity = 32.0f;
    float temperature = 21.3f;

    String footLamp = String("Foot lamp: ") + String((digitalRead(GPIO_CURRENT_SENSOR) == HIGH) ? "On\n" : "Off\n");
    String temperatureStr = String("Temperature: ") + String(temperature) + String("\n");
    String humidityStr = String("Humidity: ") + String(humidity) + String("\n");

    String status = footLamp + temperatureStr + humidityStr;

    server.send(200, "text/plain", status);
  });

  server.begin();

#ifdef DEBUG
  Serial.println("HTTP Server started!");
#endif
}

void _turnOffRelay()
{
  currentMeter();

  if (!deviceState)
  {
    digitalWrite(GPIO_RELAY, (relayState == LOW) ? HIGH : LOW);

    relayState = (relayState == LOW) ? HIGH : LOW;
  }
}

void _turnOnRelay()
{
  currentMeter();

  if (deviceState)
  {
    digitalWrite(GPIO_RELAY, (relayState == LOW) ? HIGH : LOW);

    relayState = (relayState == LOW) ? HIGH : LOW;
  }
}

void setup()
{
  Serial.begin(9600);

  _wifiConnected = false;
  _serverStarted = false;

  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize the CURRENT SENSOR pin as an input.
  pinInit(GPIO_CURRENT_SENSOR);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, LOW);

  relayState = LOW;

  _connectToWiFi();
  _startHTTPServer();
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    server.handleClient();
  }
  else
  {
#ifdef DEBUG
    Serial.println("WiFi disconnected");
#endif

    _connectToWiFi();
    _startHTTPServer();
  }

  delay(10);
}
