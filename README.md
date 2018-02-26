# ESP32_Multi-Trigger
This is a Camera WLAN-Remote Trigger based on ESP32 NodeMCU

Pure Parts Price:
- If you buy cheaper from China (Banggood): ca. 9 € without the Cable
- If you buy from Amazon.de: ca. 16 € without the Cable

Requirements are:

https://github.com/espressif/arduino-esp32
and
https://github.com/nhatuan84/esp32-webserver
and if you want to compile the OLED Version
https://github.com/ThingPulse/esp8266-oled-ssd1306

The Libaries go to Documents/Arduino/Libaries
For the Board Description follow the instructions in the Link.

What it actually can do at the moment:

- Simple Intervalometer Trigger (Interval in Seconds, Runtime in Minutes)
- Simple Remote Timer (in Seconds)
- Simple Remote Trigger (single Shot)
- Simple BULB Trigger Start/Stop (can also be used for burst depending on Camera Mode)

What may be possible:

- Triggering from different Sensors (Light, Sound and so on)
- BULB Intervalometer for Star-Trails
- everything you can imagine

What is on my To-Do-List:

- making more Prototypes and create a 3D Printed Housing
- testing Hot-Shoe mounts to get Power from the Camera

WLAN SSID is: Multi-Trigger
WLAN KEY is: 12345678

Power is provided over USB by a Powerbank or by using the Battery Module WeMos ESP32 Board (much bigger).

(can be changed to your needs)

Pins I used on the NodeMCU Version:

32 for Shutter
35 for Focus
33 for Ground (not needed if you solder this pin to GND.)

Pins I used on the WeMos OLED & Battery Version:

26 for Shutter
25 for Focus
27 for Ground (not needed if you solder this pin to GND.)

Don't forget to choose the right Board before compiling.

Parts List:

- ESP32 NodeMCU Board (Amazon.de*:http://amzn.to/2GDF24A Banggood*:http://bit.ly/2Ey2kZ2)
- or WeMos OLED Board (Amazon.de*:http://amzn.to/2osjAYF Banggood*:http://bit.ly/2sTLZvs)
- or WeMos OLED Battery Board (Amazon.de*: Banggood*:http://bit.ly/2HLnuE2)
- 2,5mm Stereo 3-Pin Female Connector (http://bit.ly/2ny1oMh)
- Remote Trigger Cable (in my Case for Sony) (Amazon.de*:http://amzn.to/2E1GwY3 or http://amzn.to/2Ec6LLt)
- Wires
- soldering Iron
- soldering Tin

just needed of you don't solder directly:
- 24x10 Pins Double Side PCB (Amazon.de*:http://amzn.to/2DWnwLg Banggood*:http://bit.ly/2nzSdLi)
- Female Header Connector for PCB (Amazon.de*:http://amzn.to/2FF5auH Banggood*:http://bit.ly/2rZof8Z)


*This are Affiliate Links that give me some little provision for each sale done over them.
