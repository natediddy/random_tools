CC = gcc
CFLAGS = -O2 -Wall -pipe
TARGET = nfsize

SOURCE = \
	nfsize.c \
	file.c \
	dir.c \
	die.c \
	conv.c

OBJECT = \
	nfsize.o \
	file.o \
	dir.o \
	die.o \
	conv.o

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET)

nfsize.o: nfsize.h
	$(CC) $(CFLAGS) -c nfsize.c

file.o: nfsize.h
	$(CC) $(CFLAGS) -c file.c

dir.o: nfsize.h
	$(CC) $(CFLAGS) -c dir.c

die.o: nfsize.h
	$(CC) $(CFLAGS) -c die.c

conv.o: nfsize.h
	$(CC) $(CFLAGS) -c conv.c

deb:
	@echo "this is where I will make a .deb installer"

rpm:
	@echo "this is where I will make a .rpm installer"

clean:
	@rm -f $(TARGET) $(OBJECT)

.PHONY: clean
