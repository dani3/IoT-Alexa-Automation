# IoT Alexa Automation

The purpose of this project is to control some components (eg. lights, sensors, etc.) with an Android app and Alexa.

The idea is to have an ESP8266 controlling each individual device that will receive commands from the app and from Alexa. The desired setup will be the following:

* Living room foot lamp controlled by an ESP8266 [called `nodev1`]
* LEDs behind the TV and underneath the chest drawers to produce some ambient lights [called `nodev2`]

## SmartHome setup

* `nodev1`:
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

* `nodev2`:
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
      * Port: `80`
      * Endpoints:
        * `/setup.xml`
        * `/upnp/control/basicevent1`
        * `/eventservice.xml`
        * `/toggleLight`

    * Ambient lights controller
      * Alexa invoke name: `Ambient lights`
      * Port: `81`
      * Endpoints:
        * `/setup.xml`
        * `/upnp/control/basicevent1`
        * `/eventservice.xml`
        * `/toggleLight`

## Alexa Integration

To send commands to the NodeMCU, Alexa has to discover the device. To do that, the Echo Dot will send a __M-SEARCH__ multicast packet, which has to be responded back with the local port of the device and the UUID. Then, Alexa will ask for the __setup.xml__, which has to contain the device name, some endpoints, etc.

Once this is complete, it's prepared to receive commands. One important note, it looks like you have to send the device state after every `SetBinaryState` request. So, if the ESP node receives a `SetBinaryState` command, it will have to answer with a `GetBinaryState`.

On top of that, it looks that the response has to 'make sense', ie. if a turn on request is received, it has to return '1' in the `GetBinaryState`. Else Alexa will say that the device is not responding or it's malfunctioning.

This will force to know the state of the relay as it can be changed from a manual switch.

## Configuration

### Static IPs

Once the app has started, it has to ask every device their status, for that it has to know their IP. Although modern routers will likely assign the same IP to a device, its cache might be restored, so next time it's probably that the IP changes. To solve this, we have to add some entries for our devices [assigning](https://www.howtogeek.com/69612/how-to-set-up-static-dhcp-on-your-dd-wrt-router/) their IPs beforehand.

___

## Components

* [ESP8266-12E](https://www.amazon.de/AZDelivery-NodeMCU-ESP8266-ESP-12E-Development/dp/B06Y1LZLLY/ref=sr_1_1_sspa?ie=UTF8&qid=1546121645&sr=8-1-spons&keywords=esp8266+12&psc=1)
* [Relay module](https://www.amazon.de/gp/product/B07FNGCSZQ/ref=oh_aui_detailpage_o03_s00?ie=UTF8&psc=1)
* [Wago connectors](https://www.amazon.es/s/ref=nb_sb_ss_c_1_5?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&url=search-alias%3Daps&field-keywords=wago+221&sprefix=wago+%2Caps%2C166&crid=234IHCFX20MSH&rh=i%3Aaps%2Ck%3Awago+221)
* [Logic level converter](https://www.sparkfun.com/products/12009)
* [Photocell](https://www.amazon.de/Qualit%C3%A4t-GL5516-Lichtabh%C3%A4ngige-Widerstand-Fotowiderstand/dp/B00NXW9WZ6/ref=sr_1_3?ie=UTF8&qid=1547139741&sr=8-3&keywords=photoresistor)

___

## References

* [Arduino docs](https://www.arduino.cc/reference/en/#page-title)
* [Static IP assignation](https://www.howtogeek.com/69612/how-to-set-up-static-dhcp-on-your-dd-wrt-router/)
* [Setting the HTTP server](https://techtutorialsx.com/2016/10/03/esp8266-setting-a-simple-http-webserver/)
* [WeMo example](https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch)
