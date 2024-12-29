#include <rte_ethdev.h>
#include "packenger.h"

Packenger::Packenger(Interface *interface, rte_mempool *mbuf_pool){
    this->interface = interface;
    this->mbuf_pool = mbuf_pool;
}

void Packenger::send(struct rte_mbuf *pkt){
    rte_eth_tx_burst(this->interface->portid, 0,
					&pkt, 1);
}

void Packenger::listen(std::function<void(struct rte_mbuf *,Packenger*)> packet_processor ) {

    struct rte_mbuf *pkt[1];
    while (!this->stop_listen){
        const uint16_t nb_rx = rte_eth_rx_burst(this->interface->portid, 0,
        pkt, 1);
            if (nb_rx > 0){
                if (packet_processor)
                    packet_processor(pkt[0],this);
            }
    }
}

void Packenger::set_stop_listen(bool stop_listen){
    this->stop_listen = stop_listen;
}

rte_mbuf *Packenger::create_raw_pkt(unsigned pkt_len)
{
    struct rte_mbuf *pkt[1];
    rte_mempool_get_bulk(this->mbuf_pool, (void **)pkt,1);
    pkt[0]->data_len = pkt_len;
    pkt[0]->pkt_len = pkt_len;
    pkt[0]->nb_segs = 1;
    return pkt[0];
}

void Packenger::setup_etherent_hdr(rte_mbuf* pkt, rte_ether_hdr *ether_hdr, unsigned offset)
{
    copy_buf_to_pkt(ether_hdr, sizeof(*ether_hdr), pkt, offset);
}

void Packenger::setup_custom_hdr(rte_mbuf* pkt, Custom_hdr *custom_hdr, unsigned offset)
{
    copy_buf_to_pkt(custom_hdr, sizeof(*custom_hdr), pkt, offset);
}

void Packenger::setup_icmp_hdr(rte_mbuf *pkt, rte_icmp_hdr *icmp_hdr, unsigned offset)
{
    copy_buf_to_pkt(icmp_hdr, sizeof(*icmp_hdr), pkt, offset);
}

void Packenger::setup_arp_hdr(rte_mbuf *pkt, rte_arp_hdr *arp_hdr, rte_arp_ipv4 *arp_hdr_ipv4, unsigned offset)
{
    copy_buf_to_pkt(arp_hdr, sizeof(*arp_hdr), pkt, offset);
    copy_buf_to_pkt(arp_hdr_ipv4, sizeof(*arp_hdr_ipv4), pkt, offset+sizeof(struct rte_arp_hdr));
}

void Packenger::setup_ipv4_hdr(rte_mbuf *pkt, rte_ipv4_hdr *ipv4_hdr, unsigned offset)
{
    copy_buf_to_pkt(ipv4_hdr, sizeof(*ipv4_hdr), pkt, offset);
}

void Packenger::copy_buf_to_pkt(void *buf, unsigned len, rte_mbuf *pkt, unsigned offset)
{
    if (offset + len <= pkt->data_len) {
		rte_memcpy(rte_pktmbuf_mtod_offset(pkt, char *, offset),
			buf, (size_t) len);
		return;
	}
}