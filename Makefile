
ARDUINO_HOME = /opt/arduino
BUILDER = $(ARDUINO_HOME)/arduino-builder
TOOLS = $(ARDUINO_HOME)/tools-builder 
HARDWARE = $(ARDUINO_HOME)/hardware
FQBN = teensy:avr:teensy31
SKETCH = Tubes.ino 

all:
	-mkdir _build
	$(BUILDER) -compile -hardware $(HARDWARE) -fqbn $(FQBN) -tools $(TOOLS) -libraries ./Lib -build-cache ./_build -build-path ./_build $(SKETCH)

clean:
	-rm -rf _build 

.PHONY: clean
