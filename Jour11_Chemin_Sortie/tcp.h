#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <time.h>
#include "ip.h"

struct tcp_hdr {
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;
    uint32_t ack;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t res1 : 4;
    uint8_t off : 4;
#else
    uint8_t off : 4;
    uint8_t res1 : 4;
#endif
    uint8_t flags;
    uint16_t win;
    uint16_t csum;
    uint16_t urp;
    uint8_t data[];
} __attribute__((packed));

// Flags TCP
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20

// États TCP (simplifiés)
enum tcp_states {
    TCP_CLOSED,
    TCP_LISTEN,
    TCP_SYN_SENT,
    TCP_SYN_RECEIVED,
    TCP_ESTABLISHED,
    TCP_FIN_WAIT_1,
    TCP_FIN_WAIT_2,
    TCP_CLOSE_WAIT,
    TCP_CLOSING,
    TCP_LAST_ACK,
    TCP_TIME_WAIT
};

#define MAX_TCB 10
#define TCP_RTO_INITIAL 3
#define MAX_RETRY 5

// Congestion Control (Jour 9)
#define INIT_CWND 1
#define INIT_SSTHRESH 65535

struct tcb {
    uint32_t saddr;
    uint32_t daddr;
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;      // Notre numéro de séquence (SND.NXT)
    uint32_t ack;      // Ce qu'on attend de l'autre (RCV.NXT)
    
    // Gestion du temps et retransmission (Jour 9)
    time_t last_send_time;
    int retransmit_count;
    int curr_rto;      // RTO actuel (ajusté par Backoff)

    // Fenêtrage et Congestion
    uint32_t cwnd;     // Congestion Window (en segments)
    uint32_t ssthresh; // Slow Start Threshold
    
    int state;
};


// --- Fonctions Utilitaires (tcp_utils.c) ---
uint16_t tcp_checksum(struct ip_hdr *ip, struct tcp_hdr *tcp);
void tcp_send_reset(int fd, struct eth_hdr *eth, struct ip_hdr *ip);
void tcp_send_ack(int fd, struct eth_hdr *eth, struct ip_hdr *ip, struct tcb *conn);

// --- Cœur du Protocole (tcp.c) ---
void tcp_init(void);
void tcp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip);
void tcp_check_timeouts(int fd);

#endif
