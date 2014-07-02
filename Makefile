CC=gcc
CFLAGS = -Wall -I.
LIBS = -lsystemd-journal
PROG = jtail jdump

all: $(PROG)

# gcc journal-tail.c  -Wall -lsystemd-journal -o tail
jtail: jtail.c
	$(CC) -o jtail jtail.c $(CFLAGS) $(LIBS)

jdump: jdump.c
	$(CC) -o jdump jdump.c $(CFLAGS) $(LIBS)

clean:
	rm -f $(PROG)
	rm -f *.out
	rm -f *.o
