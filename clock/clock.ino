#include <ESPUI.h>
#include <ESPUIclient.h>
#include <ESPUIclientFsm.h>
#include <ESPUIcontrol.h>
#include <dataControlsJS.h>
#include <dataGraphJS.h>
#include <dataIndexHTML.h>
#include <dataNormalizeCSS.h>
#include <dataSliderJS.h>
#include <dataStyleCSS.h>
#include <dataTabbedcontentJS.h>
#include <dataZeptoJS.h>

#include "./src/14-segment-digit/fourteen_segment_digit.h"

#include <DNSServer.h>
#include <WiFi.h>

#include <Preferences.h>

#define NUM_DIGITS 1

//wifi stuff
Preferences preferences;
const char* hostname = "segClock";
String ssid = "ESPUI";
String password = "espui";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

// espui variables
int timeLabelId;
int timeId;

String tempSSID;
String tempPassword;

String timeStr;
String text;
int mode = 0;

CRGB arr1[251];
fourteen_segment_digit digit1; 

// ESPUI callbacks
void timeCallback(Control *sender, int type) {
  if(type == TM_VALUE) { 
    timeStr = sender->value.substring(sender->value.indexOf("T") + 1, sender->value.length() - 5);
    Serial.print("Time: ");
    Serial.println(timeStr);
    ESPUI.print(timeLabelId, timeStr);
  }
}

void timeButtonCallback(Control* sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        // Serial.println("Button DOWN");
        break;

    case B_UP:
        Serial.println("Updating Time");
        ESPUI.updateTime(timeId);
        break;
    }
}

void wifiButtonCallback(Control* sender, int type) {
  if (type == B_UP) {
    ssid = tempSSID;
    password = tempPassword;
    Serial.print("ssid: ");
    Serial.println(ssid);
    Serial.print("password: ");
    Serial.println(password);
    preferences.putString("ssid", ssid); 
    preferences.putString("password", password);
  }
}

void wifiSSIDCallback(Control* sender, int type) {
  tempSSID = sender->value;
}

void wifiPasswordCallback(Control* sender, int type) {
  tempPassword = sender->value;
}

void messageCallback(Control* sender, int type) {
  // text = sender->value;
  String temp = "";
  for(int i = 0; i < NUM_DIGITS; i++) {
    temp.concat(' ');
  }
  text = temp + sender->value + temp;
}

void modeSelectCallback(Control* sender, int value)
{
  mode = sender->value.toInt();
  // Serial.print("mode: ");
  // Serial.println(mode);
}

void colorCallback(Control* sender, int value) {
  int number = (int) strtoul( &sender->value[1], NULL, 16);
  // Split them up into r, g, b values
  // globalRed = number >> 16;
  // globalGreen = number >> 8 & 0xFF;
  // globalBlue = number & 0xFF;
}

void setup() {
  FastLED.addLeds<NEOPIXEL, 19>(arr1, 251);
  digit1.begin(arr1, 20, 19, 1);
  digit1.erase();
  delay(1000);
  digit1.fill(50, 50, 50);
  FastLED.show();
  delay(1000);
  digit1.erase();
}


void loop() {
  // for (uint8_t i = 0; i < 14; i++)
  // {
  //   digit1.erase();
  //   digit1.setSegment(i, 50, 50, 50);
  //   FastLED.show();
  //   delay(1000);
  // }
  // digit1.erase();
  // digit1.setSegments(0b0011111100000000, 50, 50, 50);
  // FastLED.show();
  // delay(2000);
  // digit1.fill(50,50,50);
  // FastLED.show();
  // delay(2000);

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