/**
 * @file icmp6EchoRequest.c
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <icmp6.h>
#include <stdlib.h>

/**
 * @ingroup icmp
 *
 * Send an ICMP Echo (Ping) Request.
 * @param dst destination address
 * @param id  ping stream identifier
 * @param seq sequence number
 * @return OK if packet was sent, otherwise SYSERR
 */
syscall icmp6EchoRequest(struct netaddr *dst, ushort id, ushort seq)
{
    struct packet *pkt;
    struct icmp6Echo *echo;
    struct netaddr a;
    struct netaddr b;

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        return SYSERR;
    }

    pkt->len = sizeof(struct icmp6Echo);
    pkt->curr -= pkt->len;

    echo = (struct icmp6Echo *)pkt->curr;
    echo->id = hs2net(id);
    echo->seq = hs2net(seq);

    /* Setup structures */
    dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD:EF12:3456", &a);
    dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD::", &b);

    return icmp6Send(pkt, 128, 0, pkt->len, &a, &b);
}