CC := gcc
CFLAGS := -Wall -O2 -march=native

all: buildcm.o
	$(CC) $(CFLAGS) buildcm.o -o buildcm

buildcm.o:
	$(CC) $(CFLAGS) -c buildcm.c

clean:
	rm -f buildcm.o buildcm

.PHONY: clean
