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

    public:
        LEDController();
        LEDController(int pin, int numLeds, int indexController, int brightness);
        ~LEDController();

        void showStrip();
        void setPixel(int pixel, byte red, byte green, byte blue);
        void setAll(byte red, byte green, byte blue);

        void fadeIn(byte red, byte green, byte blue);
};    

#endif
