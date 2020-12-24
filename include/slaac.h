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

#define DHCP_RECV_STKSIZE NET_THR_STK
#define DHCP_RECV_PRIO    NET_THR_PRIO

struct slaacData
{
    struct netaddr ip;
    
    uchar clientHwAddr[ETH_ADDR_LEN];
};

syscall slaacClient(int descrp, uint timeout, struct slaacData *data);
void readNeighborAdvReply(int descrp, struct packet *pkt, struct netaddr* ip);
void sendNeighborSol(int descrp, struct netaddr* ip);

#endif /* _SLAAC_H_ */
