/**
 * @file dot2ipv6.c
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <ipv6.h>
#include <network.h>
#include <stdio.h>

/**
 * @ingroup ipv6
 *
 * Convert an IPv6 address in dotted-decimal notation into a netaddr.
 *
 * @param str
 *      The dotted-decimal IPv6 address string to convert.
 * @param ip
 *      The netaddr structure for conversion result.
 *
 * @return
 *      ::OK if conversion was successful, otherwise ::SYSERR.
 */
syscall dot2ipv6(const char *str, struct netaddr *ip)
{
    uint o0 = 0, o1 = 0, o2 = 0, o3 = 0, o4 = 0, o5 = 0, o6 = 0, o7 = 0;

    // TODO: Support IPv6 addresses such as 2001:db8::2:1

    /* Error check pointers */
    if ((NULL == str) || (NULL == ip))
    {
        return SYSERR;
    }

    int parseResult =
        sscanf(str, "%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X", &o0, &o1,
               &o2, &o3, &o4, &o5, &o6, &o7);
    if (parseResult > 8 || parseResult <= 0)
    {
        return SYSERR;
    }

    ip->addr[0] = (o0 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[1] = o0 & IPv6_ADDR_LOMASK;
    ip->addr[2] = (o1 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[3] = o1 & IPv6_ADDR_LOMASK;
    ip->addr[4] = (o2 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[5] = o2 & IPv6_ADDR_LOMASK;
    ip->addr[6] = (o3 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[7] = o3 & IPv6_ADDR_LOMASK;
    ip->addr[8] = (o4 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[9] = o4 & IPv6_ADDR_LOMASK;
    ip->addr[10] = (o5 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[11] = o5 & IPv6_ADDR_LOMASK;
    ip->addr[12] = (o6 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[13] = o6 & IPv6_ADDR_LOMASK;
    ip->addr[14] = (o7 & IPv6_ADDR_UPMASK) >> IPv6_ADDR_SHIFT;
    ip->addr[15] = o7 & IPv6_ADDR_LOMASK;
    ip->type = NETADDR_IPv6;
    ip->len = IPv6_ADDR_LEN;

    return OK;
}
