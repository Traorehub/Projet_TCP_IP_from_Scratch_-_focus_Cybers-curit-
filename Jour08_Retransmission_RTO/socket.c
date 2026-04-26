#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"
#include "tcp.h"
#include "ip.h" // Pour l'envoi éventuel

#define MAX_SOCKETS 10
static struct socket socket_table[MAX_SOCKETS];

// Initialisation du système de sockets
void socket_init(void) {
    memset(socket_table, 0, sizeof(socket_table));
    printf("SOCKET: Système initialisé.\n");
}

struct socket *xsocket(void) {
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (socket_table[i].state == SOCK_CLOSED && socket_table[i].id == 0) {
            socket_table[i].id = i + 1; // ID commence à 1
            socket_table[i].state = SOCK_CLOSED;
            socket_table[i].buf_head = 0;
            socket_table[i].buf_tail = 0;
            socket_table[i].data_available = 0;
            printf("SOCKET: Création socket ID %d\n", socket_table[i].id);
            return &socket_table[i];
        }
    }
    printf("SOCKET: Erreur - Plus de sockets disponibles.\n");
    return NULL;
}

int xbind(struct socket *s, uint16_t port) {
    if (!s) return -1;
    s->port = port;
    s->state = SOCK_LISTEN;
    printf("SOCKET: Socket %d lié au port %d (LISTEN)\n", s->id, port);
    
    // Idéalement, on devrait notifier la couche TCP pour qu'elle crée un TCB en LISTEN
    // Pour l'instant, notre tcp_handle_incoming crée des TCB à la volée, donc c'est implicite.
    return 0;
}

// Fonction interne pour écrire dans le buffer circulaire
void socket_push_data(uint16_t port, char *data, int len) {
    // Trouver le socket lié à ce port
    struct socket *s = NULL;
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (socket_table[i].port == port && socket_table[i].state != SOCK_CLOSED) {
            s = &socket_table[i];
            break;
        }
    }

    if (!s) {
        printf("SOCKET: Données reçues pour le port %d, mais aucun socket ne l'écoute.\n", port);
        return; 
        // Note: TCP devrait envoyer un RST ici normalement, mais socket_push_data est appelé APRES validation TCP.
    }

    // Protection Buffer Overflow (Simplifiée)
    if (s->data_available + len > RECV_BUFFER_SIZE) {
        printf("SOCKET SECURITY WARNING: Buffer Overflow détecté sur socket %d ! Données rejetées.\n", s->id);
        return;
    }

    // Copie des données dans le buffer circulaire
    for (int k = 0; k < len; k++) {
        s->recv_buf[s->buf_head] = data[k];
        s->buf_head = (s->buf_head + 1) % RECV_BUFFER_SIZE;
    }
    s->data_available += len;
    
    // printf("SOCKET: %d octets poussés dans le buffer du socket %d.\n", len, s->id); 
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

// Pour simplifier ce jour, xsend n'est pas encore pleinement câblé avec TCP (nécessite TCB lookup)
int xsend(struct socket *s, char *data, int len, uint32_t daddr, uint16_t dport) {
    (void)s; (void)data; (void)len; (void)daddr; (void)dport;
    printf("SOCKET: xsend non implémenté complètement (Jour suivant).\n");
    return 0;
}

