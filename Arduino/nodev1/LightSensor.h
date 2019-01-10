#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor
{
    private:
        short Threshold = 0;

    public:
        LightSensor();
        ~LightSensor();

        void setThreshold(short Value);
        short getThreshold(void);
        short getLight(void);
        bool isLightOn(void);
};

#endif
