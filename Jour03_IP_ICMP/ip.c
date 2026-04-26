#include <stdio.h>
#include <arpa/inet.h>
#include "ip.h"
#include "icmp.h"

uint16_t ip_checksum(void *addr, int count) {
    register uint32_t sum = 0;
    uint16_t *ptr = addr;

    while (count > 1) {
        sum += *ptr++;
        count -= 2;
    }

    if (count > 0) {
        sum += *(uint8_t *)ptr;
    }

    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}

void ip_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip) {
    if (ip->version != 4) return;

    if (ip_checksum(ip, ip->ihl * 4) != 0) {
        printf("IP: Checksum invalide\n");
        return;
    }

    if (ip->proto == 1) { // ICMP
        icmp_handle_incoming(fd, eth, ip);
    } else {
        printf("IP: Protocole %d non supporté\n", ip->proto);
    }
}
