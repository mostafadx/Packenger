#include <rte_ip.h>
#include <rte_mbuf.h>
#include "icmp.h"

class Ipv4{
public:
    Ipv4(struct rte_ipv4_hdr* ip_hdr){
        this->ip_hdr = ip_hdr;
    }

    static Ipv4 parse(rte_mbuf * packet,unsigned offset){
        return Ipv4(rte_pktmbuf_mtod_offset(packet, struct rte_ipv4_hdr *, offset));
    }

    static void process(Packenger* pknger, rte_mbuf * packet,unsigned offset){
        Ipv4 ipv4 = Ipv4::parse(packet,offset);
        switch (ipv4.ip_hdr->next_proto_id)
        {
        case 0x01: // ICMP
            Icmp::process(pknger, packet,offset+sizeof(struct rte_ipv4_hdr));
            break;
        default:
            printf("Not supported protocol!\n");
            break;
        }
    }

    struct rte_ipv4_hdr* ip_hdr;

};