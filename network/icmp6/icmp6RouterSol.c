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
    dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD:EF12:3456", &a);
    dot2ipv6("1234:5678:90AB:CDEF:1234:ABCD::", &b);

    pkt->len = sizeof(struct icmp6_options);
    pkt->curr -= pkt->len;

    mac.type = NETADDR_ETHERNET;
    mac.len = ETH_ADDR_LEN;
    mac.addr[0] = 0xAA;
    mac.addr[1] = 0xBB;
    mac.addr[2] = 0xCC;
    mac.addr[3] = 0xDD;
    mac.addr[4] = 0xEE;
    mac.addr[5] = 0xFF;

    opt = (struct icmp6_options *)pkt->curr;
    opt->zeros[0] = 0;
    opt->zeros[3] = 0;
    opt->zeros[2] = 0;
    opt->zeros[1] = 0;
    opt->type = 1;
    opt->len = 1;
    memcpy(opt->addr, &mac, ETH_ADDR_LEN);

    return icmp6Send(pkt, 133, 0, pkt->len, &a, &b);
}