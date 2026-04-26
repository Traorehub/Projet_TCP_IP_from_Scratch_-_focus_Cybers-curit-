CC=gcc
CFLAGS=-Wall -Wextra -I.

OBJ = main.o tun.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ether_stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o ether_stack
