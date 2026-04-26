#ifndef IP_H
#define IP_H

#include <stdint.h>
#include <endian.h>
#include "ethernet.h"

struct ip_hdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t ihl : 4;
    uint8_t version : 4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version : 4;
    uint8_t ihl : 4;
#endif
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_offset;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
    uint8_t data[];
} __attribute__((packed));

void ip_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip);
uint16_t ip_checksum(void *addr, int count);

#endif
