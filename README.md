Simple NTP clock with LED Matrix display, controled with MD_MAX72xx library.

LED matrix used : https://www.laskakit.cz/32x8-led-matice-s-max7219-3mm/

Connection to ESP8266: CLK D5; DATA D7; CS D8


You can change NTP server in code on line 33 by editing : NTPClient ntp(ntpUDP, "0.cz.pool.ntp.org", 0, 600000);

WiFi can be configured by connecting to ESP WiFi manager on IP 192.168.4.1

Standard and Daylight saving time can be set on local web server that ESP will be hosting after connection to router and will obtain IP by DHCP of your router.
Settings of time offset is stored in EEPROM and will be loaded back after power loss / reset


Used libraries :

ESP8266WiFi

MD_MAX72xx

MD_Parola

SPI

EEPROM

NTPClient

WiFiUdp

ESPAsyncWiFiManager

ESPAsyncWebServer

ESP8266mDNS
