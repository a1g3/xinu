/*
 * @file icmp6.h
 *
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#ifndef _ICMP6_H_
#define _ICMP6_H_

#include <stddef.h>
#include <network.h>
#include <stdint.h>
#include <ipv6.h>

#define ICMP6_HEADER_LEN		4       //data not included

struct ipv6_puesdo_header {
    uint8_t   src[IPv6_ADDR_LEN]; /**< IPv6 source                           */
    uint8_t   dst[IPv6_ADDR_LEN]; /**< IPv6 destination                      */
    uint32_t  len;
    uint8_t   next_header;
};

struct icmp6Pkt                 /**< ICMP Packet         */
{
    uint8_t type;                 /**< ICMP type            */
    uint8_t code;                 /**< ICMP code            */
    uint16_t chksum;              /**< ICMP checksum        */
    //uint8_t data[1];              /**< ICMP data            */
};

struct icmp6_options {
    uint8_t zeros[4];
    uint8_t type;
    uint8_t len;
    uint8_t addr[ETH_ADDR_LEN];
};

syscall icmp6Send(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst);
syscall icmp6RouterSol(void);
void printicmp6(struct icmp6Pkt *pkt);

#endif                          /* _ICMP6_H_ */