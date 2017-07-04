objects := $(wildcard *.o)
target = closest_pair

CC = g++
CFLAGS = -g
CPPFLAGS = -g

$(target) : $(objects)
	$(CC) -o $(target) $(objects)
	
.PHONY: clean
clean:
	-rm $(target) $(objects)
