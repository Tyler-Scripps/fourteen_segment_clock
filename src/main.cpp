#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "webpage.h"  //stores the webpage that will be served
#include "fourteen_segment_digit.h"
#include <ESP32Time.h>  //for accessing rtc
#include <elapsedMillis.h>  //provides timing function


#define NUM_DIGITS 12
#define LEDS_IN_DIGIT 251
#define DEBUG true
 
const char *ssid = "bigclock";
const char *password = "merrychristmas";

uint8_t globalRed = 50;
uint8_t globalGreen = 0;
uint8_t globalBlue = 0;

bool militaryTime = false;
uint8_t clockBase = 10;

int mode = 0;
elapsedMillis lastUpdate;
String tempStr;

ESP32Time rtc;
 
AsyncWebServer server(80);

CRGB crgbArr[NUM_DIGITS][LEDS_IN_DIGIT];
fourteen_segment_digit digits[NUM_DIGITS];

String paddedString(int unpadded, char padChar, uint8_t base, uint8_t numdigits) {
  // if (unpadded < 10 || unpadded > -10)  //check if between -10 and 10
  // {
  //   return '0' + String(unpadded);
  // } else {
  //   return String(unpadded);
  // }
  String newStr = String(unpadded, base);
  while (newStr.length() < numdigits)
  {
    newStr = padChar + newStr;
  }
  return newStr;
}

String createBaseTime(uint8_t base) {
  String baseTime = paddedString(rtc.getHour(militaryTime), ' ', base, 6);
  baseTime += String(rtc.getMinute(), base);
  if (base >= 8) //bases less than 8 require 3 digits to represent decimal 59 so it will be hard to read on only 6 digits for minutes and seconds
  {
    baseTime += paddedString(rtc.getSecond(), ' ', base, NUM_DIGITS - baseTime.length());
  }
  
  return baseTime;
}

void displayString(String str) {
  if (DEBUG)
  {
    Serial.print("displaying: ");
    Serial.println(str);
  }
  
  uint8_t tempLength = 0;
  if (str.length() < NUM_DIGITS) {
    tempLength = str.length();
  } else {
    tempLength = NUM_DIGITS;
  }
  for (uint8_t i = 0; i < tempLength; i++)
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
    // if (DEBUG)
    // {
    //   Serial.print("i: ");
    //   Serial.println(i);
    //   // Serial.print(" ");
    //   Serial.print("mode: ");
    //   Serial.println(mode);
    // }
    digits[i].erase();
    digits[i].setSegment(testCounter, globalRed, globalGreen, globalBlue);
    // if (DEBUG) {
    //   Serial.print("mode: ");
    //   Serial.println(mode);
    //   Serial.println();
    // }
  }
  FastLED.show();
  // if (DEBUG)
  // {
  //   // Serial.println();
  //   Serial.println("finished setting digits");
  //   Serial.print("mode: ");
  //   Serial.println(mode);
  // }
  
  testCounter++;
  if (testCounter >= 14)
  {
    testCounter = 0;
  }
  
  // if (DEBUG)
  // {
  //   Serial.println("finished testDigits()");
  // }
  
}

 
void setup(){
  FastLED.addLeds<NEOPIXEL, 19>(crgbArr[0], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 18>(crgbArr[1], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 5>(crgbArr[2], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 17>(crgbArr[3], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 16>(crgbArr[4], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 4>(crgbArr[5], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 0>(crgbArr[6], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 2>(crgbArr[7], LEDS_IN_DIGIT);

  FastLED.addLeds<NEOPIXEL, 32>(crgbArr[8], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 33>(crgbArr[9], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 25>(crgbArr[10], LEDS_IN_DIGIT);
  FastLED.addLeds<NEOPIXEL, 26>(crgbArr[11], LEDS_IN_DIGIT);

  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    digits[i].begin(crgbArr[i], 20, 19, 1);
    digits[i].erase();
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
    }
    if (DEBUG) {
      Serial.print("Set 24h time to: ");
      Serial.println(request->getParam("value")->value());
    }
  });

  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (DEBUG) {
      Serial.print("Set color to: ");
    }
    if (request->hasParam("red")) {
      globalRed = request->getParam("red")->value().toInt();
      if (DEBUG) {
        Serial.print(globalRed);
        Serial.print(", ");
      }
    }
    if (request->hasParam("green")) {
      globalGreen = request->getParam("green")->value().toInt();
      if (DEBUG) {
        Serial.print(globalGreen);
        Serial.print(", ");
      }
    }
    if (request->hasParam("blue")) {
      globalBlue = request->getParam("blue")->value().toInt();
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
    if (request->hasParam("value")) {
      clockBase = request->getParam("value")->value().toInt();
    }
    if (DEBUG) {
      Serial.print("Set to base: ");
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
      break;
    case 1: //base number mode
      // tempStr = paddedString(rtc.getSecond(), '0', clockBase, 2) + paddedString(rtc.getMinute(), '0', clockBase, 2);
      tempStr = createBaseTime(clockBase);
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