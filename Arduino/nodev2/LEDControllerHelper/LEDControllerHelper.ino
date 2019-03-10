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
LEDController _tvDrawerLedController;

bool _tvState               = LOW;
bool _tvPreviousState       = LOW;
bool _tvDrawerState         = LOW;
bool _tvDrawerPreviousState = LOW;
bool _allState              = LOW;
bool _allPreviousState      = LOW;

/** @brief General setup of the node.
 * 
 * @return Void.
 */
void setup() 
{
  Serial.begin(9600);

  pinMode(TV_LED_STRIP_INPUT, INPUT);
  pinMode(TV_DRAWER_LED_STRIP_INPUT, INPUT);

  _tvLedController = LEDController(
      TV_LED_STRIP_PIN
    , TV_LED_STRIP_NUM_LEDS
    , TV_LED_STRIP_INDEX
    , TV_LED_STRIP_BRIGHTNESS);

  _tvDrawerLedController = LEDController(
      TV_DRAWER_LED_STRIP_PIN
    , TV_DRAWER_LED_STRIP_NUM_LEDS
    , TV_DRAWER_LED_STRIP_INDEX
    , TV_DRAWER_LED_STRIP_BRIGHTNESS);

  delay(5000);
}

/** @brief Main loop.
 * 
 * @return Void.
 */
void loop() 
{
  _tvPreviousState       = _tvState;
  _tvDrawerPreviousState = _tvDrawerState;

  _tvState       = digitalRead(TV_LED_STRIP_INPUT);
  _tvDrawerState = digitalRead(TV_DRAWER_LED_STRIP_INPUT);

  if (_tvState != _tvPreviousState)
  {
    if (_tvState)
    {
      _tvLedController.fadeIn(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    }
    else
    {
      _tvLedController.fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    }    
  }

  if (_tvDrawerState != _tvDrawerPreviousState)
  {
    if (_tvDrawerState)
    {
      _tvDrawerLedController.fadeIn(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    }
    else
    {
      _tvDrawerLedController.fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    }    
  }

  /*if (_allState != _allPreviousState)
  {
    _tvLedController.fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
    _tvDrawerLedController.fadeOut(LIGHT_YELLOW_R, LIGHT_YELLOW_G, LIGHT_YELLOW_B);
  }*/

  delay(10);
}
