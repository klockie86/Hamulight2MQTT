/*
Hamulight RF433 sender based on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
Todo:
  - reduce repetitions of transmit data
  - return brightness on switch on
  - add favicon
  - receiving RF signals
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
// webserver
#include <spiffs_webserver.h>

////////////////////////////////////////////////////////////////////////////////
// Instantiate
////////////////////////////////////////////////////////////////////////////////

Global global;
RF433 rf433;
Hamulight hamulight;
WiFiClient wificlient;
MQTT client(wificlient);
Webserver server(WEBSERVER_PORT);

////////////////////////////////////////////////////////////////////////////////
// Calback functions
////////////////////////////////////////////////////////////////////////////////
// wifimanager need to save config
void saveConfigCallback () {
  DBG_OUTPUT_PORT.println("Should save config");
  client.shouldSave();
}

// mqtt received topic
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
    // send back state
    if(hamulight.getState()){
      client.publish(STATE_TOPIC, "ON");
    }
    else{
      client.publish(STATE_TOPIC, "OFF");
    }
  }
}

// webserver set state
void setState(){
  String sRec = server.arg("State"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
//  iRecState = sRecState.toInt();
  DBG_OUTPUT_PORT.println("Webserver received LEDstate: "+ sRec);
  hamulight.toggle();
  if (hamulight.getState()){
    server.send(200, "text/plain", "true");
  }
  else{
    server.send(200, "text/plain", "false");
  }
}

// webserver set brightness
void setBrightness(){
  String sRec = server.arg("Brightness");
  DBG_OUTPUT_PORT.println("Webserver received Brightness: "+ sRec);
  hamulight.setbrightness(sRec.toInt());
  server.send(200, "text/plain", sRec);
}

// webserver reset wifi
void reset(){
  String sRec = server.arg("Reset");
  DBG_OUTPUT_PORT.println("Reset Settings");
  server.send(200, "text/plain", sRec);

  // start wifimanager
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.reset();
}


////////////////////////////////////////////////////////////////////////////////
// Main functions
////////////////////////////////////////////////////////////////////////////////
void setup(){
  DBG_OUTPUT_PORT.println("Setting up...");
  // read settings from SPIFFS
  client.readSettings();

  DBG_OUTPUT_PORT.println("Setting up Wifimanager");
  // show custom parameters in wifi webinterface
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server",client.getServerName().c_str(), 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", client.getPort().c_str(), 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", client.getUser().c_str(), 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", client.getPass().c_str(), 30);

  // start wifimanager
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // update parameters from wifi webinterface
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);

  // make connection
  wifiManager.autoConnect(global.getName().c_str(), global.getPass().c_str());

  DBG_OUTPUT_PORT.println("Setting up MQTT");
  // read updated parameters
  client.setServerName(custom_mqtt_server.getValue());
  client.setPort(custom_mqtt_port.getValue());
  client.setUser(custom_mqtt_user.getValue());
  client.setPass(custom_mqtt_pass.getValue());

  // set MQTT server
  client.setServer();

  // store config
  client.saveSettings();
  client.setCallback(callback);

  DBG_OUTPUT_PORT.println("Setting up Webserver");

  // callback functions webserver
  server.on("/setState", setState);
  server.on("/setBrightness", setBrightness);
  server.on("/reset", reset);
  server.onNotFound([](){
    server.handleNotFound();
  });

  // start webserver
  server.begin();
  DBG_OUTPUT_PORT.println("Setting up done.");
}

void loop() {
  server.handleClient();
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
