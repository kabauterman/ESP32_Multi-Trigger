# ESP32_Multi-Trigger
This is a Camera WLAN-Remote Trigger based on ESP32 NodeMCU

Requirements are:

https://github.com/espressif/arduino-esp32
and
https://github.com/nhatuan84/esp32-webserver

What it actually can do at the moment:

- Simple Intervalometer Trigger (Interval in Seconds, Runtime in Minutes)
- Simple Remote Timer (in Seconds)
- Simple Remote Trigger (single Shot)
- Simple BULB Trigger Start/Stop (can also be used for burst depending on Camera Mode)

What may be possible:

- Triggering from different Sensors (Light, Sound and so on)
- BULB Intervalometer for Startrails
- everything you can imagine

WLAN SSID is: Intervalometer
WLAN KEY is: 12345678

(can be changed to your needs)

I used the Pins

32 for Shutter
35 for Focus
33 for Ground

But this depends on your NodeMCU Board and your Wiring.
