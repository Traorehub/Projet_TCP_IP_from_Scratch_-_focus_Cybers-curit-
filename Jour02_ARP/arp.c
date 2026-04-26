#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "arp.h"

static struct arp_cache_entry arp_cache[ARP_CACHE_LEN];

void arp_init(void) {
    memset(arp_cache, 0, sizeof(arp_cache));
}

static int update_arp_cache(struct arp_hdr *hdr, struct arp_ipv4 *data) {
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        if (arp_cache[i].state == 0) continue;
        if (arp_cache[i].hwtype == ntohs(hdr->hwtype) && arp_cache[i].sip == ntohl(data->sip)) {
            memcpy(arp_cache[i].smac, data->smac, 6);
            return 1;
        }
    }
    return 0;
}

static int insert_arp_cache(struct arp_hdr *hdr, struct arp_ipv4 *data) {
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        if (arp_cache[i].state == 0) {
            arp_cache[i].state = 1;
            arp_cache[i].hwtype = ntohs(hdr->hwtype);
            arp_cache[i].sip = ntohl(data->sip);
            memcpy(arp_cache[i].smac, data->smac, 6);
            return 0;
        }
    }
    return -1;
}

void arp_handle_incoming(int fd, struct eth_hdr *eth_hdr) {
    struct arp_hdr *arp = (struct arp_hdr *)eth_hdr->payload;
    struct arp_ipv4 *data = (struct arp_ipv4 *)arp->data;

    uint16_t hwtype = ntohs(arp->hwtype);
    uint16_t protype = ntohs(arp->protype);
    uint16_t opcode = ntohs(arp->opcode);

    if (hwtype != 1 || protype != 0x0800 || arp->hwsize != 6 || arp->prosize != 4) {
        return;
    }

    if (!update_arp_cache(arp, data)) {
        insert_arp_cache(arp, data);
    }
    
    arp_cache_print();

    uint32_t my_ip = inet_addr("10.0.0.2");
    if (data->dip != my_ip) return;

    if (opcode == 1) { // Request
        arp_reply(fd, eth_hdr, arp);
    }
}

void arp_cache_print(void) {
    printf("\n--- Cache ARP Interne ---\n");
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        if (arp_cache[i].state == 0) continue;
        struct in_addr addr = { .s_addr = htonl(arp_cache[i].sip) };
        printf("[%d] %s -> %02x:%02x:%02x:%02x:%02x:%02x\n",
               i, inet_ntoa(addr),
               arp_cache[i].smac[0], arp_cache[i].smac[1], arp_cache[i].smac[2],
               arp_cache[i].smac[3], arp_cache[i].smac[4], arp_cache[i].smac[5]);
    }
    printf("------------------------\n");
}

void arp_reply(int fd, struct eth_hdr *eth_hdr, struct arp_hdr *arp) {
    struct arp_ipv4 *data = (struct arp_ipv4 *)arp->data;
    uint32_t temp_ip;

    memcpy(data->dmac, data->smac, 6);
    temp_ip = data->sip;
    data->dip = temp_ip;

    memcpy(data->smac, "\x00\x11\x22\x33\x44\x55", 6);
    data->sip = inet_addr("10.0.0.2");

    arp->hwtype  = htons(1);
    arp->protype = htons(0x0800);
    arp->hwsize  = 6;
    arp->prosize = 4;
    arp->opcode  = htons(2); // Reply

    memcpy(eth_hdr->dmac, eth_hdr->smac, 6);
    memcpy(eth_hdr->smac, "\x00\x11\x22\x33\x44\x55", 6);

    size_t total_len = sizeof(struct eth_hdr) + sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);
    if (write(fd, eth_hdr, total_len) < 0) {
        perror("arp_reply: write");
    } else {
        printf("ARP Reply envoyé\n");
    }
}
