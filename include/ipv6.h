/*
 * @file ipv6.h
 *
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights reserved. */

#include <stddef.h>
#include <network.h>
#include <stdint.h>

#ifndef _IPv6_H_
#define _IPv6_H_

/* Tracing macros */
//#define TRACE_IPv6     TTY1
#ifdef TRACE_IPv6
#include <stdio.h>
#define IPv6_TRACE(...)     { \
		fprintf(TRACE_IPv6, "%s:%d (%d) ", __FILE__, __LINE__, gettid()); \
		fprintf(TRACE_IPv6, __VA_ARGS__); \
		fprintf(TRACE_IPv6, "\n"); }
#else
#define IPv6_TRACE(...)
#endif

#define IPv6_HDR_LEN       40
#define IPv6_HOP_LIMIT     255

#define IPv6_ADDR_LEN       16
#define IPv6_ADDR_UPMASK    0xFF00
#define IPv6_ADDR_SHIFT     8
#define IPv6_ADDR_LOMASK    0xFF

#define NXT_HDR_ICMP        58
#define ALL_ROUTER_MULTICAST_ADDR       "ff02:0000:0000:0000:0000:0000:0000:0002"
#define UNSPECIFIED_ADDR                "0000:0000:0000:0000:0000:0000:0000:0000"

struct ipv6Pkt             /**< IPv6 Packet Variables                   */
{
    uint32_t  ver_class_flow;     /**< IPv6 Version and Internet Header Len  */
    uint16_t  len;                /**< IPv6 packet payload length            */
    uint8_t  next_header;         /**< IPv6 identification                   */
    uint8_t  hop_limit;           /**< IPv6 Flags and Fragment offset        */
    uint8_t   src[IPv6_ADDR_LEN]; /**< IPv6 source                           */
    uint8_t   dst[IPv6_ADDR_LEN]; /**< IPv6 destination                      */
};


syscall dot2ipv6(const char *str, struct netaddr *ip);
syscall ipv6Send(struct packet *pkt, struct netaddr *src,
                 struct netaddr *dst, uchar proto);
syscall ipv6Recv(struct packet *pkt);
void printIpv6Packet(struct ipv6Pkt* pkt);
#endif                          /* _IPv6_H_ */
