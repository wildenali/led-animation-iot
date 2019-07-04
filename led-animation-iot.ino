#include "FastLED.h"
#define NUM_LEDS 20   // jumlah led
#define DATA_PIN D5   // pin signal

CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
}

void loop() {
  leds[0] = CRGB(255,0,0);
  leds[1] = CRGB(0,255,0);
  leds[2] = CRGB(0,0,255);
  FastLED.show();
//  delay(1000);
//  leds[0] = CRGB(0,0,255);
//  leds[1] = CRGB(255,0,0);
//  leds[2] = CRGB(0,255,0);
//  FastLED.show();
//  delay(1000);
//  leds[0] = CRGB(0,255,0);
//  leds[1] = CRGB(0,0,255);
//  leds[2] = CRGB(255,0,0);
//  FastLED.show();
//  delay(1000);
}
