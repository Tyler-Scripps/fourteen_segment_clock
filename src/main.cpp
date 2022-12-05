#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "webpage.h"
 
const char *ssid = "bigclock";
const char *password = "merrychristmas";
 
AsyncWebServer server(80);
 
void setup(){
  Serial.begin(9600);
 
  WiFi.softAP(ssid, password);
 
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
    if (request->hasParam("value")) {
        Serial.print("time: ");
        Serial.println(request->getParam("value")->value());
    }
  });
 
  server.begin();
}
 
void loop(){}