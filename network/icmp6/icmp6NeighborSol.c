#include <xinu.h>
#include <icmp6.h>

//https://blogs.infoblox.com/ipv6-coe/slaac-to-basics-part-1-of-2/
struct packet *icmp6NeighborSol(bool isDuplicateAddrDetection, struct netaddr *dst, struct netaddr *mac)
{
    struct packet *pkt;
    struct netaddr src;
    struct icmp6NeighborSol *neighborSol;

    if(dst != NULL && dst->type != NETADDR_IPv6){
        return NULL;
    }

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        printf("Failed to acquire packet buffer");
        return NULL;
    }

    if(!isDuplicateAddrDetection){
        icmp6AddLinkDestOption(pkt, mac);
    }

    pkt->len += ICMP6_NEIGHBOR_SOL_LEN;
    pkt->curr -= ICMP6_NEIGHBOR_SOL_LEN;
    neighborSol = (struct icmp6NeighborSol *)pkt->curr;
    neighborSol->zeros[0] = 0;
    neighborSol->zeros[1] = 0;
    neighborSol->zeros[2] = 0;
    neighborSol->zeros[3] = 0;

    memcpy(neighborSol->addr, dst->addr, IPv6_ADDR_LEN);

    if(!isDuplicateAddrDetection){
        if(SYSERR == icmp6Create(pkt, 135, 0, pkt->len, &(netiftab[0].ip), dst)){
            netFreebuf(pkt);
            return NULL;
        }
    } else {
        dot2ipv6(UNSPECIFIED_ADDR, &src);
        if(SYSERR == icmp6Create(pkt, 135, 0, pkt->len, &src, dst)){
            netFreebuf(pkt);
            return NULL;
        }
    }

    return pkt;
}