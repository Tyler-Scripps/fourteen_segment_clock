#include "./src/14-segment-digit/fourteen_segment_digit.h"

CRGB arr1[39];
fourteen_segment_digit digit1; 

void setup() {
  FastLED.addLeds<NEOPIXEL, 19>(arr1, 39);
  digit1.begin(arr1, 3, 3, 1);
  digit1.erase();
  delay(1000);
  digit1.fill(50, 50, 50);
  FastLED.show();
}


void loop() {

}