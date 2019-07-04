#include "define_variables.h"
#include "colors_pallete.h"
#include "basic_animation.h"




void setup() {
    Serial.begin(9600);
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);//.setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
  //colors_pallete_example();
  //basic_animation_ngeblink();
  //basic_animation_cylon();
  //basic_animation_rainbow();
  //basic_animation_rainbowWithGlitter();
  //basic_animation_confetti();
  //basic_animation_sinelon();
  //basic_animation_bpm();
  //basic_animation_juggle();
  //basic_animation_fire2012();
  basic_animation_first_light();
  
}
