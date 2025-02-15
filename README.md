# AVRRFM

Experimental project to drive an RFM69 radio module with plain avr-libc
and an Atmega328p MCU.  

This is work in progress. Simple Tx-Rx is working so far.  

To do something really extraordinary, the temperature reading of an MCP9808
sensor is periodically transmitted to the receiver.  
To save battery power, the controller, radio module and temperature sensor 
are put to power down/sleep mode in between transmissions. The idle current 
is ~57 uA, which is still quite a lot (< 10 uA should be possible), but already 
better than 8 mA :-)

![IMG_20250212_190518](https://github.com/user-attachments/assets/dd87b7de-c97d-4ecb-ab24-f5a34b849914)

The receiver currently just converts the raw temperature reading to °C and 
displays it "screensaver style" on a nice IPS TFT display.

![IMG_20250216_003419](https://github.com/user-attachments/assets/0116ceec-fb97-4eec-a0f7-bb66f7e432c5)

