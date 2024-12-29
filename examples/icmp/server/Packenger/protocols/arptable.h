#include <rte_byteorder.h>
#include <rte_ip.h>
#include <rte_ether.h>

#define ARPTABLESIZE 5

struct arprecord {
    uint8_t macaddr[6];
    rte_be32_t ipaddr;
};
struct arprecord el1{
    .macaddr = {0x01,0x0c,0x0b,0x01,0x54,0x3a},
    .ipaddr = RTE_IPV4(10, 0, 0, 5)
};
struct arprecord el2{
    .macaddr = {0x01,0x0c,0x0b,0x01,0x31,0xba},
    .ipaddr = RTE_IPV4(10, 0, 0, 4) 
};
struct arprecord el3{
    .macaddr = {0x01,0x0c,0x0b,0x01,0x6e,0x0b},
    .ipaddr = RTE_IPV4(10, 0, 0, 3) 
};
struct arprecord el4{
    .macaddr = {0x01,0x0c,0x0b,0x01,0x19,0x21},
    .ipaddr = RTE_IPV4(10, 0, 0, 2)  
};
struct arprecord el5{
    .macaddr = {0x01,0x0c,0x0b,0x01,0x5a,0xb7},
    .ipaddr = RTE_IPV4(10, 0, 0, 1) 
};
struct arprecord arptable[ARPTABLESIZE]
{
el1,el2,el3,el4,el5
};

rte_ether_addr find_mac_in_arptable(rte_be32_t ipaddr){
    uint8_t i;
    for(i = 0;i < ARPTABLESIZE;i++){
        if (arptable[i].ipaddr == ipaddr){
            struct rte_ether_addr etheraddr;
            printf("%08X\n",ipaddr);
            rte_ether_addr_copy((struct rte_ether_addr *)arptable[i].macaddr, &etheraddr);
            return etheraddr;
        }
    }
}