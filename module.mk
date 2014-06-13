-include ../make.defs

# automatically create lists of the sources and objects
C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: $(OBJS)

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d

