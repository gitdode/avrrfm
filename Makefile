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
# 1 = BGR, 0 = RGB
BGR = 0
# Invert color
INVERT = 1
# Flip image
HFLIP = 1
VFLIP = 0

ifndef RFM
override RFM = 0
endif

ifndef LORA
override LORA = 1
endif

MAIN = avrrfm.c
SRC = bitmaps.c colorspace.c dejavu.c display.c font.c i2c.c mcp9808.c \
      spi.c tft.c unifont.c usart.c rfm.c sdc.c

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -DBAUD=$(BAUD)
CFLAGS += -DRFM=$(RFM) -DLORA=$(LORA)
CFLAGS += -DDISPLAY_WIDTH=$(DISPLAY_WIDTH) -DDISPLAY_HEIGHT=$(DISPLAY_HEIGHT)
CFLAGS += -DINVERT=$(INVERT) -DBGR=$(BGR) -DHFLIP=$(HFLIP) -DVFLIP=$(VFLIP)
CFLAGS += -O2 -I.
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -g -ggdb
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections -mrelax
CFLAGS += -std=gnu99
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105523
# CFLAGS += --param=min-pagesize=0

TARGET = $(strip $(basename $(MAIN)))
SRC += $(TARGET).c
SRC += librfm$(RFM).a libsdc.a

OBJ = $(SRC:.c=.o) 
OBJ = $(SRC:.S=.o)
	
$(TARGET).elf: bitmaps.h colorspace.h dejavu.h display.h font.h i2c.h \
	mcp9808.h pins.h spi.h tft.h types.h unifont.h usart.h utils.h \
	librfm$(RFM).h libsdc.h Makefile

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
