#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "tcp.h"
#include "ip.h"
#include "socket.h"

static struct tcb tcb_table[MAX_TCB];

void tcp_init(void) { memset(tcb_table, 0, sizeof(tcb_table)); }

void tcp_handle_incoming(int fd, struct eth_hdr *eth, struct ip_hdr *ip) {
    struct tcp_hdr *tcp = (struct tcp_hdr *)ip->data;
    uint16_t dport = ntohs(tcp->dport); uint16_t sport = ntohs(tcp->sport);
    struct tcb *conn = NULL;
    for (int i = 0; i < MAX_TCB; i++) {
        if (tcb_table[i].state != TCP_CLOSED && tcb_table[i].sport == dport && tcb_table[i].dport == sport) {
            conn = &tcb_table[i]; break;
        }
    }

    if (tcp->flags & TCP_SYN) {
        if (conn) return;
        for (int i = 0; i < MAX_TCB; i++) {
            if (tcb_table[i].state == TCP_CLOSED) {
                conn = &tcb_table[i]; conn->state = TCP_SYN_RECEIVED;
                conn->saddr = ip->daddr; conn->daddr = ip->saddr;
                conn->sport = dport; conn->dport = sport;
                conn->ack = ntohl(tcp->seq) + 1; conn->seq = 1000;
                conn->last_send_time = time(NULL); conn->retransmit_count = 0; conn->curr_rto = TCP_RTO_INITIAL;
                conn->cwnd = INIT_CWND; conn->ssthresh = INIT_SSTHRESH;
                break;
            }
        }
        if (!conn) { tcp_send_reset(fd, eth, ip); return; }
        tcp->dport = htons(conn->dport); tcp->sport = htons(conn->sport);
        tcp->ack = htonl(conn->ack); tcp->seq = htonl(conn->seq);
        tcp->flags = TCP_SYN | TCP_ACK;
        tcp->csum = 0; tcp->csum = tcp_checksum(ip, tcp);
        write(fd, eth, sizeof(struct eth_hdr) + ntohs(ip->len));
        return;
    }

    if (conn) {
        uint32_t seg_ack = ntohl(tcp->ack);
        int tcp_data_len = ntohs(ip->len) - (ip->ihl * 4) - (tcp->off * 4);

        if (conn->state == TCP_SYN_RECEIVED && (tcp->flags & TCP_ACK)) {
            if (seg_ack == conn->seq + 1) { 
                conn->state = TCP_ESTABLISHED; conn->seq = seg_ack; 
                conn->last_send_time = time(NULL); conn->retransmit_count = 0; conn->curr_rto = TCP_RTO_INITIAL;
            }
        }
        else if (conn->state == TCP_ESTABLISHED && tcp_data_len > 0) {
            socket_push_data(conn->sport, (char *)tcp->data, tcp_data_len);
            conn->ack += tcp_data_len;
            
            // Slow Start : Augmenter cwnd à chaque ACK reçu
            if (conn->cwnd < conn->ssthresh) conn->cwnd++;
            
            tcp_send_ack(fd, eth, ip, conn);
            conn->last_send_time = time(NULL); conn->retransmit_count = 0; conn->curr_rto = TCP_RTO_INITIAL;
        }
        return;
    }
    tcp_send_reset(fd, eth, ip);
}

void tcp_check_timeouts(int fd) {
    time_t now = time(NULL);
    char buffer[1500];
    for (int i = 0; i < MAX_TCB; i++) {
        struct tcb *conn = &tcb_table[i];
        if (conn->state == TCP_SYN_RECEIVED || conn->state == TCP_ESTABLISHED) {
            if (difftime(now, conn->last_send_time) >= conn->curr_rto) {
                if (conn->retransmit_count >= MAX_RETRY) { conn->state = TCP_CLOSED; continue; }
                
                // Congestion Control : On réduit la fenêtre en cas de perte
                printf("TCP CONGESTION: Perte détectée. ssthresh=%d -> %d, cwnd=%d -> 1\n", 
                       conn->ssthresh, (conn->cwnd / 2), conn->cwnd);
                conn->ssthresh = (conn->cwnd > 1) ? (conn->cwnd / 2) : 2;
                conn->cwnd = 1;

                // Reconstruction et retransmission...
                struct eth_hdr *eth = (struct eth_hdr *)buffer;
                struct ip_hdr *ip = (struct ip_hdr *)(buffer + sizeof(struct eth_hdr));
                struct tcp_hdr *tcp = (struct tcp_hdr *)(buffer + sizeof(struct eth_hdr) + sizeof(struct ip_hdr));
                memcpy(eth->dmac, "\xff\xff\xff\xff\xff\xff", 6); memcpy(eth->smac, "\x00\x11\x22\x33\x44\x55", 6); eth->ethertype = htons(0x0800);
                ip->ihl = 5; ip->version = 4; ip->len = htons(40); ip->proto = 6; ip->saddr = conn->saddr; ip->daddr = conn->daddr;
                ip->csum = 0; ip->csum = ip_checksum(ip, 20);
                tcp->sport = htons(conn->sport); tcp->dport = htons(conn->dport);
                tcp->seq = htonl(conn->seq); tcp->ack = htonl(conn->ack);
                tcp->off = 5; tcp->flags = (conn->state == TCP_SYN_RECEIVED) ? (TCP_SYN | TCP_ACK) : TCP_ACK;
                tcp->win = htons(65535); tcp->csum = 0; tcp->csum = tcp_checksum(ip, tcp);
                write(fd, buffer, 54);
                
                conn->last_send_time = now; conn->retransmit_count++;
                conn->curr_rto *= 2; // Exponential Backoff
            }
        }
    }
}
