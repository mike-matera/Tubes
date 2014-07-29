
ifeq ($(strip $(PLATFORM)), Teensy3.1)
	LINKSCRIPT = mk20dx256.ld
	CHIP = -D__MK20DX256__
	CLOCK = -DF_CPU=72000000
endif

ifeq ($(strip $(PLATFORM)), Teensy3.0)
	LINKSCRIPT = mk20dx128.ld
	CHIP = -D__MK20DX128__
	CLOCK = -DF_CPU=48000000
endif

# configurable options
OPTIONS = $(CLOCK) -DUSB_SERIAL -DLAYOUT_US_ENGLISH

# options needed by many Arduino libraries to configure for Teensy 3.0
OPTIONS += $(CHIP) -DARDUINO=105 -DTEENSYDUINO=118

ifeq ($(strip $(BOARD)), Candle.B)
	OPTIONS += -DCANDLE_REV_B
endif

# Path to your arduino installation
ARDUINOPATH ?= /home/maximus/Arduino/arduino-1.0.5
# ARDUINOPATH ?= /Applications/Arduino.app/Contents/Resources/Java

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(ARDUINOPATH)/hardware/tools   # on Linux

# path location for the arm-none-eabi compiler
COMPILERPATH = $(ARDUINOPATH)/hardware/tools/arm-none-eabi/bin

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -fdata-sections -ffunction-sections -Wall -g -Os -mcpu=cortex-m4 -mthumb -nostdlib -MMD $(OPTIONS) $(INCLUDES)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -T$(LINKSCRIPT) 

# additional libraries to link
LIBS = -lm

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

PROTOC = protoc
# PROTOC = /usr/local/bin/protoc
PYTHON = python 

#COMPILER_PREFIX = arm-linux-gnueabi
#CC = $(COMPILER_PREFIX)-gcc
#CXX = $(COMPILER_PREFIX)-g++
#OBJCOPY = $(COMPILER_PREFIX)-objcopy
#SIZE = $(COMPILER_PREFIX)-size
