#include <xinu.h>
#include <icmp6.h>

syscall icmp6RouterSol(void)
{
    struct packet *pkt;
    struct icmp6RouterSol *routerSol;
    struct netaddr b;

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        printf("Failed to acquire packet buffer");
        return SYSERR;
    }
    /* Setup structures */
    dot2ipv6(ALL_ROUTER_MULTICAST_ADDR, &b);

    icmp6AddLinkDestOption(pkt);

    pkt->len += ICMP6_ROUTER_SOL_LEN;
    pkt->curr -= ICMP6_ROUTER_SOL_LEN;
    routerSol = (struct icmp6RouterSol *)pkt->curr;
    routerSol->zeros[0] = 0;
    routerSol->zeros[1] = 0;
    routerSol->zeros[2] = 0;
    routerSol->zeros[3] = 0;

    return icmp6Send(pkt, 133, 0, pkt->len, &(netiftab[0].ip), &b);
}