// Load Wi-Fi library.
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DEBUG

#define ONCE      1
#define TWICE     2
#define THRICE    3

// Network information.
const char * SSID = "OOV52-STH";
const char * PWD  = "1123581321";

// Set web server port number to 80.
ESP8266WebServer server(80);

void _quickLEDFlashing(int times)
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);

  if (times == 1) return;

  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);

  if (times == 2) return;

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

    server.send(200, "text/plain", "Done");

    _quickLEDFlashing(ONCE);
  });

  server.on("/lightOff", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Got Request to switch light off ...\n");
#endif

    _quickLEDFlashing(ONCE);

    server.send(200, "text/plain", "Done");
  });

  server.begin();

#ifdef DEBUG
  Serial.println("HTTP Server started!");
#endif
}

void setup()
{
  Serial.begin(115200);

  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

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
