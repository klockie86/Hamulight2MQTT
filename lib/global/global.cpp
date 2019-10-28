/*
Global settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////

#include <global.h>

Global::Global(){
  DBG_OUTPUT_PORT.begin(DBG_BAUDRATE);
  DBG_OUTPUT_PORT.println("\nInitializing "+name+"\n");
}

String Global::getName(void){
    return name;
}

String Global::getPass(void){
    return pass;
}
