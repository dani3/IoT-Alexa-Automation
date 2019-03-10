# Nodev2

This node will control the ambient light in the living room.

* Controller: `ESP8266-12E`
* IP: 192.168.1.5
* Components:
    * ESP8266-12E
    * 4M + 3M Led strips
    * 10A - 5V Power supply
    * 5A - 5V Power supply
    * Arduino Mega

* Wemos attached:
    * TV LEDs controller
        * Alexa invoke name: `Ambilight`
        * Port: 80
        * Endpoints:
            * `/setup.xml`
            * `/upnp/control/basicevent1`
            * `/eventservice.xml`
            * `/toggleLight`

* Wemos attached:
    * Ambient lights controller
        * Alexa invoke name: `Ambient lights`
        * Port: 81
        * Endpoints:
            * `/setup.xml`
            * `/upnp/control/basicevent1`
            * `/eventservice.xml`
            * `/toggleLight`