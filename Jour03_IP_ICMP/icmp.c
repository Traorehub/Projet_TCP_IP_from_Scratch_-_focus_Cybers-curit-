#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "icmp.h"
#include "ip.h"

void icmp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip) {
    struct icmp_v4 *icmp = (struct icmp_v4 *)ip->data;
    int icmp_len = ntohs(ip->len) - (ip->ihl * 4);

    if (icmp->type == ICMP_V4_ECHO_REQUEST) {
        printf("ICMP: Echo Request reçu\n");

        icmp->type = ICMP_V4_ECHO_REPLY;
        icmp->csum = 0;
        icmp->csum = ip_checksum(icmp, icmp_len);

        uint32_t tmp_addr = ip->saddr;
        ip->saddr = ip->daddr;
        ip->daddr = tmp_addr;

        memcpy(eth->dmac, eth->smac, 6);
        memcpy(eth->smac, "\x00\x11\x22\x33\x44\x55", 6);

        ip->csum = 0;
        ip->csum = ip_checksum(ip, ip->ihl * 4);

        size_t total_len = sizeof(struct eth_hdr) + ntohs(ip->len);
        if (write(fd, eth, total_len) < 0) {
            perror("icmp_reply: write");
        } else {
            printf("ICMP: Echo Reply envoyé\n");
        }
    }
}
