#ifndef UTILS_H
#define UTILS_H
 
#include <Arduino.h>

#define ONCE      1
#define TWICE     2
#define THRICE    3

class Utils
{
    public:
        static void quickLEDFlashing(int times);
};

#endif
