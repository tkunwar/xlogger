#
# Makefile for xmp_crypt
CC=gcc

#The below line is for debugging
#CFLAGS=-I. -ggdb -Wall -D_FILE_OFFSET_BITS=64
CFLAGS=-Wall -g -D_FILE_OFFSET_BITS=64 -lm

LIBS=

#Uncomment the line below to compile on Mac
#LIBS=-liconv

logger: logger.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c
uninstall:
	rm -f /opt/xtrempro/csc
test_generic:
	./xmpfcheck  space-bound.mp3
test_native:
	./xmpfcheck 01_Sak_Noel_-_Loca_People.mp3
format:
	astyle *.c *.h
lint:
	splint -strict -namechecks -warnposix -preproc xmp_fcheck.c
tags:
	ctags -f xmp_ctags *.c
prof:
	gprof xmpfcheck
efence:
	CC -g -o xmpcrypt-efence xmp_fcheck.c -lefence
	./xmpcrypt-efence
mem-check:
	valgrind --leak-check=yes -v ./xmpfcheck
clean:
	rm -f *.o *.out logger
