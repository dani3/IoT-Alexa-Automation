#include "LightSensor.h"
#include "Arduino.h"
#include "Defines.h"
#include "EEPROM.h"

LightSensor::LightSensor() 
{    
    EEPROM.begin(512);
  
    if ((EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS) == 0xFF && EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS + 1) == 0xFF) ||
        (EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS) == 0x00 && EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS + 1) == 0x00))
    {
#ifdef DEBUG
        Serial.println("Initialising threshold...");
#endif
        Threshold = LIGHT_THRESHOLD;

        // Set the initialisation flag.
        EEPROM.write(LIGHT_SENSOR_BASE_ADDRESS, Threshold >> 8);
        EEPROM.write(LIGHT_SENSOR_BASE_ADDRESS + 1, Threshold & 0xFF);
        
        EEPROM.commit();
    }
    else
    {
#ifdef DEBUG
        Serial.println("Reading threshold from NVM...");
#endif
        Threshold = EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS + 1);
        Threshold |= EEPROM.read(LIGHT_SENSOR_BASE_ADDRESS) << 8;
    }
}

LightSensor::~LightSensor() {}

void LightSensor::setThreshold(short Value)
{
    Threshold = Value;
    
    EEPROM.write(LIGHT_SENSOR_BASE_ADDRESS, Threshold >> 8);
    EEPROM.write(LIGHT_SENSOR_BASE_ADDRESS + 1, Threshold & 0xFF);

    EEPROM.commit();
}

short LightSensor::getThreshold(void)
{
    return Threshold;
}

short LightSensor::getLight(void)
{
    return analogRead(GPIO_PHOTORESISTOR);
}

bool LightSensor::isLightOn()
{
    #ifdef DEBUG
        Serial.print("Light intensity: ");
        Serial.println(analogRead(GPIO_PHOTORESISTOR));
    #endif

    return (analogRead(GPIO_PHOTORESISTOR) >= Threshold);
}
