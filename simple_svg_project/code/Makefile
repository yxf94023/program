sources = $(wildcard *.cc)
objects := $(patsubst %.cc, %.o, $(sources))
target = simple_svg

CC = g++
CFLAGS = -g -O2 -Wall -Werror -fno-strict-aliasing  -Wall -Werror -fPIC -I/usr/include/libxml2
CPPFLAGS = -g -O2 -Wall -Werror -fno-strict-aliasing  -Wall -Werror -fPIC -I/usr/include/libxml2
LIB_DIR = 
LIB_SO = -lxml2

$(target) : $(objects)
	$(CC) -o $(target) $(objects) $(LIB_DIR) $(LIB_SO)
	
.PHONY: clean
clean:
	-rm $(target) $(objects)
