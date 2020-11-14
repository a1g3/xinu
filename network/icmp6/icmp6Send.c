#include <xinu.h>
#include <icmp6.h>
#include <stdlib.h>

syscall icmp6Send(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst) {
    
    struct icmp6Pkt *icmp;
    struct ipv6_puesdo_header puesdo_header;

    /* Error check pointers */
    if (NULL == pkt)
    {
        return SYSERR;
    }

    bzero(&puesdo_header, sizeof(struct ipv6_puesdo_header));

    if (src->type != NETADDR_IPv6 || src->type != NETADDR_IPv6)
    {
        return SYSERR;
    }

    puesdo_header.len = datalen + ICMP6_HEADER_LEN;
    puesdo_header.next_header = NXT_HDR_ICMP;
    memcpy(&(puesdo_header.src), src->addr, IPv6_ADDR_LEN);
    memcpy(&(puesdo_header.dst), dst->addr, IPv6_ADDR_LEN);

    pkt->curr -= ICMP6_HEADER_LEN;
    pkt->len += ICMP6_HEADER_LEN;

    icmp = (struct icmp6Pkt *)pkt->curr;
    icmp->code = code;
    icmp->type = type;
    icmp->chksum = netChksum(&puesdo_header, sizeof(struct ipv6_puesdo_header));

    printicmp6(icmp);

    return ipv6Send(pkt, src, dst, NXT_HDR_ICMP);
}