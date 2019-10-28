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
// MQTT
#include <mqtt.h>

Global global;
RF433 rf433;
Hamulight hamulight;
WiFiClient wificlient;
MQTT client(wificlient);



//callback notifying us of the need to save config
void saveConfigCallback () {
  DBG_OUTPUT_PORT.println("Should save config");
  client.shouldSave();
}



void setup(){
  DBG_OUTPUT_PORT.println("Setting up");
  // read settings from SPIFFS
  client.readSettings();

  // show custom parameters in wifi webinterface
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server",client.getServer().c_str(), 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", client.getPort().c_str(), 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", client.getUser().c_str(), 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", client.getPass().c_str(), 30);

  // start wifimanager
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // update paratmeters from wifi webinterface
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);

  // make connection
  wifiManager.autoConnect(global.getName().c_str(), global.getPass().c_str());

  //read updated parameters
  client.setServer(custom_mqtt_server.getValue());
  client.setPort(custom_mqtt_port.getValue());
  client.setUser(custom_mqtt_user.getValue());
  client.setPass(custom_mqtt_pass.getValue());

  // store config
  client.saveSettings();
  DBG_OUTPUT_PORT.println("Setting up done.");
}

void loop() {
  // make MQTT connection
  if (!client.connected()) {
    client.reconnect(global.getName());
  }
  else{
    client.loop();
  }
  hamulight.send(hamulight.on_off);
  delay(2000);
}
