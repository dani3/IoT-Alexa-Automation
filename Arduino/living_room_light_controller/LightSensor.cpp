#include "LightSensor.h"
#include "Arduino.h"
#include "Defines.h"

LightSensor::LightSensor() {}

LightSensor::~LightSensor() {}

bool LightSensor::isLightOn()
{
    #ifdef DEBUG
        Serial.print("Light intensity: ");
        Serial.println(analogRead(GPIO_PHOTORESISTOR));
    #endif

    return (analogRead(GPIO_PHOTORESISTOR) >= LIGHT_THRESHOLD);
}
