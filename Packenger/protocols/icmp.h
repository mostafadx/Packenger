#include <rte_mbuf.h>
#include <rte_icmp.h>

class Icmp{
public:
    Icmp(struct rte_icmp_hdr *icmp_hdr){
        this->icmp_hdr = icmp_hdr;
    }
    static Icmp parse(struct rte_mbuf *packet,unsigned offset){
        return Icmp(rte_pktmbuf_mtod_offset(packet, struct rte_icmp_hdr *, offset));
    }
    static void process(Packenger* pknger, rte_mbuf *packet, unsigned offset){
        Icmp icmp = Icmp::parse(packet,offset);
        pknger->send(packet);
    }

    struct rte_icmp_hdr *icmp_hdr;
};