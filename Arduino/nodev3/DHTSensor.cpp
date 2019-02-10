#include "DHTSensor.h"
#include "Utils.h"
#include "Defines.h"
#include "string.h"

DHTSensor::DHTSensor() {}

DHTSensor::DHTSensor(int pin, int type)
{
    _index = 0;

    _DHTSensor = new DHT(pin, type);
}

DHTSensor::~DHTSensor() {}

byte DHTSensor::getCurrentTemperature()
{
    return _temperatureHistory[_index]; 
}

byte DHTSensor::getCurrentHumidity()
{
    return _humidityHistory[_index]; 
}

void DHTSensor::readValues()
{
    #ifdef DEBUG
        Serial.println("Reading values from DHT22:");
    #endif

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = _DHTSensor->readHumidity();
    // Read temperature as Celsius (the default)
    float t = _DHTSensor->readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) 
    {
    #ifdef DEBUG
        Serial.println("Failed to read from DHT sensor!");
    #endif
      return;
    }

    #ifdef DEBUG
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t\n");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print(" *C\n");
    #endif

    _index++;

    _temperatureHistory[_index] = (byte) t;
    _humidityHistory[_index]    = (byte) h;
}
