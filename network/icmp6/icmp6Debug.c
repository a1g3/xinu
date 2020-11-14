#include <xinu.h>
#include <icmp6.h>

void printicmp6(struct icmp6Pkt *pkt)
{
    printf("=== ICMP6 Packet ===\r\n");
    printf("Type: %d\r\n", pkt->type);
    printf("Code: %d\r\n", pkt->code);
    printf("Checksum: 0x%04X\r\n", pkt->chksum);
}