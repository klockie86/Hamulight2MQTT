/*
Hamulight settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#include <hamulight.h>

// default constructor
Hamulight::Hamulight(int pin_receive,int pin_emit){
    pin_in = pin_receive;
    pin_out = pin_emit;
}


// switch on and off, since there's only a code for toggling, remember the state
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

void Hamulight::toggle(){
    state = !state;
    send(on_off);
};

bool Hamulight::getState(void){
  return state;
}

// set brightness. Only 25, 50 ,75 and 100% available.
// scale 0-255 down to correct code
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

// Send the code
void Hamulight::send(Hamulight::command command){
    DBG_OUTPUT_PORT.println("sending state: "+(String)command);
    for(int j = 0; j < REPEATE ;j++){
        for(int i = 0; i < 148;i++){
            sendData(bitstreams[command][i]);
            delayMicroseconds(period);
        }
        sendData(false);
    }
    delayMicroseconds(period*REPEATE);
}
