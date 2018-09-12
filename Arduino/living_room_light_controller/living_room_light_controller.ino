// Load Wi-Fi library.
#include <ESP8266WiFi.h>

#define DEBUG

// Network information.
const char * SSID = "OOV52-STH";
const char * PWD  = "1123581321";

// Set web server port number to 80.
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // WiFi setup
#ifdef DEBUG
  Serial.print("Connecting to ");
  Serial.println(SSID);
#endif

  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.begin();
}

void loop()
{
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client)
  {
    String message = "";
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

        message += c;
      }
    }

#ifdef DEBUG
    Serial.println(message);
#endif

    client.stop();
  }
}
