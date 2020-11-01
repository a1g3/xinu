/**
 * @file     netaddrsprintf.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <network.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @ingroup network
 *
 * Format a network address and place in a null-terminated string.
 *
 * @param str
 *      Pointer to the output string, which must have enough space for the
 *      appropriate type of network address, including the null terminator.
 * @param addr
 *      Network address to format.
 *
 * @return
 *      A pointer to the resulting string cast to an @c int.
 */
int netaddrsprintf(char *str, const struct netaddr *addr)
{
    switch (addr->type)
    {
    case NETADDR_ETHERNET:
        sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X",
                addr->addr[0], addr->addr[1], addr->addr[2],
                addr->addr[3], addr->addr[4], addr->addr[5]);
        break;
    case NETADDR_IPv4:
        sprintf(str, "%d.%d.%d.%d",
                addr->addr[0], addr->addr[1], addr->addr[2],
                addr->addr[3]);
        break;
    case NETADDR_IPv6:
        sprintf(str,
                "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                addr->addr[0], addr->addr[1], addr->addr[2],
                addr->addr[3], addr->addr[4], addr->addr[5],
                addr->addr[6], addr->addr[7], addr->addr[8],
                addr->addr[9], addr->addr[10], addr->addr[11],
                addr->addr[12], addr->addr[13], addr->addr[14],
                addr->addr[15]);
        break;
    case NULL:
        sprintf(str, "NULL");
        break;
    default:
        sprintf(str, "ERROR");
    }
    return (int)str;
}
