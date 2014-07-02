CC=gcc
CFLAGS = -Wall -I.
LIBS = -lsystemd-journal -livykis

PROG = jtail jdump

all: $(PROG)

jtail: jtail.c
	$(CC) -o jtail jtail.c $(CFLAGS) $(LIBS)

jdump: jdump.c
	$(CC) -o jdump jdump.c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROG)
	rm -f *.out
	rm -f *.o
