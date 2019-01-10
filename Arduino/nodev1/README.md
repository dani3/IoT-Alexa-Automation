# Nodev1

This node will control the foot lamp in the living room.

* Controller: `ESP8266-12E`
* IP: 192.168.1.4
* Alexa invoke name: `Living room light`
* Components:
    * Relay module
    * Photoresistor
    * Logic level shifter
    * 3-way switch
    * 220V/5V Power supply

* Endpoints:
    * `/setup.xml`
    * `/upnp/control/basicevent1`
    * `/eventservice.xml`
    * `/toggleLight`
    * `/getLight`
    * `/setLightThreshold?Threshold=XXX`
    * `/getThreshold`