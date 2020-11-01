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


#define IPv6_ADDR_LEN       16
#define IPv6_ADDR_UPMASK    0xFF00
#define IPv6_ADDR_SHIFT     8
#define IPv6_ADDR_LOMASK    0xFF

struct ipv6Pkt             /**< IPv6 Packet Variables                   */
{
    uint32_t  ver_class_flow;     /**< IPv6 Version and Internet Header Len  */
    uint16_t  len;                /**< IPv6 packet length including IHL      */
    uint8_t  next_header;         /**< IPv6 identification                   */
    uint8_t  hop_limit;           /**< IPv6 Flags and Fragment offset        */
    uint8_t   src[IPv6_ADDR_LEN]; /**< IPv6 source                           */
    uint8_t   dst[IPv6_ADDR_LEN]; /**< IPv6 destination                      */
    uint8_t   opts[1];            /**< Options and padding is variable       */
};

syscall dot2ipv6(const char *str, struct netaddr *ip);

#endif                          /* _IPv6_H_ */
