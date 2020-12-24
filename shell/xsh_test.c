/**
 * @file     xsh_test.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <stddef.h>
#include <stdio.h>
#include <ether.h>
#include <interrupt.h>
#include <network.h>
#include <testsuite.h>
#include <ipv6.h>
#include <icmp6.h>

/**
 * @ingroup shell
 *
 * Shell command (test) provides a mechanism for testing Xinu features.  The
 * action and output varies depending on the feature currently being tested.
 * This is not meant to serve as a permanent shell command for a particular
 * action.
 * @param nargs number of arguments
 * @param args  array of arguments
 * @return non-zero value on error
 */
void readIpv6Packets();

shellcmd xsh_test(int nargs, char *args[])
{
    kprintf("\r\n===TEST BEGIN===\r\n");
    struct netaddr a;
    struct netaddr b;
    struct packet *result = NULL;
    int tid;
    int i;

    /* Setup structures */
    dot2ipv6(ALL_ROUTER_MULTICAST_ADDR, &a);
    dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD::", &b);

    //struct packet *pkt = netGetbuf();
    //result = ipv6Send(pkt, &a, &b, 59);
    //printf("ipv6Send = %d\r\n", result);

    /* This somewhat of a hack to implement a timeout:  Wait for the reply in
     * another thread to avoid blocking on read().  */
    tid = create(readIpv6Packets, NET_THR_STK, NET_THR_PRIO,
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

    return 0;
}

void readIpv6Packets(){
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
            return;
        }

        netFreebuf(pkt);
    } while (TRUE);
}
