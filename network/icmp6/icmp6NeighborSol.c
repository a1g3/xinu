#include <xinu.h>
#include <icmp6.h>

//https://blogs.infoblox.com/ipv6-coe/slaac-to-basics-part-1-of-2/
syscall icmp6NeighborSol(bool isDuplicateAddrDetection, struct netaddr *target)
{
    struct packet *pkt;
    struct netaddr src;
    struct icmp6NeighborSol *neighborSol;

    if(target != NULL && target->type != NETADDR_IPv6){
        return SYSERR;
    }

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        printf("Failed to acquire packet buffer");
        return SYSERR;
    }

    if(!isDuplicateAddrDetection){
        icmp6AddLinkDestOption(pkt);
    }

    pkt->len += ICMP6_NEIGHBOR_SOL_LEN;
    pkt->curr -= ICMP6_NEIGHBOR_SOL_LEN;
    neighborSol = (struct icmp6NeighborSol *)pkt->curr;
    neighborSol->zeros[0] = 0;
    neighborSol->zeros[1] = 0;
    neighborSol->zeros[2] = 0;
    neighborSol->zeros[3] = 0;

    memcpy(neighborSol->addr, target->addr, IPv6_ADDR_LEN);

    if(!isDuplicateAddrDetection){
        return icmp6Send(pkt, 135, 0, pkt->len, &(netiftab[0].ip), target);
    } else {
        dot2ipv6(UNSPECIFIED_ADDR, &src);
        return icmp6Send(pkt, 135, 0, pkt->len, &src, target);
    }
}