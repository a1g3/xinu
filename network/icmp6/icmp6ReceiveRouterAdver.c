#include <xinu.h>
#include <icmp6.h>

syscall isRouterAdverValid(struct packet *pkt){
    const struct etherPkt *epkt;
    const struct ipv6Pkt *ipv6;
    const struct icmp6Pkt *icmp;

    epkt = (const struct etherPkt *)pkt->data;
    ipv6 = (const struct ipv6Pkt *)epkt->data;
    icmp = (const struct icmp6Pkt *)ipv6->data;

    if(ipv6->hop_limit != IPv6_HOP_LIMIT)
        return SYSERR;
    
    // TODO: Check checksum, link local address

    if(icmp->type != 134)
        return SYSERR;

    if(icmp->code != 0)
        return SYSERR;
    
    if (ipv6->len < 16)
        return SYSERR;

    return OK;
}

syscall icmp6ReceiveRouterAdver(struct packet *pkt)
{
    struct icmp6Options opts;
    //const struct etherPkt *epkt;
    //const struct ipv6Pkt *ipv6;

    if (SYSERR == isRouterAdverValid(pkt))
        return SYSERR;

    //epkt = (const struct etherPkt *)pkt->data;
    //ipv6 = (const struct ipv6Pkt *)epkt->data;
    
    int ret = icmp6ParseOptions(pkt, &opts);
    printf("icmp6ParseOptions = %d\r\n", ret);
    
    return OK;
}