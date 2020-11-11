#include <xinu.h>
#include <ipv6.h>

void printIpv6Packet(struct ipv6Pkt *pkt){
    printf("Version, Class, Flow: 0x%08X\r\n", pkt->ver_class_flow);
    printf("Length: %d\r\n", pkt->len);
    printf("Next header: %d\r\n", pkt->next_header);
    printf("Hop Limit: %d\r\n", pkt->hop_limit);

    printf("Source: ");
    printf("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",
                pkt->src[0], pkt->src[1], pkt->src[2],
                pkt->src[3], pkt->src[4], pkt->src[5],
                pkt->src[6], pkt->src[7], pkt->src[8],
                pkt->src[9], pkt->src[10], pkt->src[11],
                pkt->src[12], pkt->src[13], pkt->src[14],
                pkt->src[15]);

    printf("Destination: ");
    printf("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\r\n",
                pkt->dst[0], pkt->dst[1], pkt->dst[2],
                pkt->dst[3], pkt->dst[4], pkt->dst[5],
                pkt->dst[6], pkt->dst[7], pkt->dst[8],
                pkt->dst[9], pkt->dst[10], pkt->dst[11],
                pkt->dst[12], pkt->dst[13], pkt->dst[14],
                pkt->dst[15]);
}