#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tcp.h"
#include "ip.h"

static struct tcb tcb_table[MAX_TCB];

void tcp_init(void) {
    memset(tcb_table, 0, sizeof(tcb_table));
}

void tcp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip) {
    struct tcp_hdr *tcp = (struct tcp_hdr *)ip->data;
    uint16_t dport = ntohs(tcp->dport);
    uint16_t sport = ntohs(tcp->sport);

    struct tcb *conn = NULL;
    for (int i = 0; i < MAX_TCB; i++) {
        if (tcb_table[i].state != TCP_CLOSED && tcb_table[i].sport == dport && tcb_table[i].dport == sport) {
            conn = &tcb_table[i];
            break;
        }
    }

    // A. Gestion du SYN (Handshake partie 1 & 2)
    if (tcp->flags & TCP_SYN) {
        if (conn) return;

        printf("TCP: Requête SYN reçue de %s:%d\n", inet_ntoa(*(struct in_addr *)&ip->saddr), sport);
        for (int i = 0; i < MAX_TCB; i++) {
            if (tcb_table[i].state == TCP_CLOSED) {
                conn = &tcb_table[i];
                conn->state = TCP_SYN_RECEIVED;
                printf("TCP STATE: LISTEN -> SYN_RECEIVED\n");
                
                conn->saddr = ip->daddr;
                conn->daddr = ip->saddr;
                conn->sport = dport;
                conn->dport = sport;
                conn->ack = ntohl(tcp->seq) + 1;
                conn->seq = rand() % 10000;
                break;
            }
        }

        if (!conn) {
            tcp_send_reset(fd, eth, ip);
            return;
        }

        // Envoyer SYN-ACK
        tcp->dport = htons(conn->dport);
        tcp->sport = htons(conn->sport);
        tcp->ack = htonl(conn->ack);
        tcp->seq = htonl(conn->seq);
        tcp->flags = TCP_SYN | TCP_ACK;
        tcp->off = 5;
        tcp->win = htons(65535);

        uint32_t tmp_ip = ip->saddr;
        ip->saddr = ip->daddr;
        ip->daddr = tmp_ip;
        memcpy(eth->dmac, eth->smac, 6);
        memcpy(eth->smac, "\x00\x11\x22\x33\x44\x55", 6);

        ip->csum = 0;
        ip->csum = ip_checksum(ip, ip->ihl * 4);
        tcp->csum = 0;
        tcp->csum = tcp_checksum(ip, tcp);
        write(fd, eth, sizeof(struct eth_hdr) + ntohs(ip->len));
        
        printf("TCP: SYN-ACK envoyé\n");
        return;
    }

    // B. Finalisation du Handshake (Handshake partie 3)
    if (conn && conn->state == TCP_SYN_RECEIVED && (tcp->flags & TCP_ACK)) {
        uint32_t seg_ack = ntohl(tcp->ack);
        if (seg_ack == conn->seq + 1) {
            conn->state = TCP_ESTABLISHED;
            conn->seq = seg_ack;
            printf("TCP STATE: SYN_RECEIVED -> ESTABLISHED. Connexion prête !\n");
        }
        return;
    }

    if (!conn) tcp_send_reset(fd, eth, ip);
}
