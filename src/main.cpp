/*
MultiJammer based on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/LedWall
Todo:
  deep sleep
*/
////////////////////////////////////////////////////////////////////////////////

// Custom libaries
#include <global.h>
//needed for WifiManager
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
//needed for MQTT
#include <PubSubClient.h>
//#include <ArduinoJson.h>

Global global;
RF433 rf433;
Hamulight hamulight;

WiFiClient wificlient;
PubSubClient client(wificlient);

void setup(){
  DBG_OUTPUT_PORT.println("\nnitializing "+global.getName()+"\n");
  WiFiManager wifiManager;
  wifiManager.autoConnect(global.getName().c_str(), "configHamu");
}

void loop() {
  hamulight.send(hamulight.on_off);
  delay(2000);
}
