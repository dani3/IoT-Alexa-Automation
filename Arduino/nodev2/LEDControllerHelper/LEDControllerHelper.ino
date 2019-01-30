/** @file LEDControllerHelper.ino
 *  @brief Main sketch for the Arduino that receives the command
 *         from the ESP8266 to turn on/off the LEDs.
 *
 *  @author Daniel Mancebo
 *  @bug No known bugs.
 */

#include <FastLED.h>
#include "LEDController.h"
#include "Defines.h"

LEDController _tvLedController;

bool _state = LOW;
bool _previousState = LOW;

/** @brief General setup of the node.
 * 
 * @return Void.
 */
void setup() 
{
  Serial.begin(9600);

  pinMode(ESP_PIN_INPUT, INPUT);

  _tvLedController = LEDController(
      TV_LED_STRIP_PIN
    , TV_LED_STRIP_NUM_LEDS
    , TV_LED_STRIP_INDEX
    , TV_LED_STRIP_BRIGHTNESS);

  delay(5000);
}

/** @brief Main loop.
 * 
 * @return Void.
 */
void loop() 
{
  _previousState = _state;
  _state = digitalRead(ESP_PIN_INPUT);

  if (_state != _previousState)
  {
    if (_state)
    {
      _tvLedController.fadeIn(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    }
    else
    {
      _tvLedController.fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);      
    }    
  }

  delay(10);
}
