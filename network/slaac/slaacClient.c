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

    // IPv6 TODO : Duplicate Address Detection 



    return OK;
}

syscall readRouterAdvertisementPackets(void){
    int maxlen;
    int mtu;
    int linkhdrlen;
    const struct etherPkt *epkt;
    const struct ipv6Pkt *ipv6;
    int descrp = ETH0;
    struct packet *pkt;

    mtu = control(descrp, NET_GET_MTU, 0, 0);
    linkhdrlen = control(descrp, NET_GET_LINKHDRLEN, 0, 0);
    if (SYSERR == mtu || SYSERR == linkhdrlen)
    {
        return SYSERR;
    }

    maxlen = linkhdrlen + mtu;

    /* Receive packets until we find a response we're waiting for.  */

    do
    {
        pkt = netGetbuf();

        /* Receive next packet from the network device.  */
        int len = read(descrp, pkt->data, maxlen);
        if (len == SYSERR || len <= 0)
        {
            return SYSERR;
        }

        pkt->len = len;
        printf("Received packet (len=%u).\r\n", pkt->len);

        epkt = (const struct etherPkt *)pkt->data;
        ipv6 = (const struct ipv6Pkt *)epkt->data;
        printf("Upper layer is 0x%04X (%d)\r\n", epkt->type, epkt->type);
        if(net2hs(epkt->type) == ETHER_TYPE_IPv6){
            printf("IPv6 Next Header: (%d)\r\n", ipv6->next_header);
            netFreebuf(pkt);
            return OK;
        }

        netFreebuf(pkt);
    } while (TRUE);
}

syscall sendRouterSolicitation(void)
{
    kprintf("\r\n===TEST BEGIN===\r\n");
    struct packet *result = NULL;
    int tid;
    int i;

    /* This somewhat of a hack to implement a timeout:  Wait for the reply in
     * another thread to avoid blocking on read().  */
    tid = create(readRouterAdvertisementPackets, NET_THR_STK, NET_THR_PRIO,
                 "readIpv6Packets", 0);
    if (isbadtid(tid))
    {
        return SYSERR;
    }

    ready(tid, RESCHED_YES, 0);

    //printf("\r\n\r\n=== ICMP Send ===\r\n");
    for(i = 0; i < 10; i++){
        printf("=== Sending Neighbor Solicitation ===\r\n");
        result = icmp6RouterSol();
        printf("icmp6RouterSol = %d\r\n", (int)result);
        sleep(1000);
    }

    /* Wait at most @timeout milliseconds for the thread to terminate before
     * returning TIMEOUT.  */
    if (TIMEOUT == recvtime(10000))
    {
        kill(tid);
        receive();
        printf("Timeout occured\r\n");
    }
    else
    {
        printf("Received an IPv6 packet!\r\n");
    }

    return OK;
}