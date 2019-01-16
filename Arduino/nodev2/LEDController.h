#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>

class LEDController
{
    private:
        CRGB* _leds;

        int _index;
        int _brightness;
        int _numLeds;

        void _showStrip();
        void _setPixel(int pixel, byte red, byte green, byte blue);
        void _setAll(byte red, byte green, byte blue);

    public:
        LEDController();
        LEDController(int pin, int numLeds, int indexController, int brightness);
        ~LEDController();

        void fadeIn(byte red, byte green, byte blue);
        void wrap(byte red, byte green, byte blue, int startLED, int endLED);
};    

#endif
