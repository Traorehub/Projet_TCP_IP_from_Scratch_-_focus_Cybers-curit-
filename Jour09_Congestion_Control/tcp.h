#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <time.h>
#include "ip.h"

struct tcp_hdr {
    uint16_t sport; uint16_t dport;
    uint32_t seq; uint32_t ack;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t res1 : 4; uint8_t off : 4;
#else
    uint8_t off : 4; uint8_t res1 : 4;
#endif
    uint8_t flags; uint16_t win;
    uint16_t csum; uint16_t urp;
    uint8_t data[];
} __attribute__((packed));

#define TCP_SYN 0x02
#define TCP_ACK 0x10
#define TCP_RST 0x04

enum tcp_states { TCP_CLOSED, TCP_LISTEN, TCP_SYN_RECEIVED, TCP_ESTABLISHED };

#define MAX_TCB 10
#define TCP_RTO_INITIAL 3
#define MAX_RETRY 5

struct tcb {
    uint32_t saddr; uint32_t daddr;
    uint16_t sport; uint16_t dport;
    uint32_t seq; uint32_t ack;
    int state;

    // Retransmission (Jour 08)
    time_t last_send_time;
    int retransmit_count;
    int curr_rto;
};

void tcp_init(void);
void tcp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip);
void tcp_check_timeouts(int fd);
uint16_t tcp_checksum(struct ip_hdr *ip, struct tcp_hdr *tcp);
void tcp_send_reset(int fd, struct eth_hdr *eth, struct ip_hdr *ip);
void tcp_send_ack(int fd, struct eth_hdr *eth, struct ip_hdr *ip, struct tcb *conn);

#endif
