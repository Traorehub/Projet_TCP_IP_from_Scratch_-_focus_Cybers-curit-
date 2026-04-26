#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include <linux/if_ether.h>
#include "ethernet.h"

#define ETH_P_ARP 0x0806
#define ARP_CACHE_LEN 16

struct arp_hdr {
    uint16_t hwtype;
    uint16_t protype;
    uint8_t  hwsize;
    uint8_t  prosize;
    uint16_t opcode;
    uint8_t  data[];
} __attribute__((packed));

struct arp_ipv4 {
    uint8_t  smac[6];
    uint32_t sip;
    uint8_t  dmac[6];
    uint32_t dip;
} __attribute__((packed));

struct arp_cache_entry {
    int state;               // 0 = free, 1 = resolved
    uint16_t hwtype;
    uint32_t sip;
    uint8_t  smac[6];
};

// Fonctions ARP
void arp_init(void);
void arp_handle_incoming(int fd, struct eth_hdr *eth_hdr);
void arp_reply(int fd, struct eth_hdr *eth_hdr, struct arp_hdr *arphdr);
void arp_cache_print(void);

#endif
