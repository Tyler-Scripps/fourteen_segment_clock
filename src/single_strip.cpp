#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "webpage.h"  //stores the webpage that will be served
#include "fourteen_segment_digit.h"
#include <ESP32Time.h>  //for accessing rtc
#include <elapsedMillis.h>  //provides timing function
#include <Preferences.h>


#define NUM_DIGITS 12
#define LEDS_IN_DIGIT 169
#define DEBUG true
 
const char *ssid = "bigclock";
const char *password = "merrychristmas";

const String numerals[60] = {"0","I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII","XIII","XIV","XV","XVI","XVII","XVIII","XIX","XX","XXI","XXII","XXIII","XXIV","XXV","XXVI","XXVII","XXVIII","XXIX","XXX","XXXI","XXXII","XXXIII","XXXIV","XXXV","XXXVI","XXXVII","XXXIIX","XXXIX","XL","XLI","XLII","XLIII","XLIV","XLV","XLVI","XLVII","XLVIII","XLIX","L","LI","LII","LIII","LIV","LV","LVI","LVII","LVIII","LIX"};

//                                    1       2         3        4        5       6         7(1/2)   7(2/2)    10        8          11         12         9         13
//                                    a       b         c        d        e       f         g        h         i         j          k          l          m         n
const uint8_t topIndices[14][2] = {{0,12}, {13,25}, {26,38}, {39,51}, {52,64}, {65,77}, {78,83}, {85,90}, {117,129}, {91,103}, {130,142}, {143,155}, {104,116}, {156,168}};

//                                    4        5        6       1        2        3        7(2/2)   7(1/2)    12         9          13         10        8          11
//                                    a        b        c       d        e        f        g        h         i          j          k          l         m          n
const uint8_t bottomIndices[14][2] = {{39,51}, {52,64}, {65,77}, {0,12}, {12,25}, {26,38}, {85,90}, {78,83}, {143,155}, {104,116}, {156,168}, {117,129}, {91,103}, {130,142}};

Preferences preferences;  //stores settings between power cycles

bool wiresTop = false;

uint8_t globalRed = 50;
uint8_t globalGreen = 0;
uint8_t globalBlue = 0;

bool militaryTime = false;
uint8_t clockBase = 10;

String message = "";  //stores message to be displayed in message mode
int messageIndex = 0; //stores current index of scrolling message
bool messageScrolling = false;  //wether or not the message should scroll

int mode = 0;
elapsedMillis lastUpdate;
String tempStr;

ESP32Time rtc;
 
AsyncWebServer server(80);

CRGB crgbArrHours[NUM_DIGITS/2][LEDS_IN_DIGIT];
CRGB crgbArrMinutes[NUM_DIGITS/2][LEDS_IN_DIGIT];
fourteen_segment_digit digits[NUM_DIGITS];

CRGB colonLeds[10];

void colonOn() {
  for (uint8_t i = 0; i < 10; i++)
  {
    colonLeds[i].r = globalRed;
    colonLeds[i].g = globalGreen;
    colonLeds[i].b = globalBlue;

  }
  
}

void colonOff() {
  for (uint8_t i = 0; i < 10; i++)
  {
    colonLeds[i].r = 0;
    colonLeds[i].g = 0;
    colonLeds[i].b = 0;
  }
  
}

String paddedString(int unpadded, char padChar, uint8_t base, uint8_t numDigits) {
  String newStr = String(unpadded, base);
  while (newStr.length() < numDigits)
  {
    newStr = padChar + newStr;
  }
  return newStr;
}

String paddedString(String unpadded, char padChar, uint8_t numDigits) {
  String newStr = unpadded;
  while (newStr.length() < numDigits)
  {
    newStr = padChar + newStr;
  }
  return newStr;
}

String createBaseTime(uint8_t base) {
  String baseTime = paddedString(rtc.getHour(militaryTime), ' ', base, 6);  
  if (base == 10 && rtc.getHour(militaryTime) < 10)
  {
    baseTime.setCharAt(4, '0');
  }

  if (base == 10 && rtc.getMinute() < 10)
  {
    baseTime += '0';
  }
  
  
  baseTime += String(rtc.getMinute(), base);
  if (base >= 8) //bases less than 8 require 3 digits to represent decimal 59 so it will be hard to read on only 6 digits for minutes and seconds
  {
    baseTime += paddedString(rtc.getSecond(), ' ', base, NUM_DIGITS - baseTime.length());
  }
  
  return baseTime;
}

String createRomanTime() {
  String romanTime = paddedString(numerals[rtc.getHour(militaryTime)], ' ', 6) + numerals[rtc.getMinute()];
  return romanTime;
}

void displayString(String str) {
  if (DEBUG)
  {
    Serial.print("displaying: ");
    Serial.println(str);
  }
  
  uint8_t tempLength = 0;
  while (str.length() < NUM_DIGITS)
  {
    str = str + ' ';
  }
  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    digits[i].setChar(str.charAt(i), globalRed, globalGreen, globalBlue);
  }
  FastLED.show();
}

int testCounter = 0;
void testDigits() {
  if (DEBUG)
  {
    Serial.print("testCounter: ");
    Serial.println(testCounter);
  }
  
  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    digits[i].erase();
    if(DEBUG) {
      Serial.print("erased digit: ");
      Serial.println(i);
    }
    digits[i].setSegment(testCounter, globalRed, globalGreen, globalBlue);
    if(DEBUG){
      Serial.print("set segment on digit: ");
      Serial.println(i);
    }
  }
  FastLED.show();
  if(DEBUG) {
    Serial.print("showed segment: ");
    Serial.println(testCounter);
  }
  
  testCounter++;
  if (testCounter >= 14)
  {
    testCounter = 0;
  }
}

 
void setup(){
    preferences.begin("big-clock", false);

    //load saved values
    mode = preferences.getUInt("mode", 0);

    wiresTop = preferences.getBool("wires", false);

    clockBase = preferences.getUInt("base", 10);

    globalRed = preferences.getUInt("red", 50);
    globalGreen = preferences.getUInt("green", 0);
    globalBlue = preferences.getUInt("blue", 0);

    militaryTime = preferences.getBool("24h", false);

    //colon
    FastLED.addLeds<NEOPIXEL, 5>(colonLeds, 10);
    colonOff();

    if (wiresTop)
    {
        FastLED.addLeds<NEOPIXEL, 19>(*crgbArrHours, LEDS_IN_DIGIT * (NUM_DIGITS/2));
        FastLED.addLeds<NEOPIXEL, 18>(*crgbArrMinutes, LEDS_IN_DIGIT * (NUM_DIGITS/2));
        //hours
        for (uint8_t i = 0; i < NUM_DIGITS/2; i++)
        {
            // digits[i].begin(crgbArrHours[NUM_DIGITS/2-1-i], 13, 13, 1, false, true, 1);
            digits[i].begin(crgbArrHours[NUM_DIGITS/2-1-i], topIndices);
            digits[i].erase();
        }

        //minutes
        for (uint8_t i = NUM_DIGITS/2; i < NUM_DIGITS; i++)
        {
            // digits[i].begin(crgbArrMinutes[i - (NUM_DIGITS/2)], 13, 13, 1, true, true, 1);
            digits[i].begin(crgbArrMinutes[i - (NUM_DIGITS/2)], topIndices);
            digits[i].erase();
        }
    } else {
        FastLED.addLeds<NEOPIXEL, 18>(*crgbArrHours, LEDS_IN_DIGIT * (NUM_DIGITS/2));
        FastLED.addLeds<NEOPIXEL, 19>(*crgbArrMinutes, LEDS_IN_DIGIT * (NUM_DIGITS/2));

        //hours
        for (uint8_t i = 0; i < NUM_DIGITS/2; i++)
        {
            // digits[i].begin(crgbArrHours[NUM_DIGITS/2-1-i], 13, 13, 1, false, false, 1);
            digits[i].begin(crgbArrHours[NUM_DIGITS/2-1-i], bottomIndices);
            digits[i].erase();
        }

        //minutes
        for (uint8_t i = NUM_DIGITS/2; i < NUM_DIGITS; i++)
        {
            // digits[i].begin(crgbArrMinutes[i - (NUM_DIGITS/2)], 13, 13, 1, true, false, 1);
            digits[i].begin(crgbArrMinutes[i - (NUM_DIGITS/2)], bottomIndices);
            digits[i].erase();
        }
    }


    if (DEBUG)
    {
        Serial.begin(9600);
    }
 
    WiFi.softAP(ssid, password);
 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    Serial.println("got request");
    });

    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OK");
        if (request->hasParam("value")) {
            if (DEBUG) {
                Serial.print("time: ");
                Serial.println(request->getParam("value")->value());
            }
            displayString(request->getParam("value")->value());
            //will recieve with month being zero indexed
            //         s  m  h  d  m  y
            //example: 19,04,19,10,11,2022
            //         0123456789012345678
            String tempString = request->getParam("value")->value();
            int seconds = tempString.substring(0, 2).toInt();
            int minutes = tempString.substring(3, 5).toInt();
            int hours = tempString.substring(6, 8).toInt();
            int days = tempString.substring(9, 11).toInt();
            int month = tempString.substring(12, 14).toInt() + 1;
            int year = tempString.substring(15).toInt();
            rtc.setTime(seconds, minutes, hours, days, month, year);  //seconds, minutes, hours, day of month, month (1 indexed), year
        }
  });

  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (request->hasParam("value")) {
      if (DEBUG) {
        Serial.print("mode: ");
        Serial.println(request->getParam("value")->value());
      }
      mode = request->getParam("value")->value().toInt();
      preferences.putUInt("mode", mode);
    }
  });

  server.on("/24h", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (request->hasParam("value")) 
    {
      if (request->getParam("value")->value() == "true") {
        militaryTime = true;
      } else if (request->getParam("value")->value() == "false") {
        militaryTime = false;
      }
      preferences.putBool("24h", militaryTime);
    }
    if (DEBUG) {
      Serial.print("Set 24h time to: ");
      Serial.println(request->getParam("value")->value());
    }
  });

  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (DEBUG) {
      Serial.print("Set color to: ");
    }
    if (request->hasParam("red")) {
      globalRed = request->getParam("red")->value().toInt();
      preferences.putUInt("red", globalRed);
      if (DEBUG) {
        Serial.print(globalRed);
        Serial.print(", ");
      }
    }
    if (request->hasParam("green")) {
      globalGreen = request->getParam("green")->value().toInt();
      preferences.putUInt("green", globalGreen);
      if (DEBUG) {
        Serial.print(globalGreen);
        Serial.print(", ");
      }
    }
    if (request->hasParam("blue")) {
      globalBlue = request->getParam("blue")->value().toInt();
      preferences.putUInt("blue", globalBlue);
      if (DEBUG) {
        Serial.print(globalBlue);
        Serial.print(", ");
      }
    }
    if (DEBUG) {
        Serial.println();
      }
  });

  server.on("/base", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (request->hasParam("value")) {
      clockBase = request->getParam("value")->value().toInt();
      preferences.putUInt("base", clockBase);
    }
    if (DEBUG) {
      Serial.print("Set to base: ");
      Serial.println(request->getParam("value")->value());
    }
  });

    server.on("/message", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OK");
        if (request->hasParam("value")) {
            message = request->getParam("value")->value();
            if (message.length() > NUM_DIGITS)  //message is too long so it needs to scroll
            {
                messageScrolling = true;
                String spaces = "";
                for (uint8_t i = 0; i < NUM_DIGITS; i++)
                {
                    spaces += ' ';
                }
                message = spaces + message + ' ';
            } else {
                messageScrolling = false;
            }
            
        }
        if (DEBUG) {
            Serial.print("Setting message to: ");
            Serial.println(request->getParam("value")->value());
        }
    });

    server.on("/wires", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OK");
        if (request->hasParam("value")) 
        {
            if (request->getParam("value")->value() == "true") {
                wiresTop = true;
            } else if (request->getParam("value")->value() == "false") {
                wiresTop = false;
            }
            preferences.putBool("wires", wiresTop);
        }
        if (DEBUG) {
            Serial.print("Set wiresTop to: ");
            Serial.println(request->getParam("value")->value());
        }
    });
 
  server.begin();
  
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Started server");
}
 
void loop(){
  if (lastUpdate > 1000)
  {
    lastUpdate = 0;
    if (DEBUG)
    {
      Serial.print("mode: ");
      Serial.println(mode);
    }
    
    switch (mode)
    {
    case 0: //testing
        testDigits();
        colonOn();
        break;
    case 1: //base number mode
        // tempStr = paddedString(rtc.getSecond(), '0', clockBase, 2) + paddedString(rtc.getMinute(), '0', clockBase, 2);
        tempStr = createBaseTime(clockBase);
        colonOn();
        displayString(tempStr);
        break;
    case 2: //message mode
        colonOff();
        if (messageScrolling)
        {
            if (messageIndex > message.length())
            {
                messageIndex = 0;
            }
            displayString(message.substring(messageIndex));
            messageIndex++;
        } else {
            displayString(message);
        }
        break;
    case 3: //romun numeral time mode
        tempStr = createRomanTime();
        colonOn();
        displayString(tempStr);
        break;
    default:
        if(DEBUG) {
            Serial.println("Invalid mode");
        }
        break;
    }
  }
  
}