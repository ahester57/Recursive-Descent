CC=gcc
CFLAGS=-Wall -g -I ./lex/
#LDLIBS= -pthread
DEPS = $(wildcard *.h)
SOURCES= $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, ${SOURCES})

.PHONY: all clean

default: all

all: frontEnd

%.o: %.c $(DEPS)

frontEnd: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJECTS)

destroy:
	$(RM) $(OBJECTS) scanner
