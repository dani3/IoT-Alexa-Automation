#include "LightSensor.h"
#include "Arduino.h"
#include "Defines.h"

LightSensor::LightSensor() {}

LightSensor::~LightSensor() {}

bool LightSensor::isLightOn()
{
    return (analogRead(GPIO_PHOTORESISTOR) >= LIGHT_THRESHOLD);
}
