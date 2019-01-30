/** @file AmbientLightController.h
 *  @brief WeMo device that controls one LEDs strip.
 *         It provides an API to change the color and to use different animations.
 *
 *  @author Daniel Mancebo
 *  @bug No known bugs.
 */

#ifndef AMBIENTLIGHTCONTROLLER_H
#define AMBIENTLIGHTCONTROLLER_H

#include <Arduino.h>

#include "Wemo.h"
#include "CallbackFunction.h"

class AmbientLightController : public Wemo
{
    private:
        int _pin;

    public:
        AmbientLightController();
        AmbientLightController(
              String alexaInvokeName
            , unsigned int port
            , CallbackFunction onCallback
            , CallbackFunction offCallback
            , int pin);
        ~AmbientLightController();

        void turnOnOff(bool on);

        virtual void _startWebServer() override;
};    

#endif
