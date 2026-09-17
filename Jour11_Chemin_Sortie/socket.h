#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include "tcp.h" // Besoins des définitions TCB pour le lien (optionnel si on découple bien)

// États simplifiés du socket (Application view)
enum socket_state {
    SOCK_CLOSED,
    SOCK_LISTEN,
    SOCK_ESTABLISHED
};

// Taille du buffer de réception
#define RECV_BUFFER_SIZE 1024

struct socket {
    int id;
    uint16_t port;
    enum socket_state state;
    
    // Buffer circulaire simple pour la réception
    char recv_buf[RECV_BUFFER_SIZE];
    int buf_head; // Où on écrit
    int buf_tail; // Où on lit
    int data_available;
};

// --- API Utilisateur (User Space) ---
// Initialise le système de sockets
void socket_init(void);

// Crée un socket
struct socket *xsocket(void);


// Lie le socket à un port
int xbind(struct socket *s, uint16_t port);

// 1 si un socket est en LISTEN sur ce port (le SYN TCP doit le consulter)
int socket_listening(uint16_t port);

// Reçoit des données (non-bloquant pour la simulation)
// Retourne le nombre d'octets lus, ou 0 si rien
int xrecv(struct socket *s, char *buf, int max_len);

// Envoie des données
int xsend(struct socket *s, char *data, int len, uint32_t daddr, uint16_t dport);


// --- API Interne (Appelée par la Stack / Kernel Space) ---
// La stack appelle cette fonction quand des données TCP validées arrivent
void socket_push_data(uint16_t port, char *data, int len);

#endif
