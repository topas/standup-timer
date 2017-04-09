# Arduino Standup Timer 

Simple Arduino countdown timer with RGB led strip visualisation and one button for start (short press) and turn off (long press). 

### Parts and wiring
 - Pro Micro - small Arduino compatible board https://www.sparkfun.com/products/12640
 - APA102 led string (30 leds) - a cheap one from Aliexpress (to SPI)
 - Big hardware button (to PIN 8)
 - !!! To avoid overloading of voltage regulator bypass it via J1 contacts 

### Libraries 
 - SimpleTimer https://github.com/marcelloromani/Arduino-SimpleTimer
 - FastLED https://github.com/FastLED/FastLED

