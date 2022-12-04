// #ifndef FOURTEEN_SEGMENT_DIGIT
// #define FOURTEEN_SEGMENT_DIGIT
#include <FastLED.h>

class fourteen_segment_digit
{
private:
    uint8_t numLongSegment = 0; //how many leds are in a long (diagonal) segment
    uint8_t numShortSegment = 0;    //how many leds are in a short (horizontal/vertical) segment
    uint8_t numMiddleGap = 0;   //number of leds between the two half length segments in the middle
    uint8_t totalLEDs = 0;  //total leds in strip
    uint8_t segments[14][2];
    CRGB* stripPtr;
public:
    fourteen_segment_digit();
    int begin(CRGB newPtr[], uint8_t longCount, uint8_t shortCount, uint8_t newGap);
    void setSegment(uint8_t segmentNum, uint8_t r, uint8_t g, uint8_t b);
    void setSegments(uint16_t segmentBits, uint8_t r, uint8_t g, uint8_t b);
    void setChar(char newChar, uint8_t r, uint8_t g, uint8_t b);
    void erase();
    void fill(uint8_t r, uint8_t g, uint8_t b);
};



// #endif  //FOURTEEN_SEGMENT_DIGIT