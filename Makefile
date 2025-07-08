# Makefile to build and flash avrrfm
#
# Simplified version from: https://github.com/hexagon5un/AVR-Programming

MCU = atmega328p
F_CPU = 8000000
BAUD = 9600
PROGRAMMER_TYPE = avrispmkII
# PROGRAMMER_TYPE = atmelice_isp
PROGRAMMER_ARGS = -B 125kHz

# Supported RFM radio module variants: 69 and 95
RFM = 95
# LoRa available only with RFM95
LORA = 1

# Display dimensions
DISPLAY_WIDTH = 320
DISPLAY_HEIGHT = 240
# true = BGR, false = RGB
BGR = true
# Invert color
INVERT = true
# Flip image
HFLIP = true
VFLIP = false

ifndef RFM
override RFM = 0
endif

ifndef LORA
override LORA = 1
endif

MAIN = avrrfm.c
SRC = bitmaps.c dejavu.c i2c.c mcp9808.c spi.c usart.c rfm.c tft.c sdc.c

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -DBAUD=$(BAUD)
CFLAGS += -DRFM=$(RFM) -DLORA=$(LORA)
CFLAGS += -DDISPLAY_WIDTH=$(DISPLAY_WIDTH) -DDISPLAY_HEIGHT=$(DISPLAY_HEIGHT)
CFLAGS += -DINVERT=$(INVERT) -DBGR=$(BGR) -DHFLIP=$(HFLIP) -DVFLIP=$(VFLIP)
CFLAGS += -O2 -I. -I../
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -g -ggdb
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections -mrelax
CFLAGS += -std=gnu99
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105523
# CFLAGS += --param=min-pagesize=0

TARGET = $(strip $(basename $(MAIN)))
SRC += $(TARGET).c
SRC += librfm$(RFM).a libtft.a libsdc.a

OBJ = $(SRC:.c=.o) 
OBJ = $(SRC:.S=.o)
	
$(TARGET).elf: dejavu.h i2c.h mcp9808.h pins.h spi.h types.h usart.h \
	utils.h Makefile

all: $(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) --output $@ 

disasm: $(TARGET).lst

eeprom: $(TARGET).eeprom

%.lst: %.elf
	$(OBJDUMP) -S $< > $@
 
size:  $(TARGET).elf
	$(AVRSIZE) --mcu=$(MCU) $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~ \
	$(TARGET).eeprom

flash: $(TARGET).hex 
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$<
