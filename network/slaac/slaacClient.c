/**
 * @file     dhcpClient.c
 */
/* Embedded Xinu, Copyright (C) 2008, 2013.  All rights reserved. */

#include <clock.h>
#include <device.h>
#include "slaac.h"
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <slaac.h>
#include <icmp6.h>

/**
 * @ingroup slaac
 *
 * SLAAC (Stateless Address Autoconfiguration) client for IPv6 autoconfiguration.
 *
 */
syscall slaacClient(int descrp, uint timeout, struct slaacData *data)
{
    if (0 == timeout)
    {
        printf("Timeout cannot be zero.\r\n");
        return SYSERR;
    }

    if (NULL == data)
    {
        printf("No data buffer provided.\r\n");
        return SYSERR;
    }

    if (NULL != netLookup(descrp))
    {
        printf("Network interface is up on device.\r\n");
        return SYSERR;
    }

    bzero(data, sizeof(*data));
    /* Client hardware address is known; get it from network device.  */
    if (SYSERR == control(descrp, ETH_CTRL_GET_MAC,
                            (long)data->clientHwAddr, 0))
    {
        printf("Failed to get client hardware address");
        return SYSERR;
    }

    dot2ipv6("fe80:0000:0000:0000:0000:0000:0000:0000", &(data->ip));

    // Create EUI-64 format (pg. 389)
    memcpy(&(data->ip.addr[8]), &(data->clientHwAddr), 3);
    memcpy(&(data->ip.addr[13]), &(data->clientHwAddr[3]), 3);
    data->ip.addr[11] = 0xFF;
    data->ip.addr[12] = 0xFE;
    data->ip.addr[8] |= 0b00000010;

    return OK;
}

void readNeighborAdvReply(int descrp, struct packet *pkt, struct netaddr* ip){
    const struct etherPkt *epkt;
    const struct ipv6Pkt *ipv6;
    const struct icmp6Pkt *icmp;
    int mtu;
    int linkhdrlen;
    uint maxlen;

    mtu = control(descrp, NET_GET_MTU, 0, 0);
    linkhdrlen = control(descrp, NET_GET_LINKHDRLEN, 0, 0);
    if (SYSERR == mtu || SYSERR == linkhdrlen)
    {
        printf("SYSERR\r\n");
        return;
    }

    maxlen = linkhdrlen + mtu;
    printf("Listening for neighbor advertisement\r\n");

    /* Receive packets until we find a response we're waiting for.  */
    do
    {
        /* Receive next packet from the network device.  */
        printf("Waiting for packet\r\n");
        int len = read(descrp, pkt->data, maxlen);
        printf("Packet\r\n");
        if (len == SYSERR || len <= 0)
        {
            printf("SYSERR\r\n");
            return;
        }

        pkt->len = len;

        /* Set up header pointers  */
        epkt = (const struct etherPkt *)pkt->data;
        ipv6 = (const struct ipv6Pkt *)epkt->data;
        icmp = (const struct icmp6Pkt *)ipv6->data;

        printf("Network layer packet is 0x%04X\r\n", net2hs(epkt->type));

        if(icmp->type == 136 && icmp->code == 0){
            printicmp6((struct icmp6Pkt *)icmp);
            printf("Found a NA packet!\r\n");
            return;
        }

    } while(TRUE);
}

void sendNeighborSol(int descrp, struct netaddr* ip){
    struct packet *neighborSolPkt;
    int retVal = 0;
    struct netaddr src;
    struct packet *pkt;

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        printf("Failed to acquire packet buffer");
        return;
    }

    char str2[40];
    netaddrsprintf(str2, ip);
    printf("sendNeighborSol IP: %s\r\n", str2);
    neighborSolPkt = icmp6NeighborSol(TRUE, ip);
    if(NULL == neighborSolPkt){
        printf("Could not create Neighbor Solicitaiton request!\r\n");
        netFreebuf(pkt);
        return;
    }

    dot2ipv6(UNSPECIFIED_ADDR, &src);
    retVal = ipv6Send(neighborSolPkt, &src, ip, NXT_HDR_ICMP);
    if(retVal == SYSERR) {
        printf("Could not send Neighbor Solicitaiton request!\r\n");
        netFreebuf(pkt);
        return;
    }

    printf("Sent neighbor solicitation packet\r\n");
    readNeighborAdvReply(descrp, pkt, ip);
}

