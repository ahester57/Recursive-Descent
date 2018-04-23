CC=gcc
CFLAGS=-Wall -g -I ./headers/
#LDLIBS= -pthread
DEPS = $(wildcard ./headers/*.h)
SOURCES= $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, ${SOURCES})

.PHONY: all clean

default: all

all: statSem

%.o: %.c $(DEPS)

statSem: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJECTS)

destroy:
	$(RM) $(OBJECTS) statSem
