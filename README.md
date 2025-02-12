# AVRRFM

Experimental project to drive an RFM69 radio module with plain avr-libc
and an Atmega328p MCU.  

This is work in progress. Simple Tx-Rx is working so far.  

To do something really extraordinary, the temperature reading of an MCP9803
sensor is periodically transmitted to the receiver.  
To save battery power, the controller, radio module and temperature sensor 
are put to power down/sleep mode in between transmissions. The idle current 
is ~57 uA, which is still quite a lot (> 10 uA should be possible), but already 
better than 8 mA :-)

![IMG_20250204_202605](https://github.com/user-attachments/assets/9ab786fc-655d-48e0-afa9-cc52b606b4b8)
