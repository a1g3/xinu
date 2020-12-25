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
    data->mac.type = NETADDR_ETHERNET;
    data->mac.len = ETH_ADDR_LEN;
    if (SYSERR == control(descrp, ETH_CTRL_GET_MAC,
                            (long)data->mac.addr, 0))
    {
        printf("Failed to get client hardware address");
        return SYSERR;
    }

    dot2ipv6("fe80:0000:0000:0000:0000:0000:0000:0000", &(data->ip));

    // Create EUI-64 format (pg. 389)
    memcpy(&(data->ip.addr[8]), &(data->mac.addr), 3);
    memcpy(&(data->ip.addr[13]), &(data->mac.addr[3]), 3);
    data->ip.addr[11] = 0xFF;
    data->ip.addr[12] = 0xFE;
    data->ip.addr[8] |= 0b00000010;

    // IPv6 TODO : Duplicate Address Detection 

    if(SYSERR == sendRouterSolicitation(data)){
        return SYSERR;
    }

    return OK;
}

syscall ethernetCreate(struct packet *pkt, struct slaacData *data, struct netaddr *dstMac, ushort proto){
    struct etherPkt *ether;

    /* Make space for Link-Level header */
    pkt->curr -= ETH_HDR_LEN;
    pkt->len += ETH_HDR_LEN;
    ether = (struct etherPkt *)(pkt->curr);

    memcpy(ether->dst, dstMac->addr, ETH_ADDR_LEN);
    memcpy(ether->src, data->mac.addr, ETH_ADDR_LEN);
    ether->type = hs2net(proto);

    return OK;
}

syscall readRouterAdvertisementPackets(struct slaacData *data){
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

syscall sendRouterSolicitation(struct slaacData *data)
{
    struct packet *result = NULL;
    struct netaddr b;
    struct netaddr mac;
    int tid;
    int ipv6Result;
    int i;

    dot2ipv6(ALL_ROUTER_MULTICAST_ADDR, &b);

    mac.type = NETADDR_ETHERNET;
    mac.len = ETH_ADDR_LEN;
    mac.addr[0] = 0x33;
    mac.addr[1] = 0x33;
    mac.addr[2] = 0x00;
    mac.addr[3] = 0x00;
    mac.addr[4] = 0x00;
    mac.addr[5] = 0x02;

    /* This somewhat of a hack to implement a timeout:  Wait for the reply in
     * another thread to avoid blocking on read().  */
    tid = create(readRouterAdvertisementPackets, NET_THR_STK, NET_THR_PRIO,
                 "readIpv6Packets", 1, data);
    if (isbadtid(tid))
    {
        return SYSERR;
    }

    ready(tid, RESCHED_YES, 0);

    for(i = 0; i < 3; i++){
        result = icmp6RouterSol(&(data->ip), &(data->mac));
        ipv6Result = ipv6Create(result, &(data->ip), &b, NXT_HDR_ICMP);
        ipv6Result = ethernetCreate(result, data, &mac, ETHER_TYPE_IPv6);

        if(result->len != write(ETH0, result->curr, result->len)){
            printf("Did not write correct size to ETH0 device!\r\n");
        }

        netFreebuf(result);
        sleep(500);
    }

    /* Wait at most @timeout milliseconds for the thread to terminate before
     * returning TIMEOUT.  */
    if (TIMEOUT == recvtime(500))
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