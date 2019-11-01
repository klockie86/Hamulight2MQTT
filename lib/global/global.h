/*
Global settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef global_h
#define global_h

#include <Arduino.h>

// custom libraries
#include <hamulight.h>

////////////////////////////////////////////////////////////////////////////////
// Settings
////////////////////////////////////////////////////////////////////////////////
// Serial for logging
#define DBG_OUTPUT_PORT Serial
#define DBG_BAUDRATE  115200
// GPIO assignment
#define RF433_RECEIVE_PIN D2
#define RF433_EMIT_PIN D3

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////
class Global{
private:
  String name = "HamuLight2MQTT";
  String pass = "configHamu";
public:
  Global();
  String getName(void);
  String getPass(void);
};

////////////////////////////////////////////////////////////////////////////////
#endif
