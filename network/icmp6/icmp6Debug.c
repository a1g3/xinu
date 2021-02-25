#include <xinu.h>
#include <icmp6.h>

void printicmp6(struct icmp6Pkt *pkt)
{
    printf("=== ICMP6 Packet ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Code: %d\r\n", pkt->code);
    printf("Checksum: 0x%04X\r\n", pkt->chksum);
}

void printicmp6RouterAdvertisement(struct icmp6RouterAdv *pkt)
{
    printf("=== ICMP6 Router Advertisement ===\r\n");
    printf("Current Hop Limit: %d\r\n", pkt->curr_hop_limit);
    printf("M/O/H and Pref: 0x%02X\r\n", pkt->moh_pref);
    printf("Router Lifetime: 0x%04X\r\n", net2hs(pkt->router_lifetime));
    printf("Reachable Time: 0x%08X\r\n", net2hl(pkt->reachable_time));
    printf("Retrans Time: 0x%08X\r\n", net2hl(pkt->retrans_time));
}

void printicmp6LinkDestOption(struct icmp6LinkDestOption *pkt)
{
    printf("=== ICMP6 Link/Dest Option ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Length: %d\r\n", pkt->len);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
                pkt->addr[0], pkt->addr[1], pkt->addr[2],
                pkt->addr[3], pkt->addr[4], pkt->addr[5]);
}

void printicmp6MtuOption(struct icmp6MtuOption *pkt)
{
    printf("=== ICMP6 MTU Option ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Length: %d\r\n", pkt->len);
    printf("MTU: %d\r\n", net2hl(pkt->mtu));
}

void printicmp6PrefixInfoOption(struct icmp6PrefixInfoOption *pkt)
{
    printf("=== ICMP6 Prefix Information Option ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Length: %d\r\n", pkt->len);
    printf("Prefix Length: %d\r\n", pkt->prefix_len);
    printf("Flags: 0x%02X\r\n", pkt->la_flags);
    printf("Valid Lifetime: %u\r\n", net2hl(pkt->valid_lifetime));
    printf("Preferred Lifetime: %u\r\n", net2hl(pkt->preferred_lifetime));
    printf("Prefix: %02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",
                pkt->prefix[0], pkt->prefix[1], pkt->prefix[2],
                pkt->prefix[3], pkt->prefix[4], pkt->prefix[5],
                pkt->prefix[6], pkt->prefix[7], pkt->prefix[8],
                pkt->prefix[9], pkt->prefix[10], pkt->prefix[11],
                pkt->prefix[12], pkt->prefix[13], pkt->prefix[14],
                pkt->prefix[15]);
}

void printicmp6RouteInfoOption(struct icmp6RouteInfoOption *pkt)
{
    printf("=== ICMP6 Route Information Option ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Length: %d\r\n", pkt->len);
    printf("Prefix Length: %d\r\n", pkt->prefix_len);
    printf("Route Prefix: 0x%02X\r\n", pkt->route_prefix);
    printf("Route Lifetime: %u\r\n", net2hl(pkt->route_lifetime));
    printf("Prefix: %02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",
                pkt->prefix[0], pkt->prefix[1], pkt->prefix[2],
                pkt->prefix[3], pkt->prefix[4], pkt->prefix[5],
                pkt->prefix[6], pkt->prefix[7], pkt->prefix[8],
                pkt->prefix[9], pkt->prefix[10], pkt->prefix[11],
                pkt->prefix[12], pkt->prefix[13], pkt->prefix[14],
                pkt->prefix[15]);
}