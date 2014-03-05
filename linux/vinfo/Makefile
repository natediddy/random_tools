#
# Makefile for vinfo
# Nathan Forbes
#

CC = gcc
CFLAGS = -g -O2 -Wall -march=native -mtune=native
TARGET = vinfo

prefix = /usr/local

all: $(TARGET)

$(TARGET): vinfo.o
	$(CC) $(CFLAGS) vinfo.o -o $(TARGET)

vinfo.o: vinfo.c
	$(CC) $(CFLAGS) -c vinfo.c

clean:
	rm -f vinfo.o

clobber:
	rm -f vinfo.o $(TARGET)

install: $(TARGET)
	install $(TARGET) $(prefix)/bin/$(TARGET)

uninstall:
	rm -f $(prefix)/bin/$(TARGET)

.PHONY: clean

