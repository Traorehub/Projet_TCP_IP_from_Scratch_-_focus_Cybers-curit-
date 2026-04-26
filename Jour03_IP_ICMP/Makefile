CC=gcc
CFLAGS=-Wall -Wextra -I.

OBJ = main.o tun.o arp.o ip.o icmp.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

icmp_stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o icmp_stack
