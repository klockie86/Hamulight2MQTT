/*
Hamulight settings for Hamulight mqtt remote on ESP8266
Jeroen Klock 23-10-2019
See: https://github.com/klockie86/Hamulight2MQTT
*/
////////////////////////////////////////////////////////////////////////////////
#include <hamulight.h>

Hamulight::Hamulight(){
    DBG_OUTPUT_PORT.println("Initializing hamulight");
    pin_in = RF433_RECEIVE_PIN;
    pin_out = RF433_EMIT_PIN;
}

void Hamulight::send(Hamulight::state state){
    DBG_OUTPUT_PORT.println("sending state: "+(String)state);
    for(int j = 0; j < REPEATE ;j++){
        for(int i = 0; i < 148;i++){
            sendData(bitstreams[state][i]);
            delayMicroseconds(period);
        }
        sendData(false);
    }
}
