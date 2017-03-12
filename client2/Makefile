CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lmraa -pthread -lm
SOURCES=main.c client.c thermistor.c LSM9DS0.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(OBJECTS)

clobber: clean
	rm -rf *~
