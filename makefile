TARGET   = nshifter
MCU      = msp430f2132
SOURCES  = nshifter.c spi.c uart.c switch.c rs485.c adc.c amp.c att.c delay.c phase.c

CFLAGS   = -mmcu=$(MCU) -g -Os -Wall -Wunused $(INCLUDES)
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

CC       = msp430-gcc
LD       = msp430-ld
AR       = msp430-ar
AS       = msp430-gcc
GASP     = msp430-gasp
NM       = msp430-nm
OBJCOPY  = msp430-objcopy
RANLIB   = msp430-ranlib
STRIP    = msp430-strip
SIZE     = msp430-size
READELF  = msp430-readelf
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -f
MV       = mv

DEPEND = $(SOURCES:.c=.d)

OBJECTS = $(SOURCES:.c=.o)
all: $(TARGET).elf $(TARGET).hex 
$(TARGET).elf: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $(TARGET).elf
	echo
%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.c
	echo "Compiling $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# rule for making assembler source listing, to see the code
%.lst: %.c
	$(CC) -c $(ASFLAGS) -Wa,-anlhd $< > $@
# include the dependencies unless we're going to clean, then forget about them.
ifneq ($(MAKECMDGOALS), clean)

-include $(DEPEND)
endif
# dependencies file
# includes also considered, since some of these are our own
# (otherwise use -MM instead of -M)
%.d: %.c
	echo "Generating dependencies $@ from $<"
	$(CC) -M ${CFLAGS} $< >$@
.SILENT:
.PHONY:    clean
clean:
	-$(RM) *.o
	-$(RM) *.d
	-$(RM) *.map
	-$(RM) *.elf
	-$(RM) *.hex

install:
	sudo mspdebug rf2500

	
