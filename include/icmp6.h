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
#define IPv6_PUESDO_HEADER_LEN		40
#define ICMP6_ROUTER_SOL_LEN        4
#define ICMP6_NEIGHBOR_SOL_LEN      20
#define ICMP6_LINKDESTOPTION_LEN    8

struct ipv6_puesdo_header {
    uint8_t   src[IPv6_ADDR_LEN]; /**< IPv6 source                           */
    uint8_t   dst[IPv6_ADDR_LEN]; /**< IPv6 destination                      */
    uint32_t  len;
    uint8_t   empty[3];
    uint8_t   next_header;
};

struct icmp6Pkt                 /**< ICMP Packet         */
{
    uint8_t type;                 /**< ICMP type            */
    uint8_t code;                 /**< ICMP code            */
    uint16_t chksum;              /**< ICMP checksum        */
    uint8_t data[1];              /**< ICMP data            */
};

struct icmp6Echo
{
    ushort id;
    ushort seq;
};

struct icmp6NeighborSol {
    uint8_t zeros[4];
    uint8_t addr[IPv6_ADDR_LEN];
};

struct icmp6RouterSol {
    uint8_t zeros[4];
};

struct icmp6LinkDestOption {
    uint8_t type;
    uint8_t len;
    uint8_t addr[ETH_ADDR_LEN];
};

syscall icmp6Create(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst);
struct packet *icmp6RouterSol(void);
struct packet *icmp6NeighborSol(bool isDuplicateAddrDetection, struct netaddr *target);
struct packet *icmp6EchoRequest(struct netaddr *dst, ushort id, ushort seq);
void printicmp6(struct icmp6Pkt *pkt);
void icmp6AddLinkDestOption(struct packet *pkt);
syscall icmp6Send(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst);

#endif                          /* _ICMP6_H_ */