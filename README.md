# AVRRFM

## About

Experimental project to drive an RFM69HCW radio module with plain avr-libc
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
displays it with the RSSI value on a nice IPS TFT display.

## Range

Setting `RegPaLevel` to `0x5f`, which gives +13 dBm with `PA1`, indoor range is very good 
and in an actual "field" test, packet reception was still reliable with an RSSI of about -100 dBm 
at about 1.6 km distance.

![IMG_20250303_1733461c](https://github.com/user-attachments/assets/2aaa5fcb-6c89-4a07-a375-faa7fd152bbd)

![FieldTest1c](https://github.com/user-attachments/assets/c1f98778-df27-42a5-9ef3-afd779d8b309)

## Fun Stuff

Looking at the payload in the transmitted signal in URH (with an RTL-SDR Blog V4):

![urh](https://github.com/user-attachments/assets/4c0b159a-5ae9-444c-99f1-6edc385ba4b1)

The four selected payload bytes are:  

- `0b00000011` Payload length (address byte + 2 byte temperature value)
- `0b01000010` Address (`0x42`)
- `0b11000001` Upper byte of raw temperature value from MCP9808
- `0b01010011` Lower byte of raw temperature value from MCP9808

Calculating the temperature (assuming >= 0°C):  

    jshell> (0b11000001 & 0x1f) * 16 + 0b01010011 / 16f
    $1 ==> 21.1875

So, 21.2 °C 🙂

The first 15 `0b10101010` bytes are the preamble, then there are 4 sync word bytes.
After the 4 payload bytes, there are 2 CRC bytes as described in the datasheet of the RFM69HCW:

![PackageFormat](https://github.com/user-attachments/assets/959babe5-9f1f-4f3d-9568-a4edbbca4bb2)

