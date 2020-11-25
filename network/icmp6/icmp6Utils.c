#include <xinu.h>
#include <icmp6.h>

void icmp6AddLinkDestOption(struct packet *pkt)
{
    struct netaddr mac;
    struct icmp6LinkDestOption *opt;

    pkt->len += ICMP6_LINKDESTOPTION_LEN;
    pkt->curr -= ICMP6_LINKDESTOPTION_LEN;

    mac.type = NETADDR_ETHERNET;
    mac.len = ETH_ADDR_LEN;
    memcpy(&(mac.addr), &(netiftab[0].hwaddr.addr), netiftab[0].hwaddr.len);

    opt = (struct icmp6LinkDestOption *)pkt->curr;
    opt->type = 1;
    opt->len = 1;
    memcpy(opt->addr, &mac.addr, ETH_ADDR_LEN);
}