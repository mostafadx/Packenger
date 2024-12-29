#include <rte_arp.h>
#include <rte_mbuf.h>
#include <rte_ethdev.h>
#include "arptable.h"

class Arp {
public:
    Arp (struct rte_arp_hdr *arp_hdr, struct rte_arp_ipv4 *arp_hdr_ipv4){
        this->arp_hdr = arp_hdr;
        this->arp_hdr_ipv4 = arp_hdr_ipv4;
    }

    static Arp parse(rte_mbuf * packet,unsigned offset){
        rte_arp_hdr *arp_hdr = rte_pktmbuf_mtod_offset(packet, struct rte_arp_hdr*, offset);
        rte_arp_ipv4 *arp_hdr_ipv4 = rte_pktmbuf_mtod_offset(packet, struct rte_arp_ipv4*, offset+sizeof(struct rte_arp_hdr));
        return Arp(arp_hdr,arp_hdr_ipv4);
    }

    static void process(Packenger* pknger, rte_mbuf * packet,unsigned offset){
        Arp arp = Arp::parse(packet,offset);    
        rte_mbuf *pkt = pknger->create_raw_pkt(64);
        rte_ether_hdr ether_hdr {
            .dst_addr = arp.arp_hdr_ipv4->arp_sha,
            .src_addr = pknger->interface->mac_addr,
            .ether_type = 0x0806
        };

        pknger->setup_etherent_hdr(pkt,&ether_hdr,0);
        arp.arp_hdr->arp_opcode = 0x2;        
        arp.arp_hdr_ipv4->arp_sha = find_mac_in_arptable(arp.arp_hdr_ipv4->arp_tip); 
        arp.arp_hdr_ipv4->arp_sip = arp.arp_hdr_ipv4->arp_tip;
        arp.arp_hdr_ipv4->arp_tha = arp.arp_hdr_ipv4->arp_sha;
        arp.arp_hdr_ipv4->arp_tip = arp.arp_hdr_ipv4->arp_sip;
        pknger->setup_arp_hdr(pkt,arp.arp_hdr,arp.arp_hdr_ipv4,sizeof(struct rte_ether_hdr));
        pknger->send(pkt);
        rte_pktmbuf_free(pkt);
    }

    struct rte_arp_hdr* arp_hdr;
    struct rte_arp_ipv4 *arp_hdr_ipv4;
};