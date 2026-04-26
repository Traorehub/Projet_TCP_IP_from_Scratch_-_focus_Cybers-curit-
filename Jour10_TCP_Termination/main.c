#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include "tun.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"
#include <net/if.h> // Required header
#include <string.h>
#define BUFLEN 65536


#include "socket.h"

int main(void) {
    int tun_fd;
    char buffer[2048];

    // Désactive le buffering pour voir les logs immédiatement avant le crash
    setbuf(stdout, NULL);

    char dev_name[IFNAMSIZ];
    strcpy(dev_name, "tap0");

    tun_fd = tun_alloc(dev_name);
    if (tun_fd < 0) {
        perror("Erreur création TAP");
        return 1;
    }


    printf("Interface tap0 créée. Configurez: sudo ip addr add 10.0.0.1/24 dev tap0 && sudo ip link set tap0 up\n");

    // --- INITIALISATION DE "L'APPLICATION" ---
    socket_init(); // Init de la couche socket
    struct socket *web_server = xsocket(); // Création du socket
    if (web_server) {
        xbind(web_server, 80); // On écoute sur le port 80
        printf("APP: Serveur Web simulé en écoute sur le port 80 via API Socket.\n");
    }
    // -----------------------------------------



    while (1) {
        int nread = read(tun_fd, buffer, sizeof(buffer));
        if (nread < 0) {
            perror("Lecture TAP");
            close(tun_fd);
            return 1;
        }

        struct eth_hdr *eth = (struct eth_hdr *)buffer;
        uint16_t type = ntohs(eth->ethertype);

        // Gestion ARP
        if (type == ETH_P_ARP) {
            arp_handle_incoming(tun_fd, eth);
        } 
        // Gestion IP
        else if (type == ETH_P_IP) {
            struct ip_hdr *ip = (struct ip_hdr *)(buffer + sizeof(struct eth_hdr));
            ip_handle_incoming(tun_fd, eth, ip);
        }

        // --- SIMULATION DU TRAVAIL DE L'APPLICATION ---
        // ... (Code Application existant) ...
        char app_buf[512];
        int recvd = xrecv(web_server, app_buf, sizeof(app_buf) - 1);
        if (recvd > 0) {
            app_buf[recvd] = '\0'; // Null-terminate pour affichage
            printf("\n--- APPLICATION USER-SPACE ---\n");
            printf("Reçu %d octets sur le port 80 :\n%s\n", recvd, app_buf);
            printf("------------------------------\n\n");
        }
        // ---------------------------------------------

        // Gèstion des Timeouts TCP (Jour 9)
        tcp_check_timeouts(tun_fd);
    }




    close(tun_fd);
    return 0;
}

