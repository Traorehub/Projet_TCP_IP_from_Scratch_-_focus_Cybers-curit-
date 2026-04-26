#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tun.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"
#include "tcp.h"
#include "socket.h"

int main() {
    char dev[16] = "tap0";
    int fd = tun_alloc(dev);
    if (fd < 0) return 1;

    arp_init(); tcp_init(); socket_init();
    struct socket *s = xsocket(); xbind(s, 80);

    printf("Interface %s prête. Gestion des Retransmissions activée.\n", dev);

    unsigned char buf[1600];
    char app_buf[1024];

    while (1) {
        // Pour éviter de bloquer indéfiniment et permettre le check_timeouts
        // On pourrait utiliser select() ou un timeout sur read().
        // Simulation simple ici : check_timeouts est appelé à chaque paquet
        int len = read(fd, buf, sizeof(buf));
        
        tcp_check_timeouts(fd); // Vérification périodique

        if (len < 0) continue;

        struct eth_hdr *eth = (struct eth_hdr *)buf;
        uint16_t ethertype = ntohs(eth->ethertype);

        if (ethertype == 0x0806) {
            arp_handle_incoming(fd, eth);
        } else if (ethertype == 0x0800) {
            struct ip_hdr *ip = (struct ip_hdr *)eth->payload;
            ip_handle_incoming(fd, eth, ip);
        }

        int recvd = xrecv(s, app_buf, sizeof(app_buf) - 1);
        if (recvd > 0) {
            app_buf[recvd] = '\0';
            printf("\n应用(Application): %s\n", app_buf);
        }
    }

    close(fd);
    return 0;
}
