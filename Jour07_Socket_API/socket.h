#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

enum socket_state {
    SOCK_CLOSED,
    SOCK_LISTEN,
    SOCK_ESTABLISHED
};

#define RECV_BUFFER_SIZE 1024

struct socket {
    int id;
    uint16_t port;
    enum socket_state state;
    char recv_buf[RECV_BUFFER_SIZE];
    int buf_head;
    int buf_tail;
    int data_available;
};

void socket_init(void);
struct socket *xsocket(void);
int xbind(struct socket *s, uint16_t port);
int xrecv(struct socket *s, char *buf, int max_len);
void socket_push_data(uint16_t port, char *data, int len);

#endif
