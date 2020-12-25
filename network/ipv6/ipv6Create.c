/**
 * @file     ipv6Create.c
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <stdlib.h>
#include <string.h>
#include <slaac.h>
#include <icmp6.h>

syscall ipv6Create(struct packet *pkt, struct netaddr *src,
                 struct netaddr *dst, uchar proto) {
    struct ipv6Pkt *ip;
    int ver_class_flow = 0;
    int originalLen = 0;

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

    /* Set up outgoing packet header */
    originalLen = pkt->len;
    pkt->len += IPv6_HDR_LEN;
    pkt->curr -= IPv6_HDR_LEN;

    ip = (struct ipv6Pkt*)pkt->curr;
    ver_class_flow = 0b01100000000000000000000000000000;
    ip->ver_class_flow = hl2net(ver_class_flow);
    ip->len = hs2net(originalLen);
    ip->next_header = proto;
    ip->hop_limit = IPv6_HOP_LIMIT;

    memcpy(ip->src, src->addr, IPv6_ADDR_LEN);
    memcpy(ip->dst, dst->addr, IPv6_ADDR_LEN);

    return OK;
}