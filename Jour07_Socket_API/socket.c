#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"

#define MAX_SOCKETS 10
static struct socket socket_table[MAX_SOCKETS];

void socket_init(void) {
    memset(socket_table, 0, sizeof(socket_table));
    printf("SOCKET: Système initialisé.\n");
}

struct socket *xsocket(void) {
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (socket_table[i].state == SOCK_CLOSED && socket_table[i].id == 0) {
            socket_table[i].id = i + 1;
            socket_table[i].state = SOCK_CLOSED;
            socket_table[i].buf_head = 0;
            socket_table[i].buf_tail = 0;
            socket_table[i].data_available = 0;
            return &socket_table[i];
        }
    }
    return NULL;
}

int xbind(struct socket *s, uint16_t port) {
    if (!s) return -1;
    s->port = port;
    s->state = SOCK_LISTEN;
    printf("SOCKET: Socket %d lié au port %d (LISTEN)\n", s->id, port);
    return 0;
}

void socket_push_data(uint16_t port, char *data, int len) {
    struct socket *s = NULL;
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (socket_table[i].port == port && socket_table[i].state != SOCK_CLOSED) {
            s = &socket_table[i];
            break;
        }
    }
    if (!s) return;

    if (s->data_available + len > RECV_BUFFER_SIZE) {
        printf("SOCKET WARNING: Buffer plein sur socket %d !\n", s->id);
        return;
    }

    for (int k = 0; k < len; k++) {
        s->recv_buf[s->buf_head] = data[k];
        s->buf_head = (s->buf_head + 1) % RECV_BUFFER_SIZE;
    }
    s->data_available += len;
}

int xrecv(struct socket *s, char *buf, int max_len) {
    if (!s || s->data_available == 0) return 0;
    int bytes_to_read = (s->data_available < max_len) ? s->data_available : max_len;
    for (int k = 0; k < bytes_to_read; k++) {
        buf[k] = s->recv_buf[s->buf_tail];
        s->buf_tail = (s->buf_tail + 1) % RECV_BUFFER_SIZE;
    }
    s->data_available -= bytes_to_read;
    return bytes_to_read;
}
