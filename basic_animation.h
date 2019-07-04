void basic_animation_blink() {
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::White;
  FastLED.show();
  delay(500);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }
void basic_animation_cylon(){
 static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    // leds[i] = CHSV(hue++, 255, 255);
    leds[i] = CHSV(0, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(100);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    // leds[i] = CHSV(hue++, 255, 255);
    leds[i] = CHSV(100, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(100);
  }
}

// ====================================================================================
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
void basic_animation_rainbow()
{
  fill_rainbow(leds, NUM_LEDS, gHue, 10);   // 10 warna, bisa di ganti bisa berapa warna yang mau di rainbow kan
  FastLED.show();
}

void addGlitter(fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void basic_animation_rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  basic_animation_rainbow();
  addGlitter(80);
  FastLED.show();
}

void basic_animation_confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
//  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  leds[pos] += CHSV( gHue + random8(4), 255, 255);
  FastLED.show();
}

void basic_animation_sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 2);  // 2 -> berapa banyak level led 
  int pos = beatsin16(25, 0, NUM_LEDS-1 );    // 25 -> speed bulak balik nya
  leds[pos] += CHSV( gHue, 255, 255);
  FastLED.show();
}

const TProgmemPalette16 myRedPurpleGreenPalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Red, // 'white' is too bright compared to red and blue
    CRGB::Red,
    CRGB::Red,
    
    CRGB::Purple,
    CRGB::Purple,
    CRGB::Purple,
    CRGB::Purple,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Green,
    CRGB::Black,
    CRGB::Black
};

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { basic_animation_rainbow, basic_animation_rainbowWithGlitter, basic_animation_confetti, basic_animation_sinelon};  //basic_animation_juggle, basic_animation_bpm
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void basic_animation_bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  //CRGBPalette16 palette = PartyColors_p;    //PartyColors_p, RainbowColors_p
  CRGBPalette16 palette = myRedPurpleGreenPalette_p;    //PartyColors_p, RainbowColors_p
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

void basic_animation_juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}
// ====================================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
bool gReverseDirection = false;

  // Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  50

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void basic_animation_fire2012()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS-1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
  FastLED.show(); // display this frame
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void basic_animation_first_light()
{
  // Move a single white led 
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    // Turn our current led on to white, then show the leds
    
    leds[whiteLed] = CRGB::Red;
  
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();
  
    // Wait a little bit
    delay(100);
  
    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Green;
  }

  // Move a single white led 
  for(int whiteLed = NUM_LEDS - 1; whiteLed > 0; whiteLed = whiteLed - 1) {
    // Turn our current led on to white, then show the leds
    
    leds[whiteLed] = CRGB::Purple;
  
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();
  
    // Wait a little bit
    delay(100);
  
    // Turn our current led back to black for the next loop around
    leds[whiteLed] = CRGB::Green;
  }
}

