#include "fourteen_segment_digit.h"

CRGB arr1[39];
fourteen_segment_digit digit1; 

void setup() {
  FastLED.addLeds<NEOPIXEL, 19>(arr1, 39);
  digit1.begin(arr1, 3, 3, 1);
  digit1.erase();
  delay(1000);
  digit1.fill(50, 50, 50);
  FastLED.show();
  delay(1000);
  digit1.erase();
}


void loop() {
  for (uint8_t i = 0; i < 14; i++)
  {
    digit1.erase();
    digit1.setSegment(i, 50, 50, 50);
    FastLED.show();
    delay(1000);
  }
  digit1.erase();
  digit1.setSegments(0b0011111100000000, 50, 50, 50);
  FastLED.show();
  delay(2000);
  digit1.fill(50,50,50);
  FastLED.show();
  delay(2000);
  digit1.erase();

  for (byte i = '0'; i <= '9'; i++)
  {
    digit1.setChar(i, 50, 0, 0);
    FastLED.show();
    delay(1500);
  }
  
  for (byte i = 'a'; i <= 'z'; i++)
  {
    digit1.setChar(i, 0, 50, 0);
    FastLED.show();
    delay(1500);
  }

  for (byte i = 'A'; i <= 'Z'; i++)
  {
    digit1.setChar(i, 0, 0, 50);
    FastLED.show();
    delay(1500);
  }

  
}