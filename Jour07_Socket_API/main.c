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

    arp_init();
    tcp_init();
    socket_init();
    
    // Simulation d'une application serveur
    struct socket *s = xsocket();
    xbind(s, 80);

    printf("Interface %s prête. En attente de connexions sur le port 80...\n", dev);

    unsigned char buf[1600];
    char app_buf[1024];

    while (1) {
        int len = read(fd, buf, sizeof(buf));
        if (len < 0) break;

        struct eth_hdr *eth = (struct eth_hdr *)buf;
        uint16_t ethertype = ntohs(eth->ethertype);

        if (ethertype == 0x0806) {
            arp_handle_incoming(fd, eth);
        } else if (ethertype == 0x0800) {
            struct ip_hdr *ip = (struct ip_hdr *)eth->payload;
            ip_handle_incoming(fd, eth, ip);
        }

        // Lecture depuis le socket (Simulation User-Space)
        int recvd = xrecv(s, app_buf, sizeof(app_buf) - 1);
        if (recvd > 0) {
            app_buf[recvd] = '\0';
            printf("\n--- APPLICATION USER-SPACE ---\n");
            printf("Reçu %d octets : %s\n", recvd, app_buf);
            printf("------------------------------\n");
        }
    }

    close(fd);
    return 0;
}
