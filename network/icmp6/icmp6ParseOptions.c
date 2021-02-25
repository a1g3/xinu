#include <xinu.h>
#include <icmp6.h>

syscall icmp6ParseOptions(struct packet *pkt, struct icmp6Options *opts){
    struct etherPkt *epkt;
    struct ipv6Pkt *ipv6;
    struct icmp6Pkt *icmp;
    struct icmp6RouterAdv *routerAdv;
    struct icmp6OptionBase *icmpBase;
    int len = 0;
    int hasReceived = 0;

    if (SYSERR == isRouterAdverValid(pkt))
        return SYSERR;

    epkt = (struct etherPkt *)pkt->data;
    ipv6 = (struct ipv6Pkt *)epkt->data;
    icmp = (struct icmp6Pkt *)ipv6->data;
    routerAdv = (struct icmp6RouterAdv *)icmp->data;
    icmpBase = (struct icmp6OptionBase *)routerAdv->data;

    printicmp6(icmp);
    printicmp6RouterAdvertisement(routerAdv);
    printf("==== Options ====\r\n");
    printf("First Type: %d\r\n", icmpBase->type);
    printf("First Len: %d\r\n", icmpBase->len);
    printf("ICMP options len: %d\r\n", len);
    len = net2hs(ipv6->len) - ICMP6_HEADER_LEN;
    printf("Total options length: %d\r\n", len);

//https://www.iana.org/assignments/icmpv6-parameters/icmpv6-parameters.xhtml#icmpv6-parameters-5
    while(len > 0){
        struct icmp6LinkDestOption* optSrc;
        struct icmp6MtuOption* optMtu;
        struct icmp6PrefixInfoOption* optPrefixInfo;
        uint optLen = 0;

        printf("Looking at ICMP6 option\r\n");
        switch (icmpBase->type)
        {
        case ICMP6_OPTION_SOURCE_ADDR:
            optSrc = (struct icmp6LinkDestOption *)icmpBase;
            optLen = ICMP6_OPTION_LINK_DEST_LEN;
            memcpy(&(opts->source), optSrc, optLen);
            printicmp6LinkDestOption(&(opts->source));
            len -= optLen;
            icmpBase = (struct icmp6OptionBase *)(optSrc->next);
            break;
        case ICMP6_OPTION_TARGET_ADDR:
            optSrc = (struct icmp6LinkDestOption *)icmpBase;
            optLen = ICMP6_OPTION_LINK_DEST_LEN;
            memcpy(&(opts->target), optSrc, optLen);
            len -= optLen;
            icmpBase = (struct icmp6OptionBase *)(optSrc->next);
            break;
        case ICMP6_OPTION_PREFIX_INFO:
            optPrefixInfo = (struct icmp6PrefixInfoOption *)icmpBase;
            optLen = ICMP6_OPTION_PREFIX_LEN;
            memcpy(&(opts->prefix), optPrefixInfo, optLen);
            printicmp6PrefixInfoOption(&(opts->prefix));
            len -= optLen;
            icmpBase = (struct icmp6OptionBase *)(optPrefixInfo->next);
            break;
        case ICMP6_OPTION_MTU:
            optMtu = (struct icmp6MtuOption *)icmpBase;
            optLen = ICMP6_OPTION_MTU_LEN;
            memcpy(&(opts->mtu), optMtu, optLen);
            printicmp6MtuOption(&(opts->mtu));
            len -= optLen;
            icmpBase = (struct icmp6OptionBase *)(optMtu->next);
            break;
        case ICMP6_OPTION_ROUTE_INFO:
            optMtu = (struct icmp6RouteInfoOption *)icmpBase;
            optLen = ICMP6_ROUTE_INFO_LEN;
            memcpy(&(opts->route), optMtu, optLen);
            printicmp6RouteInfoOption(&(opts->route));
            len -= optLen;
            icmpBase = (struct icmp6OptionBase *)(optMtu->next);
            break;
        default:
            printf("Ignoring unknown icmp6 option with type %d!\r\n", icmpBase->type);
            optLen = icmpBase->len * 8;
            len -= optLen;
            icmpBase = icmpBase + optLen;
            break;
        }

        printf("Type is %d with len %d\r\n", icmpBase->type, optLen);

        if(icmpBase->len == 0)
            return SYSERR;
    }

    return OK;
}