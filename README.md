# AVRRFM

## About

Experimental project to drive an RFM69HCW radio module with plain avr-libc
and an Atmega328p MCU.  

This is work in progress. Simple Tx-Rx with response is working so far.  

I'm impressed how well these radio modules work; the range achieved with 
simple wire antennas as well as the reliable packet transmission.

To do something really extraordinary, the temperature reading of an MCP9808
sensor is periodically transmitted to the receiver.  
To save battery power, the controller, radio module and temperature sensor 
are put to power down/sleep mode in between transmissions. The idle current 
is ~57 uA, which is still quite a lot (< 10 uA should be possible), but already 
better than 8 mA 🙂

![IMG_20250212_190518](https://github.com/user-attachments/assets/dd87b7de-c97d-4ecb-ab24-f5a34b849914)

The receiver currently converts the raw temperature reading to °C and displays 
it with the RSSI value, CRC result and transmitter output power on a nice IPS 
TFT display. It responds to the transmitter as kind of ack with the RSSI, which 
is used for some very basic power management in the transmitter, reducing the 
supply current from 45 mA down to ~15 mA. The transmitter waits for this 
response with a timeout so it won't be blocked and consumes a lot of power just 
because there is no response coming back.

## Range

Setting `RegPaLevel` to `0x5f`, which gives +13 dBm with `PA1`, indoor range is 
very good and in an actual "field" test, packet reception was still reliable 
with an RSSI of about -90 dBm at about 2.2 km distance - with simple wire 
antennas. What would be the range with +20 dBm and decent antennas?  

![IMG_20250306_180139c](https://github.com/user-attachments/assets/27c66e7a-ec16-4e98-9f94-7713fe54c7d0)

![FieldTest3](https://github.com/user-attachments/assets/f2289f8e-1f81-4b85-9146-07c2ce1bb563)

## Susceptibility to Temperature Changes

With the default frequency deviation of 5 kHz and receiver bandwidth of 
10.4 kHz, packet transmission is very unreliable and fails completely for me; 
when the temperature of the transmitter is below 10°C and above 40°C, while 
the receiver temperature is at 20°C. The receiver does not seem to be prone to 
temperature changes.  
Increasing frequency deviation to 10 kHz and receiver bandwidth to 20.8 kHz, 
temperature susceptibility is eliminated; when testing with transmitter 
temperature from -20°C to 50°C, packet transmission is perfectly reliable.

Frequency Deviation = 10 kHz (transmitter)  
`RegFdevMsb` = `0x00`  
`RegFdevLsb` = `0xa4`  

Receiver Bandwidth = 20.8 kHz  
`RegRxBw` = `0x54`  

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

The first 15 `0b10101010` bytes are the preamble, then there are 4 sync word 
bytes. After the 4 payload bytes, there are 2 CRC bytes as described in the 
datasheet of the RFM69HCW:

![PackageFormat](https://github.com/user-attachments/assets/11687645-552c-46e5-a0bf-ef490b1bca48)
