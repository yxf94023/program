sources = $(wildcard *.cc)
objects := $(patsubst %.cc, %.o, $(sources))
target = c09_process_relt

CC = g++
CFLAGS = -g -O2 -Wall -Werror -fno-strict-aliasing  -Wall -Werror -fPIC
CPPFLAGS = -g -O2 -Wall -Werror -fno-strict-aliasing  -Wall -Werror -fPIC
LIB_DIR = 
LIB_SO = 

$(target) : $(objects)
	$(CC) -o $(target) $(objects) $(LIB_DIR) $(LIB_SO)
	
.PHONY: clean
clean:
	-rm $(target) $(objects)
