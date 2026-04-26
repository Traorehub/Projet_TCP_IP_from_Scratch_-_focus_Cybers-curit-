CC=gcc
CFLAGS=-Wall -Wextra -I.

OBJ = main.o tun.o arp.o ip.o icmp.o tcp.o tcp_utils.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

tcp_stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o tcp_stack
