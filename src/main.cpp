/*
Hamulight RF433 sender based on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
Todo:
  - add webinterface
  - receiving signals
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

// Instantiate
Global global;
RF433 rf433;
Hamulight hamulight;
WiFiClient wificlient;
MQTT client(wificlient);


// callback wifimanager need to save config
void saveConfigCallback () {
  DBG_OUTPUT_PORT.println("Should save config");
  client.shouldSave();
}

// callback mqtt received topic
void callback(char* topic, byte* payload, unsigned int length) {
  DBG_OUTPUT_PORT.println("Callback received: "+String(topic));

  // convert message from byte* to String
  char buffer[length];
  memcpy(buffer,payload,length);
  buffer[length] = '\0';
  String received = String(buffer);
  DBG_OUTPUT_PORT.println("Message: "+received);

  // change brightness
  if(strcmp(topic, CMD_BRIGHT_TOPIC) == 0){
    DBG_OUTPUT_PORT.println("setting brightness: "+received);
    hamulight.setbrightness(received.toInt());
    client.publish(STATE_BRIGHT_TOPIC, received.c_str());
  }
  // change command on/off
  else if(strcmp(topic, CMD_TOPIC) == 0){
    DBG_OUTPUT_PORT.println("Swithing");
    if((String)received == "ON"){
      hamulight.switchOn();
    }
    else if((String)received == "OFF"){
      hamulight.switchOff();
    }
    client.publish(STATE_TOPIC, received.c_str());
  }
}


void setup(){
  DBG_OUTPUT_PORT.println("Setting up");
  // read settings from SPIFFS
  client.readSettings();

  // show custom parameters in wifi webinterface
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server",client.getServerName().c_str(), 40);
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

  // read updated parameters
  client.setServerName(custom_mqtt_server.getValue());
  client.setPort(custom_mqtt_port.getValue());
  client.setUser(custom_mqtt_user.getValue());
  client.setPass(custom_mqtt_pass.getValue());

  // set MQTT server
  client.setServer();//client.getServerName().c_str(), client.getPort().toInt());

  // store config
  client.saveSettings();
  client.setCallback(callback);
  DBG_OUTPUT_PORT.println("Setting up done.");
}

void loop() {
  // make MQTT connection
  if (WiFi.status() == WL_CONNECTED){
    if (!client.connected()) {
      client.reconnect(global.getName());
      // no real exit function when no connection yet. Needs to be fixed
    }
    else{
      client.loop();
    }
  }
  else{
      DBG_OUTPUT_PORT.println("Wifi not connected");
  }
}
