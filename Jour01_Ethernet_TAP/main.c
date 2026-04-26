#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tun.h"
#include "ethernet.h"

int main() {
    char dev[16] = "tap0";
    int fd = tun_alloc(dev);

    if (fd < 0) {
        fprintf(stderr, "Erreur lors de la création de l'interface\n");
        return 1;
    }

    printf("Interface %s créée. Configurez: sudo ip addr add 10.0.0.1/24 dev %s && sudo ip link set %s up\n", dev, dev, dev);

    unsigned char buf[1600];
    while (1) {
        int len = read(fd, buf, sizeof(buf));
        if (len < 0) {
            perror("Erreur de lecture");
            break;
        }

        struct eth_hdr *eth = (struct eth_hdr *)buf;
        printf("TRAME ETHERNET: SMAC=%02x:%02x:%02x:%02x:%02x:%02x -> DMAC=%02x:%02x:%02x:%02x:%02x:%02x | Type=%04x\n",
               eth->smac[0], eth->smac[1], eth->smac[2], eth->smac[3], eth->smac[4], eth->smac[5],
               eth->dmac[0], eth->dmac[1], eth->dmac[2], eth->dmac[3], eth->dmac[4], eth->dmac[5],
               ntohs(eth->ethertype));
    }

    close(fd);
    return 0;
}
