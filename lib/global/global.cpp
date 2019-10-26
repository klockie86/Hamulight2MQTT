/*
Global settings for MultiJammer based on ESP8266
Jeroen Klock 23-10-2019
*/
////////////////////////////////////////////////////////////////////////////////
#include <global.h>

void Global::init_serial(void){
  DBG_OUTPUT_PORT.begin(DBG_BAUDRATE);
  DBG_OUTPUT_PORT.println("\nInitializing "+name+"\n");
}

String Global::getName(void){
    return name;
}

void RF433::init(void){
  pin_in = RF433_RECEIVE_PIN;
  pin_out = RF433_EMIT_PIN;
  pinMode(pin_in, INPUT);
  pinMode(pin_out, OUTPUT);
}

int RF433::readData(void){
  return digitalRead(pin_in);
}

void RF433::sendData(bool signal){
  return digitalWrite(pin_out,signal);
}
