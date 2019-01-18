#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG

// Alexa invoke name
#define AMBILIGHT_NODE_NAME     "Ambilight"

// Controller ports
#define AMBILIGHT_PORT        80

// Colours
#define LIGHT_YELLOW_R      0xFF
#define LIGHT_YELLOW_G      0xCC
#define LIGHT_YELLOW_B      0x20

#define LIGHT_BLUE_R        0x7A
#define LIGHT_BLUE_G        0xDB
#define LIGHT_BLUE_B        0xFF

// Misc
#define ONCE      1
#define TWICE     2
#define THRICE    3

// GPIO Pins
#define TV_LED_STRIP_PIN    2

// Number of LEDs
#define TV_LED_STRIP_NUM_LEDS   232
#define NUM_LEDS                 38

// FastLED configuration
#define LEDS_TYPE       WS2812B
#define COLOR_ORDER         GRB

// LED strip controller indexes
#define TV_LED_STRIP_INDEX            0

// LED strips brightness
#define TV_LED_STRIP_BRIGHTNESS     150

// Animation values
#define WRAP_START_LED              194
#define WRAP_END_LED                 78

#endif
