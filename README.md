# Numi-ESP8266
Numi is an open source ESP8266 numitron clock. It's designed to be as low profile as possible, powered using USB-PD.

**Wiki**  
Software-wise it is based on my Flora-ESP8266 project, so you can head over there for basic info.  
https://github.com/mcer12/Flora-ESP8266/wiki/

**Enclosure**  
You can find the designs in this repository inside "3D" folder or download them on printables  
https://www.printables.com/model/1228268

**Some things to note:**
1) This design is made with small footprint and ultra low-profile in mind, using only 3mm high components.
2) Using mostly modern and widely available components.
3) Can be completely sourced from LCSC (except for the VFDs and incadescent bulb) and partially assembled using JLCPCB assembly service.
4) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging via USB-C
5) Each segment driven directly, not multiplexed
6) NTP based, no battery / RTC. Connect to wifi and you're done.
7) Simple and easy to set up, mobile-friendly web interface
8) diyHue and remote control support
9) 3 levels of brightness + optional low brightness night mode
10) No buttons design. Simple configuration portal is used for settings.
11) Daylight saving (summer time) support built in and super simple to set up.

**License:**  
GPL-3.0 License  
LGPL-2.1 License (modified SPI library)  
Mozilla Public License 2.0 (iro.js, https://github.com/jaames/iro.js)
