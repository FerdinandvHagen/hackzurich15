#hackzurich15
#WiLoc

---------------

Passive WiFi Localication of WiFi enabled devices on basis of the ESP8266 WiFi SoC.

Most of the projects we know about for indoor localization are based on known positions for Beacons and an app on a smartphone provides the user with position information. The biggest problem is for this type of systems that they require an app installed on a smartphone. We wanted to use the already available information from their WiFi communication to make a passive tracking system. Using extremely cheap WiFi SoCs (~5$ per module), we sniff WiFi traffic and collect/analyze the data.

Current status:
- Sniffing devices work realtively stable and analyze the frames
- The Server works as a proxy for use with Socket.IO or MatLab
- The data provided shows that there is a quite good correlation between distance and signal strength (which we can get over the RSSI strength)

Goals:
- GeoLocalization of WiFi-enabled devices
- Device Recognition
- Automatic Calibration based on stationary devices (Router, AP, ...)

Possible Applications: HomeAutomation, IoT, Behaviorel Studies, etc.
