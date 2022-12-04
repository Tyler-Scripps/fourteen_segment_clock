#ifndef FOURTEEN_SEGMENT_DIGIT
#define FOURTEEN_SEGMENT_DIGIT
#include "fourteen_segment_digit.h"

// the leds are wired up as so
//
//      9
//  1_______2
//  |\  |10/|
// 8| \ | / |3
//  |  \|/  |
// 14——— ———13
//  |  /|\  |
// 7| / | \ |4
//  |/  |11\| 
//  6———————5
//      12

// the alphanumeric_segs array is using a clockwise labeling scheme

//      a
//  i_______k
//  |\  |j /|
// f| \ | / |b
//  |  \|/  |
//  g———————h
//  |  /|\  |
// e| / | \ |c
//  |/  |m \| 
//  n———————l
//      d

static const uint16_t alphanumeric_segs[96]{
//  nmlkjihgfedcba  
  0b00000000000000, // ' ' (space)
	0b00001000001000, // '!'
	0b00001000000010, // '"'
 	0b01001101001110, // '#'  BAD
	0b01001101101101, // '$'  BAD
	0b10010000100100, // '%'
	0b00110011011001, // '&'  BAD?
	0b00001000000000, // '''
	0b00000000111001, // '('
	0b00000000001111, // ')'
	0b11111111000000, // '*'
	0b01001011000000, // '+'
	0b10000000000000, // ','
	0b00000001000000, // '-'  BAD
	0b00000000000000, // '.'  needs to be adjusted for display without seperate period
	0b10010000000000, // '/'
	0b00000000111111, // '0'
	0b00010000000110, // '1'
	0b00000011011011, // '2'
	0b00000010001111, // '3'
	0b00000011100110, // '4'
	0b00000011101101, // '5'
	0b00000011111101, // '6'
	0b01010000000001, // '7'
	0b00000011111111, // '8'
	0b00000011100111, // '9'
	0b00000000000000, // ':'  needs to be adjusted for display without seperate period
	0b10001000000000, // ';'
	0b00110000000000, // '<'
	0b00000011001000, // '='
	0b10000100000000, // '>'
    0b01000010000011, // '?'
	0b00001100111011, // '@'  BAD
	0b00000011110111, // 'A'
	0b01001010001111, // 'B'
	0b00000000111001, // 'C'
	0b01001000001111, // 'D'
	0b00000011111001, // 'E'
	0b00000011110001, // 'F'
	0b00000010111101, // 'G'
	0b00000011110110, // 'H'
	0b01001000001001, // 'I'
	0b00000000011110, // 'J'
	0b00110001110000, // 'K'
	0b00000000111000, // 'L'
	0b00010100110110, // 'M'
	0b00100100110110, // 'N'
	0b00000000111111, // 'O'
	0b00000011110011, // 'P'
	0b00100000111111, // 'Q'
	0b00100011110011, // 'R'
	0b00000110001101, // 'S'
	0b01001000000001, // 'T'
	0b00000000111110, // 'U'
	0b10010000110000, // 'V'
	0b10100000110110, // 'W'
	0b10110100000000, // 'X'
	0b01010100000000, // 'Y'
	0b10010000001001, // 'Z'
	0b00000000111001, // '['
	0b00100010000000, // '\'
	0b00000000001111, // ']'
    0b10100000000000, // '^'
	0b00000000001000, // '_'
	0b00000010000000, // '`'
	0b00000011011111, // 'a'
	0b00100001111000, // 'b'
	0b00000011011000, // 'c'
	0b10000010001110, // 'd'
	0b00000001111001, // 'e'
	0b00000001110001, // 'f'
	0b00000110001111, // 'g'
	0b00000011110100, // 'h'
	0b01000000000000, // 'i'
	0b00000000001110, // 'j'
	0b01111000000000, // 'k'
	0b01001000000000, // 'l'
	0b01000011010100, // 'm'
	0b00100001010000, // 'n'
	0b00000011011100, // 'o'
	0b00010001110001, // 'p'
	0b00100011100011, // 'q'
	0b00000001010000, // 'r'
	0b00000110001101, // 's'
	0b00000001111000, // 't'
	0b00000000011100, // 'u'
	0b10000000010000, // 'v'
	0b10100000010100, // 'w'
	0b10110100000000, // 'x'
	0b00001010001110, // 'y'
	0b10010000001001, // 'z'
	0b10000011001001, // '{'
	0b01001000000000, // '|'
	0b00110100001001, // '}'
	0b00000101010010, // '~'
	0b11111111111111, // Unknown character (DEL or RUBOUT)
};

fourteen_segment_digit::fourteen_segment_digit() {

}

int fourteen_segment_digit::begin(CRGB newPtr[], uint8_t longCount, uint8_t shortCount, uint8_t newGap) {
    this->stripPtr = newPtr;
    this->numLongSegment = longCount;
    this->numShortSegment = shortCount;
    this->numMiddleGap = newGap;
    this->totalLEDs = 4*longCount + 9*shortCount;

    this->segments[0][0] = 4*longCount + 4*shortCount; //a (9)
    this->segments[0][1] = 4*longCount + 4*shortCount + shortCount - 1; //a (9)
    this->segments[1][0] = 2*longCount + 0*shortCount; //b (3)
    this->segments[1][1] = 2*longCount + 0*shortCount + shortCount - 1; //b (3)
    this->segments[2][0] = 2*longCount + 1*shortCount; //c (4)
    this->segments[2][1] = 2*longCount + 1*shortCount + shortCount - 1; //c (4)
    this->segments[3][0] = 4*longCount + 7*shortCount; //d (12)
    this->segments[3][1] = 4*longCount + 7*shortCount + shortCount -1; //d (12)
    this->segments[4][0] = 4*longCount + 2*shortCount; //e (7)
    this->segments[4][1] = 4*longCount + 2*shortCount + shortCount - 1; //e (7)
    this->segments[5][0] = 4*longCount + 3*shortCount; //f (8)
    this->segments[5][1] = 4*longCount + 3*shortCount + shortCount - 1; //f (8)
    this->segments[6][0] = 4*longCount + 8*shortCount + (shortCount - newGap)/2 + newGap; //g (14)
    this->segments[6][1] = this->totalLEDs - 1; //g (14)
    this->segments[7][0] = 4*longCount + 8*shortCount; //h (13)
    this->segments[7][1] = 4*longCount + 8*shortCount + (shortCount - newGap)/2 - 1; //h (13)
    this->segments[8][0] = 0; //i (1)
    this->segments[8][1] = 0*longCount + 0*shortCount + longCount - 1; //i (1)
    this->segments[9][0] = 4*longCount + 5*shortCount; //j (10)
    this->segments[9][1] = 4*longCount + 5*shortCount + shortCount -1; //j (10)
    this->segments[10][0] = 1*longCount + 0*shortCount; //k (2)
    this->segments[10][1] = 1*longCount + 0*shortCount + longCount - 1; //k (2)
    this->segments[11][0] = 2*longCount + 2*shortCount; //l (5)
    this->segments[11][1] = 2*longCount + 2*shortCount + longCount - 1; //l (5)
    this->segments[12][0] = 4*longCount + 6*shortCount; //m (11)
    this->segments[12][1] = 4*longCount + 6*shortCount + shortCount - 1; //m (11)
    this->segments[13][0] = 3*longCount + 2*shortCount; //n (6)
    this->segments[13][1] = 3*longCount + 2*shortCount + longCount - 1; //n (6)

    return this->totalLEDs;
}

void fourteen_segment_digit::setSegment(uint8_t segmentNum, uint8_t r, uint8_t g, uint8_t b) {
    if (segmentNum > 13)    //validate input
    {
        return;
    }
    for (uint8_t i = this->segments[segmentNum][0]; i < this->segments[segmentNum][1] + 1; i++)
    {
        this->stripPtr[i].r = r;
        this->stripPtr[i].g = g;
        this->stripPtr[i].b = b;
    }
    
}

void fourteen_segment_digit::setSegments(uint16_t segmentBits, uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < 14; i++) {
        if (bitRead(segmentBits, i)) {
            setSegment(i, r, g, b);
        } else {
            setSegment(i, 0, 0, 0);
        }
    }
}

void fourteen_segment_digit::setChar(char newChar, uint8_t r, uint8_t g, uint8_t b) {
    if (newChar >= ' ' && newChar <= '~') {
        setSegments(alphanumeric_segs[newChar - ' '], r, g, b);
    }
}

void fourteen_segment_digit::erase() {
    for (uint8_t i = 0; i < this->totalLEDs; i++)
    {
        this->stripPtr[i].r = 0;
        this->stripPtr[i].g = 0;
        this->stripPtr[i].b = 0;
    }
}

void fourteen_segment_digit::fill(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < this->totalLEDs; i++)
    {
        this->stripPtr[i].r = r;
        this->stripPtr[i].g = g;
        this->stripPtr[i].b = b;
    }
}
#endif  //FOURTEEN_SEGMENT_DIGIT