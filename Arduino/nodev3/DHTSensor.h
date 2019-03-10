#ifndef DHTSENSOR_H
#define DHTSENSOR_H
 
#include <Arduino.h>
#include <DHT.h>

#include "Defines.h"
 
class DHTSensor 
{
    private:
        DHT * _DHTSensor;

        byte _temperatureHistory[DHT_HISTORY_SIZE];
        byte _humidityHistory[DHT_HISTORY_SIZE];

        int _index;

    public:        
        DHTSensor();
        DHTSensor(int pin, int type);
        ~DHTSensor();

        byte getCurrentTemperature();
        byte getCurrentHumidity();

        void readValues();
};
 
#endif
