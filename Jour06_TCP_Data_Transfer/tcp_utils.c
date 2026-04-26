#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tcp.h"
#include "ip.h"

// Pseudo-header pour le checksum TCP
struct tcp_pseudo {
    uint32_t saddr;
    uint32_t daddr;
    uint8_t zero;
    uint8_t proto;
    uint16_t len;
} __attribute__((packed));

uint16_t tcp_checksum(struct ip_hdr *ip, struct tcp_hdr *tcp) {
    struct tcp_pseudo pseudo;
    uint16_t tcp_len = ntohs(ip->len) - (ip->ihl * 4);

    pseudo.saddr = ip->saddr;
    pseudo.daddr = ip->daddr;
    pseudo.zero = 0;
    pseudo.proto = ip->proto;
    pseudo.len = htons(tcp_len);

    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)&pseudo;
    for (int i = 0; i < (int)(sizeof(struct tcp_pseudo) / 2); i++) sum += *ptr++;


    ptr = (uint16_t *)tcp;
    int count = tcp_len;
    while (count > 1) {
        sum += *ptr++;
        count -= 2;
    }
    if (count > 0) sum += *(uint8_t *)ptr;

    while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

void tcp_send_reset(int fd, struct eth_hdr *eth, struct ip_hdr *ip) {
    struct tcp_hdr *tcp = (struct tcp_hdr *)ip->data;
    printf("TCP: Envoi d'un RST (Reset) pour fermer la connexion invalide\n");

    uint16_t tmp_port = tcp->sport;
    tcp->sport = tcp->dport;
    tcp->dport = tmp_port;

    uint32_t received_ack = ntohl(tcp->ack);
    tcp->seq = htonl(received_ack);
    tcp->ack = 0;
    tcp->flags = TCP_RST;
    tcp->off = 5;
    tcp->win = 0;

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
}

void tcp_send_ack(int fd, struct eth_hdr *eth, struct ip_hdr *ip, struct tcb *conn) {
    struct tcp_hdr *tcp = (struct tcp_hdr *)ip->data;

    tcp->dport = htons(conn->dport);
    tcp->sport = htons(conn->sport);
    tcp->seq = htonl(conn->seq);
    tcp->ack = htonl(conn->ack);
    tcp->flags = TCP_ACK;
    tcp->off = 5;
    tcp->win = htons(65535);

    uint32_t tmp_ip = ip->saddr;
    ip->saddr = ip->daddr;
    ip->daddr = tmp_ip;
    memcpy(eth->dmac, eth->smac, 6);
    memcpy(eth->smac, "\x00\x11\x22\x33\x44\x55", 6);

    // Ajustement de la taille : plus de données, juste l'entête
    ip->len = htons(sizeof(struct ip_hdr) + 20);
    ip->csum = 0;
    ip->csum = ip_checksum(ip, ip->ihl * 4);
    tcp->csum = 0;
    tcp->csum = tcp_checksum(ip, tcp);

    write(fd, eth, sizeof(struct eth_hdr) + ntohs(ip->len));
    printf("TCP: ACK envoyé (SEQ: %u, ACK attendu: %u)\n", conn->seq, conn->ack);
}
