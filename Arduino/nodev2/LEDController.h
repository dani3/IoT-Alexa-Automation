#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>

#include "Wemo.h"
#include "CallbackFunction.h"

class LEDController : public Wemo
{
    private:
        CRGB* _leds;

        int _index;
        int _brightness;
        int _numLeds;

        bool _tvLEDsState;

        void _showStrip();
        void _setPixel(int pixel, byte red, byte green, byte blue);
        void _setAll(byte red, byte green, byte blue);

    public:
        LEDController();
        LEDController(
              String alexaInvokeName
            , unsigned int port
            , CallbackFunction onCallback
            , CallbackFunction offCallback
            , int pin
            , int numLeds
            , int indexController
            , int brightness);
        ~LEDController();

        bool isLightOn();

        void fadeIn(byte red, byte green, byte blue);
        void fadeOut(byte red, byte green, byte blue);
        void wrap(byte red, byte green, byte blue, int startLED, int endLED);

        virtual void _startWebServer() override;
};    

#endif
