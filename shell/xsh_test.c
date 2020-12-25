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
    int ipv6Result;

    /* Setup structures */
    dot2ipv6("1111:1111:1111:1111:1111:1111::", &a);
    dot2ipv6(ALL_ROUTER_MULTICAST_ADDR, &b);

    result = icmp6RouterSol(&a, &(netiftab[0].hwaddr));
    printf("icmp6NeighborSol = 0x%08X\r\n", result);
    ipv6Result = ipv6Send(result, &a, &b, NXT_HDR_ICMP);
    printf("ipv6Send = %d\r\n", ipv6Result);
    
    return 0;
}
