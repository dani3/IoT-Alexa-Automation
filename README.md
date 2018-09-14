# IoT Alexa Automation

The purpose of this project is to control some components (eg. lights, sensors, etc.) with an Android app and Alexa.

The idea is to have an ESP8266 controlling each individual device that will receive commands from the app and from Alexa.

## Components
* [Wago](https://www.amazon.es/s/ref=nb_sb_ss_c_1_5?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&url=search-alias%3Daps&field-keywords=wago+221&sprefix=wago+%2Caps%2C166&crid=234IHCFX20MSH&rh=i%3Aaps%2Ck%3Awago+221) connectors.

## Static IPs
Once the app has started, it has to ask every device their status, for that it has to know their IP. Although modern routers will likely assign the same IP to a device, its cache might be restored, so next time it's probably that the IP changes. To solve this, we have to add some entries for our devices [assigning](https://www.howtogeek.com/69612/how-to-set-up-static-dhcp-on-your-dd-wrt-router/) their IPs beforehand.

___

## References
* [Arduino docs](https://www.arduino.cc/reference/en/#page-title)
* [Static IP assignation](https://www.howtogeek.com/69612/how-to-set-up-static-dhcp-on-your-dd-wrt-router/)
* [Setting the HTTP server](https://techtutorialsx.com/2016/10/03/esp8266-setting-a-simple-http-webserver/)
