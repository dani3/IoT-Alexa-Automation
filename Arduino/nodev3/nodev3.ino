/** @file nodev3.ino
 *  @brief Main sketch for the ESP8266-12E on the Living room
 * in charge of controlling the LEDs strips.
 *
 *  @author Daniel Mancebo
 *  @bug No known bugs.
 */

#include <Arduino.h>

#include "Defines.h"

// Change this before you flash
const char* SSID = "DMA";
const char* PWD = "1123581321";

/** @brief General setup of the node.
 * 
 * @return Void.
 */
void setup() 
{
  Serial.begin(9600);
  
  // Initialize the LED_BUILTIN pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

/** @brief Main loop.
 * 
 * @return Void.
 */
void loop() 
{
  
}
