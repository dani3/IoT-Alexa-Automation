#ifndef DEFINES_H
#define DEFINES_H

#define TV_LED_STRIP_INPUT          2
#define TV_LED_STRIP_PIN            3
#define TV_DRAWER_LED_STRIP_INPUT   4
#define TV_DRAWER_LED_STRIP_PIN     5
#define TV_ALL_INPUT                6

// Colours
#define LIGHT_YELLOW_R      0xFF
#define LIGHT_YELLOW_G      0xCC
#define LIGHT_YELLOW_B      0x20

#define LIGHT_BLUE_R        0x7A
#define LIGHT_BLUE_G        0xDB
#define LIGHT_BLUE_B        0xFF

// FastLED configuration
#define LEDS_TYPE       WS2812B
#define COLOR_ORDER         GRB

// LED strip controller indexes
#define TV_LED_STRIP_INDEX              0
#define TV_DRAWER_LED_STRIP_INDEX       1

// Number of LEDs on each strip
#define TV_LED_STRIP_NUM_LEDS           232
#define TV_DRAWER_LED_STRIP_NUM_LEDS     78

// LED strips brightness
#define TV_LED_STRIP_BRIGHTNESS             85
#define TV_DRAWER_LED_STRIP_BRIGHTNESS      85

// Animation values
#define WRAP_START_LED              194
#define WRAP_END_LED                 78

#endif
