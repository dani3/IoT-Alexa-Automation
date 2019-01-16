#include <Arduino.h>
#include "LEDController.h"
#include "Defines.h"

LEDController _tvLedController;

void setup() 
{
  _tvLedController = LEDController(TV_LED_STRIP_PIN, TV_LED_STRIP_NUM_LEDS, TV_LED_STRIP_INDEX, TV_LED_STRIP_BRIGHTNESS);
}

void loop() 
{
  
}
