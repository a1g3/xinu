/**
 * file ipv6Send.c
 * 
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <stddef.h>
#include <ipv6.h>
#include <network.h>
#include <string.h>
#include <route.h>

/**
 * @ingroup ipv6
 *
 * Send an outgoing IPv6 packet.
 * @param packet packet being sent
 * @param src source IP address
 * @param dst destination IP address
 * @param proto the protocol of the ip pkt
 * @return OK if packet was sent, TIMEOUT if ARP request timed out,
 * IPv6_NO_INTERFACE if interface does not exist, IPv6_NO_HOP if next hop
 * is unknown, SYSERR otherwise.
 */
syscall ipv6Send(struct packet *pkt, struct netaddr *src,
                 struct netaddr *dst, uchar proto)
{
    struct ipv6Pkt *ip;
    struct netaddr mac;
    int ver_class_flow = 0;

    /* Error check pointers */
    if ((NULL == pkt) || (NULL == dst))
    {
        printf("Invalid args\r\n");
        return SYSERR;
    }
    if (dst->type != NETADDR_IPv6)
    {
        printf("Invalid dst type");
        return SYSERR;
    }

    // IPv6 TODO: Lookup destination in route table
    mac.type = NETADDR_ETHERNET;
    mac.len = ETH_ADDR_LEN;
    mac.addr[0] = 0xAA;
    mac.addr[1] = 0xBB;
    mac.addr[2] = 0xCC;
    mac.addr[3] = 0xDD;
    mac.addr[4] = 0xEE;
    mac.addr[5] = 0xFF;

    /* Packet has next hop in route table */
    pkt->nif = &netiftab[0];

    /* Set up outgoing packet header */
    pkt->len += IPv6_HDR_LEN;
    pkt->curr -= IPv6_HDR_LEN;

    ip = (struct ipv6Pkt*)pkt->curr;
    ver_class_flow = 0b01100000000000000000000000000000;
    ip->ver_class_flow = hl2net(ver_class_flow);
    ip->len = 0;
    ip->next_header = proto;
    ip->hop_limit = IPv6_HOP_LIMIT;

    memcpy(ip->src, src->addr, IPv6_ADDR_LEN);
    memcpy(ip->dst, dst->addr, IPv6_ADDR_LEN);

    // IPv6 TODO: Fragment IPv6 packets (if needed)
    return netSend(pkt, &mac, NULL, ETHER_TYPE_IPv6);
}