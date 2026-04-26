#include <stdio.h>
#include <arpa/inet.h>
#include "ip.h"
#include "icmp.h"
#include "tcp.h"

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
    // struct ip_hdr *ip pointer passed as argument


    if (ip->version != 4) {
        return;
    }

    // Le checksum se calcule sur la longueur de l'entête (ihl * 4 octets)
    if (ip_checksum(ip, ip->ihl * 4) != 0) {
        printf("IP: Checksum invalide\n");
        return;
    }

    char saddr[INET_ADDRSTRLEN];
    char daddr[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ip->saddr, saddr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip->daddr, daddr, INET_ADDRSTRLEN);

    printf("\n--- Paquet IP reçu ---\n");
    printf("Source: %s\n", saddr);
    printf("Destination: %s\n", daddr);
    printf("Protocol: %d\n", ip->proto);
    printf("Total Length: %d\n", ntohs(ip->len));
    
    if (ip->proto == 1) { // ICMP
        icmp_handle_incoming(fd, eth, ip);
    } else if (ip->proto == 6) { // TCP
        tcp_handle_incoming(fd, eth, ip);
    }
}
