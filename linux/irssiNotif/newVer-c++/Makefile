#
# Source: http://mrbook.org/tutorials/make/
#
# (Cause I need to learn makefile syntax =P)
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=irssinotifs.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=irssinotifs
hide := @  # trick I found from Android build system

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(hide) rm -rf *.o $(EXECUTABLE) 
