/**
 * @file     xsh_test.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <stddef.h>
#include <stdio.h>
#include <ether.h>
#include <interrupt.h>
#include <ipv4.h>
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
	char str[128];

    /* Setup structures */
    a.type = NETADDR_IPv6;
    a.len = IPv6_ADDR_LEN;
    a.addr[0] = 0x12;
    a.addr[1] = 0x34;
    a.addr[2] = 0x56;
    a.addr[3] = 0x78;
    a.addr[4] = 0x90;
    a.addr[5] = 0xAB;
    a.addr[6] = 0xCD;
    a.addr[7] = 0xEF;
    a.addr[8] = 0x12;
    a.addr[9] = 0x34;
    a.addr[10] = 0x56;
    a.addr[11] = 0x78;
    a.addr[12] = 0x90;
    a.addr[13] = 0xAB;
    a.addr[14] = 0xCD;
    a.addr[15] = 0xEF;

    netaddrsprintf(str, &a);
    printf("\tIP %s", str);
	bzero(&str, sizeof(str));

    /* Setup structures */
    a.type = NETADDR_IPv6;
    a.len = IPv6_ADDR_LEN;
    a.addr[0] = 0x12;
    a.addr[1] = 0x34;
    a.addr[2] = 0x56;
    a.addr[3] = 0x78;
    a.addr[4] = 0x90;
    a.addr[5] = 0xAB;
    a.addr[6] = 0xCD;
    a.addr[7] = 0xEF;
    a.addr[8] = 0x12;
    a.addr[9] = 0x34;
    a.addr[10] = 0x56;
    a.addr[11] = 0x78;
    a.addr[12] = 0x0;
    a.addr[13] = 0x0;
    a.addr[14] = 0x0;
    a.addr[15] = 0x0;

	netaddrsprintf(str, &a);
    printf("\tIP %s", str);
	bzero(&str, sizeof(str));

	return 0;
}
