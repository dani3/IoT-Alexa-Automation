#include <Arduino.h>
#include "LEDController.h"
#include "Defines.h"

LEDController _tvLedController;

void setup() 
{
  _tvLedController = LEDController(TV_LED_STRIP_PIN, NUM_LEDS, TV_LED_STRIP_INDEX, TV_LED_STRIP_BRIGHTNESS);

  _tvLedController.fadeIn(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
}

void loop() 
{
  
}
