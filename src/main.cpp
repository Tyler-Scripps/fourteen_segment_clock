#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "webpage.h"
#include "fourteen_segment_digit.h"

#define NUM_DIGITS 12
 
const char *ssid = "bigclock";
const char *password = "merrychristmas";
 
AsyncWebServer server(80);

CRGB crgbArr[NUM_DIGITS][171];
fourteen_segment_digit digits[NUM_DIGITS];

void displayString(String str) {
  uint8_t tempLength = 0;
  if (str.length() < NUM_DIGITS) {
    tempLength = str.length();
  } else {
    tempLength = NUM_DIGITS;
  }
  for (uint8_t i = 0; i < tempLength; i++)
  {
    digits[i].setChar(str.charAt(i), 50, 0, 0);
  }
  FastLED.show();
}

 
void setup(){
  FastLED.addLeds<NEOPIXEL, 19>(crgbArr[0], 171);
  FastLED.addLeds<NEOPIXEL, 18>(crgbArr[1], 171);
  FastLED.addLeds<NEOPIXEL, 5>(crgbArr[2], 171);
  FastLED.addLeds<NEOPIXEL, 17>(crgbArr[3], 171);
  FastLED.addLeds<NEOPIXEL, 16>(crgbArr[4], 171);
  FastLED.addLeds<NEOPIXEL, 4>(crgbArr[5], 171);
  FastLED.addLeds<NEOPIXEL, 0>(crgbArr[6], 171);
  FastLED.addLeds<NEOPIXEL, 2>(crgbArr[7], 171);

  FastLED.addLeds<NEOPIXEL, 32>(crgbArr[8], 171);
  FastLED.addLeds<NEOPIXEL, 33>(crgbArr[9], 171);
  FastLED.addLeds<NEOPIXEL, 25>(crgbArr[10], 171);
  FastLED.addLeds<NEOPIXEL, 26>(crgbArr[11], 171);

  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    digits[i].begin(crgbArr[i], 20, 19, 1);
    digits[i].erase();
  }
  


  Serial.begin(9600);
 
  WiFi.softAP(ssid, password);
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    Serial.println("got request");
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (request->hasParam("value")) {
      Serial.print("time: ");
      Serial.println(request->getParam("value")->value());
      displayString(request->getParam("value")->value());
    }
  });
 
  server.begin();
  
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Started server");
}
 
void loop(){

}