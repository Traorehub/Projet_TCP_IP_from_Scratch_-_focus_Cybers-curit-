#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tun.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"

int main() {
    char dev[16] = "tap0";
    int fd = tun_alloc(dev);

    if (fd < 0) {
        fprintf(stderr, "Erreur lors de la création de l'interface\n");
        return 1;
    }

    printf("SOCKET: Système initialisé.\n");
    arp_init();
    
    printf("Interface %s créée. Configurez: sudo ip addr add 10.0.0.1/24 dev %s && sudo ip link set %s up\n", dev, dev, dev);

    unsigned char buf[1600];
    while (1) {
        int len = read(fd, buf, sizeof(buf));
        if (len < 0) {
            perror("Erreur de lecture");
            break;
        }

        struct eth_hdr *eth = (struct eth_hdr *)buf;
        uint16_t ethertype = ntohs(eth->ethertype);

        if (ethertype == 0x0806) {
            arp_handle_incoming(fd, eth);
        } else if (ethertype == 0x0800) {
            struct ip_hdr *ip = (struct ip_hdr *)eth->payload;
            ip_handle_incoming(fd, eth, ip);
        }
    }

    close(fd);
    return 0;
}
