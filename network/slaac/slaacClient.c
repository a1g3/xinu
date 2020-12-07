/**
 * @file     dhcpClient.c
 */
/* Embedded Xinu, Copyright (C) 2008, 2013.  All rights reserved. */

#include <clock.h>
#include <device.h>
#include "slaac.h"
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <slaac.h>
#include <icmp6.h>

/**
 * @ingroup slaac
 *
 * SLAAC (Stateless Address Autoconfiguration) client for IPv6 autoconfiguration.
 *
 */
syscall slaacClient(int descrp, uint timeout, struct slaacData *data)
{
    int result = 0;

    if (0 == timeout)
    {
        printf("Timeout cannot be zero.\r\n");
        return SYSERR;
    }

    if (NULL == data)
    {
        printf("No data buffer provided.\r\n");
        return SYSERR;
    }

    if (NULL != netLookup(descrp))
    {
        printf("Network interface is up on device.\r\n");
        return SYSERR;
    }

    bzero(data, sizeof(*data));
    /* Client hardware address is known; get it from network device.  */
    if (SYSERR == control(descrp, ETH_CTRL_GET_MAC,
                            (long)data->clientHwAddr, 0))
    {
        printf("Failed to get client hardware address");
        return SYSERR;
    }

    dot2ipv6("fe80:0000:0000:0000:0000:0000:0000:0000", &(data->ip));

    // Create EUI-64 format (pg. 389)
    memcpy(&(data->ip.addr[8]), &(data->clientHwAddr), 3);
    memcpy(&(data->ip.addr[13]), &(data->clientHwAddr[3]), 3);
    data->ip.addr[11] = 0xFF;
    data->ip.addr[12] = 0xFE;
    data->ip.addr[8] |= 0b00000010;

    result = icmp6NeighborSol(TRUE, &(data->ip));
    if(SYSERR == result){
        printf("Could not send Neighbor Solicitaiton request!\r\n");
        return SYSERR;
    }

    return OK;
}