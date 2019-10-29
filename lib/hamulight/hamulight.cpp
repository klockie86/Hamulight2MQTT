/*
Hamulight settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#include <hamulight.h>

Hamulight::Hamulight(){
    pin_in = RF433_RECEIVE_PIN;
    pin_out = RF433_EMIT_PIN;
}

void Hamulight::switchOn(){
    if(!state){
      send(on_off);
    }
    state = true;
};

void Hamulight::switchOff(){
    if(state){
      send(on_off);
    }
    state = false;
};

void Hamulight::setbrightness(unsigned int val){
  if(val > maxBright){
    DBG_OUTPUT_PORT.println("brightness out of range: "+val);
  }
  else{
    if(val < (maxBright / 4)){
      send(bright25);
    }
    else if(val < (maxBright / 2)){
      send(bright50);
    }
    else if(val < (3 * (maxBright / 4))){
      send(bright75);
    }
    else{
      send(bright100);
    }
  }
};


void Hamulight::send(Hamulight::command command){
    DBG_OUTPUT_PORT.println("sending state: "+(String)command);
    for(int j = 0; j < REPEATE ;j++){
        for(int i = 0; i < 148;i++){
            sendData(bitstreams[command][i]);
            delayMicroseconds(period);
        }
        sendData(false);
    }
}
