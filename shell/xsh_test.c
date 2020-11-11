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

    /* Setup structures */
    int test = dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD:EF12:3456", &a);
    printf("dot2ipv6 = %d\r\n", test);
    for (int i = 0; i < 16; i++)
    {
        printf("%d. 0x%2X\r\n", i, a.addr[i]);
    }

    test = dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD::", &b);
    printf("dot2ipv6 = %d\r\n", test);
    for (int i = 0; i < 16; i++)
    {
        printf("%d. 0x%2X\r\n", i, b.addr[i]);
    }

    struct packet *pkt = netGetbuf();
    int result = ipv6Send(pkt, &a, &b, 59);
    printf("ipv6Send = %d\r\n", result);

    return 0;
}
