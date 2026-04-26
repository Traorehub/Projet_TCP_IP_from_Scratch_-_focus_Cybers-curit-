#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tun.h"
#include "ethernet.h"
#include "arp.h"

int main() {
    char dev[16] = "tap0";
    int fd = tun_alloc(dev);

    if (fd < 0) {
        fprintf(stderr, "Erreur lors de la création de l'interface\n");
        return 1;
    }

    printf("SOCKET: Système initialisé.\n"); // Log factice pour la cohérence
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
        
        if (ntohs(eth->ethertype) == 0x0806) {
            arp_handle_incoming(fd, eth);
        } else {
             printf("PAQUET RECU: Type=%04x\n", ntohs(eth->ethertype));
        }
    }

    close(fd);
    return 0;
}
