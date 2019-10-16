# Nodev1

This node will control the __foot lamp__ in the living room.

* Controller: `ESP8266-12E`
* IP: 192.168.1.4
* Components:
  * ESP8266-12E
  * Relay module
  * Photoresistor
  * Logic level shifter
  * 3-way switch
  * 220V/5V Power supply

* Wemos attached:
  * Foot lamp
    * Alexa invoke name: `Living room light`
    * Port: `80`
    * Endpoints:
      * `/setup.xml`
      * `/upnp/control/basicevent1`
      * `/eventservice.xml`
      * `/toggleLight`
      * `/getLight`
      * `/setLightThreshold?Threshold=XXX`
      * `/getThreshold`
