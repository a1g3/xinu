/**
 * @file dhcp.h
 *
 */
/* Embedded Xinu, Copyright (C) 2021.  All rights reserved. */

#ifndef _SLAAC_H_
#define _SLAAC_H_

#include <stddef.h>
#include <network.h>
#include <ether.h>

struct slaacData
{
    struct netaddr ip;
    
    uchar clientHwAddr[ETH_ADDR_LEN];
};

syscall slaacClient(int descrp, uint timeout, struct slaacData *data);

#endif /* _SLAAC_H_ */
