# Hamulight2MQTT
Hamulight2MQTT
Controlling the Hamulight L2046 and L2045 leds (RF 433 MhZ) via MQTT.
This software replicates the RF signals from the remote control.

## How it works
The hamulight remote sends out a 148 bit signal, probably a tri bit signal.
I've not been able to decode the logic of setting the brightness.
The bitstreams:
### on_off
  ```1010101011111100000010001000100010001000111011101110111010001000111010001000100011101110100011101000100010001000100011101000100010001110111010001110```
### brightness 25%
	```1010101011111100000010001000100010001000111011101110111010001000111010001000100011101110100011101000111010001110100011101000100011101000111011101110```
### brightness 50%
	```1010101011111100000010001000100010001000111011101110111010001000111010001000100011101110100011101000111010001000111011101000100011101000111011101000```
### brightness 75%
	```1010101011111100000010001000100010001000111011101110111010001000111010001000100011101110100011101000111011101110111011101000100011101110111010001000```
### brightness 100%
	```1010101011111100000010001000100010001000111011101110111010001000111010001000100011101110100011101000100011101110100011101000100011101000100011101110```


## Homeassistant configuration
```
light:
  - platform: mqtt
    name: "Terrasverlichting"
    state_topic: "homeassistant/light/LOCATION/status"
    command_topic: "homeassistant/light/LOCATION/switch"
    brightness_state_topic: "homeassistant/light/LOCATION/brightness/status"
    brightness_command_topic: "homeassistant/light/LOCATION/brightness/set"
    state_value_template: "{{ value_json.state }}"
    brightness_value_template: "{{ value_json.brightness }}"
    qos: 0
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
```
