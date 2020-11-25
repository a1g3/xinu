/* 
 * file ipv6Recv.c
 * 
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <stddef.h>
#include <ipv6.h>
#include <network.h>

/**
 * @ingroup ipv6
 *
 * Process an incoming IPv6 packet.
 * @param pkt pointer to the incoming packet
 * @return OK if packet was processed succesfully, otherwise SYSERR
 */
syscall ipv6Recv(struct packet *pkt)
{
    struct ipv6Pkt *ip;
    struct netaddr dst;
    struct netaddr src;
    ushort iplen;

    /* Error check pointers */
    if (NULL == pkt)
    {
        return SYSERR;
    }

    /* Setup pointer to IPv4 header */
    pkt->nethdr = pkt->curr;
    ip = (struct ipv6Pkt *)pkt->curr;

    /* Verify the IP packet is valid */
    if (!(ip->ver_class_flow & 0x60000000))
    {
        IPv6_TRACE("Invalid packet");
        netFreebuf(pkt);
        return SYSERR;
    }

    /* Obtain destination and source IP addresses */
    dst.type = NETADDR_IPv6;
    dst.len = IPv6_ADDR_LEN;
    memcpy(dst.addr, ip->dst, dst.len);
    src.type = NETADDR_IPv6;
    src.len = IPv6_ADDR_LEN;
    memcpy(src.addr, ip->src, src.len);

    /* The Ethernet driver pads packets less than 60 bytes in length.
     * If the packet length returned from the Ethernet driver (pkt->len)
     * does not agree with the packet headers, adjust the packet length 
     * to remove padding. */
    iplen = net2hs(ip->len) + IPv6_HDR_LEN;
    if ((pkt->len - pkt->nif->linkhdrlen) > iplen)
    {
        pkt->len = pkt->nif->linkhdrlen + iplen;
    }

    /* Move current pointer to application level header */
    pkt->curr += IPv6_HDR_LEN;

    IPv6_TRACE("IPv4 proto %d", ip->next_header);
    /* Switch on packet protocol */
    switch (ip->next_header)
    {
        /* ICMP Packet */
        case NXT_HDR_ICMP:
            //TODO: ICMP RECEIVE
            break;
    }

    return OK;
}