CC = gcc
CFLAGS = -Wall -Wextra -I.

all: arp_stack

arp_stack: main.o arp.o ip.o icmp.o tcp.o tcp_utils.o socket.o tun.o
	$(CC) $^ -o $@


main.o: main.c arp.h ip.h ethernet.h tun.h
	$(CC) $(CFLAGS) -c $< -o $@

arp.o: arp.c arp.h ethernet.h
	$(CC) $(CFLAGS) -c $< -o $@

ip.o: ip.c ip.h icmp.h tcp.h ethernet.h
	$(CC) $(CFLAGS) -c $< -o $@

icmp.o: icmp.c icmp.h ip.h ethernet.h
	$(CC) $(CFLAGS) -c $< -o $@

tcp.o: tcp.c tcp.h ip.h ethernet.h
	$(CC) $(CFLAGS) -c $< -o $@

tcp_utils.o: tcp_utils.c tcp.h ip.h ethernet.h
	$(CC) $(CFLAGS) -c $< -o $@

socket.o: socket.c socket.h tcp.h ip.h
	$(CC) $(CFLAGS) -c $< -o $@

tun.o: tun.c tun.h

	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o arp_stack
