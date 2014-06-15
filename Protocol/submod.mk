
PB_FILES := $(wildcard *.proto)
OBJS += $(PB_FILES:.proto=.pb.o)

all: nanopb Command.pb.c

nanopb:
	$(MAKE) -C generator/proto 

%.pb.c: %.pb
	$(PYTHON) generator/nanopb_generator.py $<
	
%.pb: %.proto
	$(PROTOC) -o $@ $< 

clean::
	$(MAKE) -C generator/proto clean
	-rm Command.pb*
	
.PHONY: nanopb

