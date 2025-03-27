# avrrfm

[![make](https://github.com/gitdode/avrrfm/actions/workflows/build.yml/badge.svg)](https://github.com/gitdode/avrrfm/actions/workflows/build.yml)

## About

Experimental project around RFM radio modules using an ATmega328P MCU 
and [librfm](https://github.com/gitdode/librfm/tree/main).

To do something really extraordinary, the temperature reading of an MCP9808
sensor is periodically transmitted to the receiver.  
To save battery power, the controller, radio module and temperature sensor 
are put to power down/sleep mode in between transmissions. The idle current 
is ~57 uA, which is still quite a lot (< 10 uA should be possible), but already 
better than 8 mA 🙂  
There is basic [SD card support](https://github.com/gitdode/libsdc/tree/main) 
to test sending larger amounts of data, but it might be useful for something 
else like a data logger.  

![IMG_20250322_012126](https://github.com/user-attachments/assets/e641ddd8-ca52-4149-8d73-3dcee39b7768)

The receiver currently converts the raw temperature reading to °C and displays 
it with the RSSI value, CRC result and transmitter output power on a nice IPS 
TFT display. It responds to the transmitter as kind of ack with the RSSI, which 
is used for some very basic power management in the transmitter, reducing the 
supply current from 50 mA down to 22 mA. The transmitter waits for this 
response with a timeout so it won't be blocked and consumes a lot of power just 
because there is no response coming back.

![IMG_20250315_175920c](https://github.com/user-attachments/assets/03405774-57c2-42f1-a39a-8fa4cc2801ac)

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
