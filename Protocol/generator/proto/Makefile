all: nanopb_pb2.py plugin_pb2.py

%_pb2.py: %.proto
	protoc --python_out=. $<

clean: 
	-rm nanopb_pb2.py* plugin_pb2.py* __init__.pyc

.PHONY: clean

