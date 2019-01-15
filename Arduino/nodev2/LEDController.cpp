#include "LEDController.h"
#include "Defines.h"

LEDController::LEDController() {}

LEDController::LEDController(int pin, int numLeds, int indexController, int brightness)
{
    _leds = new CRGB[numLeds];

    FastLED.addLeds<WS2812B, TV_LED_STRIP_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    _index = indexController;
    _brightness = brightness;
}
        
LEDController::~LEDController() {}

void LEDController::showStrip()
{
    FastLED[_index].showLeds(_brightness);
}

void LEDController::setPixel(int pixel, byte red, byte green, byte blue) 
{
    _leds[pixel].r = red;
    _leds[pixel].g = green;
    _leds[pixel].b = blue;
}

void LEDController::setAll(byte red, byte green, byte blue) 
{
    for (int i = 0; i < NUM_LEDS; i++) 
    {
        setPixel(i, red, green, blue); 
    }

    showStrip();
}

void LEDController::fadeIn(byte red, byte green, byte blue)
{
    float r, g, b;
        
    for (int k = 0; k < 256; k++) 
    { 
        r = (k / 256.f) * red;
        g = (k / 256.f) * green;
        b = (k / 256.f) * blue;

        setAll(r, g, b);

        showStrip();
        delay(10);
    }
}
