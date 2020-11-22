#include <xinu.h>
#include <icmp6.h>

syscall icmp6RouterSol(void)
{
    struct packet *pkt;
    struct icmp6_options *opt;
    struct netaddr mac;
    struct netaddr a;
    struct netaddr b;

    pkt = netGetbuf();
    if (SYSERR == (int)pkt)
    {
        printf("Failed to acquire packet buffer");
        return SYSERR;
    }
    /* Setup structures */
    dot2ipv6("fe80:0000:0000:0000:7632:D5A7:45B6:B6D7", &a);
    dot2ipv6(ALL_ROUTER_MULTICAST_ADDR, &b);

    pkt->len = sizeof(struct icmp6_options);
    pkt->curr -= pkt->len;

    mac.type = NETADDR_ETHERNET;
    mac.len = ETH_ADDR_LEN;
    memcpy(&(mac.addr), &(netiftab[0].hwaddr.addr), netiftab[0].hwaddr.len);

    opt = (struct icmp6_options *)pkt->curr;
    opt->zeros[0] = 0;
    opt->zeros[3] = 0;
    opt->zeros[2] = 0;
    opt->zeros[1] = 0;
    opt->type = 1;
    opt->len = 1;
    memcpy(opt->addr, &mac.addr, ETH_ADDR_LEN);

    return icmp6Send(pkt, 133, 0, pkt->len, &a, &b);
}