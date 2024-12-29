#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include "arp.h"
#include "ipv4.h"
#include "custom.h"

class Ethernet{
public:

    Ethernet(struct rte_ether_hdr *eth_hdr)
    {
        this->eth_hdr = eth_hdr;
    }

    static Ethernet parse(rte_mbuf * packet)
    {
      return Ethernet(rte_pktmbuf_mtod(packet, struct rte_ether_hdr *));
    }

    static void process(Packenger* pknger, rte_mbuf * packet)
    {
        Ethernet eth = Ethernet::parse(packet);
        switch (eth.eth_hdr->ether_type)
        {
        case 0x0806: // ARP
            Arp::process(pknger, packet,sizeof(struct rte_ether_hdr ));
            break;
        case 0x0800: // IPV4
            Ipv4::process(pknger, packet,sizeof(struct rte_ether_hdr ));
            break;
        case 0x0801: // Custom
            Custom::process(pknger, packet,sizeof(struct rte_ether_hdr ));
            break;
        default:
            printf("Malformed Packet!\n");
            break;
        }
    }

private:
    struct rte_ether_hdr *eth_hdr;
};