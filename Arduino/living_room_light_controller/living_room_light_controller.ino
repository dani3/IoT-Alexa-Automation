// Load Wi-Fi library.
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "current_meter.h"

#define DEBUG

#define GPIO_RELAY              2
#define GPIO_CURRENT_SENSOR     4

#define ONCE      1
#define TWICE     2
#define THRICE    3

// Network information.
const char * SSID = "OOV52-STH";
const char * PWD  = "1123581321";

int deviceState;

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
  bool _ledOn = false;

  // WiFi setup
#ifdef DEBUG
  Serial.print("Connecting to ");
  Serial.println(SSID);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, (_ledOn) ? LOW : HIGH);
    _ledOn = !_ledOn;

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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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

    const char * status = ((digitalRead(GPIO_CURRENT_SENSOR) == HIGH) ? "On" : "Off");

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

  if (deviceState == 1 && digitalRead(GPIO_RELAY) == HIGH)
  {
    digitalWrite(GPIO_RELAY, LOW);
  }
  else
  {
    digitalWrite(GPIO_RELAY, HIGH);
  }
}

void _turnOnRelay()
{
  currentMeter();

  if (deviceState == 0 && digitalRead(GPIO_RELAY) == LOW)
  {
      digitalWrite(GPIO_RELAY, HIGH);
  }
  else
  {
    digitalWrite(GPIO_RELAY, LOW);
  }
}

void setup()
{
  Serial.begin(115200);

  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize the RELAY pin as an output.
  pinMode(GPIO_RELAY, OUTPUT);
  // Initialize the CURRENT SENSOR pin as an input.
  pinInit(GPIO_CURRENT_SENSOR);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GPIO_RELAY, LOW);

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
