/*
MQTT settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 28-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#include <mqtt.h>

// default constructor
// get and set functions
String MQTT::getUser(void){
  return user;
};

String MQTT::getPass(void){
  return pass;
};

String MQTT::getServerName(void){
  return server;
};

String MQTT::getPort(void){
  return port;
};

void MQTT::setUser(String val){
  user = val;
};

void MQTT::setPass(String val){
  pass = val;
};

void MQTT::setServerName(String val){
  server = val;
};

void MQTT::setPort(String val){
  port = val;
};

// for storing settings in SPIFFS
void MQTT::shouldSave(void){
  save = true;
};


// overload function from pubsub
void MQTT::setServer(void){
  PubSubClient::setServer(server.c_str(), port.toInt());
}

// read values from spiffs
void MQTT::readSettings(void){
  if (SPIFFS.begin()) {
      DBG_OUTPUT_PORT.println("mounted file system");
    }else{
      DBG_OUTPUT_PORT.println("failed to mount FS -> formating");
      SPIFFS.format();
      if(SPIFFS.begin()){
        DBG_OUTPUT_PORT.println("mounted file system after formating");
      }
    }
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      DBG_OUTPUT_PORT.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        DBG_OUTPUT_PORT.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          DBG_OUTPUT_PORT.println("\nparsed json");
          server = json["mqtt_server"].as<char *>();
          port = json["mqtt_port"].as<char *>();
          user = json["mqtt_user"].as<char *>();
          pass = json["mqtt_pass"].as<char *>();
        }
        else {
          DBG_OUTPUT_PORT.println("failed to load json config");
        }
      }
    }
};

// store values in spiffs
void MQTT::saveSettings(void){
  if(save){
    DBG_OUTPUT_PORT.println("Saving MQTT settings");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    json["mqtt_server"] = server;
    json["mqtt_port"] = port;
    json["mqtt_user"] = user;
    json["mqtt_pass"] = pass;

    File configFile = SPIFFS.open(CONFIGFILE, "w");
    if (!configFile) {
      DBG_OUTPUT_PORT.println("ERROR: configfile could not be opened");
    }
    json.printTo(DBG_OUTPUT_PORT);
    json.printTo(configFile);
    configFile.close();
    }
};


// re-connect to server
void MQTT::reconnect(String name){
    // Loop until we're reconnected
    while(!PubSubClient::connected()) {
      DBG_OUTPUT_PORT.println("Attempting to connect to MQTT broker as "+name);
      if (connect(name.c_str(),user.c_str(), pass.c_str(), WILL_TOPIC, WILL_QOS, WILL_RETAIN, WILL_MESSAGE)){
        DBG_OUTPUT_PORT.println("connected");
        failures = 0;
        // Once connected, publish an announcement...
        publish(WILL_TOPIC, GW_ANNOUNC, WILL_RETAIN);
        // Subscribe to topic
        if (subscribe(CMD_TOPIC)){
          DBG_OUTPUT_PORT.println("subscribed to topic: "+(String)CMD_TOPIC);
        }
        if (subscribe(CMD_BRIGHT_TOPIC)){
          DBG_OUTPUT_PORT.println("subscribed to topic: "+(String)CMD_BRIGHT_TOPIC);
        }

      }
      else{
        DBG_OUTPUT_PORT.println("failed connecting to mqtt, try again in 5 sec");
        if (maxRetry != 0){
          failures ++;
          if (failures > maxRetry){
            // no real exit function yet. Needs to bee fixed
            DBG_OUTPUT_PORT.println("connection mqtt timeout");
            exit(1);
          }
        }
      delay(5000);
      }
    }
};
