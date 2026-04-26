CC=gcc
CFLAGS=-Wall -Wextra -I.

OBJ = main.o tun.o arp.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

arp_stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o arp_stack
