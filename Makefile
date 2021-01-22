
ARDUINO_HOME = /opt/arduino
BUILDER = $(ARDUINO_HOME)/arduino-builder
TOOLS = $(ARDUINO_HOME)/tools-builder 
HARDWARE = $(ARDUINO_HOME)/hardware
FQBN = teensy:avr:teensy31
SKETCH = Tubes.ino 

all:
	$(BUILDER) -compile -hardware $(HARDWARE) -fqbn $(FQBN) -tools $(TOOLS) -libraries ./Lib $(SKETCH)
