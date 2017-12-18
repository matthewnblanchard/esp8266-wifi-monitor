# esp8266-wifi-monitor

## Overview
A WiFi traffic channel monitor using the ESP8266. Fully assembled, the device sweeps through the 11 US 2.4GHz WiFi channels in configurable time steps, accumulating packets. At the end of each sweep it updates a bar graph on a 128x64 pixel 0.96" OLED display with each channels pps (packets per second) and percentage of all traffic

## Parts List
1 x [Adafruit Huzzah ESP8266 Breakout Board](https://www.adafruit.com/product/2471) - $9.95
1 x [0.96" 128x64 I2C OLED Display Module](https://www.amazon.com/gp/product/B00O2LLT30/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1) - $9.99
2 x 5.6 kiloohm resistors - < $0.01
1 x 1 kiloohm resstors - < $0.01
1 x pushbutton - ~$0.20
3 x AA Batteries + Battery pack w/ connectors - ~$5.00

Total: $25.00+

## Schematic
![Schematic](/images/schematic.png)

## Operation
Once the device boots, it will begin displaying WiFi packet volume by channel in bar graph format on the OLED display. It begins with the default sweep time of 500ms per channel. Pressing the pushbutton will double this amount, up to 8 seconds, after which it will reset it to 125 ms.


