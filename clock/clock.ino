#include "./src/14-segment-digit/fourteen_segment_digit.h"

CRGB arr1[39];
fourteen_segment_digit digit1; 

void setup() {
    digit1.begin(19, arr1, 3, 3, 1);
    digit1.erase();
}


void loop() {

}