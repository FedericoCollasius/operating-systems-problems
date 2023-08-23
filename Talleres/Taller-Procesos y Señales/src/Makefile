CC=gcc

ARCH := $(shell uname -m)

BINS=hai ejercicio1

OBJS=$(BINS:=.o)

all: $(BINS)

# Dependencias
hai.o: hai.c
ejercicio1.o: ejercicio1.c

clean:
	rm -rf $(OBJS) $(BINS)
