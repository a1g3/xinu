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
    
    return 0;
}
