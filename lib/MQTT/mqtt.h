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
#define BASE_TOPIC "/home"
#define VERSION_TOPIC "/version"
#define WILL_TOPIC "/LWT"
#define WILL_QOS 0
#define WILL_RETAIN true
#define WILL_MESSAGE "offline"
#define GW_ANNOUNC "online"

#define CMD_TOPIC "/commands/#"
#define TO_TOPIC "toMQTT"
#define FROM_TOPIC "MQTTto"

#define CONFIGFILE "/config.json"


#include <Arduino.h>
#include <global.h>

//needed for MQTT
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <WiFiManager.h>

//for storing values
#include <FS.h>


class MQTT: public PubSubClient{
private:
  const int maxRetry = 5;
  int failures = 0;
  //MQTT Parameters definition
  String user = "your_username";
  String pass = "your_password";
  String server = "192.168.1.100";
  String port = "1883";
  bool save;

public:
  MQTT(WiFiClient);
  String getUser(void);
  String getPass(void);
  String getServer(void);
  String getPort(void);

  void setUser(String);
  void setPass(String);
  void setServer(String);
  void setPort(String);

  void shouldSave(void);
  void saveSettings(void);
  void readSettings(void);

  void reconnect(String);
};


////////////////////////////////////////////////////////////////////////////////
#endif
