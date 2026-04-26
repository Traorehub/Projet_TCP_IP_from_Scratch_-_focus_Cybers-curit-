#ifndef ICMP_V4_H
#define ICMP_V4_H

#include <stdint.h>
#include "ip.h"

struct icmp_v4 {
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
} __attribute__((packed));

struct icmp_v4_echo {
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));

// Types ICMP
#define ICMP_V4_ECHO_REPLY   0
#define ICMP_V4_ECHO_REQUEST 8

void icmp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip);

#endif
