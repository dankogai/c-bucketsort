#
#  $Id: Makefile,v 0.1 2012/01/12 21:52:11 dankogai Exp dankogai $
#
#  Licensed under the MIT license.
#  http://www.opensource.org/licenses/mit-license.php
#

CFLAGS = -W -Wall -O2

all: bucketsort

.c.o:
	$(CC) $(CFLAGS) -c $<

bucketsort : bucketsort.o main.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm bucketsort *.o *.tmp

FRC:
