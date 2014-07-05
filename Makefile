PLATFORM = Teensy3.1

SUBDIRS = TeensyCore Loader FastLED OctoWS2811 LibMike Lib Programs Protocol XBeeAPI

TARGET = main

# Main Files
C_FILES += $(wildcard *.c)
CPP_FILES += $(wildcard *.cpp)

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: BUILDTYPE = all 
all: $(SUBDIRS) $(REPODIRS) $(TARGET).hex

$(SUBDIRS): 
	$(MAKE) -C $@ -f ../module.mk $(BUILDTYPE) 

make.defs:
	@echo PLATFORM = $(PLATFORM) > $@
	@echo INCLUDES = -I$(abspath .) $(addprefix -I,$(abspath $(SUBDIRS))) >> $@ 
	@cat project.mk >> $@ 

-include make.defs 

$(TARGET).elf: LINKALL = $(shell find . -name '*.o')
$(TARGET).elf: $(OBJS) $(LINKSCRIPT) $(LINKALL) Makefile
	$(CC) $(LDFLAGS) -o $@ $(LINKALL) $(LIBS) 

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(basename $@) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
	-$(abspath $(TOOLSPATH))/teensy_reboot

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean: BUILDTYPE = clean
clean:: $(SUBDIRS) 
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex make.defs

.PHONY: $(SUBDIRS) clean mrproper $(TARGET).elf

