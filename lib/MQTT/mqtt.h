/*
MQTT settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 28-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef mqtt_h
#define mqtt_h
/*-------------DEFINE YOUR MQTT ADVANCED PARAMETERS BELOW----------------*/
#define NAME "HamuLight2MQTT"
#define BASE_TOPIC "homeassistant"
#define VERSION_TOPIC BASE_TOPIC "/" NAME "/version"
#define WILL_TOPIC BASE_TOPIC "/" NAME "/LWT"
#define WILL_QOS 0
#define WILL_RETAIN true
#define WILL_MESSAGE "offline"
#define GW_ANNOUNC "online"

#define LOCATION "terras"
#define CMD_TOPIC BASE_TOPIC "/light/" LOCATION "/switch"
#define STATE_TOPIC BASE_TOPIC "/light/" LOCATION "/status"
#define CMD_BRIGHT_TOPIC BASE_TOPIC "/light/" LOCATION "/brightness/set"
#define STATE_BRIGHT_TOPIC BASE_TOPIC "/light/" LOCATION "/brightness/status"

#define CONFIGFILE "/config.json"


#include <Arduino.h>
#include <global.h>

#include <WiFiManager.h>

//needed for MQTT
#include <PubSubClient.h>
#include <ArduinoJson.h>

//for storing values
#include <FS.h>

class MQTT: public PubSubClient{
private:
  const int maxRetry = 5;  // max reconnection tries, 0 = endless
  int failures = 0;
  //MQTT Parameters definition
  String user = "your_username";
  String pass = "your_password";
  String server = "192.168.1.100";
  String port = "1883";
  bool save;
  bool failed = false;
  WiFiClient client;

public:
  MQTT(WiFiClient client):PubSubClient(client){};
  String getUser(void);
  String getPass(void);
  String getServerName(void);
  String getPort(void);

  void setUser(String);
  void setPass(String);
  void setServerName(String);
  void setPort(String);

  void shouldSave(void);
  void saveSettings(void);
  void readSettings(void);

  void setServer();
  void reconnect(String);
};


////////////////////////////////////////////////////////////////////////////////
#endif
