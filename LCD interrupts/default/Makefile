###############################################################################
# Makefile for the project LCD
###############################################################################

## General Flags
PROJECT = LCD
MCU = atmega32
TARGET = LCD.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=LCD.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = Main.o LCD.o Keyboard.o ADC.o DAC.o Interrupts.o clock.o LCD_fixed_text.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LCD.hex LCD.eep LCD.lss size

## Compile
Main.o: ../Main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD.o: ../LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Keyboard.o: ../Keyboard.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ADC.o: ../ADC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DAC.o: ../DAC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Interrupts.o: ../Interrupts.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD_fixed_text.o: ../LCD_fixed_text.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) LCD.elf dep/* LCD.hex LCD.eep LCD.lss LCD.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

