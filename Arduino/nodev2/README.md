# Nodev2

This node will control the ambient light in the living room.

* Controller: `ESP8266-12E`
* IP: 192.168.1.9
* Components:
    * ESP8266-12E
    * 4M + 3M Led strips
    * 10A - 5V Power supply
    * 5A - 5V Power supply
    * Logic level shifter

* Wemos attached:
    * TV LEDs controller
        * Alexa invoke name: `Living room light`
        * Port: 80
        * Endpoints:
            * `/setup.xml`
            * `/upnp/control/basicevent1`
            * `/eventservice.xml`
            * `/toggleLight`