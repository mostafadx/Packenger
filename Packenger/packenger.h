#include <functional>
#include <rte_mbuf.h>
#include <rte_icmp.h>
#include <rte_arp.h>
#include <rte_ip.h>
#include "interface.h"

struct Custom_hdr{
    uint64_t A;
    uint64_t B;
    uint16_t Proto;
};

class Packenger {
public:
    Packenger(Interface *interface,rte_mempool *mbuf_pool);
    void send(struct rte_mbuf *pkt);
    void listen(std::function<void(struct rte_mbuf *, Packenger*)> packet_processor = NULL);
    void set_stop_listen(bool stop_listen);
    rte_mbuf* create_raw_pkt(unsigned pkt_len);
    void setup_etherent_hdr(rte_mbuf* pkt, struct rte_ether_hdr * ether_hdr,unsigned offset);
    void setup_custom_hdr(rte_mbuf* pkt, struct Custom_hdr *custom_hdr, unsigned offset);
    void setup_icmp_hdr(rte_mbuf* pkt, struct rte_icmp_hdr *icmp_hdr, unsigned offset);
    void setup_arp_hdr(rte_mbuf* pkt, rte_arp_hdr *arp_hdr, struct rte_arp_ipv4 *arp_hdr_ipv4, unsigned offset);
    void setup_ipv4_hdr(rte_mbuf* pkt, struct rte_ipv4_hdr *ipv4_hdr, unsigned offset);

    uint16_t port;
    uint16_t queue_id;
    bool stop_listen{};
    Interface *interface;
    rte_mempool *mbuf_pool;
    void copy_buf_to_pkt(void* buf, unsigned len, struct rte_mbuf *pkt, unsigned offset);

};