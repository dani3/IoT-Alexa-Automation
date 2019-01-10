#include <functional>

#include "Arduino.h"
#include "Utils.h"
#include "Defines.h"

void Utils::quickLEDFlashing(int times)
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(25);

    if (times == ONCE) return;

    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(25);

    if (times == TWICE) return;

    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
}
