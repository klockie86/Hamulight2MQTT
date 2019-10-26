/*
RF433 settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#include <RF433.h>

RF433::RF433(){
  pinMode(pin_in, INPUT);
  pinMode(pin_out, OUTPUT);
}

int RF433::readData(void){
  return digitalRead(pin_in);
}

void RF433::sendData(bool signal){
  return digitalWrite(pin_out,signal);
}
