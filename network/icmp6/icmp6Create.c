#include <xinu.h>
#include <icmp6.h>
#include <stdlib.h>

syscall icmp6Create(struct packet *pkt, uchar type, uchar code, 
                uint datalen, struct netaddr *src, struct netaddr *dst) {
    
    struct icmp6Pkt *icmp;
    struct ipv6_puesdo_header *puesdo_header;

    /* Error check pointers */
    if (NULL == pkt)
    {
        return SYSERR;
    }

    if (dst->type != NETADDR_IPv6)
    {
        return SYSERR;
    }

    pkt->curr -= ICMP6_HEADER_LEN;
    pkt->len += ICMP6_HEADER_LEN;

    icmp = (struct icmp6Pkt *)pkt->curr;

    icmp->code = code;
    icmp->type = type;
    icmp->chksum = 0;

    pkt->curr -= IPv6_PUESDO_HEADER_LEN;
    pkt->len += IPv6_PUESDO_HEADER_LEN;

    puesdo_header = (struct ipv6_puesdo_header *)pkt->curr;

    puesdo_header->empty[0] = 0;
    puesdo_header->empty[1] = 0;
    puesdo_header->empty[2] = 0;
    puesdo_header->next_header = NXT_HDR_ICMP;
    memcpy(puesdo_header->src, src->addr, IPv6_ADDR_LEN);
    memcpy(puesdo_header->dst, dst->addr, IPv6_ADDR_LEN);
    puesdo_header->len = hl2net(ICMP6_HEADER_LEN + datalen);

    int chksum = netChksum(puesdo_header, ICMP6_HEADER_LEN + IPv6_PUESDO_HEADER_LEN + datalen);
    icmp->chksum = chksum;

    bzero(puesdo_header, sizeof(struct ipv6_puesdo_header));
    pkt->curr += IPv6_PUESDO_HEADER_LEN;
    pkt->len -= IPv6_PUESDO_HEADER_LEN;

    return OK;
}