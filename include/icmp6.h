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

#define ICMP6_OPTION_SOURCE_ADDR    1
#define ICMP6_OPTION_TARGET_ADDR    2
#define ICMP6_OPTION_PREFIX_INFO    3
#define ICMP6_OPTION_MTU            5
#define ICMP6_OPTION_ROUTE_INFO     24

struct ipv6_puesdo_header {
    uint8_t   src[IPv6_ADDR_LEN]; /**< IPv6 source                           */
    uint8_t   dst[IPv6_ADDR_LEN]; /**< IPv6 destination                      */
    uint32_t  len;
    uint8_t   resv[3];
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

// Specified in https://tools.ietf.org/html/rfc4191#section-2.2
struct icmp6RouterAdv {
    uint8_t curr_hop_limit;
    uint8_t moh_pref;
    uint16_t router_lifetime;
    uint32_t reachable_time;
    uint32_t retrans_time;
    uint8_t data[1];
};

#define ICMP6_OPTION_LINK_DEST_LEN      8
struct icmp6LinkDestOption {
    uint8_t type;
    uint8_t len;
    uint8_t addr[ETH_ADDR_LEN];
    uint8_t next[1];
};

#define ICMP6_OPTION_MTU_LEN            8
struct icmp6MtuOption {
    uint8_t type;
    uint8_t len;
    uint8_t resv[2];
    uint32_t mtu;
    uint8_t next[1];
};

struct icmp6OptionBase {
    uint8_t type;
    uint8_t len;
    uint8_t data[1];
};

#define ICMP6_OPTION_PREFIX_LEN            32
struct icmp6PrefixInfoOption {
    uint8_t type;
    uint8_t len;
    uint8_t prefix_len;
    uint8_t la_flags;
    uint32_t valid_lifetime;
    uint32_t preferred_lifetime;
    uint32_t resv;
    uint8_t prefix[IPv6_ADDR_LEN];
    uint8_t next[1];
};

#define ICMP6_ROUTE_INFO_LEN               24
struct icmp6RouteInfoOption {
    uint8_t type;
    uint8_t len;
    uint8_t prefix_len;
    uint8_t route_prefix;
    uint32_t route_lifetime;
    uint8_t prefix[IPv6_ADDR_LEN];
    uint8_t next[1];
};

struct icmp6Options {
    struct icmp6LinkDestOption source;
    struct icmp6LinkDestOption target;
    struct icmp6MtuOption mtu;
    struct icmp6PrefixInfoOption prefix;    
    struct icmp6RouteInfoOption route;
};

syscall icmp6Create(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst);
struct packet *icmp6RouterSol(struct netaddr *src, struct netaddr *mac);
struct packet *icmp6NeighborSol(bool isDuplicateAddrDetection, struct netaddr *dst, struct netaddr *mac);
struct packet *icmp6EchoRequest(struct netaddr *dst, ushort id, ushort seq);
void printicmp6(struct icmp6Pkt *pkt);
void printicmp6RouterAdvertisement(struct icmp6RouterAdv *pkt);
void printicmp6LinkDestOption(struct icmp6LinkDestOption *pkt);
void printicmp6MtuOption(struct icmp6MtuOption *pkt);
void printicmp6PrefixInfoOption(struct icmp6PrefixInfoOption *pkt);
void printicmp6RouteInfoOption(struct icmp6RouteInfoOption *pkt);
void icmp6AddLinkDestOption(struct packet *pkt, struct netaddr *mac);
syscall icmp6ParseOptions(struct packet *pkt, struct icmp6Options *opts);
syscall icmp6ReceiveRouterAdver(struct packet *pkt);
syscall icmp6Send(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst);
syscall isRouterAdverValid(struct packet *pkt);

#endif                          /* _ICMP6_H_ */