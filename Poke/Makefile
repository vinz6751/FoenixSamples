# Makefile for FoenixSamples
# Using GCC 4.6.4 and libcmini

LIBCMINIPATH=/home/vincent/Atari/Repos/libcmini
LIBCMINI=$(LIBCMINIPATH)/build

# object file directory
OBJD = obj
# source file directory
SRCD = ../src
# header file directory
HDIR = ../h
CC=m68k-atari-mint-gcc
AS=m68k-atari-mint-as
STRIP=m68k-atari-mint-strip

CFLAGS=-I$(LIBCMINIPATH)/include -DLIBCMINI -fomit-frame-pointer -Os

.PHONY: all clean

all: poke32.ttp peek32.ttp poke16.ttp peek16.ttp poke8.ttp peek8.ttp

%.ttp: %.c
	$(CC) $(CFLAGS) -nostdlib $(LIBCMINI)/crt0.o -o $@ $< -s -L$(LIBCMINI) -lcmini -lgcc
	$(STRIP) $@

clean:
	rm -f *.o a.out
	rm -f poke32.ttp peek32.ttp poke16.ttp peek16.ttp poke8.ttp peek8.ttp


