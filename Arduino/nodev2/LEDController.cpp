#include "LEDController.h"
#include "CallbackFunction.h"
#include "Utils.h"
#include "string"
#include "Defines.h"

LEDController::LEDController() : Wemo() {}

LEDController::LEDController(
    String alexaInvokeName
  , unsigned int port
  , CallbackFunction onCallback
  , CallbackFunction offCallback
  , int pin
  , int numLeds
  , int indexController
  , int brightness)
  : Wemo(alexaInvokeName, port, onCallback, offCallback)
{
    _leds = new CRGB[numLeds];

    _index      = indexController;
    _brightness = brightness;
    _numLeds    = numLeds;

    _tvLEDsState = false;

    FastLED.addLeds<WS2812B, TV_LED_STRIP_PIN, COLOR_ORDER>(_leds, _numLeds).setCorrection(TypicalLEDStrip);

    _startWebServer();
}
        
LEDController::~LEDController() {}

void LEDController::_startWebServer()
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

        if (_tvLEDsState)
        {
            _offCallback();
        }
        else
        {
            _onCallback();      
        }

        _server->send(200, "text/plain", "Done");
    });

    _server->begin();

    #ifdef DEBUG
        Serial.print("WebServer started on port: ");
        Serial.println(_localPort);
    #endif
}

void LEDController::_showStrip()
{
    FastLED[_index].showLeds(_brightness);
}

void LEDController::_setPixel(int pixel, byte red, byte green, byte blue) 
{
    _leds[pixel].r = red;
    _leds[pixel].g = green;
    _leds[pixel].b = blue;
}

void LEDController::_setAll(byte red, byte green, byte blue) 
{
    for (int i = 0; i < _numLeds; i++) 
    {
        _setPixel(i, red, green, blue); 
    }

    _showStrip();
}

void LEDController::fadeIn(byte red, byte green, byte blue)
{
    float r, g, b;
        
    for (int k = 0; k < 256; k++) 
    { 
        r = (k / 256.f) * red;
        g = (k / 256.f) * green;
        b = (k / 256.f) * blue;

        _setAll(r, g, b);

        delay(1);
    }

    _tvLEDsState = true;
}

void LEDController::fadeOut(byte red, byte green, byte blue)
{
    float r, g, b;
        
    for (int k = 255; k >= 0; k--) 
    { 
        r = (k / 255.f) * red;
        g = (k / 255.f) * green;
        b = (k / 255.f) * blue;

        _setAll(r, g, b);

        delay(1);
    }

    _tvLEDsState = false;
}

void LEDController::wrap(byte red, byte green, byte blue, int startLED, int endLED)
{
    int right = startLED + 1;
    int left  = startLED - 1;

    // Adjacent neighbours
    int rightRN;
    int rightLN;
    int leftRN;
    int leftLN;

    byte redL   = red / 2;
    byte greenL = green / 2;
    byte blueL  = blue / 2;

    // Turn off the LEDs
    _setAll(0x00, 0x00, 0x00);

    // Set the first pixel to HIGH and the adjacents to LOW.
    _setPixel(startLED, red, green, blue);
    _setPixel(right, redL, greenL, blueL);
    _setPixel(left, redL, greenL, blueL);

    _showStrip();
    delay(3);

    // Stop before reaching the last LED.
    for (int i = 0; i < (_numLeds / 2) - 2; ++i)
    {
        // Make sure we stay on the limits
        right = (right + 1 >= _numLeds) ? 0 : right + 1;
        left = (left - 1 < 0) ? _numLeds - 1 : left - 1;

        rightRN = (right + 1 >= _numLeds) ? 0 : right + 1;
        rightLN = (right - 1 < 0) ? _numLeds - 1 : right - 1;

        leftRN = (left + 1 >= _numLeds) ? 0 : left + 1;
        leftLN = (left - 1 < 0) ? _numLeds - 1 : left - 1;

        _setPixel(right, red, green, blue);
        _setPixel(left, red, green, blue);
        
        _setPixel(rightRN, redL, greenL, blueL);
        _setPixel(rightLN, redL, greenL, blueL);
        
        _setPixel(leftRN, redL, greenL, blueL);
        _setPixel(leftLN, redL, greenL, blueL);

        _showStrip();
        delay(3);
    }

    right = endLED + 1;
    left  = endLED - 1;

    // Set the first pixel to HIGH and the adjacents to LOW.
    _setPixel(endLED, red, green, blue);
    _setPixel(right, redL, greenL, blueL);
    _setPixel(left, redL, greenL, blueL);

    _showStrip();
    delay(3);

    float r, g, b;
    for (int k = 127; k < 64; k++)
    {
        r = (k / 128.f) * red;
        g = (k / 128.f) * green;
        b = (k / 128.f) * blue;

        _setPixel(endLED, r, g, b);

        _showStrip();
        delay(1);
    } 

    _setPixel(endLED, redL, greenL, blueL);
}

bool LEDController::isLightOn()
{
    return _tvLEDsState;
}
